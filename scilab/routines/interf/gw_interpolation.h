#ifdef _MSC_VER
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif

#include "../stack-c.h"

#if _MSC_VER
#include <string.h>
#endif