#include "scicos_block.h"
#include <math.h>
#include "../machine.h"
static int c__1 = 1;
static int c__0 = 0;
static int c_n1 = -1;
static int c__3 = 3;
static integer c__5 = 5;
static integer c__6 = 6;

void bouncexy(scicos_block *block,int flag)
{
  int nevprt=block->nevprt;
  double t;
  double *z__;
  int nz;
  double *rpar;
  int nrpar, *ipar, nipar;
  double *u,*y;
  int nu,kfun;

 
  static int cur = 0;
  static int verb = 0;
  static int c400=400;
  int i__1;
  
  static double rect[4];
  extern /* Subroutine */ int C2F(getlabel)();
  static double xmin, ymin, xmax, ymax;
  extern /* Subroutine */ int C2F(sxevents)();
  static int i__, n, v;
  extern /* Subroutine */ int C2F(plot2d)();
  static int na;
  extern /* Subroutine */ int C2F(dr)();
  static double dv;
  static int on;
  static double zz[10];
  static int nxname;
  extern /* Subroutine */ int C2F(sciwin)(), C2F(dr1)();
  static char buf[40];
  static int wid, nax[4];
  
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = mode : animated =0 fixed=1 */
  /*     ipar(3:3+nu-1) = colors of balls */
  
  /*     rpar(1)=xmin */
  /*     rpar(2)=xmax */
  /*     rpar(3)=ymin */
  /*     rpar(4)=ymax */
  nevprt=block->nevprt;
  nz=block->nz;
  nrpar=block->nrpar; 
  nipar=block->nipar;
  nu=block->insz[0];
  z__=block->z;
  rpar=block->rpar;
  ipar=block->ipar;
  u=block->inptr[0];
  y=block->inptr[1];
  t=get_scicos_time();

  
  /* Parameter adjustments */
  --u;
  --y;
  --ipar;
  --rpar;
  --z__;
  
  /* Function Body */
  
  if (flag == 2) {
    wid = ipar[1];
    n = nu;
    
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(dr1)("xsetdr\000", "X11\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    rect[0] = xmin;
    rect[1] = ymin;
    rect[2] = xmax;
    rect[3] = ymax;
    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, "030", buf, rect, nax);
    /*     draw new point */
    i__1 = nu;
    for (i__ = 1; i__ <= i__1; ++i__) {
      z__[(i__ - 1) * 6 + 1] = u[i__] - z__[(i__ - 1) * 6 + 3] / 2;
      z__[(i__ - 1) * 6 + 2] = y[i__] + z__[(i__ - 1) * 6 + 4] / 2;
    }
    C2F(dr1)("xset\000", "wwpc\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr1)("xfarcs\000", "v\000", &v, &ipar[3], &n, &v, &v, &v, &z__[1], &
	     dv, &dv, &dv);
    xmin = rpar[1];
    xmax = rpar[2];
    ymin = rpar[3];
    ymax = rpar[4];
    zz[0] = xmin;
    zz[1] = xmin;
    zz[2] = xmax;
    zz[3] = xmax;
    zz[4] = xmin;
    zz[5] = ymax;
    zz[6] = ymin;
    zz[7] = ymin;
    zz[8] = ymax;
    zz[9] = ymax;
    C2F(dr1)("xpolys\000", "v\000", &v, &v, &c__1, &c__1, &c__5, &v, zz, &zz[5], &dv, &dv);
    C2F(dr1)("xset\000", "wshow\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
  } else if (flag == 4) {
    wid = ipar[1];
    n = nu;
    xmin = rpar[1];
    xmax = rpar[2];
    ymin = rpar[3];
    ymax = rpar[4];
    nax[0] = 2;
    nax[1] = 10;
    nax[2] = 2;
    nax[3] = 10;
    C2F(sciwin)();

    C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    
    C2F(dr1)("xset\000", "wdim\000", &c400, &c400, &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
  C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);

    C2F(dr1)("xsetdr\000", "X11\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    on = 1;
    C2F(dr1)("xset\000", "pixmap\000", &on, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	     &dv);
    rect[0] = xmin;
    rect[1] = ymin;
    rect[2] = xmax;
    rect[3] = ymax;
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0, &c__0,
	     &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	    dv);
    C2F(dr1)("xset\000", "thickness\000", &c__1, &v, &v, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, "030", buf, rect, nax);

    zz[0] = xmin;
    zz[1] = xmin;
    zz[2] = xmax;
    zz[3] = xmax;
    zz[4] = xmin;
    zz[5] = ymax;
    zz[6] = ymin;
    zz[7] = ymin;
    zz[8] = ymax;
    zz[9] = ymax;
    C2F(dr1)("xpolys\000", "v\000", &v, &v, &c__1, &c__1, &c__5, &v, zz, &zz[5], &dv, &dv);
    C2F(dr1)("xset\000", "wshow\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    nxname = 40;
    kfun=get_block_number();
    C2F(getlabel)(&kfun, buf, &nxname);
    if (nxname > 39) {
      nxname = 39;
    }
    i__1 = nxname;
    *(buf+i__1)=*"\000";
    if (nxname == 1 && *(unsigned char *)buf == ' ' || nxname == 0) {
    } else {
      C2F(dr)("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv);
    }
    C2F(sxevents)();
  }
  C2F(dr1)("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
} 
