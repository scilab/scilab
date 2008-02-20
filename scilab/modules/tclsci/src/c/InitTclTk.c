/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/* 2007 - INRIA - Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include "InitTclTk.h"
#ifndef _MSC_VER
#include <dirent.h>
#include <ctype.h>
#endif
#include "setgetSCIpath.h"
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "scilabmode.h"
#include "ScilabEval.h"
#include "TCL_Command.h"
#include "BOOL.h"

/*--------------------------------------------------------------------------*/
BOOL TK_Started=FALSE;

/* The tclLoop thread Id
   in order to wait it ends when closing Scilab */
__threadId TclThread;

__threadSignal InterpReady;
__threadLock InterpReadyLock;


/*--------------------------------------------------------------------------*/
static char *GetSciPath(void);
/*--------------------------------------------------------------------------*/
static void *DaemonOpenTCLsci(void* in)
     /* Checks if tcl/tk has already been initialised and if not */
     /* initialise it. It must find the tcl script */
{
  char *SciPath=NULL;
  char TkScriptpath[PATH_MAX];
  char MyCommand[2048];
  BOOL tkStarted=TRUE;

#ifndef _MSC_VER
  DIR *tmpdir=NULL;
  //  Display *XTKdisplay;
#endif

  FILE *tmpfile2=NULL;

#ifdef TCL_MAJOR_VERSION
#ifdef TCL_MINOR_VERSION
#if TCL_MAJOR_VERSION >= 8
#if TCL_MINOR_VERSION > 0
  Tcl_FindExecutable(" ");
#endif
#endif
#endif
#endif
  SciPath=GetSciPath();

  /* test SCI validity */
  if (SciPath==NULL)
    {
      sciprint(_("The SCI environment variable is not set.\nTCL initialisation failed !\n"));
     }

#ifdef _MSC_VER
  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, "/modules/tclsci/tcl/TK_Scilab.tcl");

  tmpfile2 = fopen(TkScriptpath,"r");
  if (tmpfile2==NULL)
    {
      sciprint(_("Unable to find TCL initialisation scripts.\nCheck your SCI environment variable.\nTCL initialisation failed !"));
     }
  else fclose(tmpfile2);
#else
  tmpdir=opendir(SciPath);
  if (tmpdir==NULL)
    {
      sciprint(_("The SCI environment variable is not set.\nTCL initialisation failed !\n"));
     }
  else closedir(tmpdir);
  strcpy(TkScriptpath,SciPath);
  strcat(TkScriptpath,"/modules/tclsci/tcl/TK_Scilab.tcl");
  tmpfile2 = fopen(TkScriptpath,"r");
  if (tmpfile2==NULL)
    {
      sciprint(_("Unable to find TCL initialisation scripts.\nCheck your SCI environment variable.\nTCL initialisation failed !"));
     }
  else fclose(tmpfile2);
#endif /* _MSC_VER */

  if (TCLinterp == NULL)
    {
      TCLinterp = Tcl_CreateInterp();
      if ( TCLinterp == NULL )
	{
	  Scierror(999,_("TCL Error: Unable to create TCL interpreter (Tcl_CreateInterp).\n"));
	}

      if ( Tcl_Init(TCLinterp) == TCL_ERROR)
	{
	  Scierror(999,_("TCL Error: Error during the TCL initialization (Tcl_Init): %s\n"),TCLinterp->result);
	}

      if ( Tk_Init(TCLinterp) == TCL_ERROR)
	{
		Scierror(999,_("TCL Error: Error during the TK initialization (Tk_Init):%s\n"),TCLinterp->result);
		tkStarted=FALSE;
	}

      sprintf(MyCommand, "set SciPath \"%s\";",SciPath);

      if ( Tcl_Eval(TCLinterp,MyCommand) == TCL_ERROR  )
	{
	  Scierror(999,_("TCL Error: Error during the Scilab/TCL init process. Could not set SciPath: %s\n"),TCLinterp->result);
	}

      Tcl_CreateCommand(TCLinterp,"ScilabEval",TCL_EvalScilabCmd,(ClientData)1,NULL);
    }

	if (TKmainWindow == NULL && tkStarted)
    {
		#ifdef _MSC_VER
			TKmainWindow = Tk_MainWindow(TCLinterp);
			Tk_GeometryRequest(TKmainWindow,2,2);
		#endif
			if ( Tcl_EvalFile(TCLinterp,TkScriptpath) == TCL_ERROR  )
			{
				Scierror(999,_("TCL Error: Error during the Scilab/TK init process. Error while loading %s: %s\n"),TkScriptpath, TCLinterp->result);
			}
	}


  if (SciPath) {FREE(SciPath);SciPath=NULL;}

  // Signal TclInterpreter init is now over.
  __Signal(&InterpReady);
  __UnLock(&InterpReadyLock);

  // This start a periodic and endless call to "update"
  // TCL command. This causes any TCL application to start
  // and run as if it's in the main program thread.
  startTclLoop(TCLinterp);

  return(0);

}
/*--------------------------------------------------------------------------*/
int OpenTCLsci(void)
{
  __InitSignal(&InterpReady);
  // Open TCL interpreter in a separated thread.
  // Allows all Tcl application not to freeze nor decrease Scilab speed.
  // Causes also Scilab let those application live their own lifes.
  //__InitSignal(&InterpReady);
  __CreateThread(&TclThread, &DaemonOpenTCLsci);
  // Wait to be sure initialisation is complete.
  __Wait(&InterpReady, &InterpReadyLock);
  return 0;
}
/*--------------------------------------------------------------------------*/
BOOL CloseTCLsci(void)
{
  if ( getScilabMode() != SCILAB_NWNI )
    {
      if (isTkStarted())
	{
	  setTkStarted(FALSE);
	  __WaitThreadDie(TclThread);
	  TCLinterp=NULL;
	  TKmainWindow=NULL;
	  return TRUE;
	}
    }
  return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *GetSciPath(void)
     /* force SciPath to Unix format for compatibility (Windows) */
{
  char *PathUnix=NULL;
  char *SciPathTmp=NULL;
  int i=0;

  SciPathTmp=getSCIpath();

  if (SciPathTmp)
    {
      PathUnix=(char*)MALLOC( ((int)strlen(SciPathTmp)+1)*sizeof(char) );

      strcpy(PathUnix,SciPathTmp);
      for (i=0;i<(int)strlen(PathUnix);i++)
	{
	  if (PathUnix[i]=='\\') PathUnix[i]='/';
	}
    }
  if (SciPathTmp) {FREE(SciPathTmp);SciPathTmp=NULL;}
  return PathUnix;
}
/*--------------------------------------------------------------------------*/
BOOL isTkStarted(void){
  return TK_Started;
}
/*--------------------------------------------------------------------------*/
void setTkStarted(BOOL isTkSet){
  TK_Started=isTkSet;
}
/*--------------------------------------------------------------------------*/
