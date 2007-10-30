/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "InitTclTk.h"
#include "Scierror.h"
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
			Scierror(999,_("%s : Error main TCL interpreter not initialized.\n"),fname);
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
				Scierror(999,_("%s: No such slave interpreter.\n"),fname);
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
			Scierror(999,_("%s : Argument type must be character string.\n"),fname);
			return 0;
		}
	}
	else /* Rhs == 0 */
	{
		if (TCLinterp == NULL)
		{
			Scierror(999,_("%s : Error main TCL interpreter not initialized.\n"),fname);
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
