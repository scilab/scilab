/* Copyright INRIA */
#include "../machine.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"

extern int StoreCommand ( char *command);
extern int StoreCommand1 (char *command,int flag);

int TK_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int  objc,char ** argv)
{
  #if WIN32
		StoreCommand1 (argv[1],2);
  #else
		StoreCommand(argv[1]);  
  #endif
  Tcl_SetResult(theinterp,NULL,NULL);
  return TCL_OK;
}
