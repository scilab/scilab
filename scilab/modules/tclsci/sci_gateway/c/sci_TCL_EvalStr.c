/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "TCL_Command.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_TCL_EvalStr) _PARAMS((char *fname,unsigned long l))
{
  CheckRhs(1,2);
  CheckLhs(1,1);

  if (GetType(1) == sci_strings)
    {
      char *tclSlave = NULL;

      char **Str=NULL;
      int m1,n1,i,j;
      int m2,n2,l2;
      char **ReturnArrayString=NULL;
      int k=0;

      GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

      if (TCLinterp == NULL)
	{
	  freeArrayOfString(Str,m1*n1);
	  Scierror(999,_("%s: Error main TCL interpreter not initialized.\n"),fname);
	  return 0;
	}

      if (Rhs==2)
	{
	  /* two arguments given - the slave interpreter name */
	  if (GetType(2) == sci_strings)
	    {
	      GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
	      if (Tcl_GetSlave(TCLinterp,cstk(l2)) == NULL)
		{
		  freeArrayOfString(Str,m1*n1);
		  Scierror(999,_("%s: No such slave interpreter.\n"),fname);
		  return 0;
		}
	      tclSlave =  strdup(cstk(l2));
	    }
	  else
	    {
	      freeArrayOfString(Str,m1*n1);
	      Scierror(999,_("%s: Wrong input argument: String expected.\n"),fname);
	      return 0;
	    }
	}

      ReturnArrayString = (char **) MALLOC(m1*n1*sizeof(char **));

      for (i = 0; i<m1*n1 ;i++)
	{

	  if (tclSlave != NULL) {
	    sendTclCommandToSlave(Str[i], tclSlave);
	  }
	  else {
	    sendTclCommand(Str[i]);
	  }

	  if (getTclCommandReturn() == TCL_ERROR)
	    {
	      const char *trace = Tcl_GetVar(TCLinterp, "errorInfo", TCL_GLOBAL_ONLY);
	      freeArrayOfString(Str,m1*n1);
	      if(Err>0)
		{
		  Scierror(999,"%s, ScilabEval error at line %i\n	%s.\n",fname,i+1,(char *)trace);
		}
	      else
		{
		  Scierror(999,"%s, %s at line %i\n	%s\n",fname,TCLinterp->result,i+1,(char *)trace);
		}
	      return 0;
            }
	  else
	    {
	      /* return result of the successful evaluation of the script */
	      /* return a matrix of string results */
	      ReturnArrayString[k++]=getTclCommandResult();
            }
	}
      CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, ReturnArrayString);
      LhsVar(1)=Rhs+1;
      C2F(putlhsvar)();

      for (i=0;i<n1;i++) for (j=0;j<m1;j++) { FREE(ReturnArrayString[i+n1*j]);ReturnArrayString[i+n1*j]=NULL; }
      FREE(ReturnArrayString);
      freeArrayOfString(Str,m1*n1);
    }
  else
    {
      Scierror(999,_("%s: Wrong input argument: String or vector of strings expected.\n"),fname);
      return 0;
    }

  return 0;
}
/*--------------------------------------------------------------------------*/
