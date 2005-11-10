/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "InitTclTk.h"
#include "TclEvents.h"
#ifndef WIN32
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
/*-----------------------------------------------------------------------------------*/ 
extern int TCL_EvalScilabCmd(ClientData clientData,Tcl_Interp * theinterp,int objc,CONST char ** argv);
extern int IsFromC(void);
/*-----------------------------------------------------------------------------------*/ 
int TK_Started=0;
#ifndef WIN32
  int XTKsocket=0;
#endif
/*-----------------------------------------------------------------------------------*/ 
char *GetSciPath(void);
/*-----------------------------------------------------------------------------------*/ 
void initTCLTK(void)
{
	if (!IsFromC())  if ( OpenTCLsci()==0 ) TK_Started=1;
}
/*-----------------------------------------------------------------------------------*/
int OpenTCLsci(void)
/* Checks if tcl/tk has already been initialised and if not */
/* initialise it. It must find the tcl script */
{
  char *SciPath=NULL;
  char TkScriptpath[2048];
  char MyCommand[2048];

#ifndef WIN32
  DIR *tmpdir=NULL;
  Display *XTKdisplay;
#endif

  FILE *tmpfile=NULL;

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
      sciprint(TCL_WARNING1);
      return(1);
    }

#ifdef WIN32
  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, "/tcl/TK_Scilab.tcl");


  tmpfile = fopen(TkScriptpath,"r");
  if (tmpfile==NULL) 
    {
      sciprint(TCL_WARNING2);
      return(1);
    }
  else fclose(tmpfile);
#else
  tmpdir=opendir(SciPath);
  if (tmpdir==NULL) 
    {
      sciprint(TCL_WARNING1);
      return(1);
    }
  else closedir(tmpdir);
  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, "/tcl/TK_Scilab.tcl");

  tmpfile = fopen(TkScriptpath,"r");
  if (tmpfile==NULL) 
    {
      sciprint(TCL_WARNING2);
      return(1);
    }
  else fclose(tmpfile);
#endif /* WIN32 */ 

  if (TCLinterp == NULL) 
    {
      TCLinterp = Tcl_CreateInterp();
	  if ( TCLinterp == NULL )
	  {
	    Scierror(999,TCL_ERROR1);
		return (1);
	  }

      if ( Tcl_Init(TCLinterp) == TCL_ERROR)
	  {
		Scierror(999,TCL_ERROR2);
		return (1);
	  }

      if ( Tk_Init(TCLinterp) == TCL_ERROR)
	  {
		Scierror(999,TCL_ERROR3);
		return (1);
	  }

      sprintf(MyCommand, "set SciPath \"%s\";",SciPath); 
      
	  if ( Tcl_Eval(TCLinterp,MyCommand) == TCL_ERROR  )
	  {
		Scierror(999,TCL_ERROR4,TCLinterp->result);
		return (1);
	  }
      
	  Tcl_CreateCommand(TCLinterp,"ScilabEval",TCL_EvalScilabCmd,(ClientData)1,NULL);
    }
   
  if (TKmainWindow == NULL)
    {
      TKmainWindow = Tk_MainWindow(TCLinterp);
	  #ifndef WIN32
        XTKdisplay=Tk_Display(TKmainWindow);
        XTKsocket = ConnectionNumber(XTKdisplay);
	  #endif
      
      Tk_GeometryRequest(TKmainWindow,2,2);

  	  if ( Tcl_EvalFile(TCLinterp,TkScriptpath) == TCL_ERROR  )
	  {
		Scierror(999,TCL_ERROR4,TCLinterp->result);
		return (1);
	  }

      flushTKEvents();
    }

  if (SciPath) {FREE(SciPath);SciPath=NULL;}
  return(0);

}
/*-----------------------------------------------------------------------------------*/
char *GetSciPath(void)
/* force SciPath to Unix format for compatibility (Windows) */
{
	char *PathUnix=NULL;
	char *SciPathTmp=NULL;
	int i=0;

	SciPathTmp=getenv("SCI");

	if (SciPathTmp)
	{
		PathUnix=(char*)MALLOC( ((int)strlen(SciPathTmp)+1)*sizeof(char) );

		strcpy(PathUnix,SciPathTmp);
		for (i=0;i<(int)strlen(PathUnix);i++)
		{
			if (PathUnix[i]=='\\') PathUnix[i]='/';
		}
	}
	
	return PathUnix;
}
/*-----------------------------------------------------------------------------------*/
