/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_io.h"
#include "stack-c.h"
#include "getpidc.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getpid) _PARAMS((char *fname,unsigned long fname_len))
{
	int one = 1;
	int l1 = 0;
	int pid = 0;

	CheckRhs(0,0);
	CheckLhs(0,1);

	C2F(getpidc)(&pid);
	CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &one,&one, &l1);
	*istk(l1) = (int) pid;

	LhsVar(1)= Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
