/* Copyright INRIA */
#include "../machine.h"
#include "../stack-c.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"
extern int C2F(tksynchro)();

extern int StoreCommand ( char *command);
extern void SetCommandflag(int flag) ;
extern void C2F(syncexec)(char * str, int *ns, int *ierr, int *seq);

int TK_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int  objc,char ** argv)
{

  if (C2F(iop).ddt==-1) {/* trace for debugging */
    int argc=1;
    sciprint("TK_EvalScilabCmd %s",argv[1]);
    while (argv[++argc]) sciprint(" %s",argv[argc]);
    sciprint("\n");
  }

  if (argv[1] != (char *)0) {
    if (argv[2] != (char *)0 && strncmp(argv[2],"sync",4)==0) {
      int ns=strlen(argv[1]); 
      int seq=(argv[3] != (char *)0) && (strncmp(argv[3],"seq",3)==0);
      int ierr,l=0;
	  if (C2F(iop).ddt==-1) {
		sciprint("   Execution starts for %s\r\n",argv[1]);
	  }
      C2F(syncexec)(argv[1],&ns,&ierr,&seq);
      if (C2F(iop).ddt==-1) {
        sciprint("   Execution ends for %s\r\n",argv[1]);
	  }
      if (ierr != 0) return TCL_ERROR;
	}
    else {
      StoreCommand(argv[1]); 
      if (argv[2] != (char *)0 && strncmp(argv[2],"seq",3)==0) {
        SetCommandflag(1);
	  }
      else
      Tcl_SetResult(theinterp,NULL,NULL);
	}
  } else {
	  Scierror(999,"ScilabEval: at least one argument is required\r\n");
  }
  return TCL_OK;
}
