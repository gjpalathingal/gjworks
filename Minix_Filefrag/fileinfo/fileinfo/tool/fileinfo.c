#include <stdio.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/fileinfo.h>
#include <stdlib.h>
#include <sys/cdefs.h>
#include <lib.h>
#include <minix/callnr.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>

#define TRUE  1
#define FALSE 0

#define DEBUG TRUE

void usage(char *binary_path)
{
    printf("usage: %s <filename/path>\n", basename(binary_path));
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        usage(argv[0]);
        return 1;
    }

    fileinfo(argv[1]);
    // blockinfo(argv[1]);

    return 0;
}
