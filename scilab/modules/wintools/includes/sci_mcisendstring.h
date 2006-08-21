/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __INTMCISENDSTRING__
#define __INTMCISENDSTRING__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "version.h"

int C2F(sci_mcisendstring) _PARAMS((char *fname));
#if _MSC_VER
	int mcisendstringWindowsfunction _PARAMS((char *fname));
#else
	int mcisendstringUnixfunction _PARAMS((char *fname));
#endif

#endif /* __INTMCISENDSTRING__ */
/*-----------------------------------------------------------------------------------*/ 
