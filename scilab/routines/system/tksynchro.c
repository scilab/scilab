/* Copyright INRIA */

#include <math.h>
#include <stdio.h>

#include "../machine.h"
#include "../tksci/tksci.h"


void C2F(tksynchro)(l)
     int *l;
{
#ifdef WITH_TK
  char str[128];
  int RET;
  if (TK_Started) {
#ifdef WIN32 
    sprintf(str,"scipad eval {set sciprompt %d}",*l);
    RET=Tcl_Eval(TKinterp,str);
#else
    if (!Tcl_SetVar(TKinterp, "sciprompt", str, 0)) {
      InterfError("TK_SetVar : Could set Tcl Var \n");
    }
#endif
  }
#endif
}
