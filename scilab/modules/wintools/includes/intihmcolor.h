/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTIHMCOLOR__
#define __INTIHMCOLOR__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"


#ifndef NULL
  #define NULL 0
#endif



int C2F(sci_settextcolor) _PARAMS((char *fname));
int C2F(sci_settextbackgroundcolor) _PARAMS((char *fname));
#endif /* __INTIHMCOLOR__ */
/*-----------------------------------------------------------------------------------*/ 
