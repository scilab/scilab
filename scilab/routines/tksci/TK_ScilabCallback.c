/* Copyright INRIA */
#include "../machine.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"

extern int StoreCommand ( char *command);
extern int StoreCommand1 (char *command,int flag);
extern void SetCommandflag(int flag) ;
int TK_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int  objc,char ** argv)
{
  StoreCommand(argv[1]); 
  if (argv[2] != (char *)0 && strncmp(argv[2],"seq",3)==0) {
    SetCommandflag(1);
  }
  Tcl_SetResult(theinterp,NULL,NULL);
  return TCL_OK;
}
