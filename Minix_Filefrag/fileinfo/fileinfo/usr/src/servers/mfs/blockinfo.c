#include "fs.h"
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/ioc_memory.h>
#include <sys/svrctl.h>
#include <sys/select.h>
#include <minix/callnr.h>
#include <minix/com.h>
#include <minix/keymap.h>
#include <minix/const.h>
#include <minix/endpoint.h>
#include <minix/safecopies.h>
#include <minix/debug.h>
#include <minix/vfsif.h>
#include "inode.h"
#include "fs.h"
#include <assert.h>
#include <minix/vfsif.h>
#include <minix/bdev.h>
#include "inode.h"
#include "clean.h"

#define TRUE    1
#define FALSE   0

#define DEBUG   FALSE

#define SUCCESS 0
#define FAILURE -1

PUBLIC int fs_blockinfo(void)
{
    printf("System Call : fs_blockinfo called\n");
    ino_t inode_nr = fs_m_in.m1_i2;
    dev_t dev_nr = fs_m_in.m1_i3;
    struct super_block *sp;
    unsigned scale;
    unsigned blk_in_zone;
    unsigned zone_size;
    off_t off;

    struct inode *ip = find_inode(dev_nr, inode_nr);

    sp = ip->i_sp;
    scale = sp->s_log_zone_size;
    blk_in_zone = 1 << scale;
    zone_size = sp->s_block_size << scale;
    block_t blk;
    block_t expected = 0;
    int nfrags = 0;

    printf("Blocks ");
    for (off = 0; off < ip->i_size; off += zone_size)
    {
        if ((blk = read_map(ip, off)) != expected)
        {
            expected = blk;
            nfrags++;
        }

        expected += blk_in_zone;
        printf("%d ", blk);
    }
    printf("\n");

    // printf("nfrags %d\n", nfrags);
    block_t dst_blk = (ip->i_size + sp->s_block_size - 1) / sp->s_block_size;
    int n = dst_blk >> scale + (dst_blk % blk_in_zone > 0);
    printf("Block Count : %d\n", dst_blk);

    return OK;
}
