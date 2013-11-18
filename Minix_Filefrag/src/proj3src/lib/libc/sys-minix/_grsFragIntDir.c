#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <unistd.h>

int do_grsFragIntDir(char *fname)
{
  message m;

  m.m1_p2 = dirname;
  m.m1_i3 = strlen(dirname);

  return( _syscall(MFS_PROC_NR, FRAG_INT_DIR, &m));
}