/* Copyright ENPC */
#ifdef WIN32 
#include "wmen_scilab.h"
#else
#include "men_scilab.h"
#endif

extern MDialog SciMDialog;        /** used to stored the mdialog data **/

extern void ShellFormCreate(char *shellname, Widget *shell, Widget *form, Display **dpyh);
int mDialogWindow(void);

/*************************************************     
 * Ok Callback 
 **********************************************************/

extern void ShellFormCreate(char *shellname, Widget *shell, Widget *form, Display **dpyh);
static Widget  *dialoglist;
static int ok_Flag_sci;

static XtCallbackProc
mDialogOk(Widget w, int nv, caddr_t callData)
{ int ind,i,ns;
  Arg args[2];
  Cardinal iargs;
  String str1;
  for (i=0; i < nv ; i++) {
    iargs=0;
    XtSetArg(args[iargs], XtNstring, &str1); iargs++ ;
    XtGetValues(dialoglist[i],args,iargs);
    ns=strlen(str1);
    if ( ns != strlen( SciMDialog.pszName[i]))
    {
      char *p;
      p = (char *) realloc((char*) SciMDialog.pszName[i], (ns+1)*sizeof(char));
      if ( p == (char *) 0)
	{
	  SciMDialog.ierr=1;
	  return(0);
	}
      else 
	{
	  SciMDialog.pszName[i] = p ;
	}
    }
    strcpy(SciMDialog.pszName[i] ,str1);
    ind = ns - 1 ;
    if ( SciMDialog.pszName[i][ind] == '\n' ) SciMDialog.pszName[i][ind] = '\0' ;
  }
  FREE(dialoglist);
  ok_Flag_sci = 1;
  return(0);
}

/*************************************************     
 * cancel Callback 
 **********************************************************/

static XtCallbackProc 
mDialogCancel(Widget w, int nv, caddr_t callData)
{ 
  mDialogOk(w,nv,callData);
  ok_Flag_sci = -1;
  return(0);
}

/*************************************************     
 * Widgets creation 
 **********************************************************/

int mDialogWindow(void)
{
  Arg args[10];
  Cardinal iargs = 0;
  Dimension maxh1,maxw1,maxw2,maxh2,w1,w2,h1,h2;
  int i,mxdesc,mxini,siz;
  Widget shell,label,kform; 
  Widget lviewport, dviewport,dform1,dform,ok,cancel,cform;
  static Display *dpy = (Display *) NULL;

  /* Top shell with a form inside */

  ShellFormCreate("mdialogShell",&shell,&dform,&dpy);

  /* Create a Viewport+Label and resize it */

  ViewpLabelCreate(dform,&label,&lviewport,SciMDialog.labels);

  iargs=0;
  dviewport = XtCreateManagedWidget("dViewport",viewportWidgetClass,
				      dform, args, iargs);
  iargs=0;
  dform1 = XtCreateManagedWidget("form",formWidgetClass,
				      dviewport, args, iargs);

  /* Allocate an array of Widgets */
  dialoglist=(Widget *)MALLOC( (SciMDialog.nv)*sizeof(Widget));
  if ( dialoglist == (Widget *) NULL) 
    {
      Scistring("Malloc : No more place");
      /** Warning : ierr is not realy used up to now 	ierr=1; **/
      return(FALSE);
    }
  
  /* Compute common sizes for ascii and labels  */

  mxdesc=5;
  mxini=5;
  for (i=0 ; i<SciMDialog.nv ; i++) 
    {
      siz=strlen(SciMDialog.pszTitle[i]);mxdesc=Max(siz,mxdesc);
      siz=strlen(SciMDialog.pszName[i]);  mxini=Max(siz,mxini);
    }
  mxini=Min(mxini+10,60);

  /* Widgets creation */
  kform = (Widget)0;
  for (i=0 ; i<SciMDialog.nv ; i++) 
      {
	iargs=0;
	XtSetArg(args[iargs], XtNfromVert,kform);iargs++;
	kform=XtCreateManagedWidget("kForm",formWidgetClass,dform1,
				      args,iargs);
	iargs=0;
	label=XtCreateManagedWidget("kLabel",labelWidgetClass,kform,
				    args,iargs);
	iargs=0;
	XtSetArg(args[iargs], XtNfromHoriz ,label) ; iargs++;
	dialoglist[i]=XtCreateManagedWidget("kAscii",asciiTextWidgetClass,
					    kform,args,iargs);
	if (i==0) 
	  {
	    /* chaque label sera mis a (w1,h1) 
	       et chaque ascii sera mis a (w2,h2) 
	       on regarde aussi la taille global prise par 
	       un label+un ascii de 60 caracteres pour limiter ensuite 
	       la fenetre globale
	       */
	    LabelSize(label,mxdesc+1,1,&w1,&h1);
	    AsciiSize(dialoglist[0],mxini,1,&w2,&h2);
	    h1=Max(h1,h2);
	    LabelSize(label,60,1,&maxw1,&maxh1);
	    AsciiSize(dialoglist[0],60,1,&maxw2,&maxh2);
	    maxw1=Max(maxw1,maxw2);
	    maxh1=Max(maxh1,maxh2);
	  }
	SetLabel(label,SciMDialog.pszTitle[i],w1,h1);
	SetAscii(dialoglist[i],SciMDialog.pszName[i],w2,h1);
      }

  /* Buttons */
  iargs=0;
  cform = XtCreateManagedWidget("cform",formWidgetClass,dform,args,iargs);
  
  ButtonCreate(cform,&ok,(XtCallbackProc)mDialogOk,(XtPointer) SciMDialog.nv,"Ok","ok");
  ButtonCreate(cform,&cancel,(XtCallbackProc)mDialogCancel,(XtPointer) SciMDialog.nv,"Cancel","cancel");

  XtMyLoop(shell,dpy,0,&ok_Flag_sci);
  if (ok_Flag_sci == -1) 
    {
      SciMDialog.nv=0;
      return(FALSE);
    }
  else 
    {
      return(TRUE);
    }
}

