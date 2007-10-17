/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_core.h"
#include "stack-c.h"
#include "with_module.h"
#include "MALLOC.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_with_module) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	int *Status=NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;
		
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		Status=(int*)MALLOC(sizeof(int));
		*Status=with_module(VarName);

		m1=1;n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;

		C2F(putlhsvar)();

		if (Status) {FREE(Status);Status=NULL;}
	}
	else
	{
		error_scilab(999,_("Parameter must be a string."));
		return 0;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/


