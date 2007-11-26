/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_close) _PARAMS((char *fname,unsigned long l))
{
	char MyTclCommand[2048];
	int DoCloseFigure=FALSE;
	int Handle=0;

	CheckLhs(1,1);
	CheckRhs(0,1);

	if (Rhs == 0)
	{
		DoCloseFigure=TRUE;
		sprintf(MyTclCommand, "DestroyFigure 0;"); 
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			static int l1,n1,m1;
		
			GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);

			if ( (m1 == 1) && (n1 == 1) )
			{
				char TestFigureExist[256];
				Handle=*istk(l1);
				
				sprintf(TestFigureExist,"Win(%d)",Handle);
				if ( Tcl_GetVar(TCLinterp, TestFigureExist, TCL_GLOBAL_ONLY) )
				{
					DoCloseFigure=TRUE;
					if (Handle >= 0)
					{
						sprintf(MyTclCommand, "DestroyFigure %d;",Handle); 
					}
					else
					{
						Scierror(999,_("parameter must be >= 0\n"));
						return 0;
					}
				}
				else
				{
					DoCloseFigure=FALSE;
					sciprint(_("Warning : Figure %d does not exist.\n"),Handle);
				}
			}
			else
			{
				Scierror(999,_("Invalid parameter type: must be an integer.\n"));
				return 0;
			}
		}
		else
		{
			Scierror(999,_("Invalid parameter type: must be an integer.\n"));
			return 0;
		}
	}
	
    if ( DoCloseFigure && ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  ) )
    {
		Scierror(999,_("Tcl Error : %s\n"),TCLinterp->result);
		return 0;
	}
	else
	{
		char VarName[64];

		sprintf(VarName,"USERDATA_%d",Handle);
		Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

		sprintf(VarName,"STRING_%d",Handle);
		Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

		if (Rhs != 0)
		{
			sprintf(VarName,"Win(%d)",Handle);
			Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);
		}
	}

	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*--------------------------------------------------------------------------*/
