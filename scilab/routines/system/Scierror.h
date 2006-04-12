/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIERROR__
#define __SCIERROR__

#ifdef WIN32
  #include <windows.h>
#endif

#include <string.h>
#include <stdio.h>

#include "../machine.h"
#include "../stack-c.h"
#include "../version.h"


#ifndef NULL
  #define NULL 0
#endif

#define TRUE  1
#define FALSE 0

#if defined (__STDC__) || defined (WIN32)
	int  Scierror __PARAMS((int iv,char *fmt,...));
#else
	int Scierror __PARAMS(());
#endif 

#endif /* __SCIERROR__ */
/*-----------------------------------------------------------------------------------*/ 
