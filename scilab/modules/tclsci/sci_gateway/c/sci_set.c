/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "TclUiSet.h" /* InterfaceScilabToUiSet */
#include "TclEvents.h" /* flushTKEvents */
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_set) _PARAMS((char *fname,unsigned long l))
{
	CheckLhs(1,1);
	CheckRhs(3,3);

	if ( IsAScalar(1) )
	{
		static int l1,n1,m1;
		int Handle=0;
		char VarName[64];

		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		Handle=*istk(l1);

		sprintf(VarName,"Win(%d)",Handle);
		if ( ! Tcl_GetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY) )
		{
			error_scilab(999,"uicontrol %d doesn't exist.",Handle);
			return 0;
		}
		
		InterfaceScilabToUiSet(Handle,Rhs-1,Rhs);
		flushTKEvents ();
	}
	else
	{
		if (GetType(Rhs-1)==sci_strings)
		{
			error_scilab(999,"Handle must be an integer.");
			return 0;
		}
		if ( IsAScalar(Rhs-2) )
		{
			error_scilab(999,"Field parameter must be a string.");
			return 0;
		}
    }

	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
