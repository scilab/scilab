/* Allan CORNET INRIA 2005 */
#ifndef __GETEXCEPTIONCODE__
#define __GETEXCEPTIONCODE__

/* Only for Windows */

#include <Windows.h>
#include "win_mem_alloc.h" /* MALLOC */

char *GetExceptionString(DWORD ExceptionCode);

#endif /* __GETEXCEPTIONCODE__ */
