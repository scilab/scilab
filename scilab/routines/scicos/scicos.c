/* 11-03-2005,  Masoud*/
/* adding A-Jacobian*/
/* istate =-1 case;*/

#include <stdlib.h> 
#include <string.h>
#include "../machine.h"
#include "../os_specific/link.h"
#include "scicos.h"
#include "import.h"
#include "blocks.h"
#include <math.h>
#include "../graphics/Math.h"
#include "../os_specific/sci_mem_alloc.h"  /* malloc */


#ifdef FORDLL 
#define IMPORT  __declspec (dllimport)
#else 
#define IMPORT extern
#endif


#define abs(x) ((x) >= 0 ? (x) : -(x))
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#define freeall \
	      FREE(rhot);\
	      FREE(ihot);\
	      FREE(jroot);\
	      FREE(W);\
	      FREE(zcros);

#define freeallx \
	      FREE(rhot);\
	      FREE(ihot);\
	      FREE(jroot);\
	      FREE(W);\
	      FREE(zcros);\
	      FREE(scicos_xproperty);\
              FREE(Mode_save);

void cosini(double *);
void idoit(double *);
void cosend(double *);
void cdoit(double *);
void doit(double *);
void ddoit(double *);
void edoit(double *,integer *);
void odoit(double *,double *,double *,double *);
void ozdoit(double *,double *,double *,integer *);
void zdoit(double *,double *,double *,double *);
void reinitdoit(double *,integer *);
void cossimdaskr(double *);
void cossim(double *);
void callf(double *, double *, double *, double *,double *,integer *);
int C2F(simblk)(integer *, double *, double *, double *);
int C2F(simblkdaskr)(double *, double *, double *, double *, double *, integer *, double *, integer *);
int C2F(grblk)(integer *, double *, double *, integer *, double *);
int C2F(grblkdaskr)(integer *, double *, double *, double *, integer *, double *, double *, integer *);
void addevs(double ,integer *,integer *);
void putevs(double *,integer *,integer *);
void FREE_blocks();
int setmode(double *,double *,double *,integer *,double);

/* Jacobian*/
void Jdoit(double *,double *,double *,double *,int *);
int C2F(Jacobian)(double *, double *, double *, double *, double *, double *, int *);
void Multp(double *, double *,double *, int, int, int ,int);
void Set_Jacobian_flag(int flag);
double Get_Jacobian_parameter(void);
double Get_Scicos_SQUR(void);
/*void DISP(A,ra ,ca,name);*/
/* Jacobian*/

IMPORT struct {
  int cosd;
} C2F(cosdebug);


IMPORT struct {
  int counter;
} C2F(cosdebugcounter);

struct {
  int solver;
} C2F(cmsolver);


extern void  F2C(sciblk)();
extern void  sciblk2();
extern void  sciblk4();
extern void  GetDynFunc();
extern void  C2F(iislink)();


extern  integer C2F(cvstr)();
extern  integer C2F(dset)();
extern  integer C2F(dcopy)();
extern  integer C2F(iset)();
extern  integer C2F(realtime)();
extern  integer C2F(realtimeinit)();
extern  integer C2F(sxevents)();
extern  integer C2F(stimer)();
extern  integer C2F(xscion)();
extern  integer C2F(ddaskr)();
extern  integer C2F(lsodar2)();

extern int scilab_timer_check(void);

ScicosImport  scicos_imp;

static integer nblk, nordptr, nout, ng, ncord, noord, nzord,niord,
  nclock,nordclk,niord,nmod;

static integer *neq;

static  double Atol, rtol, ttol, deltat,hmax;
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
static integer panj=5;

static integer *iwa;

static integer *xptr,*modptr, *evtspt;
static integer  *funtyp, *inpptr, *outptr, *inplnk, *outlnk, *lnkptr;
static integer *clkptr, *ordptr, *ordclk, *cord, 
  *iord, *oord,  *zord,  *critev,  *zcptr;
static integer *pointi;
static integer *ierr;

static double *x,*xd,*tevts,*outtb;
static integer *mod;
static double *t0,*tf,scicos_time;

static scicos_block *Blocks; 

static integer phase;

integer *pointer_xproperty;

integer n_pointer_xproperty;

static integer *block_error;
  /* Jacobian*/
static integer Jacobian_Flag;
static double  CJJ;
static double SQuround;
  /* Jacobian*/

void call_debug_scicos(double *, double *, double *, double *,double *,integer
 *,integer,integer,integer);

static integer debug_block;
 
/* Subroutine */ 
int C2F(scicos)
     (x_in, xptr_in, z__, work,zptr,modptr_in, iz, izptr, t0_in, tf_in, tevts_in, 
      evtspt_in, nevts, pointi_in, outtb_in, nout1, funptr, funtyp_in, inpptr_in, outptr_in, 
      inplnk_in, outlnk_in, lnkptr_in, nlnkptr, rpar, rpptr, ipar, ipptr, clkptr_in, 
      ordptr_in, nordptr1, ordclk_in, cord_in, ncord1, iord_in, niord1, oord_in, noord1, 
      zord_in, nzord1, critev_in, nblk1, ztyp, zcptr_in, subscr, nsubs, simpar, 
      flag__, ierr_out)
     double *x_in,*z__;
     void **work;
     integer *modptr_in;
     integer *xptr_in;
     integer *zptr, *iz, *izptr;
     double *t0_in, *tf_in, *tevts_in;
     integer *evtspt_in, *nevts, *pointi_in;
     double *outtb_in;
     integer *nout1, *funptr, *funtyp_in, *inpptr_in, *outptr_in;
     integer *inplnk_in, *outlnk_in, *lnkptr_in,*nlnkptr;
     double *rpar;
     integer *rpptr, *ipar, *ipptr, *clkptr_in, *ordptr_in, *nordptr1;
     integer *ordclk_in, *cord_in, *ncord1, *iord_in, *niord1, *oord_in;
     integer *noord1, *zord_in, *nzord1, *critev_in, *nblk1, *ztyp, *zcptr_in;
     integer *subscr, *nsubs;
     double *simpar;
     integer *flag__, *ierr_out;

{
  integer i1,kf,lprt,in,out,job=1;

  extern /* Subroutine */ int C2F(msgs)();
  static integer mxtb, ierr0, kfun0, i, j, k;
  extern /* Subroutine */ int C2F(makescicosimport)();
  extern /* Subroutine */ int C2F(getscsmax)();
  static integer ni, no;
  extern /* Subroutine */ int C2F(clearscicosimport)();
  static integer nx, nz;
  double *W;



  /*     Copyright INRIA */
  /* iz,izptr are used to pass block labels */

  t0=t0_in;
  tf=tf_in;
  ierr=ierr_out;

  /* Parameter adjustments */
  pointi=pointi_in;
  x=x_in;
  xptr=xptr_in-1;
  modptr=modptr_in-1;
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

  /* Function Body */
  Atol = simpar[1];
  rtol = simpar[2];
  ttol = simpar[3];
  deltat = simpar[4];
  C2F(rtfactor).scale = simpar[5];
  C2F(cmsolver).solver = (integer) simpar[6];
  hmax=simpar[7];

  nordptr = *nordptr1;
  nblk = *nblk1;
  ncord = *ncord1;
  noord = *noord1;
  nzord = *nzord1;
  niord = *niord1;
  nout = *nout1;

  *ierr = 0;

  xd=&x[xptr[nblk+1]-1];

  nordclk=ordptr[nordptr]-1;
  /*     computes number of zero crossing surfaces */
  ng = zcptr[nblk + 1] - 1;
  nmod = modptr[nblk + 1] - 1;

  /*     number of  discrete real states */
  nz = zptr[nblk + 1] - 1;
  /*     number of continuous states */
  nx = xptr[nblk +1] - 1;
  neq=&nx;
  /*        add an error message please */
  /*     number of rows in ordclk is ordptr(nclkp1)-1 */

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

  if((Blocks=MALLOC(sizeof(scicos_block)*nblk))== NULL ){
    *ierr =5;
    return 0;
  }
  if(nmod>0){
    if((mod=MALLOC(sizeof(int)*nmod))== NULL ){
      *ierr =5;
      return 0;
    }
  }
  debug_block=-1; /* no debug block for start */
  C2F(cosdebugcounter).counter=0;

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
	*ierr =1000+kf+1;
	FREE_blocks();
	return 0;
      case 2:
	sciprint("type 2 function not allowed for scilab blocks\r\n");
	*ierr =1000+kf+1;
	FREE_blocks();
	return 0;
      case 3:
	Blocks[kf].funpt=sciblk2;
	Blocks[kf].type=2;
	break;
      case 5:
	Blocks[kf].funpt=sciblk4;
	Blocks[kf].type=4;
	break;
      case 99: /* debugging block */
       Blocks[kf].funpt=sciblk4;
       Blocks[kf].type=4;
       debug_block=kf;
       break;

      case 10005:
	Blocks[kf].funpt=sciblk4;
	Blocks[kf].type=10004;
	break;
      default :
	sciprint("Undefined Function type\r\n");
	*ierr =1000+kf+1;
	FREE_blocks();
	return 0;
      }
      Blocks[kf].scsptr=-i; /* set scilab function adress for sciblk */
    }
    else if (i<=ntabsim){
      Blocks[kf].funpt=*(tabsim[i-1].fonc);
      Blocks[kf].scsptr=0;     /* this is done for being able to test if a block
				is a scilab block in the debugging phase when 
				sciblk4 is called */
    }
    else {
      i -= (ntabsim+1);
      GetDynFunc(i,&Blocks[kf].funpt);
      if ( Blocks[kf].funpt == (voidf) 0) {
	sciprint("Function not found\r\n");
	*ierr =1000+kf+1;
	FREE_blocks();
	return 0;
      }
      Blocks[kf].scsptr=0;   /* this is done for being able to test if a block
				is a scilab block in the debugging phase when 
				sciblk4 is called */
    }
    Blocks[kf].ztyp=ztyp[kf+1];
    Blocks[kf].nx=xptr[kf+2]-xptr[kf+1];
    Blocks[kf].ng=zcptr[kf+2]-zcptr[kf+1];
    Blocks[kf].nz=zptr[kf+2]-zptr[kf+1];
    Blocks[kf].nrpar=rpptr[kf+2]-rpptr[kf+1];
    Blocks[kf].nipar=ipptr[kf+2]-ipptr[kf+1];
    Blocks[kf].nin=inpptr[kf+2]-inpptr[kf+1]; /* number of input ports */
    Blocks[kf].nout=outptr[kf+2]-outptr[kf+1];/* number of output ports */
    if ((Blocks[kf].insz=MALLOC(sizeof(int)*Blocks[kf].nin))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    if ((Blocks[kf].inptr=MALLOC(sizeof(double*)*Blocks[kf].nin))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    for(in=0;in<Blocks[kf].nin;in++) {
      lprt=inplnk[inpptr[kf+1]+in];
      Blocks[kf].inptr[in]=&(outtb[lnkptr[lprt]-1]);
      Blocks[kf].insz[in]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    if ((Blocks[kf].outsz=MALLOC(sizeof(int)*Blocks[kf].nout))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    if ((Blocks[kf].outptr=MALLOC(sizeof(double*)*Blocks[kf].nout))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    for(out=0;out<Blocks[kf].nout;out++) {
      lprt=outlnk[outptr[kf+1]+out];
      Blocks[kf].outptr[out]=&(outtb[lnkptr[lprt]-1]);
      Blocks[kf].outsz[out]=lnkptr[lprt+1]-lnkptr[lprt];
    }
    Blocks[kf].nevout=clkptr[kf+2] - clkptr[kf+1];
    if ((Blocks[kf].evout=CALLOC(Blocks[kf].nevout,sizeof(double)))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }

    Blocks[kf].z=&(z__[zptr[kf+1]-1]);
    Blocks[kf].rpar=&(rpar[rpptr[kf+1]-1]);
    Blocks[kf].ipar=&(ipar[ipptr[kf+1]-1]);

    if ((Blocks[kf].res=MALLOC(sizeof(double)*Blocks[kf].nx))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    
    i1=izptr[kf+2]-izptr[kf+1];
    if ((Blocks[kf].label=MALLOC(sizeof(char)*(i1+1)))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    Blocks[kf].label[i1]='\0';
    C2F(cvstr)(&i1,&(iz[izptr[kf+1]-1]),Blocks[kf].label,&job,i1);    
    if ((Blocks[kf].jroot=CALLOC(Blocks[kf].ng,sizeof(int)))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    
    Blocks[kf].work=(void **)(((double *)work)+kf);
    Blocks[kf].nmode=modptr[kf+2]-modptr[kf+1]; 
    if ( Blocks[kf].nmode!=0){
      Blocks[kf].mode=&(mod[modptr[kf+1]-1]);
    }
  }


  if((iwa=MALLOC(sizeof(int)*(*nevts)))== NULL ){
    FREE_blocks();
    *ierr =5;
    return 0;
  }


  C2F(makescicosimport)(x, &xptr[1], &zcptr[1], z__, &zptr[1],mod,&modptr[1], iz, &izptr[1], 
			&inpptr[1], &inplnk[1], &outptr[1], &outlnk[1], &lnkptr[1], 
			nlnkptr, rpar, &rpptr[1], ipar, &ipptr[1], &nblk, outtb, 
			&nout, subscr, nsubs, &tevts[1], &evtspt[1], nevts, pointi, 
			&oord[1], &zord[1], funptr, &funtyp[1], &ztyp[1], &cord[1],
			&ordclk[1], &clkptr[1], &ordptr[1], &critev[1], iwa);
  if (*flag__ == 1) {
    /*     initialisation des blocks */
    for (kf = 0; kf < nblk; ++kf) {
      *(Blocks[kf].work)=NULL;
    }
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
      cossimdaskr(t0);
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
    phase=1;
    idoit(t0);
    if (*ierr == 0) {
      if((W=MALLOC(sizeof(double)*nx))== NULL ){
	FREE(iwa);
	FREE_blocks();
	*ierr =5;
	return 0;
      }
    
      C2F(simblk)(&nx, t0, x, W);
      for (i = 0; i < nx; ++i) {
	x[i] = W[i];
      }
      FREE(W);
    }
  }
  FREE(iwa);
  FREE_blocks(); 

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

  double *W;
  jj=max(ng,nout);
  if((W=MALLOC(sizeof(double)*(jj)))== NULL ){
    *ierr =10000;
    return;
  }
  /* Jacobian*/
  Jacobian_Flag=0;
  /* Jacobian*/

  /* Function Body */
  *ierr = 0;
  /*     initialization (flag 4) */
  /*     loop on blocks */

  C2F(dset)(&jj, &c_b14, W, &c__1);
  nclock = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (funtyp[C2F(curblk).kfun] >= 0) { /* debug_block is not called here */
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
    FREE(W);
    return;
  }
  /*     initialization (flag 6) */
  nclock = 0;

  for (jj = 1; jj <= ncord; ++jj) {
    C2F(curblk).kfun = cord[jj];
    flag__ = 6;
    if (funtyp[C2F(curblk).kfun] >= 0) {
      callf(told, xd, x, x,W,&flag__);
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	FREE(W);
	return;
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
	  FREE(W);
	  return;
	}
      }
    }

    nclock = 0;

    for (jj = 1; jj <= ncord; ++jj) {
      C2F(curblk).kfun = cord[jj];
      flag__ = 6;
      if (funtyp[C2F(curblk).kfun] >= 0) {
	callf(told, xd, x, x,W,&flag__);
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  FREE(W);
	  return;
	}
      }
    }
    for (jj = 0; jj <= nout-1; ++jj) {
      if (outtb[jj] != W[jj]) {
	goto L30;
      }
    }
    FREE(W);
    return;
  L30:
    C2F(dcopy)(&nout, outtb, &c__1, W, &c__1);

  }
  *ierr = 20;
  FREE(W);
} /* cosini_ */

/* Subroutine */ void idoit(told)
     double *told;
{
  static integer flag__;
  static integer i,jj;
  static integer ierr1;
  static integer i2;
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
      callf(told, xd, x, x,x,&flag__);
	
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {
	
	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    i=2;
	  } else {
	    i=1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  i=max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	i2 =i+ clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	doit(told);
	if (*ierr != 0) {
	  return;
	}
      }
    }
  }  
} /* idoit_ */

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
  static integer jj, jt;
  static integer istate, ntimer;

  static double rhotmp;
  static integer inxsci;

  static integer kpo, kev;
  
  double *rhot;
  integer *ihot,niwp,nrwp;
  integer *jroot,*zcros;

  double *W;

  nrwp = (*neq) * max(16,*neq + 9) + 22 + ng * 3;
  /* +1 below is so that rhot starts from 1; one wasted location */
  if((rhot=MALLOC(sizeof(double)*(nrwp+1)))== NULL ){
    *ierr =10000;
    return;
  }
  niwp = *neq + 20 + ng;/* + ng is for change in lsodar2 to
				       handle masking */

  /* +1 below is so that ihot starts from 1; one wasted location */
  if((ihot=MALLOC(sizeof(int)*(niwp+1)))== NULL ){
    *ierr =10000;
    FREE(rhot);
    return;
  }
  if((jroot=MALLOC(sizeof(int)*ng))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    return;
  }

  for ( jj = 0 ; jj < ng ; jj++ )
  {
	jroot[jj] = 0 ;
  }

  if((zcros=MALLOC(sizeof(int)*ng))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    FREE(jroot);
    return;
  }
  if((W=MALLOC(sizeof(double)*ng))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    FREE(jroot);
    FREE(zcros);
    return;
  }
  
  
  /* Function Body */
  
  C2F(coshlt).halt = 0;
  *ierr = 0;
  
  C2F(xscion)(&inxsci);
  /*     initialization */
  C2F(iset)(&niwp, &c__0, &ihot[1], &c__1);
  C2F(dset)(&nrwp, &c_b14, &rhot[1], &c__1);
  C2F(realtimeinit)(told, &C2F(rtfactor).scale);
  
  phase=1;
  hot = 0;
  itask = 4;

  jt = 2;

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
  /*     initialisation (propagation of constant blocks outputs) */
  idoit(told);
  if (*ierr != 0) {
    freeall;
    return;
  }
  /*
    }
    }
  */
  
  /*     main loop on time */
  
  while(*told < *tf) {
    
    if (inxsci == 1 && scilab_timer_check() == 1) {
      C2F(sxevents)();
      /*     .     sxevents can modify halt */
    }
    if (C2F(coshlt).halt != 0) {
      C2F(coshlt).halt = 0;
      freeall;
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
      freeall;
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
	    freeall;
	    return;
	  }
	}
      } else {
	/*     integrate */
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
	  hot = 0;
	}
	rhot[1] = rhotmp;
	t = min(*told + deltat,min(t,*tf + ttol));
	
	if (ng>0 &&  hot == 0 && nmod>0) {
	  zdoit(W,x,x,told);
	  if (*ierr != 0){
	    freeall;
	    return;
	  }
	}
	
	
	if (hot){
	  istate=2;
	}else{
	  istate = 1;
	}
	if (C2F(cosdebug).cosd >= 1) {
	  sciprint("****lsodar from: %f to %f hot= %d  \r\n", *told,t,hot);
	}

	if(hmax==0){
	  iopt = 0;
	}else{
	  iopt=1;
	  C2F(iset)(&panj, &c__0, &ihot[5], &c__1);
	  C2F(dset)(&panj, &c_b14, &rhot[5], &c__1);
	  rhot[6]=hmax;
	}

	/*--discrete zero crossings----dzero--------------------*/
	/*--check for Dzeros after Mode settings or ddoit()----*/
	if (ng>0 && hot==0){
	  zdoit(W, x, x,told);
	  if (*ierr != 0) {freeall;return;}
	  for (jj = 0; jj < ng; ++jj) {
	    if((W[jj]>=0.0)&&(jroot[jj]==-5)) {istate=3;jroot[jj]=1;}
	    else if((W[jj]<0.0)&&(jroot[jj]==5)) {istate=3;jroot[jj]=-1;}
	  }
	} 
	/*--discrete zero crossings----dzero--------------------*/

	if (istate!=3){/* if there was a dzero, its event should be activated*/
	  phase=2;
	  C2F(lsodar2)(C2F(simblk), neq, x, told, &t, &c__1, &rtol, 
		       &Atol, &itask, &istate, &iopt, &rhot[1], &
		       nrwp, &ihot[1], &niwp, &jdum, &jt, 
		       C2F(grblk), &ng, jroot);
	}
	phase=1;
	
	if (*ierr > 5) {
	  /*     !           singularity in block */
	  freeall;
	  return;
	}
	if (istate <= 0) {
	  /* integration problem */
	  *ierr = 100 - istate;
	  freeall;
	  return;
	} else {
	  if (C2F(cosdebug).cosd >= 1) {
	    sciprint("****lsodar reached: %f\r\n",*told);
	  }
	  hot = 1;
	}
	
	/*     .     update outputs of 'c' type  blocks if we are at the end*/
	if (*told >= *tf) {
	  if (ncord > 0) {
	    cdoit(told);
	    freeall;
	    return;
	  }
	}
	if (istate == 4) hot=0; /* new feature of lsodar, detects unmasking */
	if (istate == 3) {
	  /*     .        at a least one root has been found */
	  hot = 0;
	  if (C2F(cosdebug).cosd >= 1) {
	    sciprint("root found at t=: %f\r\n",*told);
	  }
	  /*     .        update outputs affecting ztyp blocks ONLY FOR OLD BLOCKS */
	  zdoit(W, xd, x,told);
	  if (*ierr != 0) {
	    freeall;
	    return;
	  }
	  for (jj = 0; jj < ng; ++jj) {
	    C2F(curblk).kfun = zcros[ jj];
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
	    /*   */
	    if (kev != 0) {
	      Blocks[C2F(curblk).kfun-1].jroot=&jroot[zcptr[C2F(curblk).kfun]-1];
	      if (funtyp[C2F(curblk).kfun] > 0) {
		
		if (Blocks[C2F(curblk).kfun-1].nevout > 0) {
		  flag__ = 3;
		  /* call corresponding block to determine output event (kev) */
		  nclock = -kev;
		  callf(told, xd, x, x,W,&flag__);
		  if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    freeall;
		    return;
		  }
		  /*     .              update event agenda */
		  for (k = 0; k < Blocks[C2F(curblk).kfun-1].nevout; ++k) {
		    if (Blocks[C2F(curblk).kfun-1].evout[k] >= 0.) {
		      i3 = k + clkptr[C2F(curblk).kfun] ;
		      addevs(Blocks[C2F(curblk).kfun-1].evout[k]+(*told), &i3, &ierr1);
		      if (ierr1 != 0) {
			/*     .                       nevts too small */
			*ierr = 3;
			freeall;
			return;
		      }
		    } 
		  }
		}
		/*     .              update state */
		if ((Blocks[C2F(curblk).kfun-1].nx > 0) || (*Blocks[C2F(curblk).kfun-1].work != NULL) ) {
		  /*     .              call corresponding block to update state */
		  flag__ = 2;
		  nclock = -kev;
		  callf(told, xd, x, x,W,&flag__);
		  
		  if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    freeall;
		    return;
		  }
		}
	      }
	    }
	  }
	}
      }
      /*--discrete zero crossings----dzero--------------------*/
      if (ng>0){ /* storing ZC signs just after a ddaskr call*/
	zdoit(W, x, x, told); if (*ierr != 0) {freeall;return;  }
	for (jj = 0; jj < ng; ++jj) 
	  if(W[jj]>=0)jroot[jj]=5;else jroot[jj]=-5;
      }
      /*--discrete zero crossings----dzero--------------------*/

      C2F(realtime)(told);
    } else {
      /*     .  t==told */   
      if (C2F(cosdebug).cosd >= 1) {
	sciprint("Event: %d activated at t=%f\r\n",*pointi,*told);
	for(kev=0;kev<nblk;kev++){
	  if (Blocks[kev].nmode>0){
	    sciprint("mode of block %d=%d, ",kev,Blocks[kev].mode[0]);
	  }
	}
	sciprint("**mod**\r\n");
      }

      ddoit(told);
      if (C2F(cosdebug).cosd >= 1) {
	sciprint("End of activation\r\n");
      }
      if (*ierr != 0) {
	freeall;
	return;
      }

    }
    /*     end of main loop on time */
  }
  freeall;
} /* cossim_ */



/* Subroutine */ void cossimdaskr(told)
     double *told;
{
  /* Initialized data */
  static integer otimer = 0;
  /* System generated locals */
  integer i3,*ipardummy=NULL;
  double /*d__1,*/*rpardummy=NULL;

  /* Local variables */
  static integer flag__;
  static integer info[20];

  static integer ierr1;
  static integer j, k;
  static double t;
  static integer jj, jt;
  static integer istate, ntimer;
  static double rhotmp;
  static integer inxsci;
  static integer kpo, kev;

  double *rhot;
  integer *ihot,niwp,nrwp;
  integer *jroot,*zcros;
  integer maxord;
  integer *scicos_xproperty;

  double *W;
  integer *Mode_save;
  integer Mode_change;
  /*-------------------- Analytical Jacobian memory allocation ----------*/
  int  Jn, Jnx, Jno, Jni, Jactaille;
  double uround;
  Jactaille=0;   
  if(Jacobian_Flag==1){
    Jn=*neq;
    Jnx=Blocks[nblk-1].nx;
    Jno=Blocks[nblk-1].nout;
    Jni=Blocks[nblk-1].nin;
    Jactaille= 2+3*Jn+(Jn+Jni)*(Jn+Jno)+Jnx*(Jni+2*Jn+Jno)+(Jn-Jnx)*(2*(Jn-Jnx)+Jno+Jni)+2*Jni*Jno;}
  /*----------------------------Jacobian----------------------------------*/
  maxord = 5;
  nrwp = max(maxord + 4,7) * (*neq) + 60 + (*neq)*(*neq) + ng * 3 + Jactaille;
  niwp = (*neq) + 40 + (*neq) +ng ; /* + ng is for change in ddaskr to handle masking */ 

   /* +1 below is so that rhot starts from 1; one wasted location */
  if((rhot=MALLOC(sizeof(double)*(nrwp+1)))== NULL ){
    *ierr =10000;
    return;
  }
/* +1 below is so that ihot starts from 1; one wasted location */
  if((ihot=MALLOC(sizeof(int)*(niwp+1)))== NULL ){
    FREE(rhot);
    *ierr =10000;
    return;
  }
  if((jroot=MALLOC(sizeof(int)*ng))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    return;
  }
  if((scicos_xproperty=MALLOC(sizeof(int)*(*neq)))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    FREE(jroot);
    return;
  }
  C2F(iset)(neq, &c__1, scicos_xproperty, &c__1);
  if((zcros=MALLOC(sizeof(int)*ng))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    FREE(jroot);
    FREE(scicos_xproperty);
    return;
  }
  if((W=MALLOC(sizeof(double)*ng))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    FREE(jroot);
    FREE(scicos_xproperty);
    FREE(zcros);
    return;
  }
  if((Mode_save=MALLOC(sizeof(integer)*nmod))== NULL ){
    *ierr =10000;
    FREE(rhot);
    FREE(ihot);
    FREE(jroot);
    FREE(scicos_xproperty);
    FREE(zcros);
    FREE(W);
    return;
  }

  uround = 1.0;
  do{
    uround = uround*0.5;
  }while ( 1.0 + uround != 1.0);
  uround = uround*2.0;
  SQuround=sqrt(uround);
  /* Function Body */

  C2F(coshlt).halt = 0;
  *ierr = 0;
  /*     hot = .false. */
  phase=1;
  hot = 0;

  jt = 2;

  /*      stuck=.false. */
  C2F(xscion)(&inxsci);
  /*     initialization */
  C2F(iset)(&niwp, &c__0, &ihot[1], &c__1);
  C2F(dset)(&nrwp, &c_b14, &rhot[1], &c__1);
  C2F(realtimeinit)(told, &C2F(rtfactor).scale);
  /*     ATOL and RTOL are scalars */
  info[1] = 0;
  info[2] = 0;
  info[3] = 1;
  /*don't go beyond stopping point TSTOP defined by RWORK(1)*/

  /*     derivatives automatically computed by numerical differences */
  info[4] = 0;
  /*     full jac matrx */
  info[5] = 0;

  if(hmax==0){
    info[6] = 0;    /*  code determines maximaum step-size */
  }else{
    info[6] = 1;
    rhot[2]=hmax;  /*  user defined maximaum step-size */
  } 

  /*     code determines initial step size */
  info[7] = 0;
  /*     MAXORD=5 */
  info[8] = 0;
  /*     no info on solution sign available */
  info[9] = 0;
  /*     direc method instead of Dcrylof method */
  info[11] = 0;
  info[12] = 0;
  info[13] = 0;
  info[14] = 0; 
  info[15] = 0;
  info[16] = 0;
  info[17] = 0;

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
  /*     initialisation (propagation of constant blocks outputs) */
  idoit(told);
  if (*ierr != 0) {
    freeallx;
    return;
  }
  /*     main loop on time */
  while (*told < *tf) {
    if (inxsci == 1 && scilab_timer_check() == 1) {
      C2F(sxevents)();
      /*     .     sxevents can modify halt */
    }
    if (C2F(coshlt).halt != 0) {
      C2F(coshlt).halt = 0;
      freeallx;
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
      freeallx;
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
	  cdoit(told);
	  freeallx;
	  return;
	}
      } else {
	if (hot == 0) {
	  reinitdoit(told,scicos_xproperty);
	  if(*ierr >0){
	    freeallx;
	    return;
	  }
	}      
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
	  hot = 0;/* Do cold-restat the solver:if the new TSTOP isn't beyong the previous one*/ 
	}
	rhot[1] = rhotmp;
	t = min(*told + deltat,min(t,*tf + ttol));

	if (hot == 0){ /* CIC calculation when hot==0 */

	  if (ng>0&&nmod>0){
	    phase=1;
	    zdoit(W, xd, x,told);
	    if (*ierr != 0) {
	      freeallx;
	      return;
	    }
	  }


	  for(j=0;j<=nmod;j++){/* counter to reevaluate the 
				  modes in  mode->CIC->mode->CIC-> loop 
				  do it once in the absence of mode (nmod=0)*/
	    /* updating the modes through Flag==9, Phase==1 */
	    
	    info[0]=0;  /* cold start */
	    info[10]=1; /* inconsistent IC */
	    info[13]=1; /* return after CIC calculation */
	    reinitdoit(told,scicos_xproperty);/* filling up the scicos_xproperties */
 	    if(*ierr >0){
	      freeallx;
	      return; 
	    }
	    for (jj = 1; jj <= *neq; ++jj) {
	      ihot[jj + 40] = scicos_xproperty[jj-1];
	    }
	    phase=2;
	    /* Jacobian*/
	    info[4] =Jacobian_Flag; 
	    /*	    info[4] =0; */ /* numerical Jacobian */
	    C2F(ddaskr)(C2F(simblkdaskr), neq, told, x, xd, &t, info,  &rtol, 
			 &Atol, &istate, &rhot[1],&nrwp, &ihot[1], &niwp,
			rpardummy, ipardummy, C2F(Jacobian), rpardummy, 
			C2F(grblkdaskr), &ng, jroot);

	    if (*ierr > 5) {
	      freeallx;
	      return;
	    }
	    if (C2F(cosdebug).cosd >= 1) {
	      if (istate==4) {
		sciprint("**** daskr succesfully initialized *****/r/n" );
	      }
	      else{
		sciprint("**** daskr failed to initialize ->try again *****/r/n" );
	      }
	    }
	    /*-------------------------------------*/
	    /* saving the previous modes*/
	    for (jj = 0; jj < nmod; ++jj) {
	      Mode_save[jj] = mod[jj];
	    }
	    if (ng>0&&nmod>0){	 
	      phase=1;
	      zdoit(W, xd, x,told);
	      if (*ierr != 0) {
		freeallx;
		return; 
	      }
	    }
	    /*------------------------------------*/
	    Mode_change=0;
	    for (jj = 0; jj < nmod; ++jj) {
	      if(Mode_save[jj] != mod[jj])
		{Mode_change=1;
 		break;
		}
	    }
	    if ( Mode_change==0)      break;
	  }/* mode-CIC  counter*/
	  if(Mode_change==1){
	    *ierr = 100 - (-17);
	    freeallx;
	    return;
	  }
	  info[0]=0;  /* cold restart */
	  info[10]=1; /* to reevaluate CIC when info[0]==0*/
	  info[13]=0; /* continue after CIC calculation */
	} /* CIC calculation when hot==0 */

	info[0]=hot;  
	/*     Warning rpar and ipar are used here as dummy pointers */
	phase=2;
	if (C2F(cosdebug).cosd >= 1) {
	  sciprint("****daskr from: %f to %f hot= %d  \r\n", *told,t,hot);
	}

	/*--discrete zero crossings----dzero--------------------*/
	/*--check for Dzeros after Mode settings or ddoit()----*/
	if (ng>0 && hot==0){
	  zdoit(W, xd, x,told);
	  if (*ierr != 0) {freeallx;return;  }
	  istate=0;
	  for (jj = 0; jj < ng; ++jj) {
	    if((W[jj]>=0.0)&&( jroot[jj]==-5)) {istate=5;jroot[jj]=1;}
	    else if((W[jj]<0.0)&&( jroot[jj]==5)) {istate=5;jroot[jj]=-1;}
	    else jroot[jj]=0;
	  }
	} 
	/*--discrete zero crossings----dzero--------------------*/
	if (istate!=5){/* if there was a dzero, its event should be activated*/
	C2F(ddaskr)(C2F(simblkdaskr), neq, told, x, xd, &t, 
		    info, &rtol, &Atol, &istate, &rhot[1], &
		    nrwp, &ihot[1], &niwp, rpardummy, ipardummy
		    ,C2F(Jacobian), rpardummy, C2F(grblkdaskr), &ng, jroot);
	}

	if (istate == -1)
	  sciprint("**** Stiffness at: %26.18f %d\r\n",*told,istate);

	phase=1;
	if (*ierr > 5) {
	  freeallx; /* singularity in block */
	  return;
	}
	 
	if (istate <= -2) { /* in case istate==-1 : continue the integration*/
	  *ierr = 100 - istate;
	  freeallx;/* singularity in block */
	  return;
	} else {
	  if (C2F(cosdebug).cosd >= 1) {
	    sciprint("****daskr reached: %f\r\n",*told);
	  }
	  hot = 1;/* successful return from DDASKR => hot restart*/
	}
	
	/*     update outputs of 'c' type  blocks if we are at the end*/
	if (*told >= *tf) {
	  cdoit(told);
	  freeallx;
	  return;
	}
	if (istate == 6) hot=0; /* new feature of daskr, detects unmasking */
	if (istate == 5) {
	  /*     .        at a least one root has been found */
	  hot = 0;
	  if (C2F(cosdebug).cosd >= 1) {
	    sciprint("root found at t=: %f\r\n",*told);
	  }
	  /*     .        update outputs affecting ztyp blocks  ONLY FOR OLD BLOCKS*/
	  zdoit(W, xd, x,told);
	  if (*ierr != 0) {
	    freeallx;
	    return;
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
	      if (funtyp[C2F(curblk).kfun] > 0) {
		if (Blocks[C2F(curblk).kfun-1].nevout > 0) {
		  flag__ = 3;
		  /*     call corresponding block to determine output event (kev) */
		  nclock = -kev;
		  callf(told, xd, x, x,W,&flag__);
		  if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    freeallx;
		    return;
		  }
		  /*     update event agenda */
		  for (k = 0; k < Blocks[C2F(curblk).kfun-1].nevout; ++k) {
		    if (Blocks[C2F(curblk).kfun-1].evout[k] >= 0) {
		      i3 = k + clkptr[C2F(curblk).kfun] ;
		      addevs(Blocks[C2F(curblk).kfun-1].evout[k]+(*told), &i3, &ierr1);
		      if (ierr1 != 0) {
			/*     .                       nevts too small */
			*ierr = 3;
			freeallx;
			return;
		      }
		    }
		  }
		}
		/*     .              update state */
		if (Blocks[C2F(curblk).kfun-1].nx > 0) {
		  /*     .call corresponding block to update state */
		  flag__ = 2;
		  nclock = -kev;
		  pointer_xproperty=
		    &scicos_xproperty[-1+xptr[C2F(curblk).kfun]];
		  n_pointer_xproperty=Blocks[C2F(curblk).kfun-1].nx;
		  callf(told, xd, x, x,W,&flag__);
		  if (flag__ < 0) {
		    *ierr = 5 - flag__;
		    freeallx;
		    return;
		  }
		}
	      }
	    }
	  }
	}
	if (inxsci == 1 && scilab_timer_check() == 1) {
	  C2F(sxevents)();
	  otimer = ntimer;
	  /*     .     sxevents can modify halt */
	}
	if (C2F(coshlt).halt != 0) {
	  C2F(coshlt).halt = 0;
	  freeallx;
	  return;
	}
	  /* if(*pointi!=0){
	    t=tevts[*pointi];
	    if(*told<t-ttol){
	      cdoit(told);
	      goto L15;
	    }
	  }else{
	    if(*told<*tf){
	      cdoit(told);
	      goto L15;
	    }
	    }*/

	/*--discrete zero crossings----dzero--------------------*/
	if (ng>0){ /* storing ZC signs just after a ddaskr call*/
	  zdoit(W, xd, x, told); if (*ierr != 0) {freeallx;return;  }
	  for (jj = 0; jj < ng; ++jj) 
	    if(W[jj]>=0)jroot[jj]=5;else jroot[jj]=-5;
	}
	/*--discrete zero crossings----dzero--------------------*/
      }
      C2F(realtime)(told);
    } else {
      /*     .  t==told */
      if (C2F(cosdebug).cosd >= 1) {
	sciprint("Event: %d activated at t=%f\r\n",*pointi,*told);
      }
      
      ddoit(told);
      if (C2F(cosdebug).cosd >= 1) {
	sciprint("End of activation");
      }
      if (*ierr != 0) {
	freeallx;
	return;
      }
    }
    /*     end of main loop on time */
  }
  freeallx;
} /* cossimdaskr_ */



/* Subroutine */ void cosend(told)
     double *told;
{
  /* Local variables */
  static integer flag__;

  static integer kfune;

  /* Function Body */
  *ierr = 0;
  /*     loop on blocks */
  nclock=0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    flag__ = 5;
    if (funtyp[C2F(curblk).kfun] >= 0) {
      callf(told, xd, x, x,x,&flag__);
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

/* Subroutine */ void doit(told)
     double *told;
{
  /* System generated locals */
  integer i,i2;

  /* Local variables */
  static integer flag__, nord;

  static integer ierr1;
  integer ii, kever;

  /* Function Body */
  kever = *pointi;
  *pointi = evtspt[kever];
  evtspt[kever] = -1;

  nord = ordptr[kever + 1] - ordptr[kever];
  if (nord == 0) {
    return;
  }

  for (ii = ordptr[kever]; ii <=ordptr[kever + 1] - 1 ; ++ii) {
    C2F(curblk).kfun = ordclk[ii];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = abs(ordclk[ii + nordclk]);
      flag__ = 1;
      callf(told, xd, x, x,x,&flag__);

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
	    i= 2;
	  } else {
	    i= 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {

	  i=max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	i2 = i + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	doit(told);
	if (*ierr != 0) {
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
  static integer i,jj;
  
  /* Function Body */
  for (jj = 1; jj <= ncord; ++jj) {
    C2F(curblk).kfun = cord[jj];
    nclock = cord[jj + ncord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xd, x, x,x,&flag__);
	    
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	    i = 2;
	  } else {
	    i = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  i= max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	i2 = i + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	doit(told);
	if (*ierr != 0) {
	  return;
	}
      }
    }
  }
} /* cdoit_ */

/* Subroutine */ void ddoit(told)
     double *told;

{
  /* System generated locals */
  integer i2,j;

  /* Local variables */
  static integer flag__, kiwa;

  static integer i,i3,ierr1;
  static integer  ii, keve;
  double d__1;

  /* Function Body */
  kiwa = 0;
  edoit(told,&kiwa);
  if (*ierr != 0) {
    return;
  }
  
  /*     .  update continuous and discrete states on event */
  if (kiwa == 0) {
    return;
  }
  for (i = 0; i < kiwa; ++i) {
    keve = iwa[i];
    if(critev[keve]!= 0){
      hot = 0;
    }
    i2 = ordptr[keve + 1] - 1;
    for (ii = ordptr[keve]; ii <= i2; ++ii) {
      C2F(curblk).kfun = ordclk[ii];
      nclock=ordclk[ii + nordclk];

      if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
	if (funtyp[C2F(curblk).kfun] >= 0) {
	  d__1 =  - 1.;
	  C2F(dset)(&Blocks[C2F(curblk).kfun - 1].nevout, 
		    &d__1, Blocks[C2F(curblk).kfun-1].evout, &c__1);
	  
	  flag__ = 3;

	  if(nclock>0){ /* if event has continuous origin don't call*/
	    callf(told, xd, x, x ,x,&flag__);
	    if (flag__ < 0) {
	      *ierr = 5 - flag__;
	      return;
	    }
	  }
	  
	  for (j = 0; j < Blocks[C2F(curblk).kfun - 1].nevout; ++j) {
	    if (Blocks[C2F(curblk).kfun-1].evout[j] >= 0.) {
	      i3 = j + clkptr[C2F(curblk).kfun] ;
	      addevs(Blocks[C2F(curblk).kfun-1].evout[j]+(*told), &i3, &ierr1);
	      if (ierr1 != 0) {
		/*     !                 event conflict */
		*ierr = 3;
		return;
	      }
	    }
	  }
	}
      }
      
      if(nclock> 0) {
	if (Blocks[C2F(curblk).kfun-1].nx+Blocks[C2F(curblk).kfun-1].nz > 0||
	    *Blocks[C2F(curblk).kfun-1].work !=NULL) {
	  /*  if a hidden state exists, must also call (for new scope eg)  */
	  /*  to avoid calling non-real activations */
	  flag__ = 2;
	  callf(told, xd, x, x,x,&flag__);
	  if (flag__ < 0) {
	    *ierr = 5 - flag__;
	    return;
	  }
	}
      }else{
	if (*Blocks[C2F(curblk).kfun-1].work !=NULL) {
	  flag__ = 2;
	  nclock=0;  /* in case some hidden continuous blocks need updating */
	  callf(told, xd, x, x,x,&flag__);
	  if (flag__ < 0) {
	    *ierr = 5 - flag__;
	    return;
	  }
	}
      }
    }
  }
} /* ddoit_ */

/* Subroutine */ void edoit(told,kiwa)
     double *told;
     integer *kiwa;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__;
  static integer nord;

  static integer ierr1, i;
  integer kever, ii;
  
  /* Function Body */
  kever = *pointi;
  *pointi = evtspt[kever];
  evtspt[kever] = -1;

  nord = ordptr[kever + 1] - ordptr[kever];
  if (nord == 0) {
    return;
  }
  iwa[*kiwa] = kever;
  ++(*kiwa);
  for (ii = ordptr[kever]; ii <= ordptr[kever + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii];

    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = abs(ordclk[ii + nordclk]);
      flag__ = 1;
      callf(told, xd, x, x,x,&flag__);
	    
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
	    i = 2;
	  } else {
	    i = 1;
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  i= max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);
	}
	i2 = i + clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	edoit(told,kiwa);
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
  
  /* Function Body */
  kiwa = 0;

  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xtd, xt, residual,x,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {
	if(Blocks[C2F(curblk).kfun - 1].nmode > 0){
	  i2 = Blocks[C2F(curblk).kfun - 1].mode[0] + 
	    clkptr[C2F(curblk).kfun] - 1;
	} else{
	  if (funtyp[C2F(curblk).kfun] == -1) {
	    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }
	  } else if (funtyp[C2F(curblk).kfun] == -2) {
	    i=max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);
	  }
	  i2 =i+ clkptr[C2F(curblk).kfun] - 1;
	}
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	ozdoit(xtd, xt,told, &kiwa);
      }
    }
  }
  
  /*     .  update states derivatives */
  for (ii = 1; ii <= noord; ++ii) {
    C2F(curblk).kfun = oord[ii];
    if (Blocks[C2F(curblk).kfun-1].nx > 0||
	*Blocks[C2F(curblk).kfun-1].work !=NULL) {
      /* work tests if a hidden state exists, used for delay block */
      flag__ = 0;
      nclock = oord[ii + noord];
      callf(told, xtd, xt, residual,xt,&flag__);

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
      if (Blocks[C2F(curblk).kfun-1].nx > 0||
	*Blocks[C2F(curblk).kfun-1].work !=NULL) {
	/* work tests if a hidden state exists */
	flag__ = 0;
	nclock = abs(ordclk[ii + nordclk]);
	callf(told, xtd, xt, residual,xt,&flag__);

	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }
  }
} /* odoit_ */


/* Subroutine */ void reinitdoit(told,scicos_xproperty)
     double *told;
     integer *scicos_xproperty;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, kiwa;

  static integer ierr1, i;
  static integer ii, jj;
  
  /* Function Body */
  kiwa = 0;
  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xd, x, x,x,&flag__);
      
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
    
    if (Blocks[C2F(curblk).kfun - 1].nevout > 0 && funtyp[C2F(curblk).kfun] < 0) {
      if (funtyp[C2F(curblk).kfun] == -1) {
	if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	  i=2;
	} else {
	  i=1;
	}
      } else if (funtyp[C2F(curblk).kfun] == -2) {
	i= max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		  Blocks[C2F(curblk).kfun - 1].nevout),1);
      }
      if(Blocks[C2F(curblk).kfun - 1].nmode>0){
	Blocks[C2F(curblk).kfun - 1].mode[0]=i;
      }
      i2 =i+ clkptr[C2F(curblk).kfun] - 1;
      putevs(told, &i2, &ierr1);
      if (ierr1 != 0) {
	/*     !                 event conflict */
	*ierr = 3;
	return;
      }
      doit(told);
      if (*ierr != 0) {
	return;
      }
    }
  }
  
  /*     .  re-initialize */
  for (ii = 1; ii <= noord; ++ii) {
    C2F(curblk).kfun = oord[ii];
    if (Blocks[C2F(curblk).kfun-1].nx > 0) {
      flag__ = 7;
      nclock = oord[ii + noord];
      pointer_xproperty=&scicos_xproperty[-1+xptr[C2F(curblk).kfun]];
      n_pointer_xproperty=Blocks[C2F(curblk).kfun-1].nx;
      callf(told, xd, x, xd,x,&flag__);

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
	flag__ = 7;
	nclock = abs(ordclk[ii + nordclk]);
	n_pointer_xproperty=Blocks[C2F(curblk).kfun-1].nx;
	pointer_xproperty=&scicos_xproperty[-1+xptr[C2F(curblk).kfun]];
	callf(told, xd, x, xd,x,&flag__);

	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }
  }
} /* reinitdoit_ */

/* Subroutine */ void ozdoit(xtd, xt, told, kiwa)
     double *xtd, *xt;
     double *told;
     integer *kiwa;
{
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, nord;

  static integer ierr1, i;
  integer ii, kever; 

  /* Function Body */
  kever = *pointi;
  *pointi = evtspt[kever];
  evtspt[kever] = -1;

  nord = ordptr[kever + 1] - ordptr[kever];
  if (nord == 0) {
    return;
  }
  iwa[*kiwa] = kever;
  ++(*kiwa);

  for (ii = ordptr[kever]; ii <= ordptr[kever + 1] - 1; ++ii) {
    C2F(curblk).kfun = ordclk[ii];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      nclock = abs(ordclk[ii + nordclk]);
      flag__ = 1;
      callf(told, xtd, xt, xt,x,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }
    /*     .     Initialize tvec */
    
    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {

      if (funtyp[C2F(curblk).kfun] < 0) {

	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (phase==1 || Blocks[C2F(curblk).kfun - 1].nmode==0){
	    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }
	  }else{
	    i=Blocks[C2F(curblk).kfun - 1].mode[0];
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  if (phase==1 || Blocks[C2F(curblk).kfun - 1].nmode==0){
	    i= max(min((integer) 
		       outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		       Blocks[C2F(curblk).kfun - 1].nevout),1);
	  }else{
	    i=Blocks[C2F(curblk).kfun - 1].mode[0];
	    
	  }
	}
	i2 =i+clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	ozdoit(xtd, xt,told, kiwa);
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
  static integer ierr1, i,j;
  static integer ii, jj;
  
  /* Function Body */
  C2F(dset)(&ng, &c_b14,g , &c__1);

  kiwa = 0;
  for (jj = 1; jj <= nzord; ++jj) {
    C2F(curblk).kfun = zord[jj];
    nclock = zord[jj + nzord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;
      callf(told, xtd, xt, xt,xt,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {


	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (phase==1|| Blocks[C2F(curblk).kfun - 1].nmode==0){
	    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }
	  }else{
	    i=Blocks[C2F(curblk).kfun - 1].mode[0];
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  if (phase==1|| Blocks[C2F(curblk).kfun - 1].nmode==0){
	    i=max(min((integer) 
		      outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);
	  }else{
	    i=Blocks[C2F(curblk).kfun - 1].mode[0];
	  }
	}
	i2 =i+clkptr[C2F(curblk).kfun] - 1;
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	ozdoit(xtd, xt,told, &kiwa);
      }
    }
  }
    
  /*     .  update zero crossing surfaces */
  for (ii = 1; ii <= nzord; ++ii) {
    C2F(curblk).kfun = zord[ii];
    if (Blocks[C2F(curblk).kfun-1].ng > 0) {
      if (funtyp[C2F(curblk).kfun] > 0) {
	flag__ = 9;
	nclock = zord[ii +nzord];
	callf(told, xtd, xt, xtd,g,&flag__);
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }else{
	if (funtyp[C2F(curblk).kfun] == -1) {
	  g[zcptr[C2F(curblk).kfun]-1]=outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]];
	  if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
	    if (g[zcptr[C2F(curblk).kfun]-1] <= 0.) {
	      Blocks[C2F(curblk).kfun - 1].mode[0] = 2;
	    }
	    else {
	      Blocks[C2F(curblk).kfun - 1].mode[0] = 1;
	    }
	  }
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  for (jj=0;jj<Blocks[C2F(curblk).kfun - 1].nevout-1;++jj) {
	    g[zcptr[C2F(curblk).kfun]-1+jj]=
	      outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]]
	      -(double)(jj+2);
	  }
	  if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
	    j=max(min((integer) 
		      outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);
	    Blocks[C2F(curblk).kfun - 1].mode[0]= j;
	  }
	}
      }
    }
  }
  for (i = 0; i < kiwa; ++i) {
    keve = iwa[i];
    for (ii = ordptr[keve]; ii <= ordptr[keve + 1] - 1; ++ii) {
      C2F(curblk).kfun = ordclk[ii ];
      if (Blocks[C2F(curblk).kfun-1].ng > 0) {
	if (funtyp[C2F(curblk).kfun] > 0) {
	  flag__ = 9;
	  nclock = abs(ordclk[ii + nordclk]);
	  callf(told, xtd, xt, xtd,g,&flag__);
	  
	  if (flag__ < 0) {
	    *ierr = 5 - flag__;
	    return;
	  }
	}else{
	  if (funtyp[C2F(curblk).kfun] == -1) {
	    g[zcptr[C2F(curblk).kfun]-1]=
	      outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]];
	    if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
	      if (g[zcptr[C2F(curblk).kfun]-1] <= 0.) {
		Blocks[C2F(curblk).kfun - 1].mode[0] = 2;
	      } else {
		Blocks[C2F(curblk).kfun - 1].mode[0] = 1;
	      }
	    }
	  } else if (funtyp[C2F(curblk).kfun] == -2) {
	    for (jj=0;jj<Blocks[C2F(curblk).kfun - 1].nevout-1;++jj) {
	      g[zcptr[C2F(curblk).kfun]-1+jj]=
		outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]]
		-(double)(jj+2);
	    }
	    if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
	      j=max(min((integer) 
			outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
			Blocks[C2F(curblk).kfun - 1].nevout),1);
	      Blocks[C2F(curblk).kfun - 1].mode[0]= j;
	    }
	  }
	}
      }
    }
  } /* zdoit_ */
}





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

  if (kf==(debug_block+1)) return; /* debug block is never called */

  block_error=flag;  /* to return error from blocks of type 4 */

  flagi=*flag; /* flag 7 implicit initialization */
  if(flagi==7 && Blocks[kf-1].type<10000) *flag=0;

  if ( cosd > 1){
    sciprint("block %d is called ",kf);
    sciprint("with flag %d ",*flag);
    sciprint("at time %f \r\n",*t);
    if(debug_block>-1){
      sciprint("Entering the block \r\n");
      call_debug_scicos(t,xtd,xt,residual,g,flag,kf,flagi,debug_block);
      if (*flag<0) return;  /* error in debug block */
    }
  }

  
  C2F(scsptr).ptr=Blocks[kf-1].scsptr; /* set scilab function adress for sciblk */

  
  loc=Blocks[kf-1].funpt;
  if (Blocks[kf-1].type==4||Blocks[kf-1].type==10004) {
    scicos_time=*t;
    Blocks[kf-1].nevprt=nclock;
    loc4 = (ScicosF4) loc;
    if(Blocks[kf-1].ng>0){
	Blocks[kf-1].g=&g[zcptr[kf]-1];
      }
    if(Blocks[kf-1].nx==0){
      (*loc4)(&Blocks[kf-1],*flag);
    } 
    else {
      Blocks[kf-1].x=&xt[xptr[kf]-1];
      if(Blocks[kf-1].type==4) {
	if(*flag==0 && solver==100) {
	  Blocks[kf-1].res=&residual[xptr[kf]-1];
	  Blocks[kf-1].xd=&residual[xptr[kf]-1];
	  (*loc4)(&Blocks[kf-1],*flag);
	  Blocks[kf-1].xd=&xtd[xptr[kf]-1];
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
    if ( cosd > 1){
      if(debug_block>-1){
	if (*flag<0) return;  /* error in block */
	sciprint("Leaving block %d \r\n",kf);
	call_debug_scicos(t,xtd,xt,residual,g,flag,kf,flagi,debug_block);
      }
    }
    return;
  }
  
  /*This is for compatibility*/
  if(nclock<0){
      for (j =0;j<Blocks[kf-1].ng;++j){
	Blocks[kf-1].g[j]=(double)Blocks[kf-1].jroot[j];
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
  else
  {
	Blocks[kf-1].x  = NULL ;
	Blocks[kf-1].xd = NULL ;
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
	      Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
	      Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
    /* catenate inputs if necessary */
    ni=0;
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
	no=0;
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
	      Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	      Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double *)args[0],&ni,
	      (double *)args[1],&no);
    }
    else {
      (*loc0)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
	      Blocks[kf-1].z,&Blocks[kf-1].nz,
	      Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
		Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
		Blocks[kf-1].insz,&Blocks[kf-1].nin,
		Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
      }
      else{
	loc2z = (ScicosF2z) loc;
	(*loc2z)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].x,&Blocks[kf-1].nx,
		 Blocks[kf-1].z,&Blocks[kf-1].nz,
		 Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
		Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
		Blocks[kf-1].insz,&Blocks[kf-1].nin,
		Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
      }
      else{
	loc2z = (ScicosF2z) loc;
	(*loc2z)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		 Blocks[kf-1].z,&Blocks[kf-1].nz,
		 Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
	     Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
	       Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
	       Blocks[kf-1].ipar,&Blocks[kf-1].nipar,Blocks[kf-1].inptr,
	       Blocks[kf-1].insz,&Blocks[kf-1].nin,
	       Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
    }
    else {
      loci2z = (ScicosFi2z) loc;
      
      (*loci2z)(flag,&nclock,t,Blocks[kf-1].res,
		Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		Blocks[kf-1].z,&Blocks[kf-1].nz,
		Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
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
  for(in=0;in<Blocks[kf-1].nevout;++in){
    Blocks[kf-1].evout[in]=Blocks[kf-1].evout[in]-*t;
  }
  if ( cosd > 1){
    if(debug_block>-1){
      if (*flag<0) return;  /* error in block */
      sciprint("Leaving block %d \r\n",kf);
      call_debug_scicos(t,xtd,xt,residual,g,flag,kf,flagi,debug_block);
    }
  }
}


void call_debug_scicos(t,xtd,xt,residual,g,flag,kf,flagi,deb_blk)
     integer *flag,kf,flagi,deb_blk;
     double *t,*xtd,*xt,*residual,*g;
{
  voidf loc ; 
  int solver=C2F(cmsolver).solver,k;
  ScicosF4 loc4;
  C2F(cosdebugcounter).counter=C2F(cosdebugcounter).counter+1;

  C2F(scsptr).ptr=Blocks[deb_blk].scsptr;
  loc=Blocks[deb_blk].funpt;
  scicos_time=*t;
  Blocks[kf-1].nevprt=nclock;
  loc4 = (ScicosF4) loc;
  if(Blocks[kf-1].ng>0){
    Blocks[kf-1].g=&g[zcptr[kf]-1];
  }
  if(Blocks[kf-1].nx==0){
    (*loc4)(&Blocks[kf-1],*flag);
  } 
  else {
    Blocks[kf-1].x=&xt[xptr[kf]-1];
    if(*flag==0 && solver==100) {
      Blocks[kf-1].res=&residual[xptr[kf]-1];
      Blocks[kf-1].xd=&residual[xptr[kf]-1];
      (*loc4)(&Blocks[kf-1],*flag);
      Blocks[kf-1].xd=&xtd[xptr[kf]-1];
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
  if (*flag<0) sciprint("Error in the Debug block \r\n");
}
  




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
  C2F(dset)(neq, &c_b14,xcdot , &c__1);
  C2F(ierode).iero = 0;
  *ierr= 0;
  odoit(xcdot, xc,xcdot,t);
  C2F(ierode).iero = *ierr;
  return 0;
}
 
int C2F(simblkdaskr)(t,xc,xcdot,cj,residual,ires,rpar1,ipar1)
     integer *ires,*ipar1;
     double *t, *xc, *xcdot, *rpar1, *residual;
     double *cj;
     
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
  CJJ=*cj;
  C2F(dcopy)(neq, xcdot, &c__1, residual, &c__1);
  *ires=0;
  *ierr= 0;
  C2F(ierode).iero = 0;
  odoit(residual, xc, xcdot,t);
  C2F(ierode).iero = *ierr;
  if(C2F(ierode).iero != 0) *ires=-1;
  return 0;
}
 

int C2F(grblkdaskr)(neq1, t, xc, xtd,ng1, g,rpar1,ipar1)
     integer *neq1;
     double *t, *xc, *xtd;
     integer *ng1,*ipar1;
     double *g,*rpar1;
{
  *ierr= 0;
  C2F(ierode).iero = 0;
  zdoit(g, xtd, xc,t);
  C2F(ierode).iero = *ierr;
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
       !
     */
     
     /* Local variables */
     
{ 
 C2F(ierode).iero = 0;
 *ierr= 0;
 zdoit(g,xc, xc,t);
 C2F(ierode).iero = *ierr;
 return 0;
}


/* Subroutine */ void addevs(t, evtnb, ierr1)
     double t;
     integer *evtnb, *ierr1;
{
  static integer i, j;

  /* Function Body */
  *ierr1 = 0;
  if (evtspt[*evtnb] != -1) {
    *ierr1 = 1;
    return;
  } else {
    evtspt[*evtnb] = 0;
    tevts[*evtnb] = t;
  }
  if (*pointi == 0) {
    *pointi = *evtnb;
    return;
  }
  if (t < tevts[*pointi]) {
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
  if (t >= tevts[evtspt[i]]) {
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



void FREE_blocks()

{
  int kf;
  for (kf = 0; kf < nblk; ++kf) {
    if (Blocks[kf].insz!=NULL) {
      FREE(Blocks[kf].insz);
    }else {
      break;
    }
    if (Blocks[kf].inptr!=NULL){
      FREE(Blocks[kf].inptr);
    }else {
      break;
    }
    if (Blocks[kf].outsz!=NULL){
      FREE(Blocks[kf].outsz);
    }else {
      break;
    }
    if (Blocks[kf].outptr!=NULL){
      FREE(Blocks[kf].outptr);
    }else {
      break;
    }
    if (Blocks[kf].label!=NULL){
      FREE(Blocks[kf].label);
    }else {
      break;
    }
    if (Blocks[kf].evout!=NULL){
      FREE(Blocks[kf].evout);
    }else {
      break;
    }
  }
  FREE(Blocks);
  if(nmod>0){
    FREE(mod);
  }
  return;
}
  


int setmode(W,x,told,jroot,ttol)
     /* work space W needs to be ng+*neq*2 */
     double *W,*x,*told,ttol;
     int *jroot;
{
  int k,j,jj,diff;
  double ttmp;

  ttmp=*told+ttol;
  zdoit(W,x,x,told);  /*fix the mode*/
  if (*ierr != 0) return 1;
  for(jj=0;jj<*neq;++jj){
    W[jj]=x[jj];
  } 
  diff=1;
  k=0;
  while (diff!=0){
    /*save modes */
    for(jj=0;jj<nmod;++jj){ 
      jroot[jj]=mod[jj];
    }
    for(j=0;j<=*neq;++j){
      C2F(simblk)(neq, &ttmp, W, &W[*neq]);  
      if (*ierr != 0) return 1;
      for(jj=0;jj<*neq;++jj){
	W[jj]=x[jj]+ttol*W[jj+(*neq)];
      } 
    }
    /*recompute modes*/
    zdoit(&W[2*(*neq)],W,W,&ttmp);
    if (*ierr != 0) return 1;
    /*test against saved modes*/
    diff=0;
    for(jj=0;jj<nmod;++jj){ 
      if (jroot[jj]!=mod[jj]) {
	if(k>*neq) {
	  *ierr=22;
	  return 1;
	}
	k=k+1;
	diff=1;
	break;
      }
    }
  }  
  return 0;
}

  
int get_phase_simulation()

{
  return phase;
}



void do_cold_restart()

{
  hot=0;
  return;
}

double get_scicos_time()

{
  return scicos_time;
}
int get_block_number()

{
  return C2F(curblk).kfun;
}

void set_block_error(int err)

{
  *block_error=err;
  return;
}


void set_pointer_xproperty(int* pointer)
{
  int i;
  for (i=0;i<n_pointer_xproperty;i++){
    pointer_xproperty[i]=pointer[i];
  }
  return;
}

/* Jacobian*/
void Set_Jacobian_flag(int flag)
{
  Jacobian_Flag=flag;
  return;
}

double Get_Jacobian_parameter(void)
{
  return CJJ;
}

double Get_Scicos_SQUR(void)
{
  return  SQuround;
}

void Jdoit(residual, xt, xtd, told, job)
     double *residual, *xt, *xtd;
     double *told;
     int *job;
{ 
  /* System generated locals */
  integer i2;

  /* Local variables */
  static integer flag__, keve, kiwa;
  static integer ierr1, i;
  static integer ii, jj;
  /* Function Body */
  kiwa = 0;
  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {
      flag__ = 1;

      if ((*job==2)&&(oord[jj]==nblk)) {/* applying desired output */
      }else
	callf(told, xtd, xt, residual,x,&flag__);      
      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {
	if(Blocks[C2F(curblk).kfun - 1].nmode > 0){
	  i2 = Blocks[C2F(curblk).kfun - 1].mode[0] + 
	    clkptr[C2F(curblk).kfun] - 1;
	} else{
	  if (funtyp[C2F(curblk).kfun] == -1) {
	    if (outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }
	  } else if (funtyp[C2F(curblk).kfun] == -2) {
	    i=max(min((integer) outtb[-1+lnkptr[inplnk[inpptr[C2F(curblk).kfun]]]],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);
	  }
	  i2 =i+ clkptr[C2F(curblk).kfun] - 1;
	}
	putevs(told, &i2, &ierr1);
	if (ierr1 != 0) {
	  /*     !                 event conflict */
	  *ierr = 3;
	  return;
	}
	ozdoit(xtd, xt,told, &kiwa);
      }
    }
  }

  /*     .  update states derivatives */
  for (ii = 1; ii <= noord; ++ii) {
    C2F(curblk).kfun = oord[ii];
    if (Blocks[C2F(curblk).kfun-1].nx > 0||
	*Blocks[C2F(curblk).kfun-1].work !=NULL) {
      /* work tests if a hidden state exists, used for delay block */
      flag__ = 0;
      if (((*job==1)&&(oord[ii]==nblk))||(*job!=1)){
	if (*job==1)  flag__ = 10;
	nclock = oord[ii + noord];
	callf(told, xtd, xt, residual,xt,&flag__);
      };
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
      if (Blocks[C2F(curblk).kfun-1].nx > 0||
	*Blocks[C2F(curblk).kfun-1].work !=NULL) {
	/* work tests if a hidden state exists */

	flag__ = 0;
	if (((*job==1)&&(oord[ii]==nblk))||(*job!=1)){
	  if (*job==1)  flag__ = 10;
	  nclock = abs(ordclk[ii + nordclk]);
	  callf(told, xtd, xt, residual,xt,&flag__);
	}
	if (flag__ < 0) {
	  *ierr = 5 - flag__;
	  return;
	}
      }
    }
  }
} /* odoit_ */
/*-----------------------------------------------------------------------*/

int C2F(Jacobian)(t,xc, xcdot,residual,cj,rpar1,ipar1)
     int *ipar1;
     double *t, *xc, *xcdot, *rpar1, *residual;
     double *cj;
{ 
  int i,j,n, nx,ni,no,nb,m;
  double *RX, *Fx, *Fu, *Gx, *Gu, *Ewt,*H,*SQUR,*ERR1,*ERR2;
  double *Hx, *Hu,*Kx,*Ku,*HuGx,*FuKx,*FuKuGx,*HuGuKx;
  double del,delinv,xsave,xdsave,ysave;
  double a,b;
  int job;
  double **y = Blocks[nblk-1].outptr;
  double **u = Blocks[nblk-1].inptr;
  /*  taill1= 2+3*n+(n+ni)*(n+no)+nx(2*nx+ni+2*m+no)+m*(2*m+no+ni)+2*ni*no*/
  *ierr= 0;
  CJJ=*cj;
  n=*neq; 
  nb=nblk;
  nx=Blocks[nblk-1].nx;m=n-nx;
  no=Blocks[nblk-1].nout;
  ni=Blocks[nblk-1].nin;
  H=residual+ n * n;
  SQUR=H+1;
  Ewt=SQUR+1;
  ERR1=Ewt+n;
  ERR2=ERR1+n;
  RX=ERR2+n;
  Fx=RX+(n+ni)*(n+no); /* car (nx+ni)*(nx+no) peut etre > `a n*n*/
  Fu=Fx+nx*nx;
  Gx=Fu+nx*ni;
  Gu=Gx+no*nx;
  Hx=Gu+no*ni;
  Hu=Hx+m*m; 
  Kx=Hu+m*no;
  Ku=Kx+ni*m;
  HuGx=Ku+ni*no;
  FuKx=HuGx+m*nx;
  FuKuGx=FuKx+nx*m;
  HuGuKx=FuKuGx+nx*nx;
  /* HuGuKx+m*m;=.  m*m=size of HuGuKx */
  /* ------------------ Numerical Jacobian--->> Hx,Kx */
  job=0;/* read residuals;*/
  Jdoit(ERR1, xc, xcdot,t,&job);
  if (*ierr < 0) return -1;

  for (i=0;i<m;i++)
    for (j=0;j<ni;j++)
      Kx[j+i*ni]=u[j][0];

  for(i=0;i<m;i++){
    a= max(abs(H[0]*xcdot[i]),abs(1.0/Ewt[i]));
    b= max(1.0,abs(xc[i]));
    del=SQUR[0]*max(a,b);
    if (H[0]*xcdot[i]<0) del*=-1;
    del=(xc[i]+del)-xc[i];
    xsave=xc[i];
    xdsave=xcdot[i];
    xc[i]+=del;
    xcdot[i]+=CJJ*del;
    job=0;/* read residuals */
    Jdoit(ERR2, xc, xcdot,t,&job);
    if (*ierr < 0) return -1;
    delinv=1.0/del;
    for(j=0;j<m;j++)
      Hx[m*i+j]=(ERR2[j]-ERR1[j])*delinv;
    for (j=0;j<ni;j++)
      Kx[j+i*ni]=(u[j][0]-Kx[j+i*ni])*delinv;
    xc[i]=xsave;
    xcdot[i]=xdsave;
  }
  /*----- Numerical Jacobian--->> Hu,Ku */
  job=0;
  Jdoit(ERR1, xc, xcdot,t,&job);
  for (i=0;i<no;i++)
    for (j=0;j<ni;j++)
      Ku[j+i*ni]=u[j][0];

  for(i=0;i<no;i++){
    del=SQUR[0]* max(1.0,abs(y[i][0]));
    del=(y[i][0]+del)-y[i][0];
    ysave=y[i][0];
    y[i][0]+=del;
    job=2;/* applying y[i][0] to the output of imp block*/
    Jdoit(ERR2, xc, xcdot,t,&job);
    if (*ierr < 0) return -1;
    delinv=1.0/del;
    for(j=0;j<m;j++)
      Hu[m*i+j]=(ERR2[j]-ERR1[j])*delinv;
    for (j=0;j<ni;j++)
      Ku[j+i*ni]=(u[j][0]-Ku[j+i*ni])*delinv;
    y[i][0]=ysave;
  }
  /*----------------------------------------------*/
  job=1;/* read jacobian through flag=10; */
  Jdoit(&Fx[-m], xc, xcdot,t,&job);/* Filling up the FX:Fu:Gx:Gu*/
  if (*block_error!=0) sciprint("\n\r error in Jacobian");
  /*-------------------------------------------------*/
  Multp(Fu,Ku,RX,nx,ni,ni,no);Multp(RX,Gx,FuKuGx,nx,no,no,nx);
  for (i=0;i<nx;i++)
    for (j=0;j<nx;j++){
      residual[i+m+(j+m)*n]=Fx[i+j*nx]+FuKuGx[i+j*nx];
    }

  Multp(Hu,Gx,HuGx,m, no, no,nx);
  for (i=0;i<nx;i++)
    for (j=0;j<m;j++){
      residual[j+(i+m)*n]=HuGx[j+i*m];
    }

  Multp(Fu,Kx,FuKx,nx,ni,ni,m);
  for (i=0;i<m;i++)
    for (j=0;j<nx;j++){
      residual[(j+m)+i*n]=FuKx[j+i*nx];
    }

  Multp(Hu,Gu,RX,m,no,no,ni); Multp(RX,Kx,HuGuKx,m,ni,ni,m);
  for (i=0;i<m;i++)
    for (j=0;j<m;j++){
      residual[i+(j)*n]=Hx[i+j*m]+HuGuKx[i+j*m]; 
    }

  /*  chr='R'; DISP(residual,n,n,&chr);*/
  C2F(ierode).iero = *ierr;
 return 0;

}
/*----------------------------------------------------*/
void Multp(A,B,R,ra ,ca, rb,cb)
     double *A, *B, *R;
     int ra,rb,ca,cb;
{ 
  int i,j,k;
  if (ca!=rb) sciprint("\n\r Error in matrix multiplication");
  for (i = 0; i<ra; i++)
    for (j = 0; j<cb; j++){
      R[i+ra*j]=0.0;
      for (k = 0; k<ca; k++)
	R[i+ra*j]+=A[i+k*ra]*B[k+j*rb];
    }
  return;
}

/* void DISP(A,ra ,ca,name)
     double *A;
     int ra,ca,*name;
{
  int i,j;
  sciprint("\n\r");
  for (i=0;i<ca;i++)
    for (j=0;j<ra;j++){
      if (A[j+i*ra]!=0) 
       sciprint(" %s(%d,%d)=%g;",name,j+1,i+1,A[j+i*ra]);
    }; 
}*/
/* Jacobian*/


