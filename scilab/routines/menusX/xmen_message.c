/* Copyright ENPC */
#include "men_scilab.h"

/*******************************************************
 * XWindow part for message 
 *******************************************************/

extern SciMess ScilabMessage;

#define OK 1
#define CANCEL 2
static int ok_Flag_sci;
extern void ShellFormCreate(char *shellname, Widget *shell, Widget *form, Display **dpyh);
static void MessageOk(Widget w, caddr_t shell, caddr_t callData);
static void MessageCancel(Widget w, caddr_t shell, caddr_t callData);
int ExposeMessageWindow(void);

static void MessageOk1(Widget w, XtPointer client_data, caddr_t callData);
int ExposeMessageWindow1(void);

/*************************************************     
 * OK Callback 
 **********************************************************/

static 
void MessageOk(Widget w, caddr_t shell, caddr_t callData)
{
  ok_Flag_sci = OK;
}

/*************************************************     
 * cancel Callback 
 **********************************************************/

static 
void MessageCancel(Widget w, caddr_t shell, caddr_t callData)
{
  ok_Flag_sci = CANCEL;
}

/*************************************************     
 * Widget Creation 
 **********************************************************/

int ExposeMessageWindow(void)
{
  Arg args[10];
  Cardinal iargs = 0;
  Widget shell,dialog,mpanned,mviewport,okbutton,cancelbutton,cform;
  static Display *dpy = (Display *) NULL;

  ShellFormCreate("messageShell",&shell,&mpanned,&dpy);

  /* Create a Viewport+Label and resize it */
  ViewpLabelCreate(mpanned,&dialog,&mviewport,ScilabMessage.string);
  /* The buttons */

  iargs=0;
  cform = XtCreateManagedWidget("cform",formWidgetClass,mpanned,args,iargs);

  ButtonCreate(cform,&okbutton,(XtCallbackProc)MessageOk,(XtPointer) NULL,
	       ScilabMessage.pButName[0],"ok");
  if ( ScilabMessage.nb == 2) 
    {
      ButtonCreate(cform,&cancelbutton,(XtCallbackProc)MessageCancel,(XtPointer) NULL,ScilabMessage.pButName[1],"cancel");
    }
  XtMyLoop(shell,dpy,0,&ok_Flag_sci);
  return(ok_Flag_sci);
}

/**********************************************************
 * modless message                  
 **********************************************************/
typedef struct {
  Widget w; 
  Display * dpy;
} MessData,*MessDataPtr;


/*************************************************     
 * OK Callback 
 **********************************************************/

static 
void MessageOk1(Widget w, XtPointer client_data, caddr_t callData)
{
  MessDataPtr datas =(MessDataPtr)client_data;
  ok_Flag_sci = OK;
  XtDestroyWidget(datas->w);  
  XFlush(datas->dpy);
  XSync(datas->dpy,0);
  FREE(datas)
}

int ExposeMessageWindow1(void)
{
  Arg args[10];
  Cardinal iargs = 0;
  Widget shell,dialog,mpanned,mviewport,okbutton,cform;
  static Display *dpy = (Display *) NULL;
  MessDataPtr Mdatas;

  Mdatas= (MessDataPtr) MALLOC(sizeof(MessData) );
  if ( Mdatas == (MessDataPtr) 0) return 0;
  ShellFormCreate("messageShell",&shell,&mpanned,&dpy);

  /* Create a Viewport+Label and resize it */
  ViewpLabelCreate(mpanned,&dialog,&mviewport,ScilabMessage.string);
  /* The buttons */

  iargs=0;
  cform = XtCreateManagedWidget("cform",formWidgetClass,mpanned,args,iargs);
  Mdatas->dpy=dpy;
  Mdatas->w=shell;

  ButtonCreate(cform,&okbutton,(XtCallbackProc)MessageOk1,(XtPointer) Mdatas,
	       ScilabMessage.pButName[0],"ok");
  XtPopup(shell,XtGrabNone); 
  XFlush(dpy);
  XSync(dpy,0);
  return 0;
}









 
