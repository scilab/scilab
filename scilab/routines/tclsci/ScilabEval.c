/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "TCL_Global.h"
#include "ScilabEval.h"
/*-----------------------------------------------------------------------------------*/
/* what's the max number of commands in the queue ??*/
#define arbitrary_max_queued_callbacks 20
/*-----------------------------------------------------------------------------------*/
static int c_n1 = -1;     
/*-----------------------------------------------------------------------------------*/
extern void SetCommandflag(int flag) ;
extern void C2F(syncexec)(char * str, int *ns, int *ierr, int *seq);
extern int GetCommand(char *str);
extern int StoreCommand ( char *command); 
extern int C2F(tksynchro)();
extern integer C2F (ismenu)(void); 
/*-----------------------------------------------------------------------------------*/
int TCL_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,CONST char ** argv)
{
  int ns,ierr,seq;
  char *command;

  char *comm[arbitrary_max_queued_callbacks];
  int   seqf[arbitrary_max_queued_callbacks];
  int nc,ncomm=-1;

  if (C2F(iop).ddt==-1)
  {
	/* trace for debugging */
    int argc=1;
    sciprint("TCL_EvalScilabCmd %s",argv[1]);
    while (argv[++argc]) sciprint(" %s",argv[argc]);
    sciprint("\n");
  }

  if (argv[1] != (char *)0)
  {
    if (strlen(argv[1])>=bsiz)
	{
      command = (char *) malloc (bsiz * sizeof (char));
      if (command == (char *) 0)
      {
        sciprint ("TCL_EvalScilabCmd: No more memory\r\n");
        return TCL_ERROR;
      }
      memset(command,'\0',bsiz);
      strncpy(command,argv[1],bsiz-1);
      sciprint("Warning: ScilabTCLEval command is too long and has been truncated to %d characters!\r\n",bsiz-1);
    }
	else
	{
      command = (char *) malloc ((strlen (argv[1]) + 1) * sizeof (char));
      if (command == (char *) 0)
      {
        sciprint ("TCL_EvalScilabCmd: No more memory\r\n");
        return TCL_ERROR;
      }
      strcpy(command,argv[1]);
    }

    if ( (argv[2] != (char *)0) && (strncmp(argv[2],"sync",4)==0) )
	{
      /* sync or sync seq */
      C2F(tksynchro)(&c_n1);  /* set sciprompt to -1 (scilab busy) */
      seq= ( (argv[3] != (char *)0) && (strncmp(argv[3],"seq",3)==0) );
      ns=strlen(command); 
      if (C2F(iop).ddt==-1) sciprint(" Execution starts for %s\r\n",command);
      C2F(syncexec)(command,&ns,&ierr,&seq);
      if (C2F(iop).ddt==-1) sciprint(" Execution ends for %s\r\n",command);
      C2F(tksynchro)(&C2F(recu).paus);
      if (ierr != 0) return TCL_ERROR;
    }
    else if (strncmp(command,"flush",5)==0)
	{
      /* flush */
      if (C2F(iop).ddt==-1) sciprint(" Flushing starts for queued commands\r\n");
      while (C2F(ismenu)() && ncomm<arbitrary_max_queued_callbacks-1)
	  {
        ncomm++;
        comm[ncomm] = (char *) malloc (bsiz+1);
        if (comm[ncomm] == (char *) 0)
        {
          sciprint ("TCL_EvalScilabCmd: No more memory\r\n");
          return TCL_ERROR;
        }
        seqf[ncomm]=GetCommand (comm[ncomm]);
      }
      if (C2F(ismenu)()) sciprint("Warning: Too many callbacks in queue!\r\n");
      for (nc = 0 ; nc <= ncomm ; nc++ )
	  {
        C2F(tksynchro)(&c_n1);  /* set sciprompt to -1 (scilab busy) */
        if (C2F(iop).ddt==-1)
        {
	      if (seqf[nc]==0)
		  {
			  sciprint(" Flushed execution starts for %s - No option\r\n",comm[nc]);
		  }
	      else
		  {
			  sciprint(" Flushed execution starts for %s - seq\r\n",comm[nc]);
		  }
        }
        ns=strlen(comm[nc]);
        C2F(syncexec)(comm[nc],&ns,&ierr,&(seqf[nc]));
        if (C2F(iop).ddt==-1)
        {
          sciprint(" Flushed execution ends for %s\r\n",comm[nc]);
          free(comm[nc]);
        }
        C2F(tksynchro)(&C2F(recu).paus);
        if (ierr != 0) return TCL_ERROR;
      }
      if (C2F(iop).ddt==-1) sciprint(" Flushing ends\r\n");
    }
    else
	{
      /* seq or no option */
      StoreCommand(command); 
      if ( (argv[2] != (char *)0) && (strncmp(argv[2],"seq",3)==0) )
	  {
        SetCommandflag(1);
      }
      else
	  {
		/* unknown option */
        Tcl_SetResult(theinterp,NULL,NULL);
	  }
    }
    free(command);

  } 
  else
  {
	/* ScilabEval called without argument */
    Scierror(999,"ScilabTCLEval: at least one argument is required\r\n");
  }

  return TCL_OK;
}
