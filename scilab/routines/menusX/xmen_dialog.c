/* Copyright ENPC */
#include "men_scilab.h"

/*******************************************************
 * XWindow part for dialog 
 *******************************************************/

extern void ShellFormCreate(char *shellname, Widget *shell, Widget *form, Display **dpyh);
extern void C2F(cvstr)();
static  int ok_Flag_sci; 
int DialogWindow(void);

extern char *dialog_str ;
extern SciDialog ScilabDialog;

static XtCallbackProc DialogOk(Widget w, XtPointer client_data, XtPointer call_data);
static XtCallbackProc DialogCancel(Widget w, XtPointer client_data, XtPointer callData);


/**********************************************************
 * The dialog command OK callback 
 **********************************************************/

static XtCallbackProc DialogOk(Widget w, XtPointer client_data, XtPointer call_data)
{ 
  Arg args[1];
  Cardinal iargs=0;
  Widget dialog = (Widget) client_data;
  char *lstr;
  iargs=0;
  XtSetArg(args[iargs], XtNstring, &lstr);iargs++;
  XtGetValues( dialog, args, iargs);
  dialog_str=(char *) MALLOC( (strlen(lstr)+1)*(sizeof(char)));
  if (dialog_str != 0)
    { int ind ;
      strcpy(dialog_str,lstr);
      ind = strlen(dialog_str) - 1 ;
      if (dialog_str[ind] == '\n') dialog_str[ind] = '\0' ;
      ok_Flag_sci= 1;
    }
  else 
    {
      Scistring("Malloc : No more place");
      ok_Flag_sci= -1;
    }
  return(0);

}

/**********************************************************
 * The cancel command callback 
 **********************************************************/

static XtCallbackProc   DialogCancel(Widget w, XtPointer client_data, XtPointer callData)
{ 
  ok_Flag_sci = -1;
  return(0);
}

/**********************************************************
 * Dialog Widget creation 
 **********************************************************/

int  DialogWindow(void)
{
  Dimension height,top,bot;
  int lines =1 ;
  char *p;
  Arg args[10];
  Cardinal iargs = 0;
  Widget shell,dialog,dialogpanned,label,okbutton,wid,labelviewport,cform;
  static Display *dpy = (Display *) NULL;
  
  ShellFormCreate("dialogShell",&shell,&dialogpanned,&dpy);
  
  /* Create a Viewport+Label and resize it */
  
  ViewpLabelCreate(dialogpanned,&label,&labelviewport,ScilabDialog.description);
  
  iargs=0;
  XtSetArg(args[iargs], XtNstring ,ScilabDialog.init) ; iargs++;
  dialog = XtCreateManagedWidget("ascii",asciiTextWidgetClass,dialogpanned, args, iargs);
  
  /** Changing the height of the widget **/
  p= ScilabDialog.init;
  while ( *p != '\0' ) { if ( *p == '\n' ) lines++;p++;};

  iargs=0;
  XtSetArg(args[iargs], XtNheight ,&height) ; iargs++;
  XtSetArg(args[iargs], XtNtopMargin ,&top) ; iargs++;
  XtSetArg(args[iargs], XtNbottomMargin ,&bot) ; iargs++;
  XtGetValues(dialog,args,iargs);  
  height -= (top+bot);
  height *= lines;
  height += (top+bot);
  iargs=0;
  XtSetArg(args[iargs], XtNheight ,height) ; iargs++;
  XtSetValues(dialog,args,iargs);  


  iargs=0;
  cform = XtCreateManagedWidget("cform",formWidgetClass,dialogpanned,args,iargs);
  
  ButtonCreate(cform,&okbutton,(XtCallbackProc)DialogOk,
	       (XtPointer) dialog,ScilabDialog.pButName[0],"ok");
  ButtonCreate(cform,&wid,(XtCallbackProc)DialogCancel,
	       (XtPointer) NULL,ScilabDialog.pButName[1],"cancel");
  
  XtMyLoop(shell,dpy,0,&ok_Flag_sci);
  if (ok_Flag_sci == -1)
    {
      return(FALSE);
    }
  else
    {
      return(TRUE);
    }
}



