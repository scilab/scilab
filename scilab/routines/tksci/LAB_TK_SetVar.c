/* Copyright INRIA */
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
  
  

  MVarName = (Matrix *)Interf.Param[0];
  VarName = (char *)MatrixReadString(MVarName);

  MVarValue = (Matrix *)Interf.Param[1];
  VarValue = (char *)MatrixReadString(MVarValue);

  
  
  if ( !Tcl_SetVar(TKinterp, VarName, VarValue, 0) )
    {
      InterfError("TK_SetVar : Could set Tcl Var \n");
    }

  free(VarName);
  free(VarValue);

}

