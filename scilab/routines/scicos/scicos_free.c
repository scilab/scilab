#include "scicos_block.h"

#define FREE(x) if (x  != NULL) free((char *) x);

void scicos_free(void *p)
{
  FREE(p);
}
