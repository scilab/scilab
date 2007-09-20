#include "scicos_block.h"
#include "../os_specific/sci_mem_alloc.h"

#ifndef NULL
	#define NULL 0
#endif
#define SCICOS_FREE(x) if (x  != NULL) FREE((char *) x);

void scicos_free(void *p)
{
  SCICOS_FREE(p);
}
