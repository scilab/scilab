/* Copyright ENPC */
#include "men_scilab.h"

#define OK 1
#define CANCEL 2
#define MEMERR 3


extern PrintDial ScilabPrintD;
extern XtAppContext app_con;

static Widget filedialog=(Widget)0;
static int ok_Flag_sci;
static void     okPrint(Widget w, XEvent *event, String *params, Cardinal *num_params);
static XtActionsRec actionTable[] = 
{
  {"go-print", okPrint},
};

static void okPrint(Widget w, XEvent *event, String *params, Cardinal *num_params)
{
  SaveDlgOk(w, (caddr_t) 0 , (caddr_t) 0);
}




/******************************************
 * Print OK 
 ******************************************/

void 
PrintDlgOk(Widget w, caddr_t client_data, caddr_t callData)
{
  ok_Flag_sci = OK;
}

/******************************************
 * Save OK 
 ******************************************/

void
SaveDlgOk(Widget w, caddr_t client_data, caddr_t callData)
{
  Arg args[1];
  Cardinal iargs =0 ;
  /** Widget dialog = (Widget) client_data;**/
  int ind;
  char * str;
  iargs=0;
  XtSetArg(args[iargs], XtNstring, &str); iargs++;
  XtGetValues( filedialog, args, iargs);
  ScilabPrintD.filename= (char *) MALLOC( (strlen(str)+1)*(sizeof(char)));
  if (ScilabPrintD.filename != 0)
    {
      strcpy(ScilabPrintD.filename,str);
      ind = strlen(ScilabPrintD.filename) - 1 ;
      if (ScilabPrintD.filename[ind] == '\n') ScilabPrintD.filename[ind] = '\0' ;
      ok_Flag_sci = OK;
    }
  else 
    {
      Scistring("Malloc : No more place");
      ok_Flag_sci = MEMERR;
    }

}

/******************************************
 * Cancel Printing 
 ******************************************/

void 
PrintDlgCancel(Widget w, caddr_t client_data, caddr_t callData)
{
  ok_Flag_sci = CANCEL;
}

/******************************************
 * Activated when a printer is selected 
 ******************************************/

static void 
DoChoosePrinter(Widget widget, caddr_t client_data, caddr_t callData)
{
  XawListReturnStruct* item;
  item = (XawListReturnStruct*)callData;
  ScilabPrintD.numChoice = (item->list_index)+1 ;
}

/******************************************
 * Widgets creation 
 ******************************************/

int  ExposePrintdialogWindow(int flag, int *colored, int *orientation)
{
  int rep;
  static Display *dpy = (Display *) NULL;
  Widget shell,cancel,ok,dpanned,cform,form;
  Widget chooselist,fbox;
  Arg args[10];
  Cardinal iargs = 0;
  static char* items[] = {
    "Type",    "color",    "black&white",
    NULL,
    "Orientation",    "landscape",    "portrait",
    NULL
    };
  static int defval[]={1,0};
  static int nitems= 2;

  /* computing list of printers or file type */

  if (SetPrinterList(flag)== MEMERR) 
    return(FALSE);

  /* top level shell */

  ShellFormCreate("printShell",&shell,&dpanned,&dpy);
  XtAppAddActions(app_con, actionTable, XtNumber(actionTable));

  /* Window Label */
  iargs=0;
  form=XtCreateManagedWidget("form",formWidgetClass,dpanned,args,iargs);
  iargs = 0;
  XtSetArg(args[iargs], XtNlabel, (flag == 1) ? "Printer Selection" : "Format Selection"); iargs++;
  XtCreateManagedWidget("label",labelWidgetClass,form,args, iargs);

  iargs=0;
  XtSetArg(args[iargs], XtNlist, ScilabPrintD.PList); iargs++;
  XtSetArg(args[iargs], XtNnumberStrings, ScilabPrintD.ns); iargs++;
  chooselist=XtCreateManagedWidget("list",listWidgetClass,form,args,iargs);
  XtAddCallback(chooselist, XtNcallback,(XtCallbackProc)DoChoosePrinter , (XtPointer)0);
  XawListHighlight(chooselist,ScilabPrintD.numChoice-1);
  
  /* Toggles */

  rep = SciChoiceCreate(items,defval,nitems);
  if ( rep == 0 )
    {
      sciprint("x_choices : No more place\r\n");
      ok_Flag_sci = MEMERR;
      return(FALSE);
    }
  else if ( rep == -1 ) 
    {
      sciprint(" Can't raise the print menu : you must quit another raised menu before \r\n");
      return(FALSE);
    }
  iargs=0;
  form=XtCreateManagedWidget("choiceform",formWidgetClass,dpanned,args,iargs);
  (void) create_choices(form,(Widget) NULL,0);

  if (flag==2) 
    {
      /* create label widget */
      iargs=0;
      fbox=XtCreateManagedWidget("fileform",formWidgetClass,dpanned,args,iargs);
      XtCreateManagedWidget("filelabel",labelWidgetClass, fbox,args, iargs);
      filedialog = XtCreateManagedWidget("ascii",asciiTextWidgetClass,
				       fbox, args, iargs);
    }
  /* command widgets */

  iargs=0;
  cform = XtCreateManagedWidget("cform",formWidgetClass,dpanned,args,iargs);
  if ( flag == 1) 
    ButtonCreate(cform,&ok,(XtCallbackProc) PrintDlgOk , (XtPointer) 0,"Ok","ok");
  else 
    ButtonCreate(cform,&ok,(XtCallbackProc) SaveDlgOk, (XtPointer) filedialog,"Ok","ok");

  ButtonCreate(cform,&cancel,(XtCallbackProc) PrintDlgCancel,(XtPointer) NULL,"Cancel","cancel");

  XtMyLoop(shell,dpy,0,&ok_Flag_sci);
  
  if (   ok_Flag_sci== OK ) 
    {
      *colored=defval[0] = Everything[0]->choice.default_toggle +1 ;
      *orientation=defval[1] = Everything[1]->choice.default_toggle +1;
    }
  SciChoiceFree(nitems);
  if (   ok_Flag_sci== OK ) 
    return(TRUE);
  else
    return(FALSE);
}
