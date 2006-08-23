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


#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/
extern void C2F(gsorts)(char **data, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortd)(double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortint)(int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortuint)(unsigned int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortshort)(short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortushort)(unsigned short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortchar)(char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(gsortuchar)(unsigned char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
extern int C2F(dsort) _PARAMS((double *count, int *n, int *index));
extern int IsAScalar(int RhsNumber);
extern int C2F(deletewin)(integer *number);
#if _MSC_VER
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
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3/* ,v */;
  integer un=1,lex,iflag = 0;
  char iord[2] ; /* = { 'd','\0' }; */
  char typex[10]; /* = { 'g' ,'\0'} ; */
  /* double dv; */
  /* integer iv; */
  SciIntMat Im;
  int Type;
  iord[0] = 'd'; iord[1]='\0';
  typex[0] = 'g'; typex[1] = '\0';

  CheckRhs(1,3);

  Type=VarType(1);
  switch ( VarType(1)) 
    {
    case 10 : 
      GetRhsVar(1,"S",&m1,&n1,&S);
      break;
    case 1 :
      GetRhsVar(1,"d",&m1,&n1,&l1);
      break;
    case 8:
      GetRhsVar(1,"I",&m1,&n1,&Im);
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


  /** Scalar matrix **/
  iflag = 0;
  if (Lhs == 2) iflag = 1;

  if ( typex[0] == 'l') 
    {
      if (typex[1] == 'r') {
	CreateVar(Rhs+1,"i",&m1,&un,&lex);
      } else  {
	CreateVar(Rhs+1,"i",&un,&n1,&lex);
      }
    }
  else 
      CreateVar(Rhs+1,"i",&m1,&n1,&lex);

  LhsVar(1)=1;
  if (Type==1) 
    C2F(gsortd)(stk(l1),istk(lex),&iflag,&m1,&n1,typex,iord);
  else if (Type==8) {
    if (Im.it==4)
      C2F(gsortint)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    else if(Im.it == 14)
      C2F(gsortuint)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    else if (Im.it==2)
      C2F(gsortshort)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    else if(Im.it == 12)
      C2F(gsortushort)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    else if (Im.it==1)
      C2F(gsortchar)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
    else if(Im.it == 11)
      C2F(gsortuchar)(Im.D,istk(lex),&iflag,&m1,&n1,typex,iord);
  }
  else if (Type==10) {
    C2F(gsorts)(S,istk(lex),&iflag,&m1,&n1,typex,iord);
    CreateVarFromPtr(Rhs+Lhs,"S", &m1, &n1, S);
    LhsVar(1)=Rhs+Lhs;
  }

  if ( Lhs == 2 ) LhsVar(2)=Rhs+1;
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

  GetOptionalDoubleArg(fname,5,"arfact",&arfact,1,opts);
  GetRect(fname,6,opts,&Rect);
  GetStrf(fname,7,opts,&Strf);

  SciWin();
  C2F(scigerase)();

  if ( isDefStrf( Strf ) ) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if ( isDefRect( Rect ) ) {Strf[1]='5';} 
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

  GetOptionalDoubleArg(fname,4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(fname,5,"alpha",&alpha,1,opts);
  GetLabels(fname,6,opts,&Legend);
  GetOptionalIntArg(fname,7,"flag",&iflag,3,opts);
  GetOptionalDoubleArg(fname,8,"ebox",&ebox,6,opts);

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
    GetLogflags(fname,1,opts,&Logflags);
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

  sciGetStyle(fname,3+iskip,n1,opts,&Style);
  GetStrf(fname,4+iskip,opts,&Strf);
  GetLegend(fname,5+iskip,opts,&Legend);
  GetRect(fname,6+iskip,opts,&Rect);
  GetNax(7+iskip,opts,&Nax,&flagNax);
  if (iskip==0) GetLogflags(fname,8,opts,&Logflags);

  SciWin();
  C2F(scigerase)();

  if ( isDefStrf( Strf ) ) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if ( !isDefRect( Rect ) )
      strfl[1]='7';
    if ( !isDefLegend( Legend ) )
      strfl[0]='1';
    GetOptionalIntArg(fname,9,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(fname,9,"axesflag",&axes,1,opts);
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


  sciGetStyle(fname,5,nz,opts,&Style);
  GetStrf(fname,6,opts,&Strf);
  GetLegend(fname,7,opts,&Legend);
  GetRect(fname,8,opts,&Rect);
  GetNax(9,opts,&Nax,&flagNax);

  SciWin();
  C2F(scigerase)();

  if ( isDefStrf( Strf ) ) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if ( !isDefRect( Rect ) )
      strfl[1] = '7';
    if ( !isDefLegend( Legend ) )
      strfl[0] = '1';
    if ( !isDefNax( Nax ) )
      strfl[1] = '1';
    GetOptionalIntArg(fname,9,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(fname,9,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }


  (*func)(stk(l1),stk(l2),stk(l3),&m3,&n3,&flagx,&nz,znz,Style,Strf,Legend,Rect,Nax,4L,bsiz);
  LhsVar(1)=0;
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
#if _MSC_VER

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
#if _MSC_VER

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
	if (strcmp(fname,"xrect")==0)
        {	
	  int foreground = sciGetForeground(sciGetSelectedSubWin(sciGetCurrentFigure ()));
	  Objrect (stk(l1),stk(l2),stk(l3),stk(l4),
                   &foreground,NULL,FALSE,TRUE,0,&hdl,FALSE) ;
	}
	else
        {
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
  if (Rhs <= 0) 
    zoom();
  else {
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

  if ( Rhs == 0 )
    unzoom();
  else {
    int m,n,l,i;
    GetRhsVar(1,"h",&m,&n,&l); 
    for (i=0;i<m*n;i++) unzoom_one_axes((sciPointObj*)sciGetPointerFromHandle((long) *hstk(l+i))); /** Correction Bug 1476 + Warning Windows **/
  }
  LhsVar(1)=0; 
  return 0;
} 


/*-----------------------------------------------------------------------------------*/
int sci_delete(char *fname,unsigned long fname_len)
{
  integer m1,n1,l1,m2,n2,l2,num,cur,na,verb=0, lw;
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
  SciWin();
  C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  
  if ((Rhs == 2) && (strcmp(cstk(l2),"callback") == 0))
  {
    sciDelCallback((sciPointObj *)pobj);
  }
  else {

    sciEntityType objType    = sciGetEntityType( pobj ) ;
    BOOL          isSelected = sciGetIsSelected( pobj ) ;

    if (sciGetParentFigure(pobj) != NULL && sciGetEntityType(pobj) != SCI_FIGURE)
      { /* I added && sciGetEntityType(pobj) != SCI_FIGURE at last !!!!!!!! F.Leray 09.04.04 */
	pparentfigure = sciGetParentFigure(pobj);
	/*}*/
	sciSetCurrentObj((sciPointObj *)sciGetParent((sciPointObj *)pobj)); /* A LAISSER F.Leray 25.03.04*/

	

	sciDelGraphicObj((sciPointObj *)pobj);

        /* test here: we could have deleted the selected subwindow, we must choose an other */
	/* We must always have one selected subwindow (if at least one subwindow exists) */
	if ( objType  == SCI_SUBWIN && isSelected )
	  {
	    /* we have to select antoher subwindow if one exists at least */
	    /*sciSons *psonstmp = sciGetSons (pparentfigure);
	    if (psonstmp != (sciSons *) NULL)
	      {

		while ( (psonstmp->pnext != (sciSons *) NULL) && (sciGetEntityType (psonstmp->pointobj) != SCI_SUBWIN))
                {
		  psonstmp = psonstmp->pnext;
                }

		if( sciGetEntityType (psonstmp->pointobj) == SCI_SUBWIN ) 
                { 
		  sciSetSelectedSubWin (psonstmp->pointobj);
                }
		else
                {
		  sciSetSelectedSubWin((sciPointObj *) NULL);
                }
	      }
            */
            sciSelectFirstSubwin( pparentfigure ) ;
	  }
        
        /* redraw the window */
	sciDrawObj((sciPointObj *)pparentfigure);
      }
    else if(sciGetEntityType(pobj) == SCI_FIGURE) /* F.Leray 13.04.04: We delete the special object Figure !!*/
      {
	C2F(deletewin)( &num);
      }

  }

  if(cur != num)
    C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
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
