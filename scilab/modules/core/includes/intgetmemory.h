/*-----------------------------------------------------------------------------------
* INRIA 2005 
* Allan CORNET 
*-----------------------------------------------------------------------------------*/
#ifndef __INTGETMEMORY__
#define  __INTGETMEMORY__

#ifdef _MSC_VER
#include <string.h>
#endif

#include "machine.h"
#include "stack-c.h"


int getfreememory(void);
int getmemorysize(void);
int C2F(intgetmemory) _PARAMS((char *fname));

#endif /*__INTGETMEMORY__*/
/*-----------------------------------------------------------------------------------*/ 
