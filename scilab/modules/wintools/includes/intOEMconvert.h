/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTOEMCONVERT__
#define __INTOEMCONVERT__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int C2F(sci_oemtochar) _PARAMS((char *fname));
int C2F(sci_chartooem) _PARAMS((char *fname));

#endif /* __INTOEMCONVERT__ */
/*-----------------------------------------------------------------------------------*/ 
