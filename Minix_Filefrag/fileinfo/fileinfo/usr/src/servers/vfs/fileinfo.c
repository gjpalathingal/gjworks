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
#include "file.h"
#include "dmap.h"
#include "fproc.h"
#include "scratchpad.h"
#include "vmnt.h"
#include "vnode.h"
#include "job.h"
#include "param.h"
#include "path.h"

#define TRUE    1
#define FALSE   0

#define DEBUG   FALSE

#define SUCCESS 0
#define FAILURE -1

PUBLIC int do_fileinfo(void)
{
    ino_t file_inode_nr;

    file_inode_nr = m_in.m1_i2;

    if (DEBUG) printf("inode %lu\n", file_inode_nr);

    int filp_count, fproc_count, fp_filp_count;
    for (filp_count = 0; filp_count < NR_FILPS; ++filp_count)
    {
        if (DEBUG && (filp[filp_count].filp_vno->v_inode_nr < 10000))
            printf("inode filp %lu\n", filp[filp_count].filp_vno->v_inode_nr);

        if (filp[filp_count].filp_vno->v_inode_nr == file_inode_nr)
        {
            char pids[1024] = {0};
            struct filp *fp = &filp[filp_count];
            if (DEBUG) printf("inode matching\n");
            for (fproc_count = 0; fproc_count < NR_PROCS; ++fproc_count)
            {
                if (DEBUG) printf("%d ", fproc[fproc_count].fp_pid);

                for (fp_filp_count = 0; fp_filp_count < OPEN_MAX; ++fp_filp_count)
                {
                    if (fp == fproc[fproc_count].fp_filp[fp_filp_count])
                    {
                        if (DEBUG) printf("double inode matching\n");
                        sprintf(pids, "%s %d", pids, fproc[fproc_count].fp_pid);
                        break;
                    }
                }
            }

            sys_datacopy(VFS_PROC_NR, (vir_bytes) pids, m_in.m_source, (vir_bytes) m_in.m1_p2, (phys_bytes) strlen(pids));

            break;
        }
    }

    return SUCCESS;
}

PUBLIC int do_blockinfo()
{
    int r;
    struct vnode *vp;

    char *filename = malloc(PATH_MAX);
    memset(filename, 0, PATH_MAX);

    sys_datacopy(who_e, (vir_bytes) m_in.m1_p1, VFS_PROC_NR, (vir_bytes) filename, (phys_bytes) m_in.m1_i1);
    if (DEBUG) printf("filename %s\n", filename);

    struct vmnt *vmp;
    struct lookup resolve;

    lookup_init(&resolve, filename, PATH_NOFLAGS, &vmp, &vp);
    resolve.l_vmnt_lock = VMNT_READ;
    resolve.l_vnode_lock = VNODE_READ;

    vp = eat_path(&resolve, fp);
    if (vp == NULL) return (err_code);

    r = req_blockinfo(vp->v_fs_e, vp->v_inode_nr, vp->v_dev);

    put_vnode(vp);
    return(r);
}
