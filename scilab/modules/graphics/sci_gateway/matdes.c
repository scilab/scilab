/*-----------------------------------------------------------------------------------*/
/* Graphic interface */
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY 2003 INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "bcg.h"
#include "stack-c.h"
#include "math_graphics.h"
#include "Graphics.h"
#include "PloEch.h"
#include "CloneObjects.h"
#include "Interaction.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"
/* Constructors should NOT be called at this level (not inside matdes.c) */
#include "BuildObjects.h"
#include "DestroyObjects.h"
#include "intcommongraphics.h"
#include "GetCommandArg.h"
#include "DefaultCommandArg.h"
#include "sci_demo.h"
#include "XsetXgetParameters.h"
#include "Xcall1.h"
#include "gsort.h"


#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/

extern int C2F(dsort) _PARAMS((double *count, int *n, int *index));
extern int IsAScalar(int RhsNumber);
extern int C2F(deletewin)(integer *number);
#if _MSC_VER
extern int Interface_XS2BMP(int figurenum,char *filename); /* wgraph.c */
extern int Interface_XS2EMF(int figurenum,char *filename);
#endif
extern void setposfig(integer *i,integer *j);
extern void Objdrawaxis  _PARAMS((char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int subint,char *format,int font,int textcol,int ticscol,char flag,int seg,int nb_tics_labels));
extern void Xdrawaxis  _PARAMS((char dir,char tics,double *x,int *nx,double *y,int *ny,char **val,int sub_int,char *format,int fontsize,int textcolor,int ticscolor,char flag,int seg_flag));
extern void Objfec  _PARAMS((double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax,integer *ExtremesCol, BOOL with_mesh, BOOL flagNax));
extern void Objnumb _PARAMS((char *fname,unsigned long fname_len,integer n,integer flag,double *x,double *y,double *angle,double *box));
extern void Xfec  _PARAMS((double *x,double *y,double *noeud,double *fun,integer *n,integer *m,char *strflag,char *legend,double *brect,integer *aaint,double *Zminmax,integer *Colminmax,integer *ColOut, BOOL with_mesh));
extern void Xfpoly  _PARAMS((integer rempl,integer closed,double *x,double *y));
extern void Xfpolys  _PARAMS((int *vect,integer val1,integer val2,integer n,integer m,double *x,double *y));
extern void Xnumb  _PARAMS((char *fname,unsigned long fname_len,integer flag,double *x,double *y,double *angle,double *box));
extern void Objfpoly  _PARAMS((double *x,double *y,integer rempl,integer *closed,long *hdl, int v1));
extern void seteventhandler(int *win_num,char *name,int *ierr);
extern void ShowScales(void);
extern void Xpolys  _PARAMS((integer *vect,integer n,integer m,double *x,double *y));
extern void Xrect  _PARAMS((char *fname,unsigned long fname_len,double *x,double *y,double *width,double *height));
extern void Objrect  _PARAMS((double *x,double *y,double *width,double *height,int *foreground,int *background,BOOL isfilled,BOOL isline,int n,long *hdl,BOOL flagxtring));
extern int zoom_box(double *bbox,int *x_pixel, int *y_pixel);
extern void unzoom_one_axes(sciPointObj *psousfen);
extern void Xrects  _PARAMS((char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2));
extern void Xstring  _PARAMS((char *fname,unsigned long fname_len,integer str,double x,double y,double angle,double *box));
extern void Xtitle  _PARAMS((char *str,int n));
extern void Objstring _PARAMS((char ** fname,int nbRow,int nbCol,double x,double y,
			       double *angle,double *box, BOOL autoSize, double* userSize, long *hdl, BOOL centerPos,
			       int *foreground,int *background,BOOL isboxed,BOOL isline,
                               BOOL isfilled, sciTextAlignment alignment));
extern void Xpoly  _PARAMS((char *fname,unsigned long fname_len,int n,int close,double *x,double *y));
extern void Objpoly  _PARAMS((double *x,double *y,integer n,integer closed,int mark,long *hdl));
extern void Xsegs  _PARAMS((integer *style,integer flag,integer n,double *x,double *y, double arsize));
extern void Objsegs  _PARAMS((integer *style,integer flag,integer n,double *x,double *y, double arsize));
extern void Xfarcs  _PARAMS((char *fname,unsigned long fname_len,int *vect1,int n,double *vect2));
extern void Xarc  _PARAMS((char *fname,unsigned long fname_len,int *a1,int *a2,double *x,double *y,double *width,double *height));
extern void Xarcs  _PARAMS((char *fname,unsigned long fname_len,int *vect1,integer n,double *vect2));
extern void Objmatplot1  _PARAMS((double *z,integer *n1,integer *n2,double *xrect));
extern void Xgrayplot  _PARAMS((double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
extern void Objarc  _PARAMS((int *angle1,int *angle2,double *x,double *y,double *width,double *height,int *foreground,int *background, BOOL isfilled, BOOL isline, long *hdl));
extern void Xmatplot1  _PARAMS((double *z,integer *n1,integer *n2,double *xrect) );
extern void Xmatplot  _PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint));
extern void Objmatplot  _PARAMS((double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint, BOOL flagNax));
extern void Objgrayplot  _PARAMS((double *x,double *y,double *z,integer *n1,integer *n2,char *strflag,double *brect,integer *aaint, BOOL flagNax));
extern void Xplot3d  _PARAMS((char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,integer *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox));
extern void Objplot3d  _PARAMS((char *fname,integer *isfac,integer *izcol,double *x,double *y,double *z,double *zcol,integer *m,integer *n,double *theta,double *alpha,char *legend,integer *iflag,double *ebox, integer *m1, integer *n1, integer *m2, integer *n2, integer *m3, integer *n3,integer *m3n, integer *n3n));
extern void Xplot2d  _PARAMS((double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint));
extern void Objplot2d  _PARAMS((int ptype,char *logflags,double *x,double *y,integer *n1,integer *n2,integer *style,char *strflag,char *legend,double *brect,integer *aaint, BOOL flagNax));
/*-----------------------------------------------------------------------------------*/
int sciseteventhandler(char *fname,unsigned long fname_len);
int C2F(scigerase)();
int scicontour2d_G(char *fname,int (*func) (),unsigned long fname_len);
int sciplot2d1_G(char *fname, int ptype,
		 int (*func) __PARAMS((char *,double *,double *,integer *,integer *,
				       integer *,char *,char *,double *,integer *,
				       integer,integer,integer)),unsigned long fname_len);
int sciplot3d_G(char *fname,
		int (*func) __PARAMS((double *,double *,double *,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func1)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func2) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func3) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		unsigned long fname_len);
int scichamp_G(char *fname,int (*func) (),unsigned long fname_len);
int scisetposfig(char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/
char *pmodes[] =
  { 
    "clear", 
    "and" ,
    "andReverse" ,
    "copy", 
    "andInverted" ,
    "noop" ,
    "xor" ,
    "or" ,
    "nor" ,
    "equiv" ,
    "invert" ,
    "orReverse" ,
    "copyInverted" ,
    "orInverted" ,
    "nand" ,
    "set" ,
  };



int cf_type=1; /* used by gcf to determine if current figure is a graphic (1) or a tclsci (0) one */

static integer one = 1, zero = 0;
static char error_message[70];
static int * Style;

static double *Rect ; 

static char *Strf ; 

static char *Legend ; 

static double *Zminmax ; 

static int *Nax;
static BOOL flagNax = FALSE; /* F.Leray : to know weither or not we use Nax */

static int *Colminmax;

static int *ColOut;

static BOOL WithMesh;

static char *Logflags; 
static char logflags[3];

extern int xinitxend_flag;

extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;

/*-----------------------------------------------------------------------------------*/
/* Graphic subroutines interface */
/*-----------------------------------------------------------------------------------*/


/* ) */

/* scirects */









/*-----------------------------------------------------------------------------------*/
int addcb(char *fname,unsigned long fname_len)
{  
  unsigned long hdl;
  integer m1, n1, l1,m2, n2, l2;
  sciPointObj *pobj;

  CheckRhs(2,3);
  CheckLhs(0,1);

  /*  set or create a graphic window*/
  SciWin();
  switch(VarType(1)) 
    {
    case 1: /* first is a scalar argument so it's a legend(hdl,"str1",...)*/
      CheckRhs(3,3);
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument*/
      hdl = (unsigned long)*hstk(l1); /* on recupere le pointeur d'objet par le handle*/
      GetRhsVar(2,"c",&m1,&n1,&l1); /* Gets the command name  */
      GetRhsVar(3,"i",&m2,&n2,&l2); /* Gets the mouse event */
      break;
    case 10:/* first is a string argument so it's a sciset("str1",....)*/
      hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin(sciGetCurrentFigure())); /* Gets the figure handle value if it ones (the phather)*/
      CheckRhs(2,2);
      GetRhsVar(1,"c",&m1,&n1,&l1); /* Gets the command name    */
      GetRhsVar(2,"i",&m2,&n2,&l2); /* Gets the mouse event */
      break;
    default: 
      Scierror(999,"%s: Pad parameters\r\n",fname);
      return 0;
      break;

    }
  if ((pobj = sciGetPointerFromHandle(hdl)) != NULL )
    sciAddCallback((sciPointObj *)pobj, cstk(l1),m1*n1,*istk(l2));
  else {
    Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
    return 0;
  }
  LhsVar(1)=0;
  return 0;
}




/*-----------------------------------------------------------------------------------*/
int copy(char *fname,unsigned long fname_len)
{  
  unsigned long hdl, hdlparent;
  sciPointObj *pobj, *psubwinparenttarget, *pcopyobj;
  integer m1, n1, l1,l2;
  int numrow, numcol, outindex,lw;
  sciEntityType typ;

  CheckRhs(1,2);
  CheckLhs(0,1);

  /*  set or create a graphic window*/
  SciWin();
  lw = 1 + Top - Rhs;
  GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument*/
  if (m1!=1||n1!=1) {
    C2F(overload)(&lw,"copy",4);
    return 0;
  }

  if (Rhs == 1)
    {
      hdl = (unsigned long)*hstk(l1); /* on recupere le pointeur d'objet par le handle*/
      pobj = sciGetPointerFromHandle(hdl);
      if (pobj == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
      typ=sciGetEntityType(pobj);
      if (typ!=SCI_TEXT&&typ!=SCI_ARC&&typ!=SCI_POLYLINE&&typ!=SCI_RECTANGLE) {
	C2F(overload)(&lw,"copy",4);
	return 0;
      }
      psubwinparenttarget = sciGetParentSubwin(sciGetPointerFromHandle(hdl));
    }
  else
    {
      hdl = (unsigned long)*hstk(l1); /* on recupere le pointeur d'objet par le handle*/
      pobj = sciGetPointerFromHandle(hdl);
      if (pobj == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
      typ=sciGetEntityType(pobj);
      if (typ!=SCI_TEXT&&typ!=SCI_ARC&&typ!=SCI_POLYLINE&&typ!=SCI_RECTANGLE) {
	C2F(overload)(&lw,"copy",4);
	return 0;
      }
      GetRhsVar(2,"h",&m1,&n1,&l2); /* Gets the command name */
      hdlparent = (unsigned long)*hstk(l2); /* on recupere le pointeur d'objet par le handle*/
      psubwinparenttarget = sciGetPointerFromHandle(hdlparent);
      if ( psubwinparenttarget == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
    }
  numrow   = 1;numcol   = 1;
  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
  *hstk(outindex) = sciGetHandle(pcopyobj = sciCopyObj((sciPointObj *)pobj,(sciPointObj *)psubwinparenttarget));
  sciDrawObj((sciPointObj *)sciGetParentFigure(pcopyobj));
  LhsVar(1) = Rhs+1;
  return 0;
}




/*-----------------------------------------------------------------------------------*/
int move(char *fname,unsigned long fname_len)
{
  long hdl;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,n;
  BOOL opt;

  CheckRhs(1,3);
  /*  set or create a graphic window */
  SciWin();
  opt = FALSE;
  if (Rhs ==3) {
    GetRhsVar(3,"c",&m3,&n3,&l3);
    if (strcmp(cstk(l3),"alone") == 0) opt = TRUE;
    else {
      Scierror(999,"%s: invalid option \r\n",fname); 
      return 0;
    }
  }

  GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */    
  GetRhsVar(2,"d",&m2,&n2,&l2);
  hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */
  n=m2*n2;
  if (n != 2&&n !=3)
    { 
      Scierror(999,"%s: third argument is a vector,[x y] or [x,y,z] \r\n",fname);
      return 0;
    }
  Objmove(&hdl,stk(l2),n,opt);

  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
int glue(char *fname,unsigned long fname_len)
{
  integer numrow,numcol,l1,l2,lind,n,cx1=1,ret ;
  unsigned long hdl, parenthdl;
  long *handelsvalue;
  int outindex,i;
  sciPointObj *pobj;

  CheckRhs(1,1);
  CheckLhs(0,1);

  /*  set or create a graphic window */
  SciWin();
  GetRhsVar(1,"h",&numrow,&numcol,&l1); /* We get the scalar value if it is ones */
  n=numrow*numcol;
  CreateVar(Rhs+1,"d",&numrow,&numcol,&l2);
  CreateVar(Rhs+2,"i",&numrow,&numcol,&lind);
  if (n>1) {
    C2F(dcopy)(&n, stk(l1), &cx1, stk(l2), &cx1);
    C2F(dsort)(stk(l2),&n,istk(lind));
    for (i = 1; i < n;i++) {
      if (*stk(l2+i) == *stk(l2+i-1)) {
	Scierror(999,"%s :each handle should not appear twice\r\n",fname);
	return 0;
      }
    }
  }
  /* we must change the pobj to the Compound type */
  handelsvalue = MALLOC(n*sizeof(long));
  for (i = 0; i < n;i++)
    {
      handelsvalue[i] = (unsigned long) (hstk(l1))[i];
      pobj = sciGetPointerFromHandle(handelsvalue[i]);
      if (pobj == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
      parenthdl = (unsigned long ) sciGetHandle(sciGetParent (pobj));
      if (i == 0)
	hdl=parenthdl;
      if  (parenthdl != hdl)
	{
	  Scierror(999,"%s: Objects must have the same parent\r\n",fname);
	  return 0;
	}

    }
  ret = CheckForCompound (handelsvalue, n);
  if (ret>0) {
    Scierror(999,"%s: handle %d cannot be glued (invalid parent)\r\n",fname,ret);
    return 0;
  }
  if (ret<0) {
    Scierror(999,"%s: handle %d cannot be glued (invalid type)\r\n",fname,-ret);
    return 0;
  }
  sciSetCurrentObj ((sciPointObj *)ConstructCompound (handelsvalue, n));

  numrow = 1;
  numcol = 1;
  CreateVar(Rhs+3,"h",&numrow,&numcol,&outindex);
  hstk(outindex)[0] = sciGetHandle((sciPointObj *) sciGetCurrentObj());
  LhsVar(1) = Rhs+3;
  FREE(handelsvalue);
  return 0;
}

/*-----------------------------------------------------------------------------------*/
int unglue(char *fname,unsigned long fname_len)
{ 
  integer m1,n1,l1; 
  unsigned long hdl;
  int numrow, numcol, outindex, i;
  sciPointObj *pobj;  
  sciSons *psonstmp;

  CheckRhs(1,1);
  CheckLhs(0,1);
  /*  set or create a graphic window */
  SciWin();
  GetRhsVar(1,"h",&m1,&n1,&l1);
  hdl = (unsigned long)*hstk(l1);
  pobj = sciGetPointerFromHandle(hdl);
  if (pobj == NULL) {
    Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
    return 0;
  }
  if (sciGetEntityType (pobj) == SCI_AGREG)
    {
      psonstmp = sciGetLastSons (pobj);
      i = 0;
      psonstmp = sciGetSons((sciPointObj *) pobj);
      while ((psonstmp != (sciSons *)NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  psonstmp = psonstmp->pnext;
	  i++;
	}
      numrow   = i;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      psonstmp = sciGetSons((sciPointObj *) pobj);
      i = 0;
      while ((psonstmp != (sciSons *)NULL) && (psonstmp->pointobj != (sciPointObj *)NULL))
	{
	  hstk(outindex)[i] = sciGetHandle((sciPointObj *)psonstmp->pointobj);
	  psonstmp = psonstmp->pnext;/* psonstmp   is pointer to one son */
	  i++;
	}
      LhsVar(1) = Rhs+1;
      sciUnCompound ((sciPointObj *)pobj);
    }
  else
    {
      Scierror(999,"%s: Object must be an Compound\r\n",fname);
    }
  return 0;
}


/*-----------------------------------------------------------------------------------*/
int drawnow(char *fname,unsigned long fname_len)
{ 
  sciPointObj *pfigure = NULL;
  /*     integer m,n,l,i; */

  SciWin(); 
  CheckRhs(0,0);
  CheckLhs(0,1); 

  if (version_flag() == 0)
    {
      if (Rhs <= 0) {
	pfigure = sciGetCurrentFigure ();
	pFIGURE_FEATURE(pfigure)->auto_redraw = TRUE;

	sciDrawObj(pfigure);
	LhsVar(1) = 0;
	return 0;

      }
    }
  LhsVar(1) = 0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
int drawlater(char *fname,unsigned long fname_len)
{  
  sciPointObj *pfigure = NULL;
  /*   integer m,n,l,i; */

  SciWin(); 
  CheckRhs(0,0);
  CheckLhs(0,1); 

  if (version_flag() == 0)
    {
      if (Rhs <= 0) {
	pfigure = sciGetCurrentFigure ();
	pFIGURE_FEATURE(pfigure)->auto_redraw = FALSE;

	LhsVar(1) = 0;
	return 0;
      }
    }

  LhsVar(1) = 0;
  return 0;

}

/*-----------------------------------------------------------------------------------*/
int scixclearsubwin(char *fname,unsigned long fname_len)
{ 
  unsigned long hdl;
  sciPointObj *subwin, *tmpsubwin;
  integer m,n,l,i,numrow,numcol,outindex,j=0;

  SciWin();
  CheckRhs(0,1);
  CheckLhs(0,1);
  if (version_flag() == 0) {
    tmpsubwin = (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
    if (Rhs <= 0) { 
      sciSetdrawmode (FALSE); 
      numrow = 1;
      numcol = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      hstk(outindex)[0] = sciGetHandle((sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ()));
      LhsVar(1) = Rhs+1;
      return 0;
    }

    switch(VarType(1)) 
      {
      case 1: /* clearsubwin(handle) */
	GetRhsVar(1,"h",&m,&n,&l); 
	for (i = 0; i < n*m; i++) {
	  hdl = (unsigned long)*hstk(l+i);            /* Puts the value of the Handle to hdl */ 
	  subwin = sciGetPointerFromHandle(hdl);  
	  if (subwin == NULL) {
	    Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	    return 0;
	  } 
	  if (sciGetEntityType (subwin) != SCI_SUBWIN) {
	    Scierror(999,"%s:  handle does not reference a sub_window",fname);
	    return 0;
	  }
	  else  {
	    /*hdltab[j] = hdl; ???? SS*/
	    j++;
	    sciSetSelectedSubWin (subwin);
	    sciSetdrawmode (FALSE); 
	  }         
	} 
	break;
      case 10:/*  clearsubwin('all') */
	GetRhsVar(1,"c",&m,&n,&l);
	if (strcmp(cstk(l),"all") == 0){ 
	  sciXclear();  
	  LhsVar(1) = 0;
	  return 0;}
	else{
	  Scierror(999,"%s: 'all' or an handle on a subwindow is expected\r\n",fname);
	  return 0;}
	break; 
      default: 
	Scierror(999,"%s: 'all' or an handle on a subwindow is expected\r\n",fname);
	return 0;
	break;
      }
    sciSetSelectedSubWin (tmpsubwin); 
  }

  LhsVar(1) = 0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* erase a graphic window if necessary */
/*-----------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------*/
/*     convertion d'entier vers double */
/*     d et s peuvent en fait pointer sur le meme tableau */
/*     car la recopie est fait de n,1,-1 */
/*      implicit undefined (a-z) */
/*-----------------------------------------------------------------------------------*/
int C2F(entier2d)(integer *n,double *dx,integer *s)
{
  integer ix;
  for (ix = *n -1 ; ix >= 0; --ix) dx[ix] = (double) s[ix];
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/*     convertion de float vers double */
/*     d et s peuvent en fait pointer sur le meme tableau */
/*     car la recopie est fait de n,1,-1 */
/* Parameter adjustments */
/*-----------------------------------------------------------------------------------*/
int C2F(simple2d)(integer *n,double *dx,float *s)
{
  integer ix;
  for (ix = *n-1 ; ix >= 0; --ix)  dx[ix] = (double) s[ix];
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
int ComputeC_format(sciPointObj * pobj, char * c_format)
{
  int i,j;
  char pos;
  char xy_type;
  double *x = NULL;
  double *y = NULL;
  int *nx = NULL;
  int *ny = NULL;
  char * format = NULL;
  sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  int  xpassed = 0, ypassed = 0, Nx, Ny, x3, y3;


  if(sciGetEntityType(pobj) != SCI_AXES){
    sciprint("Error: ComputeFormat must be used with SCI_AXES objects\n");
    return -1;
  }

  /*  strcpy(pos,pAXES_FEATURE(pobj)->dir); */
  /*   strcpy(xy_type,pAXES_FEATURE (pobj)->tics); */
  pos = pAXES_FEATURE(pobj)->dir;
  xy_type = pAXES_FEATURE (pobj)->tics;
  /* Allocating space before re-copying values to not polluate the good values 
     that will be used inside Axes.c */
  if((x=MALLOC((pAXES_FEATURE (pobj)->nx)*sizeof(double)))==NULL){
    sciprint("Memory allocation failed in ComputeFormat\n");
    return -1;
  }

  if((y=MALLOC((pAXES_FEATURE (pobj)->ny)*sizeof(double)))==NULL){
    sciprint("Memory allocation failed in ComputeFormat\n");
    return -1;
  }

  if((nx=MALLOC(sizeof(int)))==NULL){
    sciprint("Memory allocation failed in ComputeFormat\n");
    return -1;
  }  
  
  if((ny=MALLOC(sizeof(int)))==NULL){
    sciprint("Memory allocation failed in ComputeFormat\n");
    return -1;
  } 

  if((format=MALLOC(5*(sizeof(char ))+1))==NULL){
    sciprint("Memory allocation failed in ComputeFormat\n");
    return -1;
  } 
  
  nx[0] = pAXES_FEATURE (pobj)->nx;
  for(i=0;i<(*nx);i++)  x[i] = pAXES_FEATURE(pobj)->vx[i];  
  
  ny[0] = pAXES_FEATURE (pobj)->ny;
  for(i=0;i<(*ny);i++)  y[i] = pAXES_FEATURE(pobj)->vy[i];

  format = pAXES_FEATURE (pobj)->format;

  /* Algo. here */
  if(xy_type == 'i') {  
    switch ( pos ) {
    case 'u' : case 'd' :  
      if(pSUBWIN_FEATURE(psubwin)->logflags[0] == 'n')
	while (x[3]>10)  x[3]=floor(x[3]/2); 
      else{
	if(x[3] > 12){ /* F.Leray arbitrary value=12 for the moment */
	  x3=(int)x[3];     /* if x[3]>12 algo is triggered to search a divisor */
	  for(j=x3-1;j>1;j--)
	    if(x3%j == 0){
	      x[3]=j; 
	      xpassed = 1;
	    }
	  if(xpassed != 1) x[3] = 1;
	}
      }
      
      break;
    case 'r' : case 'l' :
      if(pSUBWIN_FEATURE(psubwin)->logflags[1] == 'n')
	while (y[3]>10)  y[3]=floor(y[3]/2);
      else{
	if(y[3] > 12){
	  y3=(int)y[3];
	  for(j=y3-1;j>1;j--)
	    if(y3%j == 0){
	      y[3]=j;
	      ypassed = 1;
	    }
	  if(ypassed != 1) y[3] = 1;
	}
      }
    }
  }
  
      
  /** Real to Pixel values **/
  switch ( xy_type ) 
    {
    case 'v' : Nx= *nx; Ny= *ny; break;
    case 'r' :
      switch ( pos ) {
      case 'u' : case 'd' : Nx = (int) x[2]+1; break;
      case 'r' : case 'l' : Ny = (int) y[2]+1; break;
      }
      break;
    case 'i' : 
      switch ( pos ) {
      case 'u' : case 'd' : Nx = (int) x[3]+1; break; 
      case 'r' : case 'l' : Ny = (int) y[3]+1; break;
      }
      break;
    default: 
      sciprint("Sci_Axis: wrong type argument xy_type\r\n");
    }
  switch (pos ) 
    {
    case 'u' : 
    case 'd' :
      /** Horizontal axes **/
      /*   barlength =  (integer) (Cscale.WIRect1[3]/50.0); */
      /** compute a format **/
      /*   if (str == NULL && format == NULL )   */
      if (format == NULL )  
	switch (xy_type ) {
	case 'v' : ChoixFormatE1(c_format,x,Nx);break;
	case 'r' : ChoixFormatE (c_format,x[0],x[1],(x[1]-x[0])/x[2]);break;
	case 'i' : 
	  ChoixFormatE (c_format,
			(x[0] * exp10(x[2])),
			(x[1] * exp10(x[2])),
			((x[1] * exp10(x[2])) - (x[0] * exp10(x[2])))/x[3]); break; /* Adding F.Leray 06.05.04 */
	  
	}
      break;
      /** the horizontal segment **/
    case 'r' : 
    case 'l' :
      
      /** Vertical axes **/
      /*   barlength =  (integer) (Cscale.WIRect1[2]/75.0); */
      /*   if (str == NULL &&  format == NULL )   */
      if (format == NULL ) 
	switch (xy_type ) {
	case 'v' : ChoixFormatE1(c_format,y,Ny);break;
	case 'r' : ChoixFormatE(c_format,y[0],y[1],(y[1]-y[0])/y[2]);break;
	case 'i' : 
	  ChoixFormatE (c_format,
			(y[0] * exp10(y[2])),
			(y[1] * exp10(y[2])),
			((y[1] * exp10(y[2])) - (y[0] * exp10(y[2])))/y[3]); break; /* Adding F.Leray 06.05.04 */
	}
      /** the vertical segment **/
      break;
    }
  
  /* c_format should be filled now */

  FREE(x); x = NULL;
  FREE(y); y = NULL;
  FREE(nx); nx = NULL;
  FREE(ny); ny = NULL;
  FREE(format); format = NULL;
  
  return 0;
  
}


/*-----------------------------------------------------------------------------------*/
int ComputeXIntervals(sciPointObj *pobj, char xy_type, double **vector, int * N, int checkdim)
{
  int i;
  sciAxes * ppaxes = pAXES_FEATURE (pobj);
  /*   double * outvector = NULL; */
  double * val = NULL; /* reprensents ppaxes->x or ppaxes->y */
  int nval;

  int n;

  /* draw an horizontal axis : YES (horizontal axis) or NO (vertical axis) */
  BOOL ishoriz = (ppaxes->nx > ppaxes->ny)? TRUE : FALSE; 
  
  if(ishoriz == TRUE){
    val  = ppaxes->vx;
    nval = ppaxes->nx; 
  }
  else{
    val  = ppaxes->vy;
    nval = ppaxes->ny;
  }
  
  if(xy_type == 'v')
    {
      *N = n = nval;
	
      if((*vector = (double *) MALLOC(n*sizeof(double ))) == NULL){
	strcpy(error_message,"No memory left for allocating temporary tics_labels");
	return -1;
      }
	  
      for(i=0;i<n;i++)
	(*vector)[i] = val[i];
    }
  else if(xy_type == 'r')
    {
      double step = 0;
      
      *N = n = (int)val[2]+1; /* intervals number is given by  ppaxes->x or ppaxes->y */
      
      if(checkdim){
	if(nval != 3)
	  sciprint("Warning: tics_coord must be changed, xy_type is 'r' and tics_coord dimension is not 3\n");
	
	if(nval < 3){
	  sciprint("Error: tics_coord must be changed FIRST, xy_type is 'r' and tics_coord dimension < 3\n");
	  *vector = (double *) NULL;
	  return -1;
	}
      }
	  
      if((*vector = (double *) MALLOC(n*sizeof(double ))) == NULL){
	strcpy(error_message,"No memory left for allocating temporary tics_labels");
	return -1;
      }
	  
      step = (val[1] - val[0])/(n-1);
	  
      for(i=0;i<n-1;i++)
	(*vector)[i] = val[0] + i*step;
	  
      (*vector)[n-1] = val[1]; /* xmax */
	  
    }
  else if(xy_type == 'i')
    {
      double step = 0;
      
      *N = n = (int)val[3]+1;
      
      if(checkdim){
	if(nval != 4)
	  sciprint("Warning: tics_coord must be changed, xy_type is 'i' and tics_coord dimension is not 4\n");
	
	if(nval < 4){
	  sciprint("Error: tics_coord must be changed FIRST, xy_type is 'i' and tics_coord dimension < 4\n");
	  *vector = (double *) NULL;
	  return -1;
	}
      }
      
      if((*vector =(double *)  MALLOC(n*sizeof(double ))) == NULL){
	strcpy(error_message,"No memory left for allocating temporary tics_labels");
	return -1;
      }
      
      step = (val[1]*exp10(val[2]) - val[0]*exp10(val[2]))/val[3];
      
      
      for(i=0;i<n-1;i++)
	(*vector)[i] = val[0]*exp10(val[2]) + i*step;
      
      (*vector)[n-1] = val[1]*exp10(val[2]); /* xmax */
      
    }
  
  
  /*   /\* I recopy the algo. used inside Axes.c to have exactly the same strings intervals *\/ */
  /*   /\* START *\/ */
  /*   if(xy_type == 'i') {     /\* Adding F.Leray 05.03.04*\/ */
  /*     /\*F.Leray Modification on 09.03.04*\/ */
  /*     switch ( pos ) { */
  /*     case 'u' : case 'd' :   */
  /*       if(ppsubwin->logflags[0] == 'n') */
  /* 	while (x[3]>10)  x[3]=floor(x[3]/2);  */
  /*       else{ */
  /* 	if(x[3] > 12){ /\* F.Leray arbitrary value=12 for the moment *\/ */
  /* 	  x3=(int)x[3];     /\* if x[3]>12 algo is triggered to search a divisor *\/ */
  /* 	  for(j=x3-1;j>1;j--) */
  /* 	    if(x3%j == 0){ */
  /* 	      x[3]=j;  */
  /* 	      xpassed = 1; */
  /* 	    } */
  /* 	  if(xpassed != 1) x[3] = 1; */
  /* 	} */
  /*       } */
	  
  /*       break; */
  /*     case 'r' : case 'l' : */
  /*       if(ppsubwin->logflags[1] == 'n') */
  /* 	while (y[3]>10)  y[3]=floor(y[3]/2); */
  /*       else{ */
  /* 	if(y[3] > 12){ */
  /* 	  y3=(int)y[3]; */
  /* 	  for(j=y3-1;j>1;j--) */
  /* 	    if(y3%j == 0){ */
  /* 	      y[3]=j; */
  /* 	      ypassed = 1; */
  /* 	    } */
  /* 	  if(ypassed != 1) y[3] = 1; */
  /* 	} */
  /*       } */
	
  /*       break; */
  /*     } */
  /*   } */

  

  /*   /\* END *\/  */

  
  return 0;
}
/*-----------------------------------------------------------------------------------*/
void AllGraphWinDelete(void)
{
	if (version_flag() == 0) /*  New Graphic Mode */
	{
		integer iflag=0,num=0;
		int *ArrayWGraph=NULL;

		sciGetIdFigure (ArrayWGraph,&num,&iflag);
		
		if (num > 0)
		{
			int i=0;
			ArrayWGraph=(int*)MALLOC(sizeof(int)*num);

			iflag = 1;
			sciGetIdFigure (ArrayWGraph,&num,&iflag);

			for (i=0;i<num;i++)
			{
				C2F (deletewin) (&ArrayWGraph[i]);
				FREE (ArrayWGraph);
			}
			ArrayWGraph=NULL;
		}
	}
	else /* Old Graphics mode */
	{	
		integer iflag = 0, num, *ids = (integer *) 0;

		C2F (getwins) (&num, ids, &iflag);
		if (num > 0)
		{
			ids = MALLOC ((unsigned) num * sizeof (integer));
		}
		
		iflag = 1;

		if (ids != NULL)
		{
			int i;
			C2F (getwins) (&num, ids, &iflag);
			for (i = 0; i < num; i++)
			C2F (deletewin) (&ids[i]);
			FREE (ids);
		}
	}
}
/*-----------------------------------------------------------------------------------*/
/**
 * Method called for relocating handles. The input should contains a vector of handles
 * which will be relocated and a single handle which is the new parent.
 */
int sciRelocateHandle( char * fname, unsigned long fname_len )
{
  int handleCol ;
  int handleRow ;
  int nbHandle ;
  int handleStkIndex ;
  int parentCol ;
  int parentRow ;
  int parentStkIndex ;
  int outIndex ;
  int i ;
  unsigned long * handleArray = NULL ;

  /* the function should be called with relocate_handle( handle, parent_handle ) */
  CheckRhs(2,2) ;
  CheckLhs(0,1) ;

  
  GetRhsVar( 1, "h", &handleRow, &handleCol, &handleStkIndex ) ;
  nbHandle = handleRow * handleCol ;
  GetRhsVar( 2, "h", &parentRow, &parentCol, &parentStkIndex ) ;

  if ( parentCol * parentRow != 1 )
  {
    Scierror(999,"%s :Handles must be relocated under a single parent.\r\n",fname);
    return 0 ;
  }
  
  /* create an array of handles */
  handleArray = MALLOC( nbHandle * sizeof( unsigned long ) ) ;
  if ( handleArray == NULL )
  {
    Scierror(999,"Memory full, aborting operation.\r\n",fname);
    return 0 ;
  }

  for ( i = 0 ; i < nbHandle ; i++ )
  {
    handleArray[i] = (unsigned long) *hstk( handleStkIndex + i ) ;
  }
  
  if ( sciRelocateHandles( handleArray          ,
                           handleRow * handleCol,
                           (unsigned long) *hstk( parentStkIndex ) ) != 0 )
  {
    return 0 ;
  }
  FREE( handleArray ) ;
  CreateVar( Rhs + 1, "h", &handleCol, &handleRow, &outIndex ) ;
  *hstk(outIndex) = *hstk(handleStkIndex) ;
  LhsVar(1) = Rhs + 1 ;
  return 0 ;

}
/*-----------------------------------------------------------------------------------*/
/**
 * This method is called to swap two handles position in the hierarchy.
 * The input should be two single handle which will be swaped.
 */
int sciSwapHandles( char * fname, unsigned long fname_len )
{
  int firstHdlCol  ;
  int firstHdlRow  ;
  int secondHdlCol ;
  int secondHdlRow ;
  int firstHdlStkIndex  ;
  int secondHdlStkIndex ;

  CheckRhs( 2, 2 ) ;
  CheckLhs( 0, 1 ) ;

  GetRhsVar( 1, "h", &firstHdlRow, &firstHdlCol, &firstHdlStkIndex ) ;
  GetRhsVar( 2, "h", &secondHdlRow, &secondHdlCol, &secondHdlStkIndex ) ;

  if ( firstHdlRow * firstHdlCol != 1 || secondHdlRow * secondHdlCol != 1 )
  {
    Scierror(999,"%s : Routine can only swap two single handles.\r\n",fname);
    return 0 ;
  }

  /* get the two handles and swap them */
  swapHandles( (unsigned long) *hstk( firstHdlStkIndex  ),
               (unsigned long) *hstk( secondHdlStkIndex ) ) ;
  LhsVar(1) = 0 ;
  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
