/* Copyright INRIA */
#include <math.h>
#include <stdio.h>

#include "tksci.h"
#include "../stack-c.h"

int LAB_TK_EvalStr()


{
  char **Str;
  int m1,n1,i,RET;
  char fname[12]="TK_EvalStr";


  CheckRhs(1,1);
  GetRhsVar(1,"S",&m1,&n1,&Str);

  for (i = 0; i<m1*n1 ;i++) {
    RET=Tcl_Eval(TKinterp,Str[i]);
    if (RET==TCL_ERROR) {
      Scierror(999,"%s, %s at line %i \r\n",fname,TKinterp->result,i+1);
      FreeRhsSVar(Str);
      return 0;
    }
  }
  FreeRhsSVar(Str);
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}

