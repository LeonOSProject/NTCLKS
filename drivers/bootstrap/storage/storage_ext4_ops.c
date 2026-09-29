/* Native ext-family inode reads and range I/O. Higher VFS operations are
 * added after journaled mutations (tasks 8-11). */
#include "storage_internal.h"

int storage_ext4_read_inode(struct storage_volume *v, uint64_t ino, struct ext4_inode_view *out)
{
    struct storage_ext4_group_view group;
    if (!v || !out || !ino || ino > v->ext4.inodes_count || !v->ext4.inodes_per_group ||
        v->ext4.inode_size < 128 || v->ext4.inode_size > EXT4_MAX_BLOCK_SIZE ||
        !v->ext4.block_size) return -RELIEFOS_EINVAL;
    int ret = storage_ext4_read_group(v, (ino - 1) / v->ext4.inodes_per_group, &group);
    if (ret < 0) return ret;
    uint64_t offset = ((ino - 1) % v->ext4.inodes_per_group) * v->ext4.inode_size;
    uint64_t block = group.inode_table + offset / v->ext4.block_size;
    uint32_t inside = offset % v->ext4.block_size, size = v->ext4.inode_size;
    uint8_t *data, *owned = NULL;
    ret = storage_ext4_cache_get(v, block, &data, false);
    if (ret < 0) return ret;
    const uint8_t *raw = data + inside;
    if (inside + size > v->ext4.block_size) {
        uint32_t first = v->ext4.block_size - inside;
        owned = kernel_malloc(size);
        if (!owned) return -RELIEFOS_ENOMEM;
        storage_memcpy(owned, raw, first);
        ret = storage_ext4_cache_get(v, block + 1, &data, false);
        if (ret < 0) { kernel_free(owned); return ret; }
        storage_memcpy(owned + first, data, size - first); raw = owned;
    }
    ret = storage_ext4_verify_inode_checksum(raw, size, (uint32_t)ino,
                                            ext4_get_le32(raw + 100), &v->ext4.super_view);
    if (!ret) ret = storage_ext4_parse_inode(raw, size, &v->ext4.super_view, out);
    if (owned) kernel_free(owned);
    if (ret < 0) { v->ext4.fs_error = 1; return -RELIEFOS_EIO; }
    return out->mode ? 0 : -RELIEFOS_ENOENT;
}

int storage_ext4_read_file_range(struct storage_volume *v, uint64_t ino,
                                const struct ext4_inode_view *in, uint64_t offset,
                                void *buffer, uint32_t len, uint32_t *out_read)
{
    uint32_t done = 0;
    int ret = 0;
    if (out_read) *out_read = 0;
    if (!v || !in || (!buffer && len) ||
        (v->ext4.block_size != 1024 && v->ext4.block_size != 2048 && v->ext4.block_size != 4096))
        return -RELIEFOS_EINVAL;
    if (!len || offset >= in->size) return 0;
    if (len > in->size - offset) len = (uint32_t)(in->size - offset);
    /* Inline fast symlinks do not contain block pointers; xattrs can account
     * for i_blocks, so the size/flags contract selects their inline payload. */
    if ((in->mode & EXT2_S_IFMT) == EXT2_S_IFLNK && !(in->flags & EXT4_EXTENTS_FL) &&
        in->size < sizeof(in->i_block_raw)) {
        storage_memcpy(buffer, in->i_block_raw + offset, len);
        if (out_read) *out_read = len;
        return 0;
    }
    const uint32_t bs = v->ext4.block_size;
    while (done < len) {
        uint64_t pos = offset + done;
        uint32_t inside = pos % bs;
        struct storage_ext4_map_result map;
        ret = storage_ext4_map_block(v, ino, in, pos / bs, false, &map);
        if (ret < 0) break;
        uint64_t available = (uint64_t)map.length * bs - inside;
        uint32_t take = available < len - done ? (uint32_t)available : len - done;
        if (map.hole || map.unwritten) {
            storage_memzero((uint8_t *)buffer + done, take);
        } else if (inside || take < bs) {
            uint8_t *data;
            if (take > bs - inside) take = bs - inside;
            ret = storage_ext4_cache_get(v, map.physical, &data, false);
            if (ret < 0) break;
            storage_memcpy((uint8_t *)buffer + done, data + inside, take);
        } else {
            uint32_t blocks = take / bs;
            if (blocks > EXT4_READAHEAD_BLOCKS) blocks = EXT4_READAHEAD_BLOCKS;
            ret = storage_ext4_cache_read_blocks(v, map.physical, blocks, (uint8_t *)buffer + done);
            if (ret < 0) break;
            take = blocks * bs;
        }
        done += take;
    }
    v->ext4.read_bytes += done;
    if (out_read) *out_read = done;
    return ret;
}
