/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2000 Enpc/Jean-Philippe Chancelier
 *    jpc@cereve.enpc.fr 
 --------------------------------------------------------------------------*/
/*-------------------------------------------------------------------
 * Graphic subroutines interface 
 *-------------------------------------------------------------------*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../graphics/bcg.h"
#include "../stack-c.h"
#include "../graphics/Math.h"
#include "../graphics/Graphics.h"
#include "../graphics/PloEch.h"
#include "matdes.h"


/* The following NUMSETFONC and KeyTab_ definition should be coherent
with those defined in the drivers They are used in scixset to check
for invalid keys ("old_style" has been added). A better way should be to make drivers return an
error indicator in order to skip recording*/
extern void  C2F(msgs)(int *i, int *v);
#define NUMSETFONC 38
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

int cf_type=1; /* used by gcf to determine if current figure is a graphic (1) or a tksci (0) one */
static char *pmodes[] =
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

#ifndef NULL
#define NULL 0
#endif 

#if WIN32
extern int C2F(dsort) _PARAMS((double *count, int *n, int *index));
#endif

extern void setposfig __PARAMS((integer *i,integer *j));
extern int C2F (deletewin) __PARAMS((integer *number));  
extern void C2F(gsorts)  __PARAMS((char **data,int *ind,int *iflag, int *m,int *n,
				  char *type,char *iord));
extern int C2F(gsort)  __PARAMS((int *xI,double *xD,int *ind,int *iflag, int *m,int *n,
				  char *type,char *iord));
extern void ShowScales  __PARAMS((void));
extern  void C2F(seteventhandler)  __PARAMS((int *win_num,char *name,int *ierr));
#ifdef WITH_TK
extern int LAB_gcf();
#endif
extern sciPointObj *pfiguremdl; /* DJ.A 08/01/04 */
extern sciPointObj *paxesmdl;/* DJ.A 08/01/04 */
static integer one = 1, zero = 0;
/* NG beg */
int versionflag = 1; /* old mode */
sciClipTab ptabclip[15]; /* pourquoi n'est pas une prop de la figure */
static char error_message[70];
/* NG end */



/*-----------------------------------------------------------
 * get_style
 *-----------------------------------------------------------*/

static int * Style;

#define GetStyle(pos,n1,opts) if ( get_style(fname,pos,n1,opts) == 0) return 0;

static int get_style(fname,pos,n1,opts) 
     char *fname;
     int pos;
     int n1;
     rhs_opts opts[];
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
/*-----------------------------------------------------------
 * get_rect
 *-----------------------------------------------------------*/

#define GetRect(pos,opts) if ( get_rect(fname,pos,opts) == 0) return 0;

/* static double def_rect[4]  = {0.,0.,10.0,10.0};  */  /* F.Leray 29.04.04 */
static double def_rect[4]  = {0.,0.,0.0,0.0}; 
static double *Rect ; 

static int get_rect(fname,pos,opts) 
     char *fname;
     int pos;
     rhs_opts opts[];
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) 
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "d", &m, &n, &l);
	if (m * n != 4) { 
	  Scierror(999,"%s: rect has wrong size (%d), 4 expected \r\n",fname,m*n); 
	  return 0;
	}
	Rect = stk(l); 
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

#define GetStrf(pos,opts) if ( get_strf(fname,pos,opts) == 0) return 0;

#define DEFSTRF "081" 
#define DEFSTRFN "099" 

static char def_strf[]  = DEFSTRF;
static char *Strf ; 

static int get_strf(fname,pos,opts) 
     char *fname;
     int pos;
     rhs_opts opts[];
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

#define GetLegend(pos,opts) if ( get_legend(fname,pos,opts) == 0) return 0;

static char def_legend[]  = "X@Y@Z";
static char *Legend ; 

static int get_legend(fname,pos,opts) 
     char *fname;
     int pos;
     rhs_opts opts[];
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

#define GetNax(pos,opts) if ( get_nax(pos,opts)==0 ) return 0;

static int def_nax[]={2,10,2,10};
static int *Nax;
static int get_nax(pos,opts) 
     int pos;
     rhs_opts opts[];
{
  int i,m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) {
    if (VarType(pos)) {
      GetRhsVar(pos, "i", &m, &n, &l);
      CheckLength(pos,m*n,4);
      for (i = 0 ; i < 4; ++i) *istk(l+i) = Max((integer) *istk(l+i),0); /*POLPOTH09042001*/
      Nax=istk(l);
    }
    else
      {
	Nax=def_nax;
      }
  }
  else if ((kopt=FindOpt("nax",opts))) {
    GetRhsVar(kopt, "i", &m, &n, &l);
    CheckLength(kopt,m*n,4);
    for (i = 0 ; i < 4; ++i) *istk(l+i) = Max((integer) *istk(l+i),0); /*POLPOTH09042001*/
    Nax=istk(l);
  }
  else 
    {
      Nax=def_nax;
    } 
  return 1;
}

/* added by bruno 1/02/2001 from the model of GetRect */
#define GetZminmax(pos,opts) if ( get_zminmax(fname,pos,opts) == 0) return 0;

static double def_zminmax[2]  = {0.,0.};
static double *Zminmax ; 

static int get_zminmax(fname,pos,opts) 
     char *fname;
     int pos;
     rhs_opts opts[];
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

/* added by bruno 1/02/2001 on the model of get_nax */
#define GetColminmax(pos,opts) if ( get_colminmax(pos,opts)==0 ) return 0;

static int def_colminmax[]={1,1};
static int *Colminmax;
static int get_colminmax(pos,opts) 
     int pos;
     rhs_opts opts[];
{
  int m,n,l,first_opt=FirstOpt(),kopt;

  if (pos < first_opt) {
    if (VarType(pos)) {
      GetRhsVar(pos, "i", &m, &n, &l);
      CheckLength(pos,m*n,2);
      Colminmax=istk(l);
    }
    else
      {
	/** global value can be modified  **/
	def_colminmax[0] = def_colminmax[1] = 1;
	Colminmax=def_colminmax;
      }
  }
  else if ((kopt=FindOpt("colminmax",opts))) {
    GetRhsVar(kopt, "i", &m, &n, &l);
    CheckLength(kopt,m*n,2);
    Colminmax=istk(l);
  }
  else 
    {
      /** global value can be modified  **/
      def_colminmax[0] = def_colminmax[1] = 1;
      Colminmax=def_colminmax;
    } 
  return 1;
}

#define DEFLOGFLAGS "gnn" 
static char def_logflags[]  = DEFLOGFLAGS;
static char *Logflags; 
static char logflags[3];

#define GetLogflags(pos,opts) if ( get_logflags(fname,pos,opts) == 0) return 0;
static int get_logflags(fname,pos,opts) 
     char *fname;
     int pos;
     rhs_opts opts[];
{
  int m,n,l,first_opt=FirstOpt(),kopt;


  if (pos < first_opt) /* regular argument  */
    { 
      if (VarType(pos)) {
	GetRhsVar(pos, "c", &m, &n, &l);
	if ((m * n != 2)&&(m * n != 3)) {
	  Scierror(999,"%s: logflag has wrong size (%d), 2 expected \r\n",fname);
	  return 0;
	}
	if (m * n == 2) {
	  logflags[0]='g';logflags[1]=*cstk(l);logflags[2]=*cstk(l+1);
	  Logflags=logflags;}
	else
	  Logflags=cstk(l);
      }
      else /* zero type argument --> default value */
	{
	  Logflags=def_logflags;
	}
    }
  else if ((kopt=FindOpt("logflag",opts))) { /* named argument: style=value */
    GetRhsVar(kopt,"c", &m, &n, &l);
    if ((m * n != 2)&&(m * n != 3)) {
      Scierror(999,"%s: logflag has wrong size (%d), 2 expected \r\n",fname);
      return 0;
    }
    if (m * n == 2) {
      logflags[0]='g';logflags[1]=*cstk(l);logflags[2]=*cstk(l+1);
      Logflags=logflags;}
    else
      Logflags=cstk(l);
  }
  else /* unspecified argument --> default value */
    {
      Logflags=def_logflags;
    }
  return 1;
}



#define GetOptionalDoubleArg(pos,name,value,sz,opts) if ( get_optional_double_arg(fname,pos,name,value,sz,opts) == 0) return 0;
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
#define GetOptionalIntArg(pos,name,value,sz,opts) if ( get_optional_int_arg(fname,pos,name,value,sz,opts) == 0) return 0;
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


/*-------------------------------------------------------------------
 * champ 
 * champ(x,y,fx,fy,[arfact=1.0,rect=[xMin,yMin,xMax,yMax],flag])
 * champ1(x,y,fx,fy,[arfact=1.0,rect=[xMin,yMin,xMax,yMax],flag])
 *-------------------------------------------------------------------*/

int scichamp_G(fname, func, fname_len)
     char *fname;
     int (*func) ();
     unsigned long fname_len;
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

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != &(def_rect[0])) {Strf[1]='5';} 
  }

  C2F(sciwin)();
  C2F(scigerase)();
  (*func)(stk(l1 ),stk(l2 ),stk(l3 ),stk(l4 ),&m3,&n3,Strf,Rect, arfact, 4L);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *  contour(x,y,z,nz,[theta,alpha,leg,flag,ebox,zlev])
 *-----------------------------------------------------------*/

int scicontour(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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
  C2F(sciwin)();
  C2F(scigerase)();
  C2F(contour)(stk(l1), stk(l2), stk(l3), &m3, &n3, &flagx, &nz, stk(l4), theta, alpha,
	       Legend, iflag, ebox, zlev, bsiz);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * contour2d(x,y,z,nz,[style,strf,leg,rect,nax])
 *-----------------------------------------------------------*/

int scicontour2d_G(fname, func, fname_len)
     char *fname;
     int (*func) ();
     unsigned long fname_len;
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
  
 
  GetStyle(5,nz,opts);
  GetStrf(6,opts);    
  GetLegend(7,opts);
  GetRect(8,opts);
  GetNax(9,opts);
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

  C2F(sciwin)();
  C2F(scigerase)();
  (*func)(stk(l1),stk(l2),stk(l3),&m3,&n3,&flagx,&nz,znz,Style,Strf,Legend,Rect,Nax,4L,bsiz);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *  param3d(x,y,z,[theta,alpha,leg,flag,ebox])
 *-----------------------------------------------------------*/

int sciparam3d(fname, fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer izcol, *zcol=NULL, isfac;
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

  C2F(sciwin)();
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
    Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&ix1,&one,theta,alpha,Legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n);/*Adding F.Leray 12.03.04 */
  else
        Xplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&ix1,&one,theta,alpha,Legend,iflag,ebox);
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 *  param3d1(x,y,z,[theta,alpha,leg,flag,ebox])
 *  param3d1(x,y,list(z,colors),[theta,alpha,leg,flag,ebox])
 *-----------------------------------------------------------*/

int sciparam3d1(fname, fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer izcol, *zcol=NULL, isfac;
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
      GetListRhsVar(3,2,"i",&m3n,&n3n,&l3n); /* colors */
      zcol  = istk(l3n);
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
  C2F(sciwin)();
  C2F(scigerase)();
  /* NG beg */
  isfac=-1;
  if (version_flag() == 0)
    Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m1,&n1,theta,alpha,Legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n); /*Adding F.Leray 12.03.04*/
  else
        Xplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox);
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 * used in contourf, to extract contour points 
 *-----------------------------------------------------------*/

int scic2dex(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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

/*-----------------------------------------------------------
 *   [x,y]=geom3d(x1,y1,z1)
 *-----------------------------------------------------------*/

int scigeom3d(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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
  C2F(sciwin)();
  ix1 = m1 * n1;
  C2F(geom3d)(stk(l1), stk(l2), stk(l3), &ix1);
  LhsVar(1)=1;
  LhsVar(2)=2;
  return 0;
}

/*-----------------------------------------------------------
 * 
 *-----------------------------------------------------------*/

int sciplot3d_G(fname, func, func1, func2, func3,fname_len)
     char *fname;
     int (*func) __PARAMS((double *,double *,double *,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)); 
     int (*func1)  __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)); 
     int (*func2) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)); 
     int (*func3) __PARAMS((double *,double *,double *,integer *cvect,integer *p,integer *q,double *,double *,char *,integer *,double *,integer)); 
     unsigned long fname_len;
{
  static double  ebox_def [6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]={2,2,4};
  integer *iflag = iflag_def;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n = 0, n3n = 0, l3n, m3l, n3l, l3l; /*F.Leray 19.03.04 m3n and n3n set to 0.*/

  integer izcol, *zcol=NULL, isfac;

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
	GetListRhsVar(3,2,"i",&m3n,&n3n,&l3n);
	zcol  = istk(l3n);
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
  C2F(sciwin)();
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
  C2F(sciwin)();
  C2F(scigerase)();
 
  /******************** 24/015/2002 ********************/
  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
    isfac=1;
  else 
    isfac=0;

  if (version_flag() == 0)
    Objplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox,&m1,&n1,&m2,&n2,&m3,&n3,&m3n,&n3n);/*Adding F.Leray 12.03.04 and 19.03.04*/
  else
        Xplot3d (fname,&isfac,&izcol,stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox);
  /* NG end */
  LhsVar(1)=0;
  return 0;
  
}

/*-----------------------------------------------------------
 *     plot2d(x,y,[style,strf,leg,rect,nax]) 
 *-----------------------------------------------------------*/


int sciplot2d(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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
  
  GetStyle(3+iskip,n1,opts);
  GetStrf(4+iskip,opts);
  GetLegend(5+iskip,opts);
  GetRect(6+iskip,opts);
  GetNax(7+iskip,opts);
  if (iskip==0) GetLogflags(8,opts);

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
  C2F(sciwin)();
  C2F(scigerase)();

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

    Objplot2d (0,Logflags,stk(l1), stk(l2), &n1, &m1, Style, Strf,Legend, Rect,Nax);
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

/*-----------------------------------------------------------
 *   plot2dxx(str,x,y,[style,strf,leg,rect,nax])
 *-----------------------------------------------------------*/

int sciplot2d1_G(fname, ptype, func, fname_len)
     char *fname;
     int ptype; /* NG */
     int (*func) __PARAMS((char *,double *,double *,integer *,integer *,
                          integer *,char *,char *,double *,integer *,
                          integer,integer,integer));
     unsigned long fname_len;
{
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;
  integer iskip,test;
  integer m1,n1,l1, m2, n2, l2, lt, i, j ;
  static char str[]="x=(0:0.1:2*%pi)';plot2d1(x,[sin(x),sin(2*x),sin(3*x)],style=[1,2,3],rect=[0,-2,2*%pi,2]);";

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
    sci_demo(fname,str,&one);
    return 0;
  }
  CheckRhs(2,9);

  
  iskip=0;

  if ( get_optionals(fname,opts) == 0) return 0;

  if (GetType(1)==10) {
    /* logflags */
    GetLogflags(1,opts);
    iskip=1;
  }

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
  
  GetStyle(3+iskip,n1,opts);
  GetStrf(4+iskip,opts);
  GetLegend(5+iskip,opts);
  GetRect(6+iskip,opts);
  GetNax(7+iskip,opts);
  if (iskip==0) GetLogflags(8,opts);

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

  C2F(sciwin)();
  C2F(scigerase)();
  /* NG beg */
  if (version_flag() == 0)
    Objplot2d (ptype,Logflags,stk(l1), stk(l2), &n1, &m1, Style, Strf,Legend,Rect, Nax);
  else /* NG end */
    (*func)(Logflags,stk(l1),stk(l2),&n1,&m1,Style,Strf,Legend,Rect,Nax,
	    4L,strlen(Strf),strlen(Legend));
  LhsVar(1)=0;
  return 0;
} 



/*-----------------------------------------------------------
 *  grayplot(x,y,z,[strf,rect,nax])
 *-----------------------------------------------------------*/

int scigrayplot(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1]='7';
    if (Nax != def_nax)
      strfl[1]='1';
    GetOptionalIntArg(7,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(7,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }

  C2F(sciwin)();
  C2F(scigerase)();

  /* NG beg */
  if (version_flag() == 0)
    Objgrayplot (stk(l1), stk(l2), stk(l3), &m3, &n3, Strf, Rect, Nax);
  else /* NG end */
     Xgrayplot (stk(l1), stk(l2), stk(l3), &m3, &n3, Strf, Rect, Nax);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * scimatplot
 *-----------------------------------------------------------*/

int scimatplot(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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

  if (Strf == def_strf) {
    char strfl[4];
    if (version_flag() == 0)
      strcpy(strfl,DEFSTRFN);
    else
      strcpy(strfl,DEFSTRF);
    Strf = strfl;
    if (Rect != def_rect)
      strfl[1]='7';
    if (Nax != def_nax)
      strfl[1]='1';
    GetOptionalIntArg(5,"frameflag",&frame,1,opts);
    if(frame != &frame_def) 
      strfl[1] = (char)(*frame+48);
    GetOptionalIntArg(5,"axesflag",&axes,1,opts);
    if(axes != &axes_def) 
      strfl[2] = (char)(*axes+48);
  }

  C2F(sciwin)();
  C2F(scigerase)();
  /* NG beg */
  if (version_flag() == 0)
    Objmatplot (stk(l1), &m1, &n1, Strf, Rect, Nax);
  else 
    Xmatplot (stk(l1), &m1, &n1, Strf, Rect, Nax);
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 * Matplot1  
 *-----------------------------------------------------------*/

int scigray2plot(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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
  C2F(sciwin)();
  
  /* NG beg */
  if (version_flag() == 0)
    Objmatplot1 (stk(l1), &m1, &n1,stk(l2));
  else
    Xmatplot1 (stk(l1), &m1, &n1,stk(l2)); /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 * driver(driver_name) or  current_driver=driver()
 *-----------------------------------------------------------*/

int scidriver(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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

/*-----------------------------------------------------------
 * 
 *-----------------------------------------------------------*/

int scixarc(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,l2,l3,l4,l5,l6;
  long hdl;/* NG */

  C2F(sciwin)();
  CheckRhs(6,6);
  GetRhsVar(1,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
  GetRhsVar(2,"d",&m1,&n1,&l2);CheckScalar(2,m1,n1);
  GetRhsVar(3,"d",&m1,&n1,&l3);CheckScalar(3,m1,n1);
  GetRhsVar(4,"d",&m1,&n1,&l4);CheckScalar(4,m1,n1);
  GetRhsVar(5,"i",&m1,&n1,&l5);CheckScalar(5,m1,n1);
  GetRhsVar(6,"i",&m1,&n1,&l6);CheckScalar(6,m1,n1);
  /* NG beg */
  if (version_flag() == 0)
    if (strcmp(fname,"xarc")==0) 
      Objarc (istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),-1,0,&hdl);
    else
      Objarc (istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),-1,1,&hdl);
  else
    Xarc(fname,fname_len,istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4));
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
  scixarcs
 *-----------------------------------------------------------*/
int scixarcs(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2;

  /* NG beg */
  long  hdl;
  int i,a1,a2;
  long *hdltab;
  /* NG end */
  C2F(sciwin)();
  CheckRhs(1,2);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  if ( strncmp(fname,"xarcs",fname_len) == 0) {
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
      for (i = 0; i < n2; ++i)  *istk(l2 + i) = 0;
    }  
  /* NG beg */
  if (version_flag() == 0){ 
    if ((hdltab = malloc (n1 * sizeof (long))) == NULL) {
      Scierror(999,"%s: No more memory available\r\n",fname);
      return 0; 
    }
    for (i = 0; i < n1; ++i)
      { 
	a1=(int)(*stk(l1+(6*i)+4));
	a2=(int)(*stk(l1+(6*i)+5));
	Objarc (&a1,&a2,stk(l1+(6*i)),stk(l1+(6*i)+1),
		stk(l1+(6*i)+2),stk(l1+(6*i)+3),*istk(l2+i),0,&hdl); 
	hdltab[i]=hdl; /** handle of arc i **/
      }
    /** construct agregation and make it current object **/
    sciSetCurrentObj (ConstructAgregation (hdltab, n1));
    FREE(hdltab);
  }   
  else
    Xarcs(fname,fname_len,istk(l2), n1,stk(l1));
  /* NG end */
  LhsVar(1)=0;
  return 0;
} 
/*-----------------------------------------------------------
 *  xfarcs(arcs,[style])
 *-----------------------------------------------------------*/

int scixfarcs(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  int m1,n1,l1,m2,n2,l2;
  /* NG beg */
  long  hdl;
  int i,a1,a2;
  long *hdltab; /* NG end */

  C2F(sciwin)();
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
  /* NG beg */
  if (version_flag() == 0) {
    if ((hdltab = malloc (n1 * sizeof (long))) == NULL){
      Scierror(999,"%s: No more memory available\r\n",fname);
      return 0; 
    }
    for (i = 0; i < n1; ++i)
      { 
	a1 = (int)(*stk(l1+(6*i)+4));
	a2 = (int)(*stk(l1+(6*i)+5));
	Objarc (&a1,&a2,stk(l1+(6*i)),stk(l1+(6*i)+1),
		stk(l1+(6*i)+2),stk(l1+(6*i)+3),*istk(l2+i),1,&hdl); 
	hdltab[i]=hdl; /** handle of arc i **/
      }
    /** construct agregation and make it current object **/
    sciSetCurrentObj (ConstructAgregation (hdltab, n1));  
    FREE(hdltab);
  }   
  else
    Xfarcs(fname,fname_len,istk(l2), n1,stk(l1));
  /* NG end */
  LhsVar(1)=0;
  return 0;

}
 
/*-----------------------------------------------------------
 *   xarrows(nx,ny,[arsize,style])
 *-----------------------------------------------------------*/

int sciarrows(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer dstyle = -1,m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3;
  integer *style,flag;
  integer m4,n4,l4,mn2;
  double arsize=-1.0;

  C2F(sciwin)();
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
/*-----------------------------------------------------------
 *   xsegs(xv,yv,[style])
 *-----------------------------------------------------------*/

int scixsegs(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer dstyle = -1, *style,flag;
  integer mn2;
  integer m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3; 
  double arsize=0;

  C2F(sciwin)();

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
  if (Rhs == 3 && m3 * n3 != 1) {
    style=istk(l3); flag= one;
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




/*-----------------------------------------------------------
 * old version : kept for backward compatibility 
 *-----------------------------------------------------------*/

int scixaxis(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer v;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  double dv;

  CheckRhs(2,4);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);
  GetRhsVar(2,"i",&m2,&n2,&l2); CheckLength(1,m2*n2,2);
  GetRhsVar(3,"d",&m3,&n3,&l3); CheckLength(1,m3*n3,3);
  GetRhsVar(4,"d",&m4,&n4,&l4); CheckLength(1,m4*n4,2);

  C2F(sciwin)();
  C2F(dr1)("xaxis","v",&v,istk(l2),&v,&v,&v,&v,stk(l1),stk(l3),stk(l4),&dv,6L,2L);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *   [x1,y1,rect]=xchange(x,y,dir)
 *-----------------------------------------------------------*/

int scixchange(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3 ,l3,l4,quatre=4,l5,i;
  CheckRhs(3,3);
  CheckLhs(1,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  GetRhsVar(3,"c",&m3,&n3,&l3);
  CheckSameDims(1,2,m1,n1,m2,n2);

  C2F(sciwin)();
  if ( strncmp(cstk(l3),"i2f",3) == 0) 
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

/*-----------------------------------------------------------
 *     convertion d'entier vers double 
 *     d et s peuvent en fait pointer sur le meme tableau 
 *     car la recopie est fait de n,1,-1 
 *      implicit undefined (a-z) 
 *-----------------------------------------------------------*/

int C2F(entier2d)(n,dx,s)
     integer *n;
     double *dx;
     integer *s;
{
  integer ix;
  for (ix = *n -1 ; ix >= 0; --ix) dx[ix] = (double) s[ix];
  return 0;
} 

/*-----------------------------------------------------------
 *     convertion de float vers double 
 *     d et s peuvent en fait pointer sur le meme tableau 
 *     car la recopie est fait de n,1,-1 
 * Parameter adjustments 
 *-----------------------------------------------------------*/

int C2F(simple2d)(n,dx,s)
     integer *n;
     double *dx;
     float *s;
{
  integer ix;
  for (ix = *n-1 ; ix >= 0; --ix)  dx[ix] = (double) s[ix];
  return 0;
} 

/*-----------------------------------------------------------
 *   xclea(x,y,w,h) etendu a xclea([x,y,w,h])
 *-----------------------------------------------------------*/

int scixclea(fname,fname_len)
     char *fname; 
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  integer v;

  C2F(sciwin)();
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
/*-----------------------------------------------------------
 *   xclear([window-ids])
 *-----------------------------------------------------------*/

int scixclear(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer verb=0,wid,cur,win,na;
  integer ix,m1,n1,l1/*,v=0*/;
  double dv;


  CheckRhs(0,2); /* NG  */ /*CheckRhs(-1,1) */ 
  CheckLhs(0,1);
  C2F(sciwin)();
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

/*-----------------------------------------------------------
 *   [c_i,c_x,c_y,c_w,c_m]=xclick([flag])
 *-----------------------------------------------------------*/

int scixclick(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  int un=1,trois=3,rep,iw;
  integer istr,ix,iflag,v;
  double x,y,dv;

  CheckRhs(-1,1) ;
  CheckLhs(1,5) ;

  iflag = ( Rhs >= 1) ? 1 :0;

  C2F(sciwin)();

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

/*-----------------------------------------------------------
 * 
 *-----------------------------------------------------------*/

int scixend(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer v;
  double dv;
  C2F(sciwin)();
  CheckRhs(-1,0)
  C2F(dr1)("xend","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,2L);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *   xgrid([style])
 *-----------------------------------------------------------*/

int scixgrid(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer style = 1,m1,n1,l1;
  CheckRhs(-1,1);
  if (Rhs == 1) {
    GetRhsVar(1,"d",&m1,&n1,&l1);
    CheckScalar(1,m1,n1);
    style = (integer) *stk(l1);
  }
  C2F(sciwin)();
  C2F(xgrid)(&style); /* test on version_flag() inside Plo2.c F.Leray 07.05.04 */
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 *   xfpoly(xv,yv,[close])
 *-----------------------------------------------------------*/

int scixfpoly(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer close=0,m1,n1,l1,m2,n2 ,l2,m3,n3,l3,mn1 ;

  long hdl; /* NG */

  C2F(sciwin)();
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
  if (version_flag() == 0)
    Objfpoly (stk(l1),stk(l2),mn1,close,&hdl);
  else
    Xfpoly(mn1,close,stk(l1),stk(l2));
  /* NG end */
  LhsVar(1)=0;
  return 0;
    
}

/*-----------------------------------------------------------
 *  xfpolys(xpols,ypols,[fill])
 *-----------------------------------------------------------*/

int scixfpolys(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{

  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,v1=0,v2=0; /* v2 = 0 F.leray 24.02.04 unused flag*/
  /* v1 is the flag used for flat (v1==1)/interpolated (v1==2) shading */

  /* NG beg */
  int i,color;
  long hdl;
  long *hdltab;
  /* NG end */


  C2F(sciwin)();

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
    }
  /* NG beg */
  if (version_flag() == 0) {
    if ((hdltab = malloc (n1 * sizeof (long))) == NULL) {
      Scierror(999,"%s: No more memory available\r\n",fname);
      return 0; 
    }
    for (i = 0; i < n1; ++i) {
      if (*istk(l3+i) == 0) {
	/** fil(i) = 0 poly i is drawn using the current line style (or color).**/
	color= ((i==0) ? 1: sciGetForeground(sciGetPointerFromHandle((long) hdltab[i-1])));
	Objpoly (stk(l1+(i*m1)),stk(l2+(i*m1)),m1,1,color,&hdl);
      }
      else   
	/** poly i is drawn using the line style (or color) **/  
	Objfpoly (stk(l1+(i*m1)),stk(l2+(i*m1)),m1,*istk(l3+i),&hdl);
      hdltab[i]=hdl;
    }
    /** construct agregation and make it current object**/
    sciSetCurrentObj (ConstructAgregation (hdltab, n1));  
    FREE(hdltab);
  }
  Xfpolys(istk(l3),v1,v2,n2,m2,stk(l1),stk(l2));
  /* NG end */

  /* end of Code modified by polpoth 7/7/2000 */

  LhsVar(1)=0;  
  return 0;  
} 

/*-----------------------------------------------------------
 * 
 *-----------------------------------------------------------*/

int scixget(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer flagx=0,x1[10],x2, m1,n1,l1,m2,n2,l2,l3,v,i ;
  double dv;

  C2F(sciwin)();
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
  if (Rhs == 2) {GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  flagx = (integer) *stk(l2); }

  if ( strncmp(cstk(l1),"fpf",3) == 0 || strncmp(cstk(l1),"auto clear",10) == 0) 
    {
      int bufl;
      /*     special case for global variables set */
      C2F(xgetg)( cstk(l1),C2F(cha1).buf,&bufl,m1,bsiz);
      CreateVar(Rhs+1,"c",&bufl,&one,&l3);
      strncpy(cstk(l3),C2F(cha1).buf,bufl);
      LhsVar(1)=Rhs+1;
      return 0;
    }
  else if ( strncmp(cstk(l1),"colormap",8) == 0) 
    {
      /*     special case for colormap : must allocate space */
      int m3,n3=3;
      CheckColormap(&m3);
      if ( m3 == 0) n3=0;
      CreateVar(Rhs+1,"d",&m3,&n3,&l3);
      C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,stk(l3),&dv,&dv,&dv,5L,bsiz);
      LhsVar(1)=Rhs+1;
    }
  else if ( strncmp(cstk(l1),"mark size",9) == 0) {
    int i;
    C2F(dr1)("xget","mark",&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
    x1[0]=x1[1];
    x2=1;
    CreateVar(Rhs+1,"d",&one,&x2,&l3);
    for (i = 0 ; i < x2 ; ++i) *stk(l3 + i ) = (double) x1[i];      
    LhsVar(1)=Rhs+1;
  }
  else if ( strncmp(cstk(l1),"font size",9) == 0) {
    int i;
    C2F(dr1)("xget","font",&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
    x1[0]=x1[1];
    x2=1;
    CreateVar(Rhs+1,"d",&one,&x2,&l3);
    for (i = 0 ; i < x2 ; ++i) *stk(l3 + i ) = (double) x1[i];      
    LhsVar(1)=Rhs+1;
  }
  else if ( strncmp(cstk(l1),"line style",10) == 0) {
    C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);
    CreateVar(Rhs+1,"d",&one,&x2,&l3);
    *stk(l3 ) = (double) x1[0];      
    LhsVar(1)=Rhs+1;
  } 
  /* NG beg */
  else if ( strncmp(cstk(l1),"old_style",9) == 0) 
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

/*-----------------------------------------------------------
 *   xinit([driver-name])
 *-----------------------------------------------------------*/

int scixinit(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,v,v1=-1;
  double dv;
  CheckRhs(-1,1);
  if (Rhs <= 0 )
    {
      C2F(dr1)("xinit"," ",&v1,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,2L);
    } 
  else 
    {
      GetRhsVar(1,"c",&m1,&n1,&l1);
      C2F(dr1)("xinit",cstk(l1),&v1,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,m1);
    }
  LhsVar(1)=0; return 0;
}


/*-----------------------------------------------------------
 * xlfont(font-name,font-id)
 * fonts=xlfont()
 * Warning sz dimensions must be compatible with periX11.c FONTNUMBER 
 *-----------------------------------------------------------*/

int scixlfont(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,v,i,count,sz[10],num;
  double dv;

  C2F(sciwin)();
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

/*-----------------------------------------------------------
 * scixnumb(x,y,nums,[box,angles]) : NO MORE USED
 *-----------------------------------------------------------*/

int scixnumb(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5;
  integer flagx=0,mn3;
  C2F(sciwin)();

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

/*-----------------------------------------------------------
 *  xpause(microsecs)
 *-----------------------------------------------------------*/


int scixpause(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,v,sec=0;
  double dv;
  C2F(sciwin)();
  CheckRhs(-1,1);
  if (Rhs == 1) { GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); sec = (integer) *stk(l1);} 
  C2F(dr1)("xpause","v",&sec,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 *  xpoly(xv,yv,dtype,[close])
 *-----------------------------------------------------------*/

int scixpoly(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1 ,m2 ,n2 ,l2,m3,n3,l3,m4,n4,l4,close=0,mn2;

  long hdl;/* NG */
  int mark;/* NG */

  C2F(sciwin)();

  CheckRhs(2,4);
  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);
  mn2 = m2 * n2;
  if ( mn2 == 0 ) {  LhsVar(1)=0; return 0; } 
  if (Rhs >= 3) {
    GetRhsVar(3,"c",&m3,&n3,&l3);
    if ( strncmp(cstk(l3),"lines",5) == 0) {
      strcpy(C2F(cha1).buf,"xlines"); 
      mark=1; /* NG */
    } else if (strncmp(cstk(l3),"marks",5) == 0) {
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
  if (version_flag() == 0)
    Objpoly (stk(l1),stk(l2),mn2,close,mark,&hdl);
  else
    Xpoly(C2F(cha1).buf,bsiz,mn2,close,stk(l1),stk(l2));
  /* NG end */
  LhsVar(1)=0;
  return 0;
  /*************************************************/
}

/*-----------------------------------------------------------
 *   xpolys(xpols,ypols,[draw])
 *-----------------------------------------------------------*/

int scixpolys(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3;
  /* NG beg */
  int i;
  long hdl;
  long *hdltab;
  /* NG end */

  C2F(sciwin)();
  CheckRhs(2,3);

  GetRhsVar(1,"d",&m1,&n1,&l1);
  GetRhsVar(2,"d",&m2,&n2,&l2);
  CheckSameDims(1,2,m1,n1,m2,n2);

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
  /* NG beg */
  if (version_flag() == 0) {
    if ((hdltab = malloc (n1 * sizeof (long))) == NULL) {
      Scierror(999,"%s: No more memory available\r\n",fname);
            return 0; 
    }
    for (i = 0; i < n1; ++i) {
      Objpoly (stk(l1+(i*m1)),stk(l2+(i*m2)),m1,0,*istk(l3+i),&hdl);
      hdltab[i]=hdl;
      }
    /** construct agregation and make it current object**/
    sciSetCurrentObj (ConstructAgregation (hdltab, n1));  
    FREE(hdltab);
    }
   else
     Xpolys(istk(l3),n2,m2,stk(l1),stk(l2));
  /* NG end*/
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *-----------------------------------------------------------*/

int scixselect(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer v;
  double dv;
  CheckRhs(-1,0);
  C2F(dr1)("xselect","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,8L,2L);
  LhsVar(1)=0;
  return 0;
}

/*------------- ----------------------------------------------
 * xset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/

int scixset(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2, xm[5],xn[5],x[5], i, v, isdc;
  integer lr, mark[2], font[2], verb=0;
  double  xx[5],dv ;
  sciPointObj *subwin ; 

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
  if (Rhs == 1 && (strncmp(cstk(l1),"window",6L) == 0) )
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

  if (Rhs == 1 && strncmp(cstk(l1),"default",7) == 0) 
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
  if (strncmp(cstk(l1),"colormap",8) == 0 || strncmp(cstk(l1),"default",7) == 0) C2F(sedeco)(&isdc);
  if (strncmp(cstk(l1),"window",6L) != 0) C2F(sciwin)();
  isdc = 1;
  if (strncmp(cstk(l1),"colormap",8) == 0 || strncmp(cstk(l1),"default",7) == 0) C2F(sedeco)(&isdc);
  if (strncmp(cstk(l1),"wdim",8) == 0 || strncmp(cstk(l1),"wpdim",7) == 0) {
    /* Xwindows limits dimensions to 2^16 */
    if ( (x[0]>65535)||(x[1]>65535)) {
      x[0]=Min(x[0],65535);
      x[1]=Min(x[1],65535);
      i = 106;v=0;
      C2F(msgs)(&i,&v);
    }
  }

  if (strncmp(cstk(l1),"clipping",8) == 0) 
    C2F(dr1)("xset",cstk(l1),&v,&v,&v,&v,&v,&v,&xx[0],&xx[1],&xx[2],&xx[3],5L,bsiz);
  else if ( strncmp(cstk(l1),"colormap",8) == 0) {

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
  else if ( strncmp(cstk(l1),"mark size",9) == 0) {
    C2F(dr1)("xget","mark",&verb,mark,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
    mark[1]=(int)xx[0];
    C2F(dr1)("xset","mark",&(mark[0]),&(mark[1]),&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,5L);
  }
  else if ( strncmp(cstk(l1),"font size",9) == 0) {
    C2F(dr1)("xget","font",&verb,font,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,5L);
    font[1]=(int)xx[0];
    C2F(dr1)("xset","font",&(font[0]),&(font[1]),&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,5L);
  } 
 /* NG beg */
  else if ( strncmp(cstk(l1),"old_style",9) == 0) {
    if (*stk(lr) == 0) 
      versionflag = (int) *stk(lr);
    else if (*stk(lr) == 1)
      versionflag = (int) *stk(lr);
    else
      Scierror(999,"%s: Value must be 1 or 0",fname);
  }/* NG end */
  else 
    C2F(dr1)("xset",cstk(l1),&x[0],&x[1],&x[2],&x[3],&x[4],&v,&dv,&dv,&dv,&dv,5L,bsiz);
  /* NG beg */
  if ((versionflag == 0) && (strncmp(cstk(l1),"window",6L) == 0))
    sciSwitchWindow(&x[0]);

  if (version_flag() == 0)
    {
      subwin = sciGetSelectedSubWin(sciGetCurrentFigure());
      if (( strncmp(cstk(l1),"foreground",10) == 0) || (strcmp(cstk(l1),"color") == 0) ||( strncmp(cstk(l1),"pattern",7) == 0) ) {
        sciSetForeground(subwin, x[0]); 
	if (sciGetSons((sciPointObj *) subwin) == (sciSons *)NULL) {/* added SS 20.04.04 */
	  /* if subwin is empty, set also the ticscolor and fontforeground */
	  pSUBWIN_FEATURE (subwin)->axes.ticscolor= x[0];
	  sciSetFontForeground(subwin,x[0]); 
	}
       /*sciSetForeground(sciGetParent(subwin), x[0]);*/ /* TO DELETE F.Leray 02.04.04*/
      } 
      else if ( strncmp(cstk(l1),"background",10) == 0) {
        sciSetBackground(subwin, x[0]); 
        /*sciSetBackground(sciGetParent(subwin), x[0]); */ /* TO DELETE F.Leray 02.04.04*/
      }  
      else if ( strncmp(cstk(l1),"thickness",9) == 0) {
        sciSetLineWidth(subwin, x[0]); 
        sciSetLineWidth(sciGetParent(subwin), x[0]);   
      } 
      else if ( strncmp(cstk(l1),"line style",10) == 0) {
        sciSetLineStyle(subwin, x[0]); 
        sciSetLineStyle(sciGetParent(subwin), x[0]);   
      }  
      else if ( strncmp(cstk(l1),"mark",4) == 0) {
        sciSetIsMark(subwin,1); 
        sciSetIsMark(sciGetParent(subwin),1); 
	sciSetMarkStyle(subwin,x[0]); 
        sciSetMarkStyle(sciGetParent(subwin),x[0]);   
      } 
      else if ( strncmp(cstk(l1),"colormap",8) == 0) {
        sciSetColormap(sciGetParent(subwin), stk(lr),xm[0], xn[0]);
      }
      else if ( strncmp(cstk(l1),"font",4) == 0) {
        sciSetFontStyle(subwin, font[0]); 
        sciSetFontStyle(sciGetParent(subwin), font[0]);   
      } 
      else if ( strncmp(cstk(l1),"font size",9) == 0) {
        sciSetFontDeciWidth(subwin, font[1]); 
        sciSetFontDeciWidth(sciGetParent(subwin), font[1]);   
      }  
      else if ( strncmp(cstk(l1),"alufunction",11) == 0) {
        sciSetXorMode(subwin, x[0]); 
        sciSetXorMode(sciGetParent(subwin), x[0]);   
      }
      else if ( strncmp(cstk(l1),"auto clear",10) == 0) {
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
     else if ( strncmp(cstk(l1),"auto scale",10) == 0) {
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
     else if ( strncmp(cstk(l1),"wresize",6) == 0) {
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
    else if ( strncmp(cstk(l1),"wpos",4) == 0) {
        sciSetFigurePos (sciGetParent(subwin), x[0], x[1]);
	 
      }
    else if ( strncmp(cstk(l1),"wpdim",4) == 0) {
      pFIGURE_FEATURE(sciGetParent(subwin))->figuredimwidth=x[0];  
      pFIGURE_FEATURE(sciGetParent(subwin))->figuredimheight=x[1];
	 
      } 
   else if ( strncmp(cstk(l1),"wdim",4) == 0) {
     pFIGURE_FEATURE(sciGetParent(subwin))->windowdimwidth=x[0];  
     pFIGURE_FEATURE(sciGetParent(subwin))->windowdimheight=x[1];
	 
   } /*Ajout A.Djalel le 10/11/03 */
   else if ( strncmp(cstk(l1),"pixmap",6) == 0) {
     pFIGURE_FEATURE(sciGetParent(subwin))->pixmap=x[0];
     /*if(x[0] == 1) 
       sciSetVisibility (sciGetParent(subwin), FALSE);
     else{
       sciSetVisibility (sciGetParent(subwin), TRUE);
       pFIGURE_FEATURE(sciGetParent(subwin))->wshow=0;}*/
       }  
      else if ( strncmp(cstk(l1),"wshow",5) == 0) { /* a supprimer ce n'est pas une propriete mais une action */
     pFIGURE_FEATURE(sciGetParent(subwin))->wshow=1;
     sciSetVisibility (subwin, TRUE); 
     }
      sciRedrawFigure();   
    }
  /***/
  /* NG end */    
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 * xstring(x,y,str,[angle,box])
 *-----------------------------------------------------------*/

int scixstring(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  double rect[4],wc,x,y,yi,angle=0.0;
  integer i,j,iv =0,flagx=0;
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str;
  long hdlstr, hdlrect;/* NG */
  long *hdltab;/* NG */

  CheckRhs(3,5);
  
  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); angle = *stk(l4 ); }
  if (Rhs >= 5) { GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5); flagx = (integer) *stk(l5);  }

  C2F(sciwin)();
  wc = 0.;/* to keep the size of the largest line */

  /* NG beg */
  if (version_flag() == 0) {
    if ((hdltab = malloc ((m3+1) * sizeof (long))) == NULL){
      Scierror(999,"%s: No more memory available\r\n",fname);
      return 0; 
    }
    for (i = m3 -1 ; i >= 0; --i)  {
      int ib = 0;
      for (j = 0 ; j < n3 ; ++j) {
	strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
	ib += strlen(Str[i+ m3*j]);
	if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
      }
      Objstring (C2F(cha1).buf,bsiz,iv,x,y,&angle,rect,(double *)0,-1,&hdlstr);
      hdltab[m3-1-i]=hdlstr;   
      wc = Max(wc,rect[2]);
      if (i != 0 ) 
	y += rect[3] * 1.2; 
      else 
	y += rect[3];      
    } /* end for(i) */

    if ((flagx == 1) && (*stk(l4) == 0)) {
      double dx1= y - yi ;
      Objrect (&x,&yi,&wc,&dx1,0,0,1,&hdlrect);
    }
    /** construct agregation and make it current object **/ 
    if ((flagx == 1) && (*stk(l4) == 0)){
      hdltab[m3]=hdlrect;
      sciSetCurrentObj (ConstructAgregation (hdltab, m3+1));
    }
    else  { 
      if (m3 > 1)
	sciSetCurrentObj ( ConstructAgregation (hdltab, m3));
    }
    FREE(hdltab);
  } /* end if (version_flag() == 0) */
  else { /* NG end */
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
  } /* end  (version_flag() == 0) */
   
  /* we must free Str memory */ 
  FreeRhsSVar(Str);

  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 * xtitle(tit,x,y)
 *-----------------------------------------------------------*/

int scixtitle(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  int narg;
 /*  long hdl; */
/*   long *hdltab; */
 /*  sciSubWindow * ppsubwin = NULL; */
  sciPointObj * psubwin = NULL;

  if (Rhs <= 0) {
    sci_demo(fname,"x=(1:10)';plot2d(x,x);xtitle(['Titre';'Principal'],'x','y');",&one);
    return 0;
  }
  CheckRhs(1,3);
  C2F(sciwin)();
/*   if (version_flag() == 0){ */
/*     if (Rhs >= 1) */
/*       { */
/* 	if ((hdltab = malloc (Rhs * sizeof (long))) == NULL) */
/* 	  return 0; */
/*       } */
/*   } */

  if (version_flag() == 0) 
    psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
  /*   ppsubwin = pSUBWIN_FEATURE( sciGetSelectedSubWin (sciGetCurrentFigure ())); */

  for ( narg = 1 ; narg <= Rhs ; narg++) 
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
	  /*   Objtitle(C2F(cha1).buf,narg,&hdl); */
	  /* 	  hdltab[narg-1]=hdl; */

	  switch(narg){
	  case 1:
	    sciSetText(pSUBWIN_FEATURE(psubwin)->mon_title, C2F(cha1).buf , strlen(C2F(cha1).buf));
	    break;
	  case 2:
	    sciSetText(pSUBWIN_FEATURE(psubwin)->mon_x_label, C2F(cha1).buf , strlen(C2F(cha1).buf));
	    break;
	  case 3:
	    sciSetText(pSUBWIN_FEATURE(psubwin)->mon_y_label, C2F(cha1).buf , strlen(C2F(cha1).buf));
	    break;
	  }
	  sciSetCurrentObj( sciGetSelectedSubWin (sciGetCurrentFigure ()));
	  sciRedrawFigure();
	}
      else
	Xtitle (C2F(cha1).buf,narg);
    }
 /*  if (version_flag() == 0) { */
/*     if (Rhs > 1){ */
/*       sciSetCurrentObj (ConstructAgregation (hdltab, Rhs)); */
/*       FREE(hdltab); } */
/*   } */
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * xstringb 
 *-----------------------------------------------------------*/

int scixstringb(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5,m6,n6,l6;
  integer fill =0, i,j,v,ib;
  double x,y,w,hx;
  char **Str;
  double wh[2],rect[4],angle=0;
  long hdlstr;

  C2F(sciwin)();
  CheckRhs(5,6);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 
  
  GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);  w = *stk(l4);
  GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5);  hx = *stk(l5);
  
  if (Rhs == 6) {
    GetRhsVar(6,"c",&m6,&n6,&l6);
    if ( m6*n6 !=0 && strncmp(cstk(l6),"fill",4) == 0) 
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
    Objstring (C2F(cha1).buf,bsiz,0,x,y,&angle,rect,wh,fill,&hdlstr);
  } 
  else { /* NG end */
    C2F(dr1)("xstringb",C2F(cha1).buf,&fill,&v,&v,&v,&v,&v,&x,&y,&w,&hx,9L,bsiz);
  }
  FreeRhsSVar(Str); /* we must free Str2 memory */ 
  LhsVar(1)=0;
  return 0;

} 

/*-----------------------------------------------------------
 *  rect=xstringl(x,y,str)
 *-----------------------------------------------------------*/

int scixstringl(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  double rect[4],wc,dv,x,y,yi;
  integer i,j,v,un=1,quatre=4,l4,m1,n1,l1,m2,n2,l2,m3,n3;
  char **Str;

  CheckRhs(3,3);
  CheckLhs(0,1);
  
  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 
  C2F(sciwin)();
  /*     to keep the size of the largest line */
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
  CreateVar(4,"d",&un,&quatre,&l4);
  *stk(l4) = x; 
  *stk(l4+1) = y;
  *stk(l4+2) = wc ;
  *stk(l4+3) = y-yi;
  LhsVar(1)=4;
  return 0;
}

/*-----------------------------------------------------------
 * xtape: update manual XXX
 *-----------------------------------------------------------*/

int scixtape(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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

/*-----------------------------------------------------------
 * xinfo(string)
 *-----------------------------------------------------------*/

int scixinfo(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  double dv;
  integer m1,n1,l1,v;
  CheckRhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  C2F(dr)("xinfo",cstk(l1),&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,m1);
  LhsVar(1) =0;
  return 0;
}

/*------------------------------------------------------------
 * xsetech(wrect=[...],frect=[..],logflag="..", arect=[...]) 
 * or 
 * xsetech(wrect,[frect,logflag])
 * or 
 * xsetech()
 *------------------------------------------------------------*/

int scixsetech(fname, fname_len)
  char* fname;
  unsigned long fname_len;
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
  C2F(sciwin)();

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
  C2F(sciwin)();
  C2F(Nsetscale2d)(wrect,arect,frect,logflag,0L);
  LhsVar(1)=0;
  return 0;
  
}

/*-----------------------------------------------------------
 * [wrect,frect,logflag,arect]=xgetech()
 *-----------------------------------------------------------*/

int scixgetech(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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
  C2F(sciwin)();
  getscale2d(W,F,L,A);
  for ( i = 1 ; i <= Lhs ; i++) LhsVar(i) = i;
  return 0;
} 

/*-----------------------------------------------------------
 * set or create a graphic window 
 *-----------------------------------------------------------*/

int C2F(sciwin)()
{
  integer verb=0,win=0,v=1,na;
  double dv;
  C2F(dr)("xget","window",&verb,&win,&na,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  C2F(dr)("xset","window",&win,&v,PI0,PI0,PI0,PI0,&dv,&dv,&dv,&dv,5L,7L);
  if (versionflag == 0) sciSwitchWindow(&win);  /* NG */
  return 0;
} 

/* modified version by Bruno 1/2/2001
 *-----------------------------------------------------------
 * fec(x,y,triangles,func,[strf,leg,rect,nax,zminmax,colminmax]);
 *-----------------------------------------------------------*/

int scifec(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,  mn1;

  static rhs_opts opts[]= { {-1,"leg","?",0,0,0},
		            {-1,"nax","?",0,0,0},
			    {-1,"rect","?",0,0,0},
			    {-1,"strf","?",0,0,0},
			    {-1,"zminmax","?",0,0,0},
			    {-1,"colminmax","?",0,0,0},
			    {-1,NULL,NULL,0,0}};
  if (Rhs <= 0) {
    sci_demo (fname," exec(\"SCI/demos/graphics/fec/fec.ex1\");",&one);
    return 0;
  }

  CheckRhs(4,10);

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
    if (Nax != def_nax)
      strfl[1]='1';
  }
  C2F(sciwin)();
  C2F(scigerase)();
  mn1 = m1 * n1;
  
  /* NG beg */
  if (version_flag() == 0)
     Objfec (stk(l1),stk(l2),stk(l3),stk(l4),&mn1,&m3,Strf,Legend,Rect,Nax,Zminmax,Colminmax);
  else
     Xfec (stk(l1),stk(l2),stk(l3),stk(l4),&mn1,&m3,Strf,Legend,Rect,Nax,Zminmax,Colminmax);
  /* NG end */
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * rep = xgetmouse([flag],[sel])
 *-----------------------------------------------------------*/

int scixgetmouse(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer  m1=1,n1=3,l1,button,v;
  integer iflag;
  integer sel[2],m,n;
  double x,y,dv;

  CheckRhs(0,2);
  CheckLhs(1,1);
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

  C2F(sciwin)();
  C2F(dr1)("xgetmouse","xv",&button,&iflag,&v,&v,sel,&v,&x,&y,&dv,&dv,10L,3L);

  CreateVar(Rhs+1,"d",&m1,&n1,&l1);
  *stk(l1) = x;  *stk(l1+1) = y;  *stk(l1+2) = (double) button;
  LhsVar(1) = Rhs+1;
  return 0;
} 

/*-----------------------------------------------------------
 * xsave('fname' [, wid]) 
 *-----------------------------------------------------------*/

int scixsave(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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
  C2F(sciwin)();

  C2F(xsaveplots)(&wid,cstk(l1),m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * xload('fname' [, wid]) 
 *-----------------------------------------------------------*/

int scixload(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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
  C2F(sciwin)();
  C2F(xloadplots)(cstk(l1),m1);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * xdel([win-ids]) 
 *-----------------------------------------------------------*/

int scidelw(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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

/*-----------------------------------------------------------
 * impression
 *-----------------------------------------------------------*/

int scixg2psofig_G(fname,dr,fname_len,dr_len)
     char *fname,*dr;
     unsigned long fname_len;
     unsigned long dr_len;
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

/*-----------------------------------------------------------
 * [s,v]= gsort(x,[str1,str2]) 
 *       str1 = 'g','r','c','lc','lr',
 *       str2 = 'i' | 'd' 
 *-----------------------------------------------------------*/

int scixsort(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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
/*-----------------------------------------------------------
 *   x=winsid()
 *-----------------------------------------------------------*/

int sciwinsid(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer iflag =0,ids,num,un=1,l1;

  CheckRhs(-1,0) ;  /* NG beg */
 if (version_flag() == 0)
   {
    sciGetIdFigure (&ids,&num,&iflag);
    CreateVar(1,"i",&un,&num,&l1);
    iflag = 1; 
    sciGetIdFigure (istk(l1),&num,&iflag);
    LhsVar(1) = 1; 
   }/* NG end*/
 else
   {
    C2F(getwins)(&num,&ids ,&iflag);
    CreateVar(1,"i",&un,&num,&l1);
    iflag = 1; 
    C2F(getwins)(&num,istk(l1),&iflag);
    LhsVar(1)=1;
    } 
  
  return 0;
}

/*-----------------------------------------------------------
 * erase a graphic window if necessary 
 *-----------------------------------------------------------*/

int C2F(scigerase)()
{
  integer verb=0,lstr,v,na,win;
  double dv;
  char str[4];
  C2F(xgetg)("auto clear",str,&lstr,11L,4L);
  if (strncmp(str,"on",2) == 0) {
    C2F(dr1)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
    C2F(dr1)("xclear",C2F(cha1).buf,&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,bsiz);
    C2F(dr1)("xstart",C2F(cha1).buf,&win,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,bsiz);
  }
  return 0;
} 

/*-----------------------------------------------------------
 * [xi,xa,np1,np2,kMinr,kMaxr,ar]=xgraduate(xmi,xma)
 * rajouter ds le man XXXX 
 *-----------------------------------------------------------*/

int scixgraduate(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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
/*-----------------------------------------------------------
 * xname('name') : give a name to current graphic window 
 *-----------------------------------------------------------*/

int scixname(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  double dv;
  integer v,m1,n1,l1;
  CheckRhs(1,1);
  CheckLhs(1,1);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  C2F(sciwin)();
  C2F(dr1)("xname",cstk(l1),&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,6L,bsiz);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * set_posfig_dim(width,height) : give a name to current graphic window 
 *-----------------------------------------------------------*/

int scisetposfig(fname,fname_len)
     char *fname;
     unsigned long fname_len;
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


/*------------------------------------------------------------
 * dir = 'u','r','d','l'  [default -> 'l' ] 
 * fontsize =             [default -> -1   ] 
 * format_n = format to use for numbers (unused if strings) 
 * seg = flag 1 or 0 draw the base segment of the axis (default 1)
 * sub_int  = number of sub tics (default 2) 
 * textcolor =            [default -> -1 ]
 * ticscolor =            [default -> -1 ]
 * tics = 'v' 'r' 'i'     [default -> 'v' ] 
 *        gives tics type : vector | range | irange (v,r,i) 
 * val  = string matrix 
 * x = scalar | vecteur | range | irange 
 * y = scalar | vecteur | range | irange 
 * 
 * constraints : 
 * ------------
 *   dir = 'u' | 'd' ==> y= scalar | [] 
 *                       x= vecteur | range | irange 
 *   dir = 'r' | 'l' ==> x= scalar | [] 
 *                       y= vecteur | range | irange 
 *   tics = 'r'          ==> x or y is of size 3 (according to dir)
 *   tics = 'i'          ==> x or y is of size 4
 *   val  =              ==> must be of size compatible with x or y 
 *                       according to dir 
 *-------------------------------------------------------------*/

int nscixaxis(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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
  
  nopt = NumOpt();

  C2F(sciwin)();

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
    }
     
  /* NG beg */
   if (version_flag() == 0)
     Objdrawaxis(dir,tics,x,&nx,y,&ny,val,sub_int,format,fontsize,textcolor,ticscolor,'n',seg_flag);
   else
     Xdrawaxis (dir,tics,x,&nx,y,&ny,val,sub_int,format,fontsize,textcolor,ticscolor,'n',seg_flag);
   /* NG end */
 
   LhsVar(1)=0;
   return 0;
}

int check_xy(fname,dir,mn,xpos,xm,xn,xl,ypos,ym,yn,yl,ntics)
     char *fname;
     char dir;
     int mn,xpos,xm,xn,ypos,ym,yn;
     unsigned long int xl,yl;
     int *ntics;
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




/*-----------------------------------------------------------
 * metanet with Scilab graphics 
 *-----------------------------------------------------------*/
int intmeta(fname, fname_len)
     char *fname;
     unsigned long fname_len;
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


/*---------------------------------------------------
 * interface for calling the helpbrowser 
 * when scilab is compiled with gtk 
 * not the perfect place to insert this interface ...
 *---------------------------------------------------*/

extern void Sci_Help(char *,char *,char *);

static int int_gtkhelp(fname)
     char *fname;
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

/*-----------------------------------------------------------
 * utilities 
 *-----------------------------------------------------------*/

int scichamp (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scichamp_G(fname,C2F(champ),fname_len);
}

int scichamp1 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scichamp_G(fname,C2F(champ1),fname_len);
}

int sciplot3d (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot3d_G(fname,C2F(plot3d),C2F(fac3d),C2F(fac3d2),C2F(fac3d3),fname_len);
}

int sciplot3d1 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot3d_G(fname,C2F(plot3d1),C2F(fac3d1),C2F(fac3d2),C2F(fac3d3),fname_len);
}

int sciplot2d1_1 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d1",1,C2F(plot2d1),fname_len);/* NG */
}

int sciplot2d1_2 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d2",2,C2F(plot2d2),fname_len); /* NG */
}

int sciplot2d1_3 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d3",3,C2F(plot2d3),fname_len);/* NG */
}

int sciplot2d1_4 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d4",4,C2F(plot2d4),fname_len);/* NG */
}

int scicontour2d (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scicontour2d_G(fname, C2F(contour2),fname_len);
}

int scicontour2d1 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
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
int scixs2ps(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"Pos",fname_len,3);
}

/* backward compatibility */

int scixg2ps(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"Pos",fname_len,3);
}

int scixs2fig(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"Fig",fname_len,3);
}

int scixs2gif(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"GIF",fname_len,3);
}

int scixs2ppm(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"PPM",fname_len,3);
}
/*-----------------------------------------------------------
 *   rect(x,y,w,h) 
 *-----------------------------------------------------------*/

int scirect(fname,fname_len)
     char *fname; 
     unsigned long fname_len;
{
  long hdl;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4;
  C2F(sciwin)();
  CheckRhs(1,4);
  switch ( Rhs ) 
    {
    case 1 :
      GetRhsVar(1,"d",&m1,&n1,&l1); 
      CheckLength(1,m1*n1,4);
      /* version_flag()  == "0" correspond aux graphics de la version 25001 */
      if (version_flag() == 0)
         if (strcmp(fname,"xrect")==0) 
	    Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),0,0,0,&hdl);
         else
            Objrect (stk(l1),stk(l1+1),stk(l1+2),stk(l1+3),1,0,0,&hdl);
       else
        Xrect(fname,fname_len,stk(l1),stk(l1+1),stk(l1+2),stk(l1+3));
      break;
    case 4 :
      GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);
      GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);
      GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3);
      GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);
      if (version_flag() == 0)
          if (strcmp(fname,"xrect")==0) 
             Objrect (stk(l1),stk(l2),stk(l3),stk(l4),0,0,0,&hdl);
          else
             Objrect (stk(l1),stk(l2),stk(l3),stk(l4),1,0,0,&hdl);
      else
        Xrect(fname,fname_len,stk(l1),stk(l2),stk(l3),stk(l4));
      break;
    default :
      Scierror(999,"%s: wrong number of rhs argumens (%d), rhs must be 1 or 4\r\n",fname,Rhs);
    }
       LhsVar(1)=0;
  return 0;
} 

/*---- 15/02/2002 --------------------------------------------
  scirects
 *-----------------------------------------------------------*/

int scirects(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2;
  long  hdl;
  int i,j;
  long *hdltab;
  C2F(sciwin)();
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
    if ((hdltab = malloc (n1 * sizeof (long))) == NULL) {
      return 0; 
    }
    for (i = 0; i < n1; ++i) { 
      j = (i==0) ? 0 : 1;
      if (*istk(l2+i) == 0)  
	/** fil(i) = 0 rectangle i is drawn using the current line style (or color).**/
	Objrect (stk(l1+(4*i)),stk(l1+(4*i)+1),stk(l1+(4*i)+2),stk(l1+(4*i)+3),0,*istk(l2+i-j),0,&hdl);
      else  
	if (*istk(l2+i) < 0)  
	  /** fil(i) < 0 rectangle i is drawn using the line style (or color) **/
	  Objrect (stk(l1+(4*i)),stk(l1+(4*i)+1),stk(l1+(4*i)+2),stk(l1+(4*i)+3),0,*istk(l2+i),0,&hdl); 
	else         
	  /** fil(i) > 0   rectangle i is filled using the pattern (or color) **/
	  Objrect (stk(l1+(4*i)),stk(l1+(4*i)+1),stk(l1+(4*i)+2),stk(l1+(4*i)+3),1,*istk(l2+i),0,&hdl); 
      hdltab[i]=hdl; /** handle of rectangle i **/
    }
    /** construct agregation and make it current object **/
    sciSetCurrentObj (ConstructAgregation (hdltab, n1));  
    FREE(hdltab);
  }   
  else
    Xrects(fname,fname_len,istk(l2), n1,stk(l1));
   /* NG end */
  LhsVar(1)=0;
  return 0;
} 
 
int sciseteventhandler(fname, fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,ierr;
  integer verb=0,win,na,v;
  double dv;
  C2F(sciwin)();
  CheckRhs(1,1);
  CheckLhs(0,1);
  C2F(dr1)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
  if (Rhs == 1) 
    {
      GetRhsVar(1,"c",&m1,&n1,&l1);
      C2F(seteventhandler) (&win,cstk(l1),&ierr);
    }
  else 
    C2F(seteventhandler) (&win,"",&ierr);
  LhsVar(1)=0;
  return 0;
} 

typedef int (*des_interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct {
  des_interf f;    /** function **/
  char *name;      /** its name **/
} MatdesTable;

 
static MatdesTable Tab[]={
  {scichamp,"champ"},
  {scicontour,"contour"},
  {sciparam3d,"param3d"},
  {sciplot3d,"plot3d"},
  {sciplot3d1,"plot3d1"},
  {sciplot2d,"plot2d"},
  {sciplot2d1_1,"plot2d1"},
  {sciplot2d1_2,"plot2d2"},
  {sciplot2d1_3,"plot2d3"},
  {sciplot2d1_4,"plot2d4"},
  {scigrayplot,"grayplot"},
  {scidriver,"driver"},
  {scixarc,"xfarc"},
  {scixarc,"xarc"},
  {scixarcs,"xarcs"},
  {scirects,"xrects"},/* NG */
  {sciarrows,"xarrows"},
  {scixsegs,"xsegs"},
  {nscixaxis,"drawaxis"},
  {scixchange,"xchange"},
  {scixclea,"xclea"},
  {scirect,"xrect"},/* NG */
  {scirect,"xfrect"},/* NG */
  {scixclear,"xclear"},
  {scixclick,"xclick"},
  {scixend,"xend"},
  {scixfpoly,"xfpoly"},
  {scixfpolys,"xfpolys"},
  {scixget,"xget"},
  {scixinit,"xinit"},
  {scixlfont,"xlfont"},
  {scixnumb,"xnumb"},
  {scixpause,"xpause"},
  {scixpoly,"xpoly"},
  {scixpolys,"xpolys"},
  {scixselect,"xselect"},
  {scixset,"xset"},
  {scixstring,"xstring"},
  {scixstringl,"xstringl"},
  {scixtape,"xtape"},
  {scixsetech,"xsetech"},
  {scixgetech,"xgetech"},
  {scigeom3d,"geom3d"},
  {scifec,"scifec"},
  {scixgetmouse,"xgetmouse"},
  {scixinfo,"xinfo"},
  {scixtitle,"xtitle"},
  {scixgrid,"xgrid"},
  {scixfarcs,"xfarcs"},
  {scixsave,"xsave"},
  {scixload,"xload"},
  {scichamp1,"champ1"},
  {scidelw,"xdel"},
  {scicontour2d,"contour2d"},
  {scixg2ps,"xg2ps"},
  {scixs2fig,"xg2fig"},
  {scixsort,"gsort"},
  {sciwinsid,"winsid"},
  {sciparam3d1,"param3d1"},
  {scixstringb,"xstringb"},
  {scimatplot,"Matplot"},
  {scicontour2d1,"contour2di"},
  {scic2dex,"c2dex"},
  {scigray2plot,"Matplot1"}, 
  {scixgraduate,"xgraduate"},
  {scixname,"xname"},
  {scixaxis,"xaxis"},
  {sciseteventhandler,"seteventhandler"},
  {int_gtkhelp,"help_gtk"},
  /* NG beg */
  {gset,"set"},
  {gget,"get"},
  {delete,"delete"},
  {addcb,"addcb"},
  {copy,"copy"},
  {move,"move"},
  {glue,"glue"},
  {unglue,"unglue"}, 
  {drawnow,"drawnow"},
  {drawlater,"drawlater"},  
  {draw,"draw"}, /* NG end */
  {scixs2gif,"xs2gif"},
  {scixs2ppm,"xs2ppm"},
  {scixs2ps,"xs2ps"},
  {ClearScreenConsole,"clc"},
  {HomeFunction,"tohome"},
  {scisetposfig,"set_posfig_dim"},
  {ShowWindowFunction,"show_window"}
};
  

/* interface for the previous function Table */ 

int C2F(matdes)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  C2F(putlhsvar)();
  return 0;
}

/*-----------------------------------------------------------
 * Utility function for demo 
 *-----------------------------------------------------------*/

int sci_demo (fname,code, flagx) 
     char *fname, *code;
     integer *flagx;
{
  int mlhs=0,mrhs=1,ibegin=1, l1, m1=strlen(code), n1=1;
  static char name[] = "execstr" ;
  Nbvars=0;
  CreateVar(1, "c", &m1, &n1, &l1);
  strcpy(cstk(l1),code);
  /* back conversion to Scilab coding */
  Convert2Sci(1);
  /* execute the Scilab execstr function */
  if ( *flagx == 1) 
    {
      sciprint("Demo of %s\r\n",fname);
      sciprint("%s\r\n",code);
    }
  C2F(recu).krec=-1; /* added to avoid recursion errors */
  SciString(&ibegin,name,&mlhs,&mrhs);
  /* check if an error has occured while running a_function */
  LhsVar(1) = 0; 
  return 0;
}

/* Functions for Entitiy based graphic NG */
/*-----------------------------------------------------------
 * sciset(choice-name,x1,x2,x3,x4,x5)
 * or   xset()
 *-----------------------------------------------------------*/


/************** voir gset([hdl optionnel, le dernier objet],"propriete, val) ***************/
int gset(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,numrow3,numcol3,l3,num,cur,na,verb=0;
  unsigned long hdl; 
  int lw;
  BOOL vis_save;
  sciPointObj *pobj;

  /* F.Leray Adding some tmp variable for SCI_SURFACE / data case*/
  integer m3tl, n3tl, l3tl;
  int numrow[4],i;
  int numcol[4], lxyzcol[4];
  int flagc = -1;


  /* F.Leray Init. to 0*/
  for(i=0;i<4;i++){
    numrow[i] = 0;
    numcol[i] = 0;
    lxyzcol[i] = 0;
  }

  CheckRhs(2,3);
  CheckLhs(0,1);
  /*  set or create a graphic window */
  switch(VarType(1)) 
    {
    case 1: /* tksci handle */
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"set",3);return 0;
      return 0;
      break;
    case 9: /* first is a scalar argument so it's a gset(hdl,"command",[param]) */
      /* F.Leray; INFO: case 9 is considered for a matrix of graphic handles*/
      CheckRhs(3,3);
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */
      if ( *stk(l1) != sciGetHandle(pfiguremdl) && *stk(l1) != sciGetHandle(paxesmdl)
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_title)
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label))
	{
	  if (m1!=1||n1!=1) { 
	    lw = 1 + Top - Rhs;
	    C2F(overload)(&lw,"set",3);return 0;
	  }
	  if (version_flag() ==0)
	    hdl = (unsigned long)*stk(l1); /* Puts the value of the Handle to hdl */ 
	  else
	    hdl = (unsigned long)0;
	  if (hdl == (unsigned long)0 )
	    pobj = (sciPointObj *) NULL;
	  else
	    pobj = sciGetPointerFromHandle(hdl);
	} 
      else 
	{
	  hdl = (unsigned long)*stk(l1);
	  pobj = sciGetPointerFromHandle(hdl);
	}
      
      GetRhsVar(2,"c",&m2,&n2,&l2); /* Gets the command name */  
      if ( *stk(l1) != sciGetHandle(pfiguremdl) && *stk(l1) != sciGetHandle(paxesmdl)
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_title)
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label) /* Addings here F.Leray 10.06.04 */
	   &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label))
	if ((strncmp(cstk(l2),"old_style",9) !=0) 
	    &&(strncmp(cstk(l2),"default_figure",14) !=0) 
	    && (strncmp(cstk(l2),"default_axes",12) !=0) ) C2F(sciwin)();
      
      if (VarType(3) != sciType(cstk(l2),pobj)) { /* F.Leray MODIFICATION ICI*/
	Scierror(999,"%s: uncompatible values for property type  '%s' \r\n",fname,cstk(l2));
	return 0;} 
      if (VarType(3) == 1)  GetRhsVar(3,"d",&numrow3,&numcol3,&l3);
      if (VarType(3) == 9)  GetRhsVar(3,"h",&numrow3,&numcol3,&l3);
      if (VarType(3) == 10) 
	{ if (strncmp(cstk(l2),"tics_labels",11) !=0)
	  {GetRhsVar(3,"c",&numrow3,&numcol3,&l3);} 
	else
	  GetRhsVar(3,"S",&numrow3,&numcol3,&l3); }
      break;
    case 10:/* first is a string argument so it's a gset("command",[param]) */ 
      CheckRhs(2,2);
      GetRhsVar(1,"c",&m2,&n2,&l2);
      if (strncmp(cstk(l2),"default_figure",14) !=0 && strncmp(cstk(l2),"default_axes",12) !=0 )
	{
	  if ((strncmp(cstk(l2),"old_style",9) ==0) || 
	      (strncmp(cstk(l2),"current_figure",14) ==0)) {
	    hdl = (unsigned long)0;
	    pobj = (sciPointObj *) NULL;
	  }
	  else
	    {
	      C2F(sciwin)();
	      if (version_flag() == 0 )
		if ((strncmp(cstk(l2),"zoom_",5) !=0) && 
		    (strncmp(cstk(l2),"auto_",5) !=0) && 
		    (strncmp(cstk(l2),"clip_box",8) !=0) )   
		  hdl = (unsigned long ) sciGetHandle(sciGetCurrentObj ()) ; 
		else  
		  hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin (sciGetCurrentFigure ()));
	      else
		hdl = (unsigned long)0;
	      if (hdl == (unsigned long)0 )
		pobj = (sciPointObj *) NULL;
	      else
		pobj = sciGetPointerFromHandle(hdl);
	    }
	}
      else
	{
	  hdl = (unsigned long)0;
	  pobj = (sciPointObj *) NULL;
	}
      if (VarType(2) != sciType(cstk(l2),pobj)) { /* F.Leray MODIFICATION ICI*/
	if (VarType(2)!=1 || strncmp(cstk(l2),"current_figure",14) !=0) {
	  Scierror(999,"%s: uncompatible values of property type  '%s' \r\n",fname,cstk(l2));

	  return 0;} 
      }
      if ( (VarType(2) == 1) )   {GetRhsVar(2,"d",&numrow3,&numcol3,&l3); }
      if ( (VarType(2) == 9) )   {GetRhsVar(2,"h",&numrow3,&numcol3,&l3); }
      if ( (VarType(2) == 10) ) {
	if (strncmp(cstk(l2),"tics_labels",11) !=0) 
	  {GetRhsVar(2,"c",&numrow3,&numcol3,&l3);} 
	else 
	  GetRhsVar(2,"S",&numrow3,&numcol3,&l3);
      }
      break;
    default:
      Scierror(999,"%s: First argument must be a scalar a handle or a string\r\n",fname);
      return 0;
      break;
    }

  if ( (hdl != (unsigned long)0) ) { /* F.Leray 16.03.04*/
    pobj = sciGetPointerFromHandle(hdl);
    if (pobj == (sciPointObj *)NULL) {
      Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
      return 0;
    }
    vis_save=sciGetVisibility(pobj); /*used not to redraw the figure is object remains invisible SS 20.04.04*/
    /* make a test on SCI_SURFACE here*/
    if(sciGetEntityType(pobj) != SCI_SURFACE || strcmp(cstk(l2),"data") != 0)
      {
	if (sciSet(pobj, cstk(l2), &l3, &numrow3, &numcol3)!=0) {
	  Scierror(999,"%s: %s\r\n",fname,error_message);
	  return 0;
	}
      }
    else
      {	/* F.Leray Work here*/
	if (strncmp(cstk(l2),"data",4) !=0) {
	  sciprint("Impossible case: Handle must be a SCI_SURFACE one and marker must be data\n");
	  return -1;
	}
	if(VarType(3) != 16){
	  Scierror(999,"%s: Incorrect argument, must be a Tlist!\r\n",fname);
	  return -1;
	}
	GetRhsVar(3,"t",&m3tl,&n3tl,&l3tl);
	
	/* GetListRhsVar(3,1,"d",&numrow[0],&numcol[0],&lxyzcol[0]); 
	   Not good because 3,1 is the string character "3d x y z [en option col]"*/
	GetListRhsVar(3,2,"d",&numrow[0],&numcol[0],&lxyzcol[0]);
	GetListRhsVar(3,3,"d",&numrow[1],&numcol[1],&lxyzcol[1]);

	if(m3tl == 4)
	  {
	    GetListRhsVar(3,4,"d",&numrow[2],&numcol[2],&lxyzcol[2]);
	    flagc = 0;
	  }
	else if( m3tl == 5)
	  {
	    GetListRhsVar(3,4,"d",&numrow[2],&numcol[2],&lxyzcol[2]);
	    GetListRhsVar(3,5,"i",&numrow[3],&numcol[3],&lxyzcol[3]);
	    flagc = 1;
	  }
	else
	  {
	    sciprint("Error m3tl must be equal to 4 or 5\r\n");
	    return -1;
	  }
	
	if (set3ddata(pobj, lxyzcol, numrow, numcol,flagc,fname)!=0)  return 0;
      }
    if (!(vis_save==0&&sciGetVisibility(pobj)==0)) {/* do not redraw figure if object remains invisible */
      if ((strncmp(cstk(l2),"figure_style",12) !=0) &&
	  (strncmp(cstk(l2),"old_style",9) !=0 ) && 
	  (strncmp(cstk(l2),"current_axes",12) !=0) &&
	  (strncmp(cstk(l2),"default_figure",14) !=0) && 
	  (strncmp(cstk(l2),"default_axes",12) !=0) &&
	  (pobj != pfiguremdl) && (pobj != paxesmdl)
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
	  && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ){ /* Addings F.Leray 10.06.04 */
	num= sciGetNumFigure (pobj);    
	C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);  
	sciDrawObj(sciGetParentFigure(pobj)); /* F.Leray we redraw here*/
	C2F (dr) ("xset", "window",&cur,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
      }
    }
  }
  else if (sciSet((sciPointObj *) NULL, cstk(l2), &l3, &numrow3, &numcol3)!=0) {
    Scierror(999,"%s: %s\r\n",fname,error_message);
    return 0;
  }
  
  LhsVar(1)=0;
  return 0;
}

int gget(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,numrow2,numcol2,l2 ;
  int l1;
  unsigned long hdl;

  int lw;
  sciPointObj *pobj;

  CheckRhs(1,2);
  CheckLhs(0,1);
  
  /*  set or create a graphic window */

  /* le premier argument peut etre soit un Id, ou un marker(auquel cas, get renvoi la valeur de 
  l'objet courant ) */
  switch(VarType(1))
  {
  case 1: /* tksci handle */
    lw = 1 + Top - Rhs;
    C2F(overload)(&lw,"get",3);return 0;
    return 0;
    break;
  case 9: /* scalar argument (hdl + string) */
    CheckRhs(2,2);
    GetRhsVar(1,"h",&m1,&n1,&l1);
    if (m1!=1||n1!=1) { 
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"get",3);return 0;
    }
    GetRhsVar(2,"c",&numrow2,&numcol2,&l2);
    if ( *stk(l1) != sciGetHandle(pfiguremdl) && *stk(l1) != sciGetHandle(paxesmdl)
	 &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_title)
	 &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_x_label)
	 &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_y_label) /* Addings here F.Leray 10.06.04 */
	 &&  *stk(l1) != sciGetHandle(pSUBWIN_FEATURE(paxesmdl)->mon_z_label))
      {
	if ((strncmp(cstk(l2),"old_style",9) !=0)
	    &&(strncmp(cstk(l2),"default_figure",14) !=0) 
	    && (strncmp(cstk(l2),"default_axes",12) !=0) ) C2F(sciwin)();
	if (version_flag() ==0)
	  hdl = (unsigned long)*stk(l1); /* on recupere le pointeur d'objet par le handle */
	else
	  hdl = (unsigned long)0;
      }/* DJ.A 08/01/04 */
    else 
      hdl = (unsigned long)*stk(l1);
    break;
  case 10:/* string argument (string) */
    CheckRhs(1,1);
    GetRhsVar(1,"c",&numrow2,&numcol2,&l2);
    if (strncmp(cstk(l2),"default_figure",14) !=0 && strncmp(cstk(l2),"default_axes",12) !=0 )
      {
	if (strncmp(cstk(l2),"old_style",9) == 0 || strncmp(cstk(l2),"current_figure",14) ==0) {
	  hdl = (unsigned long)0;
	}
	else
	  {
	    C2F(sciwin)();
	    if (version_flag() == 0)
	      {
		/* Test debug F.Leray 13.04.04 */
		sciPointObj * ppobj;
		if ((strncmp(cstk(l2),"children",8) != 0) &&  
		    (strncmp(cstk(l2),"zoom_",5) !=0) && 
		    (strncmp(cstk(l2),"clip_box",8) !=0) && 
		    (strncmp(cstk(l2),"auto_",5) !=0)) 
		  {
		    ppobj = sciGetCurrentObj();
		    hdl = (unsigned long ) sciGetHandle(sciGetCurrentObj ());}
		else  
		  hdl = (unsigned long ) sciGetHandle(sciGetSelectedSubWin (sciGetCurrentFigure ()));/* on recupere le pointeur d'objet par le handle */
	      }
	    else
	      hdl = (unsigned long)0;
	  }/* DJ.A 08/01/04 */
      }
    else
      hdl = (unsigned long)0;
    
    break;
  default:
    Scierror(999,"%s: Incorrect argument\r\n",fname);
    return 0;
    break;
  }
  /* cstk(l2) est la commande, l3 l'indice sur les parametres de la commande */
  CheckLhs(0,1);
  if (hdl == (unsigned long)0 ) {
    if (sciGet((sciPointObj *) NULL, cstk(l2))!=0) {
      Scierror(999,"%s: %s\r\n",fname,error_message);
      return 0;
    }
  }
  else {
    if ((pobj = sciGetPointerFromHandle(hdl))){
      /* make a test on SCI_SURFACE here*/
      if(sciGetEntityType(pobj) != SCI_SURFACE || strcmp(cstk(l2),"data") != 0)
	{
	  if (sciGet(pobj, cstk(l2))!=0)
	    {
	      Scierror(999,"%s: %s\r\n",fname,error_message);
	      return 0;
	    }
	}
      else
	{
	  if (get3ddata(pobj)!=0)
	    return 0;
	}
    }
    else
      Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
  }
  LhsVar(1)=Rhs+1;
  return 0;
}




/**@name int sciset(sciPointObj *pobj,char *marker, long *x, long *y, long *w, long *h)
 * Sets the value to the object
 */
int sciSet(sciPointObj *pobj, char *marker, int *value, int *numrow, int *numcol)
{
  int xtmp;
  int  i,num,v=1,na,id;
  double dtmp,dv=0.0; 
  char  **str, **ptr, ctmp[10];    
  sciPointObj *psubwin, *figure, *tmpobj;
  struct BCG *XGC;

  /* debug F.Leray 28.04.04 */
  /* sciSubWindow * ppsubwin = NULL;*/
  
  if ((strncmp(marker,"figure_model",12) !=0) && (strncmp(marker,"axes_model",10) !=0)
      && (pobj  != pfiguremdl)  && (pobj  != paxesmdl)
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
      && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ) /* Addings F.Leray 10.06.04 */
    {
      if (pobj != (sciPointObj *)NULL) {
	psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ()); 
	str=pAXES_FEATURE(pobj)->str;  
      }
      
      if ((pobj == (sciPointObj *)NULL) && 
	  (strncmp(marker,"old_style", 9) !=0 ) && 
	  (strncmp(marker,"figure_style", 12) != 0) && 
	  (strncmp(marker,"current_figure", 14) != 0)) 
	{
	  if (version_flag() == 0)
	    strcpy(error_message,"handle not valid");
	  else
	    strcpy(error_message,"function not valid under old graphics style");
	  return -1;
	}
    }
  
  /***************** graphics mode *******************************/ 
  if (strncmp(marker,"color_map", 9) == 0)
    sciSetColormap((sciPointObj *)pobj, stk(*value), *numrow, *numcol);
  else if (strncmp(marker,"old_style", 9) == 0) {
    if ((strncmp(cstk(*value),"on", 3) == 0)) 
      versionflag = 1;
    else if ((strncmp(cstk(*value),"off", 3) == 0))
      versionflag = 0; 
    else {
      strcpy(error_message,"Graphics style must be 'on' or 'off'");
      return -1;
    }
  }
  else if (strncmp(marker,"figure_style", 12) == 0) {
    if (pobj != pfiguremdl)
	{
	  if ((strncmp(cstk(*value),"old", 3) == 0)) {
	    if (version_flag() == 0)  {
	      versionflag = 1;
	      sciXbasc();
	      C2F(dr1)("xset","default",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
	      figure = (sciPointObj *) sciGetCurrentFigure();                       
	      XGC = (struct BCG *) pFIGURE_FEATURE (figure)->pScilabXgc;
	      XGC->mafigure = (sciPointObj *)NULL; 
	      DestroyFigure (figure);
	    }
	  }
	  else if ((strncmp(cstk(*value),"new", 3) == 0)) {   
	    if (version_flag() == 1)  {
	      C2F(dr1)("xset","default",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);      
	      C2F(dr1)("xclear","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
	      C2F(dr1)("xget","window",&v,&num,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L); 
	      C2F(dr)("xstart","v",&num,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
	      XGC=(struct BCG *) sciGetCurrentScilabXgc ();
	      if ((figure = ConstructFigure (XGC)) != NULL) {
		/* Adding F.Leray 25.03.04*/
		sciSetCurrentObj(figure);
		XGC->mafigure = (sciPointObj *) figure;
		XGC->graphicsversion=1;   
		cf_type=1;
		if ((psubwin = ConstructSubWin (figure, XGC->CurWindow)) != NULL){
		  sciSetCurrentObj(psubwin);
		  sciSetOriginalSubWin (figure, psubwin);}
	      }
	    }
	  }
	  else {
	    strcpy(error_message,"Figure style must be 'old' or 'new'");    
	    return -1;
	  }
	}
    else{
      strcpy(error_message,"Cannot set the style of a model");    
      return -1;
    }  
  }  
  else if (strncmp(marker,"pixel_drawing_mode", 18) == 0) {
    if (sciGetEntityType (pobj) == SCI_FIGURE) {
      v=-1;
      for (i=0;i<16;i++) {
	if (strcmp(cstk(*value),pmodes[i])==0) {v=i;break;}
      }
      if (v>=0) 
	sciSetXorMode((sciPointObj *) pobj, v);
      else {
	strcpy(error_message,"Invalid value");  
	return -1;
      }  
    }
    else
      {strcpy(error_message,"pixel_drawing_mode: unknown property for this handle");return -1;}
  }  
  else if (strncmp(marker,"default_values", 14) == 0) {
    if (*stk(*value) == 1)
      {
	if (pobj == pfiguremdl)
	  InitFigureModel();
	else if (pobj == paxesmdl)
	  InitAxesModel();
	else
	  sciSetDefaultValues();
      }
    else {
      strcpy(error_message,"Value must be 1 to set default values");
      return -1;
    }
  }  
  else if (strncmp(marker,"visible", 7) == 0) {
    if ((strncmp(cstk(*value),"on", 2) == 0)) 
      sciSetVisibility((sciPointObj *)pobj, TRUE); 
    else if ((strncmp(cstk(*value),"off", 3) == 0))  
      sciSetVisibility((sciPointObj *)pobj, FALSE); 
    else
      {strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
  } 
  else if (strncmp(marker,"auto_resize", 10) == 0)  { 
    if ((strncmp(cstk(*value),"on", 2) == 0)) 
      sciSetResize((sciPointObj *) pobj, TRUE); 
    else if ((strncmp(cstk(*value),"off", 3) == 0))  
      sciSetResize((sciPointObj *) pobj, FALSE);
    else
      {strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
	       
  }
  /*************************** Handles Properties ********/
  else if (strncmp(marker,"current_obj", 11) == 0) 
    {
      tmpobj=(sciPointObj *)sciGetPointerFromHandle((long)stk(*value)[0]);
      if (tmpobj == (sciPointObj *)NULL) 
	{strcpy(error_message,"Object is not valid");return -1;}
      sciSetCurrentObj(tmpobj);
    }
  else if (strncmp(marker,"current_axes", 12) == 0) 
    {
      tmpobj =(sciPointObj *)sciGetPointerFromHandle((long)stk(*value)[0]);
      if (tmpobj == (sciPointObj *) NULL)
	{strcpy(error_message,"Object is not valid");return -1;}
      if (sciGetEntityType (tmpobj) == SCI_SUBWIN)
	sciSetSelectedSubWin(tmpobj);
      else
	{strcpy(error_message,"Object is not an Axes Entity");return -1;}
    }
  else if (strncmp(marker,"current_figure", 14) == 0) 
    {
      if (VarType(2) != 1) {
	tmpobj =(sciPointObj *)sciGetPointerFromHandle((long)stk(*value)[0]);
	if (tmpobj == (sciPointObj *) NULL)
	  {strcpy(error_message,"Object is not valid");return -1;}
	if (sciGetEntityType (tmpobj) != SCI_FIGURE)
	  {strcpy(error_message,"Object is not a handle on a figure");return -1;}
	num=pFIGURE_FEATURE(tmpobj)->number;
      }
      else
	num=(int)stk(*value)[0];

      C2F(dr1)("xset","window",&num,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,4L,6L);
      sciSwitchWindow(&num);
    }
	
  /************************  figure Properties *****************************/ 
  else if (strncmp(marker,"figure_position", 15) == 0)
    {
      sciSetFigurePos ((sciPointObj *)pobj, (int)stk(*value)[0], (int)stk(*value)[1]);
    } 
  else if (strncmp(marker,"axes_size", 9) == 0)
    {
      pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimwidth=(int)stk(*value)[0];  
      pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimheight=(int)stk(*value)[1];
      if ((sciPointObj *)pobj != pfiguremdl)
	C2F(dr)("xset","wdim",&(pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimwidth),
		&(pFIGURE_FEATURE((sciPointObj *)pobj)->windowdimheight),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    } 
  else if (strncmp(marker,"figure_size", 11) == 0)
    {
      pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth=(int)stk(*value)[0];  
      pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight=(int)stk(*value)[1];
      if ((sciPointObj *)pobj != pfiguremdl)
	C2F(dr)("xset","wpdim",&(pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth),
		&(pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight),PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
    }
  else if (strncmp(marker,"figure_name", 11) == 0) {
    sciSetName((sciPointObj *) pobj, cstk(*value), (*numcol)*(*numrow));
  }
  else if (strncmp(marker,"figure_id", 9) == 0){
    id = (int)stk(*value)[0];
    if ((sciPointObj *)pobj != pfiguremdl)
      {
	C2F(dr)("xset","window",&id,&v,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,5L,7L);
	sciSwitchWindow(&id);
      }
    else
      sciSetNum(pfiguremdl, &id);
  }
  else if (strncmp(marker,"rotation_style", 14) == 0)
    { 
      if (strncmp(cstk(*value),"unary",5)==0 )
	pFIGURE_FEATURE((sciPointObj *)pobj)->rotstyle = 0 ;
      else if (strncmp(cstk(*value),"multiple",8)==0 )
	pFIGURE_FEATURE((sciPointObj *)pobj)->rotstyle = 1 ;
      else  {strcpy(error_message,"Nothing to do (value must be 'unary/multiple')"); return -1;}
    }
  /*Ajout A.Djalel*/
  else if (strncmp(marker,"pixmap", 6) == 0){
    if (strncmp(cstk(*value),"on",2)==0 ){
      pFIGURE_FEATURE(pobj)->pixmap =1;
      /*sciSetVisibility (pobj, FALSE);*/}
    else if (strncmp(cstk(*value),"off",3)==0 ){
      pFIGURE_FEATURE(pobj)->pixmap =0;
      /*sciSetVisibility (pobj, TRUE);
      pFIGURE_FEATURE(pobj)->wshow=0;*/}
    else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
  }
  /********************** context graphique ******************************/
  else if (strncmp(marker,"background", 10) == 0)
    {
      /* I add this line under:*/
      sciSetBackground((sciPointObj *)pobj, (int)stk(*value)[0]);
      
    }
  else if (strncmp(marker,"foreground", 10) == 0)
    {
      sciSetForeground((sciPointObj *)pobj, (int) stk(*value)[0]);
    }

  else if (strncmp(marker,"fill_mode", 9) == 0)
    { 
      if (strncmp(cstk(*value),"on",2)==0 )
	sciSetFillFlag((sciPointObj *)pobj,1);
      else if (strncmp(cstk(*value),"off",3)==0 )
	sciSetFillFlag((sciPointObj *)pobj,0);
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')"); return -1;}
    }
  else if (strncmp(marker,"thickness", 9) == 0)  {
    sciSetLineWidth((sciPointObj *) pobj,(int) *stk(*value));
  }
  else if (strncmp(marker,"line_style", 10) == 0) {
    sciSetLineStyle((sciPointObj *) pobj,(int) *stk(*value));
  }
  else if (strncmp(marker,"mark_style", 10) == 0) {
    sciSetIsMark((sciPointObj *) pobj, 1);
    sciSetMarkStyle((sciPointObj *) pobj,(int) *stk(*value));
  }
  else if (strncmp(marker,"mark_mode", 9) == 0) { 
    if (strncmp(cstk(*value),"on",2)==0 )
      sciSetIsMark((sciPointObj *) pobj,1);
    else if (strncmp(cstk(*value),"off",3)==0 )
      sciSetIsMark((sciPointObj *) pobj,0);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if (strncmp(marker,"mark_size", 9) == 0) {
    sciSetIsMark((sciPointObj *) pobj, 1);
    sciSetLineWidth((sciPointObj *) pobj, (int)*stk(*value));
  }

  else if (strncmp(marker,"polyline_style", 14) == 0)
    {  
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	if ((stk(*value)[0]==1) || (stk(*value)[0]==2) 
	    || (stk(*value)[0]==3) ||(stk(*value)[0]==4)||(stk(*value)[0]==5))
	  pPOLYLINE_FEATURE (pobj)->plot = (int)stk(*value)[0];
	else
	  {strcpy(error_message,"Style must be 1,2,3,4 or 5");return -1;}
      else
	{strcpy(error_message,"Object is not a Polyline");return -1;}
    }

  /************* font properties *********/
  else if (strncmp(marker,"font_size", 9) == 0)
    {
      xtmp = (int)stk(*value)[0];
      sciSetFontDeciWidth((sciPointObj *) pobj, xtmp*100);
    }
  else if (strncmp(marker,"font_angle", 10) == 0)
    {
      xtmp = (int)stk(*value)[0];
      sciSetFontOrientation((sciPointObj *) pobj,(int) (*stk(*value)*10));
    }
  else if (strncmp(marker,"font_foreground", 15) == 0) 
    {
      xtmp = (int) *stk(*value);
      sciSetFontForeground((sciPointObj *) pobj, xtmp);
    }
  else if (strcmp(marker,"font_color") == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->textcolor=(int)*stk(*value);
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      sciSetFontForeground(pobj,(int)*stk(*value));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"font_color property does not exist for this handle");return -1;}
  }	
  else if (strncmp(marker,"font_style", 10) == 0)
    {
      xtmp = (int) *stk(*value);
      if ( (xtmp > 9) || xtmp < 0)
	{strcpy(error_message,"The value must be in [0 9]");return -1;}
      else
	sciSetFontStyle((sciPointObj *) pobj, xtmp);
    }
  else if (strncmp(marker,"font_name", 9) == 0) {
    sciSetFontName((sciPointObj *)pobj, cstk(*value), (*numcol)*(*numrow));
  }

  else if (strncmp(marker,"text_box_mode", 13) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_TEXT) {
	if (strncmp(cstk(*value),"off", 3) == 0)
	  pTEXT_FEATURE (pobj)->fill =  -1;
	else if  (strncmp(cstk(*value),"centered", 8) == 0)
	  pTEXT_FEATURE (pobj)->fill =  0;
	else if  (strncmp(cstk(*value),"filled",6) ==  0)
	  pTEXT_FEATURE (pobj)->fill =  1;
	else  
	  {strcpy(error_message,"Value must be 'off', 'centered' or 'filled'");
	  return -1;}
      }
      else
	{strcpy(error_message,"text_box_mode property does not exist for this handle");
	return -1;}
    }
  else if (strncmp(marker,"text_box", 8) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_TEXT) {
	pTEXT_FEATURE (pobj)->wh[0]=*stk(*value);
	pTEXT_FEATURE (pobj)->wh[1]=*stk(*value+1);
      }
      else
	{strcpy(error_message,"text_box property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"text", 4) == 0) {
    sciSetText((sciPointObj *)pobj, cstk(*value), (*numcol)*(*numrow));
  }
  /******************/
  else if (strncmp(marker,"auto_clear", 10) == 0) {
    if (strncmp(cstk(*value),"on",2)==0 )
      sciSetAddPlot((sciPointObj *) pobj,FALSE);
    else if (strncmp(cstk(*value),"off",3)==0 )
      sciSetAddPlot((sciPointObj *) pobj,TRUE);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if (strncmp(marker,"auto_scale", 10) == 0) {
    if (strncmp(cstk(*value),"on",2)==0 )
      sciSetAutoScale((sciPointObj *) pobj, TRUE);
    else if (strncmp(cstk(*value),"off",3)==0 )
      sciSetAutoScale((sciPointObj *) pobj, FALSE);
    else  {strcpy(error_message,"Value must be 'on/off'"); return -1;}
  }
  else if ((strncmp(marker,"zoom_box", 8) == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN)) { 
    /* On doit avoir avoir une matrice 4x1 */
    if (*numcol * *numrow == 4)
      scizoom(stk(*value));  
    else {strcpy(error_message,"Argument must be a vector of size 4");return -1;}
	      
  } 
  else if ((strncmp(marker,"zoom_state", 9) == 0)  && (sciGetEntityType (pobj) == SCI_SUBWIN)){
    if ((strncmp(cstk(*value),"on", 3) == 0))
      { 
	if (!sciGetZooming((sciPointObj *)pobj))
	  {strcpy(error_message,"set zoom box ( set('zoom_box',[xmin ymin xmax ymax]))");return -1;}
	else
	  {strcpy(error_message,"Object is already zoomed");return -1;}
      }
    else if ((strncmp(cstk(*value),"off", 3) == 0)) 
      { unzoom();
      sciSetZooming((sciPointObj *)pobj,FALSE);
      } 
    else
      {strcpy(error_message,"Value must be 'on/off'");       return -1;}
  }  
  /***********************************************/
  else if (strncmp(marker,"clip_box", 8) == 0)  { 
    /* On doit avoir avoir une matrice 4x1 */
    if (*numcol * *numrow == 4)
      sciSetClipping((sciPointObj *)pobj, stk(*value)); 
    else {strcpy(error_message,"Argument must be a vector of size 4");return -1;}
	      
  } 
  else if (strncmp(marker,"clip_state", 9) == 0) {
    if ((strncmp(cstk(*value),"clipgrf", 7) == 0))
      sciSetIsClipping( (sciPointObj *)pobj,0);
    else if ((strncmp(cstk(*value),"off", 3) == 0)) 
      sciSetIsClipping( (sciPointObj *)pobj,-1);
    else if ((strncmp(cstk(*value),"on", 2) == 0)) 
      {strcpy(error_message,"set clip box ( set('clip_box',[x y w h]) )");  return -1;}
    else
      {strcpy(error_message,"Value must be 'clipgrf', 'on' or 'off'"); return -1;}
  }		
  else if (strcmp(marker,"data") == 0)
    sciSetPoint((sciPointObj *)pobj, stk(*value), numrow, numcol);
     
  /**************** callback *********************/
  else if (strncmp(marker,"callbackmevent", 14) == 0) {
    sciSetCallbackMouseEvent((sciPointObj *)pobj,(int) *stk(*value));
  }
  else if (strncmp(marker,"callback", 8) == 0) {
    sciAddCallback((sciPointObj *)pobj, cstk(*value), (*numcol)*(*numrow),1);
  }
	   
       


  /******** AXES *******************************/
  else if (strncmp(marker,"x_label", 7) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"x_label property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }
 else if (strncmp(marker,"y_label", 7) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"y_label property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }
 else if (strncmp(marker,"z_label", 7) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"z_label property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }
 else if (strncmp(marker,"title", 5) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"title property undefined for this object");return -1;}
      else{
	{strcpy(error_message,"can not set directly a label object");return -1;}
      }
    }

  else if (strncmp(marker,"tics_direction", 14) == 0)
    {   
      if (pAXES_FEATURE (pobj)->ny == 1)
	{ 
	  if(strncmp(cstk(*value),"top",3) == 0) 
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"u",1);
	  else if (strncmp(cstk(*value),"bottom",6) == 0)
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"d",1);
	  else
	    {strcpy(error_message,"Second argument must be 'top' or 'bottom'");return -1;}
	} 
      else
	{
	  if(strncmp(cstk(*value),"right",5) == 0)
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"r",1);
	  else if  (strncmp(cstk(*value),"left",4) == 0)
	    strncpy(&(pAXES_FEATURE (pobj)->dir),"l",1);
	  else
	    {strcpy(error_message,"Second argument must be 'right' or 'left' ");return -1;}
	}
    }
  else if (strncmp(marker,"x_location", 10) == 0)
    {  
      char loc;
      if(strncmp(cstk(*value),"top",3) == 0)
	loc='u';
      else if  (strncmp(cstk(*value),"bottom",6) == 0)
	loc='d';
      else if (strncmp(cstk(*value),"middle",6) == 0)
	loc='c';
      else  
	{strcpy(error_message,"Second argument must be 'top', 'bottom' or 'middle'");return -1;}
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (pobj)->axes.xdir = loc;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"y_location", 10) == 0)
    {   
      char loc;
      if(strncmp(cstk(*value),"left",4) == 0)
	loc='l';
      else if  (strncmp(cstk(*value),"right",5) == 0)
	loc='r';
      else if (strncmp(cstk(*value),"middle",6) == 0)
	loc='c';
      else  
	{strcpy(error_message,"Second argument must be 'left', 'right' or 'middle'");return -1;}
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (pobj)->axes.ydir = loc;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"tight_limits", 12) == 0) 
    {                   
      if ((strncmp(cstk(*value),"off", 3) == 0)) 
	pSUBWIN_FEATURE (pobj)->tight_limits=FALSE;
      else if ((strncmp(cstk(*value),"on", 2) == 0))
	pSUBWIN_FEATURE (pobj)->tight_limits=TRUE;
      else
	{strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
    }
  /**DJ.Abdemouche 2003**/
  else if (strncmp(marker,"view", 4) == 0) 
    { 
      /* DJ.A 2003 */
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {                  
	if ((strncmp(cstk(*value),"2d", 2) == 0)) 
	  {  
	    if(pSUBWIN_FEATURE (pobj)->is3d == FALSE) return 0;  /* Adding F.Leray 18.0604 */
	    if (sciGetSurface(pobj) == (sciPointObj *) NULL)
	      {
		pSUBWIN_FEATURE (pobj)->is3d = FALSE;
		pSUBWIN_FEATURE (pobj)->project[2]= 0;
	      }
	    pSUBWIN_FEATURE (pobj)->theta_kp=pSUBWIN_FEATURE (pobj)->theta;
	    pSUBWIN_FEATURE (pobj)->alpha_kp=pSUBWIN_FEATURE (pobj)->alpha;  
	    pSUBWIN_FEATURE (pobj)->alpha  = 0.0;
	    pSUBWIN_FEATURE (pobj)->theta  = 270.0;
		if(sciGetCurrentScilabXgc () != (struct BCG *) NULL)
            sciRedrawFigure(); /* F.Leray 10.06.04 Adding 2 lines here... */
	    pSUBWIN_FEATURE (pobj)->is3d = FALSE; /*...and here */
	  } 
	else if ((strncmp(cstk(*value),"3d", 2) == 0)){
	  if(pSUBWIN_FEATURE (pobj)->is3d == TRUE) return 0; /* Adding F.Leray 18.0604 */
	  pSUBWIN_FEATURE (pobj)->is3d = TRUE;
	  Obj_RedrawNewAngle(pobj,pSUBWIN_FEATURE (pobj)->theta_kp,pSUBWIN_FEATURE (pobj)->alpha_kp);
	  wininfo("alpha=%.1f,theta=%.1f",pSUBWIN_FEATURE (pobj)->alpha_kp,pSUBWIN_FEATURE (pobj)->theta_kp);
	}            
	else
	  {strcpy(error_message,"Second argument must be '2d' or '3d'");return -1;}
      }
      else
	{strcpy(error_message,"view property does not exist for this handle");return -1;}
     
    } 
  else if (strcmp(marker,"axes_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      if (*numrow * *numcol != 4) 
	{strcpy(error_message,"Second argument must have 4 elements r");return -1;}
      for (i=0;i<4;i++) {
	pSUBWIN_FEATURE (pobj)->WRect[i]=stk(*value)[i];
      }
    }
    else
      {strcpy(error_message,"axes_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"data_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {   
      /**DJ.Abdemouche 2003**/
      if ((*numrow * *numcol != 4) && (*numrow * *numcol != 6)) 
	{strcpy(error_message,"Second argument must have 4 elements (6 if 3d view)");return -1;}

      if(pSUBWIN_FEATURE (psubwin)->logflags[0] == 'n'){ /* General case for x : logflag='n' */
	pSUBWIN_FEATURE (pobj)->SRect[0]=stk(*value)[0];
	pSUBWIN_FEATURE (pobj)->SRect[1]=stk(*value)[1];
      }
      else{/* log. case */
	/*xmin*/
	if(stk(*value)[0] <= 0. || stk(*value)[1] <= 0.)
	  {sciprint("Error: bounds on x axis must be strictly positive to use logarithmic mode\n");return -1;}
	else{
	  pSUBWIN_FEATURE (psubwin)->SRect[0]=stk(*value)[0];
	  pSUBWIN_FEATURE (psubwin)->SRect[1]=stk(*value)[1];
	}
      }
      
      if(pSUBWIN_FEATURE (psubwin)->logflags[1] == 'n'){  /* General case for y : logflag='n' */
	pSUBWIN_FEATURE (psubwin)->SRect[2]=stk(*value)[2];
	pSUBWIN_FEATURE (psubwin)->SRect[3]=stk(*value)[3];
      }
      else{/* log. case */
	/*ymin*/
	if(stk(*value)[2] <= 0. || stk(*value)[3] <= 0.)
	  {sciprint("Error: bounds on y axis must be strictly positive to use logarithmic mode\n");return -1;}
	else{
	  pSUBWIN_FEATURE (psubwin)->SRect[2]=stk(*value)[2];
	  pSUBWIN_FEATURE (psubwin)->SRect[3]=stk(*value)[3];
	}
      }
      
      if (*numrow * *numcol == 6)
	{
	  pSUBWIN_FEATURE (pobj)->SRect[4]=stk(*value)[4];
	  pSUBWIN_FEATURE (pobj)->SRect[5]=stk(*value)[5];
	}
      /* to inform plotxx function to take this boundary into account */
      pSUBWIN_FEATURE (pobj)->FirstPlot = FALSE;
          
  
    }
   else if (sciGetEntityType (pobj) == SCI_SURFACE) {
     if (*numrow * *numcol != 6) 
       {strcpy(error_message,"Second argument must have 6 elements ");return -1;}
     for (i=0;i<6;i++) {
       pSURFACE_FEATURE (pobj)->ebox[i]=stk(*value)[i];
     }
   }
    else
      {strcpy(error_message,"data_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"margins") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {   
      /**DJ.Abdemouche 2003**/
      if (*numrow * *numcol != 4) 
	{strcpy(error_message,"Second argument must have 4 elements ");return -1;}
      pSUBWIN_FEATURE (pobj)->ARect[0]=stk(*value)[0];
      pSUBWIN_FEATURE (pobj)->ARect[1]=stk(*value)[1];
      pSUBWIN_FEATURE (pobj)->ARect[2]=stk(*value)[2];
      pSUBWIN_FEATURE (pobj)->ARect[3]=stk(*value)[3];}
    else
      {strcpy(error_message,"margins property does not exist for this handle");return -1;}
  }
  else if (strncmp(marker,"tics_color", 10) == 0) {   
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->ticscolor = (int)stk(*value)[0];
    else if (sciGetEntityType (pobj) == SCI_SUBWIN)
      pSUBWIN_FEATURE (pobj)->axes.ticscolor = (int)stk(*value)[0];
    else
      {strcpy(error_message,"tics_color property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"tics_style", 10) == 0) { 
    if((strncmp(cstk(*value),"v",1) != 0)&&
       (strncmp(cstk(*value),"r",1) != 0)&&
       (strncmp(cstk(*value),"i",1) != 0)) { 
      strcpy(error_message,"tics must be 'v' or 'r' or 'i'");return -1;
    }

    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->tics = *cstk(*value);
    else {
      strcpy(error_message,"tics_style property does not exist for this handle");return -1;
    }
  }
  /*Dj.A 17/12/2003*/
  else if (strncmp(marker,"sub_tics", 8) == 0) {
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->subint= (int) *stk(*value);
    else if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      if ((*numcol != 3 )&& (*numcol != 2)) {
	strcpy(error_message,"Value must have two elements (three if 3D universe) ");return -1;}
      for (i = 0; i < *numcol;i++)
	pSUBWIN_FEATURE (pobj)->axes.subint[i]=(int) *stk(*value+i); 
    }
    else
      {strcpy(error_message,"sub_tics property does not exist for this handle");return -1;}
    
  }
  /* F.Leray 08.04.04 */
  /* Obsolete property "tics_textsize"*/
 /*  else if (strncmp(marker,"tics_textsize", 13) == 0) {    */
/*     if (sciGetEntityType (pobj) == SCI_AXES) */
/*       pAXES_FEATURE (pobj)->fontsize= (int) stk(*value)[0];  */
/*     else if (sciGetEntityType (pobj) == SCI_SUBWIN) */
/*       pSUBWIN_FEATURE (pobj)->axes.fontsize = (int) stk(*value)[0]; */
/*     else */
/*       {strcpy(error_message,"tics_textsize property does not exist for this handle");return -1;} */
/*   } */
  else if (strncmp(marker,"format_n", 8) == 0)
    {
      strncpy(pAXES_FEATURE (pobj)->format,cstk(*value),1);   
    }
  else if (strncmp(marker,"tics_segment", 12) == 0) 
    {
       if (strncmp(cstk(*value),"on",2)==0 )
 pAXES_FEATURE (pobj)->seg=1;
      else if (strncmp(cstk(*value),"off",3)==0 )
 pAXES_FEATURE (pobj)->seg=0;
      else  {strcpy(error_message,"Nothing to do (value must be 'on/off')");
      return -1;}
    }

  else if (strcmp(marker,"labels_font_size") == 0)	{
    xtmp =  100 * (int) *stk(*value);
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->fontsize = xtmp;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* pSUBWIN_FEATURE (pobj)->axes.fontsize = (int) *stk(*value);*/
      sciSetFontDeciWidth(pobj,xtmp);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_size property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_color") == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES)
      pAXES_FEATURE (pobj)->textcolor=(int)*stk(*value);
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* pSUBWIN_FEATURE (pobj)->axes.textcolor=(int)*stk(*value);*/
      sciSetFontForeground(pobj,(int)*stk(*value));} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_color property does not exist for this handle");return -1;}
  }	
  else if (strcmp(marker,"labels_font_style") == 0)	{ /* Adding F.Leray 09.04.04 : For the moment sciAxes have no style property*/
    if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE)
      sciSetFontStyle(pobj,(int)*stk(*value));
    else
      {strcpy(error_message,"labels_font_style property does not exist for this handle");return -1;}
  }	
  else if (strncmp(marker,"tics_labels", 11) == 0) 
    { 
      if (*numrow != 1)
	{strcpy(error_message,"Second argument must be a vector"); return -1;}
      else
	{
	  ptr= *(char ***)value; /**DJ.Abdemouche 2003**/
	  if (Max(pAXES_FEATURE(pobj)->nx,pAXES_FEATURE(pobj)->ny) != *numcol)
	    {sprintf(error_message,"Value must have %d elements",Max(pAXES_FEATURE(pobj)->nx,pAXES_FEATURE(pobj)->ny));return -1;}
	  else
	    {
	      FREE(pAXES_FEATURE(pobj)->str);
	      if ((pAXES_FEATURE(pobj)->str= malloc
		   (Max(pAXES_FEATURE(pobj)->nx,pAXES_FEATURE(pobj)->ny) * sizeof (char*)))== NULL)
		{strcpy(error_message,"No enough memory to allocate tics labels string !!");return -1;}
	      pAXES_FEATURE(pobj)->str =ptr;
            }
	}
    }
  else if (strncmp(marker,"xtics_coord", 11) == 0) 
    { 
      if (*numrow != 1)
	{strcpy(error_message,"Second argument must be a row vector");return -1;}
      else if (( pAXES_FEATURE(pobj)->nx == 1) &&(*numcol!=1))
	{strcpy(error_message,"Second argument must be a scalar");return -1;}
      else if (( pAXES_FEATURE(pobj)->nx != 1) &&(*numcol==1))
	{strcpy(error_message,"Second argument  must be a vector");return -1;}
      else
	{                       
	  if ( pAXES_FEATURE(pobj)->nx < *numcol)
	    {  
	      if ((str= (char **)realloc(str,(*numcol) * sizeof (char *))) == NULL)
		return 0; 
	      for (i=0; i<*numcol;i++)
		{   
		  if ((str[i]= (char *) MALLOC(strlen(ctmp) * sizeof (char ))) == NULL)
		    return 0; 
		  dtmp=*stk(*value+i);
		  sprintf(ctmp,"%.2f",dtmp);
		  strcpy(str[i],ctmp);
		} 
	      pAXES_FEATURE(pobj)->str=str;
	    }
	  if ((pAXES_FEATURE(pobj)->vx = MALLOC (*numcol * sizeof (double))) == NULL)
	    return 0;
	  pAXES_FEATURE(pobj)->nx= *numcol;
	  for (i = 0; i < *numcol;i++)
	    pAXES_FEATURE(pobj)->vx[i]=*stk(*value+i); 
	}
    }
  else if (strncmp(marker,"ytics_coord", 11) == 0) 
    {
      if (*numrow != 1)
	{strcpy(error_message,"Second argument must be a row vector");return -1;}
      else if (( pAXES_FEATURE(pobj)->ny ==1) &&( *numcol != 1))
	{strcpy(error_message,"Second argument must be a scalar");return -1;}
      else if (( pAXES_FEATURE(pobj)->ny !=1) && (*numcol == 1))
	{strcpy(error_message,"Second argument must be a vector");return -1;}
      else
	{                       
	  if ( pAXES_FEATURE(pobj)->ny < *numcol)
	    { 
	      if ((str= (char **) realloc(str,(*numcol) * sizeof (char *))) == NULL)
		return 0; 
	      for (i=0; i<*numcol;i++)
		{    
		  if ((str[i]= (char *) MALLOC(strlen(ctmp) * sizeof (char ))) == NULL)
		    return 0; 
		  dtmp=*stk(*value+i);
		  sprintf(ctmp,"%.2f",dtmp);
		  strcpy(str[i],ctmp);                                
		}
	      pAXES_FEATURE(pobj)->str=str; 
	    }
	  if ((pAXES_FEATURE(pobj)->vy = MALLOC (*numcol * sizeof (double))) == NULL)
	    return 0;
	  pAXES_FEATURE(pobj)->ny= *numcol; 
	  for (i = 0; i < *numcol;i++)
	    pAXES_FEATURE(pobj)->vy[i]=*stk(*value+i);  
	}
    } 
  else if  (strncmp(marker,"box", 3) == 0) 
    {
      if ((strncmp(cstk(*value),"on", 2) == 0)) 
	pSUBWIN_FEATURE (pobj)->axes.rect= 1; 
      else if ((strncmp(cstk(*value),"off", 3) == 0))  
	pSUBWIN_FEATURE (pobj)->axes.rect= 0;
      else
	{strcpy(error_message,"Second argument must be 'on' or 'off'");return -1;}
    }
  else if (strncmp(marker,"grid", 4) == 0) {/**DJ.Abdemouche 2003**/
    for (i = 0; i < *numcol; i++ )
      {
	if (stk(*value)[i] < -1)
	  {strcpy(error_message,"Argument must be -1 (no grid)  or number of color");return -1;}
	else
	  {pSUBWIN_FEATURE (pobj)->grid[i]=(int) stk(*value)[i];}
      }   
  }
  else if  (strncmp(marker,"axes_visible", 12) == 0) 
    {
      if ((strncmp(cstk(*value),"on", 2) == 0)) 
	pSUBWIN_FEATURE (pobj)->isaxes= TRUE; 
      else if ((strncmp(cstk(*value),"off", 3) == 0))  
	pSUBWIN_FEATURE (pobj)->isaxes= FALSE;
      else
	{strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
    } 
  /* DJ.A 2003 */
  else if (strncmp(marker,"hiddencolor", 11) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	pSUBWIN_FEATURE (pobj)->hiddencolor=(int) stk(*value)[0];
      else if (sciGetEntityType (pobj) == SCI_SURFACE)
	pSURFACE_FEATURE (pobj)->hiddencolor=(int) stk(*value)[0];
      else
	{strcpy(error_message,"hiddencolor property does not exist for this handle");return -1;}
    
    }
  /**DJ.Abdemouche 2003**/
  else if  (strncmp(marker,"isoview", 7) == 0) 
    {  /* DJ.A 2003 */
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if ((strncmp(cstk(*value),"on", 2) == 0)) 
	  pSUBWIN_FEATURE (pobj)->isoview= TRUE; 
	else if ((strncmp(cstk(*value),"off", 3) == 0))  
	  pSUBWIN_FEATURE (pobj)->isoview= FALSE;
	else
	  {strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
      }
      else
	{strcpy(error_message,"isoview property does not exist for this handle");return -1;}
     
    }  /**/
  else if  (strncmp(marker,"cube_scaling", 12) == 0)  /* F.Leray 22.04.04 */
    {  
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if(pSUBWIN_FEATURE (pobj)->is3d == FALSE)
	  sciprint("Warning: cube_scaling property is only used in 3D mode\n");
	
	if ((strncmp(cstk(*value),"on", 2) == 0)) 
	  pSUBWIN_FEATURE (pobj)->cube_scaling= TRUE; 
	else if ((strncmp(cstk(*value),"off", 3) == 0))  
	  pSUBWIN_FEATURE (pobj)->cube_scaling= FALSE;
	else
	  {strcpy(error_message,"Value must be 'on' or 'off' / Only use for 3D mode");return -1;}
      }
      else
	{strcpy(error_message,"cube_scaling property does not exist for this handle");return -1;}
     
    }  /**/
  else if (strncmp(marker,"log_flags", 9) == 0)
    {
      char *flags;
      flags=cstk(*value);
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	if ((*numrow * *numcol ==2 )&& 
	    (flags[0]=='n'||flags[0]=='l')&&
	    (flags[1]=='n'||flags[1]=='l')) {
	 
	  /* Update the data_bounds values ?? NO we will see if data_bounds contains
	     negative boundaries and send an error message to the user */

	  if( (pSUBWIN_FEATURE (pobj)->SRect[0] <= 0. || pSUBWIN_FEATURE (pobj)->SRect[1] <= 0.) 
	      && flags[0] == 'l')
	    {strcpy(error_message,"Error: data_bounds on x axis must be strictly positive to switch to logarithmic mode");return -1;}
	  else
	    pSUBWIN_FEATURE (pobj)->logflags[0]=flags[0];

	  if((pSUBWIN_FEATURE (pobj)->SRect[2] <= 0. || pSUBWIN_FEATURE (pobj)->SRect[3] <= 0.) 
	      && flags[1] == 'l')
	    {strcpy(error_message,"Error: data_bounds on y axis must be strictly positive to switch to logarithmic mode");return -1;}
	  else
	    pSUBWIN_FEATURE (pobj)->logflags[1]=flags[1];
	}
	else 
	  {strcpy(error_message,"incorrect log_flags value");return -1;}
      }
      else
	{strcpy(error_message,"log_flags property does not exist for this handle");return -1;}
      }
  else if (strncmp(marker,"arrow_size", 10) == 0) {
    if(sciGetEntityType (pobj) == SCI_SEGS)
      pSEGS_FEATURE (pobj)->arrowsize = *stk(*value);      
    else
      	{strcpy(error_message,"arrow_size property does not exist for this handle");return -1;}
  }  
  else if ((strncmp(marker,"segs_color", 10) == 0) && (sciGetEntityType (pobj) == SCI_SEGS)){  
    if (pSEGS_FEATURE (pobj)->ptype == 0){
      if ((*numrow)* (*numcol)==1) {
	pSEGS_FEATURE (pobj)->iflag=0;
	for (i = 0; i < (pSEGS_FEATURE (pobj)->Nbr1)/2 ;i++)
	  pSEGS_FEATURE (pobj)->pstyle[i]=(int)*stk(*value);
      } 
      else if (((*numrow)* (*numcol)== (pSEGS_FEATURE (pobj)->Nbr1)/2)) {
	pSEGS_FEATURE (pobj)->iflag=1;
	for (i = 0; i < (pSEGS_FEATURE (pobj)->Nbr1)/2 ;i++)
	  pSEGS_FEATURE (pobj)->pstyle[i]=(integer) *stk(*value+i);
      }
      else
	{ 
	  sprintf(error_message,"segs color has a wrong size (%d), expecting 1 or (%d )",((*numrow)* (*numcol)) ,(pSEGS_FEATURE (pobj)->Nbr1)/2 );
	  return -1;
	}
    } 
    else
      {strcpy(error_message,"segs_color property does not exist for this handle");return -1;}
  }
  else if ((strncmp(marker,"colored", 7) == 0) && (sciGetEntityType (pobj) == SCI_SEGS)){  
    if (pSEGS_FEATURE (pobj)->ptype != 0){
      if ((strncmp(cstk(*value),"on", 2) == 0)) 
	pSEGS_FEATURE (pobj)->pcolored = 1;
      else if ((strncmp(cstk(*value),"off", 3) == 0))  
	pSEGS_FEATURE (pobj)->pcolored = 0;
      else
	{strcpy(error_message,"Value must be 'on' or 'off'");return -1;}
    }
    else
      {strcpy(error_message,"colored property does not exist for Segs");return -1;}
  }
 /**************** Matplot Grayplot *********************/
  else if (strncmp(marker,"data_mapping", 12) == 0) {
    if (sciGetEntityType (pobj) == SCI_GRAYPLOT) {
      if ((strncmp(cstk(*value),"scaled", 6) == 0)||(strncmp(cstk(*value),"direct", 6) == 0)) 
	strncpy(pGRAYPLOT_FEATURE (pobj)->datamapping,cstk(*value),6);
      else
	{strcpy(error_message,"Value must be 'direct' or 'scaled'");return -1;}
    }
    else
      {strcpy(error_message,"data_mapping property does not exist for this handle");return -1;}
  } 
  /**************** Surface *******************************/
  else if (strcmp(marker,"rotation_angles") == 0) {
    /* DJ.A 2003 */
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      Obj_RedrawNewAngle(pobj,*stk(*value+1),*stk(*value));
      wininfo("alpha=%.1f,theta=%.1f",pSUBWIN_FEATURE (pobj)->alpha,pSUBWIN_FEATURE (pobj)->theta);
    }
    else
      {strcpy(error_message,"rotation_angles property does not exist for this handle");return -1;}
  }
  /*DJ.A merge*/
  else if (strcmp(marker,"color_mode") == 0) {    
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
   /*    int m3n,n3n,nc,j; */
      sciSurface * psurf = pSURFACE_FEATURE (pobj);
      

      psurf->flag[0]= (integer) stk(*value)[0];
      

      if(psurf->typeof3d==SCI_FAC3D)  /* we have to deal with colors... and may be update because we just changed  psurf->flag[0]*/
	{
	  if(psurf->flagcolor == 0)
	    {
	      pSURFACE_FEATURE (pobj)->izcol = 0;
	    }
/* 	  else if(psurf->flagcolor == 1) */
/* 	    { */
/* 	      pSURFACE_FEATURE (pobj)->izcol = 0; */
/* 	    } */
	 /*  else if(psurf->flagcolor == 2) /\* Warning: here we need a color vector (one color per facet). N = 1 x dimzy  *\/ */
/* 	    { */
/* 	      m3n = pSURFACE_FEATURE (pobj)->m3n; */
/* 	      n3n = pSURFACE_FEATURE (pobj)->n3n; */
/* 	      nc = psurf->dimzy; /\* rappel: dimzy always equal n3*\/ */
	      
/* 	      psurf->nc = nc; */
/* 	      FREE(psurf->zcol); psurf->zcol = NULL; */
	      
/* 	      if(nc>0){ */
/* 		if ((psurf->zcol = MALLOC (nc * sizeof (int))) == NULL) */
/* 		  return -1; */
/* 	      } */
	      
/* 	      if(m3n * n3n != 0 ){ /\* There is either a color matrix or vector*\/ */
/* 		if( m3n==1 || n3n ==1) */
/* 		  { */
/* 		    /\* We have just enough information to fill the psurf->zcol array*\/ */
/* 		    for (j = 0;j < nc; j++)  /\* nc value is dimzx*dimzy == m3n * n3n *\/ */
/* 		      psurf->zcol[j]= psurf->inputCMoV[j];                 	 */
/* 		  } */
/* 		else if (!( m3n==1 || n3n ==1)) */
/* 		  { */
/* 		    /\* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*\/ */
/* 		    for (j = 0;j < nc; j++)   /\* nc value is dimzy*\/ */
/* 		      psurf->zcol[j]= psurf->inputCMoV[j]; */
/* 		  } */
/* 	      } */
/* 	      else if (m3n * n3n == 0) {/\* There is no color matrix/vect. in input *\/ */
/* 		for (j = 0;j < psurf->dimzy; j++)   /\* nc value is dimzy*\/ */
/* 		  psurf->zcol[j]= abs(psurf->flag[0]); */
/* 	      } */
	      
/* 	    } */
	/*   else if (psurf->flagcolor == 3) */
/* 	    { */
/* 	      m3n = pSURFACE_FEATURE (pobj)->m3n; */
/* 	      n3n = pSURFACE_FEATURE (pobj)->n3n; */
/* 	      nc = psurf->dimzx * psurf->dimzy; */
	      
/* 	      psurf->nc = nc; */
/* 	      FREE(psurf->zcol); psurf->zcol = NULL; */
	      
	      
/* 	      if ((psurf->zcol = MALLOC ( nc * sizeof (int))) == NULL) */
/* 		return -1; */
	      
	      
/* 	      if(m3n * n3n != 0 ){ /\* There is either a color matrix or vector*\/ */
/* 		if( m3n==1 || n3n ==1) /\* COLOR VECTOR *\/ */
/* 		  { */
/* 		    /\* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*\/ */
/* 		    /\* We repeat the data:*\/ */
/* 		    for(i = 0; i< psurf->dimzy; i++){ */
/* 		      for (j = 0;j < psurf->dimzx; j++)  /\* nc value is dimzx*dimzy == m3n * n3n *\/ */
/* 			psurf->zcol[(psurf->dimzx)*i+j]= psurf->inputCMoV[i]; */
/* 		    }               	 */
/* 		  } */
/* 		else if (!( m3n==1 || n3n ==1)) /\* COLOR MATRIX *\/ */
/* 		  { */
/* 		    /\* We have just enough information to fill the psurf->zcol array*\/ */
/* 		    for (j = 0;j < (psurf->dimzx) * (psurf->dimzy); j++)   /\* nc value is dimzy*\/ */
/* 		      psurf->zcol[j]=  psurf->inputCMoV[j]; */
/* 		  } */
/* 	      } */
/* 	      else if (m3n * n3n == 0) {/\* There is no color matrix/vect. in input *\/ */
		
/* 		for(i = 0; i<  psurf->dimzy; i++){ */
/* 		  for (j = 0;j <  psurf->dimzx; j++)  /\* nc value is dimzx*dimzy == m3n * n3n *\/ */
/* 		    psurf->zcol[( psurf->dimzx)*i+j]= abs(psurf->flag[0]); */
/* 		}   */
/* 	      } */
/* 	    } */
	}
    }
    else
      {strcpy(error_message,"color_mode property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"color_flag") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      if (*numrow * *numcol != 1)
	{strcpy(error_message,"Second argument must have 1 elements ");return -1;}
      if (pSURFACE_FEATURE (pobj)->typeof3d==SCI_PLOT3D) {
	if ((*stk(*value)<0)||(*stk(*value)>1))
	  {strcpy(error_message,"Second argument must be equal to 0 or 1");return -1;}
	pSURFACE_FEATURE (pobj)->flagcolor= (int)stk(*value)[0];
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d==SCI_FAC3D) {
	int oldflagcolor,m3n,n3n,nc,j,ii,flagcolor=(int)stk(*value)[0];
	/*int *zcol;*/
	/*F.Leray psurf for debug*/
	sciSurface * psurf = pSURFACE_FEATURE (pobj);

	if ((*stk(*value)<0)||(*stk(*value)>3))
	  {strcpy(error_message,"Second argument must be 0 1 2 or 3");return -1;}


	/* F.Leray 18.03.04 Something goes wrong here: No need to re-build/re-alloc zcol normally!!*/
	/* YES in fact but use of anther fixed array named inputCMoV (inputColorMatrixorVector)*/
	


	if (pSURFACE_FEATURE (pobj)->flagcolor == stk(*value)[0])
	  return 0;
	oldflagcolor = pSURFACE_FEATURE (pobj)->flagcolor;


	if(flagcolor == 0)
	  {
	    pSURFACE_FEATURE (pobj)->izcol = 0;
	  }
	else if(flagcolor == 1)
	  {
	    pSURFACE_FEATURE (pobj)->izcol = 0;
	  }
	else if(flagcolor == 2) /* Warning: here we need a color vector (one color per facet). N = 1 x dimzy  */
	  {
	    m3n = pSURFACE_FEATURE (pobj)->m3n;
	    n3n = pSURFACE_FEATURE (pobj)->n3n;
	    nc = psurf->dimzy; /* rappel: dimzy always equal n3*/
	  
	    psurf->nc = nc;
	    FREE(psurf->zcol); psurf->zcol = NULL;

	    if(nc>0){
	      if ((psurf->zcol = MALLOC (nc * sizeof (int))) == NULL)
		return -1;
	    }
	    
	    if(m3n * n3n != 0 ){ /* There is either a color matrix or vector*/
	      if( m3n==1 || n3n ==1)
		{
		  /* We have just enough information to fill the psurf->zcol array*/
		  for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
		    psurf->zcol[j]= psurf->inputCMoV[j];                 	
		}
	      else if (!( m3n==1 || n3n ==1))
		{
		  /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
		  /* NO !! Let's do better; F.Leray 08.05.04 : */
		  /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
		  /* in our example: m3n=4 and n3n=400 */
		  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
		    {
		      double tmp = 0;
		      for(ii=0;ii<m3n;ii++)
			tmp = tmp +  psurf->inputCMoV[j*m3n + ii];
		      tmp = tmp / m3n;
		      psurf->zcol[j]= (integer)tmp;
		    }
		}
	    }
	    else if (m3n * n3n == 0) {/* There is no color matrix/vect. in input */
	      for (j = 0;j < psurf->dimzy; j++)   /* nc value is dimzy*/
		psurf->zcol[j]= abs(psurf->flag[0]);
	    }
		  
	  }
	else if (flagcolor == 3)
	  {
	    m3n = pSURFACE_FEATURE (pobj)->m3n;
	    n3n = pSURFACE_FEATURE (pobj)->n3n;
	    nc = psurf->dimzx * psurf->dimzy;

	    psurf->nc = nc;
	    FREE(psurf->zcol); psurf->zcol = NULL;
	    

	    if ((psurf->zcol = MALLOC ( nc * sizeof (int))) == NULL)
	      return -1;

	  
	    if(m3n * n3n != 0 ){ /* There is either a color matrix or vector*/
	      if( m3n==1 || n3n ==1) /* COLOR VECTOR */
		{
		  /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
		  /* We repeat the data:*/
		  for(i = 0; i< psurf->dimzy; i++){
		    for (j = 0;j < psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
		      psurf->zcol[(psurf->dimzx)*i+j]= psurf->inputCMoV[i];
		  }               	
		}
	      else if (!( m3n==1 || n3n ==1)) /* COLOR MATRIX */
		{
		  /* We have just enough information to fill the psurf->zcol array*/
		  for (j = 0;j < (psurf->dimzx) * (psurf->dimzy); j++)   /* nc value is dimzy*/
		    psurf->zcol[j]=  psurf->inputCMoV[j];
		}
	    }
	    else if (m3n * n3n == 0) {/* There is no color matrix/vect. in input */
	      
	      for(i = 0; i<  psurf->dimzy; i++){
		for (j = 0;j <  psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
		  psurf->zcol[( psurf->dimzx)*i+j]= abs(psurf->flag[0]);
	      }  
	    }
	  }
	
	pSURFACE_FEATURE (pobj)->flagcolor= flagcolor;
      }
    }    
    else
      {strcpy(error_message,"color_flag property does not exist for this handle");return -1;}
  }
  
  else if (strcmp(marker,"surface_color") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) { 
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PARAM3D1)  {
	if (pSURFACE_FEATURE (pobj)->dimzy != *numrow * *numcol) 
	  {sprintf(error_message,"Second argument must have %d elements ",
		   pSURFACE_FEATURE (pobj)->dimzy);return -1;}
	for (i=0;i<pSURFACE_FEATURE (pobj)->dimzy;i++) 
	  pSURFACE_FEATURE (pobj)->zcol[i]= (integer)stk(*value)[i];
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)  {
	strcpy(error_message,"surface_color cannot be set in this case");
	return -1;
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)  {
	if (pSURFACE_FEATURE (pobj)->flagcolor<2){
	  strcpy(error_message,"surface_color cannot be set in this case");
	  return -1;
	} 
	else {
	  int N;
	  if (pSURFACE_FEATURE (pobj)->flagcolor==2)
	    N=pSURFACE_FEATURE (pobj)->dimzy;
	  else
	    N=pSURFACE_FEATURE (pobj)->dimzy * pSURFACE_FEATURE (pobj)->dimzx;
	  if (*numrow * *numcol != N)
	    {sprintf(error_message,"Second argument must have %d elements ",N);return -1;}
	  for (i=0;i<N;i++) 
	    pSURFACE_FEATURE (pobj)->zcol[i]= (integer)stk(*value)[i];
	}
      }
    }
    else {
      strcpy(error_message,"surface_color property does not exist for this handle");
      return -1;
  } 
}
else if (strcmp(marker,"triangles") == 0) {
  if (sciGetEntityType (pobj) == SCI_FEC) { 
    double *pnoeud;
    if (*numcol != 5)
      {strcpy(error_message,"Second argument must have 5 columns ");return -1;}
    if (*numrow !=pFEC_FEATURE (pobj)->Ntr) {
      pnoeud=pFEC_FEATURE(pobj)->pnoeud;
      if ((pFEC_FEATURE(pobj)->pnoeud = malloc (*numrow * 5* sizeof (int))) == NULL){
	strcpy(error_message,"Not enough memory");
	pFEC_FEATURE(pobj)->pnoeud=pnoeud;
	return -1;
      }
    }
    for (i=0;i<*numrow*5;i++) 
      pFEC_FEATURE (pobj)->pnoeud[i]=stk(*value)[i];
  }
  else
    {strcpy(error_message,"triangles property does not exist for this handle");return -1;}
}
else if (strcmp(marker,"z_bounds") == 0) {
  if (sciGetEntityType (pobj) == SCI_FEC) { 
    if (*numcol *  *numrow!= 2)
      {strcpy(error_message,"Second argument must have 2 elements ");return -1;}

    for (i=0;i<2;i++) 
      pFEC_FEATURE (pobj)->zminmax[i]=stk(*value)[i];
  }
  else
    {strcpy(error_message,"z_bounds property does not exist for this handle");return -1;}
}

else 
{sprintf(error_message,"Unknown  property %s",marker);return -1;}
return 0;
}




/**@name sciGet(sciPointObj *pobj,)
 * Sets the value to the object
 */
int sciGet(sciPointObj *pobj,char *marker)
{
int numrow, numcol, outindex, i,j,k;
integer x[2],x1[10], x2,itmp=0, flagx=0;
sciSons *toto;
double *tab;
char **str;
sciPointObj *psubwin;
int Etype,ids,iflag=0;

if (pobj != (sciPointObj *)NULL && pobj  != pfiguremdl  && pobj  != paxesmdl
    && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_title
    && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_x_label
    && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_y_label
    && pobj != pSUBWIN_FEATURE(paxesmdl)->mon_z_label ){ /* Addings F.Leray 10.06.04 */
psubwin = sciGetSelectedSubWin (sciGetCurrentFigure ());
Etype=sciGetEntityType (pobj);}
  
if ((pobj == (sciPointObj *)NULL) && 
(strncmp(marker,"old_style", 9) !=0 ) && 
(strncmp(marker,"figure_style", 12) != 0) && 
(strncmp(marker,"current_figure", 14) != 0) &&
(strncmp(marker,"default_figure",14) !=0) && 
(strncmp(marker,"default_axes",12) !=0))
{
  if (version_flag() == 0)
    {strcpy(error_message,"handle is not valid");return -1;}
  else
    {strcpy(error_message,"function not valid under old graphics style");return -1;}
}
 else if (strncmp(marker,"figures_id", 10) == 0){
   sciGetIdFigure (&ids,&numcol,&iflag);
   numrow   = 1;
   CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
   iflag = 1; 
   sciGetIdFigure (istk(outindex),&numcol,&iflag);

 }
/***************** graphics mode *******************************/ 
 else if (strncmp(marker,"visible", 7) == 0) {
   numrow   = 1;
   numcol   = 3;
   CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
   if (sciGetVisibility((sciPointObj *)pobj))
     strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
   else 
     strncpy(cstk(outindex),"off", numrow*numcol);
 }
 else if (strncmp(marker,"pixel_drawing_mode", 18) == 0) 
   {
     if (sciGetEntityType (pobj) == SCI_FIGURE) {
       numrow = 1;
       i=pFIGURE_FEATURE (pobj)->gmode.xormode;
       numcol = strlen(pmodes[i]);
       CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
       strncpy(cstk(outindex),pmodes[i], numrow*numcol);
     }
     else
       {strcpy(error_message,"pixel_drawing_mode do not exist for this handle");return -1;}

   }  
 else if (strncmp(marker,"old_style", 9) == 0)
   {
     numrow   = 1;
     numcol   = 3;
     CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
     if (versionflag != 0)
       strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
     else 
	strncpy(cstk(outindex),"off", numrow*numcol);      
		
    }
  else if (strncmp(marker,"figure_style", 12) == 0)
    {
      numrow   = 1;
      if (pobj != pfiguremdl)
	{
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  if (version_flag()!=0)
	    strncpy(cstk(outindex),"old", numrow*numcol); 
	  else 
	    strncpy(cstk(outindex),"new", numrow*numcol);      
	}
      else
	{
	  numcol   = 10;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex),"customised", numrow*numcol);
	}
    }
  else if (strncmp(marker,"auto_resize", 10) == 0)
    {
      numrow   = 1;
      numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      if (sciGetResize((sciPointObj *) pobj))
	strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
      else 
	strncpy(cstk(outindex),"off", numrow*numcol);
    }
  /************************  figure Properties *****************************/ 
  else if (strncmp(marker,"figure_position", 15) == 0)
    {
	Etype=sciGetEntityType (pobj); /* CHECK IF NORMALLY WE HAVE TO REDECLARE Etype here F.Leray 06.04.04 */
      if (Etype != SCI_FIGURE) {
	sprintf(error_message,"%s property undefined for this object",marker);
	return -1;
      }
      numrow   = 1;numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
      if ((sciPointObj *) pobj != pfiguremdl)
	{
#if WIN32 /* Correction pour figure_position (Windows) Allan CORNET Mai 2004 */
	  stk(outindex)[0] = sciGetFigurePosX ((sciPointObj *) pobj); 
	  stk(outindex)[1] = sciGetFigurePosY ((sciPointObj *) pobj);
#else
	  stk(outindex)[0] = sciGetFigurePosX ((sciPointObj *) pobj)-4; 
	  stk(outindex)[1] = sciGetFigurePosY ((sciPointObj *) pobj)-20;
#endif
	}
      else
	{
	  stk(outindex)[0] = pFIGURE_FEATURE (pobj)->inrootposx; 
	  stk(outindex)[1] = pFIGURE_FEATURE (pobj)->inrootposy;
	}
    }  
  else if (strncmp(marker,"axes_size", 9) == 0)
    {
      numrow   = 1;
      numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      if ((sciPointObj *) pobj != pfiguremdl)
	{ 
	  stk(outindex)[0] = sciGetPosWidth ((sciPointObj *) pobj); 
	  stk(outindex)[1] = sciGetPosHeight ((sciPointObj *) pobj); 
	}
      else
	{
	  stk(outindex)[0] = pFIGURE_FEATURE (pobj)->windowdimwidth;
	  stk(outindex)[1] = pFIGURE_FEATURE (pobj)->windowdimheight; 
	}
    } 
  else if (strncmp(marker,"figure_size", 15) == 0)
    {
      numrow   = 1;
      numcol   = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      if ((sciPointObj *) pobj != pfiguremdl)
	{
	  C2F(dr)("xget","wpdim",&itmp,x,&itmp,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
	  pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth=x[0];  
	  pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight=x[1]; 
	}
      stk(outindex)[0] = pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimwidth;  
      stk(outindex)[1] = pFIGURE_FEATURE((sciPointObj *)pobj)->figuredimheight;  
    }
  else if (strncmp(marker,"figure_name", 11) == 0)
    {
      numrow = 1;
      numcol = sciGetNameLength((sciPointObj *) pobj);
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), sciGetName((sciPointObj *) pobj), numrow*numcol);
    }
  else if (strncmp(marker,"figure_id", 9) == 0)
    {
      numrow = 1;
      numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetNum((sciPointObj *)pobj);
    }
  else if (strncmp(marker,"rotation_style", 14) == 0) 
    {
      numrow = 1;numcol = 8;
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      if (pFIGURE_FEATURE((sciPointObj *)pobj)->rotstyle == 0)
	strncpy(cstk(outindex),"unary", numrow*(numcol-3));
      else
	strncpy(cstk(outindex),"multiple",numrow*numcol);
    }
  /*Ajout A.Djalel*/
  else if (strncmp(marker,"pixmap", 6) == 0) 
    {
      numrow = 1;numcol = 3;
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      if (pFIGURE_FEATURE(pobj)->pixmap==1)
	strncpy(cstk(outindex),"on", numrow*(numcol-1));
      else
	strncpy(cstk(outindex),"off", numrow*numcol);
    }

  /********** Handles Properties *********************************************/       
  else if (strncmp(marker,"type", 4) == 0)
    {
      numrow = 1;  
      numcol = strlen(sciGetCharEntityType((sciPointObj *) pobj));
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), sciGetCharEntityType((sciPointObj *) pobj), numrow*numcol);
    }
  else if (strncmp(marker,"parent", 6) == 0)
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *stk(outindex) = (double )sciGetHandle(sciGetParent((sciPointObj *)pobj));
    }
  else if (strncmp(marker,"current_axes", 12) == 0)
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      /**DJ.Abdemouche 2003**/
      *stk(outindex) = (double )sciGetHandle(sciGetSelectedSubWin(sciGetCurrentFigure()));
    }
  
  else if (strncmp(marker,"current_figure", 14) == 0)
    {
      
      if (cf_type==1) {
	C2F(sciwin)();
	numrow   = 1;
	numcol   = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	*stk(outindex) = (double )sciGetHandle(sciGetCurrentFigure());
      }
      else {
	numrow   = 1;
	numcol   = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
#ifdef WITH_TK
	*stk(outindex) = (double )LAB_gcf();
#endif
      }
    }
  else if (strncmp(marker,"current_obj", 11) == 0)
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *stk(outindex) = (double )sciGetHandle(sciGetCurrentObj());
    }
  else if (strncmp(marker,"children", 8) == 0)
    { 
      i = 0;
      toto = sciGetSons((sciPointObj *) pobj);
      while ((toto != (sciSons *)NULL) && (toto->pointobj != (sciPointObj *)NULL))
	{
	  /* DJ.A 30/12 */
	  if(sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_MERGE
	     && sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_LABEL) /* F.Leray 28.05.04 */
	    i++;
	  toto = toto->pnext;
	}
      numrow   = i;
      numcol   = 1;
      if(numrow==0) {
	CreateVar(Rhs+1,"d",&numrow,&numrow,&outindex);
      }
      else {
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	toto = sciGetSons((sciPointObj *) pobj);
	i = 0;
	while ((toto != (sciSons *)NULL) && (toto->pointobj != (sciPointObj *)NULL))
	  { /* DJ.A 30/12 */
	    if(sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_MERGE
	       && sciGetEntityType ((sciPointObj *)toto->pointobj) != SCI_LABEL) /* F.Leray 28.05.04 */
	      {
		stk(outindex)[i] = 
		  (double )sciGetHandle((sciPointObj *)toto->pointobj);
		i++;
	      }
	    toto = toto->pnext;/* toto is pointer to one son */
	  }
      }
    }
  else if (strncmp(marker,"hdl", 3) == 0)
    {
      numrow   = 1;
      numcol   = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      *stk(outindex) = (double ) sciGetHandle(sciGetCurrentObj());
    }
  /* DJ.A 08/01/04 */
  else if (strncmp(marker,"default_figure", 14) == 0)
    {
      if (pfiguremdl != (sciPointObj *) NULL) 
	{
	  numrow   = 1;
	  numcol   = 1;	
	  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	  *stk(outindex) =  (double )sciGetHandle(pfiguremdl);
	}
      else
	{
	  strcpy(error_message,"Default figure do not existed ");
	  return -1;
	}
    }
  else if (strncmp(marker,"default_axes", 12) == 0)
    { 
      if (paxesmdl != (sciPointObj *) NULL) 
	{
	  numrow   = 1;
	  numcol   = 1;
	  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	  *stk(outindex) = (double )sciGetHandle(paxesmdl);	
	}
      else
	{
	  strcpy(error_message,"Default axes do not existed ");
	  return -1;
	}
    }
  /******************************** context graphique  *****************************************/

  else if (strncmp(marker,"color_map", 9) == 0)
    { 
       if ((sciPointObj *) pobj != pfiguremdl)
	{
	  numcol = 3;
	  CheckColormap(&numrow);
	  if ( numrow == 0) numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  C2F(dr1)("xget", "colormap",&flagx,x1,&x2,PI0,PI0,PI0,stk(outindex),PD0,PD0,PD0,5L,bsiz);
	}
       else
	 {
	   numcol = 3;
	   numrow = sciGetNumColors (pobj);
	   CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	   for  (i = 0; i < numcol*numrow; i++)
	       stk(outindex)[i] = pFIGURE_FEATURE(pfiguremdl)->pcolormap[i];
	       
	 }
    }
  else if (strncmp(marker,"background", 10) == 0) /**DJ.Abdemouche 2003**/
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      
      *stk(outindex) = sciGetBackgroundToDisplay((sciPointObj *) pobj);
    }
  else if (strncmp(marker,"foreground", 10) == 0) 
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      /* *stk(outindex) = sciGetForeground((sciPointObj *) pobj);*/
      *stk(outindex) = sciGetForegroundToDisplay((sciPointObj *) pobj);
    }
  else if (strncmp(marker,"fill_mode", 9) == 0) 
    {
      numrow = 1;numcol = 3;
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      if (sciGetFillFlag((sciPointObj *) pobj)==1)
	strncpy(cstk(outindex),"on", numrow*(numcol-1));
      else
	strncpy(cstk(outindex),"off", numrow*numcol);
    }
  else if (strncmp(marker,"thickness", 9) == 0) 
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex) = sciGetLineWidth((sciPointObj *) pobj);
    }
  else if (strncmp(marker,"line_style",10) == 0)	{
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    *stk(outindex) = sciGetLineStyle((sciPointObj *) pobj);
  }
  else if (strncmp(marker,"mark_style", 10) == 0)	{
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    *stk(outindex) = sciGetMarkStyle((sciPointObj *) pobj);
  }
  else if (strncmp(marker,"mark_mode", 9) == 0)
    {
      numrow = 1;numcol = 3;
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      if (sciGetIsMark((sciPointObj *)pobj) == 1)
	strncpy(cstk(outindex),"on", numrow*(numcol-1));
      else
	strncpy(cstk(outindex),"off", numrow*numcol);
    }

  else if (strcmp(marker,"mark_size") == 0)
    {
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    *stk(outindex) = sciGetLineWidth((sciPointObj *) pobj);
    }

  else if (strncmp(marker,"polyline_style", 14) == 0)
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);  
      if (sciGetEntityType (pobj) == SCI_POLYLINE)
	*stk(outindex) = pPOLYLINE_FEATURE (pobj)->plot;
      else
	{ strcpy(error_message,"Unknown polyline property"); return -1;}
    } 

/****************************************************/
  else if (strncmp(marker,"font_size", 9) == 0)
    {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetFontDeciWidth((sciPointObj *)pobj)/100;
    }
  else if (strncmp(marker,"font_angle", 10) == 0)
    {
      numrow = 1; numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = (sciGetFontOrientation((sciPointObj *)pobj))/10;
    }
  else if (strncmp(marker,"font_foreground", 15) == 0)
    {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetFontForeground((sciPointObj *)pobj);
    }
 else if (strcmp(marker,"font_color") == 0)	{ /* F.Leray 09.04.04 : Added for FIGURE and SUBWIN objects */
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_AXES)
      *stk(outindex) = pAXES_FEATURE (pobj)->textcolor;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      *stk(outindex) = sciGetFontForegroundToDisplay(pobj);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"font_color property does not exist for this handle");return -1;}
  }
  else if (strncmp(marker,"font_style", 10) == 0)
    {
      numrow = 1; numcol = 1;
      CreateVar(Rhs+1,"i",&numrow,&numcol,&outindex);
      *istk(outindex) = sciGetFontStyle((sciPointObj *)pobj);
    }
  else if (strncmp(marker,"font_name", 9) == 0)
    {
      numrow = 1;
      numcol = sciGetFontNameLength((sciPointObj *)pobj);
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), sciGetFontName((sciPointObj *)pobj), numrow*numcol);
    }
 else if (strncmp(marker,"text_box_mode", 13) == 0)
   {
     if (sciGetEntityType (pobj) == SCI_TEXT) {
       numrow = 1;
       if (pTEXT_FEATURE (pobj)->fill == -1) {
	 numcol = 3;
	 CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	 strncpy(cstk(outindex),"off", numcol);}
       else if (pTEXT_FEATURE (pobj)->fill == 0) {
	 numcol = 8;
	 CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	 strncpy(cstk(outindex),"centered", numcol);}
       else {
	 numcol = 6;
	 CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	 strncpy(cstk(outindex),"filled", numcol);}
     }
     else
       {strcpy(error_message,"text_box_mode property does not exist for this handle");return -1;}
   }
 else if (strncmp(marker,"text_box", 8) == 0)
   {
     if (sciGetEntityType (pobj) == SCI_TEXT) {
       numrow = 1;
       numcol = 2;
       CreateVar(Rhs+1,"d", &numrow, &numcol, &outindex);
       *stk(outindex)= pTEXT_FEATURE (pobj)->wh[0];
       *stk(outindex+1)= pTEXT_FEATURE (pobj)->wh[1];
     }
     else
       {strcpy(error_message,"text_box property does not exist for this handle");return -1;}
   }
 else if (strncmp(marker,"text", 4) == 0)
   {
     numrow = 1;
     numcol = sciGetTextLength((sciPointObj *)pobj);
     CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
     /*  if(sciGetEntityType(pobj) != SCI_LABEL) */
     strncpy(cstk(outindex), sciGetText((sciPointObj *)pobj), numrow*numcol);
   /*   else */
/*        strncpy(cstk(outindex), sciGetLabel(sciGetSelectedSubWin (sciGetCurrentFigure ()), */
/* 	       pLABEL_FEATURE(pobj)->ptype), numrow*numcol); */
   }

 else if (strncmp(marker,"auto_clear", 10) == 0)
   {
     numrow = 1;numcol = 3;
     CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
     if (!sciGetAddPlot((sciPointObj *)pobj))
       strncpy(cstk(outindex),"on", numrow*(numcol-1));
     else
       strncpy(cstk(outindex),"off",numrow*numcol);
   }
 else if (strncmp(marker,"auto_scale", 10) == 0)
   {
     numrow = 1;numcol = 3;
     CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
     if ( sciGetAutoScale((sciPointObj *)pobj))
       strncpy(cstk(outindex),"on", numrow*(numcol-1));
     else
       strncpy(cstk(outindex),"off",numrow*numcol);
   }
 else if ((strncmp(marker,"zoom_box", 8) == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN))
    {
      if (!sciGetZooming((sciPointObj *)pobj))
	{
	  numrow=0; numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	}
      else
	{
	  numrow=1;numcol=4;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++)
	    stk(outindex)[i] =  pSUBWIN_FEATURE(pobj)->FRect[i];			
	} 
    }
  else if ((strncmp(marker,"zoom_state", 9) == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN))
    {
      numrow   = 1;numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      if (sciGetZooming((sciPointObj *)pobj)) 
	strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
      else 
	strncpy(cstk(outindex),"off", numrow*numcol);
    }
  else if (strncmp(marker,"clip_box", 8) == 0) 
    {
      if ((k=sciGetIsClipping ((sciPointObj *) pobj)) > 0 )
	{ 
	  numrow=1; numcol=4;  
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++)
	    stk(outindex)[i] =  ptabclip[k].clip[i];	
		 
	}
      else if ((k=sciGetIsClipping ((sciPointObj *) pobj)) == 0  && (sciGetEntityType (pobj) == SCI_SUBWIN) )
	{																/* Adding above:  && (sciGetEntityType (pobj) == SCI_SUBWIN) */
																	/* We must first distinguish between SUBWIN and other Objects */
	  numrow=1;numcol=4;  
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  stk(outindex)[0] =  pSUBWIN_FEATURE (pobj)->FRect[0];	/*Check if pobj is a SUBWIN or Other Objects (POLYLINE...) before ! F.Leray 03.03.04 */
	  stk(outindex)[1] =  pSUBWIN_FEATURE (pobj)->FRect[1];
	  stk(outindex)[2] =  pSUBWIN_FEATURE (pobj)->FRect[2] - pSUBWIN_FEATURE (pobj)->FRect[0];	
	  stk(outindex)[3] =  pSUBWIN_FEATURE (pobj)->FRect[3] - pSUBWIN_FEATURE (pobj)->FRect[1];	
	}
      else
	{ 
	  numrow=0;numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);		
	}
    }
  else if (strncmp(marker,"clip_state", 9) == 0) 
    {
      numrow   = 1;numcol   = 7;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      if (sciGetIsClipping (pobj) == 0) 
	strncpy(cstk(outindex),"clipgrf", numrow*numcol); 
      else if (sciGetIsClipping (pobj) > 0) 
	strncpy(cstk(outindex),"on", numrow*(numcol-5));	
      else 
	strncpy(cstk(outindex),"off", numrow*(numcol-4));    
    }
  else if (strcmp(marker,"data") == 0)
    {
      if ((tab = sciGetPoint ((sciPointObj *)pobj, &numrow, &numcol)) == NULL)
	{strcpy(error_message,"No point");return -1;}

      /*  if(sciGetEntityType (pobj) != SCI_SURFACE){ */
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      k=0;
      for (j=0;j < numcol*numrow;j++)
	stk(outindex)[j] = tab[j];
      FREE(tab);
      
    }
  
  /**************** callback *********************/
  else if (strncmp(marker,"callbackmevent", 14) == 0)
    {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"i", &numrow, &numcol, &outindex);
      istk(outindex)[0] = sciGetCallbackMouseEvent((sciPointObj *)pobj);
    }
  else if (strncmp(marker,"callback", 8) == 0)
    {
      numrow = 1;
      numcol = sciGetCallbackLen((sciPointObj *)pobj);
      CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
      strncpy(cstk(outindex), sciGetCallback((sciPointObj *)pobj), numrow*numcol);
    }
	


  /**************************** AXES *************/
  else if (strncmp(marker,"x_label", 7) == 0) /* we send back a handle on the x_label object */
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"x_label property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	stk(outindex)[0] = (double ) 
	  sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_x_label);
	
      }
    }
 else if (strncmp(marker,"y_label", 7) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"y_label property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	stk(outindex)[0] = (double ) 
	  sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_y_label);
      }
    }
 else if (strncmp(marker,"z_label", 7) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"z_label property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	stk(outindex)[0] = (double ) 
	  sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_z_label);
      }
    }
 else if (strncmp(marker,"title", 5) == 0)
    {
      if (sciGetEntityType (pobj) != SCI_SUBWIN)
	{strcpy(error_message,"title property undefined for this object");return -1;}
      else{
	numrow = 1;
	numcol = 1;
	CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
	stk(outindex)[0] = (double ) 
	  sciGetHandle((sciPointObj *) pSUBWIN_FEATURE(pobj)->mon_title);
      }
    }
  else if (strncmp(marker,"log_flags", 9) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow = 1;numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	*cstk(outindex)=pSUBWIN_FEATURE (pobj)->logflags[0];
	*cstk(outindex+1)=pSUBWIN_FEATURE (pobj)->logflags[1];
      }
      else 	
	{strcpy(error_message,"log_flag property undefined for this object");return -1;}

    }
  else if (strncmp(marker,"tics_direction", 14) == 0)
    {
      numrow = 1;
      switch (pAXES_FEATURE (pobj)->dir)
	{
	case 'u': 
	  numcol =  3;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "top" , numrow*numcol);
	  break;
	case 'd': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "bottom" , numrow*numcol);
	  break;
	case 'r': 
	  numcol =  5;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "right" , numrow*numcol); 
	  break;
	case 'l': 
	  numcol =  4;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "left", numrow*numcol); 
	  break;
	default : 
	  strcpy(error_message, "Unexpected error");return -1;
	  break;
	}
    }
  else if (strncmp(marker,"x_location", 10) == 0) 
    {
      char loc;
      numrow = 1;
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	loc = pSUBWIN_FEATURE (pobj)->axes.xdir;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}

      switch (loc)
	{
	case 'u': 
	  numcol =  3;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "top" , numrow*numcol);
	  break;
	case 'd': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "bottom" , numrow*numcol);
	  break;
	case 'c': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "middle" , numrow*numcol); 
	  break;
	default : 
	  strcpy(error_message, "Unexpected error"); return -1;
	  break;
	}
    }  
  else if (strncmp(marker,"y_location", 10) == 0)
    {
      char loc;
      numrow = 1;
      if (sciGetEntityType (pobj) == SCI_SUBWIN)
	loc = pSUBWIN_FEATURE (pobj)->axes.ydir;
      else
	{strcpy(error_message,"x_location property does not exist for this handle");return -1;}

      switch (loc)
	{
	case 'l': 
	  numcol =  4;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "left" , numrow*numcol);
	  break;
	case 'r': 
	  numcol =  5;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "right" , numrow*numcol);
	  break;
	case 'c': 
	  numcol =  6;CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  strncpy(cstk(outindex), "middle" , numrow*numcol); 
	  break;
	default : 
	  strcpy(error_message, "Unexpected error"); return -1;
	  break;
	}
    } 
  else if (strncmp(marker,"tight_limits", 12) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow   = 1;numcol   = 3;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	if (pSUBWIN_FEATURE (pobj)->tight_limits == 1)
	  strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
	else 
	  strncpy(cstk(outindex),"off", numrow*numcol);      
      }
      else
	{strcpy(error_message,"tight_limits property does not exist for this handle");return -1;}
    }
  /**DJ.Abdemouche 2003**/
  else if (strncmp(marker,"view", 4) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow   = 1;numcol   = 2;
	CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	if (pSUBWIN_FEATURE (pobj)->is3d)
	  strncpy(cstk(outindex),"3d", numrow*numcol); 
	else 
	  strncpy(cstk(outindex),"2d", numrow*numcol);      
      }
      else
	{strcpy(error_message,"view property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"axes_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow   = 1;numcol   = 4;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      for (i=0;i<4;i++) {
	stk(outindex)[i] = pSUBWIN_FEATURE (pobj)->WRect[i];
      }
    }
    else
      {strcpy(error_message,"axes_bounds property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"data_bounds") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      /**DJ.Abdemouche 2003**/
	numrow   = 2;
	numcol=(pSUBWIN_FEATURE (pobj)->is3d)? 3 : 2;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  stk(outindex)[0] = pSUBWIN_FEATURE (pobj)->SRect[0];
	  stk(outindex)[1] = pSUBWIN_FEATURE (pobj)->SRect[1];
	  stk(outindex)[2] = pSUBWIN_FEATURE (pobj)->SRect[2];
	  stk(outindex)[3] = pSUBWIN_FEATURE (pobj)->SRect[3];
	  if (pSUBWIN_FEATURE (pobj)->is3d)
	    {
	      stk(outindex)[4] = pSUBWIN_FEATURE (pobj)->SRect[4];
	      stk(outindex)[5] = pSUBWIN_FEATURE (pobj)->SRect[5];	
	    }
    }
    else if (sciGetEntityType (pobj) == SCI_SURFACE) {
	numrow   = 3;numcol   = 2;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol*numrow;i++) {
	  stk(outindex)[i] = pSURFACE_FEATURE (pobj)->ebox[i];
	}
    }
    else
      {strcpy(error_message,"data_bounds property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"margins") == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      /**SS  2004**/
      numrow   = 1;
      numcol=4;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      stk(outindex)[0] = pSUBWIN_FEATURE (pobj)->ARect[0];
      stk(outindex)[1] = pSUBWIN_FEATURE (pobj)->ARect[1];
      stk(outindex)[2] = pSUBWIN_FEATURE (pobj)->ARect[2];
      stk(outindex)[3] = pSUBWIN_FEATURE (pobj)->ARect[3];
    }
    else
      {strcpy(error_message,"data_bounds property does not exist for this handle");return -1;}
  } 
  else if (strncmp(marker,"tics_color", 9) == 0) 
    {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      if (sciGetEntityType (pobj) == SCI_AXES)
	*stk(outindex) = pAXES_FEATURE (pobj)->ticscolor;
      else if (sciGetEntityType (pobj) == SCI_SUBWIN)
	*stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.ticscolor;
      else
	{strcpy(error_message,"tics_color property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"tics_style", 10) == 0)
    {
      numrow = 1; numcol = 1;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex), &pAXES_FEATURE (pobj)->tics , numrow*numcol);
    }
   /*Dj.A 17/12/2003*/
  else if (strncmp(marker,"sub_tics", 8) == 0)
    {
      numrow   = 1;
      numcol   = (sciGetEntityType (pobj) == SCI_AXES) ? 1:2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex); 
      if (sciGetEntityType (pobj) == SCI_AXES)
	*stk(outindex) = pAXES_FEATURE (pobj)->subint;
      else  if (sciGetEntityType (pobj) == SCI_SUBWIN)
	{
	  numcol=(pSUBWIN_FEATURE (pobj)->is3d)? 3 : 2;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++)
	    stk(outindex)[i] = pSUBWIN_FEATURE (pobj)->axes.subint[i];
	}
      else
	{strcpy(error_message,"sub_tics property does not exist for this handle");return -1;}
    }
/* F.Leray 08.04.04 */
/* Obsolete property "tics_textsize"*/
/*   else if (strncmp(marker,"tics_textsize", 13) == 0)  */
/*     { */
/*       numrow   = 1;numcol   = 1; */
/*       CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex); */
/*       if (sciGetEntityType (pobj) == SCI_AXES) */
/* 	*stk(outindex) = pAXES_FEATURE (pobj)->fontsize; */
/*       else */
/* 	*stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.fontsize; */
/*     } */
  else if (strncmp(marker,"tics_segment", 12) == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	numrow = 1;numcol = 3;
	CreateVar(Rhs+1,"c", &numrow, &numcol, &outindex);
	if (pAXES_FEATURE (pobj)->seg==1)
	  strncpy(cstk(outindex),"on", numrow*(numcol-1));
	else
	  strncpy(cstk(outindex),"off", numrow*numcol);
      }
      else
	{strcpy(error_message,"tics_segment property does not exist for this handle");return -1;}
    }
  else if (strcmp(marker,"labels_font_size") == 0)	{
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_AXES)
      *stk(outindex) = pAXES_FEATURE (pobj)->fontsize;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* *stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.fontsize;*/
      *stk(outindex) = sciGetFontDeciWidth(pobj)/100;} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_size property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_color") == 0)	{
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_AXES)
      *stk(outindex) = pAXES_FEATURE (pobj)->textcolor;
    else if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){
      /* *stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.textcolor;*/
      *stk(outindex) = sciGetFontForegroundToDisplay(pobj);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_color property does not exist for this handle");return -1;}
  }
  else if (strcmp(marker,"labels_font_style") == 0)	{ /* Adding F.Leray 09.04.04 */
    numrow   = 1;numcol   = 1;
    CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
    if (sciGetEntityType (pobj) == SCI_SUBWIN || sciGetEntityType (pobj) == SCI_FIGURE){ /* F.Leray 09.04.04: For the moment sciAxes have no font_style property*/
      /* *stk(outindex) = pSUBWIN_FEATURE (pobj)->axes.textcolor;*/
      *stk(outindex) = sciGetFontStyle(pobj);} /* F.Leray 08.04.04 */
    else
      {strcpy(error_message,"labels_font_style property does not exist for this handle");return -1;}
  }
  else if (strncmp(marker,"format_n", 9) == 0)	{
    if (sciGetEntityType (pobj) == SCI_AXES) {
      numrow   = 1;numcol   = 1;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strncpy(cstk(outindex),pAXES_FEATURE (pobj)->format, numrow*numcol);
    }
    else
      {strcpy(error_message,"format_n property does not exist for this handle");return -1;}
  }
  else if (strncmp(marker,"xtics_coord", 11) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	numrow=1;
	numcol= pAXES_FEATURE (pobj)->nx;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol;i++)
	  stk(outindex)[i] = pAXES_FEATURE (pobj)->vx[i];
      }
      else
	{strcpy(error_message,"xtics_coord property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"ytics_coord", 11) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	numrow=1;
	numcol= pAXES_FEATURE (pobj)->ny;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol;i++)
	  stk(outindex)[i] = pAXES_FEATURE (pobj)->vy[i];
      }
      else
	{strcpy(error_message,"ytics_coord property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"tics_labels", 11) == 0)
    {
      if (sciGetEntityType (pobj) == SCI_AXES) {
	numrow=1;
	numcol= Max(pAXES_FEATURE (pobj)->nx,pAXES_FEATURE (pobj)->ny);
	str = pAXES_FEATURE (pobj)->str;
	if (str==NULL){
	  numrow=1;
	  numcol= Max(pAXES_FEATURE (pobj)->nx,pAXES_FEATURE (pobj)->ny);
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++) {
	    stk(outindex)[i] = ((pAXES_FEATURE (pobj)->nx<numcol) ? pAXES_FEATURE (pobj)->vy[i]: pAXES_FEATURE (pobj)->vx[i]);
	  }
	}
	else {
	  CreateVarFromPtr(Rhs+1,"S",&numrow,&numcol,str);
	}
      }
      else
	{strcpy(error_message,"tics_labels property does not exist for this handle");return -1;}
    }
  else if ((strncmp(marker,"box", 3) == 0) && (sciGetEntityType (pobj) == SCI_SUBWIN)) {
    numrow   = 1;
    numcol   = 3;
    CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
    if (pSUBWIN_FEATURE (pobj)->axes.rect==1)
      strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
    else 
      strncpy(cstk(outindex),"off", numrow*numcol);  
  }  
  /**DJ.Abdemouche 2003**/
  else if (strncmp(marker,"grid", 4) == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) 
	{
	numrow   = 1; 
	numcol   = (pSUBWIN_FEATURE (pobj)->is3d)? 3 : 2;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol;i++) 
	  {stk(outindex)[i] = pSUBWIN_FEATURE (pobj)->grid[i];}
	}
      else
	{strcpy(error_message,"grid property does not exist for this handle");return -1;}
    }
  else if (strncmp(marker,"axes_visible", 12) == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow   = 1;numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      if (pSUBWIN_FEATURE (pobj)->isaxes)
	strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
      else 
	strncpy(cstk(outindex),"off", numrow*numcol);  
    }
    else
      {strcpy(error_message,"axes_visible property does not exist for this handle");return -1;}
  }
   /* DJ.A 2003 */
   else if (strncmp(marker,"hiddencolor", 11) == 0) 
    {
      if (sciGetEntityType (pobj) == SCI_SUBWIN) {
	numrow   = 1;numcol   = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pSUBWIN_FEATURE (pobj)->hiddencolor;
      }
      else if (sciGetEntityType (pobj) == SCI_SURFACE) {
	numrow   = 1;numcol   = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pSURFACE_FEATURE (pobj)->hiddencolor;
      }
      else{
	strcpy(error_message,"hiddencolor property does not exist for this handle");
	return -1;
      }
    }
  /**DJ.Abdemouche 2003**/
  else if (strncmp(marker,"isoview", 7) == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow   = 1;numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      if (pSUBWIN_FEATURE (pobj)->isoview)
	strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
      else 
	strncpy(cstk(outindex),"off", numrow*numcol);  
    }
    else
      {strcpy(error_message,"isoview property does not exist for this handle");return -1;}
   }
  else if (strncmp(marker,"cube_scaling", 12) == 0) {
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow   = 1;numcol   = 3;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      if (pSUBWIN_FEATURE (pobj)->cube_scaling)
	strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
      else 
	strncpy(cstk(outindex),"off", numrow*numcol);  
    }
    else
      {strcpy(error_message,"cube_scaling property does not exist for this handle");return -1;}
  }
 /**************** SEGS  *********************/
  else if (strncmp(marker,"arrow_size", 10) == 0)
    {    
      if (sciGetEntityType (pobj) == SCI_SEGS) {
	numrow = 1;numcol = 1;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	*stk(outindex) = pSEGS_FEATURE (pobj)->arrowsize ;
      }
      else
	{strcpy(error_message,"arrow_size property does not exist for this handle");return -1;}
    } 
  else if ((strncmp(marker,"segs_color", 10) == 0) && (sciGetEntityType (pobj) == SCI_SEGS))
    {
      if (pSEGS_FEATURE (pobj)->ptype == 0)
	{
	  numrow = 1;numcol = (pSEGS_FEATURE (pobj)->Nbr1)/2;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol;i++) {
	    stk(outindex)[i] = pSEGS_FEATURE (pobj)->pstyle[i] ;
	  }
	}
    }  
  else if ((strncmp(marker,"colored", 7) == 0) && (sciGetEntityType (pobj) == SCI_SEGS)){
    if (pSEGS_FEATURE (pobj)->ptype != 0)
	{ 
	  numrow   = 1;
	  numcol   = 3;
	  CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
	  if (pSEGS_FEATURE (pobj)->pcolored != 0)
	    strncpy(cstk(outindex),"on", numrow*(numcol-1)); 
	  else 
	    strncpy(cstk(outindex),"off", numrow*numcol); 
        }
    else
     {strcpy(error_message,"colored property does not exist for Segs");return -1;} 
  }
 /**************** Matplot Grayplot *********************/
  else if (strncmp(marker,"data_mapping", 12) == 0) {
    if (sciGetEntityType (pobj) == SCI_GRAYPLOT) {
      numrow = 1;numcol = 6;
      CreateVar(Rhs+1,"c",&numrow,&numcol,&outindex);
      strcpy(cstk(outindex),pGRAYPLOT_FEATURE (pobj)->datamapping);
    }
    else
      {strcpy(error_message,"data_mapping property does not exist for this handle");return -1;}
  } 
 /**************** Surface *******************************/
  else if (strcmp(marker,"rotation_angles") == 0) {
    /* DJ.A 2003 */
    if (sciGetEntityType (pobj) == SCI_SUBWIN) {
      numrow = 1;numcol = 2;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      *stk(outindex)=pSUBWIN_FEATURE (pobj)->alpha;
      *stk(outindex+1)=pSUBWIN_FEATURE (pobj)->theta;
    }
    else
      {strcpy(error_message,"rotation_angle property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"color_mode") == 0) {/*DJ.A merge*/
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  *stk(outindex) = pSURFACE_FEATURE (pobj)->flag[0];
      }
    else
      {strcpy(error_message,"color_mode property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"color_flag") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) {
      numrow = 1;numcol = 1;
      CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
      stk(outindex)[0] = pSURFACE_FEATURE (pobj)->flagcolor;
    }
    else
      {strcpy(error_message,"color_flag property does not exist for this handle");return -1;}
  } 
  else if (strcmp(marker,"surface_color") == 0) {
    if (sciGetEntityType (pobj) == SCI_SURFACE) { 
      if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_PARAM3D1) {
	numrow=1;
	numcol=pSURFACE_FEATURE (pobj)->dimzy;
	CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	for (i=0;i<numcol*numrow;i++) 
	  stk(outindex)[i] = (double)pSURFACE_FEATURE (pobj)->zcol[i];
      }
      else if (pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D) {
	if (pSURFACE_FEATURE (pobj)->flagcolor==2) {
	  numrow=1;
	  numcol=pSURFACE_FEATURE (pobj)->dimzy;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol*numrow;i++) 
	    stk(outindex)[i] = (double)pSURFACE_FEATURE (pobj)->zcol[i];
	}
	else if  (pSURFACE_FEATURE (pobj)->flagcolor==3) {
	  numrow=pSURFACE_FEATURE (pobj)->dimzx;
	  numcol=pSURFACE_FEATURE (pobj)->dimzy;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	  for (i=0;i<numcol*numrow;i++) 
	    stk(outindex)[i] = (double)pSURFACE_FEATURE (pobj)->zcol[i];
	}
	else {
	  numrow=0; numcol=0;
	  CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
	}

      }
    }
    else
      {strcpy(error_message,"surface_color property does not exist for this handle");return -1;}
  } 
  /*  ===================================== FEC ======================================== */
 else if (strcmp(marker,"triangles") == 0) {
   if (sciGetEntityType (pobj) == SCI_FEC) { 
     numrow=pFEC_FEATURE (pobj)->Ntr;
     numcol=5;
     CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
     for (i=0;i<numcol*numrow;i++) 
       stk(outindex)[i] = (double)pFEC_FEATURE (pobj)->pnoeud[i];
   }
   else
     {strcpy(error_message,"triangles property does not exist for this handle");return -1;}
 }
 else if (strcmp(marker,"z_bounds") == 0) {
   if (sciGetEntityType (pobj) == SCI_FEC) { 
     numrow=1;
     numcol=2;
     CreateVar(Rhs+1,"d",&numrow,&numcol,&outindex);
     for (i=0;i<2;i++) 
       stk(outindex)[i] = (double)pFEC_FEATURE (pobj)->zminmax[i];
   }
   else
     {strcpy(error_message,"z_bounds property does not exist for this handle");return -1;}
 }

  else 
    {sprintf(error_message,"Unknown  property %s",marker);return -1;}

  return 0;
}

  

int delete(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{  
  integer m1,n1,l1,m2,n2,l2,num,cur,na,verb=0, lw;
  unsigned long hdl;
  sciPointObj *pobj, *pparentfigure;

  CheckRhs(0,1);
  CheckLhs(0,1);
  /*C2F(sciwin)();*/
  switch(VarType(1)) 
    {
    case 9: /* delete Entity given by a handle */
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */
      if (m1!=1||n1!=1) { 
	lw = 1 + Top - Rhs;
	C2F(overload)(&lw,"delete",6);return 0;}
      if (Rhs == 2)
	GetRhsVar(2,"c",&m2,&n2,&l2); /* Gets the command name */
      hdl = (unsigned long)*stk(l1); /* Puts the value of the Handle to hdl */
      break;
    case 10: /* delete("all") */
      CheckRhs(1,1);
      GetRhsVar(1,"c",&m2,&n2,&l2);
      if (strncmp(cstk(l2),"all", 3) == 0) 
	{   
	  C2F(sciwin)();
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
  C2F(sciwin)();
  C2F (dr) ("xget", "window",&verb,&cur,&na,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F (dr) ("xset", "window",&num,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ((Rhs == 2) && (strncmp(cstk(l2),"callback", 8) == 0))
    sciDelCallback((sciPointObj *)pobj);
  else {  
    if (sciGetParentFigure(pobj) != NULL && sciGetEntityType(pobj) != SCI_FIGURE)  
      { /* I added && sciGetEntityType(pobj) != SCI_FIGURE at last !!!!!!!! F.Leray 09.04.04 */
	pparentfigure = sciGetParentFigure(pobj);
	/*}*/
	sciSetCurrentObj((sciPointObj *)sciGetParent((sciPointObj *)pobj)); /* A LAISSER F.Leray 25.03.04*/
	sciDelGraphicObj((sciPointObj *)pobj);	
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


int addcb(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{  
	unsigned long hdl;
	integer m1, n1, l1,m2, n2, l2;
	sciPointObj *pobj;

 	CheckRhs(2,3);
	CheckLhs(0,1);

	/*  set or create a graphic window*/
	C2F(sciwin)();
	switch(VarType(1)) 
	{
	case 1: /* first is a scalar argument so it's a legend(hdl,"str1",...)*/
                CheckRhs(3,3);
                GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument*/
		hdl = (unsigned long)*stk(l1); /* on recupere le pointeur d'objet par le handle*/
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




int copy(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{  
  unsigned long hdl, hdlparent;
  sciPointObj *pobj, *psubwinparenttarget, *pcopyobj;
  integer m1, n1, l1,l2;
  int numrow, numcol, outindex;

  CheckRhs(1,2);
  CheckLhs(0,1);
        
  /*  set or create a graphic window*/
  C2F(sciwin)();
  if (Rhs == 1)
    {
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument*/
      hdl = (unsigned long)*stk(l1); /* on recupere le pointeur d'objet par le handle*/
      pobj = sciGetPointerFromHandle(hdl);
      if (pobj == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
      psubwinparenttarget = sciGetParentSubwin(sciGetPointerFromHandle(hdl));
    }
  else
    {
      GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument*/
      hdl = (unsigned long)*stk(l1); /* on recupere le pointeur d'objet par le handle*/
      pobj = sciGetPointerFromHandle(hdl);
      if (pobj == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
      GetRhsVar(2,"h",&m1,&n1,&l2); /* Gets the command name */
      hdlparent = (unsigned long)*stk(l2); /* on recupere le pointeur d'objet par le handle*/
      psubwinparenttarget = sciGetPointerFromHandle(hdlparent);
      if ( psubwinparenttarget == NULL) {
	Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
	return 0;
      }
    }
  numrow   = 1;numcol   = 1;
  CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
  *stk(outindex) = (double )sciGetHandle(
					 pcopyobj = sciCopyObj((sciPointObj *)pobj,(sciPointObj *)psubwinparenttarget));
  sciDrawObj((sciPointObj *)sciGetParentFigure(pcopyobj));
  LhsVar(1) = Rhs+1;
  return 0;
}




int move(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  long hdl;
  double x,y;
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3;
  BOOL opt;
   
  CheckRhs(1,3);
  /*  set or create a graphic window */
  C2F(sciwin)();
  opt = FALSE;
  if (Rhs ==3) {
    GetRhsVar(3,"c",&m3,&n3,&l3);
    if (strncmp(cstk(l3),"alone", 5) == 0) opt = TRUE;
    else {
      Scierror(999,"%s: invalid option \r\n",fname); 
      return 0;
    }
  }
    
  GetRhsVar(1,"h",&m1,&n1,&l1); /* Gets the Handle passed as argument */    
  GetRhsVar(2,"d",&m2,&n2,&l2);
  hdl = (unsigned long)*stk(l1); /* Puts the value of the Handle to hdl */
  if (n2 != 2)
    { 
      Scierror(999,"%s: third argument is a vector,[x y] \r\n",fname);
      return 0;
    }
  x = *stk(l2); 
  y =*stk(l2+1);
  Objmove(&hdl,x,y,opt);

  LhsVar(1)=0;
  return 0;
}

int glue(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer numrow,numcol,l1,l2,lind,n,cx1=1 ;
  unsigned long hdl, parenthdl;
  long *handelsvalue;
  int outindex,i;
  sciPointObj *pobj;

  CheckRhs(1,1);
  CheckLhs(0,1);

  /*  set or create a graphic window */
  C2F(sciwin)();
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
  /* we must change the pobj to the agregation type */
  handelsvalue = MALLOC(n*sizeof(long));
  for (i = 0; i < n;i++)
    {
      handelsvalue[i] = (long) (stk(l1))[i];
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

  sciSetCurrentObj ((sciPointObj *)ConstructAgregation (handelsvalue, n));
  
  numrow = 1;
  numcol = 1;
  CreateVar(Rhs+3,"h",&numrow,&numcol,&outindex);
  stk(outindex)[0] = (double )sciGetHandle((sciPointObj *) sciGetCurrentObj());
  LhsVar(1) = Rhs+3;
  FREE(handelsvalue);
  return 0;
}

int unglue(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{ 
  integer m1,n1,l1; 
  unsigned long hdl;
  int numrow, numcol, outindex, i;
  sciPointObj *pobj;  
  sciSons *psonstmp;

  CheckRhs(1,1);
  CheckLhs(0,1);
  /*  set or create a graphic window */
  C2F(sciwin)();
  GetRhsVar(1,"h",&m1,&n1,&l1);
  hdl = (unsigned long)*stk(l1);
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
	  stk(outindex)[i] = 
	    (double )sciGetHandle((sciPointObj *)psonstmp->pointobj);
	  psonstmp = psonstmp->pnext;/* psonstmp   is pointer to one son */
	  i++;
	}
      LhsVar(1) = Rhs+1;
      sciUnAgregation ((sciPointObj *)pobj);
    }
  else
    {
      Scierror(999,"%s: Object must be an agregation\r\n",fname);
    }
  return 0;
}


int drawnow(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{ 
     sciPointObj *subwin;
     integer m,n,l,i;
 
  C2F(sciwin)(); 
  CheckRhs(0,1);
  CheckLhs(0,1); 

  if (version_flag() == 0)
    {
      if (Rhs <= 0) {
	sciSetVisibility (sciGetSelectedSubWin(sciGetCurrentFigure ()), TRUE);  
	sciDrawObj(sciGetCurrentFigure ());
	LhsVar(1) = 0;
	return 0;}
      else
        switch(VarType(1)) 
	  {
	  case 9: /* first is a handle argument so it's a drawnow(subwin) */
	    GetRhsVar(1,"h",&m,&n,&l); 
	    for (i = 0; i < n*m; i++)
	      {
		subwin = (sciPointObj*) sciGetPointerFromHandle((long)*stk(l+i)); 
		if (subwin == NULL) {
		  Scierror(999,"%s :the handle is not or no more valid\r\n",fname);
		  return 0;
		}
		if (sciGetEntityType (subwin) != SCI_SUBWIN) {
		  Scierror(999,"%s: handle does not refer to a sub_window\r\n",fname);
		  return 0;
		}
		else 
		  {	
		    sciSetVisibility (subwin,TRUE);
		    sciDrawObj(sciGetCurrentFigure ());
		  }
                        
	      }
	    break;
	  case 10:/* first is a string argument so it's a drawnow('all') */
	    GetRhsVar(1,"c",&m,&n,&l);
	    if (strncmp(cstk(l),"all", 3) == 0){ 
	      sciSetVisibility (sciGetCurrentFigure (),TRUE);  
	      sciDrawObj(sciGetCurrentFigure ());
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
    }
  LhsVar(1) = 0;
  return 0;
}

int drawlater(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{  
  
  C2F(sciwin)(); 
  CheckRhs(-1,0);
  pFIGURE_FEATURE((sciPointObj *)sciGetCurrentFigure())->visible =FALSE; 
  LhsVar(1) = 0;
  return 0;
}

int scixclearsubwin(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{ 
  unsigned long hdl,hdltab[10];
  sciPointObj *subwin, *tmpsubwin;
  integer m,n,l,i,numrow,numcol,outindex,j=0;
 
  C2F(sciwin)();
  CheckRhs(0,1);
  CheckLhs(0,1);
  if (version_flag() == 0) {
    tmpsubwin = (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ());
    if (Rhs <= 0) { 
      sciSetdrawmode (FALSE); 
      numrow = 1;
      numcol = 1;
      CreateVar(Rhs+1,"h",&numrow,&numcol,&outindex);
      stk(outindex)[0] = (double )sciGetHandle((sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ()));
      LhsVar(1) = Rhs+1;
      return 0;
    }

    switch(VarType(1)) 
      {
      case 1: /* clearsubwin(handle) */
	GetRhsVar(1,"h",&m,&n,&l); 
	for (i = 0; i < n*m; i++) {
	  hdl = (unsigned long)*stk(l+i);            /* Puts the value of the Handle to hdl */ 
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
	    hdltab[j] = hdl;
	    j++;
	    sciSetSelectedSubWin (subwin);
	    sciSetdrawmode (FALSE); 
	  }         
	} 
	break;
      case 10:/*  clearsubwin('all') */
	GetRhsVar(1,"c",&m,&n,&l);
	if (strncmp(cstk(l),"all", 3) == 0){ 
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


int scixbascsubwin(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{ 
  unsigned long hdl;
  sciPointObj *subwin,*psubwinobj,*mafigure;
  sciSons *psons;
  integer m,n,l,i;
 
  C2F(sciwin)();
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
            hdl = (unsigned long)*stk(l+i);            /* Puts the value of the Handle to hdl */ 
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
	if (strncmp(cstk(l),"all", 3) == 0)  {   
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

int draw(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{ 
  unsigned long hdl;
  sciPointObj *pobj, *psubwin, *tmpsubwin;
  integer m,n,l,lw;
/*  BOOL tmpmode; SS 20.04.04*/
   
 
  C2F(sciwin)();
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
      hdl = (unsigned long)*stk(l);            /* Puts the value of the Handle to hdl */ 
      pobj = sciGetPointerFromHandle(hdl);   
    }
    if (pobj != ( sciPointObj *)NULL )  {  
      tmpsubwin = (sciPointObj *) sciGetSelectedSubWin (sciGetCurrentFigure ()); 
      psubwin = (sciPointObj *) sciGetParentSubwin(pobj);
      if (psubwin != ( sciPointObj *)NULL )  {  
	sciSetSelectedSubWin(psubwin); 
	/*	tmpmode = pSUBWIN_FEATURE(psubwin)->visible;
		pSUBWIN_FEATURE(psubwin)->visible = TRUE ;SS 20.04.04*/
	sciSetVisibility(pobj,TRUE) ;
	sciDrawObj(pobj);
	/*pSUBWIN_FEATURE(psubwin)->visible = tmpmode;SS 20.04.04*/
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

static int get3ddata(sciPointObj *pobj)
{
  char *variable_tlist_color[] = {"3d","x","y","z","color"};
  char *variable_tlist[] = {"3d","x","y","z"};
  int m_variable_tlist = 0;
  int n_variable_tlist = 0;
  /*int n_variable_tlist = 1; */

  int flag_c;
  /*int one = 1;
    int four = 4;*/
  int  numrow, numcol,l;

  /* F.Leray debug*/
  /*sciSurface * psurf = pSURFACE_FEATURE (pobj); */
  
  /* tests a faire sur presence et taille color */
  flag_c = pSURFACE_FEATURE (pobj)->m3n;
  if(flag_c !=0)
    {
      m_variable_tlist = 1;
      n_variable_tlist = 5;

      if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)
	{
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3*/
	  /*
	   CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	   CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	   CreateVarFromPtr(Rhs+1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);
	  */
	  
	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist_color);
	  
	  
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3*/
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);

	  numrow = pSURFACE_FEATURE (pobj)->m3n;
	  numcol = pSURFACE_FEATURE (pobj)->n3n;

	  /* F.Leray 24.03.04 Replaced by: */
	  CreateListVarFromPtr(Rhs+1, 5, "i", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->inputCMoV);
	  
	}
      else if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)
	{
	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist_color);
	  
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* but here m1!=m2!=m3 and n1!=n2!=n3*/
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	  
	  numrow = pSURFACE_FEATURE (pobj)->m2;
	  numcol = pSURFACE_FEATURE (pobj)->n2;
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);
      
	  numrow = pSURFACE_FEATURE (pobj)->m3;
	  numcol = pSURFACE_FEATURE (pobj)->n3;
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	  
	  numrow = pSURFACE_FEATURE (pobj)->m3n;
	  numcol = pSURFACE_FEATURE (pobj)->n3n;


	  /* F.Leray 24.03.04 Replaced by: */
	  CreateListVarFromPtr(Rhs+1, 5, "i", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->inputCMoV);

	}
    }
  else /* no color provided in input*/
    {
      m_variable_tlist = 1;
      n_variable_tlist = 4;
      
      if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_FAC3D)
	{
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3 */
	  /*
	   CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	   CreateVarFromPtr(Rhs+1, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	   CreateVarFromPtr(Rhs+1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);
	  */
	  
	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);
	 	  
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* Normaly here m1=m2=m3 and n1=n2=n3 */
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
      	}
      else if(pSURFACE_FEATURE (pobj)->typeof3d == SCI_PLOT3D)
	{
	  /* Add 'variable' tlist items to stack */
	  CreateVar(Rhs+1,"t",&n_variable_tlist,&m_variable_tlist,&l);
	  CreateListVarFromPtr(Rhs+1, 1, "S", &m_variable_tlist, &n_variable_tlist, variable_tlist);
      
	  numrow = pSURFACE_FEATURE (pobj)->m1;
	  numcol = pSURFACE_FEATURE (pobj)->n1; /* but here m1!=m2!=m3 and n1!=n2!=n3 */
	  CreateListVarFromPtr(Rhs+1, 2, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecx);

	  numrow = pSURFACE_FEATURE (pobj)->m2;
	  numcol = pSURFACE_FEATURE (pobj)->n2;
	  CreateListVarFromPtr(Rhs+1, 3, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecy);

	  numrow = pSURFACE_FEATURE (pobj)->m3;
	  numcol = pSURFACE_FEATURE (pobj)->n3;
	  CreateListVarFromPtr(Rhs+1, 4, "d", &numrow,&numcol, &pSURFACE_FEATURE (pobj)->pvecz);
	}
    }
  
  
  return 0;
}


/* set3ddata(pobj,cstk(l2), &l3, &numrow3, &numcol3) */
static int set3ddata(sciPointObj *pobj, int *value, int *numrow, int *numcol, int flagc, char *fname)
{
  int i=0,j,nc;
  sciSurface * psurf = pSURFACE_FEATURE (pobj);


  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n, n3n, l3n, ii;

  double * pvecx = NULL, * pvecy = NULL, * pvecz = NULL;
  integer /* * zcol = NULL,*/ izcol = 0 ;

  
  m1 = numrow[0];
  m2 = numrow[1];
  m3 = numrow[2];
  m3n = numrow[3]; /* F.Leray for color */
  n1 = numcol[0];
  n2 = numcol[1];
  n3 = numcol[2];
  n3n = numcol[3]; /* F.Leray for color */
  l1 = value[0];
  l2 = value[1];
  l3 = value[2];
  l3n = value[3]; /* F.Leray for color */
   
  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) {
    if (! (m1 == m2 && m2 == m3 && n1 == n2 && n2 == n3)) {
      Scierror(999,"%s:  Inside the Tlist (third argument): The three first arguments have incompatible length \r\n",fname);
      return 0;
    }
  } else {
    if (m2 * n2 != n3) {
      Scierror(999,"%s:  Inside the Tlist (third argument): The second and third arguments have incompatible length\r\n",fname);
      return 0;
    }
    if (m1 * n1 != m3) {
      Scierror(999,"%s:  Inside the Tlist (third argument): The first and third arguments have incompatible length\r\n",fname);
      return 0;
    }
    if ( m1*n1 <= 1 || m2*n2 <= 1 ) 
      {
	Scierror(999,"%s: Inside the Tlist (third argument):The first and second arguments should be of size >= 2\r\n",fname);
	return 0;
      }
  }
  
  if (m1 * n1 == 0 || m2 * n2 == 0 || m3 * n3 == 0) { LhsVar(1)=0; return 0;} 
  /* C2F(sciwin)();
     C2F(scigerase)();*/

  if(flagc == 1)
    {
      /*psurf->nc = 1; */  /* Wrong!!  */
      /* psurf->nc = m3n*n3n;*/
      izcol = 1;
      if (   m3n*n3n == m3*n3 ) izcol=2  ;
      psurf->izcol = izcol;
    }
  else
    {
      /*psurf->nc = 0;*/
      psurf->izcol = 0;
    }

  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) /* NG beg */
    { /* case isfac=1;*/
      if(psurf->isfac != 1)
	{
	  sciprint("Can not change the typeof3d of graphic object: its type is SCI_PLOT3D\r\n");
	  return 0;
	}
    }
  else
    { 
      /* case isfac=0;*/
      if(psurf->isfac != 0)
	{
	  sciprint("Can not change the typeof3d of graphic object: its type is SCI_FAC3D\r\n");
	  return 0;
	}
    }
  


  /* Update of the dimzx, dimzy depends on  m3, n3: */
  psurf->dimzx = m3;
  psurf->dimzy = n3;


  /* Free the old values... */
  FREE(psurf->pvecx); psurf->pvecx = NULL;
  FREE(psurf->pvecy); psurf->pvecy = NULL;
  FREE(psurf->pvecz); psurf->pvecz = NULL;
  /* ...even on zcol wich must have been initialized like others or set to NULL in case there was no color before
     The FREE macro tests the NULL pointer existence... */
  FREE(psurf->zcol); psurf->zcol = NULL;
  /* If we had a previous color matrix/vector and we do not specify a new one, I consider we are losing it.*/
  /* That's why we make a FREE as follows:*/
  FREE(psurf->inputCMoV);psurf->inputCMoV = NULL; /* F.Leray 23.03.04*/

  
  /* allocations:*/
  if ((pvecx = MALLOC (m1*n1 * sizeof (double))) == NULL) return -1;
  if ((pvecy = MALLOC (m2*n2 * sizeof (double))) == NULL) {
    FREE(pvecx); pvecx = (double *) NULL;
    return -1;
  } 
  if ((pvecz = MALLOC (m3*n3 * sizeof (double))) == NULL) {
    FREE(pvecx); pvecx = (double *) NULL;
    FREE(pvecy); pvecy = (double *) NULL;
    return -1;
  }

  /* Copy the new values F.Leray */
  for(i=0;i< m1*n1;i++)
    pvecx[i] = stk(l1)[i];
  
  for(i=0;i< m2*n2;i++)
    pvecy[i] = stk(l2)[i];

  for(i=0;i< m3*n3;i++)
    pvecz[i] = stk(l3)[i];
  
  if(flagc ==1) /* F.Leray There is a color matrix */
    {
      if(m3n * n3n != 0) /* Normally useless test here: means we have a color vector or matrix */
	{
	  if (((psurf->inputCMoV = MALLOC (( (m3n)*(n3n) * sizeof (integer)))) == NULL))
	    {
	      FREE(pvecx); pvecx = (double *) NULL;
	      FREE(pvecy); pvecy = (double *) NULL;
	      FREE(pvecz); pvecz = (double *) NULL;
	      return -1;
	    }
	  
	  for (j = 0;j < (m3n)*(n3n); j++)  
	    psurf->inputCMoV[j] = istk(l3n)[j];
	  
	}

      if(psurf->flagcolor == 2)
	{ /* case of SCI_PLOT3D avoid */
	  nc = psurf->dimzy;
	}
      else if(psurf->flagcolor == 3)
	{
	  nc = psurf->dimzx * psurf->dimzy;
	}
      else
	nc=0;

      if ((psurf->zcol = MALLOC (nc * sizeof (int))) == NULL) {
	FREE(pvecx); pvecx = (double *) NULL;
	FREE(pvecy); pvecy = (double *) NULL;
	FREE(pvecz); pvecz = (double *) NULL;
	return -1;
      }

      /* case flagcolor == 2*/
      if(psurf->flagcolor==2 && ( m3n==1 || n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	{
	  /* We have just enough information to fill the psurf->zcol array*/
	  for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3 * n3 */
	    psurf->zcol[j]= psurf->inputCMoV[j];  /* DJ.A 2003 */
	}
      else if(psurf->flagcolor==2 && !( m3n==1 || n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	{
	  /* We have too much information and we take only the first dimzy colors to fill the psurf->zcol array*/
	  /* NO !! Let's do better; F.Leray 08.05.04 : */
	  /* We compute the average value (sum of the value of the nf=m3n vertices on a facet) / (nb of vertices per facet which is nf=m3n) */
	  /* in our example: m3n=4 and n3n=400 */
	  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
	    {
	      double tmp = 0;
	      for(ii=0;ii<m3n;ii++)
		tmp = tmp +  psurf->inputCMoV[j*m3n + ii];
	      tmp = tmp / m3n;
	      psurf->zcol[j]= (integer)tmp;
	    }
	}
      /* case flagcolor == 3*/
      else if(psurf->flagcolor==3 && ( m3n==1 || n3n ==1)) /* it means we have a vector in Color input: 1 color per facet in input*/
	{
	  /* We have insufficient info. to fill the entire zcol array of dimension nc = dimzx*dimzy*/
	  /* We repeat the data:*/
	  for(i = 0; i< psurf->dimzy; i++){
	    for (j = 0;j < psurf->dimzx; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
	      psurf->zcol[psurf->dimzx*i+j]= psurf->inputCMoV[i];  /* DJ.A 2003 */
	  }
	}
      else if(psurf->flagcolor==3 && !( m3n==1 || n3n ==1)) /* it means we have a matrix in Color input: 1 color per vertex in input*/
	{
	  /* We have just enough information to fill the psurf->zcol array*/
	  for (j = 0;j < nc; j++)   /* nc value is dimzy*/
	    psurf->zcol[j]= psurf->inputCMoV[j];
	}
     
    }
  else /* else we put the value of the color_mode flag[0]*/
    {
      
      if(psurf->flagcolor == 2)
	{ /* case of SCI_PLOT3D avoid */
	  nc = psurf->dimzy;
	}
      else if(psurf->flagcolor == 3)
	{
	  nc = psurf->dimzx *  psurf->dimzy;
	}
      else
	nc=0;

      if(nc>0){
	if ((psurf->zcol = MALLOC (nc * sizeof (int))) == NULL) {
	  FREE(pvecx); pvecx = (double *) NULL;
	  FREE(pvecy); pvecy = (double *) NULL;
	  FREE(pvecz); pvecz = (double *) NULL;
	  return -1;
	}
      }

      /* case flagcolor == 2*/
      if(psurf->flagcolor==2) /* we have to fill a Color vector */
	{
	  for (j = 0;j < nc; j++)  /* nc value is dimzx*dimzy == m3n * n3n */
	    psurf->zcol[j]= abs(psurf->flag[0]);
	}
      else if(psurf->flagcolor==3) /* we have to fill a color matrix */
	{
	  for(i = 0; i< psurf->dimzy; i++){
	    for (j = 0;j < psurf->dimzx; j++)
	      psurf->zcol[psurf->dimzx*i+j]=  abs(psurf->flag[0]);
	  }
	}
      else
	{
	  /* case flagcolor == 0 or 1 */
	  psurf->zcol = NULL;
	  psurf->izcol = 0;
	}
    }
  
  psurf->pvecx = pvecx;
  psurf->pvecy = pvecy;
  psurf->pvecz = pvecz;

  psurf->nc = nc; /*Adding F.Leray 23.03.04*/

  psurf->m1 = m1;
  psurf->m2 = m2;
  psurf->m3 = m3;
  psurf->n1 = n1;
  psurf->n2 = n2;
  psurf->n3 = n3;
  psurf->m3n = m3n; /* If m3n and n3n are 0, then it means that no color matrix/vector was in input*/
  psurf->n3n = n3n;
      

  return 0;
}




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
     2. pick up the lowest positive value inside xtmp (<=> lowest positive value inside x) */
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
