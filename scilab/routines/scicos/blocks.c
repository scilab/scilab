#include <stdio.h>
#include <math.h>
#include "nsp/machine.h"
#include "nsp/math.h" 
#include "nsp/graphics/Graphics.h" 
#include "scicos.h"

struct
{
  int halt;
} C2F(coshlt);


struct
{
  double atol, rtol, ttol, deltat;
} C2F(costol);


struct
{
  int kfun;
} C2F(curblk);

#define scicos_args_poo int *flag__, int *nevprt, double *t, double *xd, double *x, \
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,\
	       double *rpar, int *nrpar, int *ipar, int *nipar, double *u,\
	       int *nu, double *y, int *ny 

#if 0 
#define blocks_args int *flag__, int *nevprt, double *t, double *xd, double *x,\
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,\
	       double *rpar, int *nrpar, int *ipar, int *nipar, double *u,\
	       int *nu, double *y, int *ny

extern int scicos_plusblk(blocks_args);
extern int scicos_evtdly(blocks_args);
extern int scicos_cstblk(blocks_args);
extern int scicos_lusat(blocks_args);
extern int scicos_pload(blocks_args);
extern int scicos_qzcel(blocks_args);
extern int scicos_qzflr(blocks_args);
extern int scicos_qzrnd(blocks_args);
extern int scicos_qztrn(blocks_args);
extern int scicos_scope(blocks_args);
extern int scicos_lsplit(blocks_args);
extern int scicos_csslti(blocks_args);
extern int scicos_dsslti(blocks_args);
extern int scicos_trash(blocks_args);
extern int scicos_zcross(blocks_args);
extern int scicos_expblk(blocks_args);
extern int scicos_logblk(blocks_args);
extern int scicos_sinblk(blocks_args);
extern int scicos_tanblk(blocks_args);
extern int scicos_powblk(blocks_args);
extern int scicos_sqrblk(blocks_args);
extern int scicos_delay(blocks_args);
extern int scicos_selblk(blocks_args);
extern int scicos_forblk(blocks_args);
extern int scicos_writef(blocks_args);
extern int scicos_invblk(blocks_args);
extern int scicos_hltblk(blocks_args);
extern int scicos_gensin(blocks_args);
extern int scicos_rndblk(blocks_args);
extern int scicos_lookup(blocks_args);
extern int scicos_timblk(blocks_args);
extern int scicos_gensqr(blocks_args);
extern int scicos_mfclck(blocks_args);
extern int scicos_sawtth(blocks_args);
extern int scicos_tcslti(blocks_args);
extern int scicos_tcsltj(blocks_args);
extern int scicos_scopxy(blocks_args);
extern int scicos_evscpe(blocks_args);
extern int scicos_integr(blocks_args);
extern int scicos_readf(blocks_args);
extern int scicos_affich(blocks_args);
extern int scicos_intpol(blocks_args);
extern int scicos_intplt(blocks_args);
extern int scicos_minblk(blocks_args);
extern int scicos_maxblk(blocks_args);
extern int scicos_dlradp(blocks_args);
extern int scicos_iocopy(blocks_args);
extern int scicos_sum2(blocks_args);
extern int scicos_sum3(blocks_args);
extern int scicos_delayv(blocks_args);
extern int scicos_mux(blocks_args);
extern int scicos_demux(blocks_args);
extern int scicos_samphold(blocks_args);
extern int scicos_dollar(blocks_args);
extern int scicos_mscope(blocks_args);
extern int scicos_intrp2(blocks_args);
extern int scicos_intrpl(blocks_args);
extern int scicos_fsv(blocks_args);
extern int scicos_memo(blocks_args);
extern int scicos_fscope(blocks_args);
extern int scicos_scoxy(blocks_args);
extern int scicos_diffblk(blocks_args);
extern int scicos_constraint(blocks_args);
extern int scicos_absblk(blocks_args);
extern int scicos_andlog(blocks_args);
extern int scicos_bidon(blocks_args);
extern int scicos_gain(blocks_args);
extern int scicos_cdummy(blocks_args);
extern int scicos_dband(blocks_args);
extern int scicos_cosblk(blocks_args);
extern int scicos_ifthel(blocks_args);
extern int scicos_eselect(blocks_args);
#endif 


int scicos_csslti (scicos_args_poo)
{
  int la, lb, lc, ld;
  static int c__1 = 1;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     continuous state space linear system simulator */
  /*     rpar(1:nx*nx)=A */
  /*     rpar(nx*nx+1:nx*nx+nx*nu)=B */
  /*     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C */
  /*     rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  la = 1;
  lb = *nx * *nx + la;
  lc = lb + *nx * *nu;
  if (*flag__ == 1 || *flag__ == 6)
    {
      /*     y=c*x+d*u */
      ld = lc + *nx * *ny;
      scicos_dmmul (&rpar[lc], ny, &x[1], nx, &y[1], ny, ny, nx, &c__1);
      dmmul1_fort(&rpar[ld], ny, &u[1], nu, &y[1], ny, ny, nu, &c__1);
      /*         if(t.gt.64.0) write(6,'(e15.8,10(e10.3,x))') t,x(1),x(2), */
      /*     $        u(1),y(1) */
    }
  else if (*flag__ == 0)
    {
      /*     xd=a*x+b*u */
      scicos_dmmul (&rpar[la], nx, &x[1], nx, &xd[1], nx, nx, nx, &c__1);
      dmmul1_fort(&rpar[lb], nx, &u[1], nu, &xd[1], nx, nx, nu, &c__1);
      /*         if(t.gt.64.0) write(6,'(e15.8,10(e10.3,x))') t,x(1),x(2), */
      /*     $        xd(1),xd(2),u(1) */
    }
  return 0;
}


int scicos_delay (scicos_args_poo) 
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Ouputs nx*dt delayed input */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1 || *flag__ == 4 || *flag__ == 6)
    {
      y[1] = z__[1];
    }
  else if (*flag__ == 2)
    {
      /*     .  shift buffer */
      i__1 = *nz - 1;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[i__] = z__[i__ + 1];
	  /* L10: */
	}
      /*     .  add new point to the buffer */
      z__[*nz] = u[1];
    }
  return 0;
}			

int
scicos_cstblk (int *flag__, int *nevprt, double *t, double *xd, double *x,
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	       double *rpar, int *nrpar, int *ipar, int *nipar, double *y,
	       int *ny)
{
  static int c__1 = 1;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     output a vector of constants out(i)=rpar(i) */
  /*     rpar(1:nrpar) : given constants */
  scicos_dcopy (nrpar, rpar, &c__1, y, &c__1);
  return 0;
}			


int
scicos_constraint (int *flag__, int *nevprt, double *t, double *res,
		   double *xd, double *x, int *nx, double *z__, int *nz,
		   double *tvec, int *ntvec, double *rpar, int *nrpar,
		   int *ipar, int *nipar, double *u, int *nu, double *y,
		   int *ny)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  --res;
  if (*flag__ == 0)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  res[i__] = xd[i__] - u[i__];
	  res[i__ + *nu] = xd[i__];
	  /* L12: */
	}
    }
  else if (*flag__ == 1)
    {
      i__1 = *ny;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = xd[i__ + *nu];
	  /* L14: */
	}
      /*      elseif(flag.eq.6.or.flag.eq.7) then */
      /*         do 12 i=1,nu */
      /* 12      continue */
    }
  return 0;
}




int
scicos_delayv (int *flag__, int *nevprt, double *t, double *xd, double *x,
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	       double *rpar, int *nrpar, int *ipar, int *nipar, double *u1,
	       int *nu1, double *u2, int *nu2, double *y, int *ny)
{

  int i__1, i__2;
  double dtat, a;
  int i__, j, k, ii, in;
  double u2r;

  /*     Copyright INRIA */
  /*     rpar(1)=dt */
  /*     delayv=u(nin) */

  --y;
  --u2;
  --u1;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  j = (*nz - 1) / *nu1;
  if (*flag__ == 3)
    {
      tvec[1] = *t + rpar[1];
      k = (int) (u2[1] / rpar[1]);
      if (k > j - 3)
	{
	  tvec[2] = *t;
	}
      if (k < 1)
	{
	  tvec[2] = *t;
	}
    }

  /*     .   shift buffer */
  if (*flag__ == 2)
    {
      i__1 = j;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[i__] = z__[i__ + 1];
	  /* L10: */
	}
      i__1 = *nu1 - 1;
      for (in = 1; in <= i__1; ++in)
	{
	  i__2 = (in + 1) * j;
	  for (ii = in * j + 1; ii <= i__2; ++ii)
	    {
	      z__[ii] = z__[ii + 1];
	      /* L35: */
	    }
	  /* L30: */
	}
      z__[*nz] = *t;
      i__1 = *nu1;
      for (in = 1; in <= i__1; ++in)
	{
	  z__[j * in] = u1[in];
	  /* L20: */
	}
    }
  if (*flag__ == 1 || *flag__ == 6)
    {
      dtat = *t - z__[*nz];
      i__1 = *nu1;
      for (in = 1; in <= i__1; ++in)
	{
	  /*     extrapolate to find values at delta.t */
	  if (u2[1] <= dtat)
	    {
	      /*     initialisation start */
	      if (dtat < rpar[1] / 100.)
		{
		  a = u2[1] / (rpar[1] + dtat);
		  /*     delete negative delay */
		  if (a <= 0.)
		    {
		      a = 0.;
		    }
		  y[in] = (1 - a) * z__[j * in] + a * z__[j * in - 1];
		}
	      else
		{
		  a = u2[1] / dtat;
		  /*     delete negative delay */
		  if (a <= 0.)
		    {
		      a = 0.;
		    }
		  y[in] = (1 - a) * u1[in] + a * z__[j * in];
		}
	    }
	  else
	    {
	      u2r = u2[1] - dtat;
	      k = (int) (u2r / rpar[1]);
	      /*     limitation of size buffer */
	      if (k > j - 3)
		{
		  k = j - 3;
		  a = 1.;
		}
	      else
		{
		  a = (u2r - k * rpar[1]) / rpar[1];
		}
	      /*     interpolate to find values at t-delay */
	      y[in] = (1 - a) * z__[j * in - k] + a * z__[j * in - k - 1];
	    }
	  /* L8: */
	}
    }
  return 0;
}


int
scicos_demux (int *flag__, int *nevprt, double *t, double *xd, double *x,
	      int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	      double *rpar, int *nrpar, int *ipar, int *nipar, double *uy1,
	      int *nuy1, double *uy2, int *nuy2, double *uy3, int *nuy3,
	      double *uy4, int *nuy4, double *uy5, int *nuy5, double *uy6,
	      int *nuy6, double *uy7, int *nuy7, double *uy8, int *nuy8,
	      double *uy9, int *nuy9)
{

  int i__1;
  int i__, k;
  /*     Copyright INRIA */
  /*     Scicos block simulator */

  --uy9;
  --uy8;
  --uy7;
  --uy6;
  --uy5;
  --uy4;
  --uy3;
  --uy2;
  --uy1;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  k = 0;
  switch (ipar[1] - 1)
    {
    case 1:
      goto L20;
    case 2:
      goto L30;
    case 3:
      goto L40;
    case 4:
      goto L50;
    case 5:
      goto L60;
    case 6:
      goto L70;
    case 7:
      goto L80;
    }

 L20:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L25: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L27: */
    }
  return 0;

 L30:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L35: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L37: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[i__] = uy1[k];
      /* L38: */
    }
  return 0;

 L40:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L41: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L42: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[i__] = uy1[k];
      /* L43: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[i__] = uy1[k];
      /* L44: */
    }
  return 0;

 L50:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L51: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L52: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[i__] = uy1[k];
      /* L53: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[i__] = uy1[k];
      /* L54: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[i__] = uy1[k];
      /* L55: */
    }
  return 0;

 L60:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L61: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L62: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[i__] = uy1[k];
      /* L63: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[i__] = uy1[k];
      /* L64: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[i__] = uy1[k];
      /* L65: */
    }
  i__1 = *nuy7;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[i__] = uy1[k];
      /* L66: */
    }
  return 0;

 L70:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L71: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L72: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[i__] = uy1[k];
      /* L73: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[i__] = uy1[k];
      /* L74: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[i__] = uy1[k];
      /* L75: */
    }
  i__1 = *nuy7;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[i__] = uy1[k];
      /* L76: */
    }
  i__1 = *nuy8;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[i__] = uy1[k];
      /* L77: */
    }
  return 0;

 L80:
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy2[i__] = uy1[k];
      /* L81: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[i__] = uy1[k];
      /* L82: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[i__] = uy1[k];
      /* L83: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[i__] = uy1[k];
      /* L84: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[i__] = uy1[k];
      /* L85: */
    }
  i__1 = *nuy7;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[i__] = uy1[k];
      /* L86: */
    }
  i__1 = *nuy8;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[i__] = uy1[k];
      /* L87: */
    }
  i__1 = *nuy9;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[i__] = uy1[k];
      /* L88: */
    }
  return 0;
}			





int
scicos_diffblk (int *flag__, int *nevprt, double *t, double *res, double *xd,
		double *x, int *nx, double *z__, int *nz, double *tvec,
		int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar,
		double *u, int *nu, double *y, int *ny)
{

  int i__1;
  int i__;

  /*     Copyright INRIA */
  /*     Scicos block simulator */



  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  --res;
  if (*flag__ == 0)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  res[i__] = x[i__] - u[i__];
	  /* L10: */
	}
    }
  else if (*flag__ == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = xd[i__];
	  /* L11: */
	}
    }
  else if (*flag__ == 6 || *flag__ == 7)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  x[i__] = u[i__];
	  /* L12: */
	}
    }
  return 0;
}			

/* Table of constant values */


int
scicos_dlradp (scicos_args_poo)
{
  static int c__1 = 1;
  static int c_n1 = -1;

  int i__1;  int m, n, iflag;
  double ww[51], yy[201], den[51];
  int mpn;
  double num[51];
  int npt;
  double yyp;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     SISO, strictly proper adapted transfer function */

  /*     u(1)    : main input */
  /*     u(2)    : modes adaptation input */

  /*     m = ipar(1) : degree of numerator */
  /*     n = ipar(2) : degree of denominator n>m */
  /*     npt = ipar(3) : number of mesh points */
  /*     x = rpar(1:npt) : mesh points abscissae */
  /*     rnr = rpar(npt+1:npt+m*npt) : rnr(i,k) i=1:m  is the real part of */
  /*          the roots of the numerator at the kth mesh point */
  /*     rni = rpar(npt+m*npt+1:npt+2*m*npt) : rni(i,k) i=1:m  is the */
  /*          imaginary part of the roots of the numerator at the kth */
  /*          mesh point */
  /*     rdr = rpar(npt+2*m*np+1:npt+(2*m+n)*npt) : rdr(i,k) i=1:n */
  /*          is the real part of the roots of the denominator at the kth */
  /*          meshpoint */
  /*     rdi = rpar(npt+(2*m+n)*np+1:npt+2*(m+n)*npt) : rdi(i,k) i=1:n */
  /*          is the imaginary part of the roots of the denominator at */
  /*          the kth  meshpoint */
  /*     g   = rpar(npt+2*(m+n)*npt+1:npt+2*(m+n)*npt+npt) is the */
  /*           gain values at the mesh points. */
  /* ! */

  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  m = ipar[1];
  n = ipar[2];
  if (*flag__ == 2)
    {
      /*     state */
      m = ipar[1];
      n = ipar[2];
      mpn = m + n;
      npt = ipar[3];
      i__1 = (mpn << 1) + 1;
      scicos_intp (&u[2], &rpar[1], &rpar[npt + 1], &i__1, &npt, yy);
      scicos_wprxc (&m, yy, &yy[m], num, ww);
      scicos_wprxc (&n, &yy[m * 2], &yy[(m << 1) + 1 + n - 1], den, ww);
      yyp =
	-scicos_ddot (&n, den, &c__1, &z__[m + 1],
		      &c__1) + (scicos_ddot (&m, num, &c__1, &z__[1],
					     &c__1) + u[1]) * yy[mpn * 2];
      if (m > 0)
	{
	  i__1 = m - 1;
	  scicos_unsfdcopy (&i__1, &z__[2], &c_n1, &z__[1], &c_n1);
	  z__[m] = u[1];
	}
      i__1 = n - 1;
      scicos_unsfdcopy (&i__1, &z__[m + 2], &c_n1, &z__[m + 1], &c_n1);
      z__[mpn] = yyp;
    }
  else if (*flag__ == 4)
    {
      /*     init */
      m = ipar[1];
      n = ipar[2];
      if (m > 50 || n > 50)
	{
	  iflag = -1;
	  return 0;
	}
    }
  /*     y */
  y[1] = z__[m + n];
  return 0;
}			


int scicos_dollar (scicos_args_poo)
{

  int i__1;
  int j;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Ouputs delayed input */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (j = 1; j <= i__1; ++j)
    {
      if (*flag__ == 1 || *flag__ == 6 || *flag__ == 4)
	{
	  y[j] = z__[j];
	}
      else if (*flag__ == 2)
	{
	  z__[j] = u[j];
	}
      /* L5: */
    }
  return 0;
}			



int scicos_dsslti (scicos_args_poo)
{
  const  int c__1 = 1;
  double w[100];
  int la, lb, lc, ld;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     discrete state space linear system simulator */
  /*     rpar(1:nx*nx)=A */
  /*     rpar(nx*nx+1:nx*nx+nx*nu)=B */
  /*     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  la = 1;
  lb = *nz * *nz + la;
  lc = lb + *nz * *nu;
  if (*flag__ == 4)
    {
      if (*nz > 100)
	{
	  *flag__ = -1;
	  return 0;
	}
    }
  else if (*flag__ == 2)
    {
      /*     x+=a*x+b*u */
      scicos_dcopy (nz, &z__[1], &c__1, w, &c__1);
      scicos_dmmul (&rpar[la], nz, w, nz, &z__[1], nz, nz, nz, &c__1);
      dmmul1_fort(&rpar[lb], nz, &u[1], nu, &z__[1], nz, nz, nu, &c__1);
    }
  else if (*flag__ == 1 || *flag__ == 6)
    {
      /*     y=c*x+d*u */
      ld = lc + *nz * *ny;
      scicos_dmmul (&rpar[lc], ny, &z__[1], nz, &y[1], ny, ny, nz, &c__1);
      dmmul1_fort(&rpar[ld], ny, &u[1], nu, &y[1], ny, ny, nu, &c__1);
    }
  return 0;
}			




int scicos_evscpe (scicos_args_poo)
{
  static int c__0 = 0;
  static int c__1 = 1;
  static int c_n1 = -1;
  static int c__2 = 2;
  static int c__3 = 3;
  /* Initialized data */
  static double frect[4] = { 0., 0., 1., 1. };
  static int cur = 0;
  static int verb = 0;
  int i__1;
  char name__[4];
  double rect[4];
  double ymin, ymax;
  char strf[40];
  int i__, v, n1;
  int na;
  double dv, xx[2], yy[2];
  int nxname;
  char buf[40];
  int wid, iwd;
  double per;
  int nax[4], iwp;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Event scope */
  /*     ipar(1) = win_num */
  /*     ipar(2) = 0/1 color flag */
  /*     ipar(3:10) = color */
  /*     ipar(nipar-3:nipar-2) = window position */
  /*     ipar(nipar-1:nipar)= window position */

  /*     rpar(1)=periode */

  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  /*      data yy / 0.00d0,0.80d0/ */

  dr1_fort("xgetdr\000", name__, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	   &dv, 7L, 4L);
  if (s_cmp (name__, "Rec", 3L, 3L) != 0)
    {
      dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
    }

  if (*flag__ == 2)
    {
      per = rpar[1];
      wid = ipar[1];
      if (*t / per >= z__[1])
	{
	  z__[1] = (int) (*t / per) + 1.;
	  /*     clear window */
	  nax[0] = 2;
	  nax[1] = 10;
	  nax[2] = 2;
	  nax[3] = 10;
	  cur=set_scicos_win(wid);
	  dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
		   &dv, &dv, 7L, 2L);
	  dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0,
		   &c__0, &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
	  scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv,
		     &dv, &dv, &dv, 7L, 2L);
	  s_copy (buf, "t@ @input and output", 40L, 20L);
	  s_copy (strf, "011\000", 40L, 4L);
	  rect[0] = per * (z__[1] - 1.);
	  rect[1] = 0.;
	  rect[2] = per * z__[1];
	  rect[3] = 1.;
	  dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0,
		   &c__0, &v, &dv, &dv, &dv, &dv, 5L, 7L);
	  plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
			 nax, 40L, 40L);
	}

      dr1_fort("xget\000", "window\000", &verb, &cur, &na, &v, &v, &v, &dv,
	       &dv, &dv, &dv, 5L, 7L);
      if (cur != wid)
	{
	  dr1_fort("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv,
		   &dv, &dv, &dv, 5L, 7L);
	}
      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      s_copy (buf, "xlines\000", 40L, 7L);
      xx[0] = *t;
      xx[1] = *t;
      /*  ------------------------------------------------------ */
      yy[0] = 0.;
      yy[1] = .8;
      i__ = 1;
      *nx = 1;
    L10:
      if ((*nevprt & *nx) != 0)
	{
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[i__ + 2], &c__1,
		   &c__2, &v, xx, yy, &dv, &dv, 7L, 2L);
	  yy[0] = (yy[0] + yy[1]) / 2;
	}
      ++i__;
      *nx <<= 1;
      if (*nx <= *nevprt)
	{
	  goto L10;
	}
      yy[0] = 0.;
      /* ---------------------------------------------------------- */
    }
  else if (*flag__ == 4)
    {
      wid = ipar[1];
      ymin = 0.;
      ymax = 1.;
      per = rpar[1];
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 2;
      nax[3] = 10;
      n1 = (int) ((int) (*t) / per);
      if (*t <= 0.)
	{
	  --n1;
	}
      scicos_sciwin ();
      cur=set_scicos_win(wid);
      rect[0] = per * (n1 + 1);
      rect[1] = ymin;
      rect[2] = per * (n1 + 2);
      rect[3] = ymax;
      setscale2scicos_d (frect, rect, "nn\000", 3L);
      iwp = *nipar - 3;
      if (ipar[iwp] >= 0)
	{
	  dr1_fort("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      iwd = *nipar - 1;
      if (ipar[iwd] >= 0)
	{
	  dr1_fort("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}

      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 12L);
      dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 2L);
      scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
		 &dv, &dv, 7L, 2L);
      s_copy (buf, "t@ @input and output", 40L, 20L);
      s_copy (strf, "011\000", 40L, 4L);
      dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	       &v, &dv, &dv, &dv, &dv, 5L, 7L);
      plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
		     nax, 40L, 40L);
      nxname = 40;
      scicos_getlabel (&C2F(curblk).kfun, buf, &nxname, 40L);
      if (nxname > 39)
	{
	  nxname = 39;
	}
      i__1 = nxname;
      s_copy (buf + i__1, "\000", nxname + 1 - i__1, 1L);
      if ( (nxname == 1 && *(unsigned char *) buf == ' ') || nxname == 0)
	{
	}
      else
	{
	  scicos_dr ("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		     &dv, 6L, 40L);
	}
      scicos_sxevents ();
      z__[1] = 0.;
    }
  dr1_fort("xsetdr\000", name__, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	   &dv, 7L, 4L);
  return 0;
}			


int scicos_evtdly (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     event delay */
  /*     delay=rpar(1) */
  if (*flag__ == 3)
    {
      tvec[0] = *t + rpar[0];
    }
  return 0;
}			


int scicos_expblk (scicos_args_poo)
{
  int i__1;
  /* Builtin functions */
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Outputs a^u(i) */
  /*     a=rpar(1) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = exp (log (rpar[1]) * u[i__]);
	  /* L15: */
	}
    }
  if (*flag__ >= 4)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = exp (log (rpar[1]) * u[i__]);
	  /* L20: */
	}
    }
  return 0;
}			


int scicos_forblk (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     For block */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 3)
    {
      if (*nevprt == 1)
	{
	  z__[2] = u[1];
	  z__[1] = 1.;

	  if (u[1] >= 1.)
	    {
	      tvec[1] = *t - 1.;
	      tvec[2] = *t + C2F(costol).ttol / 2.;
	    }
	  else
	    {
	      tvec[1] = *t - 1.;
	      tvec[2] = *t - 1.;
	    }
	}
      else
	{
	  z__[1] += 1.;

	  if (z__[1] >= z__[2])
	    {
	      tvec[1] = *t + C2F(costol).ttol / 2.;
	      tvec[2] = *t - 1.;
	    }
	  else
	    {
	      tvec[1] = *t - 1.;
	      tvec[2] = *t + C2F(costol).ttol / 2.;
	    }
	}
    }
  if (*flag__ == 1 || *flag__ == 3)
    {
      y[1] = z__[1];
    }
  return 0;
}			


int
scicos_fscope (int *flag__, int *nevprt, double *t, double *xd, double *x,
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	       double *rpar, int *nrpar, int *ipar, int *nipar)
{

  static int c__0 = 0;
  static int c__1 = 1;
  static int c_n1 = -1;
  static int c__4 = 4;
  static int c__21 = 21;
  static int c__3 = 3;
  static double c_b88 = 0.;
  static double frect[4] = { 0., 0., 1., 1. };
  static int cur = 0;
  int i__1, i__2;

  /* Builtin functions */
  int s_cmp (char *, char *, long int, long int);
  int s_copy (char *, char *, long int, long int);
  char name__[4];
  double rect[4];
  double ymin, ymax;
  char strf[20];
  int i__, k, n;
  double u[8];
  int v;
  double tsave;
  int n1, n2;
  double dt, dv;
  int nu;
  char buf[20];
  int wid, iwd;
  double per;
  int nax[4], iwp;

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



  /*      character*(4) logf */
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;

  iwp = 12;
  iwd = 14;

  dr1_fort("xgetdr\000", name__, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	   &dv, 7L, 4L);
  if (s_cmp (name__, "Rec", 3L, 3L) != 0)
    {
      dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
    }
  nu = ipar[16];
  if (*flag__ == 2)
    {
      scicos_getouttb (&nu, &ipar[17], u);
      dt = rpar[1];
      ymin = rpar[2];
      ymax = rpar[3];
      per = rpar[4];
      wid = ipar[1];
      n = ipar[3];
      k = (int) z__[1];
      if (k > 0)
	{
	  n1 = (int) (z__[k + 1] / per);
	  if (z__[k + 1] < 0.)
	    {
	      --n1;
	    }
	}
      else
	{
	  n1 = 0;
	}

      tsave = *t;
      if (dt > 0.)
	{
	  *t = z__[k + 1] + dt;
	}

      n2 = (int) (*t / per);
      if (*t < 0.)
	{
	  --n2;
	}

      /*     add new point to the buffer */
      ++k;
      z__[k + 1] = *t;
      i__1 = nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[n + 1 + (i__ - 1) * n + k] = u[i__ - 1];
	  /* L1: */
	}
      z__[1] = (double) k;
      if (n1 == n2 && k < n)
	{
	  *t = tsave;
	  return 0;
	}

      /*     plot 1:K points of the buffer */
      cur=set_scicos_win(wid);      
      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      if (k > 0)
	{
	  i__1 = nu;
	  for (i__ = 1; i__ <= i__1; ++i__)
	    {
	      /*               if(ipar(3+i).ge.0.or.flag.eq.1) then */
	      dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[i__ + 3],
		       &c__1, &k, &v, &z__[2], &z__[n + 2 + (i__ - 1) * n],
		       &dv, &dv, 7L, 2L);
	      /*               endif */
	      /* L10: */
	    }
	}
      /*     shift buffer left */
      z__[2] = z__[k + 1];
      i__1 = nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[n + 1 + (i__ - 1) * n + 1] = z__[n + 1 + (i__ - 1) * n + k];
	  /* L15: */
	}
      z__[1] = 1.;
      if (n1 != n2)
	{
	  /*     clear window */
	  nax[0] = 2;
	  nax[1] = 10;
	  nax[2] = 2;
	  nax[3] = 10;
	  dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
		   &dv, &dv, 7L, 2L);
	  dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0,
		   &c__0, &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
	  scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv,
		     &dv, &dv, &dv, 7L, 2L);
	  s_copy (buf, "t@ @input and output", 20L, 20L);
	  s_copy (strf, "011\000", 20L, 4L);
	  rect[0] = per * (n1 + 1);
	  rect[1] = ymin;
	  rect[2] = per * (n1 + 2);
	  rect[3] = ymax;
	  dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0,
		   &c__0, &v, &dv, &dv, &dv, &dv, 5L, 7L);
	  /*            call dr1('xset'//char(0),'clipping-p'//char(0),-1.0d0, */
	  /*     &           -1.0d0,200000.0d0,200000.0d0,v,dv,dv,dv,dv) */
	  plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
			 nax, &c__4, &c__21, 20L, 20L);
	  /*            call dr1('xset'//char(0),'clipping'//char(0),rect(1),ymin,per, */
	  /*     &           ymax,v,dv,dv,dv,dv) */
	}
      *t = tsave;

    }
  else if (*flag__ == 4)
    {
      wid = ipar[1];
      n = ipar[3];
      ymin = rpar[2];
      ymax = rpar[3];
      per = rpar[4];
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 2;
      nax[3] = 10;
      n1 = (int) ((int) (*t) / per);
      if (*t <= 0.)
	{
	  --n1;
	}
      scicos_sciwin ();
      cur=set_scicos_win(wid);
      if (ipar[iwp] >= 0)
	{
	  dr1_fort("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      if (ipar[iwd] >= 0)
	{
	  dr1_fort("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	  /*     to force dimensions update */
	  dr1_fort("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv,
		   &dv, &dv, &dv, 5L, 7L);
	}
      rect[0] = per * (n1 + 1);
      rect[1] = ymin;
      rect[2] = per * (n1 + 2);
      rect[3] = ymax;
      setscale2scicos_d (frect, rect, "nn\000", 3L);
      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 12L);
      dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 2L);
      scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
		 &dv, &dv, 7L, 2L);
      s_copy (buf, "t@ @input and output\000", 20L, 21L);
      s_copy (strf, "011\000", 20L, 4L);
      dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	       &v, &dv, &dv, &dv, &dv, 5L, 7L);
      plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
		     nax, &c__4, &c__21, 20L, 20L);
      z__[1] = 0.;
      z__[2] = *t;
      i__1 = nu * n;
      scicos_dset (&i__1, &c_b88, &z__[3], &c__1);
    }
  else if (*flag__ == 5)
    {
      wid = ipar[1];
      n = ipar[3];
      k = (int) z__[1];
      if (k <= 1)
	{
	  return 0;
	}
      cur=set_scicos_win(wid); 
      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      i__1 = nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  i__2 = k - 1;
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[i__ + 3], &c__1,
		   &i__2, &v, &z__[2], &z__[n + 2 + (i__ - 1) * n], &dv,
		   &dv, 7L, 2L);
	  /* L30: */
	}
    }
  dr1_fort("xsetdr\000", name__, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	   &dv, 7L, 4L);
  return 0;
}			


int scicos_fsv (scicos_args_poo) 
{
  double d__1, d__2;
  double a, g, a0, b0;
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  a = u[1];
  y[1] = 0.;
  if (a > 1.)
    {
      return 0;
    }
  g = 1.4;
  a0 = 2. / g;
  b0 = (g + 1) / g;
  if (a < .528)
    {
      d__1 = 2 / (g + 1.);
      d__2 = g / (g - 1.);
      a = pow_dd (&d__1, &d__2);
    }
  y[1] = sqrt (g * 2. * (pow_dd (&a, &a0) - pow_dd (&a, &b0)) / (g - 1.));

  return 0;
}			



int
scicos_gensin (scicos_args_poo)
{
  /* Builtin functions */
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Sine genrator */
  /*     ampl=rpar(1) */
  /*     freq=rpar(2) */
  /*     phase=rpar(3) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  y[1] = rpar[1] * sin (rpar[2] * *t + rpar[3]);
  return 0;
}			



int scicos_gensqr (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Square wave generator */
  /*     period=2*rpar(1) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 2)
    {
      z__[1] = -z__[1];
    }
  else if (*flag__ == 1 || *flag__ == 6)
    {
      y[1] = z__[1];
    }
  return 0;
}			



int scicos_hltblk (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Notify simulation to stop  when called */
  /*     ipar(1) : stop reference */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 2)
    {
      C2F(coshlt).halt = 1;
      if (*nipar > 0)
	{
	  z__[1] = (double) ipar[1];
	}
      else
	{
	  z__[1] = 0.;
	}
    }
  return 0;
}			



int
scicos_ifthel (int *flag__, int *nevprt, int *ntvec, double *rpar, int *nrpar,
	       int *ipar, int *nipar, double *u, int *nu)
{
  /*     Scicos block simulator */
  /*     if-then-else block */
  /*     if event input exits from then or else clock ouputs based */
  /*     on the sign of the unique input (if input>0 then  else ) */
  /*     Copyright INRIA */
  --u;
  --ipar;
  --rpar; 
  if (C2F(dbcos).idb == 1)
    {
      sciprint("ifthel: flag=%d\n",*flag__);
    }

  if (*flag__ == 3)
    {
      if (u[1] <= 0.)
	{
	  *ntvec = 2;
	}
      else
	{
	  *ntvec = 1;
	}
    }
  return 0;
}			


int scicos_integr (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Integrator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1 || *flag__ == 6)
    {
      y[1] = x[1];
    }
  else if (*flag__ == 0)
    {
      xd[1] = u[1];
    }
  return 0;
}			

int scicos_intplt (scicos_args_poo)
{
  int np;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     y=f(t) for f a tabulated function from R to R^ny */

  /*     ipar(1)             : np number of mesh points */
  /*     rpar(1:ny+1,1:np) : matrix of mesh point coordinates */
  /*                       first row contains t coordinate mesh points */
  /*                       next rows contains y coordinates mesh points */
  /*                       (one row for each output) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  np = ipar[1];
  scicos_intp (t, &rpar[1], &rpar[np + 1], ny, &np, &y[1]);
  return 0;
}			


int scicos_intpol (scicos_args_poo)
{
  int np;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     y=f(u) for f a tabulated function from R to R^ny */
  /*     rpar(1:ny+1,1:np) : matrix of mesh point coordinates */
  /*                       first row contains u coordinate mesh points */
  /*                       next rows contains y coordinates mesh points */
  /*                       (one row for each output) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  np = ipar[1];
  scicos_intp (&u[1], &rpar[1], &rpar[np + 1], ny, &np, &y[1]);
  return 0;
}			



int
intrp2_fort(int *flag__, int *nevprt, double *t, double *xd, double *x,
	    int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	    double *rpar, int *nrpar, int *ipar, int *nipar, double *u1,
	    int *nu1, double *u2, int *nu2, double *y1, int *ny1)
{
  int i__1;
  int i__, j;
  double vx1, vx2, vy1, vy2, vz1, vz2, vz3, vz4;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) : the number of input */



  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = ipar[1];
  for (i__ = 2; i__ <= i__1; ++i__)
    {
      if (*u1 <= rpar[i__])
	{
	  goto L200;
	}
      /* L100: */
    }
  i__ = ipar[1];
 L200:
  i__1 = ipar[2];
  for (j = 2; j <= i__1; ++j)
    {
      if (*u2 <= rpar[j + ipar[1]])
	{
	  goto L400;
	}
      /* L300: */
    }
  j = ipar[2];
 L400:
  vy1 = rpar[ipar[1] + j - 1];
  vy2 = rpar[ipar[1] + j];
  vz1 = rpar[ipar[1] + ipar[2] + (i__ - 2) * ipar[2] + j - 1];
  vz4 = rpar[ipar[1] + ipar[2] + (i__ - 2) * ipar[2] + j];
  vz2 = rpar[ipar[1] + ipar[2] + (i__ - 1) * ipar[2] + j - 1];
  vz3 = rpar[ipar[1] + ipar[2] + (i__ - 1) * ipar[2] + j];
  vx1 = rpar[i__ - 1];
  vx2 = rpar[i__];
  *y1 =
    (1. - (*u2 - vy1) / (vy2 - vy1)) * (vz1 +
					(vz2 - vz1) * (*u1 - vx1) / (vx2 -
								     vx1)) +
    (*u2 - vy1) / (vy2 - vy1) * (vz4 +
				 (vz3 - vz4) * (*u1 - vx1) / (vx2 - vx1));
  return 0;
}			



int scicos_intrpl (scicos_args_poo) 
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) : the number of input */
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nrpar / 2;
  for (i__ = 2; i__ <= i__1; ++i__)
    {
      if (*u <= rpar[i__])
	{
	  goto L200;
	}
      /* L100: */
    }
  i__ = *nrpar / 2;
 L200:
  *y =
    rpar[*nrpar / 2 + i__ - 1] + (rpar[*nrpar / 2 + i__] -
				  rpar[*nrpar / 2 + i__ - 1]) / (rpar[i__] -
								 rpar[i__ -
								      1]) *
    (*u - rpar[i__ - 1]);
  return 0;
}			



int scicos_invblk (scicos_args_poo)
{
  int i__1;
  int i__;
  double ww;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Outputs the inverse of the input */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 6)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  ww = u[i__];
	  if (ww != 0.)
	    {
	      y[i__] = 1. / ww;
	    }
	  /* L10: */
	}
    }

  if (*flag__ == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  ww = u[i__];
	  if (ww != 0.)
	    {
	      y[i__] = 1. / ww;
	    }
	  else
	    {
	      *flag__ = -2;
	      return 0;
	    }
	  /* L15: */
	}
    }
  return 0;
}			


int scicos_iocopy (scicos_args_poo)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (C2F(dbcos).idb == 1)
    {
      sciprint("ifthel: t=%10.3f, flag=%d\n",*t,*flag__);
    }

  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      y[i__] = u[i__];
      /* L15: */
    }
  return 0;
}			



int scicos_logblk (scicos_args_poo)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     y=log(u)/log(rpar(1)) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  if (u[i__] > 0.)
	    {
	      y[i__] = log (u[i__]) / log (rpar[1]);
	    }
	  else
	    {
	      *flag__ = -2;
	      return 0;
	    }
	  /* L15: */
	}
    }
  if (*flag__ == 6)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  if (u[i__] > 0.)
	    {
	      y[i__] = log (u[i__]) / log (rpar[1]);
	    }
	  /* L20: */
	}
    }
  return 0;
}			

int scicos_lookup (scicos_args_poo)
{
  int i__1;
  double dout;
  int i__, n;
  double du;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     rpar(1:n)  =  u coordinate discretisation must be strictly increasing */
  /*     rpar(n+1:2*n)  =  y coordinate discretisation */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  n = *nrpar / 2;
  if (n > 2)
    {
      i__1 = n - 1;
      for (i__ = 2; i__ <= i__1; ++i__)
	{
	  if (u[1] <= rpar[i__])
	    {
	      goto L20;
	    }
	  /* L10: */
	}
    }
  else
    {
      if (n == 1)
	{
	  y[1] = rpar[2];
	  return 0;
	}
      i__ = 2;
    }
 L20:
  du = rpar[i__] - rpar[i__ - 1];
  dout = rpar[n + i__] - rpar[n + i__ - 1];
  y[1] = rpar[n + i__] - (rpar[i__] - u[1]) * dout / du;
  return 0;
}			



int scicos_lsplit (scicos_args_poo)
{

  int i__1, i__2;
  int i__, j, k;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     splitting signals */
  /* c */

  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  j = 0;
  i__1 = *ny / *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      i__2 = *nu;
      for (k = 1; k <= i__2; ++k)
	{
	  ++j;
	  y[j] = u[k];
	  /* L1: */
	}
      /* L2: */
    }
  return 0;
}			


int scicos_lusat (int *flag__, int *nevprt, double *t, double *xd, double *x,
		  int *nx, double *z__, int *nz, double *tvec, int *ntvec,
		  double *rpar, int *nrpar, int *ipar, int *nipar, double *u,
		  int *nu, double *y, int *ny, double *g, int *ng)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Lower-Upper saturation */
  /*     Continous block, MIMO */
  --g;
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 9)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  g[i__] = u[i__] - rpar[1];
	  g[i__ + *nu] = u[i__] - rpar[2];
	  /* L10: */
	}
    }
  if (*flag__ == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  if (u[i__] <= rpar[1])
	    {
	      y[i__] = rpar[1] * rpar[3];
	    }
	  else if (u[i__] >= rpar[2])
	    {
	      y[i__] = rpar[2] * rpar[3];
	    }
	  else
	    {
	      y[i__] = rpar[3] * u[i__];
	    }
	  /* L15: */
	}
    }
  return 0;
}			


int scicos_maxblk (scicos_args_poo)
{
  int i__1;
  double d__1, d__2;
  int i__;
  double ww;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     outputs the maximum of all inputs */

  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  ww = u[1];
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      /* Computing MAX */
      d__1 = ww, d__2 = u[i__];
      ww = Max (d__1, d__2);
      /* L15: */
    }
  y[1] = ww;
  return 0;
}			




int scicos_memo (scicos_args_poo) 
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     returns sample and hold  of the input */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 2)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = u[i__];
	  /* L15: */
	}
    }
  else if (*flag__ == 4)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = rpar[i__];
	  /* L25: */
	}
    }
  return 0;
}		


int scicos_mfclck (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     multifrequency clock */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 4)
    {
      z__[1] = 0.;
    }
  else if (*flag__ == 2)
    {
      z__[1] += 1.;
      if (z__[1] == (double) ipar[1])
	{
	  z__[1] = 0.;
	}
    }
  else if (*flag__ == 3)
    {
      if (z__[1] == (double) (ipar[1] - 1))
	{
	  tvec[1] = *t - 1.;
	  tvec[2] = *t + rpar[1];
	}
      else
	{
	  tvec[1] = *t + rpar[1];
	  tvec[2] = *t - 1.;
	}
    }
  return 0;
}			

int scicos_minblk (scicos_args_poo)
{
  int i__1;
  double d__1, d__2;
  int i__;
  double ww;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     outputs the minimum of all inputs */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  ww = u[1];
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      /* Computing MIN */
      d__1 = ww, d__2 = u[i__];
      ww = Min (d__1, d__2);
      /* L15: */
    }
  y[1] = ww;
  return 0;
}			



int scicos_mscope (scicos_args_poo)
{
  static int c__1 = 1;
  static int c__0 = 0;
  static int c_n1 = -1;
  static int c__3 = 3;
  static double c_b103 = 0.;  static int cur = 0;

  int i__1, i__2, i__3;

  /* Builtin functions */
  int s_cmp (char *, char *, long int, long int);
  int s_copy (char *, char *, long int, long int);
  char name__[4];
  double rect[4];
  int kwid;
  int nwid;
  char strf[40];
  int i__, k, n, v;
  double frect[4], tsave;
  int n1, n2;
  double dt, dv;
  int it, nxname;
  char buf[40];
  int wid, iwd;
  double per;
  int nax[4], ilt, iwp;
  int herited;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = number of subwindows (input ports) */
  /*     ipar(3) = buffer size */
  /*     ipar(4:5) : window position */
  /*     ipar(6:7) : window dimension */
  /*     ipar(8:7+ipar(2)) = input port sizes */
  /*     ipar(8+ipar(2):7+ipar(2)+nu) = line type for ith curve */
  /*     ipar(8+ipar(2)+nu) = acceptance of inherited events */
  /*     rpar(1)=dt */
  /*     rpar(2)=periode */
  /*     rpar(3)=ymin_1 */
  /*     rpar(4)=ymax_1 */
  /*     ... */
  /*     rpar(2*k+1)=ymin_k */
  /*     rpar(2*k+2)=ymax_k */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;

  dr1_fort("xgetdr\000", name__, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	   &dv, 7L, 4L);
  if (s_cmp (name__, "Rec", 3L, 3L) != 0)
    {
      dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
    }

  wid = ipar[1];
  nwid = ipar[2];
  n = ipar[3];
  per = rpar[2];
  dt = rpar[1];
  if (*nipar < ipar[2] + 8 + *nu)
    {
      /*     compatibility */
      herited = TRUE;
    }
  else
    {
      herited = ipar[ipar[2] + 8 + *nu] != 0;
    }

  if (*flag__ <= 2)
    {
      k = (int) z__[1];
      if (k > 0)
	{
	  n1 = (int) (z__[k + 1] / per);
	  if (z__[k + 1] < 0.)
	    {
	      --n1;
	    }
	}
      else
	{
	  n1 = 0;
	}

      tsave = *t;
      if (dt > 0.)
	{
	  *t = z__[k + 1] + dt;
	}

      n2 = (int) (*t / per);
      if (*t < 0.)
	{
	  --n2;
	}

      /*     add new point to the buffer */
      ++k;
      z__[k + 1] = *t;
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[n + 1 + (i__ - 1) * n + k] = u[i__];
	  /* L5: */
	}
      z__[1] = (double) k;
      if (n1 == n2 && k < n)
	{
	  *t = tsave;
	  return 0;
	}

      /*     plot 1:K points of the buffer */
      cur=set_scicos_win(wid);
      dr1_fort("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	       &v, &dv, &dv, &dv, &dv, 5L, 7L);
      ilt = ipar[2] + 8;
      it = 0;
      /*     loop on input ports */
      if (k > 0)
	{
	  i__1 = nwid;
	  for (kwid = 1; kwid <= i__1; ++kwid)
	    {
	      s_copy (buf, "xlines\000", 40L, 7L);
	      rect[0] = per * n1;
	      rect[1] = rpar[(kwid << 1) + 1];
	      rect[2] = per * (n1 + 1);
	      rect[3] = rpar[(kwid << 1) + 2];
	      frect[0] = 0.;
	      frect[1] = (kwid - 1) * (1. / nwid);
	      frect[2] = 1.;
	      frect[3] = 1. / nwid;
	      setscale2scicos_d (frect, rect, "nn\000", 3L);
	      scicos_scicosclip (&c__1);
	      /*     loop on input port elements */
	      i__2 = ipar[kwid + 7];
	      for (i__ = 1; i__ <= i__2; ++i__)
		{
		  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[ilt + it],
			   &c__1, &k, &v, &z__[2], &z__[n + 2 + it * n],
			   &dv, &dv, 7L, 2L);
		  ++it;
		  /* L10: */
		}
	      scicos_scicosclip (&c__0);
	      /* L11: */
	    }
	}
      /*     shift buffer left */
      z__[2] = z__[k + 1];
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[n + 1 + (i__ - 1) * n + 1] = z__[n + 1 + (i__ - 1) * n + k];
	  /* L15: */
	}
      z__[1] = 1.;
      if (n1 != n2)
	{
	  /*     clear window */
	  nax[0] = 2;
	  nax[1] = 10;
	  nax[2] = 2;
	  nax[3] = 10;
	  dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
		   &dv, &dv, 7L, 2L);
	  dr1_fort("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0,
		   &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
	  scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv,
		     &dv, &dv, &dv, 7L, 2L);
	  s_copy (buf, "t@ @input and output", 40L, 20L);
	  s_copy (strf, "011\000", 40L, 4L);
	  dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0,
		   &c__0, &v, &dv, &dv, &dv, &dv, 5L, 7L);
	  i__1 = nwid;
	  for (kwid = 1; kwid <= i__1; ++kwid)
	    {
	      rect[0] = per * (n1 + 1);
	      rect[1] = rpar[(kwid << 1) + 1];
	      rect[2] = per * (n1 + 2);
	      rect[3] = rpar[(kwid << 1) + 2];
	      frect[0] = 0.;
	      frect[1] = (kwid - 1) * (1. / nwid);
	      frect[2] = 1.;
	      frect[3] = 1. / nwid;
	      setscale2scicos_d (frect, rect, "nn\000", 3L);
	      plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf,
			     rect, nax, 40L, 40L);
	      /* L16: */
	    }
	}
      *t = tsave;

    }
  else if (*flag__ == 4)
    {
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 2;
      nax[3] = 10;
      n1 = (int) ((int) (*t) / per);
      if (*t <= 0.)
	{
	  --n1;
	}
      scicos_sciwin ();
      cur=set_scicos_win(wid);      
      iwp = 4;
      if (ipar[iwp] >= 0)
	{
	  dr1_fort("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      iwd = 6;
      if (ipar[iwd] >= 0)
	{
	  dr1_fort("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      dr1_fort("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 12L);
      dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 2L);
      scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
		 &dv, &dv, 7L, 2L);
      s_copy (buf, "t@ @input and output", 40L, 20L);
      s_copy (strf, "011\000", 40L, 4L);
      dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	       &v, &dv, &dv, &dv, &dv, 5L, 7L);
      nxname = 40;
      scicos_getlabel (&C2F(curblk).kfun, buf, &nxname, 40L);
      if (nxname > 39)
	{
	  nxname = 39;
	}
      i__1 = nxname;
      s_copy (buf + i__1, "\000", nxname + 1 - i__1, 1L);
      if ((nxname == 1 && *(unsigned char *) buf == ' ' ) || nxname == 0)
	{
	}
      else
	{
	  scicos_dr ("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		     &dv, 6L, 40L);
	}
      i__1 = nwid;
      for (kwid = 1; kwid <= i__1; ++kwid)
	{
	  rect[0] = per * (n1 + 1);
	  rect[1] = rpar[(kwid << 1) + 1];
	  rect[2] = per * (n1 + 2);
	  rect[3] = rpar[(kwid << 1) + 2];
	  frect[0] = 0.;
	  frect[1] = (kwid - 1) * (1. / nwid);
	  frect[2] = 1.;
	  frect[3] = 1. / nwid;
	  setscale2scicos_d (frect, rect, "nn\000", 3L);
	  plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
			 nax, 40L, 40L);
	  /* L20: */
	}

      z__[1] = 0.;
      z__[2] = *t;
      i__1 = *nu * n;
      scicos_dset (&i__1, &c_b103, &z__[3], &c__1);
    }
  else if (*flag__ == 5)
    {
      k = (int) z__[1];
      if (k <= 1)
	{
	  return 0;
	}
      cur=set_scicos_win(wid);  
      dr1_fort("xset\000", "use color\000", &c__1, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);

      ilt = ipar[2] + 8;
      it = 0;
      n1 = (int) ((int) (*t) / per);
      if (*t <= 0.)
	{
	  --n1;
	}
      /*     loop on input ports */
      i__1 = nwid;
      for (kwid = 1; kwid <= i__1; ++kwid)
	{
	  rect[0] = per * (n1 + 1);
	  rect[1] = rpar[(kwid << 1) + 1];
	  rect[2] = per * (n1 + 2);
	  rect[3] = rpar[(kwid << 1) + 2];
	  frect[0] = 0.;
	  frect[1] = (kwid - 1) * (1. / nwid);
	  frect[2] = 1.;
	  frect[3] = 1. / nwid;
	  setscale2scicos_d (frect, rect, "nn\000", 3L);
	  scicos_scicosclip (&c__1);
	  /*     loop on input port elements */
	  i__2 = ipar[kwid + 7];
	  for (i__ = 1; i__ <= i__2; ++i__)
	    {
	      i__3 = k - 1;
	      dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[ilt + it],
		       &c__1, &i__3, &v, &z__[2], &z__[n + 2 + it * n],
		       &dv, &dv, 7L, 2L);
	      ++it;
	      /* L30: */
	    }
	  scicos_scicosclip (&c__0);
	  /* L35: */
	}
    }
  dr1_fort("xsetdr\000", name__, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	   &dv, 7L, 4L);
  return 0;
}		



int scicos_mux (int *flag__, int *nevprt, double *t, double *xd, double *x,
		int *nx, double *z__, int *nz, double *tvec, int *ntvec,
		double *rpar, int *nrpar, int *ipar, int *nipar, double *uy1,
		int *nuy1, double *uy2, int *nuy2, double *uy3, int *nuy3,
		double *uy4, int *nuy4, double *uy5, int *nuy5, double *uy6,
		int *nuy6, double *uy7, int *nuy7, double *uy8, int *nuy8,
		double *uy9, int *nuy9)
{
  int i__1;
  int i__, k;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) : the number of input */
  --uy9;
  --uy8;
  --uy7;
  --uy6;
  --uy5;
  --uy4;
  --uy3;
  --uy2;
  --uy1;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  k = 0;
  switch (ipar[1] - 1)
    {
    case 1:
      goto L20;
    case 2:
      goto L30;
    case 3:
      goto L40;
    case 4:
      goto L50;
    case 5:
      goto L60;
    case 6:
      goto L70;
    case 7:
      goto L80;
    }

 L20:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[k] = uy1[i__];
      /* L25: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy3[k] = uy2[i__];
      /* L27: */
    }
  return 0;

 L30:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[k] = uy1[i__];
      /* L35: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[k] = uy2[i__];
      /* L37: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy4[k] = uy3[i__];
      /* L38: */
    }
  return 0;

 L40:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[k] = uy1[i__];
      /* L41: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[k] = uy2[i__];
      /* L42: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[k] = uy3[i__];
      /* L43: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy5[k] = uy4[i__];
      /* L44: */
    }
  return 0;

 L50:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[k] = uy1[i__];
      /* L51: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[k] = uy2[i__];
      /* L52: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[k] = uy3[i__];
      /* L53: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[k] = uy4[i__];
      /* L54: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy6[k] = uy5[i__];
      /* L55: */
    }
  return 0;

 L60:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[k] = uy1[i__];
      /* L61: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[k] = uy2[i__];
      /* L62: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[k] = uy3[i__];
      /* L63: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[k] = uy4[i__];
      /* L64: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[k] = uy5[i__];
      /* L65: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy7[k] = uy6[i__];
      /* L66: */
    }
  return 0;

 L70:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy1[i__];
      /* L71: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy2[i__];
      /* L72: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy3[i__];
      /* L73: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy4[i__];
      /* L74: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy5[i__];
      /* L75: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy6[i__];
      /* L76: */
    }
  i__1 = *nuy7;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy8[k] = uy7[i__];
      /* L77: */
    }
  return 0;

 L80:
  i__1 = *nuy1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy1[i__];
      /* L81: */
    }
  i__1 = *nuy2;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy2[i__];
      /* L82: */
    }
  i__1 = *nuy3;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy3[i__];
      /* L83: */
    }
  i__1 = *nuy4;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy4[i__];
      /* L84: */
    }
  i__1 = *nuy5;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy5[i__];
      /* L85: */
    }
  i__1 = *nuy6;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy6[i__];
      /* L86: */
    }
  i__1 = *nuy7;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy7[i__];
      /* L87: */
    }
  i__1 = *nuy8;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ++k;
      uy9[k] = uy8[i__];
      /* L88: */
    }
  return 0;
}	


int scicos_pload ( scicos_args_poo) 
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Preload function */
  /*     if u(i).lt.0 then y(i)=-u(i)-rpar(i) */
  /*     else y(i)=u(i)+rpar(i) */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  /* L10: */
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      if (u[i__] < 0.)
	{
	  y[i__] = u[i__] - rpar[i__];
	}
      else if (u[i__] > 0.)
	{
	  y[i__] = u[i__] + rpar[i__];
	}
      else
	{
	  y[i__] = 0.;
	}
      /* L15: */
    }
  return 0;
}			


int scicos_powblk (scicos_args_poo)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     rpar(1) is power */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*nrpar == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  if (u[i__] < 0.)
	    {
	      if (*flag__ >= 4)
		{
		  return 0;
		}
	      *flag__ = -2;
	      return 0;
	    }
	  else if (u[i__] == 0. && rpar[1] <= 0.)
	    {
	      if (*flag__ >= 4)
		{
		  return 0;
		}
	      *flag__ = -2;
	      return 0;
	    }
	  y[i__] = pow_dd (&u[i__], &rpar[1]);
	  /* L15: */
	}
    }
  else
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  if (ipar[1] <= 0 && u[i__] == 0.)
	    {
	      if (*flag__ >= 4)
		{
		  return 0;
		}
	      *flag__ = -2;
	      return 0;
	    }
	  y[i__] = pow_di (&u[i__], &ipar[1]);
	  /* L25: */
	}
    }
  return 0;
}			


int scicos_qzcel (scicos_args_poo) 
{

  int i__1;
  double d__1;

  /* Builtin functions */
  double d_nint (double *);
  int i__;

  /*     Copyright INRIA */
  /*     Scicos block simulator */

  /*     Gives quantized signal by ceiling method */
  /*     rpar(i) quantization step used for i input */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      d__1 = u[i__] / rpar[i__] - .5;
      y[i__] = rpar[i__] * d_nint (&d__1);
      /* L15: */
    }
  return 0;
}			


int scicos_qzflr (scicos_args_poo) 
{

  int i__1;
  double d__1;

  /* Builtin functions */
  double d_nint (double *);
  int i__;

  /*     Copyright INRIA */
  /*     Scicos block simulator */

  /*     Gives quantized signal by floor method */
  /*     rpar(i) quantization step used for i input */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      d__1 = u[i__] / rpar[i__] + .5;
      y[i__] = rpar[i__] * d_nint (&d__1);
      /* L15: */
    }
  return 0;
}			



int scicos_qzrnd (scicos_args_poo)
{

  int i__1;
  double d__1;

  /* Builtin functions */
  double d_nint (double *);
  int i__;

  /*     Copyright INRIA */
  /*     Scicos block simulator */

  /*     Gives quantized signal by round method */
  /*     rpar(i) quantization step used for i input */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      if (u[i__] < 0.)
	{
	  d__1 = u[i__] / rpar[i__] + .5;
	  y[i__] = rpar[i__] * (d_nint (&d__1) - .5);
	}
      else
	{
	  d__1 = u[i__] / rpar[i__] - .5;
	  y[i__] = rpar[i__] * (d_nint (&d__1) + .5);
	}
      /* L15: */
    }
  return 0;
}			




int scicos_qztrn (scicos_args_poo)
{

  int i__1;
  double d__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Gives quantized signal by truncation method */
  /*     rpar(i) quantization step used for i input */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      if (u[i__] < 0.)
	{
	  d__1 = u[i__] / rpar[i__] + .5;
	  y[i__] = rpar[i__] * d_nint (&d__1);
	}
      else
	{
	  d__1 = u[i__] / rpar[i__] - .5;
	  y[i__] = rpar[i__] * d_nint (&d__1);
	}
      /* L15: */
    }
  return 0;
}			


int scicos_rndblk (scicos_args_poo)
{

  int i__1;
  int i__;
  double t1, si;
  int iy;
  double sr;

  /*     Copyright INRIA */
  /*     Scicos block simulator */

  /*     ipar(1) */
  /*            0 : uniform */
  /*            1 : normal */
  /*     rpar(1:ny)=mean */
  /*     rpar(ny+1:2*ny)=deviation */
  /*     rpar(2*ny+1)=dt */



  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1 || *flag__ == 6)
    {
      i__1 = *ny;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = rpar[i__] + rpar[*ny + i__] * z__[i__ + 1];
	  /* L10: */
	}
    }
  else if (*flag__ == 2 || *flag__ == 4)
    {
      /*     uniform */
      if (ipar[1] == 0)
	{
	  iy = (int) z__[1];
	  i__1 = *nz - 1;
	  for (i__ = 1; i__ <= i__1; ++i__)
	    {
	      z__[i__ + 1] = scicos_urand (&iy);
	      /* L20: */
	    }
	}
      else
	{
	  iy = (int) z__[1];
	  /*     normal */
	  i__1 = *nz - 1;
	  for (i__ = 1; i__ <= i__1; ++i__)
	    {
	    L75:
	      sr = scicos_urand (&iy) * 2. - 1.;
	      si = scicos_urand (&iy) * 2. - 1.;
	      t1 = sr * sr + si * si;
	      if (t1 > 1.)
		{
		  goto L75;
		}
	      z__[i__ + 1] = sr * sqrt (log (t1) * -2. / t1);
	      /* L30: */
	    }
	}
      z__[1] = (double) iy;
      /*         if(ntvec.eq.1) tvec(1)=t+rpar(2*(nz-1)+1) */
    }
  return 0;
}			



int scicos_samphold (scicos_args_poo) 
{

  int i__1;
  int i__;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     returns sample and hold  of the input */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1)
    {
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = u[i__];
	  /* L15: */
	}
    }
  return 0;
}			


int scicos_sawtth (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 1 && *nevprt == 0)
    {
      y[1] = *t - z__[1];
    }
  else if (*flag__ == 1 && *nevprt == 1)
    {
      y[1] = 0.;
    }
  else if (*flag__ == 2 && *nevprt == 1)
    {
      z__[1] = *t;
    }
  else if (*flag__ == 4)
    {
      z__[1] = 0.;
    }
  return 0;
}			


int
scicos_scope (int *flag__, int *nevprt, double *t, double *xd, double *x,
	      int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	      double *rpar, int *nrpar, int *ipar, int *nipar, double *u,
	      int *nu)
{
  static int c__1 = 1;
  static int c__0 = 0;
  static int c_n1 = -1;
  static int c__4 = 4;
  static int c__21 = 21;
  static int c__3 = 3;
  static double c_b86 = 0.;
  static double frect[4] = { 0., 0., 1., 1. };
  static int cur = 0;
  int i__1;

  /* Builtin functions */
  int s_copy (char *, char *, long int, long int);
  double rect[4];
  double ymin, ymax;
  char strf[40];
  int i__, k, n, v;
  double tsave;
  int n1, n2;
  double dt, dv;
  int nxname;
  char buf[40];
  int wid, iwd;
  double per;
  int nax[4], iwp;
  int herited;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = 0/1 color flag */
  /*     ipar(3) = buffer size */
  /*     ipar(4:11) = line type for ith curve */

  /*     ipar(12:13) : window position */
  /*     ipar(14:15) : window dimension */
  /*     ipar(16) : acceptance of inherited events */

  /*     rpar(1)=dt */
  /*     rpar(2)=ymin */
  /*     rpar(3)=ymax */
  /*     rpar(4)=periode */



  /*      character*(4) logf */
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;

  if (*nipar < 16)
    {
      /*     compatibility */
      herited = TRUE;
      iwp = *nipar - 3;
      iwd = *nipar - 1;
    }
  else
    {
      herited = ipar[16] != 0;
      iwp = *nipar - 4;
      iwd = *nipar - 2;
    }

  if (*flag__ == 2)
    {
      dt = rpar[1];
      ymin = rpar[2];
      ymax = rpar[3];
      per = rpar[4];
      wid = ipar[1];
      n = ipar[3];
      k = (int) z__[1];
      if (k > 0)
	{
	  n1 = (int) (z__[k + 1] / per);
	  if (z__[k + 1] < 0.)
	    {
	      --n1;
	    }
	}
      else
	{
	  n1 = 0;
	}

      tsave = *t;
      if (dt > 0.)
	{
	  *t = z__[k + 1] + dt;
	}

      n2 = (int) (*t / per);
      if (*t < 0.)
	{
	  --n2;
	}

      /*     add new point to the buffer */
      ++k;
      z__[k + 1] = *t;
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[n + 1 + (i__ - 1) * n + k] = u[i__];
	  /* L1: */
	}
      z__[1] = (double) k;
      if (n1 == n2 && k < n)
	{
	  *t = tsave;
	  return 0;
	}

      /*     plot 1:K points of the buffer */
      cur=set_scicos_win(wid);   
      dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
      /*         call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0, */
      /*     &        0,0,v,dv,dv,dv,dv) */
      if (k > 0)
	{
	  scicos_scicosclip (&c__1);
	  i__1 = *nu;
	  for (i__ = 1; i__ <= i__1; ++i__)
	    {
	      /*               if(ipar(3+i).ge.0.or.flag.eq.1) then */
	      dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[i__ + 3],
		       &c__1, &k, &v, &z__[2], &z__[n + 2 + (i__ - 1) * n],
		       &dv, &dv, 7L, 2L);
	      /*               endif */
	      /* L10: */
	    }
	  scicos_scicosclip (&c__0);
	}
      /*     shift buffer left */
      z__[2] = z__[k + 1];
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  z__[n + 1 + (i__ - 1) * n + 1] = z__[n + 1 + (i__ - 1) * n + k];
	  /* L15: */
	}
      z__[1] = 1.;
      if (n1 != n2)
	{
	  /*     clear window */
	  nax[0] = 2;
	  nax[1] = 10;
	  nax[2] = 2;
	  nax[3] = 10;
	  dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
		   &dv, &dv, 7L, 2L);
	  dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0,
		   &c__0, &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
	  scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv,
		     &dv, &dv, &dv, 7L, 2L);
	  s_copy (buf, "t@ @input and output", 40L, 20L);
	  s_copy (strf, "011\000", 40L, 4L);
	  rect[0] = per * (n1 + 1);
	  rect[1] = ymin;
	  rect[2] = per * (n1 + 2);
	  rect[3] = ymax;
	  dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0,
		   &c__0, &v, &dv, &dv, &dv, &dv, 5L, 7L);
	  /*            call dr1('xset'//char(0),'clipping-p'//char(0),-1.0d0, */
	  /*     &           -1.0d0,200000.0d0,200000.0d0,v,dv,dv,dv,dv) */
	  plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
			 nax, &c__4, &c__21, 40L, 40L);
	  /*            call dr('xset'//char(0),'clipping'//char(0),rect(1),rect(2), */
	  /*     &           rect(3),rect(4),v,v,dv,dv,dv,dv) */
	}
      *t = tsave;

    }
  else if (*flag__ == 4)
    {
      wid = ipar[1];
      n = ipar[3];
      ymin = rpar[2];
      ymax = rpar[3];
      per = rpar[4];
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 2;
      nax[3] = 10;
      n1 = (int) ((int) (*t) / per);
      if (*t <= 0.)
	{
	  --n1;
	}
      scicos_sciwin ();
      dr1_fort("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv,
	       &dv, &dv, &dv, 5L, 7L);
      dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
      if (ipar[iwp] >= 0)
	{
	  dr1_fort("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      if (ipar[iwd] >= 0)
	{
	  dr1_fort("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	  /*     to force dimensions update */
	  dr1_fort("xset\000", "window\000", &wid, &v, &v, &v, &v, &v, &dv,
		   &dv, &dv, &dv, 5L, 7L);
	}
      rect[0] = per * (n1 + 1);
      rect[1] = ymin;
      rect[2] = per * (n1 + 2);
      rect[3] = ymax;
      setscale2scicos_d (frect, rect, "nn\000", 3L);
      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 12L);
      dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 2L);
      scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
		 &dv, &dv, 7L, 2L);
      s_copy (buf, "t@ @input and output\000", 40L, 21L);
      s_copy (strf, "011\000", 40L, 4L);
      dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	       &v, &dv, &dv, &dv, &dv, 5L, 7L);
      plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
		     nax, &c__4, &c__21, 40L, 40L);
      scicos_scicosclip (&c__1);
      nxname = 40;
      scicos_getlabel (&C2F(curblk).kfun, buf, &nxname, 40L);
      if (nxname > 39)
	{
	  nxname = 39;
	}
      i__1 = nxname;
      s_copy (buf + i__1, "\000", nxname + 1 - i__1, 1L);
      if ((nxname == 1 && *(unsigned char *) buf == ' ') || nxname == 0)
	{
	}
      else
	{
	  scicos_dr ("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		     &dv, 6L, 40L);
	}
      z__[1] = 0.;
      z__[2] = *t;
      i__1 = *nu * n;
      scicos_dset (&i__1, &c_b86, &z__[3], &c__1);
    }
  else if (*flag__ == 5)
    {
      wid = ipar[1];
      n = ipar[3];
      k = (int) z__[1];
      if (k <= 1)
	{
	  return 0;
	}
      cur=set_scicos_win(wid);     
      /*         call dr1('xset'//char(0),'use color'//char(0),ipar(2),0,0, */
      /*     &        0,0,v,dv,dv,dv,dv) */
      scicos_scicosclip (&c__1);
      i__1 = *nu;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[i__ + 3], &c__1,
		   &k, &v, &z__[2], &z__[n + 2 + (i__ - 1) * n], &dv, &dv,
		   7L, 2L);
	  /* L30: */
	}
      scicos_scicosclip (&c__0);
    }
  return 0;
}			


/* Table of constant values */


int scicos_scopxy (scicos_args_poo)
{
  static int c__1 = 1;
  static int c__2 = 2;
  static int c__0 = 0;
  static int c__3 = 3;
  static int c_n1 = -1;
  static int c__6 = 6;
  static double frect[4] = { 0., 0., 1., 1. };
  static int cur = 0;
 int i__1;
 double rect[4];
 double xmin, ymin, xmax, ymax;
 char strf[40];
 int n, v;
 double dv;
 int nxname;
 char buf[40];
 int wid, iwd, nax[4], iwp;

 /*     Copyright INRIA */
 /*     Scicos block simulator */
 /*     ipar(1) = win_num */
 /*     ipar(2) = 0/1 color flag */
 /*     ipar(3) = buffer size */
 /*     ipar(4) = dash,color or mark choice */
 /*     ipar(5) = line or mark size */
 /*     ipar(6) = mode : animated =0 fixed=1 */
 /*     ipar(7:8) = window position */
 /*     ipar(9:10) = window size */

 /*     rpar(1)=xmin */
 /*     rpar(2)=xmax */
 /*     rpar(3)=ymin */
 /*     rpar(4)=ymax */



 --y;
 --u;
 --ipar;
 --rpar;
 --tvec;
 --z__;
 --x;
 --xd;

 if (*flag__ == 2)
   {
     wid = ipar[1];
     n = ipar[3];

     cur=set_scicos_win(wid); 
     /*     erase first point */
     if (ipar[6] == 0)
       {
	 z__[1] += 1.;
	 if (ipar[4] < 0)
	   {
	     dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1,
		      &c__1, &v, &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
	   }
	 else
	   {
	     dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1,
		      &c__2, &v, &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
	   }
       }
     /*     shift buffer left */
     i__1 = n - 1;
     scicos_unsfdcopy (&i__1, &z__[3], &c__1, &z__[2], &c__1);
     z__[n + 1] = u[1];
     i__1 = n - 1;
     scicos_unsfdcopy (&i__1, &z__[n + 3], &c__1, &z__[n + 2], &c__1);
     z__[(n << 1) + 1] = u[2];
     /*     draw new point */
     if (ipar[4] < 0)
       {
	 dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &c__1,
		  &v, &z__[n + 1], &z__[(n << 1) + 1], &dv, &dv, 7L, 2L);
       }
     else
       {
	 dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &c__2,
		  &v, &z__[n], &z__[n * 2], &dv, &dv, 7L, 2L);
       }
     if ((int) z__[1] > n && ipar[6] == 0)
       {
	 /*     erase memory */
	 scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv,
		    &dv, &dv, &dv, 7L, 2L);
	 xmin = rpar[1];
	 xmax = rpar[2];
	 ymin = rpar[3];
	 ymax = rpar[4];
	 rect[0] = xmin;
	 rect[1] = ymin;
	 rect[2] = xmax;
	 rect[3] = ymax;
	 setscale2scicos_d (frect, rect, "nn\000", 3L);
	 z__[1] = 0.;
       }
   }
 else if (*flag__ == 4)
   {
     wid = ipar[1];
     n = ipar[3];
     xmin = rpar[1];
     xmax = rpar[2];
     ymin = rpar[3];
     ymax = rpar[4];
     nax[0] = 2;
     nax[1] = 10;
     nax[2] = 2;
     nax[3] = 10;
     scicos_sciwin ();
     cur=set_scicos_win(wid);
     dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	      &dv, &dv, 7L, 4L);
     iwp = 7;
     if (ipar[iwp] >= 0)
       {
	 dr1_fort("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v,
		  &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
       }
     iwd = 9;
     if (ipar[iwd] >= 0)
       {
	 dr1_fort("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v,
		  &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
       }
     rect[0] = xmin;
     rect[1] = ymin;
     rect[2] = xmax;
     rect[3] = ymax;
     setscale2scicos_d (frect, rect, "nn\000", 3L);
     dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	      &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
     dr1_fort("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0,
	      &c__0, &v, &dv, &dv, &dv, &dv, 5L, 12L);
     dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	      &dv, &dv, 7L, 2L);
     scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
		&dv, &dv, 7L, 2L);
     s_copy (buf, "t@ @input and output", 40L, 20L);
     s_copy (strf, "011\000", 40L, 4L);
     dr1_fort("xset\000", "thickness\000", &c__1, &v, &v, &v, &v, &v, &dv,
	      &dv, &dv, &dv, 5L, 10L);
     dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	      &v, &dv, &dv, &dv, &dv, 5L, 7L);
     dr1_fort("xset\000", "alufunction\000", &c__3, &v, &v, &v, &v, &v,
	      &dv, &dv, &dv, &dv, 5L, 12L);
     plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
		    nax, 40L, 40L);
     nxname = 40;
     scicos_getlabel (&C2F(curblk).kfun, buf, &nxname, 40L);
     if (nxname > 39)
       {
	 nxname = 39;
       }
     i__1 = nxname;
     s_copy (buf + i__1, "\000", nxname + 1 - i__1, 1L);
     if ((nxname == 1 && *(unsigned char *) buf == ' ') || nxname == 0)
       {
       }
     else
       {
	 scicos_dr ("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		    &dv, 6L, 40L);
       }
     dr1_fort("xset\000", "alufunction\000", &c__6, &v, &v, &v, &v, &v,
	      &dv, &dv, &dv, &dv, 5L, 12L);
     scicos_sxevents ();
     /* first point drawing */
     if (ipar[4] < 0)
       {
	 dr1_fort("xset\000", "mark\000", &ipar[4], &ipar[5], &v, &v, &v,
		  &v, &dv, &dv, &dv, &dv, 5L, 5L);
	 dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &c__1,
		  &v, &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
       }
     else
       {
	 dr1_fort("xset\000", "thickness\000", &ipar[5], &v, &v, &v, &v,
		  &v, &dv, &dv, &dv, &dv, 5L, 10L);
	 dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &c__2,
		  &v, &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
       }
     z__[1] = 0.;
   }
 else if (*flag__ == 5)
   {
     wid = ipar[1];
     n = ipar[3];

     cur=set_scicos_win(wid);
     dr1_fort("xset\000", "alufunction\000", &c__3, &v, &v, &v, &v, &v,
	      &dv, &dv, &dv, &dv, 5L, 12L);
   }
 return 0;
}


int scicos_scoxy (scicos_args_poo)
{
  static int c__1 = 1;
  static int c__0 = 0;
  static int c__3 = 3;
  static int c_n1 = -1;
  /* Initialized data */
  static double frect[4] = { 0., 0., 1., 1. };
  static int cur = 0;
  int i__1;

  /* Builtin functions */
  int s_copy (char *, char *, long int, long int);
  double rect[4];
  double xmin, ymin, xmax, ymax;
  char strf[40];
  int n, v;
  double dv;
  int nxname;
  char buf[40];
  int wid, iwd, nax[4], iwp;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     ipar(1) = win_num */
  /*     ipar(2) = 0/1 color flag */
  /*     ipar(3) = buffer size */
  /*     ipar(4) = dash,color or mark choice */
  /*     ipar(5) = line or mark size */
  /*     ipar(6) = mode : animated =0 fixed=1 */
  /*     ipar(7:8) = window position */
  /*     ipar(9:10) = window size */

  /*     rpar(1)=xmin */
  /*     rpar(2)=xmax */
  /*     rpar(3)=ymin */
  /*     rpar(4)=ymax */



  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;



  if (*flag__ == 6)
    {
      n = ipar[3];
      z__[2] = u[1];
      z__[n + 2] = u[2];
      z__[1] = 1.;
    }
  else if (*flag__ == 2)
    {
      wid = ipar[1];
      n = ipar[3];

      cur=set_scicos_win(wid);
      /*     shift buffer left */
      z__[(int) z__[1] + 2] = u[1];
      z__[n + 2 + (int) z__[1]] = u[2];
      z__[1] += 1.;
      if (z__[1] < (double) n)
	{
	  return 0;
	}
      /*     draw new point */
      if (ipar[4] < 0)
	{
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &n, &v,
		   &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
	  z__[1] = 0.;
	}
      else
	{
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &n, &v,
		   &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
	  z__[2] = z__[n + 1];
	  z__[n + 2] = z__[(n << 1) + 1];
	  z__[1] = 1.;
	}
    }
  else if (*flag__ == 4)
    {
      wid = ipar[1];
      n = ipar[3];
      xmin = rpar[1];
      xmax = rpar[2];
      ymin = rpar[3];
      ymax = rpar[4];
      nax[0] = 2;
      nax[1] = 10;
      nax[2] = 2;
      nax[3] = 10;
      scicos_sciwin ();
      cur=set_scicos_win(wid);  
      dr1_fort("xsetdr\000", "Rec\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
      iwp = 7;
      if (ipar[iwp] >= 0)
	{
	  dr1_fort("xset\000", "wpos\000", &ipar[iwp], &ipar[iwp + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      iwd = 9;
      if (ipar[iwd] >= 0)
	{
	  dr1_fort("xset\000", "wdim\000", &ipar[iwd], &ipar[iwd + 1], &v,
		   &v, &v, &v, &dv, &dv, &dv, &dv, 5L, 5L);
	}
      rect[0] = xmin;
      rect[1] = ymin;
      rect[2] = xmax;
      rect[3] = ymax;
      setscale2scicos_d (frect, rect, "nn\000", 3L);
      dr1_fort("xset\000", "use color\000", &ipar[2], &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "alufunction\000", &c__3, &c__0, &c__0, &c__0,
	       &c__0, &v, &dv, &dv, &dv, &dv, 5L, 12L);
      dr1_fort("xclear\000", "v\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 2L);
      scicos_dr ("xstart\000", "v\000", &wid, &v, &v, &v, &v, &v, &dv, &dv,
		 &dv, &dv, 7L, 2L);
      s_copy (buf, "t@ @input and output", 40L, 20L);
      s_copy (strf, "011\000", 40L, 4L);
      dr1_fort("xset\000", "thickness\000", &c__1, &v, &v, &v, &v, &v, &dv,
	       &dv, &dv, &dv, 5L, 10L);
      dr1_fort("xset\000", "dashes\000", &c__0, &c__0, &c__0, &c__0, &c__0,
	       &v, &dv, &dv, &dv, &dv, 5L, 7L);
      dr1_fort("xset\000", "alufunction\000", &c__3, &v, &v, &v, &v, &v,
	       &dv, &dv, &dv, &dv, 5L, 12L);
      nxname = 40;
      scicos_getlabel (&C2F(curblk).kfun, buf, &nxname, 40L);
      if (nxname > 39)
	{
	  nxname = 39;
	}
      i__1 = nxname;
      s_copy (buf + i__1, "\000", nxname + 1 - i__1, 1L);
      if ((nxname == 1 && *(unsigned char *) buf == ' ' )|| nxname == 0)
	{
	}
      else
	{
	  scicos_dr ("xname\000", buf, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
		     &dv, 6L, 40L);
	}
      plot2scicos_d (rect, &rect[1], &c__1, &c__1, &c_n1, strf, buf, rect,
		     nax, 40L, 40L);
      dr1_fort("xset\000", "thickness\000", &ipar[5], &v, &v, &v, &v, &v,
	       &dv, &dv, &dv, &dv, 5L, 10L);
      scicos_sxevents ();
      /* first point drawing */
      z__[1] = 0.;
    }
  else if (*flag__ == 5)
    {
      wid = ipar[1];
      n = ipar[3];

      cur=set_scicos_win(wid);
      if (ipar[4] < 0)
	{
	  i__1 = (int) z__[1];
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &i__1,
		   &v, &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
	}
      else
	{
	  i__1 = (int) z__[1];
	  dr1_fort("xpolys\000", "v\000", &v, &v, &ipar[4], &c__1, &i__1,
		   &v, &z__[2], &z__[n + 2], &dv, &dv, 7L, 2L);
	}
      z__[1] = 0.;
      dr1_fort("xset\000", "alufunction\000", &c__3, &v, &v, &v, &v, &v,
	       &dv, &dv, &dv, &dv, 5L, 12L);
    }
  return 0;
}			




int scicos_selblk (scicos_args_poo)
{
  int ic, nev;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Selector block */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 2 && *nevprt > 0)
    {
      ic = 0;
      nev = *nevprt;
    L10:
      if (nev >= 1)
	{
	  ++ic;
	  nev /= 2;
	  goto L10;
	}
      z__[1] = (double) ic;
    }
  else if (*flag__ == 1 || *flag__ == 6)
    {
      y[1] = u[(int) z__[1]];
    }
  return 0;
}			



int scicos_sinblk (scicos_args_poo)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      y[i__] = sin (u[i__]);
      /* L15: */
    }
  return 0;
}			



int scicos_sqrblk (scicos_args_poo)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      if (u[i__] >= 0.)
	{
	  y[i__] = sqrt (u[i__]);
	}
      else
	{
	  *flag__ = -2;
	  return 0;
	}
      /* L15: */
    }
  return 0;
}			



int sum2_fort(int *flag__, int *nevprt, double *t, double *xd, double *x,
	      int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	      double *rpar, int *nrpar, int *ipar, int *nipar, double *u1,
	      int *nu1, double *u2, int *nu2, double *y, int *ny)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     adds the inputs weighed by rpar */
  --y;
  --u2;
  --u1;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      y[i__] = u1[i__] * rpar[1] + u2[i__] * rpar[2];
      /* L1: */
    }
  return 0;
}			



int sum3_fort(int *flag__, int *nevprt, double *t, double *xd, double *x,
	      int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	      double *rpar, int *nrpar, int *ipar, int *nipar, double *u1,
	      int *nu1, double *u2, int *nu2, double *u3, int *nu3, double *y,
	      int *ny)
{
  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     adds the inputs weighed by rpar */
  --y;
  --u3;
  --u2;
  --u1;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu1;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      y[i__] = u1[i__] * rpar[1] + u2[i__] * rpar[2] + u3[i__] * rpar[3];
      /* L1: */
    }
  return 0;
}			



int scicos_tanblk (scicos_args_poo)
{
  int i__1;
  int i__;
  double ww;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      ww = cos (u[i__]);
      if (ww != 0.)
	{
	  y[i__] = sin (u[i__]) / ww;
	}
      else
	{
	  *flag__ = -2;
	  return 0;
	}
      /* L15: */
    }
  return 0;
}			



/* Table of constant values */



int
scicos_tcslti (int *flag__, int *nevprt, double *t, double *xd, double *x,
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,
	       double *rpar, int *nrpar, int *ipar, int *nipar, double *u1,
	       int *nu1, double *u2, int *nu2, double *y, int *ny)
{
  const  int c__1 = 1;
  int la, lb, lc, ld;

  /*     Scicos block simulator */
  /*     continuous state space linear system simulator */
  /*     rpar(1:nx*nx)=A */
  /*     rpar(nx*nx+1:nx*nx+nx*nu)=B */
  /*     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C */
  /*     rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D */
  /* ! */


  --y;
  --u2;
  --u1;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  la = 1;
  lb = *nx * *nx + la;
  lc = lb + *nx * *nu1;
  if (*flag__ == 1 || *flag__ == 6)
    {
      /*     y=c*x+d*u1 */
      ld = lc + *nx * *ny;
      scicos_dmmul (&rpar[lc], ny, &x[1], nx, &y[1], ny, ny, nx, &c__1);
      dmmul1_fort(&rpar[ld], ny, &u1[1], nu1, &y[1], ny, ny, nu1, &c__1);
    }
  else if (*flag__ == 2 && *nevprt == 1)
    {
      /*     x+=u2 */
      scicos_dcopy (nx, &u2[1], &c__1, &x[1], &c__1);
    }
  else if (*flag__ == 0 && *nevprt == 0)
    {
      /*     xd=a*x+b*u1 */
      scicos_dmmul (&rpar[la], nx, &x[1], nx, &xd[1], nx, nx, nx, &c__1);
      dmmul1_fort(&rpar[lb], nx, &u1[1], nu1, &xd[1], nx, nx, nu1, &c__1);
    }
  return 0;
}			



/* Table of constant values */




#define scicos_args_poo int *flag__, int *nevprt, double *t, double *xd, double *x, \
	       int *nx, double *z__, int *nz, double *tvec, int *ntvec,\
	       double *rpar, int *nrpar, int *ipar, int *nipar, double *u,\
	       int *nu, double *y, int *ny 


int scicos_tcsltj (scicos_args_poo) 
{
  static int c__1 = 1;
  int la, lb, lc;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     continuous state space linear system simulator */
  /*     rpar(1:nx*nx)=A */
  /*     rpar(nx*nx+1:nx*nx+nx*nu)=B */
  /*     rpar(nx*nx+nx*nu+1:nx*nx+nx*nu+nx*ny)=C */
  /*     rpar(nx*nx+nx*nu+nx*ny+1:nx*nx+nx*nu+nx*ny+ny*nu)=D */
  /* ! */

  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  la = 1;
  lb = *nx * *nx + la;
  lc = lb;
  if (*flag__ == 1 || *flag__ == 6)
    {
      /*     y=c*x */
      scicos_dmmul (&rpar[lc], ny, &x[1], nx, &y[1], ny, ny, nx, &c__1);
    }
  else if (*flag__ == 2 && *nevprt == 1)
    {
      /*     x+=u2 */
      scicos_dcopy (nx, &u[1], &c__1, &x[1], &c__1);
    }
  else if (*flag__ == 0 && *nevprt == 0)
    {
      /*     xd=a*x */
      scicos_dmmul (&rpar[la], nx, &x[1], nx, &xd[1], nx, nx, nx, &c__1);
    }
  return 0;
}			


int scicos_timblk (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  y[0] = *t;
  return 0;
}



int scicos_trash (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  return 0;
}			


int  scicos_zcross (scicos_args_poo) 
{
  int i__1;
  double d__1;
  int i__, j, l, kev;

  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     zero crossing block */


  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 3 && *nevprt < 0)
    {
      kev = 0;
      i__1 = *ny;
      for (j = 1; j <= i__1; ++j)
	{
	  kev = (int) ((kev << 1) + (d__1 = y[*ny + 1 - j], Abs (d__1)));
	  /* L44: */
	}
      i__1 = *ny;
      for (j = 1; j <= i__1; ++j)
	{
	  kev <<= 1;
	  if (y[*ny + 1 - j] == -1.)
	    {
	      ++kev;
	    }
	  /* L45: */
	}
      l = kev * *ntvec;
      i__1 = *ntvec;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  tvec[i__] = rpar[l + i__] + *t;
	  /* L10: */
	}
    }
  else if (*flag__ == 9)
    {
      i__1 = *ny;
      for (i__ = 1; i__ <= i__1; ++i__)
	{
	  y[i__] = u[i__];
	  /* L20: */
	}
    }
  return 0;
}				/* zcross_ */








void 
plusblk(int *flag, int *nevprt, double *t, double *xd, double *x,
	int *nx, double *z, int *nz, double *tvec, int *ntvec, 
	double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr, 
	int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  int k,i,n;
  double *y;
  double *u;

  n=outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

  y=(double *)outptr[0];

  for (i=0;i<n;i++) {
    y[i]=0.0;
    for (k=0;k<*nin;k++) {
      u=(double *)inptr[k];
      y[i]=y[i]+u[i];
    }
  }
}


void  scicos_plusblk(int *flag, int *nevprt, double *t, double *xd, double *x,
		     int *nx, double *z, int *nz, double *tvec, int *ntvec,
		     double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
		     int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  int k,i,n;
  double *y;
  double *u;

  n=outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

  y=(double *)outptr[0];

  for (i=0;i<n;i++) {
    y[i]=0.0;
    for (k=0;k<*nin;k++) {
      u=(double *)inptr[k];
      y[i]=y[i]+u[i];
    }
  }
}
/* Copyright INRIA */
#include "nsp/machine.h"

void switchn(int *flag, int *nevprt, double *t, double *xd, double *x,
	     int *nx, double *z, int *nz, double *tvec, int *ntvec,
	     double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
	     int *insz, int *nin, double **outptr, int *outsz, int *nout)
{

  int k;
  double *y;
  double *u;
  int /*nev,*/ic;
  ic=ipar[0];
  if (*nin>1) {
    y=(double *)outptr[0];
    u=(double *)inptr[ic];
    for (k=0;k<outsz[0];k++)
      *(y++)=*(u++);  
  }
  else {
    y=(double *)outptr[ic];
    u=(double *)inptr[0];
    for (k=0;k<outsz[0];k++)
      *(y++)=*(u++);  
  }
}


/* Copyright INRIA */
#include "nsp/machine.h"

void 
selector(int *flag, int *nevprt, double *t, double *xd, double *x,
	 int *nx, double *z, int *nz, double *tvec, int *ntvec,
	 double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
	 int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  int k;
  double *y;
  double *u;
  int nev,ic;
    
  ic=(int)z[0];
  if ((*flag)<3) {
    ic=-1;
    nev=*nevprt;
    while (nev>=1) {
      ic=ic+1;
      nev=nev/2;
    }
  }
  if (*nin>1) {
    y=(double *)outptr[0];
    u=(double *)inptr[ic];
    for (k=0;k<outsz[0];k++)
      *(y++)=*(u++);  
  }
  else {
    y=(double *)outptr[ic];
    u=(double *)inptr[0];
    for (k=0;k<outsz[0];k++)
      *(y++)=*(u++);  
  }
}

/* Copyright INRIA */

#include "nsp/machine.h"

void 
relay(int *flag, int *nevprt, double *t, double *xd, double *x,
      int *nx, double *z, int *nz, double *tvec, int *ntvec,
      double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
      int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  int k;
  double *y;
  double *u;
  int nev,ic;
  ic=(int)z[0];
  if ((*flag)<3) {
    if ((*nevprt)>0) {
      ic=-1;
      nev=*nevprt;
      while (nev>=1) {
	ic=ic+1;
	nev=nev/2;
      }
    }
    if ((*flag)==2) {z[0]=ic;return ;}
    if (*nin>1) {
      y=(double *)outptr[0];
      u=(double *)inptr[ic];
      for (k=0;k<outsz[0];k++)
	*(y++)=*(u++);  
    }
    else {
      y=(double *)outptr[ic];
      u=(double *)inptr[0];
      for (k=0;k<outsz[0];k++)
	*(y++)=*(u++);  
    }
  }
}


/* Copyright INRIA */
#include "nsp/machine.h"

/* Element wise product */

void 
prod(int *flag, int *nevprt, double *t, double *xd, double *x,
     int *nx, double *z, int *nz, double *tvec, int *ntvec,
     double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
     int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  int k,i,n;
  double *y;
  double *u;
    
  n=outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

  y=(double *)outptr[0];

  for (i=0;i<n;i++) {
    y[i]=1.0;
    for (k=0;k<*nin;k++) {
      u=(double *)inptr[k];
      y[i]=y[i]*u[i];
    }
  }
}
/* Copyright INRIA */
#include "nsp/machine.h"

/* Element wise som 
 * type 2 
 */

void 
sum(int *flag, int *nevprt, double *t, double *xd, double *x,
    int *nx, double *z, int *nz, double *tvec, int *ntvec,
    double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
    int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  int k,i,n;
  double *y;
  double *u;

  n=outsz[0]; /* insz[0]==insz[1] .. ==insz[*nin]== outsz[0] */

  y=(double *)outptr[0];

  for (i=0;i<n;i++) {
    y[i]=0.0;
    for (k=0;k<*nin;k++) {
      u=(double *)inptr[k];
      y[i]=y[i]+u[i]*rpar[k];
    }
  }
}
/* Copyright INRIA */
#include "nsp/machine.h"

void 
zcross2(int *flag, int *nevprt, double *t, double *xd, double *x,
	int *nx, double *z, int *nz, double *tvec, int *ntvec,
	double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *g, int *ng)
{
  
  int i,j;
  int surface_matched,exist_enabled_surface;

  exist_enabled_surface=0;
  if ((*flag==3) &&(*nevprt<0)){
    for(i=0;i<*ntvec;i++){
      surface_matched=1;
      exist_enabled_surface=0;
      
      for (j=0;j<*ng;j++){
	if (rpar[(*ng+1)*i+j]!=0){
	  exist_enabled_surface=1;
	  if((rpar[(*ng+1)*i+j]*g[j])<=0){
	    surface_matched=0;
	  }
	}
      }
      
      if(( surface_matched==1)&&(exist_enabled_surface ==1))
	tvec[i]=*t+rpar[(*ng+1)*i+*ng];
      else
	tvec[i]=-1;
      
    }
  }
  else{
    if(*flag==9){
      for(i=0;i<*ng;i++)
	g[i]=u[i];
    }
  }
}

int scicos_bound (scicos_args_poo)
{

  int i__1;
  int i__;
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Bound y(i)=rpar(nu+i) if u(i)>rpar(i) else y(i)=0 */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  i__1 = *nu;
  for (i__ = 1; i__ <= i__1; ++i__)
    {
      if (u[i__] >= rpar[i__])
	{
	  y[i__] = rpar[*nu + i__];
	}
      else
	{
	  y[i__] = 0.;
	}
      /* L15: */
    }
  return 0;
}



void  readau(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
	     ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     /*
       ipar[1]   = lfil : file name length
       ipar[2:4] = fmt  : numbers type ascii code
       ipar[5]   = void
       ipar[6]   = n : buffer length in number of records
       ipar[7]   = maxvoie : record size
       ipar[8]   = swap
       ipar[9]   = first : first record to read
       ipar[10:9+lfil] = character codes for file name
     */
     int *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar;
     int insz[],*nin,outsz[],*nout;
     double x[],xd[],z[],tvec[],rpar[];
     double *inptr[],*outptr[],*t;

{
  char str[100],type[4];
  int job = 1,three=3;
  FILE *fd;
  int n, k, kmax, /*no, lfil,*/ m, i, irep,/* nm,*/ ierr;
  double *buffer,*record;
  long offset;
  double y;
  /*  div_t divt;*/int quot, rem;
  double SCALE=0.000030517578125;
  /*  int ETAB[8]={0, 132, 396, 924, 1980, 4092, 8316, 16764}; */
  int ETAB[8];
  int mu;
  int sig;
  int e;
  int f;
  /*  double ff;*/

  ETAB[0]=0; ETAB[1]=132; ETAB[2]= 396; ETAB[3]=924; ETAB[4]=1980;
  ETAB[5]=4092; ETAB[6]=8316; ETAB[7]=16764;

  --ipar;
  --z;
  fd=(FILE *)(long)z[3];
  buffer = (z+4);
    
  /*
    k    : record counter within the buffer
    kmax :  number of records in the buffer
  */

  if (*flag==1) {
    n    = ipar[6];
    k    = (int)z[1];
    /* copy current record to output */
    record=buffer+(k-1)*ipar[7];

    for (i=0;i<*nout;i++)
      {
	mu=(int) record[i];

	mu=255-mu;
	if(mu>127)
	  sig=1;
	else
	  sig=0;
	/* comment out for SUNOS SS 8/10/99 
	   divt=div(mu,16);
	   e=divt.quot-8*sig+1;
	   f=divt.rem;
	*/
	quot=mu/16;rem=mu-16*quot;
	e=quot-8*sig+1;
	f=rem;

	y=ldexp((double)(f),(e+2));
	/* ff=(double)(e+2);
	   y=((double) f) * pow(two, ff); */

	e=ETAB[e-1];

	y=SCALE*(1-2*sig)*(e+y);

	*outptr[i]=y;
      }
    if (*nevprt>0) {
      /*     discrete state */
      kmax =(int) z[2];
      if (k>=kmax&&kmax==n) {
	/*     read a new buffer */
	m=ipar[6]*ipar[7];
	F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
	for (i=2;i>=0;i--)
	  if (type[i]!=' ') { type[i+1]='\0';break;}
	ierr=0;
	mget2(fd,ipar[8],buffer,m,type,&ierr);
	if (ierr>0) {
	  sciprint("Read error!\n");
	  fclose(fd);
	  z[3] = 0.0;
	  *flag = -1;
	  return;
	}
	else if (ierr<0) { /* EOF reached */
	  kmax=-(ierr+1)/ipar[7];
	}
	else
	  kmax=ipar[6];

	z[1] = 1.0;
	z[2] = kmax;
      }
      else if (k<kmax) 
	z[1] = z[1]+1.0;
    }
  }
  else if (*flag==4) {
    F2C(cvstr)(&(ipar[1]),&(ipar[10]),str,&job,strlen(str));
    str[ipar[1]] = '\0';
    fd = fopen(str,"rb");
    if (!fd ) {
      sciprint("Could not open the file!\n");
      *flag = -1;
      return;
    }
    z[3]=(long)fd;
    /* skip first records */
    if (ipar[9]>1) {
      F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
      for (i=2;i>=0;i--)
	if (type[i]!=' ') { type[i+1]='\0';break;}
      offset=(ipar[9]-1)*ipar[7]*sizeof(char);
      irep = fseek(fd,offset,0) ;
      if ( irep != 0 ) 
	{
	  sciprint("Read error\r\n");
	  *flag = -1;
	  fclose(fd);
	  z[3] = 0.0;
	  return;
	}
    }
    /* read first buffer */
    m=ipar[6]*ipar[7];
    F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
    for (i=2;i>=0;i--)
      if (type[i]!=' ') { type[i+1]='\0';break;}
    mget2(fd,ipar[8],buffer,m,type,&ierr);
    if (ierr>0) {
      sciprint("Read error!\n");
      *flag = -1;
      fclose(fd);
      z[3] = 0.0;
      return;
    }
    else if (ierr<0) { /* EOF reached */
      kmax=-(ierr+1)/ipar[7];
    }
    else
      kmax=ipar[6];

    z[1] = 1.0;
    z[2] = kmax;
  }
  else if (*flag==5) {
    if(z[3]==0) return;
    fclose(fd);
    z[3] = 0.0;
  }
  return;
}



int worldsize(type)
     char type[4];
{
  char c;
  int wsz;

  c=type[0];
  if (c=='u') c=type[1];
  switch ( c )
    {
    case 'l' : wsz=sizeof(long);
      break;
    case 's' : wsz=sizeof(short);
      break;
    case 'c' : wsz=sizeof(char);
      break;
    case 'd' : wsz=sizeof(double);
      break;
    case 'f' : wsz=sizeof(float); 
      break;
    }
  return wsz;
}

void 
readc(flag,nevprt,t,xd,x,nx,z,nz,tvec,ntvec,rpar,nrpar,
      ipar,nipar,inptr,insz,nin,outptr,outsz,nout)
     /*
       ipar[1]   = lfil : file name length
       ipar[2:4] = fmt  : numbers type ascii code
       ipar[5]   = is there a time record
       ipar[6]   = n : buffer length in number of records
       ipar[7]   = maxvoie : record size
       ipar[8]   = swap
       ipar[9]   = first : first record to read
       ipar[10:9+lfil] = character codes for file name
       ipar[10+lfil:9+lfil++ny+ievt] = reading mask
     */
     int *flag,*nevprt,*nx,*nz,*ntvec,*nrpar,ipar[],*nipar;
     int insz[],*nin,outsz[],*nout;
     double x[],xd[],z[],tvec[],rpar[];
     double *inptr[],*outptr[],*t;

{
  char str[100],type[4];
  int job = 1,three=3;
  FILE *fd;
  int n, k, ievt, kmax,/* no,*//* lfil,*/ m, i, irep,/* nm,*/ ierr;
  double *buffer,*record;
  int *mask;
  long offset;
  

  --ipar;
  --z;
  fd=(FILE *)(long)z[3];
  buffer = (z+4);
  mask = ipar+11+ipar[1]-ipar[5];
    
  /*
    k    : record counter within the buffer
    kmax :  number of records in the buffer
  */

  if (*flag==1) {
    n    = ipar[6];
    ievt = ipar[5];
    k    = (int)z[1];
    /* copy current record to output */
    record=buffer+(k-1)*ipar[7]-1;

    for (i=0;i<outsz[0];i++)
      *(outptr[0]+i)=record[mask[ievt+i]];

    if (*nevprt>0) {
      /*     discrete state */
      kmax = (int)z[2];
      if (k>=kmax&&kmax==n) {
	/*     read a new buffer */
	m=ipar[6]*ipar[7];
	F2C(cvstr)(&three,&(ipar[2]),type,&job, strlen(type));
	for (i=2;i>=0;i--)
	  if (type[i]!=' ') { type[i+1]='\0';break;}
	ierr=0;
	mget2(fd,ipar[8],buffer,m,type,&ierr);
	if (ierr>0) {
	  sciprint("Read error!\n");
	  fclose(fd);
	  z[3] = 0.0;
	  *flag = -1;
	  return;
	}
	else if (ierr<0) { /* EOF reached */
	  kmax=-(ierr+1)/ipar[7];
	}
	else
	  kmax=ipar[6];

	z[1] = 1.0;
	z[2] = kmax;
      }
      else if (k<kmax) 
	z[1] = z[1]+1.0;
    }
  }
  else if (*flag==3) {
    ievt = ipar[5];
    n    = ipar[6];
    k    = (int)z[1];
    kmax = (int) z[2];
    if (k > kmax && kmax < n) {
      if(ievt) 
	tvec[0] = *t-1.0;
      else
	tvec[0] = *t*(1.0+0.0000000001);
    }
    else {
      record=buffer+(k-1)*ipar[7]-1;
      if(ievt) tvec[0] = record[mask[0]];
    }
  }
  else if (*flag==4) {
    F2C(cvstr)(&(ipar[1]),&(ipar[10]),str,&job,strlen(str));
    str[ipar[1]] = '\0';
    fd = fopen(str,"rb");
    if (!fd ) {
      sciprint("Could not open the file!\n");
      *flag = -1;
      return;
    }
    z[3]=(long)fd;
    /* skip first records */
    if (ipar[9]>1) {
      F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
      for (i=2;i>=0;i--)
	if (type[i]!=' ') { type[i+1]='\0';break;}
      offset=(ipar[9]-1)*ipar[7]*worldsize(type);
      irep = fseek(fd,offset,0) ;
      if ( irep != 0 ) 
	{
	  sciprint("Read error\r\n");
	  *flag = -1;
	  fclose(fd);
	  z[3] = 0.0;
	  return;
	}
    }
    /* read first buffer */
    m=ipar[6]*ipar[7];
    F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
    for (i=2;i>=0;i--)
      if (type[i]!=' ') { type[i+1]='\0';break;}
    mget2(fd,ipar[8],buffer,m,type,&ierr);
    if (ierr>0) {
      sciprint("Read error!\n");
      *flag = -1;
      fclose(fd);
      z[3] = 0.0;
      return;
    }
    else if (ierr<0) { /* EOF reached */
      kmax=-(ierr+1)/ipar[7];
    }
    else
      kmax=ipar[6];

    z[1] = 1.0;
    z[2] = kmax;
  }
  else if (*flag==5) {
    if(z[3]==0) return;
    fclose(fd);
    z[3] = 0.0;
  }
  return;
}


int
scicos_sctree (int *nb, int *vec, int *in, int *depu, int *outptr, int *cmat,
	       int *ord, int *nord, int *ok, int *kk)
{

  int i__1, i__2, i__3;
  int fini;
  int i__, j, l;
  int ii, lkk;

  /*     inputs: */
  /*     nb: number of regular blocks */
  /*     vec: int vector of size nb */
  /*     in: int vector */
  /*     depu: int vector, first column of dep_ut */
  /*     outptr: int vector */
  /*     cmat: int vector */
  /*     kk: int work area of size nb */

  /*     outputs: */
  /*     ok: int */
  /*     ord: int vector of size nord (=<nb) */
  /*     nord */
  /*     Copyright INRIA */


  --kk;
  --vec;
  --in;
  --depu;
  --outptr;
  --cmat;
  --ord;
  *ok = 1;
  i__1 = *nb + 2;
  for (j = 1; j <= i__1; ++j)
    {
      fini = TRUE;
      i__2 = *nb;
      for (i__ = 1; i__ <= i__2; ++i__)
	{
	  if (vec[i__] == j - 1)
	    {
	      if (j == *nb + 2)
		{
		  *ok = 0;
		  return 0;
		}
	      lkk = 0;
	      i__3 = outptr[i__ + 1] - 1;
	      for (l = outptr[i__]; l <= i__3; ++l)
		{
		  ii = in[cmat[l]];
		  if (depu[ii] == 1)
		    {
		      ++lkk;
		      kk[lkk] = ii;
		    }
		  /* L40: */
		}
	      if (lkk > 0)
		{
		  fini = FALSE;
		  i__3 = lkk;
		  for (l = 1; l <= i__3; ++l)
		    {
		      vec[kk[l]] = j;
		      /* L45: */
		    }
		}
	    }
	  /* L50: */
	}
      if (fini)
	{
	  goto L65;
	}
      /* L60: */
    }
 L65:
  i__1 = *nb;
  for (l = 1; l <= i__1; ++l)
    {
      kk[l] = -vec[l];
      /* L70: */
    }
  scicos_isort (&kk[1], nb, &ord[1]);
  *nord = 0;
  i__1 = *nb;
  for (l = 1; l <= i__1; ++l)
    {
      if (kk[l] != 1 && outptr[ord[l] + 1] - outptr[ord[l]] != 0)
	{
	  ++(*nord);
	  ord[*nord] = ord[l];
	}
      /* L80: */
    }
  return 0;
}				


/*
 * Copyright Enpc jpc 
 */

extern int C2F(getgeom)(double *);

/*----------------------------------------------------
 * erase a rectangle 
 *----------------------------------------------------*/ 

static void block_draw_rect_1(double r[],double percent)
{
  int alumode,verbose=0,narg;
  static int in=6;
  double w = r[2]*percent;
  double x = r[0];
  C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&in,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xfrect","v",PI0,PI0,PI0,PI0,PI0,PI0,&x,r+1,&w,r+3,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}

static void block_draw_rect_2(double r[],double percent)
{
  int alumode,verbose=0,narg;
  /* int flag =0;  double ang=0.0;  char foo[24]; */
  static int in=6;
  double x = r[0]+ r[2]*percent-2;
  double w = 4;
  C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&in,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  C2F(dr1)("xfrect","v",PI0,PI0,PI0,PI0,PI0,PI0,&x,r+1,&w,r+3,0L,0L);
  /* sprintf(foo,"%5.3f",percent);
     C2F(dr1)("xstring",foo,PI0,PI0,PI0,&flag,PI0,PI0,r,r+1,&ang,PD0,0L,0L);
  */

  if ( alumode != in ) C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}


static void block_draw_rect_3(double r[],double percent)
{
  int alumode,verbose=0,narg;
  int flag =0;  double ang=0.0;  char foo[24];
  static int in=6;
  double x = r[0] + (1.0/10)*r[2];
  double y = r[1] - r[3] + 2 ;
  C2F(dr)("xget","alufunction",&verbose,&alumode,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&in,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  sprintf(foo,"%5.3f",percent);
  C2F(dr1)("xstring",foo,PI0,PI0,PI0,&flag,PI0,PI0,&x,&y,&ang,PD0,0L,0L);
  if ( alumode != in ) C2F(dr1)("xset","alufunction",&alumode,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
}




/*----------------------------------------------------
 * switch to X11 driver  
 *----------------------------------------------------*/ 

#define REMOVE_REC_DRIVER() char old_rec[4]; int rem_flag ; rem_flag = scig_driverX11(old_rec); 
#define RESTORE_DRIVER() if (rem_flag == 1) C2F(SetDriver)(old_rec,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);

static int scig_driverX11(char *old) 
{
  GetDriver1(old,PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
  if ( old[0] == 'R' )
    {
      C2F(SetDriver)("X11",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0);
      return 1;
    }
  return 0;
}

/*----------------------------------------------------
 * changes the current window to win 
 * returns the old current window number 
 *----------------------------------------------------*/ 

static int set_block_win( int win)
{
  static int verbose = 0, cur,narg;
  C2F(dr)("xget","window",&verbose,&cur,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( cur != win ) 
    C2F(dr)("xset","window",&win,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return cur ;
}

/*----------------------------------------------------
 * changes the current color to color 
 * returns the old color
 *----------------------------------------------------*/ 


static int set_slider_color( int color)
{
  static int verbose = 0, cur,narg;
  C2F(dr)("xget","color",&verbose,&cur,&narg,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  if ( cur != color ) 
    C2F(dr)("xset","color",&color,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);
  return cur ;
}

/*----------------------------------------------------
 * follow an input value with a graphic slider 
 * 
 *  rpar = [min-range, max-range] 
 *  ipar = [type (1,2),color]
 *  z = [val,window,x,y,w,h] 
 *----------------------------------------------------*/

void slider(int *flag, int *nevprt, double *t, double *xd,
	    double *x, int *nx, double *z, int *nz,
	    double *tvec, int *ntvec, double *rpar,
	    int *nrpar, int *ipar, int *nipar,
	    double * u, int *nu, double * y, int *ny) 
{
  
  int wid, idb = 0 ; /* XXX remettre idb avec son common */
  int cur; 
  static double th=2 ; /* border thickness */
  static double t3d = 4.0 ; /* 3d look thickness */
  int curcolor;
  if ( idb == 1 ) 
    sciprint("Slider t=%10.3f, flag=%d \r\n",*t,*flag);

  switch ( *flag ) {
  case 2 : 
    /* standard case */ 
    wid= (int) z[1];
    if( wid < 0) return;
    cur = set_block_win(wid) ; 
    {
      double val = Min(rpar[1],Max(rpar[0],u[0]));
      double percent = (val - rpar[0])/(rpar[1]-rpar[0]);
      if ( Abs(z[0] - percent) > 0.01 ) /* a mettre en parametre XXXXX */
	{
	  REMOVE_REC_DRIVER();
	  curcolor=set_slider_color(ipar[1]);
	  switch (ipar[0]) 
	    {
	    case 1 : 
	      block_draw_rect_1(z+2,z[0]);
	      block_draw_rect_1(z+2,percent);
	      break;
	    case 2 :
	      block_draw_rect_2(z+2,z[0]);
	      block_draw_rect_2(z+2,percent);
	      break;
	    case 3 :
	      block_draw_rect_3(z+2,z[0]);
	      block_draw_rect_3(z+2,percent);
	      break;
	    }
	  curcolor=set_slider_color(curcolor);
	  z[0] = percent;
	  RESTORE_DRIVER();
	}
    }
    set_block_win(cur);
    break;
  case 4 : 
    /* initial case */ 
    z[0]= 0.0;
    C2F(getgeom)(z+1);
    z[2] = z[2]+ t3d +th ; 
    z[4] -= t3d + 2*th  ;
    z[3] = z[3] + z[5] ;
    z[5] -= t3d  ;
    wid= (int) z[1];
    if( wid < 0) return;
    cur = set_block_win(wid) ; 
    {
      REMOVE_REC_DRIVER();
      curcolor=set_slider_color(ipar[1]);
      C2F(dr1)("xclea","v",PI0,PI0,PI0,PI0,PI0,PI0,z+2,z+3,z+4,z+5,0L,0L);
      switch (ipar[0]) 
	{
	case 1 : 
	  block_draw_rect_1(z+2,z[0]);
	  break;
	case 2 :
	  block_draw_rect_2(z+2,z[0]);
	  break;
	case 3 :
	  block_draw_rect_3(z+2,z[0]);
	  break;

	}
      curcolor=set_slider_color(curcolor);
      RESTORE_DRIVER();
    }
    set_block_win(cur);
    break;
  }
}


void writeau(int *flag, int *nevprt, double *t, double *xd, double *x,
	     int *nx, double *z, int *nz, double *tvec, int *ntvec,
	     double *rpar, int *nrpar, int *ipar, int *nipar, double **inptr,
	     int *insz, int *nin, double **outptr, int *outsz, int *nout)
{
  /*
    ipar[1]   = lfil : file name length
    ipar[2:4] = fmt  : numbers type ascii code
    ipar[5]   = n : buffer length in number of records
    ipar[6]   = swap
    ipar[7:6+lfil] = character codes for file name
  */
  FILE *fd;
  int n, k,/* m,*/ i, ierr;
  double *buffer,*record;
  /*  long offset;*/
  int SCALE  = 32768;
  int BIAS   =   132;
  int CLIP   = 32635;
  int OFFSET =   335;
  double y;
  int sig;
  int e;
  double f;
  --ipar;
  --z;
  fd=(FILE *)(long)z[2];
  buffer = (z+3);
  ierr=0;
  /*
    k    : record counter within the buffer
  */

  if (*flag==2&&*nevprt>0) 
    { /* add a new record to the buffer */
      n    = ipar[5];
      k    = (int)z[1];
      /* copy current record to output 
	 printf("%i\n",k);*/
      record=buffer+(k-1)*(*nin); 

      for (i=0;i<*nin;i++)
	{
	  y= *inptr[i];
	  y=SCALE*y;
	  if (y<0.0)
	    {
	      y=-y;
	      sig=-1;
	    }
	  else
	    sig=1;
	  if(y>CLIP)
	    y=CLIP;
	  y=y+BIAS;
	  f=frexp(y,&e);
	  y=64*sig-16*e- (int) (32*f)+OFFSET;
	  record[i] = y;
	}
      if (k<n) 
	z[1] = z[1]+1.0;
      else {
	mput2(fd,ipar[6],buffer,ipar[5]*(*nin),"uc",&ierr); 
	if(ierr!=0) {
	  *flag = -3;
	  return;
	}
	z[1] = 1.0;
	
      }
      
    }
  else if (*flag==4) {
    fd = fopen("/dev/audio","wb");
    if (!fd ) {
      sciprint("Could not open /dev/audio!\n");
      *flag = -3;
      return;
    }
    z[2]=(double)(long)fd;
    z[1] = 1.0;
  }
  else if (*flag==5) {
    if(z[2]==0) return;
    k    =(int) z[1];
    if (k>1) {/* flush rest of buffer */
      mput2(fd,ipar[6],buffer,(k-1)*(*nin),"uc",&ierr);
      if(ierr!=0) {
	*flag = -3;
	return;
      }
    }
    fclose(fd);
    z[2] = 0.0;
  }
  return;
}


void 
writec(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, 
       double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, 
       int *ipar, int *nipar, double **inptr, int *insz, int *nin, double **outptr, 
       int *outsz, int *nout)
{
  /*
    ipar[1]   = lfil : file name length
    ipar[2:4] = fmt  : numbers type ascii code
    ipar[5]   = n : buffer length in number of records
    ipar[6]   = swap
    ipar[7:6+lfil] = character codes for file name
  */
  char str[100],type[4];
  int job = 1,three=3;
  FILE *fd;
  int n, k,/* m,*/ i, ierr;
  double *buffer,*record;
  /*  long offset;*/
  --ipar;
  --z;
  fd=(FILE *)(long)z[2];
  buffer = (z+3);
  ierr=0;
  /*
    k    : record counter within the buffer
  */

  if (*flag==2&&*nevprt>0) { /* add a new record to the buffer */
    n    = ipar[5];
    k    = (int)z[1];
    /* copy current record to output */
    record=buffer+(k-1)*(insz[0]);

    for (i=0;i<insz[0];i++)
      record[i] = *(inptr[0]+i);
    if (k<n) 
      z[1] = z[1]+1.0;
    else {/* buffer is full write it to the file */
      F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
      for (i=2;i>=0;i--)
	if (type[i]!=' ') { type[i+1]='\0';break;}
      mput2(fd,ipar[6],buffer,ipar[5]*insz[0],type,&ierr);
      if(ierr!=0) {
	*flag = -3;
	return;
      }
      z[1] = 1.0;
    }
  }
  else if (*flag==4) {
    F2C(cvstr)(&(ipar[1]),&(ipar[7]),str,&job,strlen(str));
    str[ipar[1]] = '\0';
    fd = fopen(str,"wb");
    if (!fd ) {
      sciprint("Could not open the file!\n");
      *flag = -3;
      return;
    }
    z[2]=(long)fd;
    z[1] = 1.0;
  }
  else if (*flag==5) {
    if(z[2]==0) return;
    k    =(int) z[1];
    if (k>=1) {/* flush rest of buffer */
      F2C(cvstr)(&three,&(ipar[2]),type,&job,strlen(type));
      for (i=2;i>=0;i--)
	if (type[i]!=' ') { type[i+1]='\0';break;}
      mput2(fd,ipar[6],buffer,(k-1)*insz[0],type,&ierr);
      if(ierr!=0) {
	*flag = -3;
	return;
      }
    }
    fclose(fd);
    z[2] = 0.0;
  }
  return;
}


int scicos_affich (scicos_args_poo)
{
  /*     Copyright INRIA */
  /*     Scicos block simulator */
  /*     Displays the value of the input in a graphic window */
  static double c_b2 = 10.;
  static int cur = 0;
  int i__1;
  double d__1;
  int v;
  double dv, ur;
  int wid;
  char drv[40];
  /*     ipar(1) = font */
  /*     ipar(2) = fontsize */
  /*     ipar(3) = color */
  /*     ipar(4) = win */
  /*     ipar(5) = nt : total number of output digits */
  /*     ipar(6) = nd number of rationnal part digits */

  /*     z(1)=value */
  /*     w(2)=window */
  /*     z(3)=x */
  /*     z(4)=y */
  /*     z(5)=width */
  /*     z(6)=height */
  --y;
  --u;
  --ipar;
  --rpar;
  --tvec;
  --z__;
  --x;
  --xd;
  if (*flag__ == 2)
    {
      /*     state evolution */
      ur = pow_di (&c_b2, &ipar[6]);
      d__1 = u[1] * ur;
      ur = scicos_round (&d__1) / ur;
      if (ur == z__[1])
	{
	  return 0;
	}
      wid = (int) z__[2];
      if (wid < 0)
	{
	  return 0;
	}
      i__1 = (int) z__[2];
      scicos_setblockwin (&i__1, &cur);
      dr1_fort("xgetdr\000", drv, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	       &dv, 7L, 40L);
      dr1_fort("xsetdr\000", "X11\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
      scicos_recterase (&z__[3]);
      z__[1] = ur;
      scicos_affdraw (&ipar[1], &ipar[5], &z__[1], &z__[3]);
      dr1_fort("xsetdr\000", drv, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	       &dv, 7L, 40L);
    }
  else if (*flag__ == 4)
    {
      /*     init */
      /*     .  initial value */
      z__[1] = 0.;
      /*     .  get geometry of the block */
      scicos_getgeom (&z__[2]);
      if (z__[2] < 0.)
	{
	  return 0;
	}
      i__1 = (int) z__[2];
      scicos_setblockwin (&i__1, &cur);
      dr1_fort("xgetdr\000", drv, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	       &dv, 7L, 40L);
      dr1_fort("xsetdr\000", "X11\000", &v, &v, &v, &v, &v, &v, &dv, &dv,
	       &dv, &dv, 7L, 4L);
      scicos_recterase (&z__[3]);
      scicos_affdraw (&ipar[1], &ipar[5], &z__[1], &z__[3]);
      dr1_fort("xsetdr\000", drv, &v, &v, &v, &v, &v, &v, &dv, &dv, &dv,
	       &dv, 7L, 40L);
    }
  return 0;
}			

int scicos_setblockwin (BCG *Xgc,int *win, int *cur)
{
  *cur = Xgc->graphic_engine->xget_curwin();
  if (*cur != *win) Xgc->graphic_engine->xset_curwin(*win,TRUE);
  return 0;
}		


int scicos_recterase (BCG *Xgc,const double r[])
{
  const double dx = .06, dy = .06;
  double w,x,y,h;
  x = r[0] + dx * r[2];
  y = r[1] + r[3];
  w = r[2] * (1. - dx);
  h = r[3] * (1. - dy);
  Xgc->graphic_engine->scale->cleararea(Xgc,x,y,w,h);
  return 0;
}	


int scicos_affdraw (BCG *Xgc,const int fontd[],const int form[],const double *val,const double r[])
{
  int fontid[2],rect[4],flag=0,pixmode;
  char buf[128];
  double x,y,angle=0.0;
  sprintf(buf,"%*.*f",form[0],form[1],val);
  Xgc->graphic_engine->xget_font(Xgc,fontid);
  Xgc->graphic_engine->xset_font(Xgc,fontd[0],fontd[1]);
  Xgc->graphic_engine->boundingbox(Xgc,buf,r[0],r[1],rect);
  x = r[0] + Max (0.0,(r[2] - rect[2]) / 2.);
  y = r[1] + Max (0.0,(r[3] - rect[3]) / 2.);
  Xgc->graphic_engine->displaystring(Xgc,buf,x,y,flag,angle);
  Xgc->graphic_engine->xset_font(Xgc,fontid[0],fontid[1]);
  pixmode = Xgc->graphic_engine->xget_pixmapOn(Xgc);
  if ( pixmode == 1) Xgc->graphic_engine->scale->xset_show(Xgc);
} 


int scicos_getgeom (double *g)
{
  static int c__5 = 5;
  int i__1;
  double d__1;
  int l;
  int il, ret;
  double rhs;
  double top;
  --g;
  d__1 = rhs + 1;
  ret = (int) scicos_scistring (&d__1, "getgeom", &c__1, &c__0, 7L);
  d__1 = top + 1;
  i__1 = scicos_lstk (&d__1);
  il = i__1 + i__1 - 1;
  i__1 = il + 4;
  l = i__1 / 2 + 1;
  d__1 = scicos_stk (&l);
  scicos_dcopy (&c__5, &d__1, &c__1, &g[1], &c__1);
  /*      top=top-1 */
  return 0;
}	

/*------------------------------------------------
 *     Scicos block simulator 
 *     A set of elementary blocks 
 *------------------------------------------------*/

typedef  void (scicos0_block) (ARGS_scicos0);
extern scicos0_block blocks_absblk, blocks_andlog, blocks_bidon, blocks_gain;
extern scicos0_block blocks_cdummy, blocks_dband, blocks_cosblk;

/*------------------------------------------------
 *     Scicos block simulator 
 *     returns Absolute value of the input 
 *------------------------------------------------*/


void blocks_absblk(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
  int i;
  for (i = 0 ; i <  *nu ; ++i ) y[i] = Abs(u[i]);
} 


/*------------------------------------------------
 *     Scicos block simulator 
 *     Logical and block
 *     if event input exists synchronuously, output is 1 else -1
 *------------------------------------------------*/

void blocks_andlog(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
  if ( *flag == 1)  y[0] = ( *nevprt != 3 ) ? -1.00 :  1.00; 
}


/*------------------------------------------------
 *     Scicos block simulator 
 *     does nothing 
 *------------------------------------------------*/

void blocks_bidon(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
}



/*------------------------------------------------
 *     Scicos block simulator 
 *     input to output Gain
 *     rpar=gain matrix
 *------------------------------------------------*/

void blocks_gain(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
  int un=1;
  blocks_dmmul(rpar,ny,u,nu,y,ny,ny,nu,&un);
}

/*------------------------------------------------
 *     Scicos block simulator 
 *     Dummy state space x'=sin(t)
 *------------------------------------------------*/

void blocks_cdummy(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
  if ( *flag == 0 ) xd[0]=sin(*t);
}

/*------------------------------------------------
 *     Scicos block simulator 
 *     Dead Band, 
 *     if u(i)<0 ,y(i)=min(0,u+DB(i)/2) 
 *     else       y(i)=max(0,u-DB(i)/2) 
 *     DB(i)=rpar(i) 
 *------------------------------------------------*/

void blocks_dband(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{
  int i;
  
  for ( i=0 ; i < *nu ; i++ ) 
    {
      if ( u[i] < 0 ) 
	y[i] = Min(0.00,u[i]+rpar[i]/2.00);
      else  
	y[i] = Max(0.00,u[i]-rpar[i]/2.00);
    }
}


/*------------------------------------------------
 *     Scicos block simulator 
 *------------------------------------------------*/

void blocks_cosblk(int *flag, int *nevprt, double *t, double *xd, double *x, int *nx, double *z, int *nz, double *tvec, int *ntvec, double *rpar, int *nrpar, int *ipar, int *nipar, double *u, int *nu, double *y, int *ny)
{

  int i;
  for ( i=0; i < *nu ; i++)     y[i]= cos(u[i]);
}

