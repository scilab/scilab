#include "scicos_block.h"
#include <math.h>
#include <stdlib.h>
#include "../machine.h"

void scicos_free(void *p)
{
  return free(p);
}
