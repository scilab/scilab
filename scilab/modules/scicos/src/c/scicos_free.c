#include "scicos_block.h"
#include "MALLOC.h"

void scicos_free(void *p)
{
  FREE(p);
}
