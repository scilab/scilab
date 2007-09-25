/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "TclEvents.h"
#include "TclUiGet.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_get) _PARAMS((char *fname,unsigned long l))
{
	CheckLhs(1,1);
	CheckRhs(2,2);

	if ( IsAScalar(Rhs-1) && (GetType(Rhs)==sci_strings) )
	{
		static int l1,n1,m1;
		int Handle=0;
		char VarName[64];

		GetRhsVar(Rhs-1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		Handle=*istk(l1);

		sprintf(VarName,"Win(%d)",Handle);
		if ( ! Tcl_GetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY) )
		{
			error_scilab(999,"uicontrol %d doesn't exist.",Handle);
			return 0;
		}

		TCL_UiGet(Handle,Rhs);
		flushTKEvents ();
	}
	else
	{
		error_scilab(999,"Invalid parameter(s) type.");
		return 0;
	}

	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
