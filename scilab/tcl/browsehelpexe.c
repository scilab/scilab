/* an executable to launch browsehelp.tcl 
 * by Matthieu PHILIPPE 01/01/2003
 */
#include <string.h>
#include <stdio.h>
#include <tcl.h>
#include <tk.h>
char *getenv();
void flushTKEvents();

Tk_Window TKmainWindow=NULL;
Display *XTKdisplay;
int XTKsocket;
char TkScriptpath[1000];

int main(int argc, char *argv[]) 
{
  Tcl_Interp *interp;
  if (argv[1]==NULL)
    {
      printf("The second argument must be initialised !\n");
      return(1);
    }
  if (getenv("SCI")==(char *)NULL)
    {
      printf("The SCI environment variable is not set.\nTK initialisation failed !\n");
      return(1);
    }
  strcpy(TkScriptpath, getenv("SCI"));
  strcat(TkScriptpath, "/tcl/browsehelp.tcl");

#ifdef TCL_MAJOR_VERSION
#ifdef TCL_MINOR_VERSION
#if TCL_MAJOR_VERSION >= 8
#if TCL_MINOR_VERSION > 0  
  Tcl_FindExecutable(NULL);
#endif
#endif
#endif
#endif 

  interp = Tcl_CreateInterp();
  /* TK_AppInit*/
  if (Tcl_Init(interp) == TCL_ERROR) 
    {
      return TCL_ERROR;
    }
  if (Tk_Init(interp) == TCL_ERROR) 
    {
      /*return TCL_ERROR;*/
    }
  if (TKmainWindow == NULL)
    {
      TKmainWindow = Tk_MainWindow(interp);
      XTKdisplay = Tk_Display(TKmainWindow);
      XTKsocket = ConnectionNumber(XTKdisplay);
      Tk_GeometryRequest(TKmainWindow,200,200);
      Tcl_SetVar(interp, "manpath", argv[1],TCL_GLOBAL_ONLY);
      Tcl_SetVar(interp, "SciPath",getenv("SCI") ,TCL_GLOBAL_ONLY);
      Tcl_EvalFile(interp, TkScriptpath);
      Tcl_Eval(interp, "wm withdraw .");
      /*Tcl_Eval(interp, "catch {destroy $sciw}");*/
      Tcl_Eval(interp, "wm protocol $sciw WM_DELETE_WINDOW exit;");
      Tcl_Eval(interp, "proc quitapp {widget} {exit;}");
      flushTKEvents();
    }
  Tk_MainLoop();
  return(0);
}

void flushTKEvents()
{
  while (Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT)==1) {}
}
