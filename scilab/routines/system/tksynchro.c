/* Copyright INRIA */

#include <math.h>
#include <stdio.h>

#include "../machine.h"

#ifdef WITH_TK
#include "../tksci/tksci.h"
#endif



void C2F(tksynchro)(l)
     int *l;
{
#ifdef WITH_TK
  char str[128];


  if (TK_Started) {
    int RET;

    sprintf(str,"scipad eval {set sciprompt %d}",*l);
    RET=Tcl_Eval(TKinterp,str);

  }
#endif
}
