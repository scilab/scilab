/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "message_scilab.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
extern BOOL FileExist(char *filename); /* From module fileio */
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_EvalFile) _PARAMS((char *fname,unsigned long l))
{
  /* execute Tcl scripts */
  int m1,n1,l1;
  int m2,n2,l2;
  int RET;
  Tcl_Interp *TCLinterpreter=NULL;

  CheckRhs(1,2);
  CheckLhs(1,1);

  if (GetType(1) == sci_strings)
  {
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

	if (TCLinterp == NULL)
	{
		Scierror(999,_("%s : Error main TCL interpreter not initialized.\n"),fname);
		return 0;
	}

	if(!FileExist(cstk(l1)))
	{
		Scierror(999,_("file %s not found.\n"),cstk(l1));
		return 0;
	}

	if (Rhs==2)
	{
		/* two arguments given - get a pointer on the slave interpreter */
		if (GetType(2) == sci_strings)
		{
			GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
			TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
			if (TCLinterpreter==NULL)
			{
				Scierror(999,_("%s: No such slave interpreter.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s : Argument type must be character string.\n"),fname);
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
		const char *trace = Tcl_GetVar(TCLinterpreter, "errorInfo", TCL_GLOBAL_ONLY);
		if (C2F(iop).err>0) 
		{
			message_scilab(_("%s, at line %i of file %s."),fname,TCLinterpreter->errorLine,cstk(l1),(char *)trace);
		}
        else
		{
			Scierror(999,_("%s, at line %i of file %s\n"),fname,TCLinterpreter->errorLine,cstk(l1),TCLinterpreter->result,(char *)trace);
			return 0;
		}
    }
  }
  else
  {
	  Scierror(999,_("%s : Argument type must be character string.\n"),fname);
	  return 0;
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
/*-----------------------------------------------------------------------------------*/
