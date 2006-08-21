/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTPATHNAME__
#define __INTPATHNAME__

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


#define MAX_PATH_LONG 32767
#define MAX_PATH_SHORT 260

int C2F(sci_getlongpathname) _PARAMS((char *fname));
int C2F(sci_getshortpathname) _PARAMS((char *fname));
#endif /* __INTPATHNAME__ */
/*-----------------------------------------------------------------------------------*/ 
