/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intClose.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intClose) _PARAMS((char *fname))
{
	char MyTclCommand[2000];

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
			int *param=NULL;

			GetRhsVar(1,"i",&m1,&n1,&l1);

			if ( (m1 == 1) && (n1 == 1) )
			{
				param=istk(l1);
				if (*param >= 0)
				{
					sprintf(MyTclCommand, "DestroyFigure %d;",*param); 
				}
				else
				{
					Scierror(999,"parameter must be >= 0\n");
					return 0;
				}
				
			}
			else
			{
				Scierror(999,"Invalid parameter type : must be a integer\n");
				return 0;
			}
		}
		else
		{
			Scierror(999,"Invalid parameter type : must be a integer\n");
			return 0;
		}
	}
	
	Tcl_Eval(TCLinterp,MyTclCommand);
	
	LhsVar(1) = 0;
	C2F(putlhsvar)();	

	return 0;
}
/*-----------------------------------------------------------------------------------*/
