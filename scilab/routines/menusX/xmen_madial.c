/* Copyright ENPC */
#include "men_scilab.h"

extern void ShellFormCreate(char *shellname, Widget *shell, Widget *form, Display **dpyh);
int MatrixDialogWindow(void);

extern MADialog MAD;

/*************************************************     
 * OK Callback 
 **********************************************************/

static Widget  *dialoglist,*labellistH, *labellistV;
static int ok_Flag_sci;

static XtCallbackProc matDialogOk(Widget w, int nv, caddr_t callData)
{ int ind,i,ns;
  Arg args[2];
  Cardinal iargs;
  String str1;
  for (i=0;i<nv;i++) 
    {
      iargs=0;
      XtSetArg(args[iargs], XtNstring, &str1); iargs++ ;
      XtGetValues(dialoglist[i],args,iargs);
      ns=strlen(str1);
      if ( ns != strlen( MAD.data[i]))
	{
	  char *p;
	  p = (char *) realloc((char*) MAD.data[i], (ns+1)*sizeof(char));
	  if ( p == (char *) 0)
	    {
	      MAD.ierr=1;
	      return(0);
	    }
	  else 
	    {
	      MAD.data[i] = p ;
	    }
	}
      strcpy(MAD.data[i] ,str1);
      ind = ns - 1 ;
      if ( MAD.data[i][ind] == '\n' ) MAD.data[i][ind] = '\0' ;
    }
  FREE( dialoglist);
  FREE( labellistH);
  FREE( labellistV);
  ok_Flag_sci = 1;
  return(0);
}

/*************************************************     
 * cancel Callback 
 **********************************************************/

static XtCallbackProc 
matDialogCancel(Widget w, int nv, caddr_t callData)
{ 
  matDialogOk(w,nv,callData);
  ok_Flag_sci = -1;
  return(0);
}

/*************************************************     
 * Widget creation 
 **********************************************************/

#define MAXLINES 16
#define MAXWIDTH 900

int MatrixDialogWindow(void)
{
    Dimension j_width,j_height;
    Arg args[12];
    Cardinal iargs=0;
    Dimension laij_width,laij_height;
    /* A revoir ce 500 inutile JPC */
    int i,j,mxdesc,smxini,mxini[500],siz, nv ;
    Widget shell,dform,label,rowi,ij,ij0,ok,cancel,lviewport,dviewport,form1,cform;
    static Display *dpy = (Display *) NULL;
    nv = MAD.nl*(MAD.nc);

    ShellFormCreate("mtdialogShell",&shell,&dform,&dpy);

    /* Create a Viewport+Label and resize it */
    
    ViewpLabelCreate(dform,&label,&lviewport,MAD.labels);

    iargs=0;
    dviewport = XtCreateManagedWidget("dViewport",viewportWidgetClass,
				      dform, args, iargs);
    iargs=0;
    form1 = XtCreateManagedWidget("form",formWidgetClass,
					  dviewport , args, iargs);

    /* Allocate an array of Widgets */
    dialoglist=(Widget *)MALLOC( (nv)*sizeof(Widget));
    labellistH=(Widget *)MALLOC( (MAD.nc)*sizeof(Widget));
    labellistV=(Widget *)MALLOC( (MAD.nl)*sizeof(Widget));
    if ( dialoglist == (Widget *) 0 || labellistH == (Widget *) 0 
	 || labellistV == (Widget *) 0) 
      {
	/** Warning : ierr is not realy used up to now 
	  ierr=1; **/
	return(FALSE);
      } 

    /* maximum sizes */

    mxdesc=0;
    for (i=0 ; i<MAD.nl ; i++)
      {
        siz=strlen(MAD.VDesc[i]);mxdesc=Max(siz,mxdesc);
      }
    smxini=0;
    for (j=0 ; j<MAD.nc ; j++)
      {
        siz=strlen(MAD.HDesc[j]);
        mxini[j]=siz;
        for (i=0 ; i<MAD.nl ; i++)
          {
            siz=strlen(MAD.data[i+j*(MAD.nl)]);  mxini[j]=Max(siz,mxini[j]);
          }
        smxini=smxini+mxini[j];
      }

    /* The first row : a set of labels */

    iargs=0;
    rowi=XtCreateManagedWidget("rowi",formWidgetClass,form1,args,iargs);
    iargs=0;
    XtSetArg(args[iargs], XtNborderWidth,(Dimension) 0) ; iargs++;
    ij0=ij=XtCreateManagedWidget("label",labelWidgetClass,rowi,args,iargs);

    for (j=0 ; j<MAD.nc ; j++)
      {
	iargs=0;
	XtSetArg(args[iargs], XtNfromHoriz,ij);iargs++;
	ij=labellistH[j]=XtCreateManagedWidget("label",labelWidgetClass,rowi,args,iargs);
      }

    /* The other rows */

    for (i=0 ; i<MAD.nl ; i++) 
      {
	iargs=0;
	XtSetArg(args[iargs], XtNfromVert , rowi) ; iargs++;
	rowi=XtCreateManagedWidget("rowi",formWidgetClass,form1,args,iargs);
	iargs=0;
	ij=labellistV[i]=XtCreateManagedWidget("label",labelWidgetClass,rowi,args,iargs);
	for (j=0 ; j<MAD.nc ; j++)
	  {
	    iargs=0;
	    XtSetArg(args[iargs], XtNfromHoriz ,ij) ; iargs++;
	    ij=XtCreateManagedWidget("ascii",asciiTextWidgetClass, rowi,args,iargs);
	    dialoglist[j*(MAD.nl)+i]=ij;
	  }
      }
   
    /* resize `matrix' widget and fix init values */

    LabelSize(ij0,mxdesc+1,1,&laij_width,&laij_height);
    
    for (j=0 ; j<MAD.nc ; j++)
      {
	AsciiSize(dialoglist[0],mxini[j],1,&j_width,&j_height);
	j_width=Max(j_width,laij_width);
	j_height=Max(j_height,laij_height);
	SetLabel(labellistH[j],MAD.HDesc[j],j_width,j_height);
	for (i=0 ; i<MAD.nl ; i++) 
	  SetAscii(dialoglist[j*(MAD.nl)+i],MAD.data[j*(MAD.nl)+i],j_width,j_height);
      }
    SetLabel(ij0," ",laij_width,j_height);
    for (i=0 ; i<MAD.nl ; i++) 
      SetLabel(labellistV[i],MAD.VDesc[i],laij_width,j_height);

    /* The buttons */
    
    iargs=0;
    cform = XtCreateManagedWidget("cform",formWidgetClass,dform,args,iargs);

    ButtonCreate(cform,&ok,(XtCallbackProc)matDialogOk,(XtPointer) nv,
		 "Ok","ok");
    ButtonCreate(cform,&cancel,(XtCallbackProc)matDialogCancel,(XtPointer) nv,
		 "Cancel","cancel");

    XtMyLoop(shell,dpy,0,&ok_Flag_sci);
    if (ok_Flag_sci == -1) 
      return(FALSE);
    else
      return(TRUE);
}

