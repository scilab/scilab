/* Copyright ENPC */
#include "men_scilab.h"

/*******************************************************
 * XWindow part for choose 
 *******************************************************/

extern void ShellFormCreate();
static  int numChoix ;
static  void CancelChoose();
static  void DoChoose();

static int ok_Flag_sci;

#define CHOOSE 1
#define CANCEL 2

/* The cancel command callback */

static void CancelChoose(w,shell,callData)
     Widget w;
     caddr_t shell,callData;
{
  ok_Flag_sci = CANCEL;
}

/* The choose command callback */

static void DoChoose(widget,shell,callData)
     Widget widget;
     caddr_t shell,callData;
{
  XawListReturnStruct* item;
  item = (XawListReturnStruct*)callData;
  numChoix = item->list_index ;
  ok_Flag_sci = CHOOSE;
}

int ExposeChooseWindow(PCh)
     ChooseMenu *PCh;
{
  Widget choosepanned,wid,list,shell,chooseviewport,chooselabel,labelviewport,cform;
  Arg args[10];
  Cardinal iargs = 0;
  static Display *dpy = (Display *) NULL;

  ShellFormCreate("chooseShell",&shell,&choosepanned,&dpy);
  
  /* Create a Viewport+Label and resize it */

  ViewpLabelCreate(choosepanned,&chooselabel,&labelviewport,PCh->description);

  /* Create a Viewport+List and resize it */
  
  ViewpListCreate(choosepanned,&list,&chooseviewport,PCh->strings,
		  PCh->nstrings);

  XtAddCallback(list, XtNcallback,(XtCallbackProc)DoChoose ,(XtPointer) NULL); 

  /* Create a button */

  iargs=0;
  cform = XtCreateManagedWidget("cform",formWidgetClass,choosepanned,args,iargs);

  ButtonCreate(cform,&wid,(XtCallbackProc) CancelChoose,(XtPointer) NULL,
	       PCh->buttonname[0],"cancel");

  /* X11 Loop */

  XtMyLoop(shell,dpy,0,&ok_Flag_sci);
  if (   ok_Flag_sci ==  CANCEL) 
    return(FALSE);
  else
    {
      PCh->choice =   numChoix ;
      return(TRUE);
    }
}

