/* Copyright (C) 1998 Chancelier Jean-Philippe */

#include "wf_fig.h"
#include "wf_figx.h"
#include "wf_resources.h"
#include "wf_mode.h"
#include "wf_w_drawprim.h"
#include "wf_w_util.h"
#include "wf_w_setup.h"

#include <string.h> /* in case of dmalloc */ 

#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif

#include "../machine.h"
#include "All-extern.h"
#include "All-extern-x1.h" 

static void set_std_color  __PARAMS((Widget w, XtPointer number, XButtonEvent *ev));  
static void choice_panel_cancel  __PARAMS((Widget w, char *dum, XButtonEvent *ev));  
static void set_color_ok  __PARAMS((Widget w, int number, XButtonEvent *ev));  

DeclareStaticArgs(20);

/* width/height of the standard color cells */
#define STD_COL_W	20
#define STD_COL_H	20
#define MAXCOLS 32 
#define x_color(i) i 
/** XXXXXX **/

void popup_choice_panel(tool) 
     Widget tool;
{
  Widget choice_popup,form,cancel,ok_button;
  NextArg(XtNresize, False);
  NextArg(XtNresizable, False);
  NextArg(XtNtitle, "View Colormap");
  /** XXXXX   NextArg(XtNcolormap, tool_cm); **/

  choice_popup = XtCreatePopupShell("ColorView",
				    transientShellWidgetClass, tool,
				    Args, ArgCount);
  
  form = XtCreateManagedWidget("form", formWidgetClass, choice_popup, NULL, 0);

  FirstArg(XtNlabel, "Cancel");
  NextArg(XtNresize, False);
  NextArg(XtNresizable, False);
  NextArg(XtNheight, 32);
  NextArg(XtNborderWidth, INTERNAL_BW);
  cancel = XtCreateManagedWidget("cancel", commandWidgetClass,
				 form, Args, ArgCount);

  XtAddEventHandler(cancel, ButtonReleaseMask, (Boolean) 0,
		    (XtEventHandler)choice_panel_cancel, (XtPointer) NULL);

  /* make the OK button */
  FirstArg(XtNlabel, "  Ok  ");
  NextArg(XtNheight, 30);
  NextArg(XtNfromHoriz, cancel);
  NextArg(XtNborderWidth, INTERNAL_BW);
  ok_button = XtCreateManagedWidget("set_color_ok", commandWidgetClass,
				    form, Args, ArgCount);
  XtAddEventHandler(ok_button, ButtonReleaseMask, (Boolean) 0,
		    (XtEventHandler) set_color_ok, (XtPointer) NULL);
  
  create_color_panel(form,cancel);
  XtPopup(choice_popup, XtGrabExclusive);
  /** XXXXX : a rajouter **/
  /**(void) XSetWMProtocols(XtDisplay(choice_popup), XtWindow(choice_popup),
    &wm_delete_window, 1);
    **/
}


void create_color_panel(form,cancel)
     Widget		 form,  cancel;
{
  int screencolor=0;
  int		 i;
  XColor	 col;
  Widget		 below=NULL, beside=NULL, stdForm, stdLabel;
  /********************************/
  /* now the standard color panel */
  /********************************/

  FirstArg(XtNlabel, "Standard Colors");
  NextArg(XtNfromVert,cancel);
  stdLabel = XtCreateManagedWidget("stdLabel", labelWidgetClass,
				   form, Args, ArgCount);

  /* put them in a form just to make a nice border */
  FirstArg(XtNfromVert, stdLabel);
  NextArg(XtNvertDistance, 0);
  stdForm = XtCreateManagedWidget("stdForm", formWidgetClass,
				  form, Args, ArgCount);

  /* create the standard color buttons */
  for (i = 0; i < MAXCOLS;  i++) 
    {
      int choice ;
      FirstArg(XtNheight, STD_COL_H);
      choice = i;
      /* check for new row of buttons */
	if (i % 10 == 0) {
	  if (i == 0)
	    below = NULL;
	  else
	    below = beside;
	  beside = NULL;
	}
      /* standard color menu */
	getcolordef(&screencolor);
	if (screencolor)
	  {
	    char num_col[4];
	    col.pixel = x_color(i);
	    /* set same so we don't get white or black when click on color */
	    NextArg(XtNforeground, 0);
	    NextArg(XtNbackground, x_color(i));
	    sprintf(num_col,"%d",i);
	    NextArg(XtNlabel, num_col);
	  } 
	NextArg(XtNwidth, STD_COL_W);
	NextArg(XtNfromVert, below);
	NextArg(XtNfromHoriz, beside);
	NextArg(XtNresize, False);
	NextArg(XtNresizable, False);
	beside = XtCreateManagedWidget("stdColor", commandWidgetClass,
				       stdForm, Args, ArgCount);
	XtAddEventHandler(beside, ButtonReleaseMask, (Boolean) 0,
			  (XtEventHandler) set_std_color, (XtPointer) i );
    }
}


/* come here when cancel is pressed.  This is in addition to the 
   cancel callback routine that gets called in w_indpanel.c */

void
cancel_color_popup(w, dum1, dum2)
     Widget	w;
     XtPointer dum1, dum2;
{

}



/* ok button */

static void
set_std_color(w, number, ev)
     Widget		 w;
     XtPointer number;
     XButtonEvent	*ev;
{
  sciprint("%d",(int) number);
}




/* ok button */

static void
choice_panel_cancel(w, dum, ev)
     Widget		 w;
     char		*dum;
     XButtonEvent	*ev;
{

}


/* ok button */

static void
set_color_ok(w, number, ev)
     Widget		 w;
     XButtonEvent	*ev;
{

}

