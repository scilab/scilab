
#include <stdlib.h> 
#include <string.h>
#include "../machine.h"
#include "../sun/link.h"
#include "scicos.h"
#include "import.h"
#include "blocks.h"
/*#include "scicos_block.h"*/


#ifdef FORDLL 
#define IMPORT  __declspec (dllimport)
#else 
#define IMPORT extern
#endif


#define abs(x) ((x) >= 0 ? (x) : -(x))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))


void cosini(double *);
void cosiord(double *);
void cosend(double *);
void cdoit(double *);
void doit(double *,integer *);
void ddoit(double *);
void edoit(double *,integer *,integer *);
void odoit(double *,double *,double *,double *);
void ozdoit(double *,double *,double *,integer *,integer *);
void zdoit(double *,double *,double *,double *);
void cossimdassl(double *);
void cossim(double *);
void callf(double *, double *, double *, double *,double *,integer *);
int C2F(simblk)(integer *, double *, double *, double *);
int C2F(simblkdassl)(double *, double *, double *, integer *, double *, integer *, double *, integer *);
int C2F(grblk)(integer *, double *, double *, integer *, double *);
int C2F(grblkdassl)(integer *, double *, double *, double *, integer *, double *, double *, integer *);
void addevs(double *,integer *,integer *);
void putevs(double *,integer *,integer *);


IMPORT struct {
  int cosd;
} C2F(cosdebug);

struct {
  int solver;
} C2F(cmsolver);


extern void  F2C(sciblk)();
extern void  sciblk2();
extern void  sciblk2i();
extern void  GetDynFunc();
extern void  sciprint();
extern void  C2F(iislink)();


extern integer  C2F(cvstr)();
extern  integer C2F(dset)();
extern  integer C2F(dcopy)();
extern  integer C2F(iset)();
extern  integer C2F(realtime)();
extern  integer C2F(realtimeinit)();
extern  integer C2F(sxevents)();
extern  integer C2F(stimer)();
extern  integer C2F(xscion)();
extern  integer C2F(ddaskr)();
extern  integer C2F(lsodar)();

ScicosImport  scicos_imp;

static integer nblk, nordptr, nout, ng, nrwp, niwp, ncord, noord, nzord,niord,
  nclock,nordclk,niord;

static integer *neq;

static  double Atol, rtol, ttol, deltat;
static integer hot;

extern struct {
  integer iero;
} C2F(ierode);

extern  struct {
  integer kfun;
} C2F(curblk);

struct {
  double scale;
}  C2F(rtfactor);

extern struct {
  integer halt;
}  C2F(coshlt);

/* Table of constant values */

static integer c__90 = 90;
static integer c__0 = 0;
static integer c__91 = 91;
static double c_b14 = 0.;
static integer c__1 = 1;
static integer c_n1 = -1;

static double tvec[100];

static double *W; /* space for coding g and outtb-bis for fixed point iteration */

static integer *iwa;

static integer *xptr, *evtspt;
static integer  *funtyp, *inpptr, *outptr, *inplnk, *outlnk, *lnkptr;
static integer *clkptr, *ordptr, *ordclk, *cord, 
  *iord, *oord,  *zord,  *critev,  *zcptr;
static integer *pointi;
static integer *ierr;

static double *x,*xd,*tevts,*outtb;

static double *rhot;
static integer *ihot;

static integer *jroot,*zcros,*mask;

static double *t0,*tf;

static scicos_block *Blocks; 


/* Subroutine */ 
int C2F(scicos)(x_in, xptr_in, z__, zptr, iz, izptr, t0_in, tf_in, tevts_in, 
		evtspt_in, nevts, pointi_in, outtb_in, nout1, funptr, funtyp_in, inpptr_in, outptr_in, 
		inplnk_in, outlnk_in, lnkptr_in, nlnkptr, rpar, rpptr, ipar, ipptr, clkptr_in, 
		ordptr_in, nordptr1, ordclk_in, cord_in, ncord1, iord_in, niord1, oord_in, noord1, 
		zord_in, nzord1, critev_in, nblk1, ztyp, zcptr_in, ng1, subscr, nsubs, simpar, 
		w, iw, iwa_in, flag__, ierr_out)
     double *x_in,*z__;
     integer *xptr_in;
     integer *zptr, *iz, *izptr;
     double *t0_in, *tf_in, *tevts_in;
     integer *evtspt_in, *nevts, *pointi_in;
     double *outtb_in;
     integer *nout1, *funptr, *funtyp_in, *inpptr_in, *outptr_in, *inplnk_in, *outlnk_in, *lnkptr_in,
       *nlnkptr;
     double *rpar;
     integer *rpptr, *ipar, *ipptr, *clkptr_in, *ordptr_in, *nordptr1, *ordclk_in, *cord_in, *
       ncord1, *iord_in, *niord1, *oord_in, *noord1, *zord_in, *nzord1, *critev_in, *
       nblk1, *ztyp, *zcptr_in, *ng1, *subscr, *nsubs;
double *simpar, *w;
integer *iw, *iwa_in, *flag__, *ierr_out;
{
  /* System generated locals */
  integer i1, i2,kf,lprt,in,out,job=1;

  /* Local variables */
  extern /* Subroutine */ int C2F(msgs)();
  static integer mxtb, ierr0, kfun0, i, j, k;
  extern /* Subroutine */ int C2F(makescicosimport)();
  static integer  lihot, lrhot;
  extern /* Subroutine */ int C2F(getscsmax)();
  static integer ni, no;
  extern /* Subroutine */ int C2F(clearscicosimport)();
  static integer lw, nx, nz;
  static integer maxord;
  static integer louttb, ljroot,lzcros,lmask, liw, lww;

  /*     Copyright INRIA */
  /* iz,izptr are used to pass block labels */

  t0=t0_in;
  tf=tf_in;
  ierr=ierr_out;

  iwa=iwa_in;

  /* Parameter adjustments */
  pointi=pointi_in;
  x=x_in;
  xptr=xptr_in-1;
  --zptr;
  --izptr;
  evtspt=evtspt_in-1;
  tevts=tevts_in-1;
  outtb=outtb_in;
  funtyp=funtyp_in-1;
  inpptr=inpptr_in-1;
  outptr=outptr_in-1;
  inplnk=inplnk_in-1;
  outlnk=outlnk_in-1;
  lnkptr=lnkptr_in-1;
  --rpptr;
  --ipptr;
  clkptr=clkptr_in-1;
  ordptr=ordptr_in-1;
  ordclk=ordclk_in-1;
  cord=cord_in-1;
  iord=iord_in-1;
  oord=oord_in-1;
  zord=zord_in-1;

  critev=critev_in-1;
  --ztyp;
  zcptr=zcptr_in-1;
  --simpar;
  --w;
  --iw;

  /* Function Body */
  Atol = simpar[1];
  rtol = simpar[2];
  ttol = simpar[3];
  deltat = simpar[4];
  C2F(rtfactor).scale = simpar[5];
  C2F(cmsolver).solver = (integer) simpar[6];

  nordptr = *nordptr1;
  nblk = *nblk1;
  ncord = *ncord1;
  noord = *noord1;
  nzord = *nzord1;
  niord = *niord1;
  nout = *nout1;

  *ierr = 0;

  xd=&x[xptr[nblk+1]];

  nordclk=ordptr[nordptr]-1;
  /*     computes number of zero crossing surfaces */
  ng = *ng1;

  /*     number of  discrete real states */
  nz = zptr[nblk + 1] - 1;
  /*     number of continuous states */
  nx = xptr[nblk +1] - 1;
  neq=&nx;
  if (C2F(cmsolver).solver == 0) {
    /*     hotstart work size */
    /* Computing MAX */
    i1 = 16, i2 = nx + 9;
    nrwp = nx * max(i1,i2) + 22 + ng * 3;
    niwp = nx + 20;
  } else if (C2F(cmsolver).solver == 100) {
    maxord = 5;
    /* Computing MAX */
    i1 = maxord + 4;
    /* Computing 2nd power */
    i2 = nx;
    nrwp = max(i1,7) * nx + 60 + i2 * i2 + ng * 3;
    niwp = nx + 40 + nx;
  } else {
    /*        add an error message please */
  }
  /*     number of rows in ordclk is ordptr(nclkp1)-1 */
  /*     maximum block state and input sizes */
  /*     split working areas into pieces */
  lw = 1;
  /*     lx must be equal to one */
  louttb = lw;
  lw = louttb + max(nout,ng);
  W=&w[louttb];
  /*     louttb used both in cosini for fixed-point and in cossim */
  /*     for temporary storage of g */
  lrhot = lw;
  rhot=&w[lrhot];
  lw = lrhot + nrwp;
  /*     reserved for futher use */
  lww = lw;
  lw = lww + 1;

  liw = 1;
  lihot = liw;
  ihot=&iw[lihot];
  liw = lihot + niwp;
  ljroot = liw;
  jroot=&iw[ljroot];
  liw =ljroot  + ng;
  lzcros = liw;
  zcros=&iw[lzcros];
  liw = lzcros + ng;
  lmask = liw;
  mask=&iw[lmask];
  liw = lmask + ng;
  /*     . lmask is the zero-crossing mask */

  for (i = 1; i <= nblk; ++i) {
    if (funtyp[i] < 10000) {
      funtyp[i] %= 1000;
    } else {
      funtyp[i] = funtyp[i] % 1000 + 10000;
    }
    ni = inpptr[i + 1] - inpptr[i];
    no = outptr[i + 1] - outptr[i];
    if (funtyp[i] == 1) {
      if (ni + no > 11) {
	/*     hard coded maxsize in callf.c */
	C2F(msgs)(&c__90, &c__0);
	C2F(curblk).kfun = i;
	*ierr = i + 1005;
	return 0;
      }
    } else if (funtyp[i] == 2 || funtyp[i] == 3) {
      /*     hard coded maxsize in scicos.h */
      if (ni + no > SZ_SIZE) {
	C2F(msgs)(&c__90, &c__0);
	C2F(curblk).kfun = i;
	*ierr = i + 1005;
	return 0;
      }
    }
    mxtb = 0;
    if (funtyp[i] == 0) {
      if (ni > 1) {
	for (j = 1; j <= ni; ++j) {
	  k = inplnk[inpptr[i] - 1 + j];
	  mxtb = mxtb + lnkptr[k + 1] - lnkptr[k];
	}
      }
      if (no > 1) {
	for (j = 1; j <= no; ++j) {
	  k = outlnk[outptr[i] - 1 + j];
	  mxtb = mxtb + lnkptr[k + 1] - lnkptr[k];
	}
      }
      if (mxtb > TB_SIZE) {
	C2F(msgs)(&c__91, &c__0);
	C2F(curblk).kfun = i;
	*ierr = i + 1005;
	return 0;
      }
    }
  }

  if((Blocks=malloc(sizeof(scicos_block)*nblk))== NULL ){
    *ierr =10000;
    return 0;
  }
  for (kf = 0; kf < nblk; ++kf) {
    C2F(curblk).kfun = kf+1;
    i=funptr[kf];
    Blocks[kf].type=funtyp[kf+1];
    if (i<0) {
      switch (funtyp[kf+1]) {
      case 0:
	Blocks[kf].funpt=F2C(sciblk);
	break;
      case 1:
	sciprint("type 1 function not allowed for scilab blocks\r\n");
	*ierr=10000;
	return 0;
      case 2:
	sciprint("type 2 function not allowed for scilab blocks\r\n");
	*ierr=10000;
	return 0;
      case 3:
	Blocks[kf].funpt=sciblk2;
	Blocks[kf].type=2;
	break;
      case 10003:
	Blocks[kf].funpt=sciblk2i;
	Blocks[kf].type=2;
	break;
      default :
	sciprint("Undefined Function type\r\n");
	*ierr=10000;
	return 0;
      }
      Blocks[kf].scsptr=-i; /* set scilab function adress for sciblk */
    }
    else if (i<=ntabsim)
      Blocks[kf].funpt=*(tabsim[i-1].fonc);
    else {
      i -= (ntabsim+1);
      GetDynFunc(i,&Blocks[kf].funpt);
      if ( Blocks[kf].funpt == (voidf) 0) {
	sciprint("Function not found\r\n");
	*ierr=1000;
	return 0;
      }
    }
    Blocks[kf].ztyp=ztyp[kf+1];
    Blocks[kf].nx=xptr[kf+2]-xptr[kf+1];
    Blocks[kf].ng=zcptr[kf+2]-zcptr[kf+1];
    Blocks[kf].nz=zptr[kf+2]-zptr[kf+1];
    Blocks[kf].nrpar=rpptr[kf+2]-rpptr[kf+1];
    Blocks[kf].nipar=ipptr[kf+2]-ipptr[kf+1];
    Blocks[kf].nin=inpptr[kf+2]-inpptr[kf+1]; /* number of input ports */
    Blocks[kf].nout=outptr[kf+2]-outptr[kf+1];/* number of output ports */
    if ((Blocks[kf].insz=malloc(sizeof(int)*Blocks[kf].nin))== NULL ){
      *ierr =10000;
      return 0;
    }
    if ((Blocks[kf].inptr=malloc(sizeof(int*)*Blocks[kf].nin))== NULL ){
      *ierr =10000;
      return 0;
    }
    for(in=0;in<Blocks[kf].nin;in++) {
      lprt=inplnk[inpptr[kf+1]+in];
      Blocks[kf].inptr[in]=&(outtb[lnkptr[lprt]-1]);
      Blocks[kf].insz[in]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    if ((Blocks[kf].outsz=malloc(sizeof(int)*Blocks[kf].nout))== NULL ){
      *ierr =10000;
      return 0;
    }
    if ((Blocks[kf].outptr=malloc(sizeof(double*)*Blocks[kf].nout))== NULL ){
      *ierr =10000;
      return 0;
    }
    for(out=0;out<Blocks[kf].nout;out++) {
      lprt=outlnk[outptr[kf+1]+out];
      Blocks[kf].outptr[out]=&(outtb[lnkptr[lprt]-1]);
      Blocks[kf].outsz[out]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    Blocks[kf].nevout=clkptr[kf+2] - clkptr[kf+1];
    Blocks[kf].z=&(z__[zptr[kf+1]-1]);
    Blocks[kf].rpar=&(rpar[rpptr[kf+1]-1]);
    Blocks[kf].ipar=&(ipar[ipptr[kf+1]-1]);


    i1=izptr[kf+2]-izptr[kf+1];
    i1=max(i1,40);
    C2F(cvstr)(&i1,&(iz[izptr[kf+1]-1]),Blocks[kf].label,&job,i1);
    Blocks[kf].label[i1]='\0';
  }

  C2F(makescicosimport)(x, &xptr[1], &zcptr[1], z__, &zptr[1], iz, &izptr[1], 
			&inpptr[1], &inplnk[1], &outptr[1], &outlnk[1], &lnkptr[1], 
			nlnkptr, rpar, &rpptr[1], ipar, &ipptr[1], &nblk, outtb, 
			&nout, subscr, nsubs, &tevts[1], &evtspt[1], nevts, pointi, 
			&oord[1], &zord[1], funptr, &funtyp[1], &ztyp[1], &cord[1],
			&ordclk[1], &clkptr[1], &ordptr[1], &critev[1], iwa, mask);
  if (*flag__ == 1) {
    /*     initialisation des blocks */
    cosini(t0);
    if (*ierr != 0) {
      ierr0=*ierr;
      kfun0 = C2F(curblk).kfun;
      cosend(t0);
      *ierr=ierr0;
      C2F(curblk).kfun = kfun0;
    }
  } else if (*flag__ == 2) {
    /*     integration */
    if (C2F(cmsolver).solver == 0) {
      cossim(t0);
  } else if (C2F(cmsolver).solver == 100) {
    cossimdassl(t0);
  } else {
    /*     add a warning message please */
  }
  if (*ierr != 0) {
    ierr0=*ierr;
    kfun0 = C2F(curblk).kfun;
    cosend(t0);
    *ierr=ierr0;
    C2F(curblk).kfun = kfun0;
  }

} else if (*flag__ == 3) {
  /*     fermeture des blocks */
  cosend(t0);
} else if (*flag__ == 4) {
  cosiord(t0);
  if (*ierr == 0) {
    C2F(simblk)(&nx, t0, x, W);
    for (i = 0; i < nx; ++i) {
      x[i] = W[i];
    }
  }
}
    
C2F(clearscicosimport)();
return 0;
} /* scicos_ */

/* Subroutine */ void cosini(told)

     double *told;

{
  static integer flag__;
  static integer i;

  static integer kfune;
  static integer jj;

  /* Function Body */
  *ierr = 0;
  /*     initialization (flag 4) */
  /*     loop on blocks */
  tvec[0] = 0.;

  C2F(dset)(&nout, &c_b14, W, &c__1);
  nclock = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (funtyp[C2F(curblk).kfun] >= 0) {
      flag__ = 4;
      callf(told, xd, x, x,W,&flag__);
      if (flag__ < 0 && *ierr == 0) {
	*ierr = 5 - flag__;
	kfune = C2F(curblk).kfun;
      }
    }
  }
  if (*ierr != 0) {
    C2F(curblk).kfun = kfune;
    return;
  }
  /*     initialization (flag 6) */
  nclock = 0;
  tvec[0] = 0.;

  if (ncord > 0) {
    for (jj = 1; jj <= ncord; ++jj) {
      C2F(curblk).kfun = cord[jj];
      flag__ = 6;
      if (funtyp[C2F(curblk).kfun] >= 0) {
	callf(told, xd, x, x,W,&flag__);
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }
  }

  /*     point-fix iterations */
  nclock =0;
  for (i = 1; i <= nblk + 1; ++i) {
    /*     loop on blocks */
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
      flag__ = 6;
      if (funtyp[C2F(curblk).kfun] >= 0) {
	callf(told, xd, x, x,W,&flag__);
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }

    nclock = 0;
    tvec[0] = 0.;

    if (ncord > 0) {
      for (jj = 1; jj <= ncord; ++jj) {
	C2F(curblk).kfun = cord[jj];
	flag__ = 6;
	if (funtyp[C2F(curblk).kfun] >= 0) {
	  callf(told, xd, x, x,W,&flag__);
	  if (flag__ < 0) {
	    *ierr = 5 - flag__;
	    return;
	  }
	}
      }
    }
    for (jj = 0; jj <= nout-1; ++jj) {
      if (outtb[jj] != W[jj]) {
	goto L30;
      }
    }
    return;
  L30:
    C2F(dcopy)(&nout, outtb, &c__1, W, &c__1);

  }
  *ierr = 20;
} /* cosini_ */

/* Subroutine */ void cosiord(told)
     double *told;
{
  static integer flag__;
  static integer jj;

  /*     Copyright INRIA */


  /* ..   Parameters .. */
  /*     maximum number of clock output for one block */

  /*     neq must contain after #states all integer data for simblk and grblk */
  /*     X must contain after state values all real data for simblk and grblk */
  /* Parameter adjustments */

  /* Function Body */

  /*     initialisation (propagation of constant blocks outputs) */

  for (jj = 1; jj <= niord; ++jj) {
    C2F(curblk).kfun = iord[jj];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = iord[jj + niord];
      flag__ = 1;
      callf(told, xd, x, x,W,&flag__);
	
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
  }
} /* cosiord_ */

/* Subroutine */ void cossim(told)
     double *told;

{
  /* Initialized data */
  static integer otimer = 0;
  /* System generated locals */
  integer i3;

  /* Local variables */
  static integer flag__, jdum;
  static integer iopt;

  static integer ierr1;
  static integer j, k;
  static double t;
  static integer itask;
  static integer ib, jj, jt;
  static integer istate, ntimer;

  static double rhotmp;
  static integer inxsci;

  static integer kpo, kev;



  /* Function Body */

  C2F(coshlt).halt = 0;
  *ierr = 0;
  hot = 0;
  C2F(xscion)(&inxsci);
  /*     initialization */
  C2F(iset)(&niwp, &c__0, &ihot[1], &c__1);
  C2F(dset)(&nrwp, &c_b14, &rhot[1], &c__1);
  C2F(realtimeinit)(told, &C2F(rtfactor).scale);


  jj = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (Blocks[C2F(curblk).kfun-1].ng >= 1) {
    zcros[jj] = C2F(curblk).kfun;
    ++jj;
  }
}
/*     . Il fault:  ng >= jj */
if (jj != ng) {
  zcros[jj] = -1;
}
/*     Initializing the zero crossing mask */
C2F(iset)(&ng, &c__0, mask, &c__1);

/*     initialisation (propagation of constant blocks outputs) */

for (jj = 1; jj <= niord; ++jj) {
  C2F(curblk).kfun = iord[jj];
  if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
    nclock = iord[jj + niord];
    flag__ = 1;
    callf(told, xd , x, x,W,&flag__);
	    
    if (flag__ < 0) {
      *ierr = 5 - flag__;
      return;
    }
  }
}

/*     main loop on time */

while(*told < *tf) {

  if (inxsci == 1) {
    ntimer = C2F(stimer)();
    if (ntimer != otimer) {
      C2F(sxevents)();
      otimer = ntimer;
      /*     .     sxevents can modify halt */
    }
  }
  if (C2F(coshlt).halt != 0) {
    C2F(coshlt).halt = 0;
    return;
  }
  if (*pointi == 0) {
    t = *tf;
  } else {
    t = tevts[*pointi];
  }
  if (abs(t - *told) < ttol) {
    t = *told;
    /*     update output part */
  }
  if (*told > t) {
    /*     !  scheduling problem */
    *ierr = 1;
    return;
  }
  if (*told != t) {
    if (xptr[nblk+1] == 1) {
      /*     .     no continuous state */
      if (*told + deltat + ttol > t) {
	*told = t;
      } else {
	*told += deltat;
      }
      /*     .     update outputs of 'c' type blocks with no continuous state */
      if (*told >= *tf) {
	/*     .     we are at the end, update continuous part before leaving */
	if (ncord > 0) {
	  cdoit(told);
	  return;
	}
      }
    } else {
      /*     integrate */
      if (hot) {
	istate = 2;
      } else {
	istate = 1;
      }
      itask = 4;

      rhotmp = *tf + ttol;
      kpo = *pointi;
    L20:
      if (critev[kpo] == 1) {
	rhotmp = tevts[kpo];
	goto L30;
      }
      kpo = evtspt[kpo];
      if (kpo != 0) {
	goto L20;
      }
    L30:
      if (rhotmp < rhot[1]) {
	istate = 1;
      }
      rhot[1] = rhotmp;

      iopt = 0;

      jt = 2;
      t = min(*told + deltat,min(t,*tf + ttol));


      if (C2F(cosdebug).cosd >= 3) {
	sciprint("****lsodar from: %f to %f\r\n", *told, t);
      }
      C2F(lsodar)(C2F(simblk), neq, x, told, &t, &c__1, &rtol, 
		  &Atol, &itask, &istate, &iopt, &rhot[1], &
		  nrwp, &ihot[1], &niwp, &jdum, &jt, 
		  C2F(grblk), &ng, jroot);
      if (C2F(cosdebug).cosd >= 3) {
	sciprint("****lsodar reached: %f",*told);
      }
      if (istate <= 0) {
	if (istate == -3) {
	  C2F(grblk)(neq, told, x, &ng, W);
	  for (ib = 0; ib < ng; ++ib) {
	    if (W[ib] == 0.) {
	      mask[ib] = 1;
	    }
	  }
	  hot = 0;
	  itask = 2;
	  rhot[5] = ttol;
	  istate = 1;
	  C2F(lsodar)(C2F(simblk), neq, x, told, &t, &c__1, &
		      rtol, &Atol, &itask, &istate, &
		      c__1, &rhot[1], &nrwp, &ihot[1], &
		      niwp, &jdum, &jt, C2F(grblk), &ng, jroot);
	  rhot[5] = (float)0.;
	  hot = 0;
	  if (istate <= 0) {
	    /*     !           integration problem */
	    *ierr = 100 - istate;
	    return;
	  }
	} else {
	  /*     !        integration problem */
	  *ierr = 100 - istate;
	  return;
	}
      } else {
	hot = 1;
      }
      /*     Initializing the zero crossing mask */
      C2F(grblk)(neq, told, x, &c_n1, W);
      for (ib = 0; ib < ng; ++ib) {
	if (W[ib] != 0. && mask[ib] == 1) {
	  hot = 0;
	  mask[ib] = 0;
	}
      }
      /*     .     update outputs of 'c' type  blocks if we are at the end*/
      if (*told >= *tf) {
	if (ncord > 0) {
	  cdoit(told);
	  return;
	}
      }
      if (istate == 3) {
	/*     .        at a least one root has been found */
	hot = 0;
	if (C2F(cosdebug).cosd >= 3) {
	  sciprint("root found at t=: %f\r\n",*told);
	}
	/*     .        update outputs affecting ztyp blocks */
	zdoit(W, xd, x,told);
	/*     .        initialize mask */
	for (ib = 0; ib < ng; ++ib) {
	  mask[ib] = 0;
	}

	for (jj = 0; jj < ng; ++jj) {
	  C2F(curblk).kfun = zcros[ jj];
	  if (C2F(curblk).kfun == -1) {
	    break; 
	  }
	  kev = 0;
	  /*
	    for (j = zcptr[C2F(curblk).kfun]-1 ; 
	    j <zcptr[C2F(curblk).kfun+1]-1 ; ++j) {
	    W[j]=(double)jroot[j];
	    if(jroot[j]!=0){
	    kev=1;
	    }
	    }
	  */
	  for (j = zcptr[C2F(curblk).kfun]-1 ; 
	       j <zcptr[C2F(curblk).kfun+1]-1 ; ++j) {
	    if(jroot[j]!=0){
	      kev=1;
	      break;
	    }
	  }
	  /*   */
	  if (kev != 0) {
	    Blocks[C2F(curblk).kfun-1].jroot=&jroot[zcptr[C2F(curblk).kfun]-1];
	    if (Blocks[C2F(curblk).kfun-1].nevout > 0) {
	      flag__ = 3;
	      /* call corresponding block to determine output event (kev) */
	      nclock = -kev;
	      callf(told, xd, x, x,W,&flag__);
	      if (flag__ < 0) {
		*ierr = 5 - flag__;
		return;
	      }
	      /*     .              update event agenda */
	      for (k = 1; k <= Blocks[C2F(curblk).kfun-1].nevout; ++k) {
		if (tvec[k - 1] >= *told) {
		  if (critev[k + clkptr[C2F(curblk).kfun] - 1] 
		      == 1) {
		    hot = 0;
		  }
		  i3 = k + clkptr[C2F(curblk).kfun] - 1;
		  addevs(&tvec[k - 1], &i3, &ierr1);
		  if (ierr1 != 0) {
		    /*     .                       nevts too small */
		    *ierr = 3;
		    return;
		  }
		} 
	      }
	    }
	    /*     .              update state */
	    if (Blocks[C2F(curblk).kfun-1].nx+Blocks[C2F(curblk).kfun-1].nz
		> 0) {
	      /*     .              call corresponding block to update state */
	      flag__ = 2;
	      nclock = -kev;
	      callf(told, xd, x, x,W,&flag__);
		
	      if (flag__ < 0) {
		*ierr = 5 - flag__;
		return;
	      }
	    }
	  }
	}
      }
    }
    C2F(realtime)(told);
  } else {
    /*     .  t==told */

    if (C2F(cosdebug).cosd >= 1) {
      sciprint("Event: %d activated at t=%f\r\n",*pointi,*told);
    }

    ddoit(told);
    if (C2F(cosdebug).cosd >= 1) {
      sciprint("End of activation\r\n");
    }
    if (*ierr != 0) {
      return;
    }

  }
  /*     end of main loop on time */
}
} /* cossim_ */



/* Subroutine */ void cossimdassl(told)
     double *told;
{
  /* Initialized data */
  static integer otimer = 0;
  /* System generated locals */
  integer i3,*ipardummy;
  double d__1,*rpardummy;

  /* Local variables */
  static integer flag__, jdum;
  static integer iopt, info[20];

  static integer ierr1;
  static integer j, k;
  static double t;
  static integer ib, jj, jt;
  static integer istate, ntimer;
  static double rhotmp;
  static integer inxsci;
  static integer kpo, kev;


  /* Function Body */

  C2F(coshlt).halt = 0;
  *ierr = 0;
  /*     hot = .false. */
  info[0] = 0;
  /*      stuck=.false. */
  C2F(xscion)(&inxsci);
  /*     initialization */
  C2F(iset)(&niwp, &c__0, &ihot[1], &c__1);
  C2F(dset)(&nrwp, &c_b14, &rhot[1], &c__1);
  C2F(realtimeinit)(told, &C2F(rtfactor).scale);
  /*     ATOL and RTOL are scalars */
  info[1] = 0;
  /*     derivatives automatically computed by numerical differences */
  info[4] = 0;
  /*     full jac matrx */
  info[5] = 0;

  /*     max step size not given (handled by deltat) */
  info[6] = 0;
  /*     code determines initial step size */
  info[7] = 0;
  /*     MAXORD=5 */
  info[8] = 0;
  /*     no info on solution sign available */
  info[9] = 0;
  /*     initial t,y,yprime are not assumed to be consistent */
  info[10] = 1;
  /*     direc method instead of Dcrylof method */
  info[11] = 0;
  info[12] = 0;
  info[13] = 0;
  info[14] = 0;
  info[15] = 0;
  info[16] = 0;
  info[17] = 0;
  for (jj = 1; jj <= *neq; ++jj) {
    ihot[jj + 40] = 1;
  }
  jj = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (Blocks[C2F(curblk).kfun-1].ng >= 1) {
    zcros[jj] = C2F(curblk).kfun;
    ++jj;
  }
}
/*     . Il fault:  ng >= jj */
if (jj != ng) {
  zcros[jj] = -1;
}
/*     Initializing the zero crossing mask */
C2F(iset)(&ng, &c__0, mask, &c__1);
    
/*     initialisation (propagation of constant blocks outputs) */
for (jj = 1; jj <= niord; ++jj) {
  C2F(curblk).kfun = iord[jj];
  if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
    nclock = iord[jj + niord];
    flag__ = 1;
    callf(told, xd, x, x,W,&flag__);

    if (flag__ < 0) {
      *ierr = 5 - flag__;
      return;
    }
  }
}
/*     main loop on time */
while (*told < *tf) {
  if (inxsci == 1) {
    ntimer = C2F(stimer)();
    if (ntimer != otimer) {
      C2F(sxevents)();
      otimer = ntimer;
      /*     .     sxevents can modify halt */
    }
  }
  if (C2F(coshlt).halt != 0) {
    C2F(coshlt).halt = 0;
    return;
  }
  if (*pointi == 0) {
    t = *tf;
  } else {
    t = tevts[*pointi];
  }
  if (abs(t - *told) < ttol) {
    t = *told;
    /*     update output part */
  }
  if (*told > t) {
    /*     !  scheduling problem */
    *ierr = 1;
    return;
  }
  if (*told != t) {
    if (xptr[nblk+1] == 1) {
      /*     .     no continuous state */
      if (*told + deltat + ttol > t) {
	*told = t;
      } else {
	*told += deltat;
      }
      /*     .     update outputs of 'c' type blocks with no continuous state */
      if (*told >= *tf) {
	/*     .     we are at the end, update continuous part before leaving */
	if (ncord > 0) {
	  cdoit(told);
	  return;
	}
      }
    } else {
      info[3] = 1;
      info[2] = 0;

      rhotmp = *tf + ttol;
      kpo = *pointi;
    L20:
      if (critev[kpo] == 1) {
	rhotmp = tevts[kpo];
	goto L30;
      }
      kpo = evtspt[kpo];
      if (kpo != 0) {
	goto L20;
      }
    L30:
      if (rhotmp < rhot[1]) {
	info[0] = 0;
      }
      rhot[1] = rhotmp;

      iopt = 0;

      jt = 2;

      t = min(*told + deltat,min(t,*tf + ttol));

      if (info[0] == 0) {
	for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; 
	     ++C2F(curblk).kfun) {
	  if (Blocks[C2F(curblk).kfun-1].nx  > 0) {
	    flag__ = 7;
	    nclock = 0;
	    callf(told, xd, x, xd,W,&flag__);

	    if (flag__ < 0) {
	      *ierr = 5 - flag__;
	      return;
	    }
	  }
	}
      }
      if (C2F(cosdebug).cosd >= 3) {
	sciprint("****daskr from: %f to %f\r\n", *told, t);
      }
      /*     Warning rpar and ipar are used here as dummy pointers */
      C2F(ddaskr)(C2F(simblkdassl), neq, told, x, xd, &t, 
		  info, &rtol, &Atol, &istate, &rhot[1], &
		  nrwp, &ihot[1], &niwp, rpardummy, ipardummy
		  , &jdum, rpardummy, C2F(grblkdassl), &ng, jroot)
	;
      if (C2F(cosdebug).cosd >= 3) {
	sciprint("****daskr reached: %f\r\n",*told);
      }
      if (istate <= 0) {
	if (istate == -33) {

	  C2F(grblkdassl)(neq, told, x,xd, &ng, W,rpardummy,ipardummy);  /*rpar,ipar used as dummy*/
	  for (ib = 0; ib < ng; ++ib) {
	    if (W[ib] == 0.) {
	      mask[ib] = 1;
	    }
	  }
	  info[0] = 0;
	  info[2] = 1;
	  d__1 = *told + ttol;
	  C2F(ddaskr)(C2F(simblkdassl), neq, told, x, xd
		      , &d__1, info, &rtol, &Atol, &
		      istate, &rhot[1], &nrwp, &ihot[1], &
		      niwp, rpardummy, ipardummy, &jdum, rpardummy
		      , C2F(grblkdassl), &ng, jroot);
	  info[0] = 0;
	  if (istate <= 0) {
	    /*     !           integration problem */
	    *ierr = 100 - istate;
	    return;
	  }
	} else {
	  /*     !        integration problem */
	  *ierr = 100 - istate;
	  return;
	}
      } else {
	info[0] = 1;
      }

      /*     Initializing the zero crossing mask */
      C2F(grblkdassl)(neq, told, x,xd, &c_n1, W,rpardummy,ipardummy);  /*rpar,ipar used as dummy*/
      for (ib = 0; ib < ng; ++ib) {
	if (W[ib] != 0. && mask[ib] == 1) {
	  info[0] = 0;
	  mask[ib] = 0;
	}
      }
      /*     .     update outputs of 'c' type  blocks if we are at the end*/
      if (*told >= *tf) {
	if (ncord > 0) {
	  cdoit(told);
	  return;
	}
      }
      if (istate == 5) {
	/*     .        at a least one root has been found */
	info[0] = 0;
	if (C2F(cosdebug).cosd >= 3) {
	  sciprint("root found at t=: %f\r\n",*told);
	}
	/*     .        update outputs affecting ztyp blocks */
	zdoit(W, xd, x,told);
	/*     .        initialize mask */
	for (ib = 0; ib < ng; ++ib) {
	  mask[ib] = 0;
	}

	for (jj = 0; jj < ng; ++jj) {
	  C2F(curblk).kfun = zcros[jj];
	  if (C2F(curblk).kfun == -1) {
	    break; 
	  }
	  kev = 0;
	  for (j = zcptr[C2F(curblk).kfun]-1 ; 
	       j <zcptr[C2F(curblk).kfun+1]-1 ; ++j) {
	    if(jroot[j]!=0){
	      kev=1;
	      break;
	    }
	  }
	  if (kev != 0) {
	    Blocks[C2F(curblk).kfun-1].jroot=&jroot[zcptr[C2F(curblk).kfun]-1];
	    if (Blocks[C2F(curblk).kfun-1].nevout > 0) {
	      flag__ = 3;
	      /*     .              call corresponding block to determine output event (kev) */
	      nclock = -kev;
	      callf(told, xd, x, x,W,&flag__);
	      if (flag__ < 0) {
		*ierr = 5 - flag__;
		return;
	      }
	      /*     .              update event agenda */
	      for (k = 1; k <= Blocks[C2F(curblk).kfun-1].nevout; ++k) {
		if (tvec[k - 1] >= *told) {
		  if (critev[k + clkptr[C2F(curblk).kfun] - 1] 
		      == 1) {
		    info[0] = 0;
		  }
		  i3 = k + clkptr[C2F(curblk).kfun] - 1;
		  addevs(&tvec[k - 1], &i3, &ierr1);
		  if (ierr1 != 0) {
		    /*     .                       nevts too small */
		    *ierr = 3;
		    return;
		  }
		}
	      }
	    }
	    /*     .              update state */
	    if (Blocks[C2F(curblk).kfun-1].nx+Blocks[C2F(curblk).kfun-1].nz
		> 0) {
	      /*     .              call corresponding block to update state */
	      flag__ = 2;
	      nclock = -kev;
	      callf(told, xd, x, x,W,&flag__);
	      if (flag__ < 0) {
		*ierr = 5 - flag__;
		return;
	      }
	    }
	  }
	}
      }
    }
    C2F(realtime)(told);
  } else {
    /*     .  t==told */
    if (C2F(cosdebug).cosd >= 1) {
      sciprint("Event: %d activated at t=%f\r\n",*pointi,*told);
    }

    hot = info[0] == 1;
    ddoit(told);
    if (C2F(cosdebug).cosd >= 1) {
      sciprint("End of activation");
    }
    if (*ierr != 0) {
      return;
    }
    if (! hot) {
      info[0] = 0;
    }

  }
  /*     end of main loop on time */
}
} /* cossimdassl_ */



/* Subroutine */ void cosend(told)
     double *told;
{
  /* Local variables */
  static integer flag__;

  static integer kfune;

  /* Function Body */
  *ierr = 0;
  /*     loop on blocks */
  tvec[0] = 0.;

  nclock=0;

  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    flag__ = 5;
    if (funtyp[C2F(curblk).kfun] >= 0) {
      callf(told, xd, x, x,W,&flag__);
      if (flag__ < 0 && *ierr == 0) {
	*ierr = 5 - flag__;
	kfune = C2F(curblk).kfun;
      }
    }
  }
  if (*ierr != 0) {
    C2F(curblk).kfun = kfune;
    return;
  }
} /* cosend_ */

/* Subroutine */ void doit(told,urg)
     double *told;
     integer *urg;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, nord;

  static integer ierr1;
  static integer ii;

  /* Function Body */
  --(*urg);
  keve = *pointi;
  *pointi = evtspt[keve];
  evtspt[keve] = -1;

  nord = ordptr[keve + 1] - ordptr[keve];
  if (nord == 0) {
    return;
  }

  for (ii = ordptr[keve]; ii <=ordptr[keve + 1] - 1 ; ++ii) {
    C2F(curblk).kfun = ordclk[ii];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = ordclk[ii + nordclk];
      flag__ = 1;
      callf(told, xd, x, x,W,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
  }

  for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii];
    /*     .     Initialize tvec */
    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {
	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    Blocks[C2F(curblk).kfun - 1].nevout = 2;
	  } else {
	    Blocks[C2F(curblk).kfun - 1].nevout = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {

	  Blocks[C2F(curblk).kfun - 1].nevout= 
	    max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	++(*urg);
	i2 = Blocks[C2F(curblk).kfun - 1].nevout + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
      }
    }
  }
} /* doit_ */

/* Subroutine */ void cdoit(told)
     double *told;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__;
  static integer ierr1;

  static integer jj;
  static integer urg;



  /* Function Body */
  urg = 0;
  for (jj = 1; jj <= ncord; ++jj) {
    C2F(curblk).kfun = cord[jj];
    nclock = cord[jj + ncord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xd, x, x,W,&flag__);
	    
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    Blocks[C2F(curblk).kfun - 1].nevout = 2;
	  } else {
	    Blocks[C2F(curblk).kfun - 1].nevout = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  Blocks[C2F(curblk).kfun - 1].nevout= 
	    max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	++urg;
	i2 = Blocks[C2F(curblk).kfun - 1].nevout + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
      }
    }
  }

  while (urg > 0) {
    doit(told,&urg);
    if (*ierr != 0) {
      return;
    }
  }
} /* cdoit_ */

/* Subroutine */ void ddoit(told)
     double *told;

{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, kiwa;

  static integer i;
  static integer  ii, urg;


  /* Function Body */
  kiwa = 0;
  urg = 1;
 
  while (urg > 0) {
    edoit(told,&urg,&kiwa);
    if (*ierr != 0) {
      return;
    }
  }
  /*     .  update continuous and discrete states on event */
  if (kiwa == 0) {
    return;
  }
  for (i = 0; i < kiwa; ++i) {
    keve = iwa[i];
    i2 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i2; ++ii) {
      C2F(curblk).kfun = ordclk[ii];
      /*     If continuous state jumps, do cold restart */
      if (Blocks[C2F(curblk).kfun-1].nx+Blocks[C2F(curblk).kfun-1].nz > 0) {
	/*     Solution not satisfying but... Have to find a better test */
	/*     to know if state can jump. If we only leave the first test */
	/*     it sets hot to false at every event! */
	if (Blocks[C2F(curblk).kfun-1].nx > 0 && ordclk[ii + nordclk] != 0) {
	  hot = 0;
	}
	flag__ = 2;
	nclock=ordclk[ii + nordclk];
	callf(told, xd, x, x,W,&flag__);
		
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }
  }
} /* ddoit_ */

/* Subroutine */ void edoit(told,urg, kiwa)
     double *told;
     integer *urg, *kiwa;
{
  /* System generated locals */
  integer i2, i3;
  double d__1;

  /* Local variables */
  static integer flag__, keve;
  static integer nord;

  static integer ierr1, i;
  static integer ii;

  /* Function Body */
  --(*urg);
  keve = *pointi;
  *pointi = evtspt[keve];
  evtspt[keve] = -1;

  nord = ordptr[keve + 1] - ordptr[keve];
  if (nord == 0) {
    return;
  }
  iwa[*kiwa] = keve;
  ++(*kiwa);
  for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii];

    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = ordclk[ii + nordclk];
      flag__ = 1;
      callf(told, xd, x, x,W,&flag__);
	    
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
  }

  for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii ];
    /*     .     Initialize tvec */
    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] >= 0) {
	d__1 = *told - 1.;
	C2F(dset)(&Blocks[C2F(curblk).kfun - 1].nevout, &d__1, tvec, &c__1);

	flag__ = 3;
	nclock=ordclk[ii + nordclk];
	callf(told, xd, x, x ,W,&flag__);
		
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}

	if (Blocks[C2F(curblk).kfun - 1].nevout >= 1) {
	  for (i = 1; i <= Blocks[C2F(curblk).kfun - 1].nevout; ++i) {
	    if (tvec[i - 1] >= *told) {
	      i3 = i + clkptr[C2F(curblk).kfun] - 1;
	      addevs(&tvec[i - 1], &i3, &ierr1);
	      if (ierr1 != 0) {
		/*     !                 event conflict */
		*ierr = 3;
		return;
	      }
	    }
	  }
	}
      } else {
	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    Blocks[C2F(curblk).kfun - 1].nevout = 2;
	  } else {
	    Blocks[C2F(curblk).kfun - 1].nevout = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  Blocks[C2F(curblk).kfun - 1].nevout= 
	    max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	++(*urg);
	i2 = Blocks[C2F(curblk).kfun - 1].nevout + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
      }
    }
  }
} /* edoit_ */

/* Subroutine */ void odoit(residual, xt, xtd, told)
     double *residual, *xt, *xtd;
     double *told;

{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, kiwa;

  static integer ierr1, i;
  static integer ii, jj;
  static integer urg;

  /* Function Body */
  urg = 0;
  kiwa = 0;

  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xtd, xt, residual,W,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    Blocks[C2F(curblk).kfun - 1].nevout = 2;
	  } else {
	    Blocks[C2F(curblk).kfun - 1].nevout = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  Blocks[C2F(curblk).kfun - 1].nevout= 
	    max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	++urg;
	i2 = Blocks[C2F(curblk).kfun - 1].nevout + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
      }
    }
  }
  while (urg > 0) {
    ozdoit(xtd, xt,told, &urg, &kiwa);
  }
  /*     .  update states derivatives */
  for (ii = 1; ii <= noord; ++ii) {
    C2F(curblk).kfun = oord[ii];
    if (Blocks[C2F(curblk).kfun-1].nx > 0) {
      flag__ = 0;
      nclock = oord[ii + noord];
      callf(told, xtd, xt, residual,W,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
  }

  for (i = 0; i < kiwa; ++i) {
    keve = iwa[i];
    for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
      C2F(curblk).kfun = ordclk[ii ];
      if (Blocks[C2F(curblk).kfun-1].nx > 0) {
	flag__ = 0;
	nclock = ordclk[ii + nordclk];
	callf(told, xtd, xt, residual,W,&flag__);

	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }
  }
} /* odoit_ */

/* Subroutine */ void ozdoit(xtd, xt,told,urg,  kiwa)
     double *xtd, *xt;
     double *told;
     integer *urg, *kiwa;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, nord;

  static integer ierr1;
  static integer ii;

  /* Function Body */
  --(*urg);
  keve = *pointi;
  *pointi = evtspt[keve];
  evtspt[keve] = -1;

  nord = ordptr[keve + 1] - ordptr[keve];
  if (nord == 0) {
    return;
  }
  iwa[*kiwa] = keve;
  ++(*kiwa);

  for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = ordclk[ii + nordclk];
      flag__ = 1;
      callf(told, xtd, xt, xt,W,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
    /*     .     Initialize tvec */
    
    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    Blocks[C2F(curblk).kfun - 1].nevout = 2;
	  } else {
	    Blocks[C2F(curblk).kfun - 1].nevout = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  Blocks[C2F(curblk).kfun - 1].nevout= 
	    max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	++(*urg);
	i2 = Blocks[C2F(curblk).kfun - 1].nevout + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
      }
    }
  }
} /* ozdoit_ */

/* Subroutine */ void zdoit(g, xtd, xt,told)
     double *g;
     double *xtd, *xt;
     double *told;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, kiwa;

  static integer ierr1, i;
  static integer ii, jj;
  static integer urg;

  /* Function Body */
  urg = 0;
  kiwa = 0;
  for (jj = 1; jj <= nzord; ++jj) {
    C2F(curblk).kfun = zord[jj];
    nclock = zord[jj + nzord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xtd, xt, xt,W,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    Blocks[C2F(curblk).kfun - 1].nevout = 2;
	  } else {
	    Blocks[C2F(curblk).kfun - 1].nevout = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  Blocks[C2F(curblk).kfun - 1].nevout= 
	    max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	++urg;
	i2 = Blocks[C2F(curblk).kfun - 1].nevout + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
      }
    }
  }


  while (urg > 0) {
    ozdoit(xtd, xt,told, &urg, &kiwa);
  }
  /*     .  update zero crossing surfaces */
  for (ii = 1; ii <= nzord; ++ii) {
    C2F(curblk).kfun = zord[ii];
    if (Blocks[C2F(curblk).kfun-1].ng > 0) {
    flag__ = 9;
    nclock = oord[ii +nzord];
    callf(told, xtd, xt, xtd,g,&flag__);
    if (flag__ < 0) {
      *ierr = 5 - flag__;
      return;
    }
  }
}

for (i = 0; i < kiwa; ++i) {
  keve = iwa[i];
  for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii ];
    if (Blocks[C2F(curblk).kfun-1].ng > 0) {
    flag__ = 9;
    nclock = ordclk[ii + nordclk];
    callf(told, xtd, xt, xtd,g,&flag__);

    if (flag__ < 0) {
      *ierr = 5 - flag__;
      return;
    }
  }
}
}
} /* zdoit_ */






void 
callf(t,xtd,xt,residual,g,flag) 
     integer *flag;
     double *t,*xtd,*xt,*residual,*g;
     
{
  voidf loc ; 
  double* args[SZ_SIZE];
  integer sz[SZ_SIZE];
  double intabl[TB_SIZE],outabl[TB_SIZE];
  int ii,kf,in,out,ki,ko,ni,no,k,j;
  int lprt,szi,flagi;
  int solver=C2F(cmsolver).solver;
  int cosd=C2F(cosdebug).cosd;
  ScicosF0 loc0;
  ScicosF loc1;
  /*  ScicosFm1 loc3;*/
  ScicosF2 loc2;
  ScicosF2z loc2z;
  ScicosFi loci1;
  ScicosFi2 loci2;
  ScicosFi2z loci2z;
  ScicosF4 loc4;
  
  kf=C2F(curblk).kfun;
  if ( cosd > 1){
    sciprint("block %d is called ",kf);
    sciprint("with flag %d ",*flag);
    sciprint("at time %f \r\n",*t);
  }
  flagi=*flag; /* flag 7 implicit initialization */
  if(flagi==7 && Blocks[kf-1].type<10000) *flag=0;

  loc=Blocks[kf-1].funpt;
  if (Blocks[kf-1].type==4||Blocks[kf-1].type==10004) {
    Blocks[kf-1].time=*t;
    Blocks[kf-1].nevprt=nclock;
    loc4 = (ScicosF4) loc;
    if(Blocks[kf-1].nx==0){
      (*loc4)(&Blocks[kf-1],*flag);
    } 
    else {
      
      if(Blocks[kf-1].ng>0){
	Blocks[kf-1].g=&g[zcptr[kf]-1];
      }
      Blocks[kf-1].x=&xt[xptr[kf]-1];
      if(Blocks[kf-1].type==4) {
	if(*flag==0 && solver==100) {
	  Blocks[kf-1].xd=&residual[xptr[kf]-1];
	  (*loc4)(&Blocks[kf-1],*flag);
	  if(flagi!=7) {
	    for (k=0;k<Blocks[kf-1].nx;k++) {
	      Blocks[kf-1].res[k]=Blocks[kf-1].res[k]-Blocks[kf-1].xd[k];
	    }
	  }
	  else {
	    for (k=0;k<Blocks[kf-1].nx;k++) {
	      Blocks[kf-1].xd[k]=Blocks[kf-1].res[k];
	    } 
	  }
	}
	else {
	  Blocks[kf-1].xd=&xtd[xptr[kf]-1];
	  (*loc4)(&Blocks[kf-1],*flag);
	}
      }
      else {
	Blocks[kf-1].xd=&xtd[xptr[kf]-1];
	Blocks[kf-1].res=&residual[xptr[kf]-1];
	(*loc4)(&Blocks[kf-1],*flag);
      }
    }
    return;
  }
  
  C2F(scsptr).ptr=Blocks[kf-1].scsptr; /* set scilab function adress for sciblk */

  /*This is for compatibility*/
  if(nclock<0){
    for (j = zcptr[C2F(curblk).kfun]-1 ;j <zcptr[C2F(curblk).kfun+1]-1 ; ++j) {
      g[j]=(double)jroot[j];
    }
  }

  if(Blocks[kf-1].ztyp>0){
    Blocks[kf-1].g=&g[zcptr[kf]-1];
  }
  if(Blocks[kf-1].nx>0){
    Blocks[kf-1].x=&xt[xptr[kf]-1];
    Blocks[kf-1].xd=&xtd[xptr[kf]-1];
    if(solver==100) {
      Blocks[kf-1].res=&residual[xptr[kf]-1];
    }
  }

  switch (Blocks[kf-1].type) {

  case 1 :			
    /* one entry for each input or output */
    for (in = 0 ; in < Blocks[kf-1].nin ; in++) 
      {
	args[in]=Blocks[kf-1].inptr[in];
	sz[in]=Blocks[kf-1].insz[in];
      }
    for (out=0;out<Blocks[kf-1].nout;out++) {
      args[in+out]=Blocks[kf-1].outptr[out];
      sz[in+out]=Blocks[kf-1].outsz[out];
    }
    if(Blocks[kf-1].ztyp>0){
      Blocks[kf-1].g=&g[zcptr[kf]-1];
      args[Blocks[kf-1].nin+Blocks[kf-1].nout]=Blocks[kf-1].g;
      sz[Blocks[kf-1].nin+Blocks[kf-1].nout]=Blocks[kf-1].ng;
    }
    loc1 = (ScicosF) loc;
    if (solver==100) {
      (*loc1)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].x,&Blocks[kf-1].nx,
	      Blocks[kf-1].z,&Blocks[kf-1].nz,
	      tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	      Blocks[kf-1].ipar,&Blocks[kf-1].nipar,
	      (double *)args[0],&sz[0],
	      (double *)args[1],&sz[1],(double *)args[2],&sz[2],
	      (double *)args[3],&sz[3],(double *)args[4],&sz[4],
	      (double *)args[5],&sz[5],(double *)args[6],&sz[6],
	      (double *)args[7],&sz[7],(double *)args[8],&sz[8],
	      (double *)args[9],&sz[9],(double *)args[10],&sz[10],
	      (double *)args[11],&sz[11],(double *)args[12],&sz[12],
	      (double *)args[13],&sz[13],(double *)args[14],&sz[14],
	      (double *)args[15],&sz[15],(double *)args[16],&sz[16],
	      (double *)args[17],&sz[17]); 
    }
    else {
      (*loc1)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
	      Blocks[kf-1].z,&Blocks[kf-1].nz,
	      tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	      Blocks[kf-1].ipar,&Blocks[kf-1].nipar,
	      (double *)args[0],&sz[0],
	      (double *)args[1],&sz[1],(double *)args[2],&sz[2],
	      (double *)args[3],&sz[3],(double *)args[4],&sz[4],
	      (double *)args[5],&sz[5],(double *)args[6],&sz[6],
	      (double *)args[7],&sz[7],(double *)args[8],&sz[8],
	      (double *)args[9],&sz[9],(double *)args[10],&sz[10],
	      (double *)args[11],&sz[11],(double *)args[12],&sz[12],
	      (double *)args[13],&sz[13],(double *)args[14],&sz[14],
	      (double *)args[15],&sz[15],(double *)args[16],&sz[16],
	      (double *)args[17],&sz[17]);
    }
    break;   
  case 0 :			
    /* concatenated entries and concatened outputs */
    ni=0;
    /* catenate inputs if necessary */

    if (Blocks[kf-1].nin>1) {
      ki=0;
      for (in=0;in<Blocks[kf-1].nin;in++) {
	lprt=inplnk[inpptr[kf]+in];
	szi=lnkptr[lprt+1]-lnkptr[lprt];
	for (ii=0;ii<szi;ii++) 
	  intabl[ki++]=outtb[lnkptr[lprt]-1+ii];
	ni=ni+szi;
      }
      args[0]=&(intabl[0]);
    }
    else {
      if (Blocks[kf-1].nin==0) {
	ni=0;
	args[0]=&(outtb[0]);
      }
      else {
	lprt=inplnk[inpptr[kf]];
	args[0]=&(outtb[lnkptr[lprt]-1]);
	ni=lnkptr[lprt+1]-lnkptr[lprt];
      }
    }
    in=Blocks[kf-1].nin;
    
    /* catenate outputs if necessary */
    if (Blocks[kf-1].nout>1) {
      ko=0;
      for (out=0;out<Blocks[kf-1].nout;out++) {
	lprt=outlnk[outptr[kf]+out];
	szi=lnkptr[lprt+1]-lnkptr[lprt];
	
	for (ii=0;ii<szi;ii++)  
	  outabl[ko++]=outtb[lnkptr[lprt]-1+ii];
	no=no+szi;
      }
      args[1]=&(outabl[0]);
    }
    else {
      if (Blocks[kf-1].nout==0) {
	no=0;
	args[1]=&(outtb[0]);
      }
      else {
	lprt=outlnk[outptr[kf]];
	args[1]=&(outtb[lnkptr[lprt]-1]);
	no=lnkptr[lprt+1]-lnkptr[lprt];
      }
    }

    loc0 = (ScicosF0) loc;
    if (solver==100) {
      (*loc0)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].x,&Blocks[kf-1].nx,
	      Blocks[kf-1].z,&Blocks[kf-1].nz,
	      tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	      Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double *)args[0],&ni,
	      (double *)args[1],&no);
    }
    else {
      (*loc0)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
	      Blocks[kf-1].z,&Blocks[kf-1].nz,
	      tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	      Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double *)args[0],&ni,
	      (double *)args[1],&no);
    }
    
    /* split output vector on each port if necessary */
    if (Blocks[kf-1].nout>1) {
      ko=0;
      for (out=0;out<Blocks[kf-1].nout;out++) {
	lprt=outlnk[outptr[kf]+out];
	szi=lnkptr[lprt+1]-lnkptr[lprt];
	for (ii=0;ii<szi;ii++)  
	  outtb[lnkptr[lprt]-1+ii]=outabl[ko++];
      }
    }
    break;
  case 2 :			

    
    if (solver==100) {
      if (Blocks[kf-1].ztyp==0){
	loc2 = (ScicosF2) loc;
	(*loc2)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].x,&Blocks[kf-1].nx,
		Blocks[kf-1].z,&Blocks[kf-1].nz,
		tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
		Blocks[kf-1].insz,&Blocks[kf-1].nin,
		Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
      }
      else{
	loc2z = (ScicosF2z) loc;
	(*loc2z)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].x,&Blocks[kf-1].nx,
		 Blocks[kf-1].z,&Blocks[kf-1].nz,
		 tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		 Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,Blocks[kf-1].insz,&Blocks[kf-1].nin,
		 Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout,
		 Blocks[kf-1].g,&Blocks[kf-1].ng);
      }
    }
    else {
      if (Blocks[kf-1].ztyp==0){
	loc2 = (ScicosF2) loc;
	(*loc2)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		Blocks[kf-1].z,&Blocks[kf-1].nz,
		tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
		Blocks[kf-1].insz,&Blocks[kf-1].nin,
		Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
      }
      else{
	loc2z = (ScicosF2z) loc;
	(*loc2z)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		 Blocks[kf-1].z,&Blocks[kf-1].nz,
		 tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		 Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
		 Blocks[kf-1].insz,&Blocks[kf-1].nin,
		 Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout,
		 Blocks[kf-1].g,&Blocks[kf-1].ng);
      }
    }
    break;
  case 10001 :			
    /* implicit block one entry for each input or output */
    for (in = 0 ; in < Blocks[kf-1].nin ; in++) 
      for (in = 0 ; in < Blocks[kf-1].nin ; in++) 
	{
	  args[in]=Blocks[kf-1].inptr[in];
	  sz[in]=Blocks[kf-1].insz[in];
	}
    for (out=0;out<Blocks[kf-1].nout;out++) {
      args[in+out]=Blocks[kf-1].outptr[out];
      sz[in+out]=Blocks[kf-1].outsz[out];
    }
    if(Blocks[kf-1].ztyp>0){
      Blocks[kf-1].g=&g[zcptr[kf]-1];
      args[Blocks[kf-1].nin+Blocks[kf-1].nout]=Blocks[kf-1].g;
      sz[Blocks[kf-1].nin+Blocks[kf-1].nout]=Blocks[kf-1].ng;
    }
    loci1 = (ScicosFi) loc;

    (*loci1)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].xd,Blocks[kf-1].x,
	     &Blocks[kf-1].nx,Blocks[kf-1].z,&Blocks[kf-1].nz,
	     tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	     Blocks[kf-1].ipar,&Blocks[kf-1].nipar,
	     (double *)args[0],&sz[0],
	     (double *)args[1],&sz[1],(double *)args[2],&sz[2],
	     (double *)args[3],&sz[3],(double *)args[4],&sz[4],
	     (double *)args[5],&sz[5],(double *)args[6],&sz[6],
	     (double *)args[7],&sz[7],(double *)args[8],&sz[8],
	     (double *)args[9],&sz[9],(double *)args[10],&sz[10],
	     (double *)args[11],&sz[11],(double *)args[12],&sz[12],
	     (double *)args[13],&sz[13],(double *)args[14],&sz[14],
	     (double *)args[15],&sz[15],(double *)args[16],&sz[16],
	     (double *)args[17],&sz[17]); 
    break; 
  case 10002 :			
    /* implicit block, inputs and outputs given by a table of pointers */
   
    if(Blocks[kf-1].ztyp==0) {
      loci2 = (ScicosFi2) loc;
      
      (*loci2)(flag,&nclock,t,Blocks[kf-1].res,
	       Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
	       Blocks[kf-1].z,&Blocks[kf-1].nz,
	       tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	       Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
	       Blocks[kf-1].insz,&Blocks[kf-1].nin,
	       Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
    }
    else {
      loci2z = (ScicosFi2z) loc;
      
      (*loci2z)(flag,&nclock,t,Blocks[kf-1].res,
		Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		Blocks[kf-1].z,&Blocks[kf-1].nz,
		tvec,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,Blocks[kf-1].insz,&Blocks[kf-1].nin,
		Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout,
		Blocks[kf-1].g,&Blocks[kf-1].ng);
    }
    break;  
  default:
    sciprint("Undefined Function type\r\n");
    *flag=-1000;
    return;
  }
  if(solver==100 && Blocks[kf-1].type<10000 && *flag==0) { /* Implicit Solver */

    if(flagi!=7) {
      for (k=0;k<Blocks[kf-1].nx;k++) {
	Blocks[kf-1].res[k]=Blocks[kf-1].res[k]-Blocks[kf-1].xd[k];
      }
    }
    else {
      for (k=0;k<Blocks[kf-1].nx;k++) {
	Blocks[kf-1].xd[k]=Blocks[kf-1].res[k];
      } 
    }
  }
}

/* ? */

integer C2F(funnum)(fname)
     char * fname;
{
  int i=0,ln;
  integer loc=-1;
  while ( tabsim[i].name != (char *) NULL) {
    if ( strcmp(fname,tabsim[i].name) == 0 ) return(i+1);
    i++;
  }
  ln=strlen(fname);
  C2F(iislink)(fname,&loc);C2F(iislink)(fname,&loc);
  if (loc >= 0) return(ntabsim+(int)loc+1);
  return(0);
}



int C2F(simblk)(neq1, t, xc, xcdot)
     integer *neq1;
     double *t, *xc, *xcdot;
    
     
     /* 
	!purpose 
	compute state derivative of the continuous part
	!calling sequence 
	neq   : integer the size of the  continuous state
	t     : current time 
	xc    : double precision vector whose  contains the continuous state. 
	xcdot : double precision vector, contain the computed derivative 
	of the state 
     */
{ 
  C2F(ierode).iero = 0;
  odoit(xcdot, xc,xcdot,t); 
  C2F(ierode).iero = *ierr;
  return 0;
}
 
int C2F(simblkdassl)(t,xc,xcdot,cj,residual,ires,rpar1,ipar1)
     integer *ires,*ipar1;
     double *t, *xc, *xcdot, *rpar1, *residual;
     integer *cj;
     
     /* 
	!purpose 
	compute residual  of the continuous part
	!calling sequence 
	t     : current time 
	xc    : double precision vector whose  contains the continuous state. 
	xcdot : double precision vector, contain the computed derivative 
	of the state 
     */
{ 
  /*printf("t xc xcdot %f %f %f\n", *t, *xc,*xcdot);*/
  *ires=0;
  C2F(ierode).iero = 0;
  odoit(residual, xc, xcdot,t);
  C2F(ierode).iero = *ierr;
  if(C2F(ierode).iero != 0) *ires=-2;
  return 0;
  /*printf("after t xc xcdot %f %f %f\n", *t, *xc,*xcdot);
    printf("residual %f \n", *residual);  */  
}
 

int C2F(grblkdassl)(neq1, t, xc, xtd,ng1, g,rpar1,ipar1)
     integer *neq1;
     double *t, *xc, *xtd;
     integer *ng1,*ipar1;
     double *g,*rpar1;
{
  integer iig;

 
  C2F(ierode).iero = 0;
  zdoit(g, xtd, xc,t);
  C2F(ierode).iero = *ierr;
  if(*ng1>-1){
    for (iig=0;iig<*ng1;++iig){
      if (scicos_imp.mask[iig]>0){
	g[iig]=1;
      }
    }
  }
  
  return 0;
}


int C2F(grblk)(neq1, t, xc, ng1, g)
     integer *neq1;
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
       ng1   : size ng or -1 if without mask 
       !
     */
     
     /* Local variables */
     
{ integer iig;
 C2F(ierode).iero = 0;
 zdoit(g,xc, xc,t);
 C2F(ierode).iero = *ierr;
 
 /* printf("surface_g1 %f \n", g[0]);
    printf("surface_g1 %f \n\n", g[1]);*/
 if(*ng1>-1){
   for (iig=0;iig<*ng1;++iig){
     if (scicos_imp.mask[iig]>0){
       g[iig]=1;
     }
   }
 }
 return 0;
}


/* Subroutine */ void addevs(t, evtnb, ierr1)
     double *t;integer *evtnb, *ierr1;
{
  static integer i, j;

  /* Function Body */
  *ierr1 = 0;
  if (evtspt[*evtnb] != -1) {
    *ierr1 = 1;
    return;
  } else {
    evtspt[*evtnb] = 0;
    tevts[*evtnb] = *t;
  }
  if (*pointi == 0) {
    *pointi = *evtnb;
    return;
  }
  if (*t < tevts[*pointi]) {
    evtspt[*evtnb] = *pointi;
    *pointi = *evtnb;
    return;
  }
  i = *pointi;

 L100:
  if (evtspt[i] == 0) {
    evtspt[i] = *evtnb;
    return;
  }
  if (*t >= tevts[evtspt[i]]) {
    j = evtspt[i];
    if (evtspt[j] == 0) {
      evtspt[j] = *evtnb;
      return;
    }
    i = j;
    goto L100;
  } else {
    evtspt[*evtnb] = evtspt[i];
    evtspt[i] = *evtnb;
  }
} /* addevs */

/* Subroutine */ void putevs(t, evtnb, ierr1)
     double *t;
     integer *evtnb, *ierr1;
{

  /* Function Body */
  *ierr1 = 0;
  if (evtspt[*evtnb] != -1) {
    *ierr1 = 1;
    return;
  } else {
    evtspt[*evtnb] = 0;
    tevts[*evtnb] = *t;
  }
  if (*pointi == 0) {
    *pointi = *evtnb;
    return;
  }
  evtspt[*evtnb] = *pointi;
  *pointi = *evtnb;
} /* putevs */

