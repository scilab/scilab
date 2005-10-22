
#include "../os_specific/sci_mem_alloc.h"  /* malloc */

#include "scicos_block.h"

void * scicos_malloc(size_t size)
{
  return MALLOC(size);
}

