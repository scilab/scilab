/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "error_scilab.h"
#include "InitTclTk.h"
#include "TclEvents.h"
#ifndef _MSC_VER
 #include <dirent.h>
 #include <X11/Xlib.h>
 #include <X11/Xatom.h>
 #include <X11/Xutil.h>
 #include <X11/cursorfont.h>
 #include <X11/Intrinsic.h>
 #include <X11/StringDefs.h>
 #include <X11/Shell.h>
 #include <X11/Xos.h>
 #include <ctype.h>
#endif
#include "setgetSCIpath.h"
#include "message_scilab.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
extern int TCL_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,CONST char ** argv);
/*-----------------------------------------------------------------------------------*/ 
int TK_Started=0;
#ifndef _MSC_VER
  int XTKsocket=0;
#endif
/*-----------------------------------------------------------------------------------*/ 
char *GetSciPath(void);
/*-----------------------------------------------------------------------------------*/
void initTCLTK(void)
{
	if ( getScilabMode() != NWNI )
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
  char TkScriptpath[2048];
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
	message_scilab("tclsci_message_2");
    return(1);
  }

#ifdef _MSC_VER
  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, "/modules/tclsci/tcl/TK_Scilab.tcl");

  tmpfile2 = fopen(TkScriptpath,"r");
  if (tmpfile2==NULL) 
  {
	message_scilab("tclsci_message_3");
    return(1);
  }
  else fclose(tmpfile2);
#else
  tmpdir=opendir(SciPath);
  if (tmpdir==NULL) 
    {
      message_scilab("tclsci_message_2");
      return(1);
    }
  else closedir(tmpdir);
  strcpy(TkScriptpath,SciPath);
  strcat(TkScriptpath, "/modules/tclsci/tcl/TK_Scilab.tcl");
  tmpfile2 = fopen(TkScriptpath,"r");
  if (tmpfile2==NULL) 
    {
      message_scilab("tclsci_message_3");
      return(1);
    }
  else fclose(tmpfile2);
#endif /* _MSC_VER */ 
  
  if (TCLinterp == NULL) 
    {
      TCLinterp = Tcl_CreateInterp();
	  if ( TCLinterp == NULL )
	  {
		error_scilab(999,"tclsci_error_2");
		return (1);
	  }

      if ( Tcl_Init(TCLinterp) == TCL_ERROR)
	  {
		error_scilab(999,"tclsci_error_3");
		return (1);
	  }

      if ( Tk_Init(TCLinterp) == TCL_ERROR)
	  {
		error_scilab(999,"tclsci_error_4");
		return (1);
	  }

      sprintf(MyCommand, "set SciPath \"%s\";",SciPath); 
      
	  if ( Tcl_Eval(TCLinterp,MyCommand) == TCL_ERROR  )
	  {
		error_scilab(999,"tclsci_error_5",TCLinterp->result);
		return (1);
	  }
      
	  Tcl_CreateCommand(TCLinterp,"ScilabEval",TCL_EvalScilabCmd,(ClientData)1,NULL);
    }
   
  if (TKmainWindow == NULL)
    {
      TKmainWindow = Tk_MainWindow(TCLinterp);
	  #ifndef _MSC_VER
        XTKdisplay=Tk_Display(TKmainWindow);
        XTKsocket = ConnectionNumber(XTKdisplay);
	  #endif
      
      Tk_GeometryRequest(TKmainWindow,2,2);

  	  if ( Tcl_EvalFile(TCLinterp,TkScriptpath) == TCL_ERROR  )
	  {
		error_scilab(999,"tclsci_error_5",TCLinterp->result);
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
	if ( getScilabMode() != NWNI )
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
char *GetSciPath(void)
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
