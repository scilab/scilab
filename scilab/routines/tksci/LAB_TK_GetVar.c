/* Copyright INRIA */
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
  

  MVarName = (Matrix *)Interf.Param[0];
  VarName = (char *)MatrixReadString(MVarName);
  
  RetStr=Tcl_GetVar(TKinterp, VarName, 0);

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

