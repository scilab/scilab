
#include <string.h>
#include "../machine.h"
#include "../sun/link.h"
#include "scicos.h"
#include "import.h"
#include "blocks.h"


#ifdef FORDLL 
#define IMPORT  __declspec (dllimport)
#else 
#define IMPORT extern
#endif


#define abs(x) ((x) >= 0 ? (x) : -(x))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))


int cosini(double *);
int cosiord(double *);
int cosend(double *);
int cdoit(double *);
int doit(double *,integer *);
int ddoit(double *);
int edoit(double *,integer *,integer *);
int oodoit(double *,double *,double *,integer *,integer *);
int odoit(double *,double *,double *,double *);
int zzdoit(double *,double *,double *,integer *,integer *);
int zdoit(double *,double *,double *,double *);
int cossimdassl(double *);
int cossim(double *);
void callf(double *, double *, double *, double *,double *,integer *);
int C2F(simblk)(integer *, double *, double *, double *);
int C2F(simblkdassl)(double *, double *, double *, integer *, double *, integer *, double *, integer *);
int C2F(grblk)(integer *, double *, double *, integer *, double *);
int C2F(grblkdassl)(integer *, double *, double *, double *, integer *, double *, double *, integer *);
int addevs(double *,integer *,integer *);
int putevs(double *,integer *,integer *);


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

static  double atol, rtol, ttol, deltat;
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
static integer ntvec;

static double *W; /* space for coding g and outtb-bis for fixed point iteration */

static integer *iwa;

static integer *xptr,*zptr, *iz, *izptr,*evtspt;
static integer  *funptr, *funtyp, *inpptr, *outptr, *inplnk, *outlnk, *lnkptr;
static integer *rpptr, *ipar, *ipptr, *clkptr, *ordptr, *ordclk, *cord, 
	 *iord, *oord,  *zord,  *critev, *ztyp, *zcptr;
static integer *pointi;
static integer *ierr;

static double *x,*xd,*z__,*tevts,*outtb, *rpar  ;

static double *rhot;
static integer *ihot;

static integer *jroot;

static double *t0,*tf;


/* Subroutine */ int C2F(scicos)(x_in, xptr_in, z_in, zptr_in, iz_in, izptr_in, t0_in, tf_in, tevts_in, 
	evtspt_in, nevts, pointi_in, outtb_in, nout1, funptr_in, funtyp_in, inpptr_in, outptr_in, 
	inplnk_in, outlnk_in, lnkptr_in, nlnkptr, rpar_in, rpptr_in, ipar_in, ipptr_in, clkptr_in, 
	ordptr_in, nordptr1, ordclk_in, cord_in, ncord1, iord_in, niord1, oord_in, noord1, 
	zord_in, nzord1, critev_in, nblk1, ztyp_in, zcptr_in, ng1, subscr, nsubs, simpar, 
	w, iw, iwa_in, flag__, ierr_out)
double *x_in;
integer *xptr_in;
double *z_in;
integer *zptr_in, *iz_in, *izptr_in;
double *t0_in, *tf_in, *tevts_in;
integer *evtspt_in, *nevts, *pointi_in;
double *outtb_in;
integer *nout1, *funptr_in, *funtyp_in, *inpptr_in, *outptr_in, *inplnk_in, *outlnk_in, *lnkptr_in,
	 *nlnkptr;
double *rpar_in;
integer *rpptr_in, *ipar_in, *ipptr_in, *clkptr_in, *ordptr_in, *nordptr1, *ordclk_in, *cord_in, *
	ncord1, *iord_in, *niord1, *oord_in, *noord1, *zord_in, *nzord1, *critev_in, *
	nblk1, *ztyp_in, *zcptr_in, *ng1, *subscr, *nsubs;
double *simpar, *w;
integer *iw, *iwa_in, *flag__, *ierr_out;
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    extern /* Subroutine */ int C2F(msgs)();
    static integer mxtb, ierr0, kfun0, i__, j, k;
    extern /* Subroutine */ int C2F(makescicosimport)();
    static integer lmask, lihot, lrhot;
    extern /* Subroutine */ int C2F(getscsmax)();
    static integer ni, no;
    extern /* Subroutine */ int C2F(clearscicosimport)();
    static integer lw, nx, nz;
    static integer maxord;
    static integer louttb, ljroot, liw, lww;

/*     Copyright INRIA */
/* iz,izptr are used to pass block labels */

    t0=t0_in;
    tf=tf_in;
    ierr=ierr_out;

    iwa=--iwa_in;

    /* Parameter adjustments */
    pointi=pointi_in;
    x=x_in;
    xptr=--xptr_in;
    z__=z_in;
    zptr=--zptr_in;
    iz=iz_in;
    izptr=--izptr_in;
    evtspt=--evtspt_in;
    tevts=--tevts_in;
    outtb=outtb_in;
    funptr=funptr_in;
    funtyp=--funtyp_in;
    inpptr=--inpptr_in;
    outptr=--outptr_in;
    inplnk=--inplnk_in;
    outlnk=--outlnk_in;
    lnkptr=--lnkptr_in;
    rpar=rpar_in;
    rpptr=--rpptr_in;
    ipar=ipar_in;
    ipptr=--ipptr_in;
    clkptr=--clkptr_in;
    ordptr=--ordptr_in;
    ordclk=--ordclk_in;
    cord=--cord_in;
    iord=--iord_in;
    oord=--oord_in;
    zord=--zord_in;

    critev=--critev_in;
    ztyp=--ztyp_in;
    zcptr=--zcptr_in;
    --subscr;
    --simpar;
    --w;
    --iw;

    /* Function Body */
    atol = simpar[1];
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
	i__1 = 16, i__2 = nx + 9;
	nrwp = nx * max(i__1,i__2) + 22 + ng * 3;
	niwp = nx + 20;
    } else if (C2F(cmsolver).solver == 100) {
	maxord = 5;
/* Computing MAX */
	i__1 = maxord + 4;
/* Computing 2nd power */
	i__2 = nx;
	nrwp = max(i__1,7) * nx + 60 + i__2 * i__2 + ng * 3;
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
    --jroot;
    liw = ljroot + (ng << 1);
    lmask = liw;
    liw = lmask + ng;
/*     . lmask is the zero-crossing mask */

    i__1 = nblk;
    for (i__ = 1; i__ <= i__1; ++i__) {
	if (funtyp[i__] < 10000) {
	    funtyp[i__] %= 1000;
	} else {
	    funtyp[i__] = funtyp[i__] % 1000 + 10000;
	}
	ni = inpptr[i__ + 1] - inpptr[i__];
	no = outptr[i__ + 1] - outptr[i__];
	if (funtyp[i__] == 1) {
	    if (ni + no > 11) {
/*     hard coded maxsize in callf.c */
		C2F(msgs)(&c__90, &c__0);
		C2F(curblk).kfun = i__;
		*ierr = i__ + 1005;
		return 0;
	    }
	} else if (funtyp[i__] == 2 || funtyp[i__] == 3) {
/*     hard coded maxsize in scicos.h */
	    if (ni + no > SZ_SIZE) {
		C2F(msgs)(&c__90, &c__0);
		C2F(curblk).kfun = i__;
		*ierr = i__ + 1005;
		return 0;
	    }
	}
	mxtb = 0;
	if (funtyp[i__] == 0) {
	    if (ni > 1) {
		i__2 = ni;
		for (j = 1; j <= i__2; ++j) {
		    k = inplnk[inpptr[i__] - 1 + j];
		    mxtb = mxtb + lnkptr[k + 1] - lnkptr[k];
		}
	    }
	    if (no > 1) {
		i__2 = no;
		for (j = 1; j <= i__2; ++j) {
		    k = outlnk[outptr[i__] - 1 + j];
		    mxtb = mxtb + lnkptr[k + 1] - lnkptr[k];
		}
	    }
	    if (mxtb > TB_SIZE) {
		C2F(msgs)(&c__91, &c__0);
		C2F(curblk).kfun = i__;
		*ierr = i__ + 1005;
		return 0;
	    }
	}
    }

    C2F(makescicosimport)(x, &xptr[1], &zcptr[1], z__, &zptr[1], iz, &
	    izptr[1], &inpptr[1], &inplnk[1], &outptr[1], &outlnk[1], &lnkptr[
	    1], nlnkptr, rpar, &rpptr[1], ipar, &ipptr[1], &
	    nblk, outtb, &nout, &subscr[1], nsubs, &
	    tevts[1], &evtspt[1], nevts, pointi, &oord[1], &zord[
	    1], funptr, &funtyp[1], &ztyp[1], &cord[1]
	    , &ordclk[1], &clkptr[1], &ordptr[1], &critev[1], &iwa[1], &iw[
	    lmask]);
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
	if (*ierr != 0) {
	    kfun0 = C2F(curblk).kfun;
	    goto L2345;
	}
	C2F(simblk)(&nx, t0, x, &w[1]);
	i__1 = nx;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    x[i__] = w[i__];
	}
    }
L2345:
    C2F(clearscicosimport)();
} /* scicos_ */

/* Subroutine */ int cosini(told)

double *told;

{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer flag__;
    static integer i__;

    static integer kfune;
    static integer jj;

    /* Function Body */
    *ierr = 0;
/*     initialization (flag 4) */
/*     loop on blocks */
    tvec[0] = 0.;
    ntvec = 0;
    C2F(dset)(&nout, &c_b14, W, &c__1);
    nclock = 0;
    i__1 = nblk;
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i__1; ++C2F(curblk).kfun) {
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
	return 0;
    }
/*     initialization (flag 6) */
    nclock = 0;
    tvec[0] = 0.;
    ntvec = 0;
    if (ncord > 0) {
	i__1 = ncord;
	for (jj = 1; jj <= i__1; ++jj) {
	  C2F(curblk).kfun = cord[jj];
	    flag__ = 6;
	    if (funtyp[C2F(curblk).kfun] >= 0) {
	      callf(told, xd, x, x,W,&flag__);
		if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    return 0;
		}
	    }
	}
    }

/*     point-fix iterations */
    nclock =0;
    i__1 = nblk + 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
/*     loop on blocks */
	i__2 = nblk;
	for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i__2; ++C2F(curblk).kfun) {
	    flag__ = 6;
	    if (funtyp[C2F(curblk).kfun] >= 0) {
	      callf(told, xd, x, x,W,&flag__);
		if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    return 0;
		}
	    }
	}

	nclock = 0;
	tvec[0] = 0.;
	ntvec = 0;
	if (ncord > 0) {
	    i__2 = ncord;
	    for (jj = 1; jj <= i__2; ++jj) {
		C2F(curblk).kfun = cord[jj];
		flag__ = 6;
		if (funtyp[C2F(curblk).kfun] >= 0) {
		  callf(told, xd, x, x,W,&flag__);
		    if (flag__ < 0) {
			*ierr = 5 - flag__;
			return 0;
		    }
		}
	    }
	}
	i__2 = nout-1;
	for (jj = 0; jj <= i__2; ++jj) {
	    if (outtb[jj] != W[jj]) {
		goto L30;
	    }
	}
	return 0;
L30:
	C2F(dcopy)(&nout, outtb, &c__1, W, &c__1);

    }
    *ierr = 20;
    return 0;
} /* cosini_ */

/* Subroutine */ int cosiord(told)
double *told;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer flag__;
    static integer jj;

/*     Copyright INRIA */


/* ..   Parameters .. */
/*     maximum number of clock output for one block */

/*     neq must contain after #states all integer data for simblk and grblk */
/*     X must contain after state values all real data for simblk and grblk */
    /* Parameter adjustments */

    /* Function Body */
    ntvec = 0;
/*     initialisation (propagation of constant blocks outputs) */
    if (niord == 0) {
	goto L10;
    }
    i__1 = niord;
    for (jj = 1; jj <= i__1; ++jj) {
	C2F(curblk).kfun = iord[jj];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = iord[jj + niord];
	    flag__ = 1;
	    callf(told, xd, x, x,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }
L10:
    return 0;
} /* cosiord_ */

/* Subroutine */ int cossim(told)
double *told;

{
    /* Initialized data */
    static integer otimer = 0;
    /* System generated locals */
    integer i__1, i__2, i__3;
    double d__1, d__2, d__3, d__4;

    /* Local variables */
    static integer flag__, jdum;
    static integer iopt;

    static integer ierr1;
    static integer j, k;
    static double t;
    static integer itask;
    static integer ib, ig, jj, jt;
    static integer istate, ntimer;

    static double rhotmp;
    static integer inxsci;

    static integer kpo, ksz, kev;



    /* Function Body */

    C2F(coshlt).halt = 0;
    *ierr = 0;
    hot = 0;
    C2F(xscion)(&inxsci);
/*     initialization */
    C2F(iset)(&niwp, &c__0, &ihot[1], &c__1);
    C2F(dset)(&nrwp, &c_b14, &rhot[1], &c__1);
    C2F(realtimeinit)(told, &C2F(rtfactor).scale);
    ntvec = 0;
/*    .  saving zcross block number in jroot(ng+xx) */
    jj = 0;
    i__1 = nblk;
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i__1; ++C2F(curblk).kfun) {
	if (zcptr[C2F(curblk).kfun + 1] - zcptr[C2F(curblk).kfun] >= 1) {
	    ++jj;
	    jroot[ng + jj] = C2F(curblk).kfun;
	}
    }
/*     . Il fault:  ng >= jj */
    if (jj != ng) {
	jroot[ng + jj + 1] = -1;
    }
/*     .  saving zcross block number in jroot(ng+xx) */
/*     Initializing the zero crossing mask */
    C2F(iset)(&ng, &c__0, &jroot[(ng << 1) + 1], &c__1);

/*     initialisation (propagation of constant blocks outputs) */
    if (niord == 0) {
	goto L10;
    }
    i__1 = niord;
    for (jj = 1; jj <= i__1; ++jj) {
	C2F(curblk).kfun = iord[jj];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = iord[jj + niord];
	    flag__ = 1;
	    callf(told, xd , x, x,W,&flag__);
	    
	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }
/*     main loop on time */
L10:
    if (*told >= *tf) {
	return 0;
    }
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
	return 0;
    }
    if (*pointi == 0) {
	t = *tf;
    } else {
	t = tevts[*pointi];
    }
    if ((d__1 = t - *told, abs(d__1)) < ttol) {
	t = *told;
/*     update output part */
    }
    if (*told > t) {
/*     !  scheduling problem */
	*ierr = 1;
	return 0;
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
	    if (ncord == 0) {
		goto L343;
	    }
	    if (*told >= *tf) {
/*     .     we are at the end, update continuous part before leaving */
		cdoit(told);
/*     .        if(ierr.ne.0) return */
		return 0;
	    }
L343:

	    ;
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
/* Computing MIN */
/* Computing MIN */
	    d__3 = t, d__4 = *tf + ttol;
	    d__1 = *told + deltat, d__2 = min(d__3,d__4);
	    t = min(d__1,d__2);


	    if (C2F(cosdebug).cosd >= 3) {
	      sciprint("****lsodar from: %f to %f\r\n", *told, t);
	    }
	    C2F(lsodar)(C2F(simblk), neq, x, told, &t, &c__1, &rtol, 
		    &atol, &itask, &istate, &iopt, &rhot[1], &
		    nrwp, &ihot[1], &niwp, &jdum, &jt, 
		    C2F(grblk), &ng, &jroot[1]);
	    if (C2F(cosdebug).cosd >= 3) {
	      sciprint("****lsodar reached: %f",*told);
	    }
	    if (istate <= 0) {
		if (istate == -3) {
		    C2F(grblk)(neq, told, x, &ng, W);
		    i__1 = ng;
		    for (ib = 1; ib <= i__1; ++ib) {
			if (W[ib-1] == 0.) {
			    jroot[(ng << 1) + ib] = 1;
			}
		    }
		    hot = 0;
		    itask = 2;
		    rhot[5] = ttol;
		    istate = 1;
		    C2F(lsodar)(C2F(simblk), neq, x, told, &t, &c__1, &
			    rtol, &atol, &itask, &istate, &
			    c__1, &rhot[1], &nrwp, &ihot[1], &
			    niwp, &jdum, &jt, C2F(grblk), &ng, &
			    jroot[1]);
		    rhot[5] = (float)0.;
		    hot = 0;
		    if (istate <= 0) {
/*     !           integration problem */
			*ierr = 100 - istate;
			return 0;
		    }
		} else {
/*     !        integration problem */
		    *ierr = 100 - istate;
		    return 0;
		}
	    } else {
		hot = 1;
	    }
/*     Initializing the zero crossing mask */
	    C2F(grblk)(neq, told, x, &c_n1, W);
	    i__1 = ng;
	    for (ib = 1; ib <= i__1; ++ib) {
		if (W[ib-1] != 0. && jroot[(ng << 1) + ib] == 1) {
		    hot = 0;
		    jroot[(ng << 1) + ib] = 0;
		}
	    }
/*     .     update outputs of 'c' type  blocks */
	    nclock = 0;
	    ntvec = 0;
	    if (ncord > 0) {
		if (*told >= *tf) {
/*     .        we are at the end, update continuous part before leaving */
		    cdoit(told);
/*                  if(ierr.ne.0) return */
		    return 0;
		}
	    }
	    if (istate == 3) {
		hot = 0;
		if (C2F(cosdebug).cosd >= 3) {
		  sciprint("root found at t=: %f\r\n",*told);
		}
/*     .        update outputs affecting ztyp blocks */
		C2F(grblk)(neq, told, x, &c_n1, W);
/*     .        at a least one root has been found */
		ig = 1;
		i__1 = ng;
		for (jj = 1; jj <= i__1; ++jj) {
		    C2F(curblk).kfun = jroot[ng + jj];
		    if (C2F(curblk).kfun == -1) {
			goto L51;
		    }
/*     .       boucle sur les zcross blocks, ils ont sauves dans jroot(ng+xx) */
/*     . */

		    ksz = zcptr[C2F(curblk).kfun + 1] - zcptr[C2F(curblk).kfun];
		    ig += ksz;
		    ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
/*     .           ............................................... */
		    kev = 0;
		    i__2 = ksz;
		    for (j = 1; j <= i__2; ++j) {
			kev = (kev << 1) + (i__3 = jroot[ig - j], abs(i__3));
		    }
		    if (kev != 0) {
			i__2 = ksz;
			for (j = 1; j <= i__2; ++j) {
			    kev <<= 1;
			    if (jroot[ig - j] == -1) {
				++kev;
			    }
			}
			
			ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[
				C2F(curblk).kfun];
			if (ntvec > 0) {
			  flag__ = 3;
			  /* call corresponding block to determine output event (kev) */
			  nclock = -kev;
			  callf(told, xd, x, x,W,&flag__);
			  
			  if (flag__ < 0) {
			    *ierr = 5 - flag__;
			    return 0;
			  }
			  /*     .              update event agenda */
			  i__2 = ntvec;
			  for (k = 1; k <= i__2; ++k) {
			    if (tvec[k - 1] >= *told) {
			      if (critev[k + clkptr[C2F(curblk).kfun] - 1] 
				  == 1) {
				hot = 0;
			      }
			      i__3 = k + clkptr[C2F(curblk).kfun] - 1;
			      addevs(&tvec[k - 1], &i__3, &
				      ierr1);
			      if (ierr1 != 0) {
				/*     .                       nevts too small */
				*ierr = 3;
				return 0;
			      }
			    }
			  }
			}
			  /*     .              update state */
			
			if (xptr[C2F(curblk).kfun+1] - xptr[C2F(curblk).kfun] + 
			    zptr[C2F(curblk).kfun + 1] - zptr[C2F(curblk).kfun] 
			    > 0) {
			  /*     .              call corresponding block to update state */
			  flag__ = 2;
			  nclock = -kev;
			  callf(told, xd, x, x,W,&flag__);
			  
			  if (flag__ < 0) {
			    *ierr = 5 - flag__;
			    return 0;
			  }
			}
		    }
		}
	    L51:
		;
	    }
/*     !     save initial signs of zero crossing surface */

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
	    return 0;
	}

    }
/*     end of main loop on time */
    goto L10;
} /* cossim_ */



/* Subroutine */ int cossimdassl(told)
double *told;
{
    /* Initialized data */
    static integer otimer = 0;
    /* System generated locals */
    integer i__1, i__2, i__3;
    double d__1, d__2, d__3, d__4;

    /* Local variables */
    static integer flag__, jdum;
    static integer iopt, info[20];

    static integer ierr1;
    static integer j, k;
    static double t;
    static integer ib, ig, jj, jt;
    static integer istate, ntimer;
    static double rhotmp;
    static integer inxsci;
    static integer kpo, ksz, kev;


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
    ntvec = 0;
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
    i__1 = *neq;
    for (jj = 1; jj <= i__1; ++jj) {
	ihot[jj + 40] = 1;
    }
/*    .  saving zcross block number in jroot(ng+xx) */
    jj = 0;
    i__1 = nblk;
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i__1; ++C2F(curblk).kfun) {
	if (zcptr[C2F(curblk).kfun + 1] - zcptr[C2F(curblk).kfun] >= 1) {
	    ++jj;
	    jroot[ng + jj] = C2F(curblk).kfun;
	}
    }
/*     . Il fault:  ng >= jj */
    if (jj != ng) {
	jroot[ng + jj + 1] = -1;
    }
/*    .  saving zcross block number in jroot(ng+xx) */
/*     Initializing the zero crossing mask */
    C2F(iset)(&ng, &c__0, &jroot[(ng << 1) + 1], &c__1);

/*     initialisation (propagation of constant blocks outputs) */
    if (niord == 0) {
	goto L10;
    }
    i__1 = niord;
    for (jj = 1; jj <= i__1; ++jj) {
	C2F(curblk).kfun = iord[jj];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = iord[jj + niord];
	    flag__ = 1;
	    callf(told, xd, x, x,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }
/*     main loop on time */
L10:
    if (*told >= *tf) {
	return 0;
    }
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
	return 0;
    }
    if (*pointi == 0) {
	t = *tf;
    } else {
	t = tevts[*pointi];
    }
    if ((d__1 = t - *told, abs(d__1)) < ttol) {
	t = *told;
/*     update output part */
    }
    if (*told > t) {
/*     !  scheduling problem */
	*ierr = 1;
	return 0;
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
	    if (ncord == 0) {
		goto L343;
	    }
	    if (*told >= *tf) {
/*     .     we are at the end, update continuous part before leaving */
/*               hot=info(1).eq.1 */
		cdoit(told);
		return 0;
	    }
L343:

	    ;
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

	    d__3 = t, d__4 = *tf + ttol;
	    d__1 = *told + deltat, d__2 = min(d__3,d__4);
	    t = min(d__1,d__2);

	    if (info[0] == 0) {
		i__1 = nblk;
		for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i__1; 
			++C2F(curblk).kfun) {
		    if (xptr[C2F(curblk).kfun+1] - xptr[C2F(curblk).kfun] > 0) {
			flag__ = 7;
			nclock = 0;
			callf(told, xd, x, xd,W,&flag__);

			if (flag__ < 0) {
			    *ierr = 5 - flag__;
			    return 0;
			}
		    }
		}
	    }
	    if (C2F(cosdebug).cosd >= 3) {
	      sciprint("****daskr from: %f to %f\r\n", *told, t);
	    }
/*     Warning rpar and ipar are used here as dummy pointers */
	    C2F(ddaskr)(C2F(simblkdassl), neq, told, x, xd, &t, 
		    info, &rtol, &atol, &istate, &rhot[1], &
		    nrwp, &ihot[1], &niwp, rpar, ipar
		    , &jdum, rpar, C2F(grblkdassl), &ng, &jroot[1])
		    ;
	    if (C2F(cosdebug).cosd >= 3) {
	      sciprint("****daskr reached: %f\r\n",*told);
	    }
	    if (istate <= 0) {
		if (istate == -33) {
/*     call dset(ng,0.d0,w,1) */
		    C2F(grblkdassl)(neq, told, x,xd, &ng, W,rpar,ipar);  /*rpar,ipar used as dummy*/
		    i__1 = ng;
		    for (ib = 1; ib <= i__1; ++ib) {
			if (W[ib-1] == 0.) {
			    jroot[(ng << 1) + ib] = 1;
			}
		    }
		    info[0] = 0;
		    info[2] = 1;
		    d__1 = *told + ttol;
		    C2F(ddaskr)(C2F(simblkdassl), neq, told, x, xd
			    , &d__1, info, &rtol, &atol, &
			    istate, &rhot[1], &nrwp, &ihot[1], &
			    niwp, rpar, ipar, &jdum, rpar
			    , C2F(grblkdassl), &ng, &jroot[1]);
		    info[0] = 0;
		    if (istate <= 0) {
/*     !           integration problem */
			*ierr = 100 - istate;
			return 0;
		    }
		} else {
/*     !        integration problem */
		    *ierr = 100 - istate;
		    return 0;
		}
	    } else {
		info[0] = 1;
	    }

/*     Initializing the zero crossing mask */
	    C2F(grblkdassl)(neq, told, x,xd, &c_n1, W,rpar,ipar);  /*rpar,ipar used as dummy*/
	    i__1 = ng;
	    for (ib = 1; ib <= i__1; ++ib) {
		if (W[ib-1] != 0. && jroot[(ng << 1) + ib] == 1) {
		    info[0] = 0;
		    jroot[(ng << 1) + ib] = 0;
		}
	    }
/*     .     update outputs of 'c' type  blocks */
	    nclock = 0;
	    ntvec = 0;
	    if (ncord > 0) {
		if (*told >= *tf) {
/*     .        we are at the end, update continuous part before leaving */
		    cdoit(told);
		    return 0;
		}
	    }
	    if (istate == 5) {
		info[0] = 0;
		if (C2F(cosdebug).cosd >= 3) {
		  sciprint("root found at t=: %f\r\n",*told);
		}
/*     .        update outputs affecting ztyp blocks */
		C2F(grblkdassl)(neq, told, x,xd, &c_n1, W,rpar,ipar);  /*rpar,ipar used as dummy*/
/*     .        at a least one root has been found */
		ig = 1;
		i__1 = ng;
		for (jj = 1; jj <= i__1; ++jj) {
		    C2F(curblk).kfun = jroot[ng + jj];
		    if (C2F(curblk).kfun == -1) {
			goto L51;
		    }
/*     .       boucle sur les zcross blocks, ils ont sauves dans jroot(ng+xx) */

		    ksz = zcptr[C2F(curblk).kfun + 1] - zcptr[C2F(curblk).kfun];
		    ig += ksz;
		    ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
/*     .           ............................................... */
		    kev = 0;
		    i__2 = ksz;
		    for (j = 1; j <= i__2; ++j) {
			kev = (kev << 1) + (i__3 = jroot[ig - j], abs(i__3));
		    }
		    if (kev != 0) {
			i__2 = ksz;
			for (j = 1; j <= i__2; ++j) {
			    kev <<= 1;
			    if (jroot[ig - j] == -1) {
				++kev;
			    }
			}
			flag__ = 3;
			ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[
				C2F(curblk).kfun];
			if (ntvec > 0) {
/*     .              call corresponding block to determine output event (kev) */
			    nclock = -kev;
			    callf(told, xd, x, x,W,&flag__);
			    if (flag__ < 0) {
				*ierr = 5 - flag__;
				return 0;
			    }
/*     .              update event agenda */
			    i__2 = ntvec;
			    for (k = 1; k <= i__2; ++k) {
				if (tvec[k - 1] >= *told) {
				    if (critev[k + clkptr[C2F(curblk).kfun] - 1] 
					    == 1) {
					info[0] = 0;
				    }
				    i__3 = k + clkptr[C2F(curblk).kfun] - 1;
				    addevs(&tvec[k - 1], &i__3, &
					    ierr1);
				    if (ierr1 != 0) {
/*     .                       nevts too small */
					*ierr = 3;
					return 0;
				    }
				}
			    }
			}
/*     .              update state */
			flag__ = 2;
			if (xptr[C2F(curblk).kfun+1] - xptr[C2F(curblk).kfun] + 
				zptr[C2F(curblk).kfun + 1] - zptr[C2F(curblk).kfun] 
				> 0) {
/*     .              call corresponding block to update state */
			    nclock = -kev;
			    callf(told, xd, x, x,W,&flag__);
			    if (flag__ < 0) {
				*ierr = 5 - flag__;
				return 0;
			    }
			}

		    }
		}
L51:
		;
	    }
/*     !     save initial signs of zero crossing surface */

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
	    return 0;
	}
	if (! hot) {
	    info[0] = 0;
	}

    }
/*     end of main loop on time */
    goto L10;
} /* cossimdassl_ */



/* Subroutine */ int cosend(told)
double *told;
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer flag__;

    static integer kfune;

    /* Function Body */
    *ierr = 0;
/*     loop on blocks */
    tvec[0] = 0.;
    ntvec = 0;
    nclock=0;
    i__1 = nblk;
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= i__1; ++C2F(curblk).kfun) {
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
	return 0;
    }
    return 0;
} /* cosend_ */

/* Subroutine */ int doit(told,urg)
double *told;
integer *urg;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

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
	return 0;
    }

    i__1 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i__1; ++ii) {
	C2F(curblk).kfun = ordclk[ii];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = ordclk[ii + nordclk];
	    flag__ = 1;
	    callf(told, xd, x, x,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }

    i__1 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i__1; ++ii) {
	C2F(curblk).kfun = ordclk[ii];
/*     .     Initialize tvec */
	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] < 0) {
		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++(*urg);
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }
} /* doit_ */

/* Subroutine */ int cdoit(told)
double *told;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer flag__;
    static integer ierr1;

    static integer jj;
    static integer urg;



    /* Function Body */
    urg = 0;
    i__1 = ncord;
    for (jj = 1; jj <= i__1; ++jj) {
	C2F(curblk).kfun = cord[jj];
	nclock = cord[jj + ncord];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    flag__ = 1;
	    callf(told, xd, x, x,W,&flag__);
	    
	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}

	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] < 0) {

		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++urg;
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }
    if (urg <= 0) {
	return 0;
    }
L20:
    doit(told,&urg);
    if (*ierr != 0) {
	return 0;
    }
    if (urg > 0) {
	goto L20;
    }
    return 0;
} /* cdoit_ */

/* Subroutine */ int ddoit(told)
double *told;

{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    static integer flag__, keve, kiwa;

    static integer i__;
    static integer  ii, urg;


    /* Function Body */
    kiwa = 0;
    urg = 1;
    if (urg > 0) {
L43:
	edoit(told,&urg,&kiwa);
	if (*ierr != 0) {
	    return 0;
	}
	if (urg > 0) {
	    goto L43;
	}
    }
/*     .  update continuous and discrete states on event */
    if (kiwa == 0) {
	return 0;
    }
    i__1 = kiwa;
    for (i__ = 1; i__ <= i__1; ++i__) {
	keve = iwa[i__];
	i__2 = ordptr[keve + 1] - 1;
	for (ii = ordptr[keve]; ii <= i__2; ++ii) {
	    C2F(curblk).kfun = ordclk[ii];
/*     If continuous state jumps, do cold restart */
	    if (xptr[C2F(curblk).kfun+1] - xptr[C2F(curblk).kfun] + zptr[
		    C2F(curblk).kfun + 1] - zptr[C2F(curblk).kfun] > 0) {
/*     Solution not satisfying but... Have to find a better test */
/*     to know if state can jump. If we only leave the first test */
/*     it sets hot to false at every event! */
		if (xptr[C2F(curblk).kfun+1] - xptr[C2F(curblk).kfun] > 0 && 
			ordclk[ii + nordclk] != 0) {
		    hot = 0;
		}
		flag__ = 2;
		nclock=ordclk[ii + nordclk];
		callf(told, xd, x, x,W,&flag__);
		
		if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    return 0;
		}
	    }
	}
    }
} /* ddoit_ */

/* Subroutine */ int edoit(told,urg, kiwa)
double *told;
integer *urg, *kiwa;
{
    /* System generated locals */
    integer i__1, i__2, i__3;
    double d__1;

    /* Local variables */
    static integer flag__, keve;
    static integer nord;

    static integer ierr1, i__;
    static integer ii;

    /* Function Body */
    --(*urg);
    keve = *pointi;
    *pointi = evtspt[keve];
    evtspt[keve] = -1;

    nord = ordptr[keve + 1] - ordptr[keve];
    if (nord == 0) {
	return 0;
    }

    ++(*kiwa);
    iwa[*kiwa] = keve;
    i__1 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i__1; ++ii) {
	C2F(curblk).kfun = ordclk[ii];

	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = ordclk[ii + nordclk];
	    flag__ = 1;
	    callf(told, xd, x, x,W,&flag__);
	    
	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }

    i__1 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i__1; ++ii) {
	C2F(curblk).kfun = ordclk[ii ];
/*     .     Initialize tvec */
	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] >= 0) {
		d__1 = *told - 1.;
		C2F(dset)(&ntvec, &d__1, tvec, &c__1);

		flag__ = 3;
		nclock=ordclk[ii + nordclk];
		callf(told, xd, x, x ,W,&flag__);
		
		if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    return 0;
		}

		if (ntvec >= 1) {
		    i__2 = ntvec;
		    for (i__ = 1; i__ <= i__2; ++i__) {
			if (tvec[i__ - 1] >= *told) {
			    i__3 = i__ + clkptr[C2F(curblk).kfun] - 1;
			    addevs(&tvec[i__ - 1], &i__3, &ierr1);
			    if (ierr1 != 0) {
/*     !                 event conflict */
				*ierr = 3;
				return 0;
			    }
			}
		    }
		}
	    } else {
		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++(*urg);
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }
} /* edoit_ */

/* Subroutine */ int oodoit(xtd, xt,told,urg, kiwa)
double *xtd, *xt;
double *told;
integer *urg, *kiwa;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

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
	return 0;
    }
    ++(*kiwa);
    iwa[*kiwa] = keve;

    i__1 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i__1; ++ii) {
	C2F(curblk).kfun = ordclk[ii];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = ordclk[ii + nordclk];
	    flag__ = 1;
	    callf(told, xtd, xt, xt,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
/*     .     Initialize tvec */
	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] < 0) {

		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++(*urg);
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }
} /* oodoit_ */

/* Subroutine */ int odoit(residual, xt, xtd, told)
double *residual, *xt, *xtd;
double *told;

{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer flag__, keve, kiwa;

    static integer ierr1, i__;
    static integer ii, jj;
    static integer urg;

    /* Function Body */
    urg = 0;
    kiwa = 0;

    i__1 = noord;
    for (jj = 1; jj <= i__1; ++jj) {
	C2F(curblk).kfun = oord[jj];
	nclock = oord[jj + noord];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    flag__ = 1;
	    callf(told, xtd, xt, residual,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}

	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] < 0) {

		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++urg;
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }

    if (urg > 0) {
L21:
	oodoit(xtd, xt,told, &urg, &kiwa);
	if (urg > 0) {
	    goto L21;
	}
    }
/*     .  update states derivatives */
    i__1 = noord;
    for (ii = 1; ii <= i__1; ++ii) {
	C2F(curblk).kfun = oord[ii];
	if (xptr[C2F(curblk).kfun+1 ] - xptr[C2F(curblk).kfun] > 0) {
	    flag__ = 0;
	    nclock = oord[ii + noord];
	    callf(told, xtd, xt, residual,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }

    i__1 = kiwa;
    for (i__ = 1; i__ <= i__1; ++i__) {
	keve = iwa[i__];
	i__2 = ordptr[keve + 1] - 1;
	for (ii = ordptr[keve]; ii <= i__2; ++ii) {
	    C2F(curblk).kfun = ordclk[ii ];
	    if (xptr[C2F(curblk).kfun+1 ] - xptr[C2F(curblk).kfun] > 0) {
		flag__ = 0;
		nclock = ordclk[ii + nordclk];
		callf(told, xtd, xt, residual,W,&flag__);

		if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    return 0;
		}
	    }
	}
    }
} /* odoit_ */

/* Subroutine */ int zzdoit(xtd, xt,told,urg,  kiwa)
double *xtd, *xt;

double *told;
integer *urg, *kiwa;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

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
	return 0;
    }

    ++(*kiwa);
    iwa[*kiwa] = keve;

    i__1 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i__1; ++ii) {
	C2F(curblk).kfun = ordclk[ii];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    nclock = ordclk[ii + nordclk];
	    flag__ = 1;
	    callf(told, xtd, xt, xt,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
/*     .     Initialize tvec */
	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] < 0) {

		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++(*urg);
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }
} /* zzdoit_ */

/* Subroutine */ int zdoit(g, xtd, xt,told)
double *g;
double *xtd, *xt;
double *told;
{
    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Local variables */
    static integer flag__, keve, kiwa;

    static integer ierr1, i__;
    static integer ii, jj;
    static integer urg;

    /* Function Body */
    urg = 0;
    kiwa = 0;
    i__1 = nzord;
    for (jj = 1; jj <= i__1; ++jj) {
	C2F(curblk).kfun = zord[jj];
	nclock = zord[jj + nzord];
	if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
	    flag__ = 1;
	    callf(told, xtd, xt, xt,W,&flag__);

	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}

	ntvec = clkptr[C2F(curblk).kfun + 1] - clkptr[C2F(curblk).kfun];
	if (ntvec > 0) {
	    if (funtyp[C2F(curblk).kfun] < 0) {

		if (funtyp[C2F(curblk).kfun] == -1) {
		    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
			ntvec = 2;
		    } else {
			ntvec = 1;
		    }
		} else if (funtyp[C2F(curblk).kfun] == -2) {
/* Computing MAX */
/* Computing MIN */
		    i__3 = (integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]
			    ]]];
		    i__2 = min(i__3,ntvec);
		    ntvec = max(i__2,1);
		}
		++urg;
		i__2 = ntvec + clkptr[C2F(curblk).kfun] - 1;
		putevs(told, &i__2, &ierr1);
		if (ierr1 != 0) {
/*     !                 event conflict */
		    *ierr = 3;
		    return 0;
		}
	    }
	}
    }


    if (urg <= 0) {
	goto L22;
    }
L21:
    zzdoit(xtd, xt,told, &urg, &kiwa);
    if (urg > 0) {
	goto L21;
    }
L22:
/*     .  update zero crossing surfaces */
    i__1 = nzord;
    for (ii = 1; ii <= i__1; ++ii) {
	C2F(curblk).kfun = zord[ii];
	if (zcptr[C2F(curblk).kfun + 1] - zcptr[C2F(curblk).kfun] > 0) {
	    flag__ = 9;
	    nclock = oord[ii +nzord];
	    callf(told, xtd, xt, xtd,g,&flag__);
	    if (flag__ < 0) {
		*ierr = 5 - flag__;
		return 0;
	    }
	}
    }

    i__1 = kiwa;
    for (i__ = 1; i__ <= i__1; ++i__) {
	keve = iwa[i__];
	i__2 = ordptr[keve + 1] - 1;
	for (ii = ordptr[keve]; ii <= i__2; ++ii) {
	    C2F(curblk).kfun = ordclk[ii ];
	    if (zcptr[C2F(curblk).kfun + 1] - zcptr[C2F(curblk).kfun] > 0) {
		flag__ = 9;
		nclock = ordclk[ii + nordclk];
		callf(told, xtd, xt, xtd,g,&flag__);

		if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    return 0;
		}
	    }
	}
    }
    return 0;
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
  int ii,i,kf,nx,nz,nzc,nrpar,nipar,in,out,ki,ko,ni,no,k;
  int nin,noutc,lprt,szi,funtype,flagi;
  int solver=C2F(cmsolver).solver;
  int cosd=C2F(cosdebug).cosd;
  ScicosF0 loc0;
  ScicosF loc1;
  /*  ScicosFm1 loc3;*/
  ScicosF2 loc2;
  ScicosFi loci1;
  ScicosFi2 loci2;
  integer kfun;

  
  kfun=C2F(curblk).kfun;
  if ( cosd > 1){
    sciprint("block %d is called ",kfun);
    sciprint("with flag %d ",*flag);
    sciprint("at time %f \r\n",*t);
  }
  kf=kfun;
  i=funptr[kf-1];
  funtype=funtyp[kf];
  flagi=*flag; /* flag 7 implicit initialization */
  if(flagi==7 && funtype<10000) *flag=0;

  if (i<0) {
    switch (funtype) {
    case -1:
      sciprint("type -1 function not allowed for scilab blocks\r\n");
      *flag=-1000-(kfun);
    case 0:
      loc=F2C(sciblk);
      break;
    case 1:
      sciprint("type 1 function not allowed for scilab blocks\r\n");
      *flag=-1000-(kfun);
      return;
    case 2:
      sciprint("type 2 function not allowed for scilab blocks\r\n");
      *flag=-1000-(kfun);
      break;
    case 3:
      loc=sciblk2;
      funtype=2;
      break;
    case 10003:
      loc=sciblk2i;
      funtype=2;
      break;
    default :
      sciprint("Undefined Function type\r\n");
      *flag=-1000;
      return;
    }
    C2F(scsptr).ptr=-i; /* set scilab function adress for sciblk */
  }
  else if (i<=ntabsim)
    loc=*(tabsim[i-1].fonc);
  else {
    i -= (ntabsim+1);
    GetDynFunc(i,&loc);
    if ( loc == (voidf) 0)
      {
	sciprint("Function not found\r\n");
	*flag=-1000-(kfun);
	return;
      }
  }
  
  nx=xptr[kf+1]-xptr[kf];
  nzc=zcptr[kf+1]-zcptr[kf];
  nz=zptr[kf+1]-zptr[kf];
  nrpar=rpptr[kf+1]-rpptr[kf];
  nipar=ipptr[kf+1]-ipptr[kf];
  nin=inpptr[kf+1]-inpptr[kf]; /* number of input ports */
  noutc=outptr[kf+1]-outptr[kf];/* number of output ports */
  switch (funtype) {

  case 1 :			
    /* one entry for each input or output */
    for (in = 0 ; in < nin ; in++) 
      {
	lprt=inplnk[inpptr[kf]+in];
	args[in]=&(outtb[lnkptr[lprt]-1]);
	sz[in]=lnkptr[lprt+1]-lnkptr[lprt];
      }
    for (out=0;out<noutc;out++) {
      lprt=outlnk[outptr[kf]+out];
      args[in+out]=&(outtb[lnkptr[lprt]-1]);
      sz[in+out]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    if(nzc>0){
      args[nin+noutc]=&(g[zcptr[kf]-1]);
      sz[nin+noutc]=nzc;
	}
    loc1 = (ScicosF) loc;
    if (solver==100) {
 (*loc1)(flag,&nclock,t,&(residual[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,&(z__[zptr[kf]-1]),&nz,
	    tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,
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
    (*loc1)(flag,&nclock,t,&(xtd[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,&(z__[zptr[kf]-1]),&nz,
	    tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,
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

    if (nin>1) {
      ki=0;
      for (in=0;in<nin;in++) {
	lprt=inplnk[inpptr[kf]+in];
	szi=lnkptr[lprt+1]-lnkptr[lprt];
	for (ii=0;ii<szi;ii++) 
	  intabl[ki++]=outtb[lnkptr[lprt]-1+ii];
	ni=ni+szi;
      }
      args[0]=&(intabl[0]);
    }
    else {
      if (nin==0) {
	ni=0;
	args[0]=&(outtb[0]);
      }
      else {
	lprt=inplnk[inpptr[kf]];
	args[0]=&(outtb[lnkptr[lprt]-1]);
	ni=lnkptr[lprt+1]-lnkptr[lprt];
      }
    }
    in=nin;
    
    /* catenate outputs if necessary */
    if (noutc>1) {
      ko=0;
      for (out=0;out<noutc;out++) {
	lprt=outlnk[outptr[kf]+out];
	szi=lnkptr[lprt+1]-lnkptr[lprt];
	
	for (ii=0;ii<szi;ii++)  
	  outabl[ko++]=outtb[lnkptr[lprt]-1+ii];
	no=no+szi;
      }
      args[1]=&(outabl[0]);
    }
    else {
      if (noutc==0) {
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
      (*loc0)(flag,&nclock,t,&(residual[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,&(z__[zptr[kf]-1]),&nz,
	      tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	      &(ipar[ipptr[kf]-1]),&nipar,(double *)args[0],&ni,
	      (double *)args[1],&no);
    }
    else {
      (*loc0)(flag,&nclock,t,&(xtd[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,&(z__[zptr[kf]-1]),&nz,
	      tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	      &(ipar[ipptr[kf]-1]),&nipar,(double *)args[0],&ni,
	      (double *)args[1],&no);
    }
    
    /* split output vector on each port if necessary */
    if (noutc>1) {
      ko=0;
      for (out=0;out<noutc;out++) {
	lprt=outlnk[outptr[kf]+out];
	szi=lnkptr[lprt+1]-lnkptr[lprt];
	for (ii=0;ii<szi;ii++)  
	  outtb[lnkptr[lprt]-1+ii]=outabl[ko++];
      }
    }
    break;
  case 2 :			
    /* inputs and outputs given by a table of pointers */
    for (in=0;in<nin;in++) {
      lprt=inplnk[inpptr[kf]+in];
      args[in]=&(outtb[lnkptr[lprt]-1]);
      sz[in]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    for (out=0;out<noutc;out++) {
      lprt=outlnk[outptr[kf]+out];
      args[in+out]=&(outtb[lnkptr[lprt]-1]);
      sz[in+out]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    loc2 = (ScicosF2) loc;
    if (solver==100) {
      (*loc2)(flag,&nclock,t,&(residual[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,
	      &(z__[zptr[kf]-1]),&nz,
	      tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	      &(ipar[ipptr[kf]-1]),&nipar,&(args[0]),&(sz[0]),&nin,
	      &(args[in]),&(sz[in]),&noutc);
    }
    else {
      (*loc2)(flag,&nclock,t,&(xtd[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,
	    &(z__[zptr[kf]-1]),&nz,
	    tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,&(args[0]),&(sz[0]),&nin,
	    &(args[in]),&(sz[in]),&noutc);
    }
    break;
  case 10001 :			
    /* implicit block one entry for each input or output */
    for (in = 0 ; in < nin ; in++) 
      {
	lprt=inplnk[inpptr[kf]+in];
	args[in]=&(outtb[lnkptr[lprt]-1]);
	sz[in]=lnkptr[lprt+1]-lnkptr[lprt];
      }
    for (out=0;out<noutc;out++) {
      lprt=outlnk[outptr[kf]+out];
      args[in+out]=&(outtb[lnkptr[lprt]-1]);
      sz[in+out]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    if(nzc>0){
      args[nin+noutc]=&(g[zcptr[kf]-1]);
      sz[nin+noutc]=nzc;
	}
    loci1 = (ScicosFi) loc;

    (*loci1)(flag,&nclock,t,&(residual[xptr[kf]-1]),&(xtd[xptr[kf]-1]),&(xt[xptr[kf]-1]),
	     &nx,&(z__[zptr[kf]-1]),&nz,
	     tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	     &(ipar[ipptr[kf]-1]),&nipar,
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
    for (in=0;in<nin;in++) {
      lprt=inplnk[inpptr[kf]+in];
      args[in]=&(outtb[lnkptr[lprt]-1]);
      sz[in]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    for (out=0;out<noutc;out++) {
      lprt=outlnk[outptr[kf]+out];
      args[in+out]=&(outtb[lnkptr[lprt]-1]);
      sz[in+out]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    loci2 = (ScicosFi2) loc;

    (*loci2)(flag,&nclock,t,&(residual[xptr[kf]-1]),&(xtd[xptr[kf]-1]),&(xt[xptr[kf]-1]),&nx,
	    &(z__[zptr[kf]-1]),&nz,
	    tvec,&ntvec,&(rpar[rpptr[kf]-1]),&nrpar,
	    &(ipar[ipptr[kf]-1]),&nipar,&(args[0]),&(sz[0]),&nin,
	    &(args[in]),&(sz[in]),&noutc);
    break;  
  default:
    sciprint("Undefined Function type\r\n");
    *flag=-1000;
    return;
  }
  if(solver==100 && funtype<10000 && *flag==0) { /* Implicit Solver */

    if(flagi!=7) {
      for (k=0;k<nx;k++) {
	residual[xptr[kf]-1+k]=residual[xptr[kf]-1+k]-xtd[xptr[kf]-1+k];
      }
    }
    else {
      for (k=0;k<nx;k++) {
	xtd[xptr[kf]-1+k]=residual[xptr[kf]-1+k];
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


/* Subroutine */ int addevs(t, evtnb, ierr1)
double *t;integer *evtnb, *ierr1;
{
    static integer i__, j;

    /* Function Body */
    *ierr1 = 0;
    if (evtspt[*evtnb] != -1) {
	*ierr1 = 1;
	return 0;
    } else {
	evtspt[*evtnb] = 0;
	tevts[*evtnb] = *t;
    }
    if (*pointi == 0) {
	*pointi = *evtnb;
	return 0;
    }
    if (*t < tevts[*pointi]) {
	evtspt[*evtnb] = *pointi;
	*pointi = *evtnb;
	return 0;
    }
    i__ = *pointi;

L100:
    if (evtspt[i__] == 0) {
	evtspt[i__] = *evtnb;
	return 0;
    }
    if (*t >= tevts[evtspt[i__]]) {
	j = evtspt[i__];
	if (evtspt[j] == 0) {
	    evtspt[j] = *evtnb;
	    return 0;
	}
	i__ = j;
	goto L100;
    } else {
	evtspt[*evtnb] = evtspt[i__];
	evtspt[i__] = *evtnb;
    }
} /* addevs */

/* Subroutine */ int putevs(t, evtnb, ierr1)
double *t;
integer *evtnb, *ierr1;
{

    /* Function Body */
    *ierr1 = 0;
    if (evtspt[*evtnb] != -1) {
	*ierr1 = 1;
	return 0;
    } else {
	evtspt[*evtnb] = 0;
	tevts[*evtnb] = *t;
    }
    if (*pointi == 0) {
	*pointi = *evtnb;
	return 0;
    }
    evtspt[*evtnb] = *pointi;
    *pointi = *evtnb;
    return 0;
} /* putevs */

