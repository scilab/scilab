/* Copyright INRIA */
/* UI generator main routine */
/* Bertrand Guiheneuf, INRIA 1997 */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "TK_uicontrol.h"



void LAB_gcf()


{

 
  char MyCommand[2000];
  char *StrHandle;
  int Handle;

  Matrix *MOutputHandle;
  double *OutputHandle;



  sprintf(MyCommand, "set MyTmpBertrand [GetGcf];"); 
     


  Tcl_Eval(TKinterp,MyCommand);
  StrHandle = Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
  Handle = (int)atoi(StrHandle);

  MOutputHandle = MatrixCreate(1,1,"real");
  OutputHandle = (double *)MatrixGetPr(MOutputHandle);
  *OutputHandle = Handle;

  ReturnParam(MOutputHandle);



}














