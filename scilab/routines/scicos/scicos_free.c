#include "scicos_block.h"

#define NULL 0
#define FREE(x) if (x  != NULL) free((char *) x);

void scicos_free(void *p)
{
  FREE(p);
}
