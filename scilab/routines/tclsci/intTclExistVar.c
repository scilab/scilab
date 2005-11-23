/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclExistVar.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclExistVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	static int l2,n2,m2;

	Tcl_Interp *TCLinterpreter=NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);
	
	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}
		
		if (Rhs==2)
		{
			/* two arguments given - get a pointer on the slave interpreter */
			if (GetType(2) == sci_strings)
			{
				GetRhsVar(2,"c",&m2,&n2,&l2)
				TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
				if (TCLinterpreter==NULL)
				{
					Scierror(999,TCL_ERROR17,fname);
					return 0;
				}
			}
			else
			{
				 Scierror(999,TCL_ERROR14);
				 return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			TCLinterpreter=TCLinterp;
		}

		n1=1;
		if ( Tcl_GetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY) )
		{
			CreateVar(Rhs+1, "b", &n1,&n1,&l1);
			*istk(l1)=(int)(TRUE);
		}
		else
		{
			CreateVar(Rhs+1, "b", &n1,&n1,&l1);
			*istk(l1)=(int)(FALSE);
		}
	
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		 Scierror(999,TCL_ERROR14,fname);
		 return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
