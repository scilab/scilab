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
			sciprint("%s, at line %i of file %s\r\n","ScilabEval",TCLinterp->errorLine,cstk(l1));
		}
        else
		{
			Scierror(999,"%s, at line %i of file %s: %s\r\n",fname,TCLinterp->errorLine,cstk(l1),TCLinterp->result);
			return 0;
		}
    }
  }
  else
  {
	  Scierror(999,"argument type incorrect: Must be a string");
	  return 0;
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
