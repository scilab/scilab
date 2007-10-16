/*-----------------------------------------------------------------------------------*/
/* Graphic interface */
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY 2003 INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../graphics/bcg.h"
#include "../stack-c.h"
#include "../graphics/Math.h"
#include "../graphics/Graphics.h"
#include "../graphics/PloEch.h"
#include "../graphics/CloneObjects.h"
#include "../graphics/Interaction.h"
#include "../graphics/SetProperty.h"
#include "../graphics/GetProperty.h"
#include "../graphics/InitObjects.h"
#include "../graphics/DrawObjects.h"
/* Constructors should NOT be called at this level (not inside matdes.c) */
#include "../graphics/BuildObjects.h"
#include "../graphics/DestroyObjects.h"
#include "intcommongraphics.h"

#ifdef WIN32
#include "../os_specific/win_mem_alloc.h" /* MALLOC */
#else
#include "../os_specific/sci_mem_alloc.h" /* MALLOC */
#endif
/*-----------------------------------------------------------------------------------*/
extern void C2F(gsorts)(char **data, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsort)(int *xI, double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern void  C2F(msgs)(int *i, int *v);
extern int C2F(dsort) _PARAMS((double *count, int *n, int *index));
extern int IsAScalar(int RhsNumber);
extern int C2F(deletewin)(integer *number);
#if WIN32
extern int Interface_XS2BMP(int figurenum,char *filename); /* wgraph.c */
extern int Interface_XS2EMF(int figurenum,char *filename);
#endif
extern void setposfig(integer *i,integer *j);
extern int check_xy _PARAMS((char *fname, char dir, int mn, int xpos, int xm, int xn, long unsigned int xl, int ypos, int ym, int yn, long unsigned int yl, int *ntics));
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
extern void Objstring _PARAMS((char *fname,unsigned long fname_len,integer str,double x,double y,
			       double *angle,double *box,double* wh, long *hdl, int fill,
			       int *foreground,int *background,BOOL isboxed,BOOL isline, BOOL isfilled));
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
double  sciFindLogMinSPos(double *x, int n);
void SetTKGraphicalMode(BOOL SetTKMode);
int sci_demo (char *fname,char *code,integer *flagx);
int C2F(scigerase)();
int scixg2psofig_G(char *fname,char *dr,unsigned long fname_len,unsigned long dr_len);
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
void updateScaleIfRequired( sciPointObj * pSubWin ) ;
/*-----------------------------------------------------------------------------------*/
static int get_style(char *fname,int pos, int n1,rhs_opts opts[]);
static int get_rect(char *fname,int pos,rhs_opts opts[]);
static int get_strf(char *fname,int pos,rhs_opts opts[]);
static int get_legend(char *fname,int pos,rhs_opts opts[]);
static int get_nax(int pos,rhs_opts opts[]);
static int get_zminmax(char *fname,int pos,rhs_opts opts[]);
static int get_colminmax(char *fname,int pos,rhs_opts opts[]);
static int get_colout(char *fname,int pos,rhs_opts opts[]);
static int get_with_mesh(char *fname,int pos,rhs_opts opts[]);
static int get_logflags(char *fname,int pos,rhs_opts opts[]);
static int get_optional_double_arg();
static int get_optional_int_arg();
/*-----------------------------------------------------------------------------------*/
#define sciGetStyle(pos,n1,opts) if ( get_style(fname,pos,n1,opts) == 0) return 0;
#define GetRect(pos,opts) if ( get_rect(fname,pos,opts) == 0) return 0;
#define GetStrf(pos,opts) if ( get_strf(fname,pos,opts) == 0) return 0;
#define GetLegend(pos,opts) if ( get_legend(fname,pos,opts) == 0) return 0;
#define GetNax(pos,opts) if ( get_nax(pos,opts)==0 ) return 0;
#define GetZminmax(pos,opts) if ( get_zminmax(fname,pos,opts) == 0) return 0;
#define GetColminmax(pos,opts) if ( get_colminmax(fname,pos,opts)==0 ) return 0;
#define GetColOut(pos,opts) if ( get_colout(fname,pos,opts)==0 ) return 0;
#define GetWithMesh(pos,opts) if ( get_with_mesh(fname,pos,opts)==0 ) return 0;
#define GetLogflags(pos,opts) if ( get_logflags(fname,pos,opts) == 0) return 0;
#define GetOptionalDoubleArg(pos,name,value,sz,opts) if ( get_optional_double_arg(fname,pos,name,value,sz,opts) == 0) return 0;
#define GetOptionalIntArg(pos,name,value,sz,opts) if ( get_optional_int_arg(fname,pos,name,value,sz,opts) == 0) return 0;
/*-----------------------------------------------------------------------------------*/
#ifndef NULL
#define NULL 0
#endif 
#define NUMSETFONC 38
#define DEFSTRF "081" 
#define DEFSTRFN "099" 
#define DEFLOGFLAGS "gnn" 
#define SciWin() if(C2F(sciwin)() !=0) { Scierror(999,"%s :Requested figure cannot be created \r\n",fname);return 0;  }
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

static char *KeyTab_[] = {
  "alufunction",
  "auto clear",
  "background",
  "clipoff",
  "clipping",
  "clipgrf",
  "color",
  "colormap",
  "dashes",
  "default",
  "figure",
  "font",
  "font size",
  "foreground",
  "fpf",
  "gc",
  "gccolormap",
  "hidden3d",
  "lastpattern",
  "line mode",
  "line style",
  "mark",
  "mark size",
  "old_style",
  "pattern",
  "pixmap",
  "thickness",
  "use color",
  "version",
  "viewport",
  "wdim",
  "white",
  "window",
  "wpdim",
  "wpos",
  "wresize",
  "wshow",
  "wwpc",
  " ", /* added */
};


int cf_type=1; /* used by gcf to determine if current figure is a graphic (1) or a tclsci (0) one */
int versionflag = 1; /* old mode */
static integer one = 1, zero = 0;
static BOOL TKModeON=FALSE;
static char error_message[70];
static int * Style;

static double def_rect[4]  = {0.,0.,0.0,0.0}; 
static double *Rect ; 

static char def_strf[]  = DEFSTRF;
static char *Strf ; 

static char def_legend[]  = "X@Y@Z";
static char *Legend ; 

static double def_zminmax[2]  = {0.,0.};
static double *Zminmax ; 

static int def_nax[]={2,10,2,10};
static int *Nax;
static BOOL flagNax = FALSE; /* F.Leray : to know weither or not we use Nax */

static int def_colminmax[]={0,0};
static int *Colminmax;

static int def_colout[]={-1,-1};
static int *ColOut;

static BOOL def_with_mesh=FALSE;
static BOOL WithMesh;

static char def_logflags[]  = DEFLOGFLAGS;
static char *Logflags; 
static char logflags[3];

extern int xinitxend_flag;

extern sciPointObj *pfiguremdl;
extern sciPointObj *paxesmdl;
/*-----------------------------------------------------------------------------------*/
/* Graphic subroutines interface */
/*-----------------------------------------------------------------------------------*/


/*-----------------------------------------------------------------------------------*/
/*plot2d(x,y,[style,strf,leg,rect,nax]) */
/*-----------------------------------------------------------------------------------*/
int sciplot2d(char *fname,unsigned long fname_len)
{
  static char str[]="x=(0:0.1:2*%pi)';plot2d(x,[sin(x),sin(2*x),sin(3*x)],style=[-1,-2,3],rect=[0,-2,2*%pi,2]);";

  integer m1, n1, l1, m2, n2, l2, lt;
  int test,i,j,iskip;
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;

  /* F.Leray 18.05.04 : log. case test*/
  int size_x,size_y;
  double xd[2];
  double *x1;
  char dataflag;

  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
			    {-1,"frameflag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"logflag","?",0,0,0},
			    {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,"style","?",0,0,0},
			    {-1,NULL,NULL,0,0}};
  if (Rhs == 0) 
    {
      sci_demo(fname,str,&one);
      return 0;
    }

  CheckRhs(1,9);

  iskip=0;
  if ( get_optionals(fname,opts) == 0) return 0;

  if (GetType(1)==10) {
    /* logflags */
    GetLogflags(1,opts);
    iskip=1;
  }

  if (Rhs == 1+iskip)       /** plot2d([loglags,] y); **/
    {
      if ( FirstOpt() <= Rhs) {
	sciprint("%s: misplaced optional argument, first must be at position %d\r\n",fname,3+iskip);
	Error(999); 
	return(0);
      }

      GetRhsVar(1+iskip, "d", &m2, &n2, &l2);
      /* if (m2 * n2 == 0) { LhsVar(1) = 0; return 0;} */
      CreateVar(2+iskip,"d",  &m2, &n2, &l1);
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      m1 = m2;  n1 = n2;
      for (i = 0; i < m2 ; ++i) 
	for (j = 0 ; j < n2 ;  ++j)
	  *stk( l1 + i + m2*j) = (double) i+1;
    }

  if (Rhs >= 2+iskip) {
    if ( FirstOpt() < 3+iskip) {
      sciprint("%s: misplaced optional argument, first must be at position %d\r\n", fname,3+iskip);
      Error(999); 
      return(0);
    }

    /** plot2d([loglags,] x,y,....); **/

    /* x */
    GetRhsVar(1+iskip, "d", &m1, &n1, &l1);

    /* y */
    GetRhsVar(2+iskip, "d", &m2, &n2, &l2);
    /* if (m2 * n2 == 0) {m1 = 1; n1 = 0;}  */

    test = (m1*n1 == 0)||
      ((m1 == 1 || n1 == 1) && (m2 == 1 || n2 ==1) && (m1*n1 == m2*n2))  ||
      ((m1 == m2) && (n1 == n2)) ||
      ((m1 == 1 && n1 == m2) || (n1 == 1 && m1 == m2));
    CheckDimProp(1+iskip,2+iskip,!test);

    if (m1*n1 == 0) { /* default x=1:n */
      CreateVar(Rhs+1,"d",  &m2, &n2, &lt);
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      for (i = 0; i < m2 ; ++i) 
	for (j = 0 ; j < n2 ;  ++j)
	  *stk( lt + i + m2*j) = (double) i+1;
      m1 = m2;
      n1 = n2;
      l1 = lt;
    }
    else if ((m1 == 1 || n1 == 1) && (m2 != 1 && n2 != 1) ) {
      /* a single x vector for mutiple columns for y */
      CreateVar(Rhs+1,"d",  &m2, &n2, &lt);
      for (i = 0; i < m2 ; ++i) 
	for (j = 0 ; j < n2 ;  ++j)
	  *stk( lt + i + m2*j) = *stk(l1 +i);
      m1 = m2;
      n1 = n2;
      l1 = lt;
    }
    else if ((m1 == 1 && n1 == 1) && (n2 != 1) ) {
      /* a single y row vector  for a single x */
      CreateVar(Rhs+1,"d",  &m1, &n2, &lt);
      for (j = 0 ; j < n2 ;  ++j)
	*stk( lt + j ) = *stk(l1);
      n1 = n2;
      l1 = lt;
    }
    else {
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      if (m1 == 1 && n1 > 1) { m1 = n1; n1 = 1;}
    }
  }

  if(n1 == -1 || n2 == -1 || m1 == -1 || m2 == -1)
    {
      sciprint("%s: bad argument specified in input\r\n", fname);
      Error(999); 
      return(0);
    }

  sciGetStyle(3+iskip,n1,opts);
  GetStrf(4+iskip,opts);
  GetLegend(5+iskip,opts);
  GetRect(6+iskip,opts);
  GetNax(7+iskip,opts);
  if (iskip==0) GetLogflags(8,opts);

  SciWin(); 
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1] = '7';
    if (Legend != def_legend)
      strfl[0] = '1';
    if(version_flag() != 0){
      if (Nax != def_nax) /* F.Leray 12.10.04 1. If rect does not exist, there is a pb here... */
	strfl[1] = '1';   /*                  2. Where is the link between Nax and Rect ??? */
    }
    GetOptionalIntArg(9,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(9,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }

  /* NG beg */
  if (version_flag() == 0){
    /* Make a test on log. mode : available or not depending on the bounds set by Rect arg. or xmin/xmax :
       Rect case :
       - if the min bound is strictly posivite, we can use log. mode
       - if not, send error message 
       x/y min/max case:
       - we find the first strictly positive min bound in Plo2dn.c ?? */

    switch (Strf[1])  {
    case '0': 
      /* no computation, the plot use the previous (or default) scale */
      break;
    case '1' : case '3' : case '5' : case '7':
      /* based on Rect arg */ 
      if(Rect[0] > Rect[2] || Rect[1] > Rect[3])
	{sciprint("Error:  Impossible status min > max in x or y rect data\n");return -1;}

      if(Rect[0] <= 0. && Logflags[1] =='l') /* xmin */
	{sciprint("Error: bounds on x axis must be strictly positive to use logarithmic mode\n");return -1;}

      if(Rect[1] <= 0. && Logflags[2] =='l') /* ymin */
	{sciprint("Error: bounds on y axis must be strictly positive to use logarithmic mode\n");return -1;}

      break;
    case '2' : case '4' : case '6' : case '8': case '9':
      /* computed from the x/y min/max */
      if ( (int)strlen(Logflags) < 1) dataflag='g' ; else dataflag=Logflags[0];

      switch ( dataflag ) {
      case 'e' : 
	xd[0] = 1.0; xd[1] = (double)m1;
	x1 = xd;size_x = (m1 != 0) ? 2 : 0 ;
	break; 
      case 'o' : 
	x1 = stk(l1);size_x = m1;
	break;
      case 'g' : 
      default  : 
	x1 = stk(l1);size_x = (n1*m1) ;
	break; 
      }

      if (size_x != 0) 
	if(Logflags[1] == 'l' && sciFindLogMinSPos(stk(l1),size_x) < 0.)
	  {sciprint("Error: at least one x data must be strictly positive to compute the bounds and use logarithmic mode\n");return -1;}

      size_y = (n1*m1) ;

      if (size_y != 0) 
	if(Logflags[2] == 'l' && sciFindLogMinSPos(stk(l2),size_y) < 0.)
	  {sciprint("Error: at least one y data must be strictly positive to compute the bounds and use logarithmic mode\n");return -1;}

      break;
    }

    Objplot2d (1,Logflags,stk(l1), stk(l2), &n1, &m1, Style, Strf,Legend, Rect,Nax,flagNax);
    /*sciSetCurrentObj (sciGetSelectedSubWin(sciGetCurrentFigure())); F.Leray 25.03.04 */
  } 
  else { /* NG end */
    if (Logflags != def_logflags) 
      C2F(plot2d1)(Logflags,stk(l1),stk(l2),&n1,&m1,Style,Strf,Legend,Rect,Nax,
		   4L,strlen(Strf),strlen(Legend));
    else 
      Xplot2d (stk(l1), stk(l2), &n1, &m1, Style, Strf,Legend, Rect, Nax); /* NG */
  }
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/*  contour(x,y,z,nz,[theta,alpha,leg,flag,ebox,zlev]) */
/*-----------------------------------------------------------------------------------*/
int scicontour(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4, flagx,nz ;
  static integer iflag_def[3]={2,2,3};
  integer *iflag = iflag_def;
  static double ebox_def[6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def;
  double alpha_def = 35.0, theta_def=45.0, zlev_def=0.0;
  double *alpha=&alpha_def, *theta=&theta_def, *zlev=&zlev_def;
  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
			    {-1,"ebox","?",0,0,0},
			    {-1,"flag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"theta","?",0,0,0},
			    {-1,"zlev","?",0,0,0},
			    {-1,NULL,NULL,0,0}};


  if (Rhs <= 0) {
    sci_demo(fname,"contour(1:5,1:10,rand(5,10),5);", &one);
    return 0;
  }

  CheckRhs(4,10) ;
  CheckLhs(0,1) ;

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 5) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,5);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);
  CheckVector(1,m1,n1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  CheckVector(2,m2,n2);
  GetRhsVar(3, "d", &m3, &n3, &l3)
    if (m3 * n3 == 0) {LhsVar(1)=0;  return 0;} 
  if (m3 == 1 || n3 == 1) {
    Scierror(999,"%s: third argument is a vector, expecting a matrix \r\n",fname,m3*n3);
    return 0;
  }

  CheckDimProp(1,3, m1 * n1 != m3); 
  CheckDimProp(2,3, m2 * n2 != n3); 

  GetRhsVar(4, "d", &m4, &n4, &l4);
  if (m4 * n4 == 0) {LhsVar(1)=0;  return 0;} 
  if (m4 * n4 == 1) {
    flagx = 0;  nz = Max(1,(integer) *stk(l4));
  } else {
    flagx = 1;  nz = m4 * n4;
  }
  GetOptionalDoubleArg(5,"theta",&theta,1,opts);
  GetOptionalDoubleArg(6,"alpha",&alpha,1,opts);
  GetLegend(7,opts);
  GetOptionalIntArg(8,"flag",&iflag,3,opts);
  GetOptionalDoubleArg(9,"ebox",&ebox,6,opts);
  GetOptionalDoubleArg(10,"zlev",&zlev,1,opts);
  SciWin();
  C2F(scigerase)();
  C2F(contour)(stk(l1), stk(l2), stk(l3), &m3, &n3, &flagx, &nz, stk(l4), theta, alpha,
	       Legend, iflag, ebox, zlev, bsiz);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/*  param3d(x,y,z,[theta,alpha,leg,flag,ebox]) */
/*-----------------------------------------------------------------------------------*/
int sciparam3d(char *fname,unsigned long fname_len)
{
  integer izcol, isfac;
  static double  ebox_def[6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]= {1,2,4};
  integer iflag[3], *ifl, ix1, one=1;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n = 0, n3n = 0; /* F.Leray 19.03.04*/
  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
			    {-1,"ebox","?",0,0,0},
			    {-1,"flag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"theta","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  if (Rhs <= 0) {
    sci_demo(fname,"t=0:0.1:5*%pi;param3d(sin(t),cos(t),t/10,35,45,'X@Y@Z',[2,4]);", &one);
    return 0;
  }

  CheckRhs(3,8);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 4) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,4);
    Error(999); 
    return(0);
  }

  GetRhsVar(1, "d", &m1, &n1, &l1);
  if (m1 * n1 == 0) { LhsVar(1) = 0; return 0;} 
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  CheckSameDims(1,2,m1,n1,m2,n2);
  CheckSameDims(2,3,m2,n2,m3,n3);

  SciWin();
  GetOptionalDoubleArg(4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(5,"alpha",&alpha,1,opts);
  GetLegend(6,opts);

  if (version_flag() == 0) iflag_def[1]=8;
  else iflag_def[1]=2; /* F.Leray 15.06.04 : if switching back to old graphic style */
  ifl=&(iflag_def[1]);
  GetOptionalIntArg(7,"flag",&ifl,2,opts);
  iflag[0]=iflag_def[0];iflag[1]=ifl[0];iflag[2]=ifl[1];
  GetOptionalDoubleArg(8,"ebox",&ebox,6,opts);

  C2F(scigerase)();
  ix1 = m1 * n1;

  /* NG beg */
  isfac=-1;
  izcol=0;
  if (version_flag() == 0)
    Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),(double *) NULL,&ix1,&one,theta,alpha,Legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n);/*Adding F.Leray 12.03.04 */
  else
    Xplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),(integer *) NULL,&ix1,&one,theta,alpha,Legend,iflag,ebox);

  /* NG end */
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/* param3d1(x,y,z,[theta,alpha,leg,flag,ebox]) */
/* param3d1(x,y,list(z,colors),[theta,alpha,leg,flag,ebox]) */
/*-----------------------------------------------------------------------------------*/
int sciparam3d1(char *fname,unsigned long fname_len)
{
  integer izcol, isfac;
  double *zcol=NULL;
  static double  ebox_def [6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]={1,2,4};
  integer iflag[3] , *ifl;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n = 0, n3n = 0, l3n, m3l, n3l, l3l;
  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
			    {-1,"ebox","?",0,0,0},
			    {-1,"flag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"theta","?",0,0,0},
			    {-1,NULL,NULL,0,0}};


  if (Rhs <= 0) {
    sci_demo(fname,  "t=0:0.1:5*%pi;param3d1(sin(t),cos(t),t/10,35,45,'X@Y@Z',[2,4]);", &one);
    return 0;
  }

  CheckRhs(3,8);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 4) {
    sciprint("%s: misplaced optional argument, first must be at osition %d \r\n",
	     fname,4);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);/* x */
  if (m1 == 1 && n1 > 1) {m1 = n1;n1 = 1;}

  GetRhsVar(2, "d", &m2, &n2, &l2);/* y */
  if (m2 == 1 && n2 > 1) {m2 = n2;n2 = 1;}

  if (m1 * n1 == 0) { LhsVar(1) = 0; return 0;};
  CheckSameDims(1,2,m1,n1,m2,n2); 

  switch ( VarType(3) ) 
    {
    case 1 : 
      izcol = 0;
      GetRhsVar(3, "d", &m3, &n3, &l3); /* z */
      break;
    case 15 : 
      izcol = 1;
      /* z = list(z,colors) */
      GetRhsVar(3,"l",&m3l,&n3l,&l3l);
      if ( m3l != 2 ) 
	{
	  Scierror(999,"%s: second argument has a wrong size (%d), expecting a list of size %d\r\n",
		   fname,m3l,2);
	  return 0;
	}
      GetListRhsVar(3,1,"d",&m3,&n3,&l3); /* z*/ 
      GetListRhsVar(3,2,"d",&m3n,&n3n,&l3n); /* colors */
      zcol  = stk(l3n);
      if (m3n * n3n != n3) 
	{
	  Scierror(999,"%s: third argument: color specification has wrong size, expecting %d \r\n",fname,n3);
	  return 0;
	}
      break ;
    default : 
      OverLoad(3);
      return 0;
    }
  if (m3 == 1 && n3 > 1) {m3 = n3;n3 = 1;}
  CheckSameDims(1,3,m1,n1,m3,n3); 

  GetOptionalDoubleArg(4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(5,"alpha",&alpha,1,opts);
  GetLegend(6,opts);
  if (version_flag() == 0) iflag_def[1]=8;
  else iflag_def[1]=2; /* F.Leray 15.06.04 : if switching back to old graphic style */
  ifl=&(iflag_def[1]);
  GetOptionalIntArg(7,"flag",&ifl,2,opts);
  iflag[0]=iflag_def[0];iflag[1]=ifl[0];iflag[2]=ifl[1];

  GetOptionalDoubleArg(8,"ebox",&ebox,6,opts);

  if (m1 == 1 && n1 > 1) { m1 = n1;    n1 = 1; }
  SciWin();
  C2F(scigerase)();
  /* NG beg */
  isfac=-1;
  if (version_flag() == 0)
    Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m1,&n1,theta,alpha,Legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n); /*Adding F.Leray 12.03.04*/
  else{
    integer *cvect = NULL,i;
    if((cvect=MALLOC((m3n)*(n3n)*sizeof(integer)))==NULL){
      Scierror(999,"Allocation failed for color matrix in %s\r\n",fname);
      return 0;
    }    

    for(i=0;i<m3n*n3n;i++) cvect[i] = (int) zcol[i];

    Xplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),cvect,&m3,&n3,theta,alpha,Legend,iflag,ebox);
    FREE(cvect); cvect = (integer *)NULL;
  }
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 


/*-----------------------------------------------------------------------------------*/
/* used in contourf, to extract contour points */
/*-----------------------------------------------------------------------------------*/
int scic2dex(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,l2;
  double  *hl1, *hl2;

  CheckRhs(-1,0);
  CheckLhs(1,2);

  C2F(getconts)(&hl1, &hl2, &m1, &n1);

  switch ( Lhs ) 
    {
    case 0 :
    default :
      LhsVar(1)=0; break;
    case 1 : 
      if (n1 == 0)
	{ CreateVar(1,"d", &n1, &n1, &l1); } 
      else 
	{ CreateVarFromPtr(1,"d", &m1, &n1, &hl1); } 
      LhsVar(1)=1;
      break;
    case 2 :
      if (n1 == 0)
	{
	  CreateVar(1,"d", &n1, &n1, &l1);
	  CreateVar(2,"d", &n1, &n1, &l2);
	}
      else 
	{
	  CreateVarFromPtr(1,"d", &m1, &n1, &hl1);
	  CreateVarFromPtr(2,"d", &m1, &n1, &hl2);
	}
      LhsVar(1)=1;
      LhsVar(2)=2;
    }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int draw(char *fname,unsigned long fname_len)
{ 
  unsigned long hdl;
  sciPointObj *pobj, *psubwin, *tmpsubwin;
  integer m,n,l,lw;
  /*  BOOL tmpmode; SS 20.04.04*/


  SciWin();
  CheckRhs(0,1);
  CheckLhs(0,1);
  if (version_flag() == 0) {
    if (Rhs == 0)  
      pobj =( sciPointObj *) sciGetCurrentObj();
    else {
      GetRhsVar(1,"h",&m,&n,&l); 
      if (m!=1||n!=1) { 
	lw = 1 + Top - Rhs;
	C2F(overload)(&lw,"draw",4);return 0;
      }
      hdl = (unsigned long)*hstk(l);            /* Puts the value of the Handle to hdl */ 
      pobj = sciGetPointerFromHandle(hdl);   
    }
    if (pobj != ( sciPointObj *)NULL )  {  
      tmpsubwin = (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ()); 
      psubwin = (sciPointObj *) sciGetParentSubwin(pobj);
      if (psubwin != ( sciPointObj *)NULL )  {  
	sciSetSelectedSubWin(psubwin); 
	set_scale ("tttftt", pSUBWIN_FEATURE (psubwin)->WRect, 
		   pSUBWIN_FEATURE (psubwin)->FRect,
		   NULL, pSUBWIN_FEATURE (psubwin)->logflags, 
		   pSUBWIN_FEATURE (psubwin)->ARect); 

	sciDrawObj(pobj); 
	sciSetSelectedSubWin(tmpsubwin);
      }
      else {
	Scierror(999,"%s: object has no parent !!",fname);
	return 0;
      }
    }
  }
  LhsVar(1) = 0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* [x,y]=geom3d(x1,y1,z1) */
/*-----------------------------------------------------------------------------------*/
int scigeom3d(char *fname,unsigned long fname_len)
{
  integer ix1, m1, n1, l1, m2, n2, l2, m3, n3, l3;

  if (Rhs <= 0) {
    sci_demo(fname, "t=0:0.1:5*%pi,[x,y]=geom3d(sin(t),cos(t),t/10);", &one);
    return 0;
  }

  CheckRhs(3,3);
  CheckLhs(2,3);

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  CheckSameDims(1,2,m1,n1,m2,n2);
  CheckSameDims(2,3,m2,n2,m3,n3);
  if (m1 * n1 == 0)  { LhsVar(1) = 0; return 0;}
  SciWin();
  ix1 = m1 * n1;
  C2F(geom3d)(stk(l1), stk(l2), stk(l3), &ix1);
  LhsVar(1)=1;
  LhsVar(2)=2;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/*  grayplot(x,y,z,[strf,rect,nax]) */
/*-----------------------------------------------------------------------------------*/
int scigrayplot(char *fname,unsigned long fname_len)
{
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
			    {-1,"frameflag","?",0,0,0},
			    {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  if (Rhs <= 0) {
    sci_demo(fname, "t=-%pi:0.1:%pi;m=sin(t)'*cos(t);grayplot(t,t,m);",&one);
    return 0;
  }
  CheckRhs(3,7);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 4) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,4);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);
  CheckVector(1,m1,n1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  CheckVector(2,m2,n2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  if (m3 * n3 == 0) { LhsVar(1)=0; return 0;} 

  CheckDimProp(2,3,m2 * n2 != n3);
  CheckDimProp(1,3,m1 * n1 != m3);

  GetStrf(4,opts);
  GetRect(5,opts);
  GetNax(6,opts);

  SciWin();
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1]='7';
    if(version_flag() != 0){
      if (Nax != def_nax)
	strfl[1]='1';
    }
    GetOptionalIntArg(7,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(7,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }



  /* NG beg */
  if (version_flag() == 0)
    Objgrayplot (stk(l1), stk(l2), stk(l3), &m3, &n3, Strf, Rect, Nax, flagNax);
  else /* NG end */
    Xgrayplot (stk(l1), stk(l2), stk(l3), &m3, &n3, Strf, Rect, Nax);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* scimatplot */
/*-----------------------------------------------------------------------------------*/
int scimatplot(char *fname,unsigned long fname_len)
{
  integer m1, n1, l1;
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
			    {-1,"frameflag","?",0,0,0},
			    {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  if (Rhs <= 0) {
    sci_demo(fname,"m=[1,2;3,4];Matplot(m);", &one);
    return 0;
  }
  CheckRhs(1,5);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 2) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,2);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);
  if (m1 * n1 == 0) {  LhsVar(1)=0; return 0;} 
  GetStrf(2,opts);
  GetRect(3,opts);
  GetNax(4,opts);

  SciWin();
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1]='7';
    if(version_flag() != 0){
      if (Nax != def_nax)
	strfl[1]='1';
    }
    GetOptionalIntArg(5,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(5,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }


  /* NG beg */
  if (version_flag() == 0)
    Objmatplot (stk(l1), &m1, &n1, Strf, Rect, Nax, flagNax);
  else 
    Xmatplot (stk(l1), &m1, &n1, Strf, Rect, Nax);
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* Matplot1 */
/*-----------------------------------------------------------------------------------*/
int scigray2plot(char *fname,unsigned long fname_len)
{
  integer m1, n1, l1,m2,n2,l2;
  if (Rhs <= 0) {
    sci_demo(fname,"plot2d([0,10],[0,10],0);a=ones(50,50);a= 3*tril(a)+2*a;Matplot1(a,[4,4,9,9]);", &one);
    return 0;
  }

  CheckRhs(2,2);
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  CheckLength(2,m2*n2,4);
  if (m1 * n1 == 0) {  LhsVar(1)=0; return 0;} 
  SciWin();

  /* NG beg */
  if (version_flag() == 0)
    Objmatplot1 (stk(l1), &m1, &n1,stk(l2));
  else
    Xmatplot1 (stk(l1), &m1, &n1,stk(l2)); /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* driver(driver_name) or  current_driver=driver() */
/*-----------------------------------------------------------------------------------*/
int scidriver(char *fname,unsigned long fname_len)
{
  integer m1=3,n1=1,l1,v ;
  double dv;
  CheckRhs(-1,1);
  CheckLhs(0,1);
  if (Rhs <= 0) 
    {
      CreateVar(1,"c",&m1,&n1,&l1);
      C2F(dr1)("xgetdr", cstk(l1), &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 7L, m1);
      LhsVar(1) = 1;
    } 
  else 
    {
      GetRhsVar(1,"c",&m1,&n1,&l1);
      C2F(dr1)("xsetdr", cstk(l1), &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv, 7L, m1);
      LhsVar(1)=0;
    }
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* xarcs */
/*-----------------------------------------------------------------------------------*/
int scixarc(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,l2,l3,l4,l5,l6;
  long hdl;/* NG */

  SciWin();
  CheckRhs(6,6);
  GetRhsVar(1,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
  GetRhsVar(2,"d",&m1,&n1,&l2);CheckScalar(2,m1,n1);
  GetRhsVar(3,"d",&m1,&n1,&l3);CheckScalar(3,m1,n1);
  GetRhsVar(4,"d",&m1,&n1,&l4);CheckScalar(4,m1,n1);
  GetRhsVar(5,"i",&m1,&n1,&l5);CheckScalar(5,m1,n1);
  GetRhsVar(6,"i",&m1,&n1,&l6);CheckScalar(6,m1,n1);
  /* NG beg */
  if (version_flag() == 0){
    int curcolor = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ())); /* current color equivalent for new graphics mode */
    if (strcmp(fname,"xarc")==0)
      Objarc (istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),&curcolor,NULL,FALSE,TRUE,&hdl);
    else /* xfarc case */
      Objarc (istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),NULL,&curcolor,TRUE,FALSE,&hdl);
  }
  else
    Xarc(fname,fname_len,istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4));
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* scixarcs */
/*-----------------------------------------------------------------------------------*/
int scixarcs(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2;

  /* NG beg */
  long  hdl;
  int i,a1,a2;
  /* NG end */
  SciWin();
  CheckRhs(1,2);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  if ( strcmp(fname,"xarcs") == 0) {
    if (m1 != 6) {
      Scierror(999,"%s: arcs has a wrong size (6,n) expected \r\n",fname);
      return 0;
    }
  } else {
    if (m1 != 4) {
      Scierror(999,"%s: rects has a wrong size (4,n) expected \r\n",fname);
      return 0;
    }
  }

  if (Rhs == 2) 
    {
      GetRhsVar(2,"i",&m2,&n2,&l2);
      CheckVector(2,m2,n2);
      if (m2 * n2 != n1) {
	Scierror(999,"%s: first and second arguments have incompatible length\r\n",fname);
	return 0;
      }
    }
  else 
    {
      int i;
      m2=1,n2=n1; CreateVar(2,"i",&m2,&n2,&l2);
      if (version_flag() == 0)
	for (i = 0; i < n2; ++i)  *istk(l2 + i) = 
				    sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
      else
	for (i = 0; i < n2; ++i)  *istk(l2 + i) = 0;
    }  
  /* NG beg */
  if (version_flag() == 0){ 
    for (i = 0; i < n1; ++i)
      { 
	a1=(int)(*stk(l1+(6*i)+4));
	a2=(int)(*stk(l1+(6*i)+5));
	Objarc (&a1,&a2,stk(l1+(6*i)),stk(l1+(6*i)+1),
		stk(l1+(6*i)+2),stk(l1+(6*i)+3),istk(l2+i),NULL,FALSE,TRUE,&hdl); 
      }
    /** construct Compound and make it current object **/
    sciSetCurrentObj (ConstructCompoundSeq (n1));
  }   
  else
    Xarcs(fname,fname_len,istk(l2), n1,stk(l1));
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/*  xfarcs(arcs,[style]) */
/*-----------------------------------------------------------------------------------*/
int scixfarcs(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2;
  long  hdl;
  int i,a1,a2;

  SciWin();
  CheckRhs(1,2);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  if (m1 != 6) {
    Scierror(999,"%s: arcs has a wrong size (6,n) expected \r\n",fname);
    return 0;
  }

  if (Rhs == 2) {
    GetRhsVar(2,"i",&m2,&n2,&l2);
    CheckVector(2,m2,n2);
    if ( n1 != m2*n2) {
      Scierror(999,"%s: arguments have incompatible size\r\n",fname);
      return 0;
    }
  }
  else 
    {
      int i;
      m2=1,n2=n1; CreateVar(2,"i",&m2,&n2,&l2);
      for (i = 0; i < n2; ++i)  *istk(l2 + i) = i+1;
    }
  if (version_flag() == 0) {
    for (i = 0; i < n1; ++i)
      { 
	a1 = (int)(*stk(l1+(6*i)+4));
	a2 = (int)(*stk(l1+(6*i)+5));
	Objarc (&a1,&a2,stk(l1+(6*i)),stk(l1+(6*i)+1),
		stk(l1+(6*i)+2),stk(l1+(6*i)+3),istk(l2+i),istk(l2+i),TRUE,FALSE,&hdl); 
      }
    /** construct Compound and make it current object **/
    sciSetCurrentObj (ConstructCompoundSeq (n1));
  }
  else
    Xfarcs(fname,fname_len,istk(l2), n1,stk(l1));
  LhsVar(1)=0;
  return 0;

}



/*-----------------------------------------------------------------------------------*/
/* xarrows(nx,ny,[arsize,style]) */
/*-----------------------------------------------------------------------------------*/
int sciarrows(char *fname,unsigned long fname_len)
{
  integer dstyle = -1,m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3;
  integer *style,flag;
  integer m4,n4,l4,mn2;
  double arsize=-1.0;

  SciWin();
  CheckRhs(2,4);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  mn2 = m2 * n2;
  CheckSameDims(1,2,m1,n1,m2,n2);
  if (mn2 == 0) {   LhsVar(1)=0;  return 0;} 

  if (Rhs >= 3) { GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3); arsize = *stk(l3); } 

  if (Rhs >= 4) { 
    GetRhsVar(4,"i",&m4,&n4,&l4); CheckVector(4,m4,n4);
    if (m4 * n4 == 1) dstyle = *istk(l4);
    if (m4 * n4 != 1 && m2 * n2 / 2 != m4 * n4) {
      Scierror(999,"%s: style has a wrong size (%d), expecting (%d)\r\n",fname,m4*n4, m2 * n2 / 2 );
      return 0;
    }
  }
  /* NG beg */
  if (Rhs == 4 && m4 * n4 != 1) {
    style=istk(l4); flag= one;
  } 
  else {
    style=&dstyle ; flag= zero;
  }
  if (version_flag() == 0)
    Objsegs (style,flag,mn2,stk(l1),stk(l2),arsize);
  else
    Xsegs (style,flag,mn2,stk(l1),stk(l2),arsize);
  /* NG end */


  LhsVar(1)=0;
  return 0;
} 


/*-----------------------------------------------------------------------------------*/
/* xsegs(xv,yv,[style]) */
/*-----------------------------------------------------------------------------------*/
int scixsegs(char *fname,unsigned long fname_len)
{
  integer dstyle = -1, *style,flag;
  integer mn2;
  integer m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3; 
  double arsize=0;

  SciWin();

  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  if (m2*n2 == 0) { LhsVar(1)=0; return 0;} 

  if (Rhs == 3) {
    GetRhsVar(3,"i",&m3,&n3,&l3); CheckVector(3,m3,n3);
    if (m3 * n3 == 1) dstyle = *istk(l3 );
    if (m3 * n3 != 1 && m2 * n2 / 2 != m3 * n3) {
      Scierror(999,"%s: style has a wrong size (%d), expecting (%d)\r\n",fname,m3 * n3, m2 * n2 / 2);
      return 0;
    }
  }
  mn2 = m2 * n2; 

  /* NG beg */
  if (version_flag() == 0){
    sciPointObj * psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

    if (Rhs == 3 && m3 * n3 != 1) {
      style = istk(l3); flag = one;
    }
    else if (Rhs == 3 && m3 * n3 == 1) {
      style = istk(l3); flag = zero;
    }
    else { /* Rhs < 3 => no color specified, use current color (taken from axes parent) */
      int col = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
      style = &col;
      flag= zero;
    }

    Objsegs (style,flag,mn2,stk(l1),stk(l2),arsize);

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0) {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
			/*     EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(sciGetCurrentObj ()); 

  }
  else{
    if (Rhs == 3 && m3 * n3 != 1) {
      style = istk(l3); flag = one;
    } 
    else {
      style = &dstyle ; flag = zero;
    }

    Xsegs (style,flag,mn2,stk(l1),stk(l2),arsize);
  }
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 




/*-----------------------------------------------------------------------------------*/
/* old version : kept for backward compatibility */
/*-----------------------------------------------------------------------------------*/
int scixaxis(char *fname,unsigned long fname_len)
{
  integer v;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  double dv;

  CheckRhs(2,4);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);
  GetRhsVar(2,"i",&m2,&n2,&l2); CheckLength(1,m2*n2,2);
  GetRhsVar(3,"d",&m3,&n3,&l3); CheckLength(1,m3*n3,3);
  GetRhsVar(4,"d",&m4,&n4,&l4); CheckLength(1,m4*n4,2);

  SciWin();
  C2F(dr1)("xaxis","v",&v,istk(l2),&v,&v,&v,&v,stk(l1),stk(l3),stk(l4),&dv,6L,2L);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* [x1,y1,rect]=xchange(x,y,dir) */
/*-----------------------------------------------------------------------------------*/
int scixchange(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3 ,l3,l4,quatre=4,l5,i;
  CheckRhs(3,3);
  CheckLhs(1,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  GetRhsVar(3,"c",&m3,&n3,&l3);
  CheckSameDims(1,2,m1,n1,m2,n2);

  SciWin();
  if ( strcmp(cstk(l3),"i2f") == 0) 
    {
      GetRhsVar(1,"i",&m1,&n1,&l1);
      GetRhsVar(2,"i",&m2,&n2,&l2);
      CreateVar(3,"d",&m1,&n1,&l3);
      CreateVar(4,"d",&m1,&n1,&l4);
      C2F(echelle2d)(stk(l3),stk(l4),istk(l1),istk(l2),&m1,&n1,"i2f",3L);
    }
  else 
    {
      CreateVar(3,"i",&m1,&n1,&l3);
      CreateVar(4,"i",&m1,&n1,&l4);
      C2F(echelle2d)(stk(l1),stk(l2),istk(l3),istk(l4),&m1,&n1,"f2i",3L);
    }
  CreateVar(5,"d",&one,&quatre,&l5);
  for (i=0; i < quatre ; i++) *stk(l5+i) =  Cscale.WIRect1[i];
  LhsVar(1)=3;
  LhsVar(2)=4;
  LhsVar(3)=5;
  return 0;
}








/*-----------------------------------------------------------------------------------*/
/* xclea(x,y,w,h) etendu a xclea([x,y,w,h]) */
/*-----------------------------------------------------------------------------------*/
int scixclea(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  integer v;

  SciWin();
  CheckRhs(1,4);
  switch ( Rhs ) 
    {
    case 1 :
      GetRhsVar(1,"d",&m1,&n1,&l1); 
      CheckLength(1,m1*n1,4);
      C2F(dr1)(fname,"v",&v,&v,&v,&v,&v,&v,stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),fname_len,2L);      
      break;
    case 4 :
      GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);
      GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3);
      GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);
      C2F(dr1)(fname,"v",&v,&v,&v,&v,&v,&v,stk(l1),stk(l2),stk(l3),stk(l4),fname_len,2L);
      break;
    default :
      Scierror(999,"%s: wrong number of rhs argumens (%d), rhs must be 1 or 4\r\n",fname,Rhs);
    }
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/* xclear([window-ids]) */
/*-----------------------------------------------------------------------------------*/
int scixclear(char *fname,unsigned long fname_len)
{
  integer verb=0,wid,cur,win,na;
  integer ix,m1,n1,l1/*,v=0*/;
  double dv;


  CheckRhs(0,2); /* NG  */ /*CheckRhs(-1,1) */ 
  CheckLhs(0,1);
  SciWin();
  if (Rhs == 1) 
    {
      GetRhsVar(1,"d",&m1,&n1,&l1);
      C2F(dr1)("xget","window",&verb,&cur,&na,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      for (ix = 0 ; ix < m1*n1 ; ++ix) 
	{
	  wid = (integer) *stk(l1 +ix );
	  C2F(dr1)("xset","window",&wid,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
	  C2F(dr1)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,7L,2L);
	}
      C2F(dr1)("xset","window",&cur,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
    }
  else 
    {
      C2F(dr1)("xget","window",&verb,&win,&na,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr1)("xset","window",&win,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr1)("xclear","v",PI0,PI0,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,7L,2L); 
    } 
  if (version_flag() == 0) sciXclear();   /* NG */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* [c_i,c_x,c_y,c_w,c_m]=xclick([flag]) */
/*-----------------------------------------------------------------------------------*/
int scixclick(char *fname,unsigned long fname_len)
{
  int un=1,trois=3,rep,iw;
  integer istr,ix,iflag,v;
  double x,y,dv;

  CheckRhs(-1,1) ;
  CheckLhs(1,5) ;

  iflag = ( Rhs >= 1) ? 1 :0;

  SciWin();

  switch (Lhs) {
  case 4 : 
    C2F(dr1)("xclickany","xv",&ix,&iw,&iflag,&v,&v,&v,&x,&y,&dv,&dv,10L,3L);
    break;
  case 5 :
    istr = 1;
    C2F(dr1)("xclickany",C2F(cha1).buf,&ix,&iw,&iflag,&v,&v,&istr,&x,&y,&dv,&dv,10L,bsiz);
    break;
  default :
    istr = 0;
    C2F(dr1)("xclick","xv",&ix,&iflag,&istr,&v,&v,&v,&x,&y,&dv,&dv,7L,3L);
  }

  if ( ix < 0 ) {
    x=y=-1; /*return_a_nan(); */ /* to better manage string conversion of the output */ /* F.Leray 20.04.05 */
  }

  if ( Lhs == 1 )
    {
      LhsVar(1) = Rhs+1;
      CreateVar(Rhs+1,"d",&un,&trois,&rep);
      *stk(rep) = (double) ix; *stk(rep + 1) = x; *stk(rep + 2) = y;
    }
  else 
    {
      LhsVar(1) = Rhs+1; CreateVar(Rhs+1,"d",&un,&un,&rep); *stk(rep) = (double) ix ;
    }
  if ( Lhs >= 2) 
    { LhsVar(2) = Rhs+2; CreateVar(Rhs+2,"d",&un,&un,&rep); *stk(rep) = x ;} 
  if ( Lhs >= 3)
    { LhsVar(3) = Rhs+3; CreateVar(Rhs+3,"d",&un,&un,&rep); *stk(rep) = y ;}
  if ( Lhs >=4 ) 
    { LhsVar(4) = Rhs+4; CreateVar(Rhs+4,"d",&un,&un,&rep); *stk(rep) = (double) iw ;}
  if ( Lhs >= 5) 
    {
      if (istr == 0) { istr = 4; strcpy(C2F(cha1).buf,"void");}
      LhsVar(5) = Rhs+5; CreateVar(Rhs+5,"c",&istr,&un,&rep); 
      strncpy(cstk(rep),C2F(cha1).buf,istr);
    }
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xend */
/*-----------------------------------------------------------------------------------*/
int scixend(char *fname,unsigned long fname_len)
{
  integer v;
  double dv;
  sciPointObj * figure = NULL;
  struct BCG *Xgc;

  SciWin();

  CheckRhs(-1,0);
  /* printf("Type de Driver: %c\n",GetDriver());
     printf("Type d'ID     : %d\n\n",GetDriverId());
     fflush(NULL); */
  if(version_flag() == 0) {
    xinitxend_flag = 0;   /* we DO draw now into the file/memory (depending on the driver type) */
    figure = sciGetCurrentFigure();
    sciDrawObj(figure);
    /*     xinitxend_flag = 1; */
  }
  C2F(dr1)("xend","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,2L);
  if(version_flag() == 0) {
    /* figure already defined above */
    Xgc = (struct BCG *) pFIGURE_FEATURE(figure)->pScilabXgc;
    DestroyAllGraphicsSons (figure);
    DestroyFigure (figure); figure = (sciPointObj *) NULL;
    Xgc->mafigure = (sciPointObj *) NULL;
  }
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xgrid([style]) */
/*-----------------------------------------------------------------------------------*/
int scixgrid(char *fname,unsigned long fname_len)
{
  integer style = 1,m1,n1,l1;
  CheckRhs(-1,1);
  if (Rhs == 1) {
    GetRhsVar(1,"d",&m1,&n1,&l1);
    CheckScalar(1,m1,n1);
    style = (integer) *stk(l1);
  }
  SciWin();
  C2F(xgrid)(&style); /* test on version_flag() inside Plo2.c F.Leray 07.05.04 */
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/* xpoly(xv,yv,dtype,[close]) */
/*-----------------------------------------------------------------------------------*/
int scixpoly(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1 ,m2 ,n2 ,l2,m3,n3,l3,m4,n4,l4,close=0,mn2;

  long hdl;/* NG */
  int mark;/* NG */

  SciWin();

  CheckRhs(2,4);
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  mn2 = m2 * n2;
  
  if (Rhs >= 3) {
    GetRhsVar(3,"c",&m3,&n3,&l3);
    if ( strcmp(cstk(l3),"lines") == 0) {
      strcpy(C2F(cha1).buf,"xlines"); 
      mark=1; /* NG */
    } else if (strcmp(cstk(l3),"marks") == 0) {
      strcpy(C2F(cha1).buf,"xmarks"); 
      mark=0; /* NG */
    } else {
      Scierror(999,"%s:  dtype must be \"lines\" or \"marks\"\r\n",fname);
      return 0;
    } 
  }
  else {
    strcpy(C2F(cha1).buf,"xlines");
    mark=1; /* NG */
  }

  if (Rhs >= 4) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); close = (integer) *stk(l4);} 
  /* NG beg */
  if (version_flag() == 0){
    sciPointObj *pobj = NULL;
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

    Objpoly (stk(l1),stk(l2),mn2,close,mark,&hdl);

    pobj = sciGetCurrentObj(); /* the polyline newly created */
    if(mark == 0){ 
      /* marks are enabled but markstyle & foreground 
	 is determined by parents' markstyle & foreground */
      sciSetIsMark(pobj, TRUE);
      sciSetIsLine(pobj,FALSE);
      sciSetMarkStyle (pobj,sciGetMarkStyle(psubwin));
      sciSetForeground (pobj, sciGetForeground (psubwin));
    }
    else{
      sciSetIsMark(pobj, FALSE);
      sciSetIsLine(pobj, TRUE);
      sciSetLineStyle(pobj, sciGetLineStyle (psubwin));
      sciSetForeground (pobj, sciGetForeground (psubwin));
    }

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0){
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
			/*    EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} */ /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(pobj);
  }
  else
    Xpoly(C2F(cha1).buf,bsiz,mn2,close,stk(l1),stk(l2));
  /* NG end */
  LhsVar(1)=0;
  return 0;
  /*************************************************/
}

/*-----------------------------------------------------------------------------------*/
/* xpolys(xpols,ypols,[draw]) */
/*-----------------------------------------------------------------------------------*/
int scixpolys(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3;
  int i;
  long hdl;

  SciWin();
  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if ( m1 * n1 == 0 || m2 * n2 == 0 )
  {
    /* dimension 0, 0 polyline to draw */
    LhsVar(1)=0 ;
    return 0 ;
  }

  if (Rhs == 3) 
    {
      GetRhsVar(3,"i",&m3,&n3,&l3); CheckVector (3,m3,n3); CheckDimProp(1,3,m3 * n3 < n1);
    }
  else
    {
      int un=1;
      CreateVar(3,"i",&un,&n1,&l3);
      for (i = 0 ; i < n1 ; ++i) *istk(l3+i) = 1;
    } 
  if (version_flag() == 0)
  {
	sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    for (i = 0; i < n1; ++i) 
      Objpoly (stk(l1+(i*m1)),stk(l2+(i*m2)),m1,0,*istk(l3+i),&hdl);

    /** construct Compound and make it current object**/
    sciSetCurrentObj (ConstructCompoundSeq (n1));

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0){
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
			/*    EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} */ /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(sciGetCurrentObj ());
  }
  else
    Xpolys(istk(l3),n2,m2,stk(l1),stk(l2));
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xselect */
/*-----------------------------------------------------------------------------------*/
int scixselect(char *fname,unsigned long fname_len)
{
  integer v;
  double dv;
  CheckRhs(-1,0);
  C2F(dr1)("xselect","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xset(choice-name,x1,x2,x3,x4,x5) */
/* or   xset() */
/*-----------------------------------------------------------------------------------*/
int scixset(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2, xm[5],xn[5],x[5], i, v, isdc;
  integer lr, mark[2], font[2], verb=0;
  double  xx[5];
  double dv = 0.0 ;
  sciPointObj *subwin = NULL; 

  if (Rhs <= 0) {int zero=0; sci_demo(fname,"xsetm();",&zero); return 0; }

  CheckRhs(1,6);
  CheckLhs(0,1);

  GetRhsVar(1,"c",&m1,&n1,&l1);

  for (i=0; i < NUMSETFONC ; i++) 
    if (strcmp(cstk(l1),KeyTab_[i])==0) goto OK;

  i = 105;v=m1*n1;
  strncpy(C2F(cha1).buf,cstk(l1),v);
  C2F(msgs)(&i,&v);
  LhsVar(1)=0;
  return 0;

 OK:
  /* Allan CORNET Avril 2004 */
  /* Bloque la commande xset('window') sans numero de fenetre */
  if (Rhs == 1 && (strcmp(cstk(l1),"window") == 0) )
    {
      sciprint("xset(\"window\",window-number): window-number must be set\r\n");
      LhsVar(1)=0; return 0;
    }

  if (Rhs == 2 && VarType(2) != 1) 
    {
      /* second argument is not a scalar it must be a string */ 
      GetRhsVar(2,"c",&m2,&n2,&l2);
      C2F(xsetg)(cstk(l1),cstk(l2),m1,m2);
      LhsVar(1)=0; return 0;
    }

  if (Rhs == 1 && strcmp(cstk(l1),"default") == 0) 
    {
      /* first treatment for xsetg : then we continue */
      C2F(xsetg)(cstk(l1),"void",m1,4L);
    }

  for ( i = 2 ; i <= Rhs ; i++ ) 
    {
      GetRhsVar(i,"d",&xm[i-2],&xn[i-2],&lr);
      x[i - 2] = (integer) *stk(lr); xx[i - 2] = *stk(lr);
    }
  /* initialisation of a window if argument is not xset('window') 
   * with special cases if xset('colormap') or xset('default') 
   * and window does not exists we want to get into set_default_colormap 
   * only once 
   */
  isdc = 0;
  if (strcmp(cstk(l1),"colormap") == 0 || strcmp(cstk(l1),"default") == 0) C2F(sedeco)(&isdc);
  if (strcmp(cstk(l1),"window") != 0) SciWin();
  isdc = 1;
  if (strcmp(cstk(l1),"colormap") == 0 || strcmp(cstk(l1),"default") == 0) C2F(sedeco)(&isdc);
  if (strcmp(cstk(l1),"wdim") == 0 || strcmp(cstk(l1),"wpdim") == 0) {
    /* Xwindows limits dimensions to 2^16 */
    if ( (x[0]>65535)||(x[1]>65535)) {
      x[0]=Min(x[0],65535);
      x[1]=Min(x[1],65535);
      i = 106;v=0;
      C2F(msgs)(&i,&v);
    }
  }

  if (strcmp(cstk(l1),"clipping") == 0) 
    C2F(dr1)("xset",cstk(l1),&v,&v,&v,&v,&v,&v,&xx[0],&xx[1],&xx[2],&xx[3],5L,bsiz);
  else if ( strcmp(cstk(l1),"colormap") == 0) {

    if (version_flag() == 0)
      {
	sciSetColormap (sciGetCurrentFigure(), stk(lr), *xm, *xn);
	sciRedrawFigure();
      }
    else {
      C2F(dr1)("xset",cstk(l1),xm,xn,&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,bsiz);
      x[0] = xm[0]+1;
      C2F(dr1)("xset","color",&x[0],&x[1],&x[2],&x[3],&x[4],&v,&dv,&dv,&dv,&dv,5L,bsiz);
    }
  }
  else if ( strcmp(cstk(l1),"mark size") == 0) {
    C2F(dr1)("xget","mark",&verb,mark,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

    if(version_flag() == 0){
      subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      sciSetMarkSizeUnit(subwin,2); /* force switch to tabulated mode : old syntax */
    }

    mark[1]=(int)xx[0];
    if(version_flag() == 0){
      subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      sciSetMarkSize(subwin,mark[1]);
    }
    else
      C2F(dr1)("xset","mark",&(mark[0]),&(mark[1]),&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,5L);
  }
  else if ( strcmp(cstk(l1),"mark") == 0) {
    if(version_flag() == 0){
      subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      sciSetMarkSizeUnit(subwin,2); /* force switch to tabulated mode : old syntax */
      sciSetMarkStyle(subwin,(int) xx[0]);
      sciSetMarkSize(subwin,(int) xx[1]);
    }
    else
      C2F(dr1)("xset","mark",&(x[0]),&(x[1]),&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,5L);
  }
  else if ( strcmp(cstk(l1),"font size") == 0) {
    verb=0;
    C2F(dr1)("xget","font",&verb,font,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
    font[1]=(int)xx[0];
    C2F(dr1)("xset","font",&(font[0]),&(font[1]),&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,5L);
  } 
  /* NG beg */
  else if ( strcmp(cstk(l1),"old_style") == 0) {
    if (*stk(lr) == 0) 
      versionflag = (int) *stk(lr);
    else if (*stk(lr) == 1)
      versionflag = (int) *stk(lr);
    else
      Scierror(999,"%s: Value must be 1 or 0",fname);
  }/* NG end */
  else if((strcmp(cstk(l1),"default")==0) && (version_flag()==0)) {
    sciPointObj * pfigure = sciGetCurrentFigure();
    ResetFigureToDefaultValues(pfigure);

    /*special treatement for xset default and old_style off F.Leray 23.09.04 */
    /* mimic clf(gcf(),'reset') behaviour here */
    sciXbasc();
  }
  else if((strcmp(cstk(l1),"clipgrf")==0) && (version_flag()==0)) {
    /* special treatement for xset("cligrf") */
    sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    sciSetIsClipping(psubwin,0);
  }
  else if((strcmp(cstk(l1),"clipoff")==0) && (version_flag()==0)) {
    /* special treatement for xset("clipoff") */
    sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    sciSetIsClipping(psubwin,-1);
  }
  else if((strcmp(cstk(l1),"hidden3d")==0) && (version_flag()==0)) {
    /* special treatement for xset("hidden3d") */
    sciPointObj * psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
    pSUBWIN_FEATURE(psubwin)->hiddencolor = x[0];
  }
  else 
    C2F(dr1)("xset",cstk(l1),&x[0],&x[1],&x[2],&x[3],&x[4],&v,&dv,&dv,&dv,&dv,5L,bsiz);
  /* NG beg */
  if ((version_flag() == 0) && (strcmp(cstk(l1),"window") == 0))
    if (sciSwitchWindow(&x[0]) != 0){
      Scierror(999,"%s: It was not possible to create the requested figure",fname);
    }

  if (version_flag() == 0&&strcmp(cstk(l1),"wshow")!=0)
    {
      subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      if (( strcmp(cstk(l1),"foreground") == 0) || (strcmp(cstk(l1),"color") == 0) ||( strcmp(cstk(l1),"pattern") == 0) ) {
	sciSetForeground(subwin, x[0]); 
	if (sciGetSons((sciPointObj *) subwin) == (sciSons *)NULL) {/* added SS 20.04.04 */
	  /* if subwin is empty, set also the ticscolor and fontforeground */
	  pSUBWIN_FEATURE (subwin)->axes.ticscolor= x[0];
	  sciSetFontForeground(subwin,x[0]); 
	}
	/*sciSetForeground(sciGetParent(subwin), x[0]);*/ /* TO DELETE F.Leray 02.04.04*/
      } 
      else if ( strcmp(cstk(l1),"background") == 0) {
	sciSetBackground(subwin, x[0]); 
	/*sciSetBackground(sciGetParent(subwin), x[0]); */ /* TO DELETE F.Leray 02.04.04*/
      }  
      else if ( strcmp(cstk(l1),"thickness") == 0) {
	sciSetLineWidth(subwin, x[0]); 
	sciSetLineWidth(sciGetParent(subwin), x[0]);   
      } 
      else if ( strcmp(cstk(l1),"line style") == 0) {
	sciSetLineStyle(subwin, x[0]); 
	sciSetLineStyle(sciGetParent(subwin), x[0]);   
      }  
      else if ( strcmp(cstk(l1),"mark") == 0) {
	sciSetIsMark(subwin,1);                  /* A REVOIR F.Leray 21.01.05 */
	sciSetIsMark(sciGetParent(subwin),1);
	sciSetMarkStyle(subwin,x[0]); 
	sciSetMarkStyle(sciGetParent(subwin),x[0]);   
      } 
      else if ( strcmp(cstk(l1),"colormap") == 0) {
	sciSetColormap(sciGetParent(subwin), stk(lr),xm[0], xn[0]);
      }
      else if ( strcmp(cstk(l1),"font size") == 0) {
	int fontsize_ = 100*font[1];
	sciSetFontDeciWidth(subwin, fontsize_); 
	sciSetFontDeciWidth(sciGetParent(subwin), fontsize_);
      }     
      else if ( strcmp(cstk(l1),"dashes") == 0) {
	sciSetLineStyle(subwin, x[0]); 
	sciSetLineStyle(sciGetParent(subwin), x[0]);   
      }  
      else if ( strcmp(cstk(l1),"font") == 0) {
	sciSetFontStyle(subwin, x[0]); 
	sciSetFontDeciWidth(subwin,  x[1]*100);  
	sciSetFontStyle(sciGetParent(subwin), x[0]); 
	sciSetFontDeciWidth(sciGetParent(subwin),x[1]*100);  
      } 
      else if ( strcmp(cstk(l1),"alufunction") == 0) {
	sciSetXorMode(subwin, x[0]); 
	sciSetXorMode(sciGetParent(subwin), x[0]);   
      }
      else if ( strcmp(cstk(l1),"auto clear") == 0) {
	if ( x[0] == 1 )
	  {
	    sciSetAddPlot(subwin, TRUE); 
	    sciSetAddPlot(sciGetParent(subwin), TRUE);  
	  }
	else  
	  {
	    sciSetAddPlot(subwin, FALSE); 
	    sciSetAddPlot(sciGetParent(subwin), FALSE);  
	  } 
      } 
      else if ( strcmp(cstk(l1),"auto scale") == 0) {
	if ( x[0] == 1 )
	  {
	    sciSetAutoScale(subwin, TRUE); 
	    sciSetAutoScale(sciGetParent(subwin), TRUE);  
	  }
	else  
	  {
	    sciSetAutoScale(subwin, FALSE); 
	    sciSetAutoScale(sciGetParent(subwin), FALSE);  
	  } 
      }
      else if ( strcmp(cstk(l1),"wresize") == 0) {
	if ( x[0] == 1 )
	  {
	    sciSetResize(subwin, TRUE); 
	    sciSetResize(sciGetParent(subwin), TRUE);  
	  }
	else  
	  {
	    sciSetResize(subwin, FALSE); 
	    sciSetResize(sciGetParent(subwin), FALSE);  
	  }
      }
      else if ( strcmp(cstk(l1),"wpos") == 0) {
	sciSetFigurePos (sciGetParent(subwin), x[0], x[1]);

      }
      else if ( strcmp(cstk(l1),"wpdim") == 0) {
	pFIGURE_FEATURE(sciGetParent(subwin))->figuredimwidth=x[0];  
	pFIGURE_FEATURE(sciGetParent(subwin))->figuredimheight=x[1];

      } 
      else if ( strcmp(cstk(l1),"wdim") == 0) {
	pFIGURE_FEATURE(sciGetParent(subwin))->windowdimwidth=x[0];  
	pFIGURE_FEATURE(sciGetParent(subwin))->windowdimheight=x[1];

      } /*Ajout A.Djalel le 10/11/03 */
      else if ( strcmp(cstk(l1),"pixmap") == 0) {
	pFIGURE_FEATURE(sciGetParent(subwin))->pixmap=x[0];
	/*if(x[0] == 1) 
	  sciSetVisibility (sciGetParent(subwin), FALSE);
	  else{
	  sciSetVisibility (sciGetParent(subwin), TRUE);
	  pFIGURE_FEATURE(sciGetParent(subwin))->wshow=0;}*/
      }  
      else if ( strcmp(cstk(l1),"wshow") == 0) { /* a supprimer ce n'est pas une propriete mais une action */
	pFIGURE_FEATURE(sciGetParent(subwin))->wshow=1;
	sciSetVisibility (subwin, TRUE); 
      }
      if(strcmp(cstk(l1),"window") != 0) sciRedrawFigure();   
    }
  /***/
  /* NG end */    
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* xstring(x,y,str,[angle,box]) */
/*-----------------------------------------------------------------------------------*/
int scixstring(char *fname,unsigned long fname_len)
{
  double rect[4],wc,x,y,yi,angle=0.0;
  integer i,j,iv =0,flagx=0;
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str;
  long hdlstr/* , hdlrect */;

  CheckRhs(3,5);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); angle = *stk(l4 ); }
  if (Rhs >= 5) { GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5); flagx = (integer) *stk(l5);  }

  SciWin();
  wc = 0.;/* to keep the size of the largest line */

  if (version_flag() == 0) {
    BOOL isboxed = FALSE;

    if ((flagx == 1) && (*stk(l4) == 0))
      isboxed = TRUE;

    for (i = m3 -1 ; i >= 0; --i)  {
      int ib = 0;
      for (j = 0 ; j < n3 ; ++j) {
	strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
	ib += strlen(Str[i+ m3*j]);
	if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
      }
      Objstring (C2F(cha1).buf,bsiz,iv,x,y,&angle,rect,(double *)0,&hdlstr,-1,NULL,NULL,isboxed,TRUE,FALSE);
      wc = Max(wc,rect[2]);
      if (i != 0 ) 
	y += rect[3] * 1.2; 
      else 
	y += rect[3];
    } /* end for(i) */

    /*    if ((flagx == 1) && (*stk(l4) == 0)) { */
    /*       double dx1= y - yi ; */
    /*       Objrect (&x,&yi,&wc,&dx1,0,0,1,&hdlrect,TRUE); */
    /*     } */
    /*     /\** construct Compound and make it current object **\/  */
    /*     if ((flagx == 1) && (*stk(l4) == 0)) */
    /*       sciSetCurrentObj (ConstructCompoundSeq (m3+1)); */
    /*     else   */
    if (m3 > 1) sciSetCurrentObj ( ConstructCompoundSeq (m3));

  }
  else {
    for (i = m3 -1 ; i >= 0; --i) {
      int ib = 0;
      for (j = 0 ; j < n3 ; ++j) {
	strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
	ib += strlen(Str[i+ m3*j]);
	if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
      }
      Xstring (C2F(cha1).buf,bsiz,iv,x,y,angle,rect);
      wc = Max(wc,rect[2]);
      if (i != 0 ) 
	y += rect[3] * 1.2;
      else 
	y += rect[3];
    }
    if (flagx == 1) {
      double dx1 = y - yi;
      Xrect ("xrect",6L,&x,&y,&wc,&dx1); 
    }
  } 

  /* we must free Str memory */ 
  FreeRhsSVar(Str);

  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* xtitle(tit,x,y) */
/*-----------------------------------------------------------------------------------*/
int scixtitle(char *fname,unsigned long fname_len)
{
  int  narg          ;
  int  nbLabels      ; /* number of modified labels */
  int  box      = 0  ;
  BOOL isBoxSpecified = FALSE ;
  sciPointObj * psubwin = NULL;
  static rhs_opts opts[] = { {-1,"boxed","i" ,0,0,0},
                             {-1,NULL   ,NULL,0,0,0} };

  if (Rhs <= 0) {
    sci_demo(fname,"x=(1:10)';plot2d(x,x);xtitle(['Titre';'Principal'],'x','y');",&one);
    return 0;
  }
  
  CheckRhs(1,5);
  

  nbLabels = Rhs ;

  /* get the given options from the name in opts */
  if ( !get_optionals(fname,opts) ) return 0;

  /* compatibility with previous version in which box was put */
  /* at the fourth position */
  
  if ( Rhs == 4 )
  {
    int type = GetType(4);
    if ( type == 1 || type == 8 )/* double or integer */
    {
      int n,m ;
      int boxPtr   = -1 ; /* pointer of box on the stack */
      GetRhsVar(4,"i",&m,&n,&boxPtr);
      CheckScalar(4,m,n);
      box = *istk( boxPtr ) ;
      nbLabels-- ; /* it is not a label text */
      isBoxSpecified = TRUE ;
    }
  }
  
  if ( opts[0].position != -1 && !isBoxSpecified )
  {
    /* check if "box" is in the options */
    box = *istk(opts[0].l) ;
    if ( opts[0].m * opts[0].n != 1 )
     {       
       /* check size */
       Scierror( 999, "The boxed parameter must be a scalar", fname ) ;
       return 1 ;
     } 
    nbLabels-- ; /* it is not a label text */
  }
  
  SciWin();

  if (version_flag() == 0)
  {
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  }
  

  for ( narg = 1 ; narg <= nbLabels ; narg++)
    {
      int i,m,n;
      char **Str;
      GetRhsVar(narg,"S",&m,&n,&Str);
      if ( m*n == 0 ) continue;
      strcpy(C2F(cha1).buf,Str[0]);
      for ( i= 1 ; i < m*n ; i++) 
	{
	  strcat(C2F(cha1).buf,"@"); 
	  strcat(C2F(cha1).buf,Str[i]);
	}
      FreeRhsSVar(Str);
      if (version_flag() == 0)
	{
          sciPointObj * modifiedLabel ;
	  switch(narg){
	  case 1:
            modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_title ;
	    break;
	  case 2:
            modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_x_label ;
            break;
	  case 3:
            modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_y_label ;
            break;
          case 4:
            modifiedLabel = pSUBWIN_FEATURE(psubwin)->mon_z_label ;
          default:
            break;
          }
          sciSetText( modifiedLabel, C2F(cha1).buf , strlen(C2F(cha1).buf) ) ;
          if ( box == 1 )
          {
            sciSetIsFilled( modifiedLabel, TRUE ) ;
          }
          else
          {
            sciSetIsFilled( modifiedLabel, FALSE ) ;
          }
	  /*  sciRedrawFigure(); */
	}
      else
      {
	Xtitle (C2F(cha1).buf,narg);
      }
    }

  if (version_flag() == 0){
    /*     sciSetCurrentObj( sciGetSelectedSubWin (sciGetCurrentFigure ())); */
    sciSetCurrentObj(psubwin);
    /*    EraseAndOrRedraw(psubwin); */ /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */
    sciRedrawFigure();
  }

  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xstringb */
/*-----------------------------------------------------------------------------------*/
int scixstringb(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5,m6,n6,l6;
  integer fill =0, i,j,v,ib;
  double x,y,w,hx;
  char **Str;
  double wh[2],rect[4],angle=0;
  long hdlstr;

  SciWin();

  if ( Rhs <= 0 )
  {
    /* demo */
    int one = 1 ;
    char demo[] = "scf(); axes = gca() ; axes.axes_visible = 'on' ; str = ['Scilab','is';'not','Esilab']; xstringb(0.1,0.1,str,0.5,0.5,'fill') ;" ;
    sci_demo( fname, demo, &one ) ;
    return 0 ;
  }

  CheckRhs(5,6);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);  w = *stk(l4);
  GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5);  hx = *stk(l5);

  if (Rhs == 6) {
    GetRhsVar(6,"c",&m6,&n6,&l6);
    if ( m6*n6 !=0 && strcmp(cstk(l6),"fill") == 0) 
      fill =1;
    else {
      Scierror(999,"%s: optional argument has a wrong value 'fill' expected\r\n",
	       fname);
      return 0;   }
  }
  ib = 0;
  for (i = 0 ; i < m3 ; ++i) {
    for (j = 0 ; j < n3; ++j) 
      {
	strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
	ib += strlen(Str[i+ m3*j]);
	if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
      }
    C2F(cha1).buf[ib]= '\n'; ib++;
  }
  C2F(cha1).buf[ib-1]='\0';

  if (version_flag() == 0) {
    wh[0]=w;wh[1]=hx;
    Objstring (C2F(cha1).buf,bsiz,0,x,y,&angle,rect,wh,&hdlstr,fill,NULL,NULL,FALSE,TRUE,FALSE);
  }
  else { /* NG end */
    C2F(dr1)("xstringb",C2F(cha1).buf,&fill,&v,&v,&v,&v,&v,&x,&y,&w,&hx,9L,bsiz);
  }
  FreeRhsSVar(Str); /* we must free Str2 memory */ 
  LhsVar(1)=0;
  return 0;

} 

/*-----------------------------------------------------------------------------------*/
/*  rect=xstringl(x,y,str [,font_id [,font_size]]) */
/*-----------------------------------------------------------------------------------*/
int scixstringl(char *fname,unsigned long fname_len)
{
  double rect[4],wc,dv,x,y,yi;
  integer i,j,v,verb=0,un=1,quatre=4,m4,n4,l4,m1,n1,l1,m2,n2,l2,m3,n3;
  integer m5,n5,l5;
  int font_[2], cur_font_[2];
  char **Str;

  CheckRhs(3,5);
  CheckLhs(0,1);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);

  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 
  SciWin();

  C2F(dr1)("xget","font",&verb,font_,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

  cur_font_[0] = font_[0];
  cur_font_[1] = font_[1];

  if (Rhs >= 4) { GetRhsVar(4,"i",&m4,&n4,&l4); CheckScalar(4,m4,n4); font_[0] = *istk(l4);}
  if (Rhs >= 5) { GetRhsVar(5,"i",&m5,&n5,&l5); CheckScalar(5,m5,n5); font_[1] = *istk(l5);}

  C2F(dr1)("xset","font",&font_[0],&font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  if ( version_flag() == 0 )
  {
    updateScaleIfRequired( sciGetSelectedSubWin ( sciGetCurrentFigure () ) ) ;
  }

  wc = 0.;
  for (i = m3 -1 ; i >= 0; --i) 
    {
      int ib = 0;
      for (j = 0 ; j < n3 ; ++j) 
	{
	  strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
	  ib += strlen(Str[i+ m3*j]);
	  if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
	}
      C2F(dr1)("xstringl",C2F(cha1).buf,&v,&v,&v,&v,&v,&v,&x,&y,rect,&dv,9L,bsiz);
      wc = Max(wc,rect[2]);
      if (i != 0 ) 
	y += rect[3] * 1.2;
      else 
	y += rect[3];
    }
  FreeRhsSVar(Str);
  CreateVar(Rhs+1,"d",&un,&quatre,&l4);
  *stk(l4) = x; 
  *stk(l4+1) = y;
  *stk(l4+2) = wc ;
  *stk(l4+3) = y-yi;

  C2F(dr1)("xset","font",&cur_font_[0],&cur_font_[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  LhsVar(1)=Rhs+1;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* matrixLine2String                                                                 */
/* convert a line of a string matrix to an unique string. Each member of the line is */
/* separated by a space. The destination string is not allocate here.                */ 
/*-----------------------------------------------------------------------------------*/
void matrixLine2String( char ** matrix, int matrixSize[2], int numLine, char * dest )
{
  int lineIndex = 0 ;
  int j ;
  for ( j = 0 ; j < matrixSize[1] ; j++ )
    {
      /* insert the space but not before the first element */
      if ( j != 0 )
	{
	  dest[lineIndex] = ' ';
	  lineIndex++ ;
	}

      strcpy( dest + lineIndex, matrix[ numLine + matrixSize[0] * j ] ) ;
      lineIndex += strlen( matrix[ numLine + matrixSize[0] * j ]) ;
     
    }
      
}

/*-----------------------------------------------------------------------------------*/
/* stringBoundingRect                                                                */
/* compute the bounding rect [x,y,w,h] in pixels of matrix of strings.               */
/*-----------------------------------------------------------------------------------*/
void stringBoundingRect( char ** textMatrix, int textSize[2], int textPos[2], int rect[4] )
{
  
  int i ;
  
  rect[0] = textPos[0] ;
  rect[1] = textPos[1] ;
  rect[2] = 0 ;
  
  /* compute the rectangle [x,y,w,h] for the matrix */
  /* the width of the matrix is the max of the width of the lines */
  /* its heigth is the sum of the heigth plus little gap between */
  /* each lines */
  for ( i = textSize[0] - 1 ; i >= 0 ; i-- )
    {
      integer curRect[4] ;
      /* convert the line into a big string */
      matrixLine2String( textMatrix, textSize, i, C2F(cha1).buf ) ;

      C2F(dr)("xstringl",C2F(cha1).buf,&rect[0],&rect[1],curRect,PI0,PI0,PI0,PD0,PD0,PD0,PD0,9L,bsiz);
      rect[2] = Max( rect[2], curRect[2] );
      if ( i > 0 )
	{
	  /* a line is after this one */
	  /* here a little error is done sonce we work in pixels */
	  /* moreover the 1.2 should not be coded "en dur" */
	  rect[1] -= round( curRect[3] * 1.2 ) ;
	}
      else
	{
	  rect[1] -= curRect[3] ;
	}
    }
  rect[3] = textPos[1] - rect[1] ;
}


/*-----------------------------------------------------------------------------------*/
/* getStringBox                                                                      */
/* compute the four corners of a text                                                */
/*-----------------------------------------------------------------------------------*/
void getStringBox( char   ** text         ,
                   double    textPos[2]   ,
                   int       textSize[2]  ,
                   double    angle        ,
                   int       fontId       , 
                   int       fontSize     , 
                   double    corners[4][2] )
{
  integer    zero = 0   ;
  integer    rect[4]    ;
  integer    curFont[2] ;
  integer    center[2]  ;
  integer    v          ;


  /* get the current font */
  /* we need to change the defaut font before using xstringl */
  C2F(dr)("xget","font",&zero,curFont,&v,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,5L);

  /* set the new font */
  C2F(dr)("xset","font",&fontId,&fontSize,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* get the bounding box with xstringl */
  /* we must work with pixel values since the text is unaffected by scales */
  /* the lower-left pixel of the text is put is in (10,10) */
  /* to avoid <0 values for log axis */
  updateScaleIfRequired( sciGetSelectedSubWin ( sciGetCurrentFigure () ) ) ;
  center[0]  = XDouble2Pixel( textPos[0] ) ;
  center[1]  = YDouble2Pixel( textPos[1] ) ;
  
  /* get the bounding retcnagle [x,y,w,h] in pixels */
  stringBoundingRect( text, textSize, center, rect ) ;
  
  /* return to the previous size */
  C2F(dr)("xset","font",&curFont[0],&curFont[1],PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);

  /* get the 4 corners not turned */
  /* rect[0-1] == upper-left point */
  /* rect[2] == width  */
  /* rect[3] == heigth */
  
  corners[0][0] = rect[0] ;
  corners[0][1] = rect[1] + rect[3] ;

  corners[1][0] = rect[0] ;
  corners[1][1] = rect[1] ;

  corners[2][0] = rect[0] + rect[2] ;
  corners[2][1] = rect[1] ;

  corners[3][0] = corners[2][0] ;
  corners[3][1] = corners[0][1] ;
  
  /* rotate everything around corner1 if needed */
  if ( Abs( angle ) > EPSILON )
    {
      double cosAngle = cos( angle ) ;
      double sinAngle = sin( angle ) ;
      rotate2Dim( corners[1], corners[0], cosAngle, sinAngle, corners[1] ) ;
      rotate2Dim( corners[2], corners[0], cosAngle, sinAngle, corners[2] ) ;
      rotate2Dim( corners[3], corners[0], cosAngle, sinAngle, corners[3] ) ;
  
      /* take everything back to user coordinates */
      /* to retrieve exactly the first corner as in stringl we take the input */
      corners[0][0] = textPos[0] ; /*XDPixel2Double( corners[0][0] ) ;*/
      corners[0][1] = textPos[1] ; /*YDPixel2Double( corners[0][1] ) ;*/
    
      corners[1][0] = XDPixel2Double( corners[1][0] ) ;
      corners[1][1] = YDPixel2Double( corners[1][1] ) ;
    
      corners[2][0] = XDPixel2Double( corners[2][0] ) ;
      corners[2][1] = YDPixel2Double( corners[2][1] ) ;
    
      corners[3][0] = XDPixel2Double( corners[3][0] ) ;
      corners[3][1] = YDPixel2Double( corners[3][1] ) ;
    
    }
  else
    {
      /* to retrieve exactly the first corner as in stringl we take the input */
      corners[0][0] = textPos[0] ; /* XDPixel2Double( corners[0][0] ) ;*/
      corners[0][1] = textPos[1] ; /* YDPixel2Double( corners[0][1] ) ; */
    
      corners[1][0] = corners[0][0] ;
      corners[1][1] = YDPixel2Double( corners[1][1] ) ;
    
      corners[2][0] = XDPixel2Double( corners[2][0] ) ;
      corners[2][1] = corners[1][1] ;
    
      corners[3][0] = corners[2][0] ;
      corners[3][1] = corners[0][1] ;
    }

}

/*-----------------------------------------------------------------------------------*/
/* sciStringBox                                                                      */
/*-----------------------------------------------------------------------------------*/
int sciStringBox( char * fname, unsigned long fname_len )
{
  integer    two   = 2     ;
  integer    four  = 4     ;
  char    ** text          ;
  double     angle         ;
  integer    fontSize      ;
  double     textPos[2]    ;
  int        textSize[2]   ;
  integer    fontId        ;
  integer    m,n           ;
  integer    stackPointer  ;
  double     corners[4][2] ; /* the four edges of the boundingRect */


  /* The function should be called with stringbox(text,posX,posY,angle,fontSize,fontId) */
  CheckRhs( 6, 6 ) ;
  CheckLhs( 0, 1 ) ;  
  
  /* get the string which is boxed */
  GetRhsVar( 1, "S", &textSize[0], &textSize[1], &text ) ;
  
  /* get the position of the text */
  GetRhsVar( 2, "d", &m, &n, &stackPointer ) ;
  textPos[0] = *stk( stackPointer ) ;
  GetRhsVar( 3, "d", &m, &n, &stackPointer ) ;
  textPos[1] = *stk( stackPointer ) ;

  /* get the angle in radian in clockwise direction */
  GetRhsVar( 4, "d", &m, &n, &stackPointer ) ;
  angle = *stk( stackPointer ) ;
  angle = DEG2RAD( angle ) ;

  /* get the font size */
  GetRhsVar( 5, "i", &m, &n, &stackPointer ) ;
  fontId = *istk( stackPointer ) ;
  
  /* get the font id */
  GetRhsVar( 6, "i", &m, &n, &stackPointer ) ;
  fontSize = *istk( stackPointer ) ;

  /* create a window if needed to initialize the X11 graphic context  */
  SciWin() ;

  /* get the string box */
  getStringBox( text, textPos, textSize, angle, fontId, fontSize, corners ) ;

  FreeRhsSVar( text ) ;

  /* copy everything into the lhs */
  CreateVar( Rhs + 1, "d", &two, &four, &stackPointer ) ;
  *stk( stackPointer     )  = corners[0][0] ; 
  *stk( stackPointer + 1 )  = corners[0][1] ;
  *stk( stackPointer + 2 )  = corners[1][0] ;
  *stk( stackPointer + 3 )  = corners[1][1] ;
  *stk( stackPointer + 4 )  = corners[2][0] ;
  *stk( stackPointer + 5 )  = corners[2][1] ;
  *stk( stackPointer + 6 )  = corners[3][0] ;
  *stk( stackPointer + 7 )  = corners[3][1] ;

  LhsVar( 1 ) = Rhs + 1 ;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xtape */
/*-----------------------------------------------------------------------------------*/
int scixtape(char *fname,unsigned long fname_len)
{
  static double  rect_def[4] = { 0,0,10,10}, ebox_def[6] = {0,1,0,1,0,1};
  static integer iflag_def[4] = { 0,0,0,0 };
  static integer aint_def[4] = { 0,0,0,0 };
  static integer iscflag_def[2] = { 1,0 };
  static integer flagx_def[3] = { 1,1,1} ;
  integer *iflag = iflag_def,*aint = aint_def,*iscflag = iscflag_def, *flagx= flagx_def,num,v;
  double alpha = 35.0 ,theta = 45.0,  *rect = rect_def ,*ebox = ebox_def , dv;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5,m6,n6,l6,m7,n7,l7;

  CheckRhs(1,7);

  /* NG beg */
  if (version_flag() == 0) {
    Scierror(999,"%s: not implemented with new figure style\r\n",fname);
    return 0;
    /* call a Scilab function to handle compatibility */
  }/* NG end */

  /* first argument is a string */
  GetRhsVar(1,"c",&m1,&n1,&l1);

  if ( strcmp(cstk(l1),"on") == 0) 
    {
      CheckRhs(1,1);
      C2F(dr)("xsetdr","Rec",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,4L);
    }
  else if ( strcmp(cstk(l1),"clear") == 0)
    {
      CheckRhs(2,2);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
      C2F(dr)("xstart","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
    }
  else if (strcmp(cstk(l1),"replay") == 0) 
    {
      CheckRhs(2,2);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
      C2F(dr)("xreplay","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
    }
  else if (strcmp(cstk(l1),"replaysh") == 0) 
    {
      CheckRhs(2,2);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
      C2F(dr)("xreplaysh","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
    }
  else if (strcmp(cstk(l1),"replaysc") == 0) 
    {
      CheckRhs(2,5);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
      /*     s'il n'y a que trois argument le 3ieme est rect[4] */
      if (Rhs == 3) { GetRhsVar(3,"d",&m3,&n3,&l3); CheckLength(3,m3*n3,4); rect = stk(l3); }
      else if ( Rhs > 3 ) 
	{
	  GetRhsVar(3,"i",&m3,&n3,&l3); CheckLength(3,m3*n3,2); iscflag = istk(l3);
	  if ( Rhs >=4 ) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckLength(4,m4*n4,4); rect = stk(l4); }
	  if ( Rhs >=5 ) { GetRhsVar(5,"i",&m5,&n5,&l5); CheckLength(5,m5*n5,4); aint = istk(l5);}       
	}
      C2F(dr)("xreplaysc","v",&num,iscflag,&v,aint,&v,&v,rect,&dv,&dv,&dv,10L,2L);
    }
  else if (strcmp(cstk(l1),"replayna") == 0) 
    {
      CheckRhs(2,5);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); num = (integer) *stk(l2);      
      if ( Rhs >= 3 ) {GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3); theta = *stk(l3);}
      if ( Rhs >= 4 ) {GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); alpha = *stk(l4);}
      if ( Rhs >= 5 ) {GetRhsVar(5,"i",&m5,&n5,&l5); CheckLength(5,m5*n5,4); iflag = istk(l5);}
      if ( Rhs >= 6 ) {GetRhsVar(6,"i",&m6,&n6,&l6); CheckLength(6,m6*n6,3); flagx = istk(l6);}
      if ( Rhs >= 7 ) {GetRhsVar(7,"d",&m7,&n7,&l7); CheckLength(7,m7*n7,6); ebox = stk(l7);}
      C2F(dr)("xreplayna","v",&num,&v,&v,iflag,flagx,&v,&theta,&alpha,ebox,&dv,10L,2L);
    }
  else 
    {
      Scierror(999,"%s: first argument has a wrong value %s\r\n",fname,cstk(l1));
      return 0;
    }
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xinfo(string) */
/*-----------------------------------------------------------------------------------*/
int scixinfo(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1;
  CheckRhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  SciWin();
  C2F(dr)("xinfo",cstk(l1),PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,6L,m1);
  if (version_flag() == 0)
  {
    sciSetInfoMessage(sciGetCurrentFigure(),cstk(l1));
    
  }
  LhsVar(1) =0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* xsetech(wrect=[...],frect=[..],logflag="..", arect=[...]) */
/* or */
/* xsetech(wrect,[frect,logflag]) */
/* or  */
/* xsetech() */
/*-----------------------------------------------------------------------------------*/
int scixsetech(char* fname,unsigned long fname_len)
{ 
  double *wrect = 0,*frect=0,*arect=0;
  char *logflag = 0;
  static char logflag_def[]="nn";
  /** optional names must be stored in alphabetical order in opts **/
  static rhs_opts opts[]= { 
    {-1,"arect","d",0,0,0},
    {-1,"frect","d",0,0,0},
    {-1,"logflag","c",0,0,0},
    {-1,"wrect","d",0,0,0},
    {-1,NULL,NULL,0,0}
  };
  int minrhs = 0,maxrhs = 0,minlhs=0,maxlhs=1,nopt;

  nopt = NumOpt();
  SciWin();

  if ( nopt == 0) 
    {
      /** compatibility with old version **/

      int m1,n1,l1,m2,n2,l2,m3,n3,l3;
      minrhs = -1,maxrhs = 3,minlhs=0,maxlhs=1;
      CheckRhs(minrhs,maxrhs) ;
      CheckLhs(minlhs,maxlhs) ;

      if ( Rhs <= 0) 
	{
	  ShowScales();
	  LhsVar(1) = 0;
	  return 0;
	}

      GetRhsVar(1,"d", &m1, &n1, &l1);
      CheckDims(1,m1,n1,1,4);
      wrect = stk(l1);

      if (Rhs >= 2) { GetRhsVar(2,"d", &m2, &n2, &l2); CheckDims(2,m2,n2,1,4); frect=stk(l2);} 
      if (Rhs >= 3) { GetRhsVar(3,"c", &m3, &n3, &l3); CheckLength(3,m3,2); logflag = cstk(l3);}
      else
	logflag = logflag_def ; /* compatibility with old version */
    }
  else 
    {
      CheckRhs(minrhs,maxrhs+nopt) ;
      CheckLhs(minlhs,maxlhs) ;

      if ( get_optionals(fname,opts) == 0) return 0;

      if ( opts[0].position != -1 ) { 
	arect = stk(opts[0].l);	CheckLength(opts[0].position,opts[0].m*opts[0].n,4);
      }
      if ( opts[1].position != -1 ) { 
	frect = stk(opts[1].l);	CheckLength(opts[1].position,opts[1].m*opts[1].n,4);
      } 
      if ( opts[2].position != -1 ) { 
	logflag = cstk(opts[2].l);CheckLength(opts[2].position,opts[2].m*opts[2].n,2);
      } 
      else
	logflag = logflag_def ; 
      if ( opts[3].position != -1 ) { 
	wrect = stk(opts[3].l);	CheckLength(opts[3].position,opts[3].m*opts[3].n,4);
      } 
    }
  SciWin();
  C2F(Nsetscale2d)(wrect,arect,frect,logflag,0L);
  LhsVar(1)=0;
  return 0;

}





/*-----------------------------------------------------------------------------------*/
/* xsetech(wrect=[...],frect=[..],logflag="..", arect=[...]) */
/* or */
/* xsetech(wrect,[frect,logflag]) */
/* or  */
/* xsetech() */
/*-----------------------------------------------------------------------------------*/
int scinewaxes(char* fname,unsigned long fname_len)
{ 

  int minrhs = 0,maxrhs = 0,minlhs=0,maxlhs=1;
  sciPointObj *masousfen;
  int outindex,numrow   = 1,numcol   = 1;
  CheckRhs(minrhs,maxrhs) ;
  CheckLhs(minlhs,maxlhs) ;

  SciWin();
  if ((masousfen = ConstructSubWin (sciGetCurrentFigure(), 0)) != NULL)
    {
      sciSetCurrentObj(masousfen);
      sciSetSelectedSubWin(masousfen);
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *hstk(outindex) = sciGetHandle(masousfen);
      LhsVar(1)=1;
    }
  else {
    Scierror(999,"%s: running out of memory \r\n",fname);
  }
  return 0;

}











/*-----------------------------------------------------------------------------------*/
/* xfpoly(xv,yv,[close]) */
/*-----------------------------------------------------------------------------------*/
int scixfpoly(char *fname,unsigned long fname_len)
{
  integer close=0,m1,n1,l1,m2,n2 ,l2,m3,n3,l3,mn1 ;

  long hdl; /* NG */

  SciWin();
  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  if (Rhs == 3) {
    GetRhsVar(3,"d",&m3,&n3,&l3);
    CheckScalar(3,m3,n3);
    close = (integer) *stk(l3);
  } 
  mn1 = m1 * n1;
  /* NG beg */
  if (version_flag() == 0){
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

    if(close == 0)
      close = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
    Objfpoly (stk(l1),stk(l2),mn1,&close,&hdl,0);

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0) {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
			/*    EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(sciGetCurrentObj ());
  }
  else
    Xfpoly(mn1,close,stk(l1),stk(l2));
  /* NG end */
  LhsVar(1)=0;
  return 0;

}

/*-----------------------------------------------------------------------------------*/
/* xfpolys(xpols,ypols,[fill]) */
/*-----------------------------------------------------------------------------------*/
int scixfpolys(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,v1=0,v2=0; /* v2 = 0 F.leray 24.02.04 unused flag*/
  /* v1 is the flag used for flat (v1==1) or interpolated (v1==2) shading */

  int i,color;
  long hdl;

  SciWin();

  CheckRhs(2,3);
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);


  if (Rhs == 3) 
    {
      GetRhsVar(3,"i",&m3,&n3,&l3); 

      if (m3*n3==m1*n1) 
	{ /* Code modified by polpoth 7/7/2000 */
	  CheckSameDims(1,3,m1,n1,m3,n3);
	  v1=2; /* interpolated shading */

	  if ( m3 != 3 && m3 != 4 ) 
	    {
	      Scierror(999,"%s: interpolated shading only works for polygons of size 3 or 4\r\n",fname);
	      return 0;
	    }
	} else
	  {
	    CheckVector(3,m3,n3);
	    CheckDimProp(2,3,m3 * n3 != n2);
	    v1=1; /* flat shading */
	  }
    }
  else 
    {
      int un=1,ix;
      CreateVar(3,"i",&un,&n2,&l3);
      for (ix = 0 ; ix < n2 ; ++ix) *istk(l3+ix) = 0;
      m3 = n3 = 1;
    }
  if (version_flag() == 0) {
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
    for (i = 0; i < n1; ++i) {
      if(m3 == 1 || n3 == 1) /* color vector specified */
	{
	  if (*istk(l3+i) == 0)
	    {
	      color= sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	      Objpoly (stk(l1+(i*m1)),stk(l2+(i*m1)),m1,1,color,&hdl);
	    }
	  else
	    {
	      Objfpoly (stk(l1+(i*m1)),stk(l2+(i*m1)),m1,istk(l3+i),&hdl,v1); /* F.Leray fix bug 04.10.05 */
	    }
	}
      else /* we have a color matrix used for interpolated shading : one color per vertex */
	Objfpoly (stk(l1+(i*m1)),stk(l2+(i*m1)),m1,istk(l3+i*m3),&hdl,v1); /* F.Leray fix bug 04.10.05 */
    }
    /** construct Compound and make it current object**/
    sciSetCurrentObj (ConstructCompoundSeq (n1));

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0) {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
			/*    EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} /\* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 *\/ */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(sciGetCurrentObj ());
  }
  else
    Xfpolys(istk(l3),v1,v2,n2,m2,stk(l1),stk(l2));

  LhsVar(1)=0;  
  return 0;  
} 

/*-----------------------------------------------------------------------------------*/
/* xget */ 
/*-----------------------------------------------------------------------------------*/
int scixget(char *fname,unsigned long fname_len)
{
  integer flagx=0,x1[10],x2, m1,n1,l1,m2,n2,l2,l3,v,i ;
  double dv;

  SciWin();
  if (Rhs <= 0) {
    sci_demo(fname,"xsetm();",&zero);
    return 0;
  }

  CheckRhs(1,2);
  CheckLhs(0,1);

  GetRhsVar(1,"c",&m1,&n1,&l1);

  /* check if key is valid */
  for (i=0; i < NUMSETFONC ; i++) 
    if (strcmp(cstk(l1),KeyTab_[i])==0) goto OK;

  i = 105;v=m1*n1;
  strncpy(C2F(cha1).buf,cstk(l1),v);
  C2F(msgs)(&i,&v);
  x2=0;
  CreateVar(Rhs+1,"d",&x2,&x2,&l3);
  LhsVar(1)=Rhs+1;
  return 0;

 OK:  
  if (Rhs == 2) {
    GetRhsVar(2,"d",&m2,&n2,&l2); 
    CheckScalar(2,m2,n2);  
    flagx = (integer) *stk(l2); }
  else
    flagx = 0;

  if ( strcmp(cstk(l1),"fpf") == 0 || strcmp(cstk(l1),"auto clear") == 0) 
    {
      int bufl;
      /*     special case for global variables set */
      C2F(xgetg)( cstk(l1),C2F(cha1).buf,&bufl,m1,bsiz);
      CreateVar(Rhs+1,"c",&bufl,&one,&l3);
      strncpy(cstk(l3),C2F(cha1).buf,bufl);
      LhsVar(1)=Rhs+1;
      return 0;
    }
  else if ( strcmp(cstk(l1),"colormap") == 0) 
    {
      /*     special case for colormap : must allocate space */
      int m3,n3=3;
      /*      CheckColormap(&m3); */
      C2F(dr)("xget","cmap_size",x1,&m3,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);
      
      if ( m3 == 0) n3=0;
      CreateVar(Rhs+1,"d",&m3,&n3,&l3);
      C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,stk(l3),&dv,&dv,&dv,5L,bsiz);
      LhsVar(1)=Rhs+1;
    }
  else if ( strcmp(cstk(l1),"mark") == 0) {
    int i;
    if(version_flag() == 0){
      sciPointObj * subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      x1[0] = sciGetMarkStyle(subwin);
      x1[1] = sciGetMarkSize(subwin);
    }
    else
      C2F(dr1)("xget","mark",&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

    x2=2;
    CreateVar(Rhs+1,"d",&one,&x2,&l3);
    for (i = 0 ; i < x2 ; ++i) *stk(l3 + i ) = (double) x1[i];      
    LhsVar(1)=Rhs+1;
  }
  else if ( strcmp(cstk(l1),"mark size") == 0) {
    int i;
    if(version_flag() == 0){
      sciPointObj * subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      x1[0] = x1[1] = sciGetMarkSize(subwin);
    }
    else
      C2F(dr1)("xget","mark",&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);

    x1[0]=x1[1];
    x2=1;
    CreateVar(Rhs+1,"d",&one,&x2,&l3);
    for (i = 0 ; i < x2 ; ++i) *stk(l3 + i ) = (double) x1[i];      
    LhsVar(1)=Rhs+1;
  }
  else if ( strcmp(cstk(l1),"line style") == 0) {
    if(version_flag() == 0){
      sciPointObj * subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      x1[0] = sciGetLineStyle(subwin);
    }
    else
      C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);

    x2=1;
    CreateVar(Rhs+1,"d",&one,&x2,&l3);
    *stk(l3 ) = (double) x1[0];      
    LhsVar(1)=Rhs+1;
  } 
  /* NG beg */
  else if ( strcmp(cstk(l1),"old_style") == 0) 
    {
      x2=1;
      CreateVar(Rhs+1,"d",&one,&x2,&l3);
      *stk(l3 ) = version_flag();      
      LhsVar(1)=Rhs+1;          
    }   /* NG end*/
  else 
    {
      int i;
      x2=0;
      if(version_flag()==0){ /* NG */
	sciPointObj *psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
	if(strcmp(cstk(l1),"font")==0){
	  x1[0] = sciGetFontStyle(psubwin);
	  x1[1] = sciGetFontDeciWidth(psubwin)/100;
	  x2 = 2;
	}
	else if(strcmp(cstk(l1),"font size")==0){
	  x1[0] = sciGetFontDeciWidth(psubwin)/100;
	  x2 = 1;
	}
	else if(strcmp(cstk(l1),"dashes")==0){
	  x1[0] = sciGetLineStyle(psubwin);
	  x2 = 1;
	}
	else if(strcmp(cstk(l1),"hidden3d")==0){
	  x1[0] = pSUBWIN_FEATURE(psubwin)->hiddencolor;
	  x2 = 1;
	}
	else
	  C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);
      }
      else
	C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);

      if (x2 > 0) {
	CreateVar(Rhs+1,"d",&one,&x2,&l3);
	for (i = 0 ; i < x2 ; ++i) *stk(l3 + i ) = (double) x1[i];      
      }
      else {
	x2=0;
	CreateVar(Rhs+1,"d",&x2,&x2,&l3);
      }
      LhsVar(1)=Rhs+1;
    }
  return 0;
}













/*-----------------------------------------------------------------------------------*/
/* xinit([driver-name]) */
/*-----------------------------------------------------------------------------------*/
int scixinit(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,v,v1=-1;
  double dv;
  CheckRhs(-1,1);

  if (Rhs <= 0 )
    {
      C2F(dr1)("xinit"," ",&v1,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,2L);
      if(version_flag() == 0) xinitxend_flag = 1; /* we do not draw now into the file/memory (depending on the driver type) */
    } 
  else 
    {
      char *param1=NULL;
      GetRhsVar(1,"c",&m1,&n1,&l1);

      param1=cstk(l1);
      if (param1[0]=='.') SetTKGraphicalMode(TRUE);
      C2F(dr1)("xinit",cstk(l1),&v1,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,m1);
      if(version_flag() == 0) xinitxend_flag = 1; /* we do not draw now into the file/memory (depending on the driver type) */

    }
  LhsVar(1)=0; return 0;
}


/*-----------------------------------------------------------------------------------*/
/* xlfont(font-name,font-id) */
/* fonts=xlfont() */
/* Warning sz dimensions must be compatible with periX11.c FONTNUMBER */
/*-----------------------------------------------------------------------------------*/
int scixlfont(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,v,i,count,sz[10],num;
  double dv;

  SciWin();
  if (Rhs <= 0) 
    {
      char **S;
      /*     we list the fonts and return their names in a string matrix */
      int m = 0;
      C2F(dr1)("xgfont",C2F(cha1).buf,&m,sz,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,bsiz);
      if (m == 0) { LhsVar(1)=0; return 0;}
      if (( S= (char **) MALLOC( (m+1)*sizeof(char*))) == NULL) 
	{
	  Scierror(999,"%s: running out of memory \r\n",fname);
	  return 0;
	}
      count =0;
      /* OS Windows: Pb here due to fonttab again: its size is 10 and NUMBERFONT is 11 (=m) so...*/
      /* By-pass here for the moment: FONTNUMBER set to FONTNUMBER-- in queryfamily (file periWin.c)*/
      for ( i = 0 ; i < m ; i++) {
	if ((S[i]= (char *) MALLOC((sz[i]+1)*sizeof(char))) == NULL) 
	  {
	    Scierror(999,"%s: running out of memory \r\n",fname);
	    return 0;
	  }
	strncpy(S[i],C2F(cha1).buf+count,sz[i]);
	count += sz[i]; 
	S[i][sz[i]]='\0';
      } 
      S[m]= (char *) 0;
      CreateVarFromPtr(1,"S",&one,&m,S);
      FreeRhsSVar(S);
      LhsVar(1)=1;
      return 0;
    }
  CheckRhs(2,2);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);  CheckScalar(2,m2,n2);  num = (integer) *stk(l2);
  C2F(dr1)("xlfont",cstk(l1),&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* scixnumb(x,y,nums,[box,angles]) : NO MORE USED */
/*-----------------------------------------------------------------------------------*/
int scixnumb(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5;
  integer flagx=0,mn3;
  SciWin();

  CheckRhs(3,5);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  GetRhsVar(3,"d",&m3,&n3,&l3);
  CheckSameDims(1,2,m1,n1,m2,n2);
  CheckSameDims(2,3,m2,n2,m3,n3);
  mn3=m3*n3;
  if ( mn3 == 0) {   LhsVar(1)=0;  return 0;}

  if (Rhs >= 4) {GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); flagx = (integer) *stk(l4);}
  if (Rhs >= 5) {
    GetRhsVar(5,"d",&m5,&n5,&l5); CheckSameDims(1,5,m1,n1,m5,n5);
  }
  else {
    int i;
    CreateVar(Rhs+1,"d",&m3,&n3,&l5);
    for ( i=0 ; i < mn3 ; i++ ) *stk(l5+i) = 0.0;
  } 
  /* NG beg */
  if (version_flag() == 0)
    Objnumb(fname,fname_len,mn3,flagx,stk(l1),stk(l2),stk(l3),stk(l5));
  else
    Xnumb(fname,mn3,flagx,stk(l1),stk(l2),stk(l3),stk(l5));
  /* NG end */
  LhsVar(1)=0;
  return 0;

} 

/*-----------------------------------------------------------------------------------*/
/* xpause(microsecs) */
/*-----------------------------------------------------------------------------------*/
int scixpause(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,v,sec=0;
  double dv;
  SciWin();
  CheckRhs(-1,1);
  if (Rhs == 1) { GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); sec = (integer) *stk(l1);} 
  C2F(dr1)("xpause","v",&sec,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* [wrect,frect,logflag,arect]=xgetech() */
/*-----------------------------------------------------------------------------------*/
int scixgetech(char *fname,unsigned long fname_len)
{
  double WRect[4],FRect[4],ARect[4];
  double *W= WRect, *F = FRect, *A= ARect ;
  int un=1,deux=2,quatre=4,i,l1,l2,l3,l4;
  char logf[2], *L=logf;

  CheckRhs(0,0);
  CheckLhs(1,4);
  if ( Lhs >=1 ) { CreateVar( 1, "d", &un, &quatre,&l1); W= stk(l1);}
  if ( Lhs >=2 ) { CreateVar( 2, "d", &un, &quatre,&l2); F= stk(l2);}
  if ( Lhs >=3 ) { CreateVar( 3, "c", &un, &deux,  &l3); L= cstk(l3);}
  if ( Lhs >=4 ) { CreateVar( 4, "d", &un, &quatre,&l4); A= stk(l4);}
  SciWin();
  getscale2d(W,F,L,A);
  for ( i = 1 ; i <= Lhs ; i++) LhsVar(i) = i;
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/* fec(x,y,triangles,func,[strf,leg,rect,nax,zminmax,colminmax]); */
/*-----------------------------------------------------------------------------------*/
int scifec(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4, mn1;

  static rhs_opts opts[]= { {-1,"colminmax","?",0,0,0},
			    {-1,"colout","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"mesh","?",0,0,0},
			    {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,"zminmax","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  if (Rhs <= 0) {
    sci_demo (fname," exec(\"SCI/demos/graphics/fec/fec.ex1\");",&one);
    return 0;
  }

  CheckRhs(4,12);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 5) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,5);
    Error(999); 
    return(0);
  }
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

  GetRhsVar(3,"d",&m3,&n3,&l3);
  if (n3 != 5) {
    Scierror(999,"%s: triangles have %d columns,expecting 5\r\n",fname,n3);
    return 0;
  }

  GetRhsVar(4,"d",&m4,&n4,&l4);

  if (m1 * n1 == 0 || m3 == 0) { LhsVar(1)=0;     return 0;} 

  GetStrf(5,opts);
  GetLegend(6,opts);
  GetRect(7,opts);
  GetNax(8,opts);
  GetZminmax(9,opts);
  GetColminmax(10,opts);
  GetColOut(11,opts);
  GetWithMesh(12,opts);

  SciWin();
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1]='7';
    if (Legend != def_legend)
      strfl[0]='1';
    if(version_flag() != 0){
      if (Nax != def_nax)
	strfl[1]='1';
    }
  }
  mn1 = m1 * n1;

  /* NG beg */
  if (version_flag() == 0)
    Objfec (stk(l1),stk(l2),stk(l3),stk(l4),&mn1,&m3,Strf,Legend,Rect,Nax,Zminmax,Colminmax,ColOut,WithMesh,flagNax);
  else
    Xfec (stk(l1),stk(l2),stk(l3),stk(l4),&mn1,&m3,Strf,Legend,Rect,Nax,Zminmax,Colminmax,ColOut,WithMesh);
  /* NG end */
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* [rep,win] = xgetmouse([flag],[sel]) */
/*-----------------------------------------------------------------------------------*/
int scixgetmouse(char *fname,unsigned long fname_len)
{
  integer  m1=1,n1=3,l1,l2,button,v;
  integer iflag;
  integer sel[2],m,n,v2;
  double x,y,dv;

  CheckRhs(0,2);
  CheckLhs(1,2);
  
  if (Rhs<=0) {
    iflag=0;sel[0]=1;sel[1]=0;
  }
  else {
    if (GetType(1)==4) {
      iflag=0;
      GetRhsVar(1, "b", &m, &n, &l1);
      CheckDims(1,m*n,1,2,1);
      sel[0]=*istk(l1);sel[1]=*istk(l1+1);}
    else {
      iflag=1;
      sel[0]=1;sel[1]=0;
    }
    if (Rhs==2) {
      if (iflag==1) {
	GetRhsVar(2, "b", &m, &n, &l1);
	CheckDims(2,m*n,1,2,1);
	sel[0]=*istk(l1);sel[1]=*istk(l1+1);}
    }
  }

  SciWin();
  
  switch (Lhs) {
  case 1: 
    v2=0;
    C2F(dr1)("xgetmouse","xv",&button,&iflag,&v,&v,sel,&v2,&x,&y,&dv,&dv,10L,3L); 
    CreateVar(Rhs+1,"d",&m1,&n1,&l1);
    if (button==-100)
      {
	*stk(l1) = -1;
	*stk(l1+1) = -1;
	*stk(l1+2) = (double) button;
      }
    else
      {
	*stk(l1) = x;  *stk(l1+1) = y;  *stk(l1+2) = (double) button;
      }
    LhsVar(1) = Rhs+1;
    return 0;
  case 2:
    v2=2;
    C2F(dr1)("xgetmouse","xv",&button,&iflag,&v,&v,sel,&v2,&x,&y,&dv,&dv,10L,3L);
    CreateVar(Rhs+1,"d",&m1,&n1,&l1);
    if (button==-100)
      {
	*stk(l1) = -1;
	*stk(l1+1) = -1;
	*stk(l1+2) = (double) button;
      }
    else
      {
	*stk(l1) = x;  *stk(l1+1) = y;  *stk(l1+2) = (double) button;
      }
    LhsVar(1) = Rhs+1;

    CreateVar(Rhs+2,"d",&m1,&m1,&l2);
    *stk(l2) = iflag; /* this is the window number */
    LhsVar(2) = Rhs+2;
    return 0;
  }
  return -1 ;
} 

/*-----------------------------------------------------------------------------------*/
/* xsave('fname' [, wid]) */
/*-----------------------------------------------------------------------------------*/
int scixsave(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,v,wid;
  double dv;

  CheckRhs(1,2);
  /* NG beg */
  if (version_flag() == 0) {
    /* call a Scilab function to handle compatibility */
    C2F(callscifun)("%xsave",(unsigned long) 6);
    return 0;
  }/* NG end */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if (Rhs == 2) { GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); wid = (integer) *stk(l2 ); } 
  else {
    integer verb=0,na;
    C2F(dr)("xget","window",&verb,&wid,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
  }
  SciWin();

  C2F(xsaveplots)(&wid,cstk(l1),m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xload('fname' [, wid]) */
/*-----------------------------------------------------------------------------------*/
int scixload(char *fname,unsigned long fname_len)
{
  double dv;
  integer m1,n1,l1,m2,n2,l2,wid,v=1;

  CheckRhs(1,2);
  if (version_flag() == 0) {
    /* call a Scilab function to handle compatibility */
    C2F(callscifun)("%xload",(unsigned long) 6);
    return 0;
  }/* NG end */
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if (Rhs == 2) {
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); wid = (integer) *stk(l2 );
    C2F(dr)("xset","window",&wid,&v,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  }
  SciWin();
  C2F(xloadplots)(cstk(l1),m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
/* xdel([win-ids]) */
/*-----------------------------------------------------------------------------------*/
int scidelw(char *fname,unsigned long fname_len)
{ 
  integer verb=0,na,v,m1,n1,l1,win;
  double dv;
  CheckRhs(-1,1);
  if (Rhs >= 1) {
    int i;
    GetRhsVar(1,"d",&m1,&n1,&l1); 
    for ( i=0; i < m1*n1 ; i++ ) 
      {
	win = (integer) *stk(l1+i);
	C2F(deletewin)(&win); 
      }
  } else {
    C2F(dr)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
    C2F(deletewin)(&win);
  }
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* [s,v]= gsort(x,[str1,str2]) */
/*       str1 = 'g','r','c','lc','lr', */
/*       str2 = 'i' | 'd' */
/*-----------------------------------------------------------------------------------*/
int scixsort(char *fname,unsigned long fname_len)
{
  char **S;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,v;
  integer un=1,lex,iflag = 0;
  char iord[2] ; /* = { 'd','\0' }; */
  char typex[10]; /* = { 'g' ,'\0'} ; */
  double dv;
  integer iv;
  iord[0] = 'd'; iord[1]='\0';
  typex[0] = 'g'; typex[1] = '\0';

  CheckRhs(1,3);

  switch ( VarType(1)) 
    {
    case 10 : 
      GetRhsVar(1,"S",&m1,&n1,&S);
      break;
    case 1 :
      GetRhsVar(1,"d",&m1,&n1,&l1);
      break;
    default :
      Scierror(999,"%s: first argument has a wrong type, expecting scalar or string matrix\r\n",fname);
      return 0;
    }

  if (Rhs >= 2) 
    {
      char c;
      GetRhsVar(2,"c",&m2,&n2,&l2);
      if ( m2 == 0 ) {
	Scierror(999,"%s: second argument is an empty string\r\n",fname);
	return 0;
      }
      c = *cstk(l2);
      if (c != 'r' && c != 'c' && c != 'g' && c != 'l') {
	Scierror(999,"%s: second argument has a wrong value %s should be in r,c,g,lr,lc\r\n",fname,cstk(l2));
	return 0;
      }
      strcpy(typex,cstk(l2));
    }

  if (Rhs >= 3) 
    {
      GetRhsVar(3,"c",&m3,&n3,&l3);
      CheckLength(3,m3,1);
      if ( *cstk(l3) != 'i' && *cstk(l3) != 'd') 
	{
	  Scierror(999,"%s: third argument must be \"i\" or \"d\"\r\n",fname);
	  return 0;
	}
      iord[0] = *cstk(l3);
    }


  if ( VarType(1) == 1) 
    {
      /** Scalar matrix **/
      if (Lhs == 2) {
	iflag = 1;
	if ( typex[0] == 'l') 
	  {
	    if (typex[1] == 'r') 	  {
	      CreateVar(Rhs+1,"i",&m1,&un,&lex);
	    } else  {
	      CreateVar(Rhs+1,"i",&un,&n1,&lex);
	    }
	    GetRhsVar(1,"i",&m1,&n1,&l1);
	    C2F(gsort)(istk(l1),&dv,istk(lex),&iflag,&m1,&n1,typex,iord);
	  } 
	else 
	  {
	    CreateVar(Rhs+1,"i",&m1,&n1,&lex);
	    C2F(gsort)(&iv,stk(l1),istk(lex),&iflag,&m1,&n1,typex,iord);
	  }
      } 
      else 
	{
	  iflag = 0;
	  if ( typex[0] == 'l') 
	    {
	      GetRhsVar(1,"i",&m1,&n1,&l1);
	      C2F(gsort)(istk(l1),&dv,&iv,&iflag,&m1,&n1,typex,iord);
	    } 
	  else 
	    {
	      C2F(gsort)(&iv,stk(l1),&iv,&iflag,&m1,&n1,typex,iord );
	    }
	}
      LhsVar(1)=1;
      if ( Lhs == 2 ) LhsVar(2)=Rhs+1;
    }
  else 
    {
      /** String matrix **/
      if (Lhs == 2) {
	iflag = 1;
	if ( typex[0] == 'l') 
	  {
	    if (typex[1] == 'r') 	  {
	      CreateVar(Rhs+1,"i",&m1,&un,&lex);
	    } else  {
	      CreateVar(Rhs+1,"i",&un,&n1,&lex);
	    }
	    C2F(gsorts)(S,istk(lex),&iflag,&m1,&n1,typex,iord);
	  } 
	else 
	  {
	    CreateVar(Rhs+1,"i",&m1,&n1,&lex);
	    C2F(gsorts)(S,istk(lex),&iflag,&m1,&n1,typex,iord);
	  }
	CreateVarFromPtr(Rhs+2,"S", &m1, &n1, S);
      } else {
	iflag = 0;
	C2F(gsorts)(S,&v,&iflag,&m1,&n1,typex,iord);
	CreateVarFromPtr(Rhs+1,"S", &m1, &n1, S);
      }
      /* we must free Str2 memory */ 
      FreeRhsSVar(S);
      if ( Lhs == 2 ) 
	{
	  LhsVar(1)=Rhs+2;
	  LhsVar(2)=Rhs+1;
	}
      else 
	{
	  LhsVar(1)=Rhs+1;
	}
    }
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/* x=winsid() */
/*-----------------------------------------------------------------------------------*/
int sciwinsid(char *fname,unsigned long fname_len)
{
  integer iflag =0,ids,num,un=1,l1;

  CheckRhs(-1,0) ;  /* NG beg */
  /*  if (version_flag() == 0) */
  /*    { */
  /*     sciGetIdFigure (&ids,&num,&iflag); */
  /*     CreateVar(1,"i",&un,&num,&l1); */
  /*     iflag = 1;  */
  /*     sciGetIdFigure (istk(l1),&num,&iflag); */
  /*     LhsVar(1) = 1;  */
  /*    }/\* NG end*\/ */
  /*  else */
  /*    { */
  C2F(getwins)(&num,&ids ,&iflag);
  CreateVar(1,"i",&un,&num,&l1);
  iflag = 1; 
  C2F(getwins)(&num,istk(l1),&iflag);
  LhsVar(1)=1;
  /*     }  */

  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* [xi,xa,np1,np2,kMinr,kMaxr,ar]=xgraduate(xmi,xma) */
/* rajouter ds le man XXXX */
/*-----------------------------------------------------------------------------------*/
int scixgraduate(char *fname,unsigned long fname_len)
{
  double xa,xi;
  integer m1,n1,l1,m2,n2,l2,i;
  integer kMinr,kMaxr,ar,lr,np1,np2,un=1;

  CheckRhs(2,2);
  CheckLhs(2,7);
  GetRhsVar(1,"d",&m1,&n1,&l1);  CheckScalar(1,m1,n1);
  GetRhsVar(2,"d",&m2,&n2,&l2);  CheckScalar(2,m2,n2);

  C2F(graduate)(stk(l1),stk(l2),&xi,&xa,&np1,&np2,&kMinr,&kMaxr,&ar);

  *stk(l1) = xi;
  *stk(l2) = xa;

  if (Lhs >= 3) { CreateVar(3,"d",&un,&un,&lr); *stk(lr ) = (double) np1;  }
  if (Lhs >= 4) { CreateVar(4,"d",&un,&un,&lr); *stk(lr ) = (double) np2;  }
  if (Lhs >= 5) { CreateVar(5,"d",&un,&un,&lr); *stk(lr ) = (double) kMinr;  }
  if (Lhs >= 6) { CreateVar(6,"d",&un,&un,&lr); *stk(lr ) = (double) kMaxr;  }
  if (Lhs >= 7) { CreateVar(7,"d",&un,&un,&lr); *stk(lr ) = (double) ar;  }
  for (i= 1; i <= Lhs ; i++)  LhsVar(i) = i;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* name('name') : give a name to current graphic window */
/*-----------------------------------------------------------------------------------*/
int scixname(char *fname,unsigned long fname_len)
{
  double dv;
  integer v,m1,n1,l1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  SciWin();
  if ( version_flag() == 0 )
  {
    sciSetName( sciGetCurrentFigure(), cstk(l1), m1 ) ;
    /*C2F(dr)("xname",cstk(l1),&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,bsiz);*/
  }
  else
  {
    C2F(dr1)("xname",cstk(l1),&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,bsiz);
  }

  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* set_posfig_dim(width,height) : give a name to current graphic window */
/*-----------------------------------------------------------------------------------*/
int scisetposfig(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2;
  CheckRhs(2,2);
  CheckLhs(1,1);
  GetRhsVar(1,"i",&m1,&n1,&l1);
  GetRhsVar(2,"i",&m2,&n2,&l2);
  setposfig(istk(l1),istk(l2));
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* /\* dir = 'u','r','d','l'  [default -> 'l' ]  */
/*    /\* fontsize =             [default -> -1   ]  */
/*    /\* format_n = format to use for numbers (unused if strings)  */
/*    /\* seg = flag 1 or 0 draw the base segment of the axis (default 1) */
/*    /\* sub_int  = number of sub tics (default 2)  */
/*    /\* textcolor =            [default -> -1 ] */
/*    /\* ticscolor =            [default -> -1 ] */
/*    /\* tics = 'v' 'r' 'i'     [default -> 'v' ]  */
/*    /\*        gives tics type : vector | range | irange (v,r,i)  */
/*    /\* val  = string matrix  */
/*    /\* x = scalar | vecteur | range | irange  */
/*    /\* y = scalar | vecteur | range | irange  */
/*    /\*  */
/*    /\* constraints :  */
/*    /\* ------------ */
/*    /\*   dir = 'u' | 'd' ==> y= scalar | []  */
/*    /\*                       x= vecteur | range | irange  */
/*    /\*   dir = 'r' | 'l' ==> x= scalar | []  */
/*    /\*                       y= vecteur | range | irange  */
/*    /\*   tics = 'r'          ==> x or y is of size 3 (according to dir) */
/*    /\*   tics = 'i'          ==> x or y is of size 4 */
/*    /\*   val  =              ==> must be of size compatible with x or y  */
/*    /\*                       according to dir  */
/*    /\*-----------------------------------------------------------------------------------*\/ */
int nscixaxis(char *fname,unsigned long fname_len)
{
  /** XXXXX : un point en suspens c'est le "S" ou une adresse est 
   *  stockees ds un unsigned long : est ce sufisant ? 
   */
  static rhs_opts opts[]= { 
    {-1,"dir","c",0,0,0},
    {-1,"fontsize","i",0,0,0},
    {-1,"format_n","c",0,0,0},
    {-1,"seg","i",0,0,0},
    {-1,"sub_int","i",0,0,0},
    {-1,"textcolor","i",0,0,0},
    {-1,"tics","c",0,0,0},
    {-1,"ticscolor","i",0,0,0},
    {-1,"val","S",0,0,0},
    {-1,"x","d",0,0,0},
    {-1,"y","d",0,0,0},
    {-1,NULL,NULL,0,0}
  };

  int minrhs = -1,maxrhs = 0,minlhs=0,maxlhs=1,nopt;
  char dir = 'l', *format = NULL, tics = 'v', **val = NULL;
  int fontsize = -1, sub_int=2, seg_flag = 1,textcolor = -1,ticscolor=-1;
  double *x = NULL,*y = NULL;
  int nx=0,ny=0,ntics;
  int nb_tics_labels = -1;

  nopt = NumOpt();

  SciWin();

  CheckRhs(minrhs,maxrhs+nopt) ;
  CheckLhs(minlhs,maxlhs) ;

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( opts[0].position != -1 ) 
    { 
      CheckLength(opts[0].position,opts[0].m,1);
      dir = *cstk(opts[0].l);
    } 
  if ( opts[1].position != -1 ) 
    {
      CheckScalar(opts[1].position,opts[1].m,opts[1].n);
      fontsize= *istk(opts[1].l);
    }
  if ( opts[2].position != -1 ) 
    { 
      /* verfier ce que l'on recoit avec "" XXX */
      format = cstk(opts[2].l);
    }

  if ( opts[3].position != -1 ) 
    { 
      CheckScalar(opts[3].position,opts[3].m,opts[3].n);
      seg_flag = *istk(opts[3].l);
    }

  if ( opts[4].position != -1 ) 
    { 
      CheckScalar(opts[4].position,opts[4].m,opts[4].n);
      sub_int= *istk(opts[4].l);
    }

  if ( opts[5].position != -1 ) 
    { 
      CheckScalar(opts[5].position,opts[5].m,opts[5].n);
      textcolor= *istk(opts[5].l);
    }

  if ( opts[6].position != -1 ) 
    { 
      CheckLength(opts[6].position,opts[6].m,1);
      tics = *cstk(opts[6].l);
    } 

  if ( opts[7].position != -1 ) 
    { 
      CheckScalar(opts[7].position,opts[7].m,opts[7].n);
      ticscolor= *istk(opts[7].l);
    }

  if ( opts[8].position != -1 ) 
    { 
      val = (char **) opts[8].l; /* F.Leray 08.03.04*/
    } 

  if ( opts[9].position != -1 ) 
    { 
      x = stk(opts[9].l);
      nx = opts[9].m * opts[9].n ;  /* F.Leray OK here opts[9].m and opts[9].n are integers.*/
    }
  else 
    {
      static double x_def[1];
      nx = 1;
      x = x_def ;
      if ( dir == 'l' ) 
	x_def[0] = Cscale.frect[0];
      else if ( dir == 'r' ) 
	x_def[0] = Cscale.frect[2];
    }

  if ( opts[10].position != -1 ) 
    { 
      y = stk(opts[10].l);
      ny = opts[10].m * opts[10].n ;
    }
  else 
    {
      static double y_def[1];
      ny = 1;
      y = y_def ;
      if ( dir == 'd' ) 
	y_def[0] = Cscale.frect[1];
      else if ( dir == 'u' ) 
	y_def[0] = Cscale.frect[3];
    }

  /* compatibility test */
  switch (tics ) 
    {
    case 'r' :
      if ( check_xy(fname,dir,3,opts[9].position,opts[9].m,opts[9].n,opts[9].l,
		    opts[10].position,opts[10].m,opts[10].n,opts[10].l,&ntics)==0) 
	return 0;
      break;
    case 'i' :
      if ( check_xy(fname,dir,4,opts[9].position,opts[9].m,opts[9].n,opts[9].l,
		    opts[10].position,opts[10].m,opts[10].n,opts[10].l,&ntics)==0) 
	return 0;
      break;
    case 'v' :
      if ( check_xy(fname,dir,-1,opts[9].position,opts[9].m,opts[9].n,opts[9].l,
		    opts[10].position,opts[10].m,opts[10].n,opts[10].l,&ntics)==0) 
	return 0;
      break;
    default :
      Scierror(999,"%s: tics has a wrong value \"%c\" should be one of \"r\",\"v\" and \"i\" \r\n", 
	       fname,dir);
      return 0;
    }

  if ( val != 0) 
    {
      /** sciprint("nombre de tics %d\r\n",ntics); **/
      CheckLength( opts[8].position, opts[8].m*opts[8].n,ntics);
      nb_tics_labels = opts[8].m*opts[8].n;
    }

  /* NG beg */
  if (version_flag() == 0)
    Objdrawaxis(dir,tics,x,&nx,y,&ny,val,sub_int,format,fontsize,textcolor,ticscolor,'n',seg_flag,nb_tics_labels);
  else
    Xdrawaxis (dir,tics,x,&nx,y,&ny,val,sub_int,format,fontsize,textcolor,ticscolor,'n',seg_flag);
  /* NG end */

  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------------------------------*/
int scixbascsubwin(char *fname,unsigned long fname_len)
{ 
  unsigned long hdl;
  sciPointObj *subwin,*psubwinobj,*mafigure;
  sciSons *psons;
  integer m,n,l,i;

  SciWin();
  CheckRhs(0,1);
  CheckLhs(0,1);
  if (version_flag() == 0)     {
    mafigure = (sciPointObj *) sciGetCurrentFigure();
    psons = sciGetSons (mafigure);
    if (psons->pnext == NULL) { 
      sciXbasc();
      return 0;
    }
    if (Rhs <= 0) {
      DestroyAllGraphicsSons((sciPointObj *) sciGetSelectedSubWin(mafigure));
      psons = sciGetSons (mafigure); 
      psubwinobj = psons->pointobj;
      if (sciGetEntityType (psubwinobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (psubwinobj)->isselected = TRUE;
      else {
	Scierror(999,"%s:  no sub_window selected !!",fname);
	return 0;
      }
    }
    else {
      switch(VarType(1)) {
      case 1: /* first is a scalar argument so it's a drawnow(subwin) */
	GetRhsVar(1,"h",&m,&n,&l); 
	for (i = 0; i < n*m; i++)
	  {
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
	      if ((sciPointObj *)sciGetSelectedSubWin(mafigure) == subwin ) {
		DestroyAllGraphicsSons((sciPointObj *) subwin);  
		psons = sciGetSons (mafigure); 
		psubwinobj = psons->pointobj;
		if (sciGetEntityType (psubwinobj) == SCI_SUBWIN)
		  pSUBWIN_FEATURE (psubwinobj)->isselected = TRUE; 
		else
		  Scierror(999,"%s:  no sub_window is selected !!",fname);
	      }
	      else
		DestroyAllGraphicsSons((sciPointObj *) subwin);   
	    }
	  }         

	break;
      case 10:/* first is a string argument so it's a drawnow('all') */
	GetRhsVar(1,"c",&m,&n,&l);
	if (strcmp(cstk(l),"all") == 0){
	  sciXbasc();
	  return 0;
	}  
	else {
	  Scierror(999,"%s: 'all' or an handle on a subwindow is expected\r\n",fname);
	  return 0;
	}  
	break; 
      default: 
	Scierror(999,"%s: 'all' or an handle on a subwindow is expected\r\n",fname);
	return 0;
	break;
      }
    }
    sciDrawObj(mafigure);
  }
  LhsVar(1) = 0;
  return 0;
}




/*-----------------------------------------------------------------------------------*/
/* metanet with Scilab graphics */
/*-----------------------------------------------------------------------------------*/
int intmeta(char *fname,unsigned long fname_len)
{
  static int d_iwdim[2]={1000,1000},d_ewdim[2]={600,600};
  int *iwdim = d_iwdim, *ewdim = d_ewdim, window=0;
  static rhs_opts opts[]= { 
    {-1,"gmode","c",0,0,0},
    {-1,"graph","c",0,0,0},
    {-1,"wdim","i",0,0,0},
    {-1,"window","i",0,0,0},
    {-1,"wpdim","i",0,0,0},
    {-1,NULL,NULL,0,0}
  };
  int minrhs = -1,maxrhs = 0,minlhs=0,maxlhs=1,nopt;
  static char d_mode[]="rep";
  char *mode = d_mode;
  char *graph = NULL;

  nopt = NumOpt();
  CheckRhs(minrhs,maxrhs+nopt) ;
  CheckLhs(minlhs,maxlhs) ;

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( opts[0].position != -1 ) 
    { 
      mode = cstk(opts[0].l);
    } 
  if ( opts[1].position != -1 ) 
    { 
      graph = cstk(opts[1].l);
    }

  if ( opts[2].position != -1 ) 
    {
      CheckLength(opts[2].position,opts[2].m*opts[2].n,2);
      iwdim= istk(opts[2].l);
    }

  if ( opts[3].position != -1 ) 
    { 
      CheckScalar(opts[3].position,opts[3].m,opts[3].n);
      window = *istk(opts[3].l);
    }

  if ( opts[4].position != -1 ) 
    { 
      CheckLength(opts[4].position,opts[4].m*opts[4].n,2);
      ewdim= istk(opts[4].l);
    }

  /* Mymetane(graph,iwdim,ewdim,window,mode,0); */
  LhsVar(1) = 0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
/* interface for calling the helpbrowser */
/* when scilab is compiled with gtk */
/* not the perfect place to insert this interface ... */
/*-----------------------------------------------------------------------------------*/
int int_gtkhelp(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2,m3,n3,l3;
  CheckRhs(3,3);
  CheckLhs(0,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  GetRhsVar(2,"c",&m2,&n2,&l2);
  GetRhsVar(3,"c",&m3,&n3,&l3);
#ifdef WITH_GTK 
  Sci_Help(cstk(l1),cstk(l2),cstk(l3));
#endif 
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int scichamp (char *fname,unsigned long fname_len)
{
  return scichamp_G(fname,C2F(champ),fname_len);
}
/*-----------------------------------------------------------------------------------*/
int scichamp1 (char *fname,unsigned long fname_len)
{
  return scichamp_G(fname,C2F(champ1),fname_len);
}
/*-----------------------------------------------------------------------------------*/
/* champ */
/* champ(x,y,fx,fy,[arfact=1.0,rect=[xMin,yMin,xMax,yMax],flag]) */
/* champ1(x,y,fx,fy,[arfact=1.0,rect=[xMin,yMin,xMax,yMax],flag]) */
/*-----------------------------------------------------------------------------------*/
int scichamp_G(char *fname,int (*func) (),unsigned long fname_len)
{
  double arfact_def=1.0;
  double * arfact=&arfact_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3, m4, n4, l4;
  static rhs_opts opts[]= { {-1,"arfact","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  CheckRhs(-1,7) ;
  CheckLhs(0,1) ;

  if (Rhs <= 0) {
    if (strcmp(fname,"champ")==0) 
      sci_demo(fname,"champ(1:10,1:10,rand(10,10),rand(10,10),1.0);",&one);
    else if (strcmp(fname,"champ1")==0) 
      sci_demo(fname,"champ1(1:10,1:10,rand(10,10),rand(10,10),1.0);",&one);
    return 0;
  } 
  else if ( Rhs < 4 ) 
    {
      Scierror(999,"%s: expecting at least 4 arguments\r\n",fname);
      return 0;
    }
  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 5) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,5);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  GetRhsVar(4, "d", &m4, &n4, &l4);
  CheckSameDims(3,4,m3,n3,m4,n4);
  CheckDimProp(2,3,m2 * n2 != n3);
  CheckDimProp(1,3,m1 * n1 != m3); 
  if (m3 * n3 == 0) { LhsVar(1) = 0; return 0;} 

  GetOptionalDoubleArg(5,"arfact",&arfact,1,opts);
  GetRect(6,opts);
  GetStrf(7,opts);

  SciWin();
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != &(def_rect[0])) {Strf[1]='5';} 
  }


  (*func)(stk(l1 ),stk(l2 ),stk(l3 ),stk(l4 ),&m3,&n3,Strf,Rect, arfact, 4L);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int sciplot3d (char *fname,unsigned long fname_len)
{
  return sciplot3d_G(fname,C2F(plot3d),C2F(fac3d),C2F(fac3d2),C2F(fac3d3),fname_len);
}
/*-----------------------------------------------------------------------------------*/
int sciplot3d1 (char *fname,unsigned long fname_len)
{
  return sciplot3d_G(fname,C2F(plot3d1),C2F(fac3d1),C2F(fac3d2),C2F(fac3d3),fname_len);
}
/*-----------------------------------------------------------------------------------*/
int sciplot3d_G(char *fname,
		int (*func) __PARAMS((double *,double *,double *,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func1)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func2) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		int (*func3) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)), 
		unsigned long fname_len)
{
  static double  ebox_def [6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]={2,2,4};
  integer *iflag = iflag_def;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n = 0, n3n = 0, l3n, m3l, n3l, l3l; /*F.Leray 19.03.04 m3n and n3n set to 0.*/

  integer izcol,  isfac;
  double *zcol=NULL;

  static rhs_opts opts[]= { {-1,"alpha","?",0,0,0},
			    {-1,"ebox","?",0,0,0},
			    {-1,"flag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"theta","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  if (Rhs <= 0) {
    sprintf(C2F(cha1).buf,"t=-%%pi:0.3:%%pi;%s(t,t,sin(t)'*cos(t),35,45,'X@Y@Z',[2,2,4]);",fname);
    sci_demo(fname,C2F(cha1).buf, &one);
    return 0;
  }

  CheckRhs(3,8);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 4) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,4);
    Error(999); 
    return(0);
  }

  GetRhsVar(1, "d", &m1, &n1, &l1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  if (m1 * n1 == 0) 	{ LhsVar(1) = 0; return 0;}

  if (Rhs >= 3) {
    /*     third argument can be a matrix z or a list list(z,zcol) */
    switch ( VarType(3) ) 
      {
      case 1 : 
	GetRhsVar(3, "d", &m3, &n3, &l3);
	izcol = 0;
	break;
      case 15 : 
	izcol = 1;
	/* z = list(z,colors) */
	GetRhsVar(3,"l",&m3l,&n3l,&l3l);
	if ( m3l != 2 ) 
	  {
	    Scierror(999,"%s: second argument has a wrong size (%d), expecting a list of size %d\r\n",
		     fname,m3l,2);
	    return 0;
	  }
	GetListRhsVar(3,1,"d",&m3,&n3,&l3);
	GetListRhsVar(3,2,"d",&m3n,&n3n,&l3n);
	zcol  = stk(l3n);
	if (m3n * n3n != n3 &&  m3n*n3n != m3*n3)
	  {
	    Scierror(999,"%s: third argument: color specification has wrong size, expecting %d or %d \r\n",fname,n3,m3*n3);
	    return 0;
	  }
	/* 
	 *   Added by E Segre 4/5/4000. In the case where zcol is a 
	 *   matrix of the same size as z, we set izcol to 2. This
	 *   value is later transmitted to the C2F(fac3dg) routine,
	 *   which has been modified to do the interpolated shading 
	 *    (see the file SCI/routines/graphics/Plo3d.c 
	 */
	if (   m3n*n3n == m3*n3 ) izcol=2  ;
	break ;
      default : 
	OverLoad(3);
	return 0;
      }
  }
  SciWin();
  if (version_flag() == 0) iflag_def[1]=8;
  else iflag_def[1]=2; /* F.Leray 15.06.04 : if switching back to old graphic style */

  GetOptionalDoubleArg(4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(5,"alpha",&alpha,1,opts);
  GetLegend(6,opts);
  GetOptionalIntArg(7,"flag",&iflag,3,opts);
  GetOptionalDoubleArg(8,"ebox",&ebox,6,opts);

  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) {
    if (! (m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3)) {
      Scierror(999,"%s: The three first arguments have incompatible length \r\n",fname);
      return 0;
    }
  } else {
    if (m2 * n2 != n3) {
      Scierror(999,"%s: second and third arguments have incompatible length\r\n",fname);
      return 0;
    }
    if (m1 * n1 != m3) {
      Scierror(999,"%s: first and third arguments have incompatible length\r\n",fname);
      return 0;
    }
    if ( m1*n1 <= 1 || m2*n2 <= 1 ) 
      {
	Scierror(999,"%s: first and second arguments should be of size >= 2\r\n",fname);
	return 0;
      }
  }

  if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0) { LhsVar(1)=0; return 0;} 
  SciWin();
  C2F(scigerase)();

  /******************** 24/015/2002 ********************/
  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
    isfac=1;
  else 
    isfac=0;

  if (version_flag() == 0)
    Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n);/*Adding F.Leray 12.03.04 and 19.03.04*/
  else{
    integer *cvect = NULL,i;
    if((m3n)*(n3n) != 0){
      if((cvect=MALLOC((m3n)*(n3n)*sizeof(integer)))==NULL){
	Scierror(999,"Allocation failed for color matrix in %s\r\n",fname);
	return 0;
      }
    }

    for(i=0;i<m3n*n3n;i++) cvect[i] = (int) zcol[i];

    Xplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),cvect,&m3,&n3,theta,alpha,Legend,iflag,ebox);
    FREE(cvect); cvect = (integer *)NULL;
  }
  /* NG end */
  LhsVar(1)=0;
  return 0;

}
/*-----------------------------------------------------------------------------------*/
int sciplot2d1_1 (char *fname,unsigned long fname_len)
{
  return sciplot2d1_G("plot2d1",1,C2F(plot2d1),fname_len);/* NG */
}
/*-----------------------------------------------------------------------------------*/
int sciplot2d1_2 (char *fname,unsigned long fname_len)
{
  return sciplot2d1_G("plot2d2",2,C2F(plot2d2),fname_len); /* NG */
}
/*-----------------------------------------------------------------------------------*/
int sciplot2d1_3 (char *fname,unsigned long fname_len)
{
  return sciplot2d1_G("plot2d3",3,C2F(plot2d3),fname_len);/* NG */
}
/*-----------------------------------------------------------------------------------*/
int sciplot2d1_4 (char *fname,unsigned long fname_len)
{
  return sciplot2d1_G("plot2d4",4,C2F(plot2d4),fname_len);/* NG */
}
/*-----------------------------------------------------------------------------------*/
int sciplot2d1_G(char *fname,int ptype,int (*func)
		 __PARAMS((char *,double *,double *,integer *,integer *,
			   integer *,char *,char *,double *,integer *,
			   integer,integer,integer)),unsigned long fname_len)
{
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  integer iskip,test;
  integer m1,n1,l1, m2, n2, l2, lt, i, j ;
  /* static char str[]="x=(0:0.1:2*%pi)';plot2d1(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);"; */

  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
			    {-1,"frameflag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"logflag","?",0,0,0},
			    {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,"style","?",0,0,0},
			    {-1,NULL,NULL,0,0}};

  if (Rhs <= 0) {
    /* lauch the default routines depending on the name of the calling funtion */
    if ( strcmp( fname, "plot2d2" ) == 0 )
    {
      char demo[]="x=(0:0.1:2*%pi)';plot2d2(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);";
      sci_demo( fname, demo, &one ) ;
    }
    else if ( strcmp( fname, "plot2d3" ) == 0 )
    {
      char demo[]="x=(0:0.1:2*%pi)';plot2d3(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);";
      sci_demo( fname, demo, &one ) ;
    }
    else if ( strcmp( fname, "plot2d4" ) == 0 )
    {
      char demo[]="x=(0:0.1:2*%pi)';plot2d4(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);";
      sci_demo( fname, demo, &one ) ;
    }
    else
    {
      char demo[]="x=(0:0.1:2*%pi)';plot2d1(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);";
      sci_demo( fname, demo, &one ) ;
    }
    /* sci_demo(fname,str,&one); */
    return 0;
  }
  CheckRhs(1,9); /* to allow plot2dxx(y) */


  iskip=0;
  if ( get_optionals(fname,opts) == 0) return 0;

  if (GetType(1)==10) {
    /* logflags */
    GetLogflags(1,opts);
    iskip=1;
  }

  /* added to support plot2dxx([logflags],y) */
  if (Rhs == 1+iskip)
    {
      if ( FirstOpt() <= Rhs) {
	sciprint("%s: misplaced optional argument, first must be at position %d\r\n",fname,3+iskip);
	Error(999); 
	return(0);
      }

      GetRhsVar(1+iskip, "d", &m2, &n2, &l2);
      /* if (m2 * n2 == 0) { LhsVar(1) = 0; return 0;} */
      CreateVar(2+iskip,"d",  &m2, &n2, &l1);
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      m1 = m2;  n1 = n2;
      for (i = 0; i < m2 ; ++i) 
	for (j = 0 ; j < n2 ;  ++j)
	  *stk( l1 + i + m2*j) = (double) i+1;
    }


  if (Rhs >= 2+iskip) {
    if ( FirstOpt() < 3+iskip) {
      sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	       fname,3+iskip);
      Error(999); 
      return(0);
    }


    /* x */
    GetRhsVar(1+iskip, "d", &m1, &n1, &l1);
    if (iskip==1) 
      if (Logflags[0]=='e') {m1=0;n1=0;}

    /* y */
    GetRhsVar(2+iskip, "d", &m2, &n2, &l2);
    /* if (m2 * n2 == 0) { m1 = 0; n1 = 0;}  */

    test = (m1*n1 == 0) /* x = [] */
      /* x,y vectors of same length */  
      || ((m1 == 1 || n1 == 1) && (m2 == 1 || n2 ==1) && (m1*n1 == m2*n2))
      || ((m1 == m2) && (n1 == n2)) /* size(x) == size(y) */
      /* x vector size(y)==[size(x),.] */
      || ((m1 == 1 && n1 == m2) || (n1 == 1 && m1 == m2)); 

    CheckDimProp(1+iskip,2+iskip,!test);

    if (m1*n1 == 0) { /* default x=1:n */
      CreateVar(Rhs+1,"d",  &m2, &n2, &lt);
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      for (i = 0; i < m2 ; ++i) 
	for (j = 0 ; j < n2 ;  ++j)
	  *stk( lt + i + m2*j) = (double) i+1;
      m1 = m2;
      n1 = n2;
      l1 = lt;
    }
    else if ((m1 == 1 || n1 == 1) && (m2 != 1 && n2 != 1) ) {
      /* a single x vector for mutiple columns for y */
      CreateVar(Rhs+1,"d",  &m2, &n2, &lt);
      for (i = 0; i < m2 ; ++i) 
	for (j = 0 ; j < n2 ;  ++j)
	  *stk( lt + i + m2*j) = *stk(l1 +i);
      m1 = m2;
      n1 = n2;
      l1 = lt;
    }
    else if ((m1 == 1 && n1 == 1) && (n2 != 1) ) {
      /* a single y row vector  for a single x */
      CreateVar(Rhs+1,"d",  &m1, &n2, &lt);
      for (j = 0 ; j < n2 ;  ++j)
	*stk( lt + j ) = *stk(l1);
      n1 = n2;
      l1 = lt;
    }
    else {
      if (m2 == 1 && n2 > 1) { m2 = n2; n2 = 1;}
      if (m1 == 1 && n1 > 1) { m1 = n1; n1 = 1;}
    }
  }

  sciGetStyle(3+iskip,n1,opts);
  GetStrf(4+iskip,opts);
  GetLegend(5+iskip,opts);
  GetRect(6+iskip,opts);
  GetNax(7+iskip,opts);
  if (iskip==0) GetLogflags(8,opts);

  SciWin();
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1]='7';
    if (Legend != def_legend)
      strfl[0]='1';
    GetOptionalIntArg(9,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(9,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }


  /* NG beg */
  if (version_flag() == 0){
    if(ptype == 0) ptype = 1;
    Objplot2d (ptype,Logflags,stk(l1), stk(l2), &n1, &m1, Style, Strf,Legend,Rect, Nax, flagNax);
  }
  else /* NG end */
    (*func)(Logflags,stk(l1),stk(l2),&n1,&m1,Style,Strf,Legend,Rect,Nax,
	    4L,strlen(Strf),strlen(Legend));
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int scicontour2d (char *fname,unsigned long fname_len)
{
  return scicontour2d_G(fname, C2F(contour2),fname_len);
}
/*-----------------------------------------------------------------------------------*/
/* contour2d(x,y,z,nz,[style,strf,leg,rect,nax]) */
/*-----------------------------------------------------------------------------------*/
int scicontour2d_G(char *fname,int (*func) (),unsigned long fname_len)
{
  int flagx=0,nz= 10; /* default number of level curves : 10 */
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  double *znz= NULL;
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  static rhs_opts opts[]= { {-1,"axesflag","?",0,0,0},
			    {-1,"frameflag","?",0,0,0},
			    {-1,"leg","?",0,0,0},
			    {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,"style","?",0,0,0},
			    {-1,NULL,NULL,0,0}};


  if (Rhs <= 0) {
    sci_demo(fname,"contour2d(1:5,1:10,rand(5,10),5);",&one);
    return 0;
  }

  CheckRhs(3,9);

  if ( get_optionals(fname,opts) == 0) return 0;
  if ( FirstOpt() < 5) {
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	     fname,5);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);
  CheckVector(1,m1,n1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  CheckVector(2,m2,n2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  if (m3 * n3 == 0) { LhsVar(1) = 0; return 0; }
  if (m3 == 1 || n3 == 1) {
    Scierror(999,"%s: third argument is a vector, expecting a matrix\r\n",fname);
    return 0;
  }

  CheckDimProp(1,3, m1 * n1 != m3); 
  CheckDimProp(2,3, m2 * n2 != n3); 

  /*     number of level curves */
  if ( Rhs >= 4 ) 
    {
      GetRhsVar(4, "d", &m4, &n4, &l4);
      if (m4 * n4 == 1) {
	flagx = 0;  nz = Max(1,(integer) *stk(l4)), znz= stk(l4);
      } else {
	flagx = 1;  nz = m4 * n4; znz=stk(l4);
      }
    }


  sciGetStyle(5,nz,opts);
  GetStrf(6,opts);    
  GetLegend(7,opts);
  GetRect(8,opts);
  GetNax(9,opts);

  SciWin();
  C2F(scigerase)();

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1] = '7';
    if (Legend != def_legend)
      strfl[0] = '1';
    if (Nax != def_nax)
      strfl[1] = '1';
    GetOptionalIntArg(9,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(9,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }


  (*func)(stk(l1),stk(l2),stk(l3),&m3,&n3,&flagx,&nz,znz,Style,Strf,Legend,Rect,Nax,4L,bsiz);
  LhsVar(1)=0;
  return 0;
}



/*-----------------------------------------------------------------------------------*/
int scicontour2d1 (char *fname,unsigned long fname_len)
{
  int flagx=0,nz=10; /* default number of level curves : 10 */
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,l5;
  double  *hl1, *hl2;
  double *znz= NULL;
  int ix4, i, un = 1;

  CheckRhs(3,4);
  CheckLhs(2,2);

  GetRhsVar(1, "d", &m1, &n1, &l1);
  CheckVector(1,m1,n1);
  GetRhsVar(2, "d", &m2, &n2, &l2);
  CheckVector(2,m2,n2);
  GetRhsVar(3, "d", &m3, &n3, &l3);
  if (m3 * n3 == 0) { LhsVar(1) = 0; return 0; }
  if (m3 == 1 || n3 == 1) {
    Scierror(999,"%s: third argument is a vector, expecting a matrix\r\n",fname);
    return 0;
  }

  CheckDimProp(1,3, m1 * n1 != m3); 
  CheckDimProp(2,3, m2 * n2 != n3); 

  /*     number of level curves */
  if ( Rhs == 4 ) 
    {
      GetRhsVar(4, "d", &m4, &n4, &l4);
      if (m4 * n4 == 1) {
	flagx = 0;  nz = Max(1,(integer) *stk(l4)),znz= stk(l4);
      } else {
	flagx = 1;  nz = m4 * n4; znz=stk(l4);
      }
    }

  ix4 = Max(nz,2);
  CreateVar(Rhs+1,"i",&un,&ix4,&l5);
  for (i =0 ; i < ix4 ; ++i) *istk(l5 + i ) = i+1;
  if (nz == 1) *istk(l5 +1) = 1;

  C2F(contourif)(stk(l1),stk(l2),stk(l3),&m3,&n3,&flagx,&nz,znz,istk(l5));
  C2F(getconts)(&hl1, &hl2, &m1, &n1);
  if (n1 == 0)
    {
      CreateVar(6,"d", &n1, &n1, &l1);
      CreateVar(7,"d", &n1, &n1, &l2);
    }
  else 
    {
      CreateVarFromPtr(6,"d", &m1, &n1, &hl1);
      CreateVarFromPtr(7,"d", &m1, &n1, &hl2);
    }
  LhsVar(1)=6;
  LhsVar(2)=7;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int scixs2ps(char *fname,unsigned long fname_len)
{
  return scixg2psofig_G(fname,"Pos",fname_len,3);
}
/*-----------------------------------------------------------------------------------*/
int scixg2ps(char *fname,unsigned long fname_len)
{
  return scixg2psofig_G(fname,"Pos",fname_len,3);
}
/*-----------------------------------------------------------------------------------*/
int scixs2fig(char *fname,unsigned long fname_len)
{
  return scixg2psofig_G(fname,"Fig",fname_len,3);
}
/*-----------------------------------------------------------------------------------*/
int scixs2gif(char *fname,unsigned long fname_len)
{
  return scixg2psofig_G(fname,"GIF",fname_len,3);
}
/*-----------------------------------------------------------------------------------*/
int scixs2ppm(char *fname,unsigned long fname_len)
{
  return scixg2psofig_G(fname,"PPM",fname_len,3);
}
/*-----------------------------------------------------------------------------------*/
int scixg2psofig_G(char *fname,char *dr,unsigned long fname_len,unsigned long dr_len)
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,flagx = -1,iwin;
  CheckRhs(2,3);

  /* the window number */ 
  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); iwin  = (integer) *stk(l1);
  /* the file name */ 
  GetRhsVar(2,"c",&m2,&n2,&l2);
  /* color or n & b */ 
  if (Rhs >= 3) {GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3); flagx = (integer) *stk(l3); }
  /* nouveau graphique ?????*/
  C2F(xg2psofig)(cstk(l2),&m2,&iwin,&flagx,dr,bsiz,dr_len);
  LhsVar(1)=0;
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int intxs2bmp(char *fname,unsigned long fname_len)
{
  int bOK=0;
#if WIN32

  CheckLhs(0,1);
  CheckRhs(2,2);
  if ( (GetType(2) == sci_strings) && IsAScalar(1) )
    {
      integer m1,n1,l1;
      int figurenum=-1;
      GetRhsVar(1,"i",&m1,&n1,&l1);
      figurenum=*istk(l1);
      if (figurenum>=0)
	{
	  char *FileName=NULL;
	  GetRhsVar(2,"c",&m1,&n1,&l1);
	  FileName=cstk(l1);
	  bOK=Interface_XS2BMP(figurenum,FileName);
	}
      else
	{
	  Scierror(999,"%s: First Argument. Must be >=0.\r\n",fname);
	  return 0;
	}

    }
  else
    {
      if ( IsAScalar(1) )
	{
	  Scierror(999,"%s: First Argument. Must be a integer scalar.\r\n",fname);
	  return 0;
	}
      if ( GetType(2) != sci_strings)
	{
	  Scierror(999,"%s: Second Argument. Must be a string.\r\n",fname);
	  return 0;
	}
    }

#else
  sciprint("%s: Only for Windows.\r\n",fname);
  bOK=0;
#endif
  return bOK;
}
/*-----------------------------------------------------------------------------------*/
int intxs2emf(char *fname,unsigned long fname_len)
{
  int bOK=0;
#if WIN32

  CheckLhs(0,1);
  CheckRhs(2,2);
  if ( (GetType(2) == sci_strings) && IsAScalar(1) )
    {
      integer m1,n1,l1;
      int figurenum=-1;
      GetRhsVar(1,"i",&m1,&n1,&l1);
      figurenum=*istk(l1);
      if (figurenum>=0)
	{
	  char *FileName=NULL;
	  GetRhsVar(2,"c",&m1,&n1,&l1);
	  FileName=cstk(l1);
	  bOK=Interface_XS2EMF(figurenum,FileName);
	}
      else
	{
	  Scierror(999,"%s: First Argument. Must be >=0.\r\n",fname);
	  return 0;
	}

    }
  else
    {
      if ( IsAScalar(1) )
	{
	  Scierror(999,"%s: First Argument. Must be a integer scalar.\r\n",fname);
	  return 0;
	}
      if ( GetType(2) != sci_strings)
	{
	  Scierror(999,"%s: Second Argument. Must be a string.\r\n",fname);
	  return 0;
	}
    }

#else
  sciprint("%s: Only for Windows.\r\n",fname);
  bOK=0;
#endif
  return bOK;
}
/*-----------------------------------------------------------------------------------*/
/* rect(x,y,w,h) */
/*-----------------------------------------------------------------------------------*/
int scirect(char *fname,unsigned long fname_len)
{
  long hdl;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  SciWin();
  CheckRhs(1,4);
  switch ( Rhs ) 
    {
    case 1 :
      GetRhsVar(1,"d",&m1,&n1,&l1); 
      CheckLength(1,m1*n1,4);
      if (version_flag() == 0){
	sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
	if (strcmp(fname,"xrect")==0){
	  int foreground = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	  Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
		   &foreground,NULL,FALSE,TRUE,0,&hdl,FALSE);
	}
	else{ /* xfrect case */
	  int foreground = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	  Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),
		   NULL,&foreground,TRUE,FALSE,0,&hdl,FALSE);
	}

        if ( hdl < 0 )
        {
          return -1 ;
        }

	if (pSUBWIN_FEATURE(psubwin)->surfcounter>0) {
	  Merge3d(psubwin); /* an addtomerge function should be much more efficient */
	  sciDrawObj(sciGetCurrentFigure ());}
	else
	  sciDrawObjIfRequired(sciGetCurrentObj ());
      }
      else
	Xrect(fname,fname_len,stk(l1),stk(l1+1),stk(l1+2),stk(l1+3));
      break;
    case 4 :
      GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);
      GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3);
      GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);
      if (version_flag() == 0){
	sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());
	if (strcmp(fname,"xrect")==0) {	
	  int foreground = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	  Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
		   &foreground,NULL,FALSE,TRUE,0,&hdl,FALSE);
	}
	else{
	  int foreground = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	  Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
		   NULL,&foreground,TRUE,FALSE,0,&hdl,FALSE);
	}

        if ( hdl < 0 )
        {
          return -1 ;
        }

	if (pSUBWIN_FEATURE(psubwin)->surfcounter>0) {
	  Merge3d(psubwin); /* an addtomerge function should be much more efficient */
	  sciDrawObj(sciGetCurrentFigure ());}
	else
	  sciDrawObjIfRequired(sciGetCurrentObj ());
      }
      else
	Xrect(fname,fname_len,stk(l1),stk(l2),stk(l3),stk(l4));
      break;
    default :
      Scierror(999,"%s: wrong number of rhs argumens (%d), rhs must be 1 or 4\r\n",fname,Rhs);
    }
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
/* scirects */
/*-----------------------------------------------------------------------------------*/
int scirects(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2;
  long  hdl;
  int i;
  SciWin();
  CheckRhs(1,2);

  GetRhsVar(1,"d",&m1,&n1,&l1);

  if (m1 != 4) {
    Scierror(999,"%s: rects has a wrong size (4,n) expected \r\n",fname);
    return 0;
  }


  if (Rhs == 2) 
    {
      GetRhsVar(2,"i",&m2,&n2,&l2);
      CheckVector(2,m2,n2);
      if (m2 * n2 != n1) {
	Scierror(999,"%s: first and second arguments have incompatible length\r\n",fname);
	return 0;
      }
    }
  else 
    {
      int i;
      m2=1,n2=n1; CreateVar(2,"i",&m2,&n2,&l2);
      for (i = 0; i < n2; ++i)  *istk(l2 + i) = 0;
    }  
  /* NG beg */
  if (version_flag() == 0){
    sciPointObj *psubwin = (sciPointObj *)sciGetSelectedSubWin (sciGetCurrentFigure ());

    for (i = 0; i < n1; ++i) { 
      /*       j = (i==0) ? 0 : 1; */
      if (*istk(l2+i) == 0){
	/** fil(i) = 0 rectangle i is drawn using the current line style (or color).**/
	/* color setting is done now */
	int foreground = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	Objrect (stk(l1+(4*i)),stk(l1+(4*i)+1),stk(l1+(4*i)+2),stk(l1+(4*i)+3),
		 &foreground,NULL,FALSE,TRUE,0,&hdl,FALSE);
      }
      else  
	if (*istk(l2+i) < 0){
	  /** fil(i) < 0 rectangle i is drawn using the line style (or color) **/
	  int tmp = - (*istk(l2+i));
	  Objrect (stk(l1+(4*i)),stk(l1+(4*i)+1),stk(l1+(4*i)+2),stk(l1+(4*i)+3),
		   &tmp,NULL,FALSE,TRUE,0,&hdl,FALSE);
	}
	else         
	  /** fil(i) > 0   rectangle i is filled using the pattern (or color) **/
	  Objrect (stk(l1+(4*i)),stk(l1+(4*i)+1),stk(l1+(4*i)+2),stk(l1+(4*i)+3),
		   NULL,istk(l2+i),TRUE,FALSE,0,&hdl,FALSE);
    }
    /** construct Compound and make it current object **/
    sciSetCurrentObj (ConstructCompoundSeq (n1));  

    if (pSUBWIN_FEATURE(psubwin)->surfcounter>0) {
      Merge3d(psubwin); /* an addtomerge function should be much more efficient */
			/*     EraseAndOrRedraw(sciGetSelectedSubWin (sciGetCurrentFigure ()));} */ /* inhibit EraseAndOrRedraw for now F.Leray 20.12.04 */
      sciDrawObj(sciGetCurrentFigure ());}
    else
      sciDrawObjIfRequired(sciGetCurrentObj ());

  }   
  else
    Xrects(fname,fname_len,istk(l2), n1,stk(l1));
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
int sciseteventhandler(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,ierr;
  integer verb=0,win,na,v;
  double dv;
  SciWin();
  CheckRhs(1,1);
  CheckLhs(0,1);
  C2F(dr1)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
  if (Rhs == 1) 
    {
      GetRhsVar(1,"c",&m1,&n1,&l1);
      seteventhandler(&win,cstk(l1),&ierr);
    }
  else 
    seteventhandler(&win,"",&ierr);
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
int scizoomrect(char *fname,unsigned long fname_len)
{
  int m,n,l;
  int x_pixel=0,y_pixel=0;
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  SciWin();
  if (Rhs <= 0)
  {
    zoom();
  }
  else
  {
    GetRhsVar(1,"d",&m,&n,&l); 
    CheckLength(1,4,m*n);
    zoom_box(stk(l),&x_pixel,&y_pixel);
  }

  LhsVar(1)=0; 
  return 0;
} 

/*-----------------------------------------------------------------------------------*/
int sciunzoom(char *fname,unsigned long fname_len)
{
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  SciWin();
  if ( Rhs == 0 )
  {
    unzoom();
  }
  else
  {
    int m,n,l,i;
    GetRhsVar(1,"h",&m,&n,&l); 
    for (i=0;i<m*n;i++)
    {
      unzoom_one_axes((sciPointObj*)sciGetPointerFromHandle((long) *hstk(l+i))); /** Correction Bug 1476 + Warning Windows **/
    }
  }
  LhsVar(1)=0; 
  return 0;
} 


/*-----------------------------------------------------------------------------------*/
int sci_delete(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,num,lw;
  unsigned long hdl;
  sciPointObj *pobj, *pparentfigure;

  CheckRhs(0,1);
  CheckLhs(0,1);
  /*SciWin();*/
  switch(VarType(1))
    {
    case 9: /* delete Entity given by a handle */
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */
      if (m1!=1||n1!=1) {
	lw = 1 + Top - Rhs;
	C2F(overload)(&lw,"delete",6);return 0;}
      if (Rhs == 2)
	GetRhsVar(2,"c",&m2,&n2,&l2); /* Gets the command name */
      hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */
      break;
    case 10: /* delete("all") */
      CheckRhs(1,1);
      GetRhsVar(1,"c",&m2,&n2,&l2);
      if (strcmp(cstk(l2),"all") == 0)
	{
	  SciWin();
	  sciXbasc();return 0;
	}
      else
	{
	  Scierror(999,"%s :Incorrect argument\r\n",fname);
	  return 0;
	}
      break;
    default:
      CheckRhs(0,0);
      hdl = (unsigned long ) sciGetHandle((sciPointObj *)sciGetCurrentObj());
      break;
    }

  pobj = sciGetPointerFromHandle(hdl);
  if (pobj == (sciPointObj *)NULL) {
    Scierror(999,"%s :the handle is not valid\r\n",fname);
    return 0;
  }
  num= sciGetNumFigure (pobj);
    
  if ((Rhs == 2) && (strcmp(cstk(l2),"callback") == 0))
  {
    sciDelCallback((sciPointObj *)pobj);
  }
  else
{

    sciEntityType objType    = sciGetEntityType( pobj ) ;

    if (sciGetParentFigure(pobj) != NULL && sciGetEntityType(pobj) != SCI_FIGURE)
    {
      /* I added && sciGetEntityType(pobj) != SCI_FIGURE at last !!!!!!!! F.Leray 09.04.04 */
      sciPointObj * parentSubWin = sciGetParentSubwin( pobj ) ;
      BOOL selected = sciGetIsSelected( pobj ) ;
      pparentfigure = sciGetParentFigure(pobj);
      sciSetCurrentObj( sciGetParent(pobj) ) ; /* A LAISSER F.Leray 25.03.04*/
      sciDelGraphicObj( pobj ) ; /* don't use pobj after this point */
      pobj = NULL ;

      /* If there is a merge object, we must update the axes */
      if ( objType != SCI_SUBWIN && pSUBWIN_FEATURE(parentSubWin)->facetmerge )
      {
        updateMerge( parentSubWin ) ;
      }

      /* test here: we could have deleted the selected subwindow, we must choose an other */
      /* We must always have one selected subwindow (if at least one subwindow exists) */
      if ( objType == SCI_SUBWIN && selected )
      {
        /* we have to select antoher subwindow if one exists at least */
        sciSelectFirstSubwin( pparentfigure ) ;
      }

      /* redraw the window */
      sciDrawObj( pparentfigure ) ;
    }
    else if(sciGetEntityType(pobj) == SCI_FIGURE) /* F.Leray 13.04.04: We delete the special object Figure !!*/
    {
      C2F(deletewin)( &num);
    }

  }

  LhsVar(1)=0;
  return 0;
}


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
int C2F(scigerase)()
{
  integer verb=0,lstr,v,na,win;
  double dv;
  char str[4];
  C2F(xgetg)("auto clear",str,&lstr,11L,4L);
  if (strcmp(str,"on") == 0) {
    C2F(dr1)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
    C2F(dr1)("xclear",C2F(cha1).buf,&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,bsiz);
    C2F(dr1)("xstart",C2F(cha1).buf,&win,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,bsiz);
  }
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
/* get_style */
/*-----------------------------------------------------------------------------------*/
static int get_style(char *fname,int pos, int n1,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt,un=1,ix,i,l1;

  Nbvars=Max(Nbvars,Rhs);


  if (pos < first_opt) /* regular argument  */
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "i", &m, &n, &l);
	if (m * n < n1) {
	  Scierror(999,"%s: style is too small (%d < %d)\r\n",fname,m*n,n1);
	  return 0;
	}
	if (n1==1&&m*n==1) {
	  ix = 2;
	  CreateVar(Nbvars+1,"i",&un,&ix,&l1);
	  *istk(l1)=*istk(l);
	  *istk(l1+1)=1;
	  l=l1;
	}
	Style=istk(l);
      }
      else /* zero type argument --> default value */
	{
	  ix = Max(n1,2);
	  CreateVar(Nbvars+1,"i",&un,&ix,&l);
	  for (i = 0 ; i < n1 ; ++i)  *istk(l + i) = i+1;
	  if (n1 == 1)  *istk(l +1) = 1;
	  Style=istk(l);
	}
    }
  else if ((kopt=FindOpt("style",opts))) { /* named argument: style=value */
    GetRhsVar(kopt, "i", &m, &n, &l);
    if (m * n < n1) {
      Scierror(999,"%s: style is too small (%d < %d)\r\n",fname,m*n,n1);
      return 0;
    }
    if (n1==1&&m*n==1) {
      ix = 2;
      CreateVar(Nbvars+1,"i",&un,&ix,&l1);
      *istk(l1)=*istk(l);
      *istk(l1+1)=1;
      l=l1;
    }
    Style=istk(l);
  }
  else /* unspecified argument --> default value */
    {
      ix = Max(n1,2);
      CreateVar(Nbvars+1,"i",&un,&ix,&l);
      for (i = 0 ; i < n1 ; ++i)  *istk(l + i) = i+1;
      if (n1 == 1)  *istk(l +1) = 1;
      Style=istk(l);
    }
  return 1;
}
/*-----------------------------------------------------------------------------------*/
/* get_rect */
/*-----------------------------------------------------------------------------------*/
static int get_rect(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt,i;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "d", &m, &n, &l);
	if (m * n != 4) { 
	  Scierror(999,"%s: rect has wrong size (%d), 4 expected \r\n",fname,m*n); 
	  return 0;
	}
	Rect = stk(l);
	
	for(i=0;i<4;i++)
	  if(finite(Rect[i]) == 0){
	    Scierror(999,"%s: rect has Nan or Inf values, 4 finite values expected \r\n",fname,m*n); 
	    return 0;
	  }
      }
      else 
	{
	  /** global value can be modified  **/
	  /*   def_rect[0] = def_rect[1] = 0.0; def_rect[2]=def_rect[3]=10.0; */  /* F.Leray 29.04.04 */
	  def_rect[0] = def_rect[1] = 0.0; def_rect[2]=def_rect[3]=0.0;
	  Rect = def_rect ;
	}
    }
  else if ((kopt=FindOpt("rect",opts))) {/* named argument: rect=value */
    GetRhsVar(kopt, "d", &m, &n, &l);
    if (m * n != 4) { 
      Scierror(999,"%s: rect has wrong size (%d), 4 expected \r\n",fname,m*n); 
      return 0;
    }
    Rect = stk(l);
    
    for(i=0;i<4;i++)
      if(finite(Rect[i]) == 0){
	Scierror(999,"%s: rect has Nan or Inf values, 4 finite values expected \r\n",fname,m*n); 
	return 0;
      }
  }
  else
    {
      /** global value can be modified  **/
      /*   def_rect[0] = def_rect[1] = 0.0; def_rect[2]=def_rect[3]=10.0; */ /* F.Leray 29.04.04 */
      def_rect[0] = def_rect[1] = 0.0; def_rect[2]=def_rect[3]=0.0;
      Rect = def_rect ;
    }
  /*if ((Rect != def_rect)&&(Strf !=def_strf)) {
    }*/
    
  return 1;
}
/*-----------------------------------------------------------------------------------*/
static int get_strf(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt)
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "c", &m, &n, &l);
	if (m * n != 3) { 
	  Scierror(999,"%s: strf has wrong size (%d), 3 expected \r\n",fname,m*n); 
	  return 0;
	}
	Strf = cstk(l); 
      }
      else
	{
	  /* def value can be changed */
	  strcpy(def_strf,DEFSTRF);  Strf = def_strf ;
	}
    }
  else if ((kopt=FindOpt("strf",opts))) {
    GetRhsVar(kopt, "c", &m, &n, &l);
    if (m * n != 3) { 
      Scierror(999,"%s: strf has wrong size (%d), 3 expected \r\n",fname,m*n); 
      return 0;
    }
    Strf = cstk(l); 
  }
  else
    {
      /* def value can be changed */
      
      if (version_flag() == 0){
	strcpy(def_strf,DEFSTRFN);  Strf = def_strf ;
      }
      else {
	strcpy(def_strf,DEFSTRF);  Strf = def_strf ;
      }
    }
  return 1;
}

/*-----------------------------------------------------------------------------------*/
static int get_legend(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "c", &m, &n, &l);
	Legend = cstk(l); 
      }
      else
	{
	  Legend = def_legend ;
	}
    }
  else if ((kopt=FindOpt("leg",opts))) {
    GetRhsVar(kopt, "c", &m, &n, &l);
    Legend = cstk(l); 
  }
  else
    {
      Legend = def_legend ;
    }
  return 1;
}

/*-----------------------------------------------------------------------------------*/
static int get_nax(int pos,rhs_opts opts[])
{
  int i,m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) {
    if (VarType(pos)) {
      GetRhsVar(pos, "i", &m, &n, &l);
      CheckLength(pos,m*n,4);
      for (i = 0 ; i < 4; ++i) *istk(l+i) = Max((integer) *istk(l+i),0); /*POLPOTH09042001*/
      Nax=istk(l);
      flagNax = TRUE;
    }
    else
      {
	Nax=def_nax;
	flagNax = FALSE;
      }
  }
  else if ((kopt=FindOpt("nax",opts))) {
    GetRhsVar(kopt, "i", &m, &n, &l);
    CheckLength(kopt,m*n,4);
    for (i = 0 ; i < 4; ++i) *istk(l+i) = Max((integer) *istk(l+i),0); /*POLPOTH09042001*/
    Nax=istk(l);
    flagNax = TRUE;
  }
  else 
    {
      Nax=def_nax;
      flagNax = FALSE;
    } 
  return 1;
}


/*-----------------------------------------------------------------------------------*/
static int get_zminmax(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
        GetRhsVar(pos, "d", &m, &n, &l);
	if (m * n != 2) { 
	  Scierror(999,"%s: zminmax has wrong size (%d), 2 expected \r\n",fname,m*n); 
	  return 0;
	}
	Zminmax = stk(l); 
      }
      else 
	{
	  /** global value can be modified  **/
	  def_zminmax[0] = def_zminmax[1] = 0.0;
	  Zminmax = def_zminmax ;
	}
    }
  else if ((kopt=FindOpt("zminmax",opts))) {/* named argument: rect=value */
    GetRhsVar(kopt, "d", &m, &n, &l);
    if (m * n != 2) { 
      Scierror(999,"%s: zminmax has wrong size (%d), 2 expected \r\n",fname,m*n); 
      return 0;
    }
    Zminmax = stk(l); 
  }
  else
    {
      /** global value can be modified  **/
      def_zminmax[0] = def_zminmax[1] = 0.0;
      Zminmax = def_zminmax ;
    }
    
  return 1;
}

/*-----------------------------------------------------------------------------------*/
static int get_colminmax(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    {
      if (VarType(pos)) 
	{
	  GetRhsVar(pos, "i", &m, &n, &l);
	  CheckLength(pos,m*n,2);
	  Colminmax=istk(l);
	}
      else
	{
	  /** global value can be modified  **/
	  def_colminmax[0] = def_colminmax[1] = 0;
	  Colminmax=def_colminmax;
	}
    }
  else if ((kopt=FindOpt("colminmax",opts))) 
    {
      GetRhsVar(kopt, "i", &m, &n, &l);
      CheckLength(kopt,m*n,2);
      Colminmax=istk(l);
    }
  else 
    {
      /** global value can be modified  **/
      def_colminmax[0] = def_colminmax[1] = 0;
      Colminmax=def_colminmax;
    } 
  return 1;
}

/*-----------------------------------------------------------------------------------*/
static int get_colout(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    {
      if (VarType(pos)) 
	{
	  GetRhsVar(pos, "i", &m, &n, &l);
	  CheckLength(pos,m*n,2);
	  ColOut = istk(l);
	}
      else
	{
	  /** global value can be modified  **/
	  def_colout[0] = def_colout[1] = -1;
	  ColOut = def_colout;
	}
    }
  else if ((kopt=FindOpt("colout",opts))) 
    {
      GetRhsVar(kopt, "i", &m, &n, &l);
      CheckLength(kopt,m*n,2);
      ColOut=istk(l);
    }
  else 
    {
      /** global value can be modified  **/
      def_colout[0] = def_colout[1] = -1;
      ColOut = def_colout;
    } 
  return 1;
}
/*-----------------------------------------------------------------------------------*/
static int get_with_mesh(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    {
      if (VarType(pos)) 
	{
	  GetRhsVar(pos, "b", &m, &n, &l);
	  CheckLength(pos,m*n,1);
	  WithMesh = *(istk(l));
	}
      else
	{
	  /** global value can be modified  **/
	  def_with_mesh = FALSE;
	  WithMesh = def_with_mesh;
	}
    }
  else if ((kopt=FindOpt("mesh",opts))) 
    {
      GetRhsVar(kopt, "b", &m, &n, &l);
      CheckLength(kopt,m*n,1);
      WithMesh = *(istk(l));
    }
  else 
    {
      /** global value can be modified  **/
      def_with_mesh = FALSE;
      WithMesh = def_with_mesh;
    } 
  return 1;
}

/*-----------------------------------------------------------------------------------*/
static int get_logflags(char *fname,int pos,rhs_opts opts[])
{
  int m,n,l,first_opt=FirstOpt(),kopt;
  
  if (pos < first_opt) /* regular argument  */
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "c", &m, &n, &l);
	if ((m * n != 2)&&(m * n != 3)) {
	  Scierror(999,"%s: logflag has wrong size (%d), expected 2 or 3\r\n",fname,m*n);
	  return 0;
	}
	if (m * n == 2) {
	  if ((*cstk(l)!='l'&&*cstk(l)!='n')||(*cstk(l+1)!='l'&&*cstk(l+1)!='n')){
	    Err=pos;
	    Error(116);
	    return 0;
	  }
	  logflags[0]='g';logflags[1]=*cstk(l);logflags[2]=*cstk(l+1);
	  Logflags=logflags;}
	else {
	  if (((*cstk(l)!='g')&&(*cstk(l)!='e')&&(*cstk(l)!='o')) || 
	      (*cstk(l+1)!='l'&&*cstk(l+1)!='n') || 
	      (*cstk(l+2)!='l'&&*cstk(l+2)!='n')){
	    Err=pos;
	    Error(116);
	    return 0;
	  }
	  Logflags=cstk(l);
	}
      }
      else /* zero type argument --> default value */
	{
	  Logflags=def_logflags;
	}
    }
  else if ((kopt=FindOpt("logflag",opts))) { /* named argument: style=value */
    GetRhsVar(kopt,"c", &m, &n, &l);
    if ((m * n != 2)&&(m * n != 3)) {
      Scierror(999,"%s: logflag has wrong size (%d), expected 2 or 3\r\n",fname,m * n);
      return 0;
    }
    if (m * n == 2) {
      if ((*cstk(l)!='l'&&*cstk(l)!='n')||(*cstk(l+1)!='l'&&*cstk(l+1)!='n')){
	Err=kopt;
	Error(116);
	return 0;
      }
      logflags[0]='g';logflags[1]=*cstk(l);logflags[2]=*cstk(l+1);
      Logflags=logflags;}
    else {
      if (((*cstk(l)!='g')&&(*cstk(l)!='e')&&(*cstk(l)!='o')) || 
	  (*cstk(l+1)!='l'&&*cstk(l+1)!='n') || 
	  (*cstk(l+2)!='l'&&*cstk(l+2)!='n')){
	Err=kopt;
	Error(116);
	return 0;
      }
      
      Logflags=cstk(l);
    }
  }
  else /* unspecified argument --> default value */
    {
      Logflags=def_logflags;
    }
  return 1;
}


/*-----------------------------------------------------------------------------------*/
static int get_optional_double_arg(fname,pos,name,value,sz,opts) 
     char *fname, *name;
     int pos,sz;
     double **value;
     rhs_opts opts[];
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "d", &m, &n, &l);
	CheckLength(pos,m*n,sz)
	  *value = stk(l);
      }
    }
  else if ((kopt=FindOpt(name,opts))) {
    GetRhsVar(kopt,"d", &m, &n, &l);
    CheckLength(kopt,m*n,sz)
      *value = stk(l);
  }
  return 1;
}
/*-----------------------------------------------------------------------------------*/
static int get_optional_int_arg(fname,pos,name,value,sz,opts) 
     char *fname, *name;
     int pos,sz;
     int **value;
     rhs_opts opts[];
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "i", &m, &n, &l);
	CheckLength(pos,m*n,sz)
	  *value = istk(l);
      }
    }
  else if ((kopt=FindOpt(name,opts))) {
    GetRhsVar(kopt,"i", &m, &n, &l);
    CheckLength(kopt,m*n,sz)
      *value = istk(l);
  }
  return 1;
}
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
void SetTKGraphicalMode(BOOL SetTKMode)
{
  TKModeON=SetTKMode;
}
/*-----------------------------------------------------------------------------------*/
BOOL IsTKGraphicalMode(void)
{
  return TKModeON;
}
/*-----------------------------------------------------------------------------------*/
/* set or create a graphic window */
/*-----------------------------------------------------------------------------------*/
int C2F(sciwin)()
{
  integer verb=0,win=0,v=1,na;
  double dv;
  C2F(dr)("xget","window",&verb,&win,&na,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  C2F(dr)("xset","window",&win,&v,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  if (version_flag() == 0) { 
    return sciSwitchWindow(&win); 
  }
  return 0;
} 
/*-----------------------------------------------------------------------------------*/
int check_xy(char *fname, char dir, int mn, int xpos, int xm, int xn, 
	     long unsigned int xl, int ypos, int ym, int yn, long unsigned int yl, 
	     int *ntics)
{
  switch ( dir ) 
    {
    case 'l': case 'r' : 
      /* x must be scalar */
      if ( xpos != -1 ) CheckScalar(xpos,xm,xn);
      /* y must be of size mn */
      if ( mn != -1 ) CheckDims(ypos,ym,yn,1,mn);
      switch (mn) 
	{
	case 3: 
	  *ntics = (int) *stk(yl+2)+1;break;
	case 4: 
	  *ntics = (int) *stk(yl+3)+1;break;
	case -1: 
	  *ntics =  ym*yn;break;
	}
      break;
    case 'u' : case 'd' : 
      /* y must be scalar */
      if ( ypos  != -1 ) CheckScalar(ypos,ym,yn);
      /* x must be of size mn */
      if (mn != -1 ) CheckDims(xpos,xm,xn,1,mn);
      switch (mn) 
	{
	case 3: 
	  *ntics =  (int) *stk(xl+2)+1;break;
	case 4: 
	  *ntics =  (int) *stk(xl+3)+1;break;
	case -1: 
	  *ntics =  xm*xn;break;
	}
      break;
    default :
      Scierror(999,"%s: dir has a wrong value \"%c\" should be one of \"u\",\"d\",\"r\" and \"l\"\r\n", 
	       fname,dir);
      return 0;
    }
  return 1;
}
/*-----------------------------------------------------------------------------------*/


int sci_demo (char *fname,char *code,integer *flagx)
{
	int mlhs=0,mrhs=1,ibegin=1, l1, m1=strlen(code), n1=1;
	static char name[] = "execstr" ;
	Nbvars=0;
	
	CreateVar(1, "c", &m1, &n1, &l1);
	strcpy(cstk(l1),code);
	
	/* back conversion to Scilab coding */
	Convert2Sci(1);
	
	/* execute the Scilab execstr function */
	if ( *flagx == 1){
		sciprint("\r\n");
		sciprint("Demo of %s()\r\n",fname);
		sciprint("========================================");
		sciprint("\r\n");
		sciprint("%s\r\n",code);
		sciprint("\r\n");
	}
	
	C2F(recu).krec=-1; /* added to avoid recursion errors */
	SciString(&ibegin,name,&mlhs,&mrhs);
	
	/* check if an error has occured while running a_function */
	LhsVar(1) = 0; 
	return 0;
}


/*-----------------------------------------------------------------------------------*/
/* In case of a Logarithmic scale, we pick up the Min Strictly Positive */
double  sciFindLogMinSPos(double *x, int n) 
{
  int i;
  double XMIN;
  double * xtmp;
  int compteur = 0;  

  if ( (xtmp = MALLOC(n*sizeof(double))) == NULL)
    { 
      Scistring(" xtmp allocation failed \n");
      return -1;
    }  
  
  /* 1. sort the positive value and store them into xtmp 
     2. pick up the lowest value inside xtmp (<=> lowest positive value inside x) */
  for(i=0;i<n;i++)
    if(x[i] > 0.){
      xtmp[compteur] = x[i];
      compteur++;
    }

  if(compteur != 0){
    XMIN = xtmp[0];
    for(i=0;i<compteur;i++)
      if(XMIN > xtmp[i]) XMIN = xtmp[i];
     
    FREE(xtmp);
    return XMIN;
  }
  else{
    FREE(xtmp);
     
    return -1.;
  }
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

