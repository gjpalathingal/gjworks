#include <sys/cdefs.h>
#include "namespace.h"
#include <lib.h>
#include <unistd.h>

int do_grsFragInt()
{
  message m;

  return( _syscall(MFS_PROC_NR, FRAG_INT, &m));
}