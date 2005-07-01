#include "../sci_mem_alloc.h" /* MALLOC */
#include "scicos_block.h"

void * scicos_malloc(size_t size)
{
  return MALLOC(size);
}

