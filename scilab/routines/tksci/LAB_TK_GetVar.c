/* Copyright INRIA */
/* Modifications to fix bug #851 by Francois VOGEL, August 2004 */

#include "C-LAB_Interf.h"

#include <math.h>
#include <stdio.h>

#include "tksci.h"

void LAB_TK_GetVar()

{
  Matrix *MVarName;
  char *VarName;
  
  Matrix *MRetStr;
  char *RetStr;

  if (Interf.NbParamIn !=  1)
  {
	InterfError("TK_GetVar : Wrong number of input arguments \n");
	return;
  }

  if (Interf.NbParamOut != 1)
  {
	InterfError("TK_GetVar : Wrong number of output arguments \n");
	return;
  }

  MVarName = (Matrix *)Interf.Param[0];

  if (MatrixIsString(MVarName) != 1)
  {
    InterfError("TK_GetVar : Argument type must be character string \n");
	return;
  }

  VarName = (char *)MatrixReadString(MVarName);

  RetStr= (char*)Tcl_GetVar(TKinterp, VarName, 0);

  if ( RetStr )
	{
	  MRetStr = MatrixCreateString(RetStr);
	  ReturnParam(MRetStr);
	}
  else
	{
	  InterfError("TK_GetVar : Could not read Tcl Var \n");
	}

  free(VarName);

}

