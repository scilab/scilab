/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "sci_timer.h"
#include "timer.h"

/*-----------------------------------------------------------------------------------*/
int C2F(sci_timer) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1;
	double timeval=0;
	
	Rhs = Max(0, Rhs);
	CheckLhs(0,1);
	CheckRhs(0,0);

	C2F(timer)(&timeval);

	n1=1;
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &n1, &n1,&l1);
	*stk(l1) = (double)timeval;
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
