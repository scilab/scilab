/* Copyright INRIA */
#include "../machine.h"
#include "../stack-c.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"

extern int StoreCommand ( char *command);
extern int StoreCommand1 (char *command,int flag);
extern void SetCommandflag(int flag) ;
extern void C2F(syncexec)(char * str, int *ns, int *ierr);

int TK_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int  objc,char ** argv)
{
  if (argv[2] != (char *)0 && strncmp(argv[2],"sync",4)==0) {
    int ns=strlen(argv[1]); 
    int ierr;
    C2F(syncexec)(argv[1],&ns,&ierr);
    if (ierr != 0) return TCL_ERROR;
  }
  else {
    StoreCommand(argv[1]); 
    if (argv[2] != (char *)0 && strncmp(argv[2],"seq",3)==0) {
      SetCommandflag(1);
    
    }
    Tcl_SetResult(theinterp,NULL,NULL);
  }
  return TCL_OK;
}
