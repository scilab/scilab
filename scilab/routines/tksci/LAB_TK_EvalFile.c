/* Copyright INRIA */
#include "C-LAB_Interf.h"
#include "../stack-c.h"

#include <math.h>
#include <stdio.h>
#include "tksci.h"

static int LAB_TK_EvalFile1(void);

void LAB_TK_EvalFile(void) { 
  LAB_TK_EvalFile1();
}


static int LAB_TK_EvalFile1(void)
{
  int m1,n1,l1;
  int RET;
  char fname[12]="TK_EvalFile";
 
  CheckRhs(1,1);
  CheckLhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  RET=Tcl_EvalFile(TKinterp,cstk(l1));

  if (RET==TCL_ERROR) {
    Scierror(999,"%s, at line %i of file %s: %s\r\n",fname,
	     TKinterp->errorLine,cstk(l1),TKinterp->result);
    return 0;
  }
  LhsVar(1)=0;
  C2F(putlhsvar)();
  return 0;
}

