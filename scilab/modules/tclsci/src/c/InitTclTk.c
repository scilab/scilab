/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "InitTclTk.h"
#include "TclEvents.h"
#ifndef _MSC_VER
 #include <dirent.h>
 #include <ctype.h>
#endif
#include "setgetSCIpath.h"
#include "sciprint.h"
#include "Scierror.h"
#include "scilabmode.h"
#include "ScilabEval.h"
/*-----------------------------------------------------------------------------------*/ 
int TK_Started=0;
/*-----------------------------------------------------------------------------------*/ 
static char *GetSciPath(void);
/*-----------------------------------------------------------------------------------*/
void initTCLTK(void)
{
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if ( OpenTCLsci()==0 ) 
		{
			TK_Started=1;
		}
		else
		{
			TK_Started=0;
		}
	}
	else
	{
		TK_Started=0;
	}
}
/*-----------------------------------------------------------------------------------*/
int OpenTCLsci(void)
/* Checks if tcl/tk has already been initialised and if not */
/* initialise it. It must find the tcl script */
{
  char *SciPath=NULL;
  char TkScriptpath[PATH_MAX];
  char MyCommand[2048];

#ifndef _MSC_VER
  DIR *tmpdir=NULL;
  Display *XTKdisplay;
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
	sciprint(_("The SCI environment variable is not set.\n"));
    return(1);
  }

#ifdef _MSC_VER
  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, _("/modules/tclsci/tcl/TK_Scilab.tcl"));

  tmpfile2 = fopen(TkScriptpath,"r");
  if (tmpfile2==NULL) 
  {
	sciprint(_("Unable to find TCL initialisation scripts.\n"));
    return(1);
  }
  else fclose(tmpfile2);
#else
  tmpdir=opendir(SciPath);
  if (tmpdir==NULL) 
    {
      sciprint(_("The SCI environment variable is not set.\n"));
      return(1);
    }
  else closedir(tmpdir);
  strcpy(TkScriptpath,SciPath);
  strcat(TkScriptpath, _("/modules/tclsci/tcl/TK_Scilab.tcl"));
  tmpfile2 = fopen(TkScriptpath,"r");
  if (tmpfile2==NULL) 
    {
      sciprint(_("Unable to find TCL initialisation scripts.\n"));
      return(1);
    }
  else fclose(tmpfile2);
#endif /* _MSC_VER */ 
  
  if (TCLinterp == NULL) 
    {
      TCLinterp = Tcl_CreateInterp();
	  if ( TCLinterp == NULL )
	  {
		Scierror(999,_("Tcl Error  : Tcl_CreateInterp.\n"));
		return (1);
	  }

      if ( Tcl_Init(TCLinterp) == TCL_ERROR)
	  {
		Scierror(999,_("Tcl Error  : Tcl_Init.\n"));
		return (1);
	  }

      if ( Tk_Init(TCLinterp) == TCL_ERROR)
	  {
		Scierror(999,_("Tcl Error  : Tk_Init.\n"));
		return (1);
	  }

      sprintf(MyCommand, "set SciPath \"%s\";",SciPath); 
      
	  if ( Tcl_Eval(TCLinterp,MyCommand) == TCL_ERROR  )
	  {
		Scierror(999,_("Tcl Error : %s\n"),TCLinterp->result);
		return (1);
	  }
      
	  Tcl_CreateCommand(TCLinterp,"ScilabEval",TCL_EvalScilabCmd,(ClientData)1,NULL);
    }
   
  if (TKmainWindow == NULL)
    {
      TKmainWindow = Tk_MainWindow(TCLinterp);
      Tk_GeometryRequest(TKmainWindow,2,2);

  	  if ( Tcl_EvalFile(TCLinterp,TkScriptpath) == TCL_ERROR  )
	  {
		Scierror(999,_("Tcl Error : %s\n"),TCLinterp->result);
		return (1);
	  }

      flushTKEvents();
    }

  if (SciPath) {FREE(SciPath);SciPath=NULL;}
  return(0);

}
/*-----------------------------------------------------------------------------------*/
int CloseTCLsci(void)
{
	int bOK=0;
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (TK_Started)
		{
			Tcl_DeleteInterp(TCLinterp);
			TCLinterp=NULL;
			TKmainWindow=NULL;
			bOK=1;
			TK_Started=0;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
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
/*-----------------------------------------------------------------------------------*/
