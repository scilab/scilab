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
#include "../stack-c.h"
#include "../graphics/Math.h"
#include "../graphics/PloEch.h"
#include "matdes.h"

#ifndef NULL
#define NULL 0
#endif 

extern int C2F (deletewin) __PARAMS((integer *number));  
extern void C2F(gsorts)  __PARAMS((char **data,int *ind,int *iflag, int *m,int *n,
				  char *type,char *iord));
extern int C2F(gsort)  __PARAMS((int *xI,double *xD,int *ind,int *iflag, int *m,int *n,
				  char *type,char *iord));
extern void ShowScales  __PARAMS((void));
static integer one = 1, zero = 0;

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

static double def_rect[4]  = {0.,0.,10.0,10.0}; 
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
	  def_rect[0] = def_rect[1] = 0.0; def_rect[2]=def_rect[3]=10.0;
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
      def_rect[0] = def_rect[1] = 0.0; def_rect[2]=def_rect[3]=10.0;
      Rect = def_rect ;
    }
  /*if ((Rect != def_rect)&&(Strf !=def_strf)) {
    }*/
    
  return 1;
}

#define GetStrf(pos,opts) if ( get_strf(fname,pos,opts) == 0) return 0;

#define DEFSTRF "081" 
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
      strcpy(def_strf,DEFSTRF);  Strf = def_strf ;
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
      for (i = 0 ; i < 4; ++i) *istk(l+i) = Max((integer) *istk(l+i),1);
      Nax=istk(l);
    }
    else
      {
	/** global value can be modified  **/
	def_nax[0]=2;def_nax[1]=10;def_nax[2]=2;def_nax[3]=10; 
	Nax=def_nax;
      }
  }
  else if ((kopt=FindOpt("nax",opts))) {
    GetRhsVar(kopt, "i", &m, &n, &l);
    CheckLength(kopt,m*n,4);
    for (i = 0 ; i < 4; ++i) *istk(l+i) = Max((integer) *istk(l+i),1);
    Nax=istk(l);
  }
  else 
    {
      /** global value can be modified  **/
      def_nax[0]=2;def_nax[1]=10;def_nax[2]=2;def_nax[3]=10; 
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
  static double  ebox_def[6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]= {1,2,4};
  integer iflag[3] , *ifl,  ix1 ;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
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


  GetOptionalDoubleArg(4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(5,"alpha",&alpha,1,opts);
  GetLegend(6,opts);
  ifl=&(iflag_def[1]);
  GetOptionalIntArg(7,"flag",&ifl,2,opts);
  iflag[0]=iflag_def[0];iflag[1]=ifl[0];iflag[2]=ifl[1];
  GetOptionalDoubleArg(8,"ebox",&ebox,6,opts);

  C2F(sciwin)();
  C2F(scigerase)();
  ix1 = m1 * n1;
  C2F(param3d)(stk(l1 ),stk(l2 ),stk(l3 ),&ix1, theta, alpha,Legend,iflag, ebox,strlen(Legend));
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
  integer izcol, *zcol=NULL;
  static double  ebox_def [6]= { 0,1,0,1,0,1};
  double *ebox = ebox_def ;
  static integer iflag_def[3]={1,2,4};
  integer iflag[3] , *ifl;
  double  alpha_def=35.0 , theta_def=45.0 ;
  double *alpha=&alpha_def, *theta=&theta_def;
  integer m1, n1, l1, m2, n2, l2, m3, n3, l3;
  integer m3n, n3n, l3n, m3l, n3l, l3l;
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
    sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
	       fname,4);
    Error(999); 
    return(0);
  }
  GetRhsVar(1, "d", &m1, &n1, &l1);/* x */
  GetRhsVar(2, "d", &m2, &n2, &l2);/* y */
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

  CheckSameDims(1,3,m1,n1,m3,n3); 

  GetOptionalDoubleArg(4,"theta",&theta,1,opts);
  GetOptionalDoubleArg(5,"alpha",&alpha,1,opts);
  GetLegend(6,opts);
  ifl=&(iflag_def[1]);
  GetOptionalIntArg(7,"flag",&ifl,2,opts);
  iflag[0]=iflag_def[0];iflag[1]=ifl[0];iflag[2]=ifl[1];

  GetOptionalDoubleArg(8,"ebox",&ebox,6,opts);

  if (m1 == 1 && n1 > 1) { m1 = n1;    n1 = 1; }
  C2F(sciwin)();
  C2F(scigerase)();
  C2F(param3d1)(stk(l1), stk(l2), stk(l3), &m1, &n1, &izcol,zcol, 
		theta, alpha, Legend,iflag,ebox, bsiz);
  LhsVar(1) = 0;
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
  integer m3n, n3n, l3n, m3l, n3l, l3l;

  integer izcol, *zcol=NULL;

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

  if (m1 * n1 == m3 * n3 && m1 * n1 == m2 * n2 && m1 * n1 != 1) 
    { 
      if (izcol == 0) 
	{
	  /*  Here we are in the case where x,y and z specify some polygons */
	  (*func1)(stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox,bsiz);
	} 
      else if (izcol == 2) 
	{
	  /*  New case for the fac3d3 call (interpolated shadig)
	   */
	  (*func3)(stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox,bsiz);	}
      else 
	{
	  (*func2)(stk(l1),stk(l2),stk(l3),zcol,&m3,&n3,theta,alpha,Legend,iflag,ebox,bsiz);
	}
    } 
  else 
    {
      (*func)(stk(l1 ),stk(l2 ),stk(l3 ),&m3,&n3,theta,alpha,Legend,iflag,ebox,bsiz);
    }
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
  static char str[]="x=0:0.1:2*%pi,plot2d([x;x;x]',[sin(x);sin(2*x);sin(3*x)]',[-1,-2,3],'151','L1@L2@L3',[0,-2,2*%pi,2]);";
  integer m1, n1, l1, m2, n2, l2, lt;
  int test,i,j,iskip;
  int frame_def=8;
  int *frame=&frame_def;
  int axes_def=1;
  int *axes=&axes_def;

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
	sciprint("%s: misplaced optional argument, first must be at position %d \r\n",
		 fname,3+iskip);
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
  if (Logflags != def_logflags) {
    C2F(plot2d1)(Logflags,stk(l1),stk(l2),&n1,&m1,Style,Strf,Legend,Rect,Nax,
		 4L,strlen(Strf),strlen(Legend));
  }
  else
    C2F(plot2d)(stk(l1), stk(l2), &n1, &m1, Style, Strf,Legend, Rect, Nax, 4L, bsiz);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *   plot2dxx(str,x,y,[style,strf,leg,rect,nax])
 *-----------------------------------------------------------*/

int sciplot2d1_G(fname, func, fname_len)
     char *fname;
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
    sprintf(C2F(cha1).buf,
	    "x=0:0.1:2*%%pi; %s('gnn',[x;x;x]',[sin(x);sin(2*x);sin(3*x)]',[-1,-2,3],'151','L1@L2@L3',[0,-2,2*%%pi,2]);",fname);
    sci_demo(fname,C2F(cha1).buf,&one);
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
  C2F(xgray)(stk(l1), stk(l2), stk(l3), &m3, &n3, Strf, Rect, Nax, 4L);
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
  C2F(xgray1)(stk(l1), &m1, &n1, Strf, Rect, Nax, 4L);
  LhsVar(1) = 0;
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
  C2F(xgray2)(stk(l1), &m1, &n1,stk(l2));
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
  integer m1,n1,l1,l2,l3,l4,l5,l6,v;
  C2F(sciwin)();
  CheckRhs(6,6);
  GetRhsVar(1,"d",&m1,&n1,&l1);CheckScalar(1,m1,n1);
  GetRhsVar(2,"d",&m1,&n1,&l2);CheckScalar(2,m1,n1);
  GetRhsVar(3,"d",&m1,&n1,&l3);CheckScalar(3,m1,n1);
  GetRhsVar(4,"d",&m1,&n1,&l4);CheckScalar(4,m1,n1);
  GetRhsVar(5,"i",&m1,&n1,&l5);CheckScalar(5,m1,n1);
  GetRhsVar(6,"i",&m1,&n1,&l6);CheckScalar(6,m1,n1);
  C2F(dr1)(fname,"v",&v,&v,&v,&v,istk(l5),istk(l6),stk(l1),stk(l2),stk(l3),stk(l4),
	   fname_len,1L);
  LhsVar(1)=0;
  return 0;
} 

/*-----------------------------------------------------------
 *-----------------------------------------------------------*/

int scixarcs(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,v;
  double dv;
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
  C2F(dr1)(fname,"v",&v,istk(l2),&n1,&v,&v,&v,stk(l1),&dv,&dv,&dv,fname_len,2L);
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
  int m1,n1,l1,m2,n2,l2,v;
  double dv;

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
  C2F(dr1)(fname,"v",&v,istk(l2),&n1,&v,&v,&v,stk(l1),&dv,&dv,&dv,fname_len,2L);
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
  integer dstyle = -1,v,m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3;
  integer m4,n4,l4,mn2;
  double dv,arsize=-1.0 ;

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
  if (Rhs == 4 && m4 * n4 != 1) {
    C2F(dr1)("xarrow","v",istk(l4),&one,&mn2,&v,&v,&v,stk(l1),stk(l2),&arsize,&dv,7L,2L);
  } else {
    C2F(dr1)("xarrow","v",&dstyle,&zero,&mn2,&v,&v,&v,stk(l1),stk(l2),&arsize,&dv,7L,2L);
  }
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
  integer dstyle = -1;
  integer v,mn2;
  integer m1,n1,l1,m2,n2,l2,m3=1,n3=1,l3;
  double dv;
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
  if (Rhs == 3 && m3 * n3 != 1) {
    C2F(dr1)("xsegs","v",istk(l3),&one,&mn2,&v,&v,&v,stk(l1),stk(l2),&dv,&dv,6L,2L);
  } else {
    C2F(dr1)("xsegs","v",&dstyle,&zero,&mn2,&v,&v,&v,stk(l1),stk(l2),&dv,&dv,6L,2L);
  }
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
  integer ix,m1,n1,l1,v;
  double dv;

  CheckRhs(-1,1) ;
  C2F(sciwin)();

  if (Rhs == 1) 
    {
      GetRhsVar(1,"d",&m1,&n1,&l1);
      C2F(dr1)("xget","window",&verb,&cur,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
      for (ix = 0 ; ix < m1*n1 ; ++ix) 
	{
	  wid = (integer) *stk(l1 +ix );
	  C2F(dr1)("xset","window",&wid,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
	  C2F(dr1)("xclear","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
	}
      C2F(dr1)("xset","window",&cur,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
    } 
  else 
    {
      C2F(dr1)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr1)("xset","window",&win,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
      C2F(dr1)("xclear","v",&v,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,7L,2L);
    }
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
  C2F(xgrid)(&style);
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
  integer close=0,m1,n1,l1,m2,n2 ,l2,m3,n3,l3,mn1,v ;
  double dv;

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
  C2F(dr1)("xarea","xvoid",&mn1,&v,&v,&close,&v,&v,stk(l1),stk(l2),&dv,&dv,6L,5L);
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

  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,v,v1=0,v2;
  double dv;



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

    
    
  C2F(dr1)("xliness","v",&v1,&v2,istk(l3),&n2,&m2,&v,stk(l1),stk(l2),&dv,&dv,8L,2L);
  
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
  integer flagx=0,x1[10],x2, m1,n1,l1,m2,n2,l2,l3,v ;
  double dv;

  C2F(sciwin)();
  if (Rhs <= 0) {
    sci_demo(fname,"xsetm();",&zero);
    return 0;
  }

  CheckRhs(1,2);
  CheckLhs(0,1);

  GetRhsVar(1,"c",&m1,&n1,&l1);
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
  else 
    {
      int i;
      C2F(dr1)("xget",cstk(l1),&flagx,x1,&x2,&v,&v,&v,&dv,&dv,&dv,&dv,5L,bsiz);
      CreateVar(Rhs+1,"d",&one,&x2,&l3);
      for (i = 0 ; i < x2 ; ++i) *stk(l3 + i ) = (double) x1[i];      
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
 * xnumb(x,y,nums,[box,angles]) : 
 *-----------------------------------------------------------*/

int scixnumb(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,m5,n5,l5;
  integer flagx=0,v,mn3;
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
  C2F(dr1)("xnum","xv",&v,&v,&v,&v,&mn3,&flagx,stk(l1),stk(l2),stk(l3),stk(l5),5L,3L);
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
  integer m1,n1,l1 ,m2 ,n2 ,l2,m3,n3,l3,m4,n4,l4,close=0,mn2,v;
  double dv;

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
    } else if (strncmp(cstk(l3),"marks",5) == 0) {
      strcpy(C2F(cha1).buf,"xmarks");
    } else {
      Scierror(999,"%s:  dtype must be \"lines\" or \"marks\"\r\n",fname);
      return 0;
    } 
  }
  else {
    strcpy(C2F(cha1).buf,"xlines");
  }
    
  if (Rhs >= 4) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); close = (integer) *stk(l4);}
  C2F(dr1)(C2F(cha1).buf,"xv",&mn2,&v,&v,&close,&v,&v,stk(l1),stk(l2),&dv,&dv,bsiz,3L);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 *   xpolys(xpols,ypols,[draw])
 *-----------------------------------------------------------*/

int scixpolys(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer v,m1,n1,l1,m2,n2,l2,m3,n3,l3;
  double dv;

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
      int un=1,i;
      CreateVar(3,"i",&un,&n1,&l3);
      for (i = 0 ; i < n1 ; ++i) *istk(l3+i) = 1;
    }
  C2F(dr1)("xpolys","v",&v,&v,istk(l3),&n2,&m2,&v,stk(l1),stk(l2),&dv,&dv,7L,2L);
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

/*-----------------------------------------------------------
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

  if (Rhs <= 0) {int zero=0; sci_demo(fname,"xsetm();",&zero); return 0; }

  CheckRhs(1,6);
  CheckLhs(0,1);

  GetRhsVar(1,"c",&m1,&n1,&l1);
  
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

  if (strncmp(cstk(l1),"clipping",8) == 0) 
    C2F(dr1)("xset",cstk(l1),&v,&v,&v,&v,&v,&v,&xx[0],&xx[1],&xx[2],&xx[3],5L,bsiz);
  else if ( strncmp(cstk(l1),"colormap",8) == 0) 
    C2F(dr1)("xset",cstk(l1),xm,xn,&v,&v,&v,&v,stk(lr),&dv,&dv,&dv,5L,bsiz);
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
  else 
    C2F(dr1)("xset",cstk(l1),&x[0],&x[1],&x[2],&x[3],&x[4],&v,&dv,&dv,&dv,&dv,5L,bsiz);
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
  double rect[4],wc,dv,x,y,yi,angle=0.0;
  integer i,j,iv =0,v,flagx=0;
  integer m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,m5,n5,l5;
  char **Str;

  CheckRhs(3,5);
  
  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  yi = y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  if (Rhs >= 4) { GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4); angle = *stk(l4 ); }
  if (Rhs >= 5) { GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5); flagx = (integer) *stk(l5);  }

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
      C2F(dr1)("xstring",C2F(cha1).buf,&v,&v,&v,&iv,&v,&v,&x,&y,&angle,&dv,8L,bsiz);
      C2F(dr1)("xstringl",C2F(cha1).buf,&v,&v,&v,&v,&v,&v,&x,&y,rect,&dv,9L,bsiz);
      wc = Max(wc,rect[2]);
      if (i != 0 ) 
	y += rect[3] * 1.2;
      else 
	y += rect[3];
    }
  if (flagx == 1) {
    double dx1 = y - yi;
    C2F(dr1)("xrect","v",&v,&v,&v,&v,&v,&v,&x,&y,&wc,&dx1,6L,2L);
  }
  /* we must free Str2 memory */ 
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
  double dv;
  int narg;
  if (Rhs <= 0) {
    sci_demo(fname,"x=(1:10)';plot2d(x,x);xtitle(['Titre';'Principal'],'x','y');",&one);
    return 0;
  }
  CheckRhs(1,3);
  C2F(sciwin)();
  for ( narg = 1 ; narg <= Rhs ; narg++) 
    {
      int i,m,n,v;
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
      C2F(dr1)("xstringa",C2F(cha1).buf,&narg,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,9L,bsiz);
    }
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

  C2F(sciwin)();
  CheckRhs(5,6);

  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1);  x = *stk(l1);
  GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2);  y = *stk(l2);
  GetRhsVar(3,"S",&m3,&n3,&Str);
  if ( m3*n3 == 0 ) { LhsVar(1)=0; return 0;} 

  GetRhsVar(4,"d",&m4,&n4,&l4); CheckScalar(4,m4,n4);  w = *stk(l4);
  GetRhsVar(5,"d",&m5,&n5,&l5); CheckScalar(5,m5,n5);  hx = *stk(l5);

  if (Rhs == 6) {
    GetRhsVar(3,"c",&m6,&n6,&l6);
    if ( m6*n6 !=0 && strncmp(cstk(l6),"fill",4) == 0) 
      fill =1;
    else 
      {
	Scierror(999,"%s: optional argument has a wrong value 'fill' expected\r\n",
		 fname);
	return 0;
      }
  }

  ib = 0;
  for (i = 0 ; i < m3 ; ++i)
    {
      for (j = 0 ; j < n3; ++j) 
	{
	  strcpy(C2F(cha1).buf + ib,Str[i+ m3*j]);
	  ib += strlen(Str[i+ m3*j]);
	  if ( j != n3-1) { C2F(cha1).buf[ib]=' '; ib++;}
	}
      C2F(cha1).buf[ib]= '\n'; ib++;
    }

  C2F(dr1)("xstringb",C2F(cha1).buf,&fill,&v,&v,&v,&v,&v,&x,&y,&w,&hx,9L,bsiz);
  /* we must free Str2 memory */ 
  FreeRhsSVar(Str);
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
      /* 
      if (wrect != 0) sciprint("wrect = [%5.2f,%5.2f,%5.2f,%5.2f]\r\n",wrect[0],wrect[1],wrect[2],wrect[3]);
      if (arect != 0) sciprint("arect = [%5.2f,%5.2f,%5.2f,%5.2f]\r\n",arect[0],arect[1],arect[2],arect[3]);
      if (frect != 0) sciprint("frect = [%5.2f,%5.2f,%5.2f,%5.2f]\r\n",frect[0],frect[1],frect[2],frect[3]);
      if (logflag != 0) sciprint("logflag = \"%s\"\r\n",logflag);
      */
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
      if ( opts[3].position != -1 ) { 
	wrect = stk(opts[3].l);	CheckLength(opts[3].position,opts[3].m*opts[3].n,4);
      } 
      /* 
      if (wrect != 0) sciprint("wrect = [%5.2f,%5.2f,%5.2f,%5.2f]\r\n",wrect[0],wrect[1],wrect[2],wrect[3]);
      if (arect != 0) sciprint("arect = [%5.2f,%5.2f,%5.2f,%5.2f]\r\n",arect[0],arect[1],arect[2],arect[3]);
      if (frect != 0) sciprint("frect = [%5.2f,%5.2f,%5.2f,%5.2f]\r\n",frect[0],frect[1],frect[2],frect[3]);
      if (logflag != 0) sciprint("logflag = \"%s\"\r\n",logflag);
      */
    }

  C2F(sciwin)();
  C2F(Nsetscale2d)(wrect,arect,frect,logflag,0L);
  LhsVar(1) = 0;
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
  integer verb=0,win,v,na;
  double dv;
  C2F(dr)("xget","window",&verb,&win,&na,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
  C2F(dr)("xset","window",&win,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
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
    sci_demo (fname," exec(\"SCI/demos/fec/fec.ex1\");",&one);
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
  C2F(fec)(stk(l1),stk(l2),stk(l3),stk(l4),&mn1,&m3,Strf,Legend,Rect,Nax,
	   Zminmax,Colminmax,4L,bsiz);
  LhsVar(1)=0;
  return 0;
}

/*-----------------------------------------------------------
 * rep = xgetmouse([flag])
 *-----------------------------------------------------------*/

int scixgetmouse(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  integer  m1=1,n1=3,l1,button,v;
  integer iflag =  (Rhs >= 1) ? 1 : 0;
  double x,y,dv;

  CheckRhs(0,1);
  CheckLhs(1,1);

  C2F(sciwin)();
  C2F(dr1)("xgetmouse","xv",&button,&iflag,&v,&v,&v,&v,&x,&y,&dv,&dv,10L,3L);

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
  integer m1,n1,l1,m2,n2,l2,wid,v;

  CheckRhs(1,2);
  GetRhsVar(1,"c",&m1,&n1,&l1);
  if (Rhs == 2) {
    GetRhsVar(2,"d",&m2,&n2,&l2); CheckScalar(2,m2,n2); wid = (integer) *stk(l2 );
    C2F(dr)("xset","window",&wid,&v,&v,&v,&v,&v,&dv,&dv,&dv,&dv,5L,7L);
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
  GetRhsVar(1,"d",&m1,&n1,&l1); CheckScalar(1,m1,n1); iwin  = (integer) *stk(l1);
  GetRhsVar(2,"c",&m2,&n2,&l2);
  if (Rhs >= 3) {GetRhsVar(3,"d",&m3,&n3,&l3); CheckScalar(3,m3,n3); flagx = (integer) *stk(l3); }
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
      } else {
	iflag = 0;
	C2F(gsorts)(S,&v,&iflag,&m1,&n1,typex,iord);
      }
      CreateVarFromPtr(Rhs+2,"S", &m1, &n1, S);
      /* we must free Str2 memory */ 
      FreeRhsSVar(S);
      LhsVar(1)=Rhs+2;
      if ( Lhs == 2 ) LhsVar(2)=Rhs+1;
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
  CheckRhs(-1,0) ;
  C2F(getwins)(&num,&ids ,&iflag);
  CreateVar(1,"i",&un,&num,&l1);
  iflag = 1;
  C2F(getwins)(&num,istk(l1),&iflag);
  LhsVar(1)=1;
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
      val = (char **) opts[8].l;
    } 

  if ( opts[9].position != -1 ) 
    { 
      x = stk(opts[9].l);
      nx = opts[9].m * opts[9].n ;
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

  sci_axis(dir,tics,x,&nx,y,&ny,val,sub_int,format,fontsize,textcolor,ticscolor,'n',seg_flag);

  LhsVar(1) = 0;
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
	  *ntics =  *stk(yl+2)+1;break;
	case 4: 
	  *ntics =  *stk(yl+3)+1;break;
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
	  *ntics =  *stk(xl+2)+1;break;
	case 4: 
	  *ntics =  *stk(xl+3)+1;break;
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
/*  return sciplot2d1_G("plot2d1",plot2d1_,fname_len); */
  return sciplot2d1_G("plot2d1",C2F(plot2d1),fname_len);
}

int sciplot2d1_2 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d2",C2F(plot2d2),fname_len);
}

int sciplot2d1_3 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d3",C2F(plot2d3),fname_len);
}

int sciplot2d1_4 (fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return sciplot2d1_G("plot2d4",C2F(plot2d4),fname_len);
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

int scixg2ps(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"Pos",fname_len,3);
}

int scixg2fig(fname,fname_len)
     char *fname;
     unsigned long fname_len;
{
  return scixg2psofig_G(fname,"Fig",fname_len,3);
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
  {scixarcs,"xrects"},
  {sciarrows,"xarrows"},
  {scixsegs,"xsegs"},
  {nscixaxis,"drawaxis"},
  {scixchange,"xchange"},
  {scixclea,"xclea"},
  {scixclea,"xrect"},
  {scixclea,"xfrect"},
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
  {scixg2fig,"xg2fig"},
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
  {intmeta,"xmeta"},
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
  SciString(&ibegin,name,&mlhs,&mrhs);
  /* check if an error has occured while running a_function */
  LhsVar(1) = 0; 
  return 0;
}
