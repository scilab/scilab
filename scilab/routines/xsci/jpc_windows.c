/*****************************************************************************
 *  Based on
 *  xdbx - X Window System interface to dbx
 *
 *  Copyright 1989, 1990 The University of Texas at Austin
 *
 *  Permission to use, copy, modify, and distribute this software and its
 *  documentation for any purpose and without fee is hereby granted,
 *  provided that the above copyright notice appear in all copies and that
 *  both that copyright notice and this permission notice appear in
 *  supporting documentation, and that the name of The University of Texas
 *  not be used in advertising or publicity pertaining to distribution of
 *  the software without specific, written prior permission.  The
 *  University of Texas makes no representations about the suitability of
 *  this software for any purpose.  It is provided "as is" without express
 *  or implied warranty.
 *
 *  THE UNIVERSITY OF TEXAS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS
 *  SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 *  FITNESS, IN NO EVENT SHALL THE UNIVERSITY OF TEXAS BE LIABLE FOR ANY
 *  SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 *  RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 *  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 *  CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 *  Author:  	Po Cheung, The University of Texas at Austin
 *  Created:   	March 10, 1989
 *
 *****************************************************************************/

/*  windows.c:
 *
 *    CreateTitleBar() :	Create title bar.
 *    CreateFileLabel() :	Create file label in file window.
 *    CreateLineLabel() :	Create line label in file window.
 *    CreateFileWindow() :	Create file window.
 *    CreateMessageWindow() :	Create message window.
 *    CreateSubWindows() :	Create the subwindows.
 *    UpdateFileLabel() :	Update file label.
 *    UpdateLineLabel() :	Update line label.
 *    UpdateMessageWindow() :	Update message window.
 */

#include "x_ptyx.h"
#include "x_data.h"
#include "jpc_global.h"
#include "../version.h"

extern Widget realToplevel;

Widget	fileWindow,			/* parent of fileLabel and lineLabel */
	messageWindow,			/* window for displaying messages */
	separator;			/* separator in vpane */

/** Unused 
static void CreateFileLabel  __PARAMS((Widget parent));  
static void CreateLineLabel  __PARAMS((Widget parent));  
static void CreateFileWindow  __PARAMS((Widget parent));  
**/
static void CreateMessageWindow  __PARAMS((Widget parent));  

static Widget 	fileLabel,		/* filename of displayed text */
		lineLabel;		/* line number of caret position */

/*
 *  Private routines for creating various subwindows for xdbx.
 */
/**
static void CreateFileLabel(parent)
     Widget parent;
{
  Arg 	args[MAXARGS];
  Cardinal 	n;
  n = 0;
  XtSetArg(args[n], XtNlabel, (XtArgVal) "No Source File");           n++;
  XtSetArg(args[n], XtNborderWidth, (XtArgVal) 0);           		n++;
  fileLabel = XtCreateManagedWidget("fileLabel", labelWidgetClass, 
				    parent, args, n);
}
**/
/** 
static void CreateLineLabel(parent)
     Widget parent;
{
  Arg 	args[MAXARGS];
  Cardinal 	n;
  
  n = 0;
  XtSetArg(args[n], XtNlabel, (XtArgVal) "");           		n++;
  XtSetArg(args[n], XtNborderWidth, (XtArgVal) 0);           		n++;
  XtSetArg(args[n], XtNfromHoriz, (XtArgVal) fileLabel);          	n++;
  XtSetArg(args[n], XtNhorizDistance, (XtArgVal) 0);          	n++;
  lineLabel = XtCreateManagedWidget("lineLabel", labelWidgetClass, 
				    parent, args, n);
}
**/
/**** 
static void CreateFileWindow(parent)
     Widget parent;
{
    Arg 	args[MAXARGS];
    Cardinal 	n;
    n = 0;
    XtSetArg(args[n], XtNshowGrip, (XtArgVal) False);			n++;
    fileWindow = XtCreateManagedWidget("fileWindow", formWidgetClass, 
				       parent, args, n);
    CreateFileLabel(fileWindow);
    CreateLineLabel(fileWindow);
}
**/


static void CreateMessageWindow(parent)
Widget parent;
{
    Arg 	args[MAXARGS];
    Widget      fm;
    Cardinal 	n=0;
    fm = XtCreateManagedWidget("msWindow",formWidgetClass,
					  parent, args, n);
    n = 0;
    XtSetArg(args[n], XtNlabel, (XtArgVal) VERSION); n++;
    XtCreateManagedWidget("logoWindow",labelWidgetClass,
					  fm, args, n);
    XtSetArg(args[n], XtNlabel, (XtArgVal) DEFAULT_MES); n++;
    messageWindow = XtCreateManagedWidget("messageWindow",labelWidgetClass,
					  fm, args, n);
}

void DefaultMessageWindow()
{
    Arg 	args[MAXARGS];
    Cardinal 	n=0;
    XtSetArg(args[n], XtNlabel,(XtArgVal) DEFAULT_MES);	n++;
    XtSetValues(messageWindow, args, n);
}



/*  PUBLIC ROUTINES */
/*
 *  Top level function for creating all the xdbx subwindows.
 */

extern WidgetClass xtermWidgetClass;

XtermWidget CreateSubWindows(parent)
Widget parent;
{
  Arg 	args[1];
  Cardinal n=0;
  XtermWidget CreateTermWindow();
  Widget	vpane;	 /* outer widget containing various subwindows */
  vpane = XtCreateManagedWidget("vpane", panedWidgetClass, parent, args, n);
  AddAcceptMessage(parent);
  CreateMessageWindow(vpane);
  CreateCommandPanel(vpane);
  return(CreateTermWindow(vpane));
} 

/******************
 * icon 
 *****************/

#include "x_xbas.icon.X" 

void set_scilab_icon()
{
  /** Arg args[MAXARGS];
  Cardinal n; **/
  XWMHints wm_hints;
  Pixmap	sci_icon;
  
  sci_icon = XCreateBitmapFromData
    (XtDisplay(realToplevel),
     RootWindow(XtDisplay(realToplevel),
		DefaultScreen(XtDisplay(realToplevel))),
     (char *) sci_bits, sci_width, sci_height);
  
  /*  n = 0;
      XtSetArg(args[n], XtNiconPixmap, sci_icon); n++;
      XtSetValues(realToplevel, args, n);*/

  wm_hints.icon_pixmap = sci_icon;
  wm_hints.flags = IconPixmapHint;
  XSetWMHints (XtDisplay(realToplevel), XtWindow(realToplevel), 
		     &wm_hints);
}


/** we want Xscilab to accept messages : NewGraphWindowMessageAtom **/

Atom   NewGraphWindowMessageAtom;

XtEventHandler UseMessage(w, child, e)
     Widget w;
     Widget child;
     XClientMessageEvent *e;
{
  if (e->type == ClientMessage 
      && e->message_type == NewGraphWindowMessageAtom)
      {
	/*
	  fprintf(stderr,"It was a Client Message Of the Good Type \n");
	  fprintf(stderr,"I Need to Create Window %d",e->data.l[0]);
	*/
      }
  return(0);
}

static Widget WidgetUseMessage ;

void AddAcceptMessage(parent)
     Widget parent;
{
  WidgetUseMessage=parent ;
  XtAddEventHandler(parent,ClientMessage, True, (XtEventHandler) UseMessage,(XtPointer) 0);  
}

void ReAcceptMessage()
{
  XtAddEventHandler(WidgetUseMessage,ClientMessage, True, (XtEventHandler) UseMessage,(XtPointer) 0);  
}

XtermWidget CreateTermWindow(parent)
Widget parent;
{
  XtermWidget term1;
  Arg 	args[MAXARGS];
  Cardinal 	n=0;
  /** 
    mis en ressources : jpc 1994
    XtSetArg(args[n], XtNmin, (XtArgVal) 200);n++;
    **/
  term1 = (XtermWidget) XtCreateManagedWidget("Vtsci", xtermWidgetClass, parent, args,n);
  return(term1);
}

/*
 *  Routines for updating fields for the filename and line number
 *  in the file window, and the execution status in the message window.
 */

void UpdateFileLabel(string)
char *string;
{
    Arg 	args[MAXARGS];
    Cardinal 	n;
    n = 0;
    XtSetArg(args[n], XtNlabel, (XtArgVal) string);        		n++;
    XtSetValues(fileLabel, args, n);
}

void UpdateLineLabel(line)
Cardinal line;
{
    Arg 	args[MAXARGS];
    Cardinal 	n;
    char 	string[10];
    n = 0;
    if (line > n)
    	sprintf(string, "%d", line);
    else
	strcpy(string, "");
    XtSetArg(args[n], XtNlabel, (XtArgVal) string);        	n++;
    XtSetValues(lineLabel, args, n);
}

void UpdateMessageWindow(format, arg)
char *format, *arg;
{
    char 	message[LINESIZ], string[LINESIZ];
    Arg 	args[MAXARGS];
    Cardinal 	n;

    strcpy(message, "  ");
    sprintf(string, format, arg);
    strcat(message, string);
    n = 0;
    XtSetArg(args[n], XtNlabel, (XtArgVal) message);		n++;
    XtSetValues(messageWindow, args, n);
}
