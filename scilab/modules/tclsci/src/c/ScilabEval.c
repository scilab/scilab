/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "TCL_Global.h"
#include "ScilabEval.h"
#include "sciprint.h"
#include "Scierror.h"
#include "tksynchro.h"
#include "../../localization/includes/localization.h"
#include "syncexec.h"
#include "dynamic_menus.h"
/*-----------------------------------------------------------------------------------*/
/* what's the max number of commands in the queue ??*/
#define arbitrary_max_queued_callbacks 20
#define AddCharacters 4
/*-----------------------------------------------------------------------------------*/
static int c_n1 = -1;     
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
	char *AsciiFromUTF8=NULL;
	char *msg=_("TCL_EvalScilabCmd %s");

	AsciiFromUTF8=MALLOC(sizeof(char)*(strlen(argv[1])+AddCharacters));

    /* UTF to ANSI */
	Tcl_UtfToExternal(theinterp, NULL, argv[1], (int)strlen(argv[1]), 0, NULL, AsciiFromUTF8, (int)(strlen(argv[1])+AddCharacters), NULL, NULL,NULL);

    sciprint_full(msg,AsciiFromUTF8);
	if (msg) {FREE(msg);msg=NULL;}

    while (argv[++argc]) sciprint(" %s",argv[argc]);
    sciprint("\n");

	if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}
  }

  if (argv[1] != (char *)0)
  {
	  char *AsciiFromUTF8=NULL;
	  AsciiFromUTF8=MALLOC(sizeof(char)*(strlen(argv[1])+AddCharacters));

	  /* UTF to ANSI */
	  Tcl_UtfToExternal(theinterp, NULL, argv[1], (int)strlen(argv[1]), 0, NULL, AsciiFromUTF8, (int)(strlen(argv[1])+AddCharacters), NULL, NULL,NULL);

    if (strlen(AsciiFromUTF8)>=bsiz)
	{
      command = (char *) MALLOC (bsiz * sizeof (char));
      if (command == (char *) 0)
      {
		sciprint(_("TCL_EvalScilabCmd: No more memory.\n"));
        return TCL_ERROR;
      }
      memset(command,'\0',bsiz);
      strncpy(command,AsciiFromUTF8,bsiz-1);
	  sciprint(_("Warning: ScilabEval command is too long and has been truncated to %d characters!\n"),bsiz-1);
    }
	else
	{
      command = (char *) MALLOC ((strlen (AsciiFromUTF8) + 1) * sizeof (char));
      if (command == (char *) 0)
      {
		  sciprint(_("TCL_EvalScilabCmd: No more memory.\n"));
          return TCL_ERROR;
      }
      strcpy(command,AsciiFromUTF8);
    }
	if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}

    if ( (argv[2] != (char *)0) && (strncmp(argv[2],"sync",4)==0) )
	{
      /* sync or sync seq */
      C2F(tksynchro)(&c_n1);  /* set sciprompt to -1 (scilab busy) */
      seq= ( (argv[3] != (char *)0) && (strncmp(argv[3],"seq",3)==0) );
      ns=(int)strlen(command); 
      if (C2F(iop).ddt==-1)
	  {
		  char *msg=_(" Execution starts for %s");
		  sciprint_full(msg,command);
		  if (msg){FREE(msg);msg=NULL;}
          sciprint("\n");
	  }
      C2F(syncexec)(command,&ns,&ierr,&seq,ns);
      if (C2F(iop).ddt==-1)
	  {
		  char *msg=_(" Execution ends for %s");
		  sciprint_full(msg,command);
		  if (msg){FREE(msg);msg=NULL;}
          sciprint("\n");
	  }
      C2F(tksynchro)(&C2F(recu).paus);
      if (ierr != 0) return TCL_ERROR;
    }
    else if (strncmp(command,"flush",5)==0)
	{
      /* flush */
      if (C2F(iop).ddt==-1) sciprint(_(" Flushing starts for queued commands.\n"));
      while (ismenu() && ncomm<arbitrary_max_queued_callbacks-1)
	  {
        ncomm++;
        comm[ncomm] = (char *) MALLOC (bsiz+1);
        if (comm[ncomm] == (char *) 0)
        {
			sciprint(_("TCL_EvalScilabCmd: No more memory.\n"));
			return TCL_ERROR;
        }
        seqf[ncomm]=GetCommand (comm[ncomm]);
      }
      if (ismenu()) sciprint(_("Warning: Too many callbacks in queue!\n"));
      for (nc = 0 ; nc <= ncomm ; nc++ )
	  {
        C2F(tksynchro)(&c_n1);  /* set sciprompt to -1 (scilab busy) */
        if (C2F(iop).ddt==-1)
        {
	      if (seqf[nc]==0)
		  {
			  char *msg=_(" Flushed execution starts for %s - No option");
			  sciprint_full(msg,comm[nc]);
			  if (msg){FREE(msg);msg=NULL;}
              sciprint("\n");
		  }
	      else
		  {
			  char *msg=_(" Flushed execution starts for %s - seq");
			  sciprint_full(msg,comm[nc]);
			  if (msg){FREE(msg);msg=NULL;}
              sciprint("\n");
		  }
        }
        ns=(int)strlen(comm[nc]);
        C2F(syncexec)(comm[nc],&ns,&ierr,&(seqf[nc]),ns);
        if (C2F(iop).ddt==-1)
        {
			char *msg=_(" Flushed execution ends for %s");
			sciprint_full(msg,comm[nc]);
			if (msg){FREE(msg);msg=NULL;}
            sciprint("\n");
        }
        FREE(comm[nc]);
        C2F(tksynchro)(&C2F(recu).paus);
        if (ierr != 0) return TCL_ERROR;
      }
      if (C2F(iop).ddt==-1) sciprint(_(" Flushing ends\n"));
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
	Scierror(999,_("ScilabEval: at least one argument is required.\n"));
  }

  return TCL_OK;
}
