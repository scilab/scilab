/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "setvar.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "GlobalTclInterp.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_SetVar(char *fname,unsigned long l)
{
  static int l1,n1,m1;
  static int l2,n2,m2;
  int *paramoutINT=(int*)MALLOC(sizeof(int));
  Tcl_Interp *TCLinterpreter=NULL;

  CheckRhs(2,3);
  CheckLhs(0,1);

  if (getTclInterp() == NULL)
    {
      releaseTclInterp();
      Scierror(999,_("%s: Error main TCL interpreter not initialized.\n"),fname);
      return 0;
    }
  releaseTclInterp();

  if (Rhs==3)
    {
      /* three arguments given - get a pointer on the slave interpreter */
      if (GetType(3) == sci_strings)
	{
	  GetRhsVar(3,STRING_DATATYPE,&m2,&n2,&l2);
	  TCLinterpreter=Tcl_GetSlave(getTclInterp(),cstk(l2));
	  if (TCLinterpreter==NULL)
	    {
              releaseTclInterp();
	      Scierror(999,_("%s: No such slave interpreter.\n"),fname);
	      return 0;
	    }
	}
      else
	{
	  Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 3);
	  return 0;
	}
    }
  else
    {
      /* only two arguments given - use the main interpreter */
      TCLinterpreter=getTclInterp();
    }

  if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
    {
      char *VarName=NULL;
      char **Str=NULL;

      GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
      VarName=cstk(l1);

      GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

      /* Efface valeur precedente */
      Tcl_UnsetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);

      if ( (m1==1) && (n1==1) )
	{
	  *paramoutINT=SetVarAString(TCLinterpreter,VarName,Str);
	}
      else
	{
	  *paramoutINT=SetVarStrings(TCLinterpreter,VarName,Str,m1,n1);
	}
      freeArrayOfString(Str,m1*n1);
    }
  else
    if ( (GetType(1) == sci_strings) && (GetType(2) == sci_matrix) )
      {
#define COMPLEX 1
	char *VarName=NULL;
	int *header=NULL;
	int Cmplx;

	header = (int *) GetData(2);   
	Cmplx=header[3];
	if (Cmplx==COMPLEX)
	  {
	    releaseTclInterp();
	    Scierror(999,_("This function doesn't work with Complex.\n"));
	    return 0;
	  }

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	VarName=cstk(l1);

	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

	if ( (m1==0) && (n1==0) )
		{
			releaseTclInterp();
			Scierror(999,_("[] doesn't work with Tcl/Tk.\n"));
			return 0;
		}
	
	if ( (m1==1) && (n1==1) )
		{
			*paramoutINT=SetVarScalar(TCLinterpreter,VarName,(double)*stk(l1));
		}
	else
		{
			*paramoutINT=SetVarMatrix(TCLinterpreter,VarName,l1,m1,n1);
		}
      }
    else
		{
			if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
			if (GetType(1) != sci_strings) {
				Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname ,1);
			} 
			if (GetType(2) != sci_matrix) {
				Scierror(999,_("%s: Wrong type for input argument #%d: Matrix expected.\n"),fname ,2);
			}
	releaseTclInterp();
	return 0;
      }

  m1=1;
  n1=1;
  l1=0;
  CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE,  &m1, &n1, &l1);
  *istk(l1)=*paramoutINT;

  if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
  releaseTclInterp();

  LhsVar(1)=Rhs+1;
  PutLhsVar();

  return 0;
}
/*--------------------------------------------------------------------------*/
