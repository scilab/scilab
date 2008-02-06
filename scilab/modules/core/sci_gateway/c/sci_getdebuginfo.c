/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include "machine.h"
#include "getstaticdebuginfo.h"
#include "gw_core.h"
#include "stack-c.h"
#include "version.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/

int C2F(sci_getdebuginfo) _PARAMS((char *fname,unsigned long fname_len))
{

	static int n1=1,m1=0,n2=1,m2=0;
	CheckRhs(0,0);
	CheckLhs(0,2);
	

#ifndef _MSC_VER

	char **outputDynamicList=getDynamicDebugInfo(&m1);
	char **outputStaticList=getStaticDebugInfo(&m2);

	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, outputDynamicList);
	LhsVar(1) = Rhs+1;
	CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE, &m2, &n2, outputStaticList);
	LhsVar(2) = Rhs+2;
#else
	/* TO DO : Windows part */
	/* In my todo list ;) A.C */
	LhsVar(1) = 0;
#endif
	C2F(putlhsvar)();
return 0;
}
/*--------------------------------------------------------------------------*/
