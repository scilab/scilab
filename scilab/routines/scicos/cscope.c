#include "scicos_block.h"
#include <math.h>
#include "../machine.h"

#if WIN32
#define NULL    0
#endif

static int c__1 = 1;
static int c__0 = 0;
static int c_n1 = -1;
static int c__4 = 4;
static int c__21 = 21;
static int c__3 = 3;
static double c_b84 = 0.;

void cscope(scicos_block *block,int flag)
{
  double t;
  double *z__;
  double *rpar;
  int *ipar, nipar,nu,kfun;
  
 
  /* Initialized data */
  
  static double frect[4] = { 0.,0.,1.,1. };
  static int cur = 0;
  static int verb = 0;
  
  int i__1,i;
  
  static double rect[4];
  extern int C2F(getlabel)(), C2F(dset)();
  static double ymin, ymax;
  static int i__, k, n, v;
  static double tsave;
  static int n1, n2;
  extern int C2F(plot2d)(), C2F(setscale2d)();
  static int na;
  extern int C2F(dr)();
  static double dt, dv;
  static int nxname;
  extern int C2F(sciwin)(), C2F(dr1)(), C2F(scicosclip)();
  static char buf[40];
  static int wid, iwd;
  static double per;
  static int nax[4], iwp;
  
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = 0/1 color flag */
  /*     ipar(3) = buffer size */
  /*     ipar(4:11) = line type for ith curve */
  
  /*     ipar(12:13) : window position */
  /*     ipar(14:15) : window dimension */
  
  /*     rpar(1)=dt */
  /*     rpar(2)=ymin */
  /*     rpar(3)=ymax */
  /*     rpar(4)=periode */
  
   nu=block->insz[0];
  if (nu>8) {nu=8;}
  rpar=block->rpar;
  ipar=block->ipar;
  nipar=block->nipar;
  t=get_scicos_time();
  
  /*      character*(4) logf */
  /* Parameter adjustments */
  --ipar;
  --rpar;
  
  iwp = nipar - 3;
  iwd = nipar - 1;

  wid = ipar[1];
  if(wid==-1){
    wid=20000+get_block_number();
  }
  
  if (flag == 2) {
    z__=*block->work; 
    --z__;
    dt = rpar[1];
    ymin = rpar[2];
    ymax = rpar[3];
    per = rpar[4];
    n = ipar[3];
    k = (int) z__[1];
    if (k > 0) {
      n1 = (int) (z__[k + 1] / per);
      if (z__[k + 1] < 0.) {
	--n1;
      }
    } else {
      n1 = 0;
    }
    
    tsave = t;
    if (dt > 0.) {
      t = z__[k + 1] + dt;
    }
    
    n2 = (int) (t / per);
    if (t < 0.) {
      --n2;
    }
    
    /*     add new point to the buffer */
    ++k;
    z__[k + 1] = t;
    for (i = 0; i < nu; ++i) {
      z__[n + 1 + i * n + k] = block->inptr[0][i];
      /* L1: */
    }
    z__[1] = (double) k;
    if (n1 == n2 && k < n) {
      t = tsave;
      return ;
    }
    
    /*     plot 1:K points of the buffer */
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(dr1)("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    if (k > 0) {
      C2F(scicosclip)(&c__1);
      for (i__ = 0; i__ < nu; ++i__) {
	C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[i__ + 4], &c__1, &k,
		 &v, &z__[2], &z__[n + 2 + i__  * n], &dv, &dv);
      }
      C2F(scicosclip)(&c__0);
    }
    /*     shift buffer left */
    z__[2] = z__[k + 1];
    for (i__ = 0; i__ < nu; ++i__) {
      z__[n + 1 + i__ * n + 1] = z__[n + 1 + i__ * n + k];
    }
    z__[1] = 1.;
	if (n1 != n2) {
	  /*     clear window */
	  nax[0] = 2;
	  nax[1] = 10;
	    nax[2] = 2;
	    nax[3] = 10;
	    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		     &dv);
	    C2F(dr1)("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0, &
		     c__0, &v, &dv, &dv, &dv, &dv);
	    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &
		    dv, &dv);
	    rect[0] = per * (n1 + 1);
	    rect[1] = ymin;
	    rect[2] = per * (n1 + 2);
	    rect[3] = ymax;
	    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, 
		     &v, &dv, &dv, &dv, &dv);
	    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, "011", buf, rect, nax,
			&c__4, &c__21);
	}
	t = tsave;
	
  } else if (flag == 4) {/* the workspace is used to store buffer 
			     */
    if ((*block->work=
	 scicos_malloc(sizeof(double)*(1+ipar[3]*(1+nu))))== NULL ) {
      set_block_error(-16);
      return;
    }
    z__=*block->work; 
    --z__;
    z__[1]=-1.0;


    n = ipar[3];
    ymin = rpar[2];
    ymax = rpar[3];
    per = rpar[4];
    nax[0] = 2;
    nax[1] = 10;
    nax[2] = 2;
    nax[3] = 10;
    n1 = (int) (t / per);
    if (t <= 0.) {
      --n1;
    }
    C2F(sciwin)();
    C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &
	     dv, &dv);
    C2F(dr1)("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    if (ipar[iwp] >= 0) {
	    C2F(dr1)("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v, &v, &
		     v, &v, &dv, &dv, &dv, &dv);
    }
    if (ipar[iwd] >= 0) {
      C2F(dr1)("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
      /*     to force dimensions update */
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    rect[0] = per * (n1 + 1);
    rect[1] = ymin;
    rect[2] = per * (n1 + 2);
    rect[3] = ymax;
    C2F(setscale2d)(frect, rect, "nn\000");
    C2F(dr1)("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0, &
	     c__0, &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0, &c__0,
	     &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	    dv);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, "011", buf, rect, nax, &
		c__4, &c__21);
    C2F(scicosclip)(&c__1);
    nxname = 40;
    kfun=get_block_number();
    C2F(getlabel)(&kfun, buf, &nxname);
    if (nxname > 39) {
      nxname = 39;
    }
    i__1 = nxname;
    *(buf+i__1)=*"\000";
    if ((nxname == 1 && *(unsigned char *)buf == ' ') || (nxname == 0)) {
    } else {
      C2F(dr)("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv);
    }
    z__[1] = 0.;
    z__[2] = t;
    i__1 = nu * n;
    C2F(dset)(&i__1, &c_b84, &z__[3], &c__1);
  } else if (flag == 5) {
    z__=*block->work; 
    --z__;
    n = ipar[3];
    k = (int) z__[1];
    if (k <= 1) {
      scicos_free(*block->work);
      return ;
    }
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    C2F(scicosclip)(&c__1);
    for (i__ = 0; i__ < nu; ++i__) {
      C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[i__ + 4], &c__1, &k, &v,
	       &z__[2], &z__[n + 2 + i__ * n], &dv, &dv);
    }
    C2F(scicosclip)(&c__0);
    scicos_free(*block->work);
  }
}
