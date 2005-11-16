/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclDeleteInterp.h"
/*-----------------------------------------------------------------------------------*/
extern int CloseTCLsci(void);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclDeleteInterp) _PARAMS((char *fname))
{
	CheckRhs(0,1);
	CheckLhs(1,1);

	if (Rhs==1)
	{
		int TypeVar1=GetType(1);

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}

		if (TypeVar1 == sci_strings)
		{
			static int l2,n2,m2;
			Tcl_Interp *TCLinterpreter=NULL;

			GetRhsVar(1,"c",&m2,&n2,&l2)
			TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
			if (TCLinterpreter==NULL)
			{
				Scierror(999,TCL_ERROR17,fname);
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
			Scierror(999,TCL_ERROR14,fname);
			return 0;
		}
	}
	else /* Rhs == 0 */
	{
		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}

		if (TK_Started)
		{
			CloseTCLsci();
		}
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
