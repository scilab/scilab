/* Copyright ENPC */
/********************************************************
 * choice.c The XWindow part 
 * All the generic stuff for dealing with choice widgets.
 * (from xgc) 
 ***********************************************************/

#include "men_scilab.h"

extern int set_cmap __PARAMS((Window w)); 
extern void DisplayInit __PARAMS((char *string,Display **dpy,Widget *toplevel));
extern int ok_prep  __PARAMS((char *filemask,char *dirname,char *title,int flag,int *err));
extern int ok_end  __PARAMS((void));
extern int w_init  __PARAMS((  Widget w));
extern int set_temp_wf_cursor __PARAMS((Cursor cursor));
extern void Rescan    __PARAMS( (Widget widget, XEvent*	event,
				String* params,    Cardinal* num_params));
extern int reset_wf_cursor  __PARAMS((void));
extern int create_file_panel  __PARAMS((  Widget w, char * description));
extern int getMenuBut0  __PARAMS((  Widget *w));

extern void C2F(cvstr)();

extern XtAppContext app_con; 

extern Widget file_popup;
extern Boolean	file_msg_is_popped;
extern Widget	file_msg_popup;
extern Cursor   cur_wf_cursor,arrow_wf_cursor,wait_wf_cursor;

static int ok_Flag_sci; 
static char *xgetf_str ;
static int error=0;

/*************************************************     
 * Really Ugly I'm ashamed 
 * The main function to activate the file menu 
 **********************************************************/

int GetFileWindow(char *filemask, char **file, char *dirname, int flag,
		  int *ierr, char *title)
{
  int err=0;
  static Display *dpy = (Display *) NULL;
  static  Widget toplevel,toplevel1;
  error= 0;
  getMenuBut0(&toplevel);
  DisplayInit("",&dpy,&toplevel1);
  if ( toplevel == (Widget) 0) 
    {
      /* we are in scilab -nw */
      toplevel = toplevel1;
    }
  popup_file_panel1(toplevel,title);
  ok_prep(filemask,dirname,title,flag,&err);
  if ( err != 1 )   XtSpecialLoop();
  ok_end();
  XtSetSensitive(toplevel,True);
  *file = xgetf_str;
  *ierr = error;
  if ( ok_Flag_sci == -1 ) 
    return(FALSE);
  else
    return(TRUE);
}

/*************************************************     
 * X11 Event loop 
 **********************************************************/

void XtSpecialLoop(void)
{
  XEvent event;
  ok_Flag_sci= 0;
  for (;;) {
    XtAppNextEvent(app_con,&event);
    XtDispatchEvent(&event);
    if (ok_Flag_sci != 0) break;
  }
}

/*************************************************     
 * The cancel command callback 
 **********************************************************/

void cancel_getfile(void)
{ 
  ok_Flag_sci = -1;
}

/*************************************************     
 * Activated when file is choosed 
 * write result in a static var (str) 
 **********************************************************/

int write_getfile(char *dir, char *file)
{
  xgetf_str = (char *) MALLOC( (strlen(dir)+strlen(file)+2)*(sizeof(char)));
  if (xgetf_str != 0)
    { 
      int ind ;
      sprintf(xgetf_str,"%s/%s",dir,file);
      ind = strlen(xgetf_str) - 1 ;
      if (xgetf_str[ind] == '\n') xgetf_str[ind] = '\0' ;
    }
  else 
    {
      Scistring("Malloc : No more place");
      error = 1;
    }
  ok_Flag_sci= 1;
  return(0);
}

/*************************************************     
 * Activate the file menu 
 **********************************************************/

int popup_file_panel1(Widget w, char *description)
{
    extern Atom     wm_delete_window;
    w_init(w);
    set_temp_wf_cursor(wait_wf_cursor);
    XtSetSensitive(w, False);
    if (!file_popup)
	create_file_panel(w,description);
    else
      Rescan((Widget) 0, (XEvent*) 0, (String*) 0, (Cardinal*) 0);

    XtPopup(file_popup, XtGrabNonexclusive);
    (void) XSetWMProtocols(XtDisplay(file_popup), XtWindow(file_popup),
			   &wm_delete_window, 1);
    if (file_msg_is_popped)
	XtAddGrab(file_msg_popup, False, False);
    reset_wf_cursor();
    return(0);
}



