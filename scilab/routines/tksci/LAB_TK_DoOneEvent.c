/* Copyright INRIA */
#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>
#include "tksci.h"

void LAB_TK_DoOneEvent()

{

  Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT);
}




