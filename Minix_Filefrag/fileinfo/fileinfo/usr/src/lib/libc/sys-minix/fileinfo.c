#include <stdio.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <lib.h>
#include <minix/callnr.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/fileinfo.h>
#include <sys/stat.h>

#define SUCCESS 0

int fileinfo(const char *name)
{
    printf("Filename : %s\n", name);
    message m;

    struct stat filestat;
    if (stat(name, &filestat) == -1)
    {
        if (errno == ENOENT) printf("File not found\n");
        else printf("Unable to access the file");

        return 2;
    }

    printf("Inode : %llu\n", filestat.st_ino);
    m.m1_i2 = filestat.st_ino;

    char *pids = (char *) malloc(1024);
    memset(pids, 0, 1024);
    m.m1_p2 = pids;

    _syscall(VFS_PROC_NR, FILEINFO, &m);

    printf("PID : %s\n", pids);

    blockinfo(name);

    return SUCCESS;
}

int blockinfo(const char *name)
{
    printf("blockinfo\n");
    message m;

    m.m1_i1 = strlen(name);
    m.m1_p1 = name;

    _syscall(VFS_PROC_NR, BLOCKINFO, &m);

    return SUCCESS;
}
