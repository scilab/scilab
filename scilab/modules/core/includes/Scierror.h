/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIERROR__
#define __SCIERROR__

#include <string.h>
#include <stdio.h>

#include "machine.h"
#include "stack-c.h"
#include "version.h"

#if defined (__STDC__) || defined (_MSC_VER)
	int  Scierror __PARAMS((int iv,char *fmt,...));
#else
	int Scierror __PARAMS(());
#endif 

#endif /* __SCIERROR__ */
/*-----------------------------------------------------------------------------------*/ 
