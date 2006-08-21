/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIERROR__
#define __SCIERROR__

#ifdef _MSC_VER
  #include <windows.h>
#endif

#include <string.h>
#include <stdio.h>

#include "machine.h"
#include "stack-c.h"
#include "version.h"


#ifndef NULL
  #define NULL 0
#endif

#if defined (__STDC__) || defined (_MSC_VER)
	int  Scierror __PARAMS((int iv,char *fmt,...));
#else
	int Scierror __PARAMS(());
#endif 

#endif /* __SCIERROR__ */
/*-----------------------------------------------------------------------------------*/ 
