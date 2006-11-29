/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "../../src/c/fromjava.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intfromjava) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
int C2F(sci_fromjava) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfromjava)(fname);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intfromjava) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	Rhs = Max(0, Rhs);
	CheckRhs(0,0);
	CheckLhs(1,1);

	if ( IsFromJava() )
	{
		*paramoutINT=(int)(TRUE);
	}
	else
	{
		*paramoutINT=(int)(FALSE);
	}

	n1=1;
	CreateVarFromPtr(1, "b", &n1, &n1, &paramoutINT);
	LhsVar(1)=1;
	C2F(putlhsvar)();
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
