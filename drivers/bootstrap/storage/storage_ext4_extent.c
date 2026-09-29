/* Bounded, non-recursive extent/indirect lookup. Callers serialize a volume
 * through the storage execution lock; no pointer survives a cache lookup.
 * Linux fs/ext4/extents.c and ext4_extents.h define the disk format. */
#include "storage_internal.h"

static int ext4_map_bad(struct storage_volume *volume)
{
    volume->ext4.fs_error = 1;
    return -RELIEFOS_EIO;
}

static bool ext4_map_block_valid(const struct storage_volume *v, uint64_t block, uint32_t n)
{
    return n && block > v->ext4.first_data_block && block < v->ext4.blocks_count &&
           n <= v->ext4.blocks_count - block;
}

static uint32_t ext4_map_run(uint64_t n)
{ return n > UINT32_MAX ? UINT32_MAX : (uint32_t)n; }

static int ext4_extent_lookup(struct storage_volume *v, uint64_t ino,
                              const struct ext4_inode_view *in, uint64_t logical,
                              struct storage_ext4_map_result *out)
{
    const uint8_t *node = in->i_block_raw;
    uint64_t visited[EXT4_MAX_EXTENT_DEPTH] = {0};
    uint64_t lower = 0, upper = (uint64_t)EXT4_MAX_LOGICAL_BLOCK + 1;
    uint32_t length = 60, expected_depth = ext4_get_le16(node + 6), level = 0;
    if (expected_depth > EXT4_MAX_EXTENT_DEPTH) return ext4_map_bad(v);
    for (;;) {
        struct storage_ext4_extent_header_view h;
        if (storage_ext4_parse_extent_header(node, length, &h) < 0 ||
            !h.max_entries || h.max_entries > (length - 12) / 12 ||
            h.depth != expected_depth || (!h.entries && h.depth))
            return ext4_map_bad(v);
        if (level && (v->ext4.super_view.feature_ro_compat & EXT4_FEATURE_RO_COMPAT_METADATA_CSUM)) {
            uint32_t tail = 12 + 12 * h.max_entries;
            uint8_t word[4];
            if (tail + 4 > length) return ext4_map_bad(v);
            uint32_t crc = storage_ext4_super_csum_seed(&v->ext4.super_view);
            ext4_put_le32(word, (uint32_t)ino); crc = storage_ext4_crc32c(crc, word, 4);
            ext4_put_le32(word, in->generation); crc = storage_ext4_crc32c(crc, word, 4);
            if (storage_ext4_crc32c(crc, node, tail) != ext4_get_le32(node + tail))
                return ext4_map_bad(v);
        }
        uint64_t previous_end = lower;
        for (uint32_t i = 0; i < h.entries; ++i) {
            const uint8_t *e = node + 12 + i * 12;
            uint64_t start = ext4_get_le32(e), end, physical;
            uint32_t n;
            if (h.depth) {
                n = 1; end = start + 1;
                physical = ext4_get_le32(e + 4) | ((uint64_t)ext4_get_le16(e + 8) << 32);
            } else {
                n = ext4_get_le16(e + 4);
                if (n > 32768) n -= 32768;
                end = start + n;
                physical = ext4_get_le32(e + 8) | ((uint64_t)ext4_get_le16(e + 6) << 32);
            }
            if ((level && !i && start != lower) || start < previous_end || end > upper ||
                !ext4_map_block_valid(v, physical, n)) return ext4_map_bad(v);
            for (uint32_t j = 0; j < level; ++j)
                if (visited[j] >= physical && visited[j] - physical < n)
                    return ext4_map_bad(v);
            previous_end = end;
        }
        /* Binary upper_bound: select the final key <= logical. */
        uint32_t lo = 0, hi = h.entries;
        while (lo < hi) {
            uint32_t mid = lo + (hi - lo) / 2;
            if (ext4_get_le32(node + 12 + mid * 12) <= logical) lo = mid + 1;
            else hi = mid;
        }
        if (!h.depth) {
            if (lo) {
                const uint8_t *e = node + 12 + (lo - 1) * 12;
                uint64_t start = ext4_get_le32(e);
                uint32_t raw_len = ext4_get_le16(e + 4);
                uint32_t n = raw_len > 32768 ? raw_len - 32768 : raw_len;
                if (logical - start < n) {
                    out->physical = (ext4_get_le32(e + 8) |
                                     ((uint64_t)ext4_get_le16(e + 6) << 32)) + logical - start;
                    out->length = n - (uint32_t)(logical - start);
                    out->unwritten = raw_len > 32768;
                    return 0;
                }
            }
            if (lo < h.entries) upper = ext4_get_le32(node + 12 + lo * 12);
            out->hole = true; out->length = ext4_map_run(upper - logical);
            return out->length ? 0 : ext4_map_bad(v);
        }
        if (!lo) {
            out->hole = true;
            out->length = ext4_map_run(ext4_get_le32(node + 12) - logical);
            return out->length ? 0 : ext4_map_bad(v);
        }
        const uint8_t *selected = node + 12 + (lo - 1) * 12;
        lower = ext4_get_le32(selected);
        if (lo < h.entries) upper = ext4_get_le32(selected + 12);
        uint64_t child = ext4_get_le32(selected + 4) |
                         ((uint64_t)ext4_get_le16(selected + 8) << 32);
        uint8_t *data;
        visited[level++] = child;
        int ret = storage_ext4_cache_get(v, child, &data, false);
        if (ret < 0) return ret;
        node = data; length = v->ext4.block_size; expected_depth--;
    }
}

static int ext4_indirect_lookup(struct storage_volume *v, const struct ext4_inode_view *in,
                                uint64_t logical, struct storage_ext4_map_result *out)
{
    uint64_t n = v->ext4.block_size / 4, offset = logical, span = 1;
    uint64_t path[3] = {0}; uint32_t levels = 0;
    const uint8_t *entries = in->i_block_raw;
    uint32_t index = (uint32_t)logical, count = 12;
    if (logical >= 12) {
        offset -= 12;
        for (levels = 1; levels <= 3; ++levels) {
            span *= n;
            if (offset < span) break;
            offset -= span;
        }
        if (levels > 3) return -RELIEFOS_EFBIG;
        uint64_t ptr = ext4_get_le32(in->i_block_raw + (11 + levels) * 4);
        for (uint32_t depth = 0; depth < levels; ++depth) {
            if (!ptr) {
                out->hole = true; out->length = ext4_map_run(span - offset);
                return 0;
            }
            if (!ext4_map_block_valid(v, ptr, 1)) return ext4_map_bad(v);
            for (uint32_t i = 0; i < depth; ++i)
                if (path[i] == ptr) return ext4_map_bad(v);
            path[depth] = ptr;
            uint8_t *data;
            int ret = storage_ext4_cache_get(v, ptr, &data, false);
            if (ret < 0) return ret;
            span /= n; index = (uint32_t)(offset / span); offset %= span;
            entries = data; count = (uint32_t)n;
            ptr = ext4_get_le32(entries + index * 4);
        }
    }
    uint64_t physical = ext4_get_le32(entries + index * 4);
    if (physical && !ext4_map_block_valid(v, physical, 1)) return ext4_map_bad(v);
    for (uint32_t i = 0; i < levels; ++i)
        if (physical && physical == path[i]) return ext4_map_bad(v);
    out->physical = physical; out->hole = !physical; out->length = 1;
    for (uint32_t i = index + 1; i < count; ++i) {
        uint64_t next = ext4_get_le32(entries + i * 4);
        if (physical ? next != physical + out->length : next != 0) break;
        if (next && !ext4_map_block_valid(v, next, 1)) return ext4_map_bad(v);
        for (uint32_t j = 0; j < levels; ++j)
            if (next && next == path[j]) return ext4_map_bad(v);
        ++out->length;
    }
    return 0;
}

int storage_ext4_map_block(struct storage_volume *v, uint64_t ino,
                           const struct ext4_inode_view *in, uint64_t logical,
                           bool allocate, struct storage_ext4_map_result *out)
{
    if (!v || !in || !out || !ino || ino > v->ext4.inodes_count ||
        (v->ext4.block_size != 1024 && v->ext4.block_size != 2048 && v->ext4.block_size != 4096))
        return -RELIEFOS_EINVAL;
    *out = (struct storage_ext4_map_result){0};
    if (allocate) return -RELIEFOS_EOPNOTSUPP; /* Writes require task 8 journal handles. */
    if (logical > EXT4_MAX_LOGICAL_BLOCK) return -RELIEFOS_EFBIG;
    if (in->flags & EXT4_EXTENTS_FL) {
        if (!(v->ext4.super_view.feature_incompat & EXT4_FEATURE_INCOMPAT_EXTENTS))
            return ext4_map_bad(v);
        return ext4_extent_lookup(v, ino, in, logical, out);
    }
    return ext4_indirect_lookup(v, in, logical, out);
}

int storage_ext4_insert_extent(struct storage_volume *v, uint64_t ino, uint64_t logical,
                               uint64_t physical, uint32_t len, bool unwritten)
{
    (void)v; (void)ino; (void)logical; (void)physical; (void)len; (void)unwritten;
    return -RELIEFOS_EOPNOTSUPP;
}
int storage_ext4_remove_range(struct storage_volume *v, uint64_t ino, uint64_t start, uint64_t end)
{
    (void)v; (void)ino; (void)start; (void)end;
    return -RELIEFOS_EOPNOTSUPP;
}

int storage_ext4_fiemap(struct storage_volume *v, uint64_t ino, uint64_t start, uint64_t len,
                        struct storage_ext4_fiemap_extent *out, uint32_t capacity, uint32_t *count)
{
    struct ext4_inode_view in;
    if (!v || !count || (capacity && !out)) return -RELIEFOS_EINVAL;
    *count = 0;
    int ret = storage_ext4_read_inode(v, ino, &in);
    if (ret < 0) return ret;
    if (!len || start >= in.size) return 0;
    uint64_t end = len > in.size - start ? in.size : start + len;
    uint64_t pos = start, bs = v->ext4.block_size;
    while (pos < end) {
        struct storage_ext4_map_result map;
        ret = storage_ext4_map_block(v, ino, &in, pos / bs, false, &map);
        if (ret < 0) return ret;
        uint64_t bytes = (uint64_t)map.length * bs - pos % bs;
        if (bytes > end - pos) bytes = end - pos;
        if (!map.hole) {
            if (capacity && *count == capacity) return 0;
            if (out && capacity) out[*count] = (struct storage_ext4_fiemap_extent){
                .logical = pos, .physical = map.physical * bs + pos % bs,
                .length = bytes, .flags = map.unwritten ? EXT4_FIEMAP_UNWRITTEN : 0};
            if (*count == UINT32_MAX) return -RELIEFOS_EOVERFLOW;
            ++*count;
        }
        pos += bytes;
    }
    if (out && capacity && *count && end == in.size) out[*count - 1].flags |= EXT4_FIEMAP_LAST;
    return 0;
}
