/* Copyright INRIA */
#include "../machine.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"


int TK_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int  objc,char ** argv)
{
  StoreCommand(argv[1]);  
  Tcl_SetResult(theinterp,NULL,NULL);
  return TCL_OK;
}
