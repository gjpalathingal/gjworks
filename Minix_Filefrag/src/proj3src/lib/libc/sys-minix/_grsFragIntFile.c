#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <unistd.h>

int do_grsFragIntFile(char *fname)
{
 message m;

  m.m1_p2 = fname;
  m.m1_i3 = strlen(fname);

  return( _syscall(MFS_PROC_NR, FRAG_INT_FILE, &m));
}