#include "scicos_block.h"

#include "sci_mem_alloc.h"  /* malloc */

void * scicos_malloc(size_t size)
{
  return MALLOC(size);
}

