/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclEvalStr.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclEvalStr) _PARAMS((char *fname))
{ 
	CheckRhs(1,2);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char **Str=NULL;
		int m1,n1,i,RET;
		int m2,n2,l2;
    Tcl_Interp *TCLinterpreter=NULL;

		GetRhsVar(1,"S",&m1,&n1,&Str);

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
				 Scierror(999,TCL_ERROR14,fname);
				 return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			TCLinterpreter=TCLinterp;
		}

		for (i = 0; i<m1*n1 ;i++)
		{
			RET=Tcl_Eval(TCLinterpreter,Str[i]);

			if (RET==TCL_ERROR)
			{
				if(C2F(iop).err>0)
				{
                    Scierror(999,TCL_ERROR10,fname,i+1);
				}
				else
				{
					Scierror(999,TCL_ERROR11,fname,TCLinterpreter->result,i+1);
				}
				FreeRhsSVar(Str);
				return 0;
			}
		}
		FreeRhsSVar(Str);
		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,TCL_ERROR12);
		return 0;
	}
	
  	return 0;
}
/*-----------------------------------------------------------------------------------*/
