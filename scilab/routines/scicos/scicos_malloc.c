#include "scicos_block.h"
#include <math.h>
#include <stdlib.h>
#include "../machine.h"

void * scicos_malloc(size_t size)
{
  return malloc(size);
}

