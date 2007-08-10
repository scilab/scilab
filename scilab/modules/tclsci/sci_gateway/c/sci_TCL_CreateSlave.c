/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_CreateSlave) _PARAMS((char *fname,unsigned long l))
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
			error_scilab(999,"tclsci_error_12",fname);
			return 0;
		}

		GetRhsVar(1,STRING_DATATYPE,&m2,&n2,&l2);
		TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
		if (TCLinterpreter==NULL)
		{
			TCLinterpreter=Tcl_CreateSlave(TCLinterp,cstk(l2), 1);
			if (TCLinterpreter==NULL)
			{
				error_scilab(999,"tclsci_error_13",fname);
				return 0;
			}
		}
		else
		{
			error_scilab(999,"tclsci_error_14",fname,cstk(l2));
			return 0;
		}
	}
	else
	{
		error_scilab(999,"tclsci_error_15",fname);
		return 0;
	}
	
	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
