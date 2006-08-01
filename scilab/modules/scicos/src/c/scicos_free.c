#include "scicos_block.h"


void scicos_free(void *p)
{
  FREE(p);
}
