/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclEvalFile.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclEvalFile) _PARAMS((char *fname))
{
  FILE* tmpFile; 
  /* execute Tcl scripts */
  int m1,n1,l1;
  int m2,n2,l2;
  int RET;
  Tcl_Interp *TCLinterpreter=NULL;

  CheckRhs(1,2);
  CheckLhs(1,1);

  if (GetType(1) == sci_strings)
  {
	GetRhsVar(1,"c",&m1,&n1,&l1);

	if (TCLinterp == NULL)
	{
		Scierror(999,TCL_ERROR13,fname);
		return 0;
	}

	if( (tmpFile=fopen(cstk(l1),"r")) == 0 ) 
	{ 
		Scierror(999,TCL_ERROR26,cstk(l1));
		return 0;
	} 
	else 
	{ 
		fclose(tmpFile); 
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

	RET=Tcl_EvalFile(TCLinterpreter,cstk(l1));

	if (RET==TCL_ERROR)
	{
		if (C2F(iop).err>0) 
		{
			sciprint(TCL_MSG1,fname,TCLinterpreter->errorLine,cstk(l1));
		}
        else
		{
			Scierror(999,TCL_MSG1,fname,TCLinterpreter->errorLine,cstk(l1),TCLinterpreter->result);
			return 0;
		}
    }
  }
  else
  {
	  Scierror(999,TCL_ERROR14,fname);
	  return 0;
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
