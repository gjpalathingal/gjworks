#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <unistd.h>

int do_grsFragExt()
{
  message m;

  return( _syscall(MFS_PROC_NR, FRAG_EXT, &m));
}