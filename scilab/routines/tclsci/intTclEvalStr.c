/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclEvalStr.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclEvalStr) _PARAMS((char *fname))
{ 
	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char **Str=NULL;
		int m1,n1,i,RET;

		GetRhsVar(1,"S",&m1,&n1,&Str);
		for (i = 0; i<m1*n1 ;i++)
		{
			RET=Tcl_Eval(TCLinterp,Str[i]);
			if (RET==TCL_ERROR)
			{
				if(C2F(iop).err>0)
				{
                    Scierror(999,"%s, ScilabTclEval error at line %i \r\n",fname,i+1);
				}
				else
				{
					Scierror(999,"%s, %s at line %i \r\n",fname,TCLinterp->result,i+1);
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
		Scierror(999,"argument type incorrect: Must be a string or vector of strings");
		return 0;
	}
	
  	return 0;
}
/*-----------------------------------------------------------------------------------*/
