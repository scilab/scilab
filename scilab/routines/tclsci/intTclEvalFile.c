/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclEvalFile.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclEvalFile) _PARAMS((char *fname))
{
  /* execute Tcl scripts */
  int m1,n1,l1;
  int RET;
   
  CheckRhs(1,1);
  CheckLhs(1,1);

  if (GetType(1) == sci_strings)
  {
	GetRhsVar(1,"c",&m1,&n1,&l1);
	RET=Tcl_EvalFile(TCLinterp,cstk(l1));

	if (RET==TCL_ERROR)
	{
		if (C2F(iop).err>0) 
		{
			sciprint(TCL_MSG1,"ScilabEval",TCLinterp->errorLine,cstk(l1));
		}
        else
		{
			Scierror(999,TCL_MSG1,fname,TCLinterp->errorLine,cstk(l1),TCLinterp->result);
			return 0;
		}
    }
  }
  else
  {
	  Scierror(999,TCL_ERROR9);
	  return 0;
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
