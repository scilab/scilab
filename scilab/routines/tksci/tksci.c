/* Copyright INRIA */
/* scilab-tk link main module */
/* Bertrand Guiheneuf INRIA 1997 */


#include "tksci.h"
#include "TK_ScilabCallback.h"
#include <stdio.h>

#ifdef IS_WIN32
#else
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

Tcl_Interp *TKinterp=NULL;
Tk_Window TKmainWindow=NULL;
int XTKsocket;

Matrix *UserData[MAX_HANDLE];



int TK_Wait=0;
int TK_NbEv=10;
int TK_Started=0;


void flushTKEvents()
{
  while (Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT)==1) {}
}

/***/
int OpenTksci()
     /* Checks if tk has already been initialised and if not */
     /* initialise it. It must find the tcl script */
{
  int i;
  char *SciPath;
  char TkScriptpath[1000];
  char MyCommand[1000];
#ifndef IS_MSVC 
  DIR *tmpdir;
#endif

  FILE *tmpfile;

  Display *XTKdisplay;

  SciPath=getenv("SCI");
  
  /* test SCI validity) */
  if (SciPath==NULL)
    {
      Cout("The SCI environment variable is not set.\nTK initialisation failed !\n");
      return(1);
    }

#ifdef IS_MSVC
  /* for visual C++ */
 
  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, "/tcl/TK_Scilab.tcl");

  tmpfile = fopen(TkScriptpath,"r");
  if (tmpfile==NULL) 
  {
    Cout("Unable to find TK initialisation scripts \n. Check your SCI environment variable.\nTK initialisation failed !\n");
    return(1);
  }
  else fclose(tmpfile);
   
#else
  tmpdir=opendir(SciPath);
  if (tmpdir==NULL) 
    {
      Cout("The SCI environment variable is not set properly.\nTK initialisation failed !\n");
      return(1);
    }
  else closedir(tmpdir);

  strcpy(TkScriptpath, SciPath);
  strcat(TkScriptpath, "/tcl/TK_Scilab.tcl");

  tmpfile = fopen(TkScriptpath,"r");
  if (tmpfile==NULL) 
  {
    Cout("Unable to find TK initialisation scripts \n. Check your SCI environment variable.\nTK initialisation failed !\n");
    return(1);
  }
  else fclose(tmpfile);
  
#endif /* WIN32 */ 

  if (TKinterp == NULL) 
    {
      TKinterp = Tcl_CreateInterp();
      Tcl_Init(TKinterp);
      Tk_Init(TKinterp);

      sprintf(MyCommand, "set SciPath \"%s\";",SciPath); 
      Tcl_Eval(TKinterp,MyCommand);
      Tcl_CreateCommand(TKinterp,"ScilabEval",TK_EvalScilabCmd,(ClientData)1,NULL);
      for (i=0; i<MAX_HANDLE; i++) UserData[i]=0;
    }
   
  if (TKmainWindow == NULL)
    {
      TKmainWindow = Tk_MainWindow(TKinterp);
      XTKdisplay=Tk_Display(TKmainWindow);
      XTKsocket = ConnectionNumber(XTKdisplay);
      
      Tk_GeometryRequest(TKmainWindow,200,200);
      Tcl_EvalFile(TKinterp,TkScriptpath);
      flushTKEvents();
    }

  TK_Started=1;
  
  return(0);

}







void tkpipo()
{
  
  
}

void inittk()
{
  if (0==1)
    {tkpipo();}
  if ( OpenTksci()==0 ) TK_Started==1;

}





void nocase (s)
     char *s;
{
  int lg;
  int i;
  char c;
  
  lg = strlen(s);
  for (i=0; i<lg; i++)
    {
      c = s[i];
      if ( (c >= 'A') && (c <= 'Z') ) s[i] = 'a' + (c-'A');
    }
  
} 
