/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCIERROR__
#define __SCIERROR__

#include <stdarg.h>
#include "PARAMS.h"

#if defined (__STDC__) || defined (_MSC_VER)
	int  Scierror __PARAMS((int iv,char *fmt,...));
#else
	int Scierror __PARAMS(());
#endif 

#endif /* __SCIERROR__ */
/*-----------------------------------------------------------------------------------*/ 
