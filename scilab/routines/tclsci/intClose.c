/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intClose.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intClose) _PARAMS((char *fname))
{
	char MyTclCommand[2048];
	int DoCloseFigure=FALSE;

	CheckLhs(1,1);
	CheckRhs(0,1);

	if (Rhs == 0)
	{
		sprintf(MyTclCommand, "DestroyFigure 0;"); 
	}
	else
	{
		if ( GetType(1) == sci_matrix )
		{
			static int l1,n1,m1;
			int param=0;

			GetRhsVar(1,"i",&m1,&n1,&l1);

			if ( (m1 == 1) && (n1 == 1) )
			{
				char TestFigureExist[256];
				param=*istk(l1);
				
				sprintf(TestFigureExist,"Win(%d)",param);
				if ( Tcl_GetVar(TCLinterp, TestFigureExist, TCL_GLOBAL_ONLY) )
				{
					DoCloseFigure=TRUE;
					if (param >= 0)
					{
						sprintf(MyTclCommand, "DestroyFigure %d;",param); 
					}
					else
					{
						Scierror(999,TCL_ERROR5);
						return 0;
					}
				}
				else
				{
					DoCloseFigure=FALSE;
					sciprint(TCL_WARNING3,param);
				}
			}
			else
			{
				Scierror(999,TCL_ERROR6);
				return 0;
			}
		}
		else
		{
			Scierror(999,TCL_ERROR7);
			return 0;
		}
	}
	
    if ( DoCloseFigure && ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  ) )
    {
		Scierror(999,TCL_ERROR4,TCLinterp->result);
		return 0;
	}

	
	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
