/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "TCL_Command.h"
#include "GlobalTclInterp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_TCL_EvalStr(char *fname,unsigned long l)
{

  CheckRhs(1,2);
  CheckLhs(1,1);

  if (GetType(1) == sci_strings)
    {
      char *tclSlave = NULL;

      char **Str=NULL;
      int m1,n1,i;
      int m2,n2,l2;
      char **ReturnArrayString=NULL;
      int k=0;
      int tclInterpReturnValue;

      GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

      if (!existsGlobalInterp())
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
	      if (!existsSlaveInterp(cstk(l2)))
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
	      Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
	      return 0;
	    }
	}

      ReturnArrayString = (char **) MALLOC(m1*n1*sizeof(char **));

      for (i = 0; i<m1*n1 ;i++)
	{

	  if (tclSlave != NULL) {
	    tclInterpReturnValue = sendTclCommandToSlave(Str[i], tclSlave);
	  }
	  else {
	    tclInterpReturnValue = sendTclCommand(Str[i]);
	  }

	  if (tclInterpReturnValue == TCL_ERROR)
	    {
	      const char *trace = NULL;

              /* Read the error trace in the slave or in the main interpreter */
              if (tclSlave != NULL)
                {
                  trace = Tcl_GetVar(Tcl_GetSlave(getTclInterp(),tclSlave), "errorInfo", TCL_GLOBAL_ONLY);
                }
              else
                {
                  trace = Tcl_GetVar(getTclInterp(), "errorInfo", TCL_GLOBAL_ONLY);
                }
	      releaseTclInterp();
	      freeArrayOfString(Str,m1*n1);

              /* Display the error message */
              if(Err>0) /* Scilab error */
		{
		  /* Note: there a ScilabEval here in the error message because it should only
		     trigger for commands such as  TCL_EvalStr("ScilabEval scilab==nsp sync")
		     i.e. when the string passed to TCL_EvalStr will run something in the
		     Scilab parser (thus ScilabEval), and this "something" triggers a Scilab
		     error                                                                   */
		  Scierror(999,"%s, ScilabEval error at line %i\n	%s.\n",fname,i+1,(char *)trace);
		}
	      else /* TCL error */
		{
                  const char *result = NULL;

                  if (tclSlave != NULL) /* In the slave */
                    {
                      result = Tcl_GetStringResult(Tcl_GetSlave(getTclInterp(),tclSlave));
                    }
                  else /* In the main interpreter */
                    {
                      result = Tcl_GetStringResult(getTclInterp());
                    }
                  Scierror(999,"%s, %s at line %i\n	%s\n", fname, (char *)result, i+1, (char *)trace);
		  releaseTclInterp();
		}
	      return 0;
            }
	  else
	    {
	      /* return result of the successful evaluation of the script */
	      /* return a matrix of string results */
	      ReturnArrayString[k++] = getTclCommandResult();
            }
	}
      CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, ReturnArrayString);
      LhsVar(1)=Rhs+1;
      C2F(putlhsvar)();

	  freeArrayOfString(ReturnArrayString,m1*n1);
      freeArrayOfString(Str,m1*n1);
    }
  else
    {
      Scierror(999,_("%s: Wrong type for input argument #%d: String or vector of strings expected.\n"), fname, 1);
      return 0;
    }
  return 0;
}
/*--------------------------------------------------------------------------*/
