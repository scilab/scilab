/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_getlanguage.h"
#include "setgetlanguage.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_getlanguagealias) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Output=getlanguagealias();

	if (Output)
	{
		n1=1;
		CreateVarFromPtr( Rhs+1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();	
		if (Output) {FREE(Output);Output=NULL;}
	}
	else
	{
		m1=0;
		n1=0;
		l1=0;
		CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		return 0;
	}

	
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
