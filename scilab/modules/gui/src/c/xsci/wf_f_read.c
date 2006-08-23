/*
 * FIG : Facility for Interactive Generation of figures
 * Copyright (c) 1985 by Supoj Sutanthavibul
 *
 * "Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both the copyright
 * notice and this permission notice appear in supporting documentation. 
 * No representations are made about the suitability of this software for 
 * any purpose.  It is provided "as is" without express or implied warranty."
 */

#include "wf_fig.h"
#include "wf_figx.h"
#include "wf_resources.h"
#include "wf_mode.h"
#include "wf_w_setup.h"
#include "wf_w_util.h"

#include "../machine.h"
#include "All-extern.h"

/* file popup information */
extern Widget	file_popup;	/* the popup itself */

/* so w_file.c can access */
Boolean	file_msg_is_popped=False;
Widget	file_msg_popup;

static XtEventHandler file_msg_panel_dismiss  __PARAMS((Widget w, XButtonEvent *ev));  

static Widget	file_msg_panel,
file_msg_win, file_msg_dismiss;
static Boolean	first_file_msg;
static int	file_msg_length=0;
static char    *read_file_name;
static char	tmpstr[100];

static String	file_msg_translations =
"<Message>WM_PROTOCOLS: DismissFileMsg()\n";
static XtEventHandler file_msg_panel_dismiss();
static XtActionsRec	file_msg_actions[] =
{
  {"DismissFileMsg", (XtActionProc) file_msg_panel_dismiss},
};

DeclareStaticArgs(10);

/* modif jpc */

void file_msg(format, arg1)
     char	   *format;
     char *arg1;
{
  XawTextBlock block;
  popup_file_msg();
  if (first_file_msg)
    {
      first_file_msg = False;
      file_msg("---------------------",(char *) NULL);
      file_msg("File %s:",read_file_name);
    }
  sprintf(tmpstr, format, arg1);
  strcat(tmpstr,"\n");
  /* append this message to the file message widget string */
  block.firstPos = 0;
  block.ptr = tmpstr;
  block.length = strlen(tmpstr);
  block.format = FMT8BIT;
  /* make editable to add new message */
  FirstArg(XtNeditType, XawtextEdit);
  SetValues(file_msg_win);
  /* insert the new message after the end */
  (void) XawTextReplace(file_msg_win,(XawTextPosition) file_msg_length, 
			(XawTextPosition)file_msg_length,&block);
  (void) XawTextSetInsertionPoint(file_msg_win,(XawTextPosition)file_msg_length);

  /* make read-only again */
  FirstArg(XtNeditType, XawtextRead);
  SetValues(file_msg_win);
  file_msg_length += block.length;
}

void  clear_file_message(w, ev)
     Widget	    w;
     XButtonEvent   *ev;
{
  XawTextBlock	block;
  int			replcode;

  if (!file_msg_popup)
    return;

  tmpstr[0]=' ';
  block.firstPos = 0;
  block.ptr = tmpstr;
  block.length = 1;
  block.format = FMT8BIT;

  /* make editable to clear message */
  FirstArg(XtNeditType, XawtextEdit);
  NextArg(XtNdisplayPosition, 0);
  SetValues(file_msg_win);

  /* replace all messages with one blank */
  replcode = XawTextReplace(file_msg_win,(XawTextPosition)0,(XawTextPosition)file_msg_length,&block);
  if (replcode == XawPositionError)
    fprintf(stderr,"XawTextReplace XawPositionError\n");
  else if (replcode == XawEditError)
    fprintf(stderr,"XawTextReplace XawEditError\n");

  /* make read-only again */
  FirstArg(XtNeditType, XawtextRead);
  SetValues(file_msg_win);
  file_msg_length = 0;
}

static Bool grabbed=False;

static XtEventHandler file_msg_panel_dismiss(w, ev)
     Widget	    w;
     XButtonEvent   *ev;
{
  XtPopdown(file_msg_popup);
  file_msg_is_popped=False;
  return(0);
}

void popup_file_msg()
{
  extern Atom wm_delete_window;

  if (file_msg_popup)
    {
      if (!file_msg_is_popped)
	{
	  XtPopup(file_msg_popup, XtGrabNonexclusive);
	  XSetWMProtocols(XtDisplay(file_msg_popup), 
			  XtWindow(file_msg_popup),
			  &wm_delete_window, 1);
	  grabbed = True;
	}
      file_msg_is_popped = True;
      return;
    }

  file_msg_is_popped = True;
  FirstArg(XtNx, 0);
  NextArg(XtNy, 0);
  NextArg(XtNtitle, " File error messages");
  file_msg_popup = XtCreatePopupShell("xsci_file_msg",
				      transientShellWidgetClass,
				      w_toplevel, Args, ArgCount);
  XtOverrideTranslations(file_msg_popup,
			 XtParseTranslationTable(file_msg_translations));
  XtAppAddActions(app_con, file_msg_actions, XtNumber(file_msg_actions));

  file_msg_panel = XtCreateManagedWidget("file_msg_panel", formWidgetClass,
					 file_msg_popup,(ArgList) 0,(Cardinal)0);
  FirstArg(XtNwidth, 500);
  NextArg(XtNheight, 200);
  NextArg(XtNeditType, XawtextRead);
  NextArg(XtNdisplayCaret, False);
  NextArg(XtNscrollHorizontal, XawtextScrollWhenNeeded);
  NextArg(XtNscrollVertical, XawtextScrollAlways);
  file_msg_win = XtCreateManagedWidget("file_msg_win", asciiTextWidgetClass,
				       file_msg_panel, Args, ArgCount);

  FirstArg(XtNlabel, "Dismiss");
  NextArg(XtNheight, 25);
  NextArg(XtNborderWidth, INTERNAL_BW);
  NextArg(XtNfromVert, file_msg_win);
  file_msg_dismiss = XtCreateManagedWidget("dismiss", commandWidgetClass,
					   file_msg_panel, Args, ArgCount);
  XtAddEventHandler(file_msg_dismiss, ButtonReleaseMask, (Boolean) 0,
		    (XtEventHandler)file_msg_panel_dismiss, (XtPointer) NULL);

  FirstArg(XtNlabel, "Clear");
  NextArg(XtNheight, 25);
  NextArg(XtNborderWidth, INTERNAL_BW);
  NextArg(XtNfromVert, file_msg_win);
  NextArg(XtNfromHoriz, file_msg_dismiss);
  file_msg_dismiss = XtCreateManagedWidget("clear", commandWidgetClass,
					   file_msg_panel, Args, ArgCount);
  XtAddEventHandler(file_msg_dismiss, ButtonReleaseMask, (Boolean) 0,
		    (XtEventHandler)clear_file_message, (XtPointer) NULL);


  XtPopup(file_msg_popup, XtGrabNonexclusive);
  XSetWMProtocols(XtDisplay(file_msg_popup), 
		  XtWindow(file_msg_popup),
		  &wm_delete_window, 1);
  grabbed = True;

}
