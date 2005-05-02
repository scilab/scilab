/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTMCISENDSTRING__
#define __INTMCISENDSTRING__

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

int C2F(intmcisendstring) _PARAMS((char *fname));
#if WIN32
	int mcisendstringWindowsfunction _PARAMS((char *fname));
#else
	int mcisendstringUnixfunction _PARAMS((char *fname));
#endif

#endif /* __INTMCISENDSTRING__ */
/*-----------------------------------------------------------------------------------*/ 
