/* Copyright INRIA */
/* UI generator main routine */
/* Bertrand Guiheneuf, INRIA 1997 */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "TK_uicontrol.h"



int LAB_gcf()


{

 
  char MyCommand[2000];
  char *StrHandle;







  sprintf(MyCommand, "set MyTmpBertrand [GetGcf];"); 
     


  Tcl_Eval(TKinterp,MyCommand);
  StrHandle = (char *) Tcl_GetVar(TKinterp, "MyTmpBertrand", 0);
  return (int)atoi(StrHandle);
}














