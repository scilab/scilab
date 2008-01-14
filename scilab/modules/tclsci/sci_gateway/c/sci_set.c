/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "TclUiSet.h" /* InterfaceScilabToUiSet */
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
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
			Scierror(999,_("uicontrol %d doesn't exist.\n"),Handle);
			return 0;
		}

		InterfaceScilabToUiSet(Handle,Rhs-1,Rhs);
	}
	else
	{
		if (GetType(Rhs-1)==sci_strings)
		{
			Scierror(999,_("Handle must be an integer.\n"));
			return 0;
		}
		if ( IsAScalar(Rhs-2) )
		{
			Scierror(999,_("Field parameter must be a string.\n"));
			return 0;
		}
    }

	LhsVar(1) = 0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
