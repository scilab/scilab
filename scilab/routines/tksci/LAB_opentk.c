/* Copyright INRIA */


#include "C-LAB_Interf.h"


#include <math.h>
#include <stdio.h>
#include "TK_ScilabCallback.h"



#include "tksci.h"

#ifndef NULL
#define NULL 0
#endif

void LAB_opentk()

{

char argc;
char *argv[10];


TKinterp = Tcl_CreateInterp();
Tcl_Init(TKinterp);
Tk_Init(TKinterp);
TKmainWindow = Tk_MainWindow(TKinterp);
Tk_GeometryRequest(TKmainWindow,200,200);
Tk_SetWindowBackground(TKmainWindow, WhitePixelOfScreen(Tk_Screen(TKmainWindow)));



}



int Tcl_AppInit(interp)
     Tcl_Interp *interp;
 {


if (Tcl_Init(interp) == TCL_ERROR)
  { return TCL_ERROR; }

if (Tk_Init(interp) == TCL_ERROR)
  { return TCL_ERROR; }

return TCL_OK;

}



