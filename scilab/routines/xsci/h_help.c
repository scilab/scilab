/* Copyright (C) 1998 Chancelier Jean-Philippe */
/*
 * help.c : The Help browser 
 *
 * George Ferguson, ferguson@cs.rochester.edu, 23 Apr 1993. 
 */

#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/List.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/Xaw/Paned.h>

#include "version.h"
#include "../sun/h_help.h"
#include "../graphics/Math.h"

extern Widget   toplevel;
extern Atom     wm_delete_window;
extern  char  *getenv();
extern Widget initColors  __PARAMS((Widget));  

/** a terminer : normalement c'est statique plus une action **/

void            popupHelpPanel();

static void     initHelpWidgets();
static void     helpCallback();
static void     helpCallback1();
static void     helpDoneAction();
static void     queryAproposAction();
static void MyXawListChange();
static int NewString();

/*
 * Data defined here 
 */
static Widget   helpShell;
static Widget   helpLabel, helpViewport, helpList, helpList1, helpViewport1;
static Widget   helpScrollbar;
static Widget   aproposText;
static Widget   aproposInfo;

static Boolean  isPoppedUp;

static XtActionsRec actionTable[] = 
{
  {"help", popupHelpPanel},
  {"help-done", helpDoneAction},
  {"query-apropos", queryAproposAction},
};

/*************************************************************
 * Help Popup 
 *************************************************************/

void initHelpActions(appContext)
     XtAppContext    appContext;
{
  XtAppAddActions(appContext, actionTable, XtNumber(actionTable));
}

static int using_menu_help =0;

void popupHelpPanel()
{
  if (isPoppedUp) 
    {
      XRaiseWindow(XtDisplay(helpShell), XtWindow(helpShell));
      return;
    }
  if (helpShell == NULL) 
    {
      if ( Help_Init() == 1) 
	{
	  sciprint("can't use man\r\n");
	  return;
	}
      initHelpWidgets();
      isPoppedUp = True;
      using_menu_help =1;
      XtPopup(helpShell, XtGrabNone);
    }
  else 
    {
      isPoppedUp = True;
      using_menu_help = 1;
      XtPopup(helpShell, XtGrabNone);
    }
}

int help_popped_status()
{
  return(using_menu_help);
}

/**********************************************
 * Creates the help popup panel 
 *********************************************/


static void helpSetHints(topW)
     Widget topW;
{
 XSizeHints		size_hints;
 size_hints.width	= 680;
 size_hints.height	= 500;
 size_hints.min_width	= 650;
 size_hints.min_height	= 300;
 size_hints.flags = /** USPosition |**/ USSize | PMinSize;
 XSetNormalHints(XtDisplay(topW),XtWindow(topW), &size_hints);
 /*
  *  Add a protocol flag indicating we wish to handle
  *  WM_DELETE_WINDOW requests (in fact we ignore it)
  */
  (void) XSetWMProtocols(XtDisplay(helpShell), XtWindow(topW), &wm_delete_window, 1);
}


static void
initHelpWidgets()
{
  Widget          form,color;
  char            buf[64];
  Arg             args[1];
  Widget toplevel;
  Display *dpy;
  int iargs =0;

  DisplayInit("",&dpy,&toplevel);
  XtSetArg(args[iargs],XtNtitle,"Scilab Help Panel");iargs++;
  helpShell = XtAppCreateShell("Xscilab","Xscihelp",topLevelShellWidgetClass,dpy,
				      args,iargs);

  color = initColors(helpShell);

  /* helpShell = XtCreatePopupShell("helpShell", topLevelShellWidgetClass,
				 toplevel, (ArgList) 0, (Cardinal) 0);
  */
  form = XtCreateManagedWidget("helpForm", formWidgetClass,
			       color, (ArgList) 0, (Cardinal) 0);
  helpLabel = XtCreateManagedWidget("helpLabel", labelWidgetClass,
				    form, (ArgList) 0, (Cardinal) 0);
  sprintf(buf, "This is help for %s", VERSION);
  if (helpLabel != NULL && buf != NULL) {
					  XtSetArg(args[0], XtNlabel, buf);
					  XtSetValues(helpLabel, args, 1);
					};
  helpViewport = XtCreateManagedWidget("helpViewport", viewportWidgetClass,
				       form, (ArgList) 0, (Cardinal) 0);
  helpList = XtCreateManagedWidget("helpList", listWidgetClass,
				   helpViewport, (ArgList) 0, (Cardinal) 0);

  MyXawListChange(helpList, helpTopicInfo, nTopicInfo, 0, True);
  XtAddCallback(helpList, XtNcallback, helpCallback, (XtPointer) NULL);
  helpViewport1 = XtCreateManagedWidget("helpViewport1", viewportWidgetClass,
					form, (ArgList) 0, (Cardinal) 0);
  helpList1 = XtCreateManagedWidget("helpList1", listWidgetClass,
				    helpViewport1, (ArgList) 0, (Cardinal) 0);

  XawListChange(helpList1, helpInfo, nInfo, 0, True);
  XtAddCallback(helpList1, XtNcallback, helpCallback1, (XtPointer) NULL);
  (void) XtCreateManagedWidget("helpDoneButton", commandWidgetClass,
			       form, (ArgList) 0, (Cardinal) 0);
  helpScrollbar = XtNameToWidget(helpViewport, "vertical");
  (void) XtCreateManagedWidget("aproposLabel", labelWidgetClass,
			       form, (ArgList) 0, (Cardinal) 0);
  aproposText = XtCreateManagedWidget("aproposText", asciiTextWidgetClass,
				      form, (ArgList) 0, (Cardinal) 0);
  aproposInfo= XtCreateManagedWidget("aproposInfo",labelWidgetClass, 
				      form, (ArgList) 0, (Cardinal) 0);

  XtRealizeWidget(helpShell);
  helpSetHints(helpShell);
}


/* meme chose mais appel r'eduit pour appel a partir de C */

void help_info(message,str1,str2)
     char *message,*str1,*str2;
{
  char buf[56];
  Arg args[1];
  Cardinal n = 0;
  sprintf(buf,"%s %s %s",message,str1,str2);
  XtSetArg(args[n], XtNlabel,buf);n++;
  XtSetValues(aproposInfo, args, n);
}



/***************************************
 * Changes the current help list to 
 * chapter i
 **************************************/

void changeHelpList(i)
     int  i;
{
  setHelpTopicInfo(i);
  if (nTopicInfo > 0)
    MyXawListChange(helpList, helpTopicInfo, nTopicInfo, 0, True);
}

/** Changes Widget List with a copy of help **/

static void MyXawListChange(w,help,ntopic,f1,f2)
     Widget w;
     char **help;
     int ntopic,f1,f2;
{
  static char **help_c=(char **)0,**help_c1;
  static int ntopic_c=0;
  if ( CopyListForWidget(&help_c1,help,ntopic,f1,f2) == 1) return;
  if ( help_c != ( char **) 0) FreeWidgetList( help_c,ntopic_c);
  help_c = help_c1;
  ntopic_c = ntopic;
  XawListChange(w,help_c,ntopic,f1,f2);
}

int CopyListForWidget( help_c,help,ntopic)
     char ***help_c,**help;
     int ntopic;
{
  int k;
  *help_c = (char **) MALLOC((ntopic + 1) * sizeof(char *));
  if ( *help_c == NULL) {
    sciprint("Not enough memory to allocate help tables\r\n");
    return(1);
  }
  for ( k = 0 ; k < ntopic ; k++)
    {
      char *str;
      int k2;
      if ( NewString(&(*help_c)[k],help[k]) == 1) 
	{
	  int j;
	  for ( j = 0 ; j < k ; j++ ) FREE((*help_c)[j]);
	  sciprint("Not enough memory to allocate help tables\r\n");
	  return(1);
	}
      str = (*help_c)[k]; 
      for ( k2 = strlen(str) ; k2 > 0 ; k2--) 
	{
	  if ( str[k2]=='@' ) str[k2] ='\0';
	}
    }
  (*help_c)[ntopic]= (char *) 0;
  return(0);
}


static int NewString(hstr,line)
     char **hstr, *line;
{
  *hstr = (char *) MALLOC((strlen(line) + 1) * (sizeof(char)));
  if ( (*hstr) == NULL)
    {
      sciprint("Not enough memory to allocate help tables\r\n");
      return(1);
    }
  strcpy(*hstr, line);
  return(0);
}

int FreeWidgetList( help_c,ntopic)
     char **help_c;
     int ntopic;
{
  int k;
  for ( k = 0 ; k < ntopic ; k++ ) FREE(help_c[k]);
  FREE(help_c);
}

/************************************************
 * Changes the state of the help popup 
 * If it exists 
 ************************************************/

void
setHelpShellState(state)
     int state;
{
  if (!isPoppedUp)
    return;
  switch (state) {
  case NormalState:
    XtMapWidget(helpShell);
    break;
  case IconicState:
    XtUnmapWidget(helpShell);
    break;
  }
}


/************************************
 * Scilab apropos function 
 ************************************/

static void SciApropos(str)
	char           *str;
{
  int status = AP.nTopic;
  if ( SetAproposTopics(str) == 1) return ; /** memory allocation **/
  if (AP.nTopic == 0) 
    {
      if ( status != 0) 
	{
	  /** the previous valid list was an apropos one we clear it **/
	  changeHelpList(1);
	  help_info("No Info on topic ",str,", back to chapter one");
	}
      else
	help_info("No Info on topic ", str,"");
      return;
    }
  help_info("","","");
  MyXawListChange(helpList, AP.HelpTopic, AP.nTopic, 0, True);
}


/******************************************************************
 * Callback procedure 
 * activate the help ( xless application on the selected topic 
 *****************************************************************/

static void
helpCallback(w, client_data, call_data)
     Widget          w;
     XtPointer       client_data;	/* not used */
     XtPointer       call_data;	/* returnStruct */
{
  int             ntopic = ((XawListReturnStruct *) call_data)->list_index;
  HelpActivate(ntopic) ;
}

static void
helpCallback1(w, client_data, call_data)
	Widget          w;
	XtPointer       client_data;	/* not used */
	XtPointer       call_data;	/* returnStruct */
{
  int             topic = ((XawListReturnStruct *) call_data)->list_index;
  changeHelpList(topic + 1);
}


/****************************************************
 * Action procedures 
 ****************************************************/

static void helpDoneAction(w,event,params,num_params) 
     Widget w; XEvent *event; String *params; Cardinal *num_params;
{
  XtPopdown(helpShell);
  isPoppedUp = False;
}

char           *
getWidgetString(widget)
	Widget          widget;
{
  Arg             args[1];
  char           *s;
  XtSetArg(args[0], XtNstring, &s);
  XtGetValues(widget, args, 1);
  return (s);
}

static void queryAproposAction(w,event,params,num_params) 
     Widget w; XEvent *event; String *params; Cardinal *num_params;
{
  char           *apropos;
  if ((apropos = getWidgetString(aproposText)) == NULL || *apropos == '\0') 
    {
      help_info("Apropos string is empty ","","");
      return;
    } 
  else 
    {
      SciApropos(apropos);
    }
}

