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
	char *UTF8Arg=NULL;

	UTF8Arg=MALLOC(sizeof(char)*(strlen(argv[1])+2));
	/* UTF to ANSI */
	Tcl_UtfToExternal(theinterp, NULL, argv[1], strlen(argv[1]), 0, NULL, UTF8Arg, (int)(strlen(argv[1])+1), NULL, NULL,NULL);
	
    sciprint(TCL_MSG7,UTF8Arg);
    while (argv[++argc]) sciprint(" %s",argv[argc]);
    sciprint("\n");
	if (UTF8Arg){FREE(UTF8Arg);UTF8Arg=NULL;}
  }

  if (argv[1] != (char *)0)
  {
	  char *UTF8Arg=NULL;
	  UTF8Arg=MALLOC(sizeof(char)*(strlen(argv[1])+2));

	  /* UTF to ANSI */
	  Tcl_UtfToExternal(theinterp, NULL, argv[1], strlen(argv[1]), 0, NULL, UTF8Arg, (int)(strlen(argv[1])+1), NULL, NULL,NULL);

    if (strlen(UTF8Arg)>=bsiz)
	{
      command = (char *) MALLOC (bsiz * sizeof (char));
      if (command == (char *) 0)
      {
        sciprint (TCL_ERROR28);
        return TCL_ERROR;
      }
      memset(command,'\0',bsiz);
      strncpy(command,UTF8Arg,bsiz-1);
      sciprint(TCL_WARNING4,bsiz-1);
    }
	else
	{
      command = (char *) MALLOC ((strlen (UTF8Arg) + 1) * sizeof (char));
      if (command == (char *) 0)
      {
        sciprint (TCL_ERROR28);
        return TCL_ERROR;
      }
      strcpy(command,UTF8Arg);
    }
	if (UTF8Arg){FREE(UTF8Arg);UTF8Arg=NULL;}

    if ( (argv[2] != (char *)0) && (strncmp(argv[2],"sync",4)==0) )
	{
      /* sync or sync seq */
      C2F(tksynchro)(&c_n1);  /* set sciprompt to -1 (scilab busy) */
      seq= ( (argv[3] != (char *)0) && (strncmp(argv[3],"seq",3)==0) );
      ns=strlen(command); 
      if (C2F(iop).ddt==-1) sciprint(TCL_MSG8,command);
      C2F(syncexec)(command,&ns,&ierr,&seq);
      if (C2F(iop).ddt==-1) sciprint(TCL_MSG9,command);
      C2F(tksynchro)(&C2F(recu).paus);
      if (ierr != 0) return TCL_ERROR;
    }
    else if (strncmp(command,"flush",5)==0)
	{
      /* flush */
      if (C2F(iop).ddt==-1) sciprint(TCL_MSG10);
      while (C2F(ismenu)() && ncomm<arbitrary_max_queued_callbacks-1)
	  {
        ncomm++;
        comm[ncomm] = (char *) MALLOC (bsiz+1);
        if (comm[ncomm] == (char *) 0)
        {
          sciprint (TCL_ERROR28);
          return TCL_ERROR;
        }
        seqf[ncomm]=GetCommand (comm[ncomm]);
      }
      if (C2F(ismenu)()) sciprint(TCL_WARNING5);
      for (nc = 0 ; nc <= ncomm ; nc++ )
	  {
        C2F(tksynchro)(&c_n1);  /* set sciprompt to -1 (scilab busy) */
        if (C2F(iop).ddt==-1)
        {
	      if (seqf[nc]==0)
		  {
			  sciprint(TCL_MSG11,comm[nc]);
		  }
	      else
		  {
			  sciprint(TCL_MSG12,comm[nc]);
		  }
        }
        ns=strlen(comm[nc]);
        C2F(syncexec)(comm[nc],&ns,&ierr,&(seqf[nc]));
        if (C2F(iop).ddt==-1)
        {
          sciprint(TCL_MSG13,comm[nc]);
          FREE(comm[nc]);
        }
        C2F(tksynchro)(&C2F(recu).paus);
        if (ierr != 0) return TCL_ERROR;
      }
      if (C2F(iop).ddt==-1) sciprint(TCL_MSG14);
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
    FREE(command);

  } 
  else
  {
	/* ScilabEval called without argument */
    Scierror(999,TCL_ERROR29);
  }

  return TCL_OK;
}
