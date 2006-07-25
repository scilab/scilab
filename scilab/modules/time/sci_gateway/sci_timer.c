/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "sci_timer.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern int C2F(timer)(double *etime);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_timer) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	double timeval=0;
	double *paramout=NULL;
	
	Rhs = Max(0, Rhs);
	CheckLhs(0,1);
	CheckRhs(0,0);
	C2F(timer)(&timeval);
	
	paramout=(double*)MALLOC(sizeof(int));
	*paramout=timeval;
	n1=1;
	CreateVarFromPtr(1, "d", &n1, &n1, &paramout);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	if (paramout) {FREE(paramout);paramout=NULL;}


	return 0;
}
/*-----------------------------------------------------------------------------------*/
