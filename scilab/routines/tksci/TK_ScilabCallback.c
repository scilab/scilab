/* Copyright INRIA */
#include "../machine.h"
#include "../stack-c.h"

#include "TK_ScilabCallback.h"
#include "tksci.h"

extern int StoreCommand ( char *command);
extern void SetCommandflag(int flag) ;
extern void C2F(syncexec)(char * str, int *ns, int *ierr, int *seq);
extern int GetCommand(char *str); 
extern integer C2F (ismenu)(void); 

int TK_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,char ** argv)
{
  int ns,ierr,seq;
#define arbitrary_max_queued_callbacks 20     /* what's the max number of commands in the queue ??*/
  char *comm[arbitrary_max_queued_callbacks];
  int   seqf[arbitrary_max_queued_callbacks];
  int nc,ncomm=-1;
  if (C2F(iop).ddt==-1) {/* trace for debugging */
    int argc=1;
    sciprint("TK_EvalScilabCmd %s",argv[1]);
    while (argv[++argc]) sciprint(" %s",argv[argc]);
    sciprint("\n");
  }

  if (argv[1] != (char *)0) {
    if (argv[2] != (char *)0 && strncmp(argv[2],"sync",4)==0) {
      seq=(argv[3] != (char *)0) && (strncmp(argv[3],"seq",3)==0);
      ns=strlen(argv[1]); 
      if (C2F(iop).ddt==-1) {
        sciprint(" Execution starts for %s\r\n",argv[1]);
      }
      C2F(syncexec)(argv[1],&ns,&ierr,&seq);
      if (C2F(iop).ddt==-1) {
        sciprint(" Execution ends for %s\r\n",argv[1]);
      }
      if (ierr != 0) return TCL_ERROR;
    }
    else if (strncmp(argv[1],"flush",5)==0) {
      if (C2F(iop).ddt==-1) {
        sciprint(" Flushing starts for queued commands\r\n");
      }
      while (C2F(ismenu)() && ncomm<arbitrary_max_queued_callbacks-1) {
        ncomm++;
        comm[ncomm] = (char *) malloc (bsiz+1);
			  if (comm[ncomm] == (char *) 0)
				{
					sciprint ("TK_EvalScilabCmd: No more memory\r\n");
					return TCL_ERROR;
				}
        seqf[ncomm]=GetCommand (comm[ncomm]);
      }
      if (C2F(ismenu)()) {
          sciprint("Warning: Too many callbacks in queue!\r\n");
      }
      for (nc = 0 ; nc <= ncomm ; nc++ ) {
        if (C2F(iop).ddt==-1) {
		      if (seqf[nc]==0) { sciprint(" Flushed execution starts for %s - No option\r\n",comm[nc]); }
		      else             { sciprint(" Flushed execution starts for %s - seq\r\n",comm[nc]); }
        }
        ns=strlen(comm[nc]);
        C2F(syncexec)(comm[nc],&ns,&ierr,&(seqf[nc]));
        if (C2F(iop).ddt==-1) {
          sciprint(" Flushed execution ends for %s\r\n",comm[nc]);
          free(comm[nc]);
        }
        if (ierr != 0) return TCL_ERROR;
      }
      if (C2F(iop).ddt==-1) {
        sciprint(" Flushing ends\r\n");
      }
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

