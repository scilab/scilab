/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "sci_getlanguage.h"
#include "setgetlanguage.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
int C2F(sci_getlanguage) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Output=getlanguage();

	n1=1;
	CreateVarFromPtr( 1, "c",(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = 1;
	C2F(putlhsvar)();	
	if (Output) {FREE(Output);Output=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
