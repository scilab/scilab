/* Copyright INRIA */
#include "../machine.h"
#include "import.h"
extern struct {
    integer nblk, nordptr, nout, ng, nrwp, niwp, ncord, noord, 
	    nzord;
} C2F(cossiz);

extern struct {
    integer iero;
} C2F(ierode);

ScicosImport  scicos_imp;

extern struct {
    integer kfun;
} C2F(curblk);

static integer c1 = 1;

int C2F(grblkdassl)(neq, t, xc, xd,ng1, g,rpar,ipar)
     integer *neq;
     double *t, *xc, *xd;
     integer *ng1,*ipar;
     double *g,*rpar;
{
  integer nordclk,i1,klink, kport, ig,n,i2;

 
  C2F(ierode).iero = 0;
  nordclk=scicos_imp.ordptr[C2F(cossiz).nordptr-1]-1;
  C2F(zdoit)(neq, xd, xc, scicos_imp.xptr, scicos_imp.z, scicos_imp.zptr, scicos_imp.iz, 
	     scicos_imp.izptr, t, 
	     scicos_imp.tevts, scicos_imp.evtspt, &scicos_imp.nevts, &scicos_imp.pointi, 
	     scicos_imp.inpptr, scicos_imp.inplnk, scicos_imp.outptr, scicos_imp.outlnk, 
	     scicos_imp.lnkptr, 
	     scicos_imp.clkptr, scicos_imp.ordptr, &C2F(cossiz).nordptr, scicos_imp.ordclk, 
	     &nordclk, scicos_imp.cord, 
	     scicos_imp.oord, scicos_imp.zord, 
	     scicos_imp.critev, scicos_imp.rpar, scicos_imp.rpptr, scicos_imp.ipar, 
	     scicos_imp.ipptr, scicos_imp.funptr, scicos_imp.funtyp, scicos_imp.outtb, 
	     &C2F(ierode).iero);
  /*     form z vector (concatenation of threshold inputs) */
  ig = 0;
  /* loop on zero crossing blocks */
  i1 = C2F(cossiz).nblk;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i1; ++C2F(curblk).kfun) {
    if (scicos_imp.ztyp[C2F(curblk).kfun-1]>0) {
      /*loop on block input ports */
      i2 = scicos_imp.inpptr[C2F(curblk).kfun] - 1;
      for (kport = scicos_imp.inpptr[C2F(curblk).kfun-1]; kport <= i2; ++kport) {
	/* get corresponding link pointer */
	klink = scicos_imp.inplnk[kport-1]-1;
	n = scicos_imp.lnkptr[klink + 1] - scicos_imp.lnkptr[klink];
	/* copy vector valued link in g */
	F2C(dcopy)(&n, &(scicos_imp.outtb[scicos_imp.lnkptr[klink]-1]), &c1, &g[ig], &c1);
	ig += n;
      }
    }
  }
  return 0;
}


int C2F(grblk)(neq, t, xc, ng1, g)
     integer *neq;
     double *t, *xc;
     integer *ng1;
     double *g;
    
 
     /*
	!purpose 
	interface to grbl1 at the lsodar format 
	!calling sequence 
	neq   : integer  the size of the continuous state
	t     : current time 
	xc    : double precision vector contains the continuous state
	g     : computed zero crossing surface (see lsodar) 
	ng1   : size ng 
	!
	*/

     /* Local variables */

    { integer nordclk,i1,klink, kport, ig,n,i2;
	C2F(ierode).iero = 0;
	nordclk=scicos_imp.ordptr[C2F(cossiz).nordptr-1]-1;
	C2F(zdoit)(neq, xc, xc, scicos_imp.xptr, scicos_imp.z, scicos_imp.zptr, scicos_imp.iz, 
	       scicos_imp.izptr, t, 
	       scicos_imp.tevts, scicos_imp.evtspt, &scicos_imp.nevts, &scicos_imp.pointi, 
	       scicos_imp.inpptr, scicos_imp.inplnk, scicos_imp.outptr, scicos_imp.outlnk, 
	       scicos_imp.lnkptr, 
	       scicos_imp.clkptr, scicos_imp.ordptr, &C2F(cossiz).nordptr, scicos_imp.ordclk, 
	       &nordclk, scicos_imp.cord, 
	       scicos_imp.oord, scicos_imp.zord, 
	       scicos_imp.critev, scicos_imp.rpar, scicos_imp.rpptr, scicos_imp.ipar, 
	       scicos_imp.ipptr, scicos_imp.funptr, scicos_imp.funtyp, scicos_imp.outtb, 
	       &C2F(ierode).iero);
      /*     form z vector (concatenation of threshold inputs) */
      ig = 0;
      /* loop on zero crossing blocks */
      i1 = C2F(cossiz).nblk;
      for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i1; ++C2F(curblk).kfun) {
	  if (scicos_imp.ztyp[C2F(curblk).kfun-1]>0) {
	      /*loop on block input ports */
	      i2 = scicos_imp.inpptr[C2F(curblk).kfun] - 1;
	      for (kport = scicos_imp.inpptr[C2F(curblk).kfun-1]; kport <= i2; ++kport) {
		  /* get corresponding link pointer */
		  klink = scicos_imp.inplnk[kport-1]-1;
		  n = scicos_imp.lnkptr[klink + 1] - scicos_imp.lnkptr[klink];
		  /* copy vector valued link in g */
		  F2C(dcopy)(&n, &(scicos_imp.outtb[scicos_imp.lnkptr[klink]-1]), &c1, &g[ig], &c1);
		  ig += n;
	      }
	  }
      }
 /*     printf("surface_g %f \n", *g);*/
      return 0;
  }

