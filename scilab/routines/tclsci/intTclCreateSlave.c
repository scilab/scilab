/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclCreateSlave.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclCreateSlave) _PARAMS((char *fname))
{
	int TypeVar1=0;
	CheckRhs(1,1);
	CheckLhs(1,1);
	
	TypeVar1=GetType(1);

	if (TypeVar1 == sci_strings)
	{
		static int l2,n2,m2;
		Tcl_Interp *TCLinterpreter=NULL;

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}

		GetRhsVar(1,"c",&m2,&n2,&l2)
		TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
		if (TCLinterpreter==NULL)
		{
			TCLinterpreter=Tcl_CreateSlave(TCLinterp,cstk(l2), 1);
			if (TCLinterpreter==NULL)
			{
				Scierror(999,TCL_ERROR19,fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,TCL_ERROR25,fname,cstk(l2));
			return 0;
		}
	}
	else
	{
		Scierror(999,TCL_ERROR14,fname);
		return 0;
	}
	
	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
