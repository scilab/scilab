/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclExistInterp.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclExistInterp) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int TypeVar1=0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	TypeVar1=GetType(1);

	if (TypeVar1 == sci_strings)
	{
		Tcl_Interp *TCLinterpreter=NULL;

		char *InterpName=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		InterpName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}

		TCLinterpreter=Tcl_GetSlave(TCLinterp,InterpName);

		n1=1;
		if (TCLinterpreter)
		{
			CreateVar(Rhs+1, "b", &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1, "b", &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		 Scierror(999,TCL_ERROR14,fname);
		 return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
