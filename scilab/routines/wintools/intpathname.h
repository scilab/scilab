/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTPATHNAME__
#define __INTPATHNAME__

#ifdef WIN32
  #include <windows.h>
#endif

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

#define MAX_PATH_LONG 32767
#define MAX_PATH_SHORT 260

int C2F(intgetlongpathname) _PARAMS((char *fname));
int C2F(intgetshortpathname) _PARAMS((char *fname));
#endif /* __INTPATHNAME__ */
/*-----------------------------------------------------------------------------------*/ 
