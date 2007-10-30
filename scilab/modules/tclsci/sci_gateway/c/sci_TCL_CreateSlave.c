/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
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
			Scierror(999,_("%s : Error main TCL interpreter not initialized.\n"),fname);
			return 0;
		}

		GetRhsVar(1,STRING_DATATYPE,&m2,&n2,&l2);
		TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
		if (TCLinterpreter==NULL)
		{
			TCLinterpreter=Tcl_CreateSlave(TCLinterp,cstk(l2), 1);
			if (TCLinterpreter==NULL)
			{
				Scierror(999,_("%s: Could not create a slave interpreter.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: %s interpreter already exist.\n"),fname,cstk(l2));
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s : Argument type must be character string.\n"),fname);
		return 0;
	}
	
	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
