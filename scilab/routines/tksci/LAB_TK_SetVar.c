/* Copyright INRIA */
/* Modifications to fix bug #851 by Francois VOGEL, August 2004 */

#include "C-LAB_Interf.h"

#include <math.h>
#include <stdio.h>

#include "tksci.h"

void LAB_TK_SetVar()

{
  Matrix *MVarName;
  char *VarName;

  Matrix *MVarValue;
  char *VarValue;

  if (Interf.NbParamIn !=  2)
  {
	InterfError("TK_SetVar : Wrong number of input arguments \n");
	return;
  }

  if (Interf.NbParamOut != 1)
  {
	InterfError("TK_SetVar : Wrong number of output arguments \n");
	return;
  }

  MVarName = (Matrix *)Interf.Param[0];
  MVarValue = (Matrix *)Interf.Param[1];

  if ((MatrixIsString(MVarValue) != 1) || (MatrixIsString(MVarName) != 1))
  {
    InterfError("TK_SetVar : Argument type must be character string \n");
	return;
  }

  VarName  = (char *)MatrixReadString(MVarName);
  VarValue = (char *)MatrixReadString(MVarValue);

  if ( !Tcl_SetVar(TKinterp, VarName, VarValue, 0) )
	{
	  InterfError("TK_SetVar : Could not set Tcl Var \n");
	}

  free(VarValue);
  free(VarName);
}

