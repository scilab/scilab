/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_TCL_ExistInterp) _PARAMS((char *fname,unsigned long l))
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

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		InterpName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,_("%s : Error main TCL interpreter not initialized.\n"),fname);
			return 0;
		}

		TCLinterpreter=Tcl_GetSlave(TCLinterp,InterpName);

		n1=1;
		if (TCLinterpreter)
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s : Argument type must be character string.\n"),fname);
		return 0;
	}
	
	return 0;
}
/*--------------------------------------------------------------------------*/
