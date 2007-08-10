/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "InitTclTk.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_DeleteInterp) _PARAMS((char *fname,unsigned long l))
{
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (Rhs==1)
	{
		int TypeVar1=GetType(1);

		if (TCLinterp == NULL)
		{
			error_scilab(999,"tclsci_error_12",fname);
			return 0;
		}

		if (TypeVar1 == sci_strings)
		{
			static int l2,n2,m2;
			Tcl_Interp *TCLinterpreter=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m2,&n2,&l2);
			TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
			if (TCLinterpreter==NULL)
			{
				error_scilab(999,"tclsci_error_16",fname);
				return 0;
			}
			else
			{
				Tcl_DeleteInterp(TCLinterpreter);
				TCLinterpreter=NULL;
			}
		}
		else
		{
			error_scilab(999,"tclsci_error_15",fname);
			return 0;
		}
	}
	else /* Rhs == 0 */
	{
		if (TCLinterp == NULL)
		{
			error_scilab(999,"tclsci_error_12",fname);
			return 0;
		}
		CloseTCLsci();
		initTCLTK();
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
