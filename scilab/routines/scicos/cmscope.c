#include "scicos_block.h"
#include <math.h>
#include "../machine.h"

#if WIN32
#define NULL    0
#endif

static int c__1 = 1;
static int c__0 = 0;
static int c_n1 = -1;
static int c__3 = 3;
static double c_b103 = 0.;

void cmscope(scicos_block *block,int flag)
{
  double t;
  double *z__;
  double *rpar;
  int *ipar, nipar,nu;
  


  static int cur = 0;
  static int verb = 0;
  
  int i__1, i__2, i__3,nwid,kk,i,j,sum;
  
  int kfun;  
  
  static double rect[4];
  static int kwid;
  extern int C2F(getlabel)();
  extern int C2F(dset)();
  static int i__, k, n, v;
  static double frect[4], tsave;
  static int n1, n2;
  extern int C2F(plot2d)(), C2F(setscale2d)();
  static int na;
  extern int C2F(dr)();
  static double dt, dv;
  static int it, nxname;
  extern int C2F(sciwin)(), C2F(dr1)(), C2F(scicosclip)();
  static char buf[40];
  static int wid, iwd;
  static double per;
  static int nax[4], ilt, iwp;

  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = number of subwindows (input ports) */
  /*     ipar(3) = buffer size */
  /*     ipar(4:5) : window position */
  /*     ipar(6:7) : window dimension */
  /*     ipar(8:7+ipar(2)) = input port sizes */
  /*     ipar(8+ipar(2):7+ipar(2)+nu) = line type for ith curve */
  /*     rpar(1)=dt */
  /*     rpar(2)=periode */
  /*     rpar(3)=ymin_1 */
  /*     rpar(4)=ymax_1 */
  /*     ... */
  /*     rpar(2*k+1)=ymin_k */
  /*     rpar(2*k+2)=ymax_k */

  nu=block->insz[0];
  rpar=block->rpar;
  ipar=block->ipar;
  nipar=block->nipar;
  t=get_scicos_time();

  --ipar;
  --rpar;

  wid = ipar[1];
  if(wid==-1){
    wid=20000+get_block_number();
  }
  nwid = ipar[2];
  n = ipar[3];
  per = rpar[2];
  dt = rpar[1];

  if (flag == 2) {
    z__=*block->work; 
    --z__;
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
    kk=0;
    for (i=0;i<block->nin;++i){
      for (j = 0; j <block->insz[i] ; ++j) {
	z__[n + 1 + kk * n + k] =block->inptr[i][j] ;
	++kk;
      }
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
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    C2F(dr1)("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    ilt = ipar[2] + 8;
    it = 0;
    /*     loop on input ports */
    if (k > 0) {
      i__1 = nwid;
      for (kwid = 1; kwid <= i__1; ++kwid) {
	rect[0] = per * n1;
	rect[1] = rpar[(kwid << 1) + 1];
	rect[2] = per * (n1 + 1);
	rect[3] = rpar[(kwid << 1) + 2];
	frect[0] = 0.;
	frect[1] = (kwid - 1) * (1. / nwid);
	frect[2] = 1.;
	frect[3] = 1. / nwid;
	C2F(setscale2d)(frect, rect, "nn\000");
	C2F(scicosclip)(&c__1);
	/*     loop on input port elements */
	i__2 = ipar[kwid + 7];
	for (i__ = 1; i__ <= i__2; ++i__) {
	  C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[ilt + it], &
		   c__1, &k, &v, &z__[2], &z__[n + 2 + it * n], &dv, 
		   &dv);
	  ++it;
	}
	C2F(scicosclip)(&c__0);
      }
    }
    /*     shift buffer left */
    z__[2] = z__[k + 1];
    sum=0;
    for (i=0;i<block->nin;++i){
      sum=sum+block->insz[i];
    }    i__1 = sum;
    for (i__ = 1; i__ <= i__1; ++i__) {
      z__[n + 1 + (i__ - 1) * n + 1] = z__[n + 1 + (i__ - 1) * n + k];
    }
    z__[1] = 1.;
    if (n1 != n2) {
      /*     clear window */
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 5;
      nax[3] = 4;
      C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	       &dv);
      C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &
	       c__0, &v, &dv, &dv, &dv, &dv);
      C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &
	      dv, &dv);
      C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, 
	       &v, &dv, &dv, &dv, &dv);
      i__1 = nwid;
      for (kwid = 1; kwid <= i__1; ++kwid) {
	rect[0] = per * (n1 + 1);
	rect[1] = rpar[(kwid << 1) + 1];
	rect[2] = per * (n1 + 2);
	rect[3] = rpar[(kwid << 1) + 2];
	frect[0] = 0.;
	frect[1] = (kwid - 1) * (1. / nwid);
	frect[2] = 1.;
	frect[3] = 1. / nwid;
	C2F(setscale2d)(frect, rect, "nn\000");
	C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, "011", "xlines", rect, 
		    nax);
      }
    }
    t = tsave;

  } else if (flag == 4) {
    sum=0;
    for (i=0;i<block->nin;++i){
      sum=sum+block->insz[i];
    }
    if ((*block->work=
	 scicos_malloc(sizeof(double)*(1+ipar[3]*(1+sum))))== NULL ) {
      set_block_error(-16);
      return;
    }
    z__=*block->work; 
    --z__;
    z__[1]=-1.0;
    nax[0] = 2;
    nax[1] = 10;
    nax[2] = 5;
    nax[3] = 4;
    n1 = (int) (t / per);
    if (t <= 0.) {
      --n1;
    }
    C2F(sciwin)();
    C2F(dr1)("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv, &dv,
	     &dv, &dv);
    if (cur != wid) {
      C2F(dr1)("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv);
    }
    iwp = 4;
    if (ipar[iwp] >= 0) {
      C2F(dr1)("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
    }
    iwd = 6;
    if (ipar[iwd] >= 0) {
      C2F(dr1)("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v, &v, &
	       v, &v, &dv, &dv, &dv, &dv);
    }
    C2F(dr1)("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0, &c__0,
	     &v, &dv, &dv, &dv, &dv);
    C2F(dr1)("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	     dv);
    C2F(dr)("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv, &dv, &
	    dv);
    C2F(dr1)("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0, &v, 
	     &dv, &dv, &dv, &dv);
    nxname = 40;
    kfun=get_block_number();
    C2F(getlabel)(&kfun, buf, &nxname);
    if (nxname > 39) {
      nxname = 39;
    }
    i__1 = nxname;
    *(buf+i__1)=*"\000";
    if ((nxname == 1 && *(unsigned char *)buf == ' ') || nxname == 0) {
    } else {
      C2F(dr)("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv, &dv);
    }
    i__1 = nwid;
    for (kwid = 1; kwid <= i__1; ++kwid) {
      rect[0] = per * (n1 + 1);
      rect[1] = rpar[(kwid << 1) + 1];
      rect[2] = per * (n1 + 2);
      rect[3] = rpar[(kwid << 1) + 2];
      frect[0] = 0.;
      frect[1] = (kwid - 1) * (1. / nwid);
      frect[2] = 1.;
      frect[3] = 1. / nwid;
      C2F(setscale2d)(frect, rect, "nn\000");
      C2F(plot2d)(rect, &rect[1], &c__1, &c__1, &c_n1, "011", buf, rect, nax);
    }
    
    z__[1] = 0.;
    z__[2] = t;
    i__1 = sum * n;
    C2F(dset)(&i__1, &c_b103, &z__[3], &c__1);
  } else if (flag == 5) {
    z__=*block->work; 
    --z__;
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
    C2F(dr1)("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0, &c__0, &
	     v, &dv, &dv, &dv, &dv);
    
    ilt = ipar[2] + 8;
    it = 0;
    n1 = (int) (z__[2] / per);
    /*     loop on input ports */
    i__1 = nwid;
    for (kwid = 1; kwid <= i__1; ++kwid) {
      rect[0] = per * (n1 + 0);
      rect[1] = rpar[(kwid << 1) + 1];
      rect[2] = per * (n1 + 1);
      rect[3] = rpar[(kwid << 1) + 2];
      frect[0] = 0.;
      frect[1] = (kwid - 1) * (1. / nwid);
      frect[2] = 1.;
      frect[3] = 1. / nwid;
      F2C(setscale2d)(frect, rect, "nn\000");
      F2C(scicosclip)(&c__1);
      /*     loop on input port elements */
      i__2 = ipar[kwid + 7];
      for (i__ = 1; i__ <= i__2; ++i__) {
	i__3 = k - 1;
	C2F(dr1)("xpolys\000", "v\000", &v, &v, &ipar[ilt + it], &c__1, &
		 i__3, &v, &z__[2], &z__[n + 2 + it * n], &dv, &dv);
	++it;
      }
      C2F(scicosclip)(&c__0);
    }
  scicos_free(*block->work);
  }
} 
