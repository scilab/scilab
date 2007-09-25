/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "message_scilab.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
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
						error_scilab(999,"parameter must be >= 0");
						return 0;
					}
				}
				else
				{
					DoCloseFigure=FALSE;
					message_scilab("Warning : Figure %d does not exist.",Handle);
				}
			}
			else
			{
				error_scilab(999,"Invalid parameter type: must be an integer.");
				return 0;
			}
		}
		else
		{
			error_scilab(999,"Invalid parameter type: must be an integer.");
			return 0;
		}
	}
	
    if ( DoCloseFigure && ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  ) )
    {
		error_scilab(999,"Tcl Error : %s",TCLinterp->result);
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
/*-----------------------------------------------------------------------------------*/
