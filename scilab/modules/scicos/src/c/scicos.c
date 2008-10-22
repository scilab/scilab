/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/* 11-03-2005,  Masoud
 * adding A-Jacobian
 * istate =-1 case;
 *
 * xx/03/06, Alan
 * enable matrix typed
 * input/output regular ports
 *
 * xx/02/07, Alan
 * added object paramaters/states
 *
 * 20/07/07,  Masoud
 * CVODE (sundials) replaced LSODAR
 * IDA  (Sundials)  replaced DASKR
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dynamic_link.h"
#include "scicos-def.h"
#include "stack-def.h"
#include "sciprint.h"
#include "scicos.h"
#include "import.h"
#include "blocks.h"
#include "core_math.h"
#include "dynamic_menus.h"
#include "syncexec.h"

#include "math_graphics.h"
#include "MALLOC.h"  /* malloc */

/* Sundials includes */
#include "cvode.h"           /* prototypes for CVODES fcts. and consts. */
#include "cvode_dense.h"     /* prototype for CVDense */
#include "ida.h"
#include "ida_dense.h"
#include "nvector_serial.h"  /* serial N_Vector types, fcts., and macros */
#include "sundials_dense.h" /* definitions DenseMat and DENSE_ELEM */
#include "sundials_types.h" /* definition of type realtype */
#include "sundials_math.h"
#include "ida_impl.h"

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

typedef struct {
	int iero;
} IERSCODE_struct;

IERSCODE_struct C2F(ierscode);


typedef struct {
  void *ida_mem;
  N_Vector ewt;
  double *rwork;
  int *iwork;
  double *gwork; /* just added for a very special use: a
		    space passing to grblkdakr for zero crossing surfaces
		    when updating mode variables during initialization */
} *UserData;

/* defined in scicos-def.h */
/*
#ifdef FORDLL
#define IMPORT  __declspec (dllimport)
#else
#define IMPORT extern
#endif
*/

#ifdef abs
	#undef abs
#endif
#define abs(x) ((x) >= 0 ? (x) : -(x))
#ifndef WIN32
#define max(a,b) ((a) >= (b) ? (a) : (b))
#define min(a,b) ((a) <= (b) ? (a) : (b))
#endif

#define freeall \
              if (*neq>0) CVodeFree(&cvode_mem);\
              if (*neq>0) N_VDestroy_Serial(y);\
	      if ( ng>0 ) FREE(jroot);\
	      if ( ng>0 ) FREE(zcros);

#define freeallx \
              if (*neq>0 && (Jacobian_Flag>0))  FREE(data->rwork);\
	      if ( ng>0 ) FREE(data->gwork);\
              if (*neq>0) N_VDestroy_Serial(data->ewt);\
              if (*neq>0) FREE(data);\
              if (*neq>0) IDAFree(&ida_mem);\
              if (*neq>0) N_VDestroy_Serial(IDx);\
              if (*neq>0) N_VDestroy_Serial(yp);\
              if (*neq>0) N_VDestroy_Serial(yy);\
              if ( ng>0 ) FREE(jroot);\
              if ( ng>0 ) FREE(zcros);\
              if (nmod>0) FREE(Mode_save);

#define freeouttbptr \
              FREE(outtbd);\
              FREE(outtbc);\
              FREE(outtbs);\
              FREE(outtbl);\
              FREE(outtbuc);\
              FREE(outtbus);\
              FREE(outtbul);


#define ONE   RCONST(1.0)
#define ZERO  RCONST(0.0)
#define T0    RCONST(0.0)
/* #define Ith(v,i)    NV_Ith_S(v,i-1)*/        /* Ith numbers components 1..NEQ */
/* #define IJth(A,i,j) DENSE_ELEM(A,i-1,j-1)*/  /* IJth numbers rows,cols 1..NEQ */

static int check_flag(void *flagvalue, char *funcname, int opt);
void cosini(double *);
void idoit(double *);
void cosend(double *);
void cdoit(double *);
void doit(double *);
void ddoit(double *);
void edoit(double *,int *);
void odoit(double *,double *,double *,double *);
void ozdoit(double *,double *,double *,int *);
void zdoit(double *,double *,double *,double *);
void reinitdoit(double *, realtype *);
void cossimdaskr(double *);
void cossim(double *);
void callf(double *, double *, double *, double *,double *,int *);
int simblk(realtype t,N_Vector yy,N_Vector yp, void *f_data);
int simblkdaskr(realtype tres, N_Vector yy, N_Vector yp, N_Vector resval, void *rdata);
int grblkdaskr(realtype t, N_Vector yy, N_Vector yp, realtype *gout, void *g_data);
int grblk(realtype t, N_Vector yy, realtype *gout, void *g_data);
void addevs(double ,int *,int *);
void putevs(double *,int *,int *);
void FREE_blocks(void);
int setmode(double *,double *,double *,int *,double);

/* Jacobian*/
void Jdoit(double *,double *,double *,double *,int *);
int Jacobians(long int Neq, realtype, N_Vector, N_Vector, N_Vector, realtype, void *, DenseMat, N_Vector, N_Vector, N_Vector);
void Multp(double *, double *,double *, int, int, int ,int);
void Set_Jacobian_flag(int flag);
double Get_Jacobian_parameter(void);
double Get_Scicos_SQUR(void);
/*void DISP(A,ra ,ca,name);*/
/* Jacobian*/

/* defined in scicos-def.h */
/*
IMPORT struct {
  int cosd;
} C2F(cosdebug);


IMPORT struct {
  int counter;
} C2F(cosdebugcounter);

struct {
  int solver;
} C2F(cmsolver);
*/
extern void  F2C(sciblk)();
extern void  sciblk2();
extern void  sciblk4();
extern void  GetDynFunc();
extern void  C2F(iislink)();


extern  int C2F(cvstr)();
extern  int C2F(dset)();
extern  int C2F(dcopy)();
extern  int C2F(iset)();
extern  int C2F(realtime)();
extern  int C2F(realtimeinit)();
extern  int C2F(sxevents)();
extern  int C2F(stimer)();
extern  int C2F(xscion)();

extern int scilab_timer_check();

static int nblk, nordptr, nlnk, ng, ncord, noord, nzord,niord,
  nclock,nordclk,niord,nmod;

static int *neq;

static  double Atol, rtol, ttol, deltat,hmax;
static int hot;

/* Variable declaration moved to scicos.c because it was in the scicos-def.h therefore
 * multiple declaration of the variable and linkers were complaining about duplicate
 * symbols
 */

COSDEBUGCOUNTER_struct C2F(cosdebugcounter);
IMPORT_SCICOS  SOLVER_struct C2F(cmsolver);
IMPORT_SCICOS CURBLK_struct C2F(curblk);
IMPORT_SCICOS RTFACTOR_struct C2F(rtfactor);
IMPORT_SCICOS SCSPTR_struct C2F(scsptr);
IMPORT_SCICOS DBCOS_struct C2F(dbcos);
IMPORT_SCICOS COSTOL_struct C2F(costol);
IMPORT_SCICOS COSHLT_struct C2F(coshlt);
IMPORT_SCICOS COSDEBUG_struct C2F(cosdebug);

/* defined in scicos-def.h */
/*
extern struct {
  int iero;
} C2F(ierode);

extern  struct {
  int kfun;
} C2F(curblk);

struct {
  double scale;
}  C2F(rtfactor);

extern struct {
  int halt;
}  C2F(coshlt);
*/
/* Table of constant values */

static int c__90 = 90;
static int c__0 = 0;
static int c__91 = 91;
static double c_b14 = 0.;
static int c__1 = 1;
static int *iwa;

static int *xptr,*modptr, *evtspt;
static int  *funtyp, *inpptr, *outptr, *inplnk, *outlnk;
static int *clkptr, *ordptr, *ordclk, *cord,
  *iord, *oord,  *zord,  *critev,  *zcptr;
static int *pointi;
static int *ierr;

static double *x,*xd,*tevts,*g;
static int *mod;
static double *t0,*tf,scicos_time;

/* declaration of ptr for typed port */
static void **outtbptr;     /*pointer array of object of outtb*/
static int *outtbsz;    /*size of object of outtb*/
static int *outtbtyp;   /*type of object of outtb*/

SCSREAL_COP *outtbdptr;     /*to store double of outtb*/
SCSINT8_COP *outtbcptr;     /*to store int8 of outtb*/
SCSINT16_COP *outtbsptr;    /*to store int16 of outtb*/
SCSINT32_COP *outtblptr;    /*to store int32 of outtb*/
SCSUINT8_COP *outtbucptr;   /*to store unsigned int8 of outtb */
SCSUINT16_COP *outtbusptr;  /*to store unsigned int16 of outtb */
SCSUINT32_COP *outtbulptr;  /*to store unsigned int32 of outtb */

static outtb_el *outtb_elem;
static int nelem;
static scicos_block *Blocks;

static int phase;

realtype *pointer_xproperty;

int n_pointer_xproperty;

static int *block_error;
  /* Jacobian*/
static int Jacobian_Flag;
static int AJacobian_block;
static double  CJJ;
static double SQuround;
  /* Jacobian*/

void call_debug_scicos(double *, double *, double *, double *,double *,int
 *,int,int,int);

static int debug_block;


//** ----------- The Very Scicos simulation engine start here ---------------------------

/* Subroutine */
int C2F(scicos)(double *x_in, int *xptr_in, double *z__,
                void **work,int *zptr,int *modptr_in,
                void **oz,int *ozsz,int *oztyp,int *ozptr,
                int *iz,int *izptr,double *t0_in,
                double *tf_in,double *tevts_in,int *evtspt_in,
                int *nevts,int *pointi_in,void **outtbptr_in,
                int *outtbsz_in,int *outtbtyp_in,
                outtb_el *outtb_elem_in,int *nelem1,int *nlnk1,
                int *funptr,int *funtyp_in,int *inpptr_in,
                int *outptr_in, int *inplnk_in,int *outlnk_in,
                double *rpar,int *rpptr,int *ipar,int *ipptr,
                void **opar,int *oparsz,int *opartyp,int *opptr,
                int *clkptr_in,int *ordptr_in,int *nordptr1,
                int *ordclk_in,int *cord_in,int *ncord1,
                int *iord_in,int *niord1,int *oord_in,
                int *noord1,int *zord_in,int *nzord1,
                int *critev_in,int *nblk1,int *ztyp,
                int *zcptr_in,int *subscr,int *nsubs,
                double *simpar,int *flag__,int *ierr_out)
{
  int i1,kf,lprt,in,out,job=1;

  extern /* Subroutine */ int C2F(msgs)();
  static int mxtb, ierr0, kfun0, i, j, k, jj;
  extern /* Subroutine */ int C2F(makescicosimport)();
  extern /* Subroutine */ int C2F(getscsmax)();
  static int ni, no;
  extern /* Subroutine */ int C2F(clearscicosimport)();
  static int nx, nz, noz, nopar;
  double *W;

  // Set FPU Flag to Extended for scicos simulation
  // in order to override Java setting it to Double.
#if defined(linux) && defined(__i386__)
  setFPUToExtended();
#endif

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
  --ozptr;
  evtspt=evtspt_in-1;
  tevts=tevts_in-1;
  outtbptr=outtbptr_in;
  outtbsz=outtbsz_in;
  outtbtyp=outtbtyp_in;
  outtb_elem=outtb_elem_in;
  funtyp=funtyp_in-1;
  inpptr=inpptr_in-1;
  outptr=outptr_in-1;
  inplnk=inplnk_in-1;
  outlnk=outlnk_in-1;
  --rpptr;
  --ipptr;
  --opptr;
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
  C2F(cmsolver).solver = (int)  simpar[6];
  hmax=simpar[7];

  nordptr = *nordptr1;
  nblk = *nblk1;
  ncord = *ncord1;
  noord = *noord1;
  nzord = *nzord1;
  niord = *niord1;
  nlnk  = *nlnk1;
  nelem = *nelem1;
  *ierr = 0;

  xd=&x[xptr[nblk+1]-1];

  nordclk=ordptr[nordptr]-1;
  /*     computes number of zero crossing surfaces */
  ng = zcptr[nblk + 1] - 1;
  nmod = modptr[nblk + 1] - 1;

  /*     number of discrete real states */
  nz = zptr[nblk + 1] - 1;
  /*     number of discrete object states */
  noz = ozptr[nblk + 1] - 1;
  /*     number of object parameters */
  nopar = opptr[nblk + 1] - 1;
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
          mxtb = mxtb + (outtbsz[k-1]*outtbsz[(k-1)+nlnk]);
        }
      }
      if (no > 1) {
        for (j = 1; j <= no; ++j) {
          k = outlnk[outptr[i] - 1 + j];
          mxtb = mxtb + (outtbsz[k-1]*outtbsz[(k-1)+nlnk]);
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

  if(nmod>0){
    if((mod=MALLOC(sizeof(int)*nmod))== NULL ){
      *ierr =5;
      return 0;
    }
  }
  if(ng>0){ /* g becomes global */
    if((g=MALLOC(sizeof(double)*ng))== NULL ){
      *ierr =5;
      if(nmod>0) FREE(mod);
      return 0;
    }
  }

  debug_block=-1; /* no debug block for start */
  C2F(cosdebugcounter).counter=0;

  if((Blocks=MALLOC(sizeof(scicos_block)*nblk))== NULL ){
    *ierr =5;
    if(nmod>0) FREE(mod);
    if(ng>0) FREE(g);
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
    Blocks[kf].noz=ozptr[kf+2]-ozptr[kf+1];
    Blocks[kf].nrpar=rpptr[kf+2]-rpptr[kf+1];
    Blocks[kf].nipar=ipptr[kf+2]-ipptr[kf+1];
    Blocks[kf].nopar=opptr[kf+2]-opptr[kf+1];
    Blocks[kf].nin=inpptr[kf+2]-inpptr[kf+1]; /* number of input ports */
    Blocks[kf].nout=outptr[kf+2]-outptr[kf+1];/* number of output ports */

    /* in insz, we store :
     *  - insz[0..nin-1] : first dimension of input ports
     *  - insz[nin..2*nin-1] : second dimension of input ports
     *  - insz[2*nin..3*nin-1] : type of data of input ports
     */
    Blocks[kf].insz=NULL;
    Blocks[kf].inptr=NULL;
    if (Blocks[kf].nin!=0) {
      if ((Blocks[kf].insz=MALLOC(Blocks[kf].nin*3*sizeof(int)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
      if ((Blocks[kf].inptr=MALLOC(Blocks[kf].nin*sizeof(double*)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
    }
    for(in=0;in<Blocks[kf].nin;in++) {
      lprt=inplnk[inpptr[kf+1]+in];
      Blocks[kf].inptr[in]=outtbptr[lprt-1];
      Blocks[kf].insz[in]=outtbsz[lprt-1];
      Blocks[kf].insz[Blocks[kf].nin+in]=outtbsz[(lprt-1)+nlnk];
      Blocks[kf].insz[2*Blocks[kf].nin+in]=outtbtyp[lprt-1];
    }

    /* in outsz, we store :
     *  - outsz[0..nout-1] : first dimension of output ports
     *  - outsz[nout..2*nout-1] : second dimension of output ports
     *  - outsz[2*nout..3*nout-1] : type of data of output ports
     */
    Blocks[kf].outsz=NULL;
    Blocks[kf].outptr=NULL;
    if (Blocks[kf].nout!=0) {
      if ((Blocks[kf].outsz=MALLOC(Blocks[kf].nout*3*sizeof(int)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
      if ((Blocks[kf].outptr=MALLOC(Blocks[kf].nout*sizeof(double*)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
    }
    for(out=0;out<Blocks[kf].nout;out++) {
      lprt=outlnk[outptr[kf+1]+out];
      Blocks[kf].outptr[out]=outtbptr[lprt-1];
      Blocks[kf].outsz[out]=outtbsz[lprt-1];
      Blocks[kf].outsz[Blocks[kf].nout+out]=outtbsz[(lprt-1)+nlnk];
      Blocks[kf].outsz[2*Blocks[kf].nout+out]=outtbtyp[lprt-1];
    }
    Blocks[kf].evout=NULL;
    Blocks[kf].nevout=clkptr[kf+2] - clkptr[kf+1];
    if (Blocks[kf].nevout!=0) {
      if ((Blocks[kf].evout=CALLOC(Blocks[kf].nevout,sizeof(double)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
    }

    Blocks[kf].z=&(z__[zptr[kf+1]-1]);

    Blocks[kf].ozsz=NULL;
    if (Blocks[kf].noz==0) {
     Blocks[kf].ozptr=NULL;
     Blocks[kf].oztyp=NULL;
    }
    else {
     Blocks[kf].ozptr=&(oz[ozptr[kf+1]-1]);
     if ((Blocks[kf].ozsz=MALLOC(Blocks[kf].noz*2*sizeof(int)))== NULL ) {
       FREE_blocks();
       *ierr =5;
       return 0;
     }
     for (i=0;i<Blocks[kf].noz;i++) {
      Blocks[kf].ozsz[i]=ozsz[(ozptr[kf+1]-1)+i];
      Blocks[kf].ozsz[i+Blocks[kf].noz]=ozsz[(ozptr[kf+1]-1+noz)+i];
     }
     Blocks[kf].oztyp=&(oztyp[ozptr[kf+1]-1]);
    }

    Blocks[kf].rpar=&(rpar[rpptr[kf+1]-1]);
    Blocks[kf].ipar=&(ipar[ipptr[kf+1]-1]);

    Blocks[kf].oparsz=NULL;
    if (Blocks[kf].nopar==0) {
     Blocks[kf].oparptr=NULL;
     Blocks[kf].opartyp=NULL;
    }
    else {
     Blocks[kf].oparptr=&(opar[opptr[kf+1]-1]);
     if ((Blocks[kf].oparsz=MALLOC(Blocks[kf].nopar*2*sizeof(int)))== NULL ) {
       FREE_blocks();
       *ierr =5;
       return 0;
     }
     for (i=0;i<Blocks[kf].nopar;i++) {
      Blocks[kf].oparsz[i]=oparsz[(opptr[kf+1]-1)+i];
      Blocks[kf].oparsz[i+Blocks[kf].nopar]=oparsz[(opptr[kf+1]-1+nopar)+i];
     }
     Blocks[kf].opartyp=&(opartyp[opptr[kf+1]-1]);
    }

    Blocks[kf].res=NULL;
    if (Blocks[kf].nx!=0) {
      if ((Blocks[kf].res=MALLOC(Blocks[kf].nx*sizeof(double)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
    }

    i1=izptr[kf+2]-izptr[kf+1];
    if ((Blocks[kf].label=MALLOC(sizeof(char)*(i1+1)))== NULL ){
      FREE_blocks();
      *ierr =5;
      return 0;
    }
    Blocks[kf].label[i1]='\0';
    C2F(cvstr)(&i1,&(iz[izptr[kf+1]-1]),Blocks[kf].label,&job,i1);
    Blocks[kf].jroot=NULL;
    if (Blocks[kf].ng!=0) {
     if ((Blocks[kf].jroot=CALLOC(Blocks[kf].ng,sizeof(int)))== NULL ){
        FREE_blocks();
        *ierr =5;
        return 0;
      }
    }

    Blocks[kf].work=(void **)(((double *)work)+kf);
    Blocks[kf].nmode=modptr[kf+2]-modptr[kf+1];
    if ( Blocks[kf].nmode!=0){
      Blocks[kf].mode=&(mod[modptr[kf+1]-1]);
    }
  }

  iwa=NULL;
  if ((*nevts)!=0)
  {
   if((iwa=MALLOC(sizeof(int)*(*nevts)))== NULL ){
     FREE_blocks();
     *ierr =5;
     return 0;
   }
  }

  /* save ptr of scicos in import structure */
  C2F(makescicosimport)(x,&nx,&xptr[1],&zcptr[1],z__,&nz,&zptr[1],
                        &noz,oz,ozsz,oztyp,&ozptr[1],
                        g,&ng,mod,&nmod,&modptr[1],iz,&izptr[1],
                        &inpptr[1],&inplnk[1],&outptr[1],&outlnk[1],
                        outtbptr,outtbsz,outtbtyp,
                        outtb_elem,&nelem,
                        &nlnk,rpar,&rpptr[1],ipar,&ipptr[1],
                        opar,oparsz,opartyp,&opptr[1],
                        &nblk,subscr,nsubs,
                        &tevts[1],&evtspt[1],nevts,pointi,
                        &iord[1],&niord,&oord[1],&noord,&zord[1],&nzord,
                        funptr,&funtyp[1],&ztyp[1],
                        &cord[1],&ncord,&ordclk[1],&clkptr[1],&ordptr[1],&nordptr,
                        &critev[1],iwa,Blocks,
                        t0,tf,&Atol,&rtol,&ttol,&deltat,&hmax);

  if (*flag__ == 1) { /*start*/
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
  } else if (*flag__ == 2) { /*run*/
    /*     integration */

    if (C2F(cmsolver).solver == 0) {      /*  CVODE: Method: BDF,   Nonlinear solver= NEWTON     */
      cossim(t0);
    }else if (C2F(cmsolver).solver == 1) {/*  CVODE: Method: BDF,   Nonlinear solver= FUNCTIONAL */
      cossim(t0);
    }else if (C2F(cmsolver).solver == 2) {/*  CVODE: Method: ADAMS, Nonlinear solver= NEWTON     */
      cossim(t0);
    }else if (C2F(cmsolver).solver == 3) {/*  CVODE: Method: ADAMS, Nonlinear solver= FUNCTIONAL */
      cossim(t0);
    } else if(C2F(cmsolver).solver == 100){/* IDA  : Method:       , Nonlinear solver=  */
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

  } else if (*flag__ == 3) { /*finish*/
    /*     fermeture des blocks */
    cosend(t0);
  } else if (*flag__ == 4) { /*linear*/
    phase=1;
    idoit(t0);
    if (*ierr == 0) {
      if((W=MALLOC(sizeof(double)*nx))== NULL ){
          FREE(iwa);
          FREE_blocks();
          *ierr =5;
          return 0;
      }

      /*---------à la place de old simblk--------*/
      /*  C2F(simblk)(&nx, t0, x, W);  */
      for(jj=0;jj<nx;jj++) W[jj]=0.0;
      C2F(ierode).iero = 0;   *ierr= 0;
      odoit(W, x,W,t0);
      C2F(ierode).iero = *ierr;
      /*-----------------------------------------*/
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


static int check_flag(void *flagvalue, char *funcname, int opt)
{
  int *errflag;

  /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
  if (opt == 0 && flagvalue == NULL) {
    sciprint("\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n", funcname);
    return(1); }
  /* Check if flag < 0 */
  else if (opt == 1) {
    errflag = (int *) flagvalue;
    if (*errflag < 0) {
      sciprint("\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n",
	      funcname, *errflag);
      return(1); }}
  /* Check if function returned NULL pointer - no memory allocated */
  else if (opt == 2 && flagvalue == NULL) {
    sciprint("\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",funcname);
    return(1); }

  return(0);
}


/* Subroutine */ void cosini(told)

     double *told;

{
  static int flag__;
  static int i;

  static int kfune;
  static int jj;

  SCSREAL_COP *outtbd=NULL;    /*to save double of outtb*/
  SCSINT8_COP *outtbc=NULL;    /*to save int8 of outtb*/
  SCSINT16_COP *outtbs=NULL;   /*to save int16 of outtb*/
  SCSINT32_COP *outtbl=NULL;   /*to save int32 of outtb*/
  SCSUINT8_COP *outtbuc=NULL;  /*to save unsigned int8 of outtb*/
  SCSUINT16_COP *outtbus=NULL; /*to save unsigned int16 of outtb*/
  SCSUINT32_COP *outtbul=NULL; /*to save unsigned int32 of outtb*/
  int szouttbd=0;  /*size of arrays*/
  int szouttbc=0,  szouttbs=0,  szouttbl=0;
  int szouttbuc=0, szouttbus=0, szouttbul=0;
  int curouttbd=0; /*current position in arrays*/
  int curouttbc=0,  curouttbs=0,  curouttbl=0;
  int curouttbuc=0, curouttbus=0, curouttbul=0;

  int ii,kk; /*local counters*/
  int sszz;  /*local size of element of outtb*/

  /*Allocation of arrays for outtb*/
  for (ii=0;ii<nlnk;ii++)
  {
   switch (outtbtyp[ii])
   {
    case SCSREAL_N    : szouttbd+=outtbsz[ii]*outtbsz[ii+nlnk]; /*double real matrix*/
                        outtbd=(SCSREAL_COP *) REALLOC (outtbd,szouttbd*sizeof(SCSREAL_COP));
                        break;

    case SCSCOMPLEX_N : szouttbd+=2*outtbsz[ii]*outtbsz[ii+nlnk]; /*double complex matrix*/
                        outtbd=(SCSCOMPLEX_COP *) REALLOC (outtbd,szouttbd*sizeof(SCSCOMPLEX_COP));
                        break;

    case SCSINT8_N    : szouttbc+=outtbsz[ii]*outtbsz[ii+nlnk]; /*int8*/
                        outtbc=(SCSINT8_COP *) REALLOC (outtbc,szouttbc*sizeof(SCSINT8_COP));
                        break;

    case SCSINT16_N   : szouttbs+=outtbsz[ii]*outtbsz[ii+nlnk]; /*int16*/
                        outtbs=(SCSINT16_COP *) REALLOC (outtbs,szouttbs*sizeof(SCSINT16_COP));
                        break;

    case SCSINT32_N   : szouttbl+=outtbsz[ii]*outtbsz[ii+nlnk]; /*int32*/
                        outtbl=(SCSINT32_COP *) REALLOC (outtbl,szouttbl*sizeof(SCSINT32_COP));
                        break;

    case SCSUINT8_N   : szouttbuc+=outtbsz[ii]*outtbsz[ii+nlnk]; /*uint8*/
                        outtbuc=(SCSUINT8_COP *) REALLOC (outtbuc,szouttbuc*sizeof(SCSUINT8_COP));
                        break;

    case SCSUINT16_N  : szouttbus+=outtbsz[ii]*outtbsz[ii+nlnk]; /*uint16*/
                        outtbus=(SCSUINT16_COP *) REALLOC (outtbus,szouttbus*sizeof(SCSUINT16_COP));
                        break;

    case SCSUINT32_N  : szouttbul+=outtbsz[ii]*outtbsz[ii+nlnk]; /*uint32*/
                        outtbul=(SCSUINT32_COP *) REALLOC (outtbul,szouttbul*sizeof(SCSUINT32_COP));
                        break;

    default  : /* Add a message here */
               break;
   }
  }

  /* Jacobian*/
  Jacobian_Flag=0;
  AJacobian_block=0;

  /* Jacobian*/

  /* Function Body */
  *ierr = 0;
  /*     initialization (flag 4) */
  /*     loop on blocks */

  C2F(dset)(&ng, &c_b14, g, &c__1);
  nclock = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (funtyp[C2F(curblk).kfun] >= 0) { /* debug_block is not called here */
      flag__ = 4;
      callf(told, xd, x, x,g,&flag__);
      if (flag__ < 0 && *ierr == 0) {
        *ierr = 5 - flag__;
        kfune = C2F(curblk).kfun;
      }
      if ((Jacobian_Flag==1)&&(AJacobian_block==0)) AJacobian_block=C2F(curblk).kfun;
    }
  }
  if (*ierr != 0) {
    C2F(curblk).kfun = kfune;
    freeouttbptr;
    return;
  }
  /*     initialization (flag 6) */
  nclock = 0;

  for (jj = 1; jj <= ncord; ++jj) {
    C2F(curblk).kfun = cord[jj];
    flag__ = 6;
    if (funtyp[C2F(curblk).kfun] >= 0) {
      callf(told, xd, x, x,g,&flag__);
      if (flag__ < 0) {
        *ierr = 5 - flag__;
        freeouttbptr;
        return;
      }
    }
  }
  /*     point-fix iterations */
  nclock =0;
  for (i = 1; i <= nblk + 1; ++i) { /*for each block*/
    /*     loop on blocks */
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
      flag__ = 6;
      if (funtyp[C2F(curblk).kfun] >= 0) {
        callf(told, xd, x, x,g,&flag__);
        if (flag__ < 0) {
          *ierr = 5 - flag__;
          freeouttbptr;
          return;
        }
      }
    }

    nclock = 0;

    for (jj = 1; jj <= ncord; ++jj) { /*for each continous block*/
      C2F(curblk).kfun = cord[jj];
      flag__ = 6;
      if (funtyp[C2F(curblk).kfun] >= 0) {
        callf(told, xd, x, x,g,&flag__);
        if (flag__ < 0) {
         *ierr = 5 - flag__;
         freeouttbptr;
         return;
        }
      }
    }

    /*comparison between outtb and arrays*/
    curouttbd=0;  curouttbc=0;  curouttbs=0; curouttbl=0;
    curouttbuc=0; curouttbus=0; curouttbul=0;
    for (jj=0; jj<nlnk; jj++)
    {
      switch (outtbtyp[jj]) /*for each type of ports*/
      {
       case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[jj]; /*double real matrix*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for(kk=0;kk<sszz;kk++)
                           {
                            if(outtbdptr[kk]!=(SCSREAL_COP)outtbd[curouttbd+kk]) goto L30;
                           }
                           curouttbd+=sszz;
                           break;

       case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[jj]; /*double complex matrix*/
                           sszz=2*outtbsz[jj]*outtbsz[jj+nlnk];
                           for(kk=0;kk<sszz;kk++)
                           {
                            if(outtbdptr[kk]!=(SCSCOMPLEX_COP)outtbd[curouttbd+kk]) goto L30;
                           }
                           curouttbd+=sszz;
                           break;

       case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[jj]; /*int8*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for(kk=0;kk<sszz;kk++)
                           {
                            if(outtbcptr[kk]!=(SCSINT8_COP)outtbc[curouttbc+kk]) goto L30;
                           }
                           curouttbc+=sszz;
                           break;

       case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[jj]; /*int16*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for (kk=0;kk<sszz;kk++)
                           {
                            if(outtbsptr[kk]!=(SCSINT16_COP)outtbs[curouttbs+kk]) goto L30;
                           }
                           curouttbs+=sszz;
                           break;

       case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[jj]; /*int32*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for (kk=0;kk<sszz;kk++)
                           {
                            if(outtblptr[kk]!=(SCSINT32_COP)outtbl[curouttbl+kk]) goto L30;
                           }
                           curouttbl+=sszz;
                           break;

       case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[jj]; /*uint8*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for (kk=0;kk<sszz;kk++)
                           {
                            if(outtbucptr[kk]!=(SCSUINT8_COP)outtbuc[curouttbuc+kk]) goto L30;
                           }
                           curouttbuc+=sszz;
                           break;

       case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[jj]; /*uint16*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for (kk=0;kk<sszz;kk++)
                           {
                            if(outtbusptr[kk]!=(SCSUINT16_COP)outtbus[curouttbus+kk]) goto L30;
                           }
                           curouttbus+=sszz;
                           break;

       case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[jj]; /*uint32*/
                           sszz=outtbsz[jj]*outtbsz[jj+nlnk];
                           for (kk=0;kk<sszz;kk++)
                           {
                            if(outtbulptr[kk]!=(SCSUINT32_COP)outtbul[curouttbul+kk]) goto L30;
                           }
                           curouttbul+=sszz;
                           break;

       default  : /* Add a message here */
                  break;
      }
    }
    freeouttbptr;
    return;

  L30:
       /*Save data of outtb in arrays*/
       curouttbd=0;
       curouttbc=0;  curouttbs=0;  curouttbl=0;
       curouttbuc=0; curouttbus=0; curouttbul=0;
       for (ii=0;ii<nlnk;ii++) /*for each link*/
       {
        switch (outtbtyp[ii])  /*switch to type of outtb object*/
        {
         case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[ii];  /*double real matrix*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             C2F(dcopy)(&sszz, outtbdptr, &c__1, &outtbd[curouttbd], &c__1);
                             curouttbd+=sszz;
                             break;

         case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[ii];  /*double complex matrix*/
                             sszz=2*outtbsz[ii]*outtbsz[ii+nlnk];
                             C2F(dcopy)(&sszz, outtbdptr, &c__1, &outtbd[curouttbd], &c__1);
                             curouttbd+=sszz;
                             break;

         case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[ii];    /*int8*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             for (kk=0;kk<sszz;kk++) outtbc[curouttbc+kk]=(SCSINT8_COP)outtbcptr[kk];
                             curouttbc+=sszz;
                             break;

         case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[ii];   /*int16*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             for (kk=0;kk<sszz;kk++) outtbs[curouttbs+kk]=(SCSINT16_COP)outtbsptr[kk];
                             curouttbs+=sszz;
                             break;

         case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[ii];    /*int32*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             for (kk=0;kk<sszz;kk++) outtbl[curouttbl+kk]=(SCSINT32_COP)outtblptr[kk];
                             curouttbl+=sszz;
                             break;

         case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[ii];  /*uint8*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             for (kk=0;kk<sszz;kk++) outtbuc[curouttbuc+kk]=(SCSUINT8_COP)outtbucptr[kk];
                             curouttbuc+=sszz;
                             break;

         case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[ii]; /*uint16*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             for (kk=0;kk<sszz;kk++) outtbus[curouttbus+kk]=(SCSUINT16_COP)outtbusptr[kk];
                             curouttbus+=sszz;
                             break;

         case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[ii];  /*uint32*/
                             sszz=outtbsz[ii]*outtbsz[ii+nlnk];
                             for (kk=0;kk<sszz;kk++) outtbul[curouttbul+kk]=(SCSUINT32_COP)outtbulptr[kk];
                             curouttbul+=sszz;
                             break;

         default  : /* Add a message here */
                    break;
        }
       }
  }
  *ierr = 20;
  freeouttbptr;
} /* cosini_ */

/* Subroutine */ void idoit(told)
     double *told;
{
  static int flag__;
  static int i,jj;
  static int ierr1;
  static int i2;
  static int cond;
  /*     Copyright INRIA */


  /* ..   Parameters .. */
  /*     maximum number of clock output for one block */

  /*     neq must contain after #states all int data for simblk and grblk */
  /*     X must contain after state values all real data for simblk and grblk */
  /* Parameter adjustments */

  /* Function Body */

  /*     initialisation (propagation of constant blocks outputs) */

  for (jj = 1; jj <= niord; ++jj) {
    C2F(curblk).kfun = iord[jj];
    if (funtyp[C2F(curblk).kfun] > -1){ /* if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
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

        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
          if (outtbdptr[0] <= 0.) {*/
	  switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbcptr <= 0);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbsptr <= 0);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtblptr <= 0);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbucptr <= 0);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbusptr <= 0);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbulptr <= 0);
                               break;

           default  : /* Add a message here */
                      break;
          }
          if (cond) {
	    i=2;
	  } else {
	    i=1;
	  }
        /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
          i=max(min((int)  outtbdptr[0],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);*/
	  switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbcptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbsptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtblptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbucptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbusptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbulptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           default  : /* Add a message here */
                      break;
          }
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

/* Subroutine */
void cossim(told)
     double *told;
{
  /* System generated locals */
  int i3;

  /* La vita e bella ! */
  //** used for the [stop] button
  static char CommandToUnstack[1024];
  static int CommandLength;
  static int SeqSync;
  static int zero = 0;
  static int one = 1;

  /* Local variables */
  static int flag__;
  static int ierr1;
  static int j, k;
  static double t;
  static int jj;
  static double rhotmp, tstop;
  static int inxsci;
  static int kpo, kev;
  int Discrete_Jump;
  int *jroot,*zcros;
  realtype reltol, abstol;
  N_Vector y;
  void *cvode_mem;
  int flag, flagr;
  int cnt=0;


  jroot = NULL;
  if (ng!=0) {
    if((jroot=MALLOC(sizeof(int)*ng))== NULL ){
      *ierr =10000;
      return;
    }
  }

  for ( jj = 0 ; jj < ng ; jj++ )
    jroot[jj] = 0 ;

  zcros=NULL;
  if (ng!=0) {
    if((zcros=MALLOC(sizeof(int)*ng))== NULL ){
      *ierr =10000;
      if ( ng>0 )FREE(jroot);
      return;
    }
  }

  if (*neq>0){ /* Unfortunately CVODE does not work with NEQ==0 */
    y = NULL;
    y = N_VNewEmpty_Serial(*neq);
    if (check_flag((void *)y, "N_VNewEmpty_Serial", 0)) {
      *ierr=10000;
      if ( ng>0 ) FREE(jroot);
      if ( ng>0 )FREE(zcros);
      return;
    };

    NV_DATA_S(y)=x;

    reltol = (realtype) rtol;
    abstol = (realtype) Atol;  /* Ith(abstol,1) = realtype) Atol;*/

    cvode_mem = NULL;
    switch (C2F(cmsolver).solver)
      {
      case 0:   cvode_mem = CVodeCreate(CV_BDF, CV_NEWTON);break;
      case 1:   cvode_mem = CVodeCreate(CV_BDF, CV_FUNCTIONAL);break;
      case 2:   cvode_mem = CVodeCreate(CV_ADAMS, CV_NEWTON);break;
      case 3:   cvode_mem = CVodeCreate(CV_ADAMS, CV_FUNCTIONAL);break;
      }

    /*    cvode_mem = CVodeCreate(CV_ADAMS, CV_FUNCTIONAL);*/

    if (check_flag((void *)cvode_mem, "CVodeCreate", 0)) {
      *ierr=10000;
      N_VDestroy_Serial(y); FREE(jroot); FREE(zcros);
      return;
    };

    flag = CVodeMalloc(cvode_mem, simblk, T0, y, CV_SS, reltol, &abstol);
    if (check_flag(&flag, "CVodeMalloc", 1)) {
      *ierr=300+(-flag);
      freeall
      return;
    };

    flag = CVodeRootInit(cvode_mem, ng, grblk, NULL);
    if (check_flag(&flag, "CVodeRootInit", 1)) {
      *ierr=300+(-flag);
      freeall
      return;
    };

  /* Call CVDense to specify the CVDENSE dense linear solver */
    flag = CVDense(cvode_mem, *neq);
    if (check_flag(&flag, "CVDense", 1)) {
      *ierr=300+(-flag);
      freeall
	return;
    };

    if(hmax>0){
      flag=CVodeSetMaxStep(cvode_mem, (realtype) hmax);
      if (check_flag(&flag, "CVodeSetMaxStep", 1)) {
	*ierr=300+(-flag);
	freeall;
	return;
      };
    }
  /* Set the Jacobian routine to Jac (user-supplied)
     flag = CVDenseSetJacFn(cvode_mem, Jac, NULL);
     if (check_flag(&flag, "CVDenseSetJacFn", 1)) return(1);  */

  }/* testing if neq>0 */

  /* Function Body */
  C2F(coshlt).halt = 0;
  *ierr = 0;

  C2F(xscion)(&inxsci);
  /*     initialization */
  C2F(realtimeinit)(told, &C2F(rtfactor).scale);

  phase=1;
  hot = 0;

  jj = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (Blocks[C2F(curblk).kfun-1].ng >= 1) {
      zcros[jj] = C2F(curblk).kfun;
      ++jj;
    }
  }
  /*     . Il faut:  ng >= jj */
  if (jj != ng) {
    zcros[jj] = -1;
  }
  /*     initialisation (propagation of constant blocks outputs) */
  idoit(told);
  if (*ierr != 0) {
    freeall;
    return;
  }
  /*--discrete zero crossings----dzero--------------------*/
  if (ng>0){ /* storing ZC signs just after a solver call*/
    zdoit(g, x, x, told);
    if (*ierr != 0) {freeall;return;  }
    for (jj = 0; jj < ng; ++jj)
      if(g[jj]>=0) jroot[jj]=5;else jroot[jj]=-5;
  }
  /*--discrete zero crossings----dzero--------------------*/

  //**
  //** STARDARD SCICOS SIMULATION LOOP : EXPLICIT SOLVER
  //**
  /*     main loop on time */
  while(*told < *tf) { /* while time is less than simulation final time */

  // removed in scilab 5
  /*
     if (inxsci == 1 && scilab_timer_check() == 1) {
       C2F(sxevents)();
       //     .     sxevents can modify halt
       }
  */

  //** BRUNO/SIMONE : patch for scicos halt (STOP) simulation
  while (ismenu()) //** if the user has done something, do the actions
       {
	SeqSync = GetCommand(CommandToUnstack); //** get at the action
        CommandLength = (int)strlen(CommandToUnstack);
        syncexec(CommandToUnstack, &CommandLength, &zero, &one, CommandLength); //** execute it
       }
  //** Typically the user click over the "STOP" button; the "haltscicos" callback macros is executed
  //*  then the built in "sci_haltscicos()" put set "C2F(coshlt).halt = 1", the the following if is true :)

  if (C2F(coshlt).halt != 0)  /* if the menu has ben activated OR the simulation has been forced to stop */
    {
      if (C2F(coshlt).halt==2)
         *told=*tf;   /* if the simulation has been stopped because the final time is reached  */

      C2F(coshlt).halt = 0; //** reset the flag for the next simulation

      freeall;

      return; //** EXIT from the function
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
	if (*pointi!=0){
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
	  if (rhotmp < tstop) {
	    hot = 0;
	  }
	}
	tstop = rhotmp;
	t = min(*told + deltat,min(t,*tf + ttol));

	if (ng>0 &&  hot == 0 && nmod>0) {
	  zdoit(g,x,x,told);
	  if (*ierr != 0){
	    freeall;
	    return;
	  }
	}

	if (hot==0){ /* hot==0 : cold restart*/
	  flag = CVodeSetStopTime(cvode_mem, (realtype)tstop);  /* Setting the stop time*/
	  if (check_flag(&flag, "CVodeSetStopTime", 1)) {
	    *ierr=300+(-flag);
	    freeall;
	    return;
	  };

	  flag =CVodeReInit(cvode_mem, simblk, (realtype)(*told), y, CV_SS, reltol, &abstol);
	  if (check_flag(&flag, "CVodeReInit", 1)) {
	    *ierr=300+(-flag);
	    freeall;
	    return;
	  };
	}

       if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
        {
	  sciprint("****lsodar from: %f to %f hot= %d  \r\n", *told,t,hot);
	}

	/*--discrete zero crossings----dzero--------------------*/
	/*--check for Dzeros after Mode settings or ddoit()----*/
	Discrete_Jump=0;

	if (ng>0 && hot==0){
	  zdoit(g, x, x,told);
	  if (*ierr != 0) {freeall;return;}
	  for (jj = 0; jj < ng; ++jj) {
	    if((g[jj]>=0.0)&&(jroot[jj]==-5)) {Discrete_Jump=1;jroot[jj]=1;}
	    else if((g[jj]<0.0)&&(jroot[jj]==5)) {Discrete_Jump=1;jroot[jj]=-1;}
	    else jroot[jj]=0;
	  }
	}
	/*--discrete zero crossings----dzero--------------------*/

	if (Discrete_Jump==0){/* if there was a dzero, its event should be activated*/
	  phase=2;
	  flag = CVode(cvode_mem, t, y, told, CV_NORMAL_TSTOP);
	  if (*ierr != 0) {freeall;return;}
	  phase=1;
	}else{
	  flag = CV_ROOT_RETURN; /* in order to handle discrete jumps */
	}

	/*     .     update outputs of 'c' type  blocks if we are at the end*/
	if (*told >= *tf) {
	  if (ncord > 0) {
	    cdoit(told);
	    freeall;
	    return;
	  }
	}

	if (flag>=0){
	  if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
	    sciprint("****SUNDIALS.Cvode reached: %f\r\n",*told);
	  hot = 1;
          cnt = 0;
	} else if ( flag==CV_TOO_MUCH_WORK ||  flag == CV_CONV_FAILURE || flag==CV_ERR_FAILURE) {
	  sciprint("**** SUNDIALS.Cvode: too much work at time=%g (stiff region, change RTOL and ATOL)\r\n",*told);
	  hot = 0;
          cnt++;
          if (cnt>5) {
           *ierr=300+(-flag);
            freeall;
            return;
          }
	}else{
	  if (flag<0) *ierr=300+(-flag);    /* raising errors due to internal errors, other wise erros due to flagr*/
	  freeall;
	  return;
	};

	if (flag == CV_ZERO_DETACH_RETURN){hot=0;};  /* new feature of sundials, detects zero-detaching */

	if (flag == CV_ROOT_RETURN) {
	  /*     .        at a least one root has been found */
	  hot = 0;
	  if (Discrete_Jump==0){
	    flagr = CVodeGetRootInfo(cvode_mem, jroot);
	    if (check_flag(&flagr, "CVodeGetRootInfo", 1)) {
	      *ierr=300+(-flagr);
	      freeall;
	      return;
	    };
	  }
	  /*     .        at a least one root has been found */
	  if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
          {
	    sciprint("root found at t=: %f\r\n",*told);
	  }
	  /*     .        update outputs affecting ztyp blocks ONLY FOR OLD BLOCKS */
	  zdoit(g, xd, x,told);
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
		  callf(told, xd, x, x,g,&flag__);
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
		if (Blocks[C2F(curblk).kfun-1].nx > 0) {
		  /*     .              call corresponding block to update state */
		  flag__ = 2;
		  nclock = -kev;
		  callf(told, xd, x, x,g,&flag__);

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
      if (ng>0){ /* storing ZC signs just after a sundials call*/
	zdoit(g, x, x, told); if (*ierr != 0) {freeall;return;  }
	for (jj = 0; jj < ng; ++jj)
	  if(g[jj]>=0)jroot[jj]=5;else jroot[jj]=-5;
      }
      /*--discrete zero crossings----dzero--------------------*/

      C2F(realtime)(told);
    } else {
      /*     .  t==told */
      if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
      {
	sciprint("Event: %d activated at t=%f\r\n",*pointi,*told);
	for(kev=0;kev<nblk;kev++){
	  if (Blocks[kev].nmode>0){
	    sciprint("mode of block %d=%d, ",kev,Blocks[kev].mode[0]);
	  }
	}
	sciprint("**mod**\r\n");
      }

      ddoit(told);
      if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
      {
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



/* Subroutine */
void cossimdaskr(told)
     double *told;
{
  /* Initialized data */
  static int otimer = 0;
  /* System generated locals */
  int i3;

  /* La vita e bella ! */
  //** used fot the [stop] button
  static char CommandToUnstack[1024];
  static int CommandLength;
  static int SeqSync;
  static int zero = 0;
  static int one = 1;

  /* Local variables */
  static int flag__;
  static int ierr1;
  static int j, k;
  static double t;
  static int jj, jt;
  static int  ntimer;
  static double rhotmp,tstop;
  static int inxsci;
  static int kpo, kev;

  int *jroot,*zcros;
  int maxord;
  int *Mode_save;
  int Mode_change;

  int flag, flagr;
  N_Vector   yy, yp;
  realtype reltol, abstol;
  int Discrete_Jump;
  N_Vector IDx;
  realtype *scicos_xproperty;
  void *ida_mem;
  UserData data;
  IDAMem copy_IDA_mem;
  int maxnj, maxnit;
  /*-------------------- Analytical Jacobian memory allocation ----------*/
  int  Jn, Jnx, Jno, Jni, Jactaille;
  double uround;
  int cnt=0;
  maxord = 5;


  jroot=NULL;
  if (ng!=0) {
    if((jroot=MALLOC(sizeof(int)*ng))== NULL ){
      *ierr =10000;
      return;
    }
  }
  for ( jj = 0 ; jj < ng ; jj++ )  jroot[jj] = 0 ;

  zcros=NULL;
  if (ng!=0) {
    if((zcros=MALLOC(sizeof(int)*ng))== NULL ){
      *ierr =10000;
       if (ng!=0) FREE(jroot);
      return;
    }
  }

  Mode_save=NULL;
  if (nmod!=0) {
    if((Mode_save=MALLOC(sizeof(int) *nmod))== NULL ){
      *ierr =10000;
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      return;
    }
  }

  if (*neq>0){
    yy=NULL;
    yy = N_VNewEmpty_Serial(*neq);
    if(check_flag((void *)yy, "N_VNew_Serial", 0)){
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
    };
    NV_DATA_S(yy)=x;

    yp=NULL;
    yp = N_VNewEmpty_Serial(*neq);
    if(check_flag((void *)yp, "N_VNew_Serial", 0)){
      if (*neq>0) N_VDestroy_Serial(yy);
      if (ng!=0)  FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    }
    NV_DATA_S(yp)=xd;

    reltol = (realtype) rtol;
    abstol = (realtype) Atol;  /*  Ith(abstol,1) = realtype) Atol;*/

    IDx = NULL;
    IDx = N_VNew_Serial(*neq);
    if (check_flag((void *)IDx, "N_VNew_Serial", 0)) {
      *ierr=10000;
      if (*neq>0) N_VDestroy_Serial(yp);
      if (*neq>0) N_VDestroy_Serial(yy);
      if (ng!=0)  FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    };

    /* Call IDACreate and IDAMalloc to initialize IDA memory */
    ida_mem = NULL;
    ida_mem = IDACreate();
    if(check_flag((void *)ida_mem, "IDACreate", 0)) {
      if (*neq>0) N_VDestroy_Serial(IDx);
     if (*neq>0) N_VDestroy_Serial(yp);
     if (*neq>0) N_VDestroy_Serial(yy);
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0)  FREE(Mode_save);
      return;
    }
    copy_IDA_mem= (IDAMem) ida_mem;

    flag = IDAMalloc(ida_mem, simblkdaskr, T0, yy, yp, IDA_SS, reltol, &abstol);
    if(check_flag(&flag, "IDAMalloc", 1)){
      *ierr=200+(-flag);
      if (*neq>0)IDAFree(&ida_mem);
      if (*neq>0)N_VDestroy_Serial(IDx);
     if (*neq>0) N_VDestroy_Serial(yp);
      if (*neq>0)N_VDestroy_Serial(yy);
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    }


    flag = IDARootInit(ida_mem, ng, grblkdaskr, NULL);
    if (check_flag(&flag, "IDARootInit", 1)) {
      *ierr=200+(-flag);
      if (*neq>0)IDAFree(&ida_mem);
      if (*neq>0)N_VDestroy_Serial(IDx);
      if (*neq>0)N_VDestroy_Serial(yp);
      if (*neq>0)N_VDestroy_Serial(yy);
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    };


    flag = IDADense(ida_mem, *neq);
    if (check_flag(&flag, "IDADense", 1)) {
      *ierr=200+(-flag);
      if (*neq>0)IDAFree(&ida_mem);
      if (*neq>0)N_VDestroy_Serial(IDx);
      if (*neq>0)N_VDestroy_Serial(yp);
      if (*neq>0)N_VDestroy_Serial(yy);
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    };


    data=NULL;
    if ((data = (UserData) MALLOC(sizeof(*data)))==NULL){
      *ierr=10000;
      if (*neq>0)IDAFree(&ida_mem);
      if (*neq>0)N_VDestroy_Serial(IDx);
      if (*neq>0)N_VDestroy_Serial(yp);
      if (*neq>0)N_VDestroy_Serial(yy);
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    };
    data->ida_mem = ida_mem;
    data->ewt   = NULL;
    data->iwork = NULL;
    data->rwork = NULL;
    data->gwork = NULL;

    data->ewt   = N_VNew_Serial(*neq);
    if (check_flag((void *)data->ewt, "N_VNew_Serial", 0)) {
      *ierr=200+(-flag);
      if (*neq>0)FREE(data);
      if (*neq>0)IDAFree(&ida_mem);
     if (*neq>0) N_VDestroy_Serial(IDx);
     if (*neq>0) N_VDestroy_Serial(yp);
      if (*neq>0) N_VDestroy_Serial(yy);
      if (ng!=0) FREE(jroot);
      if (ng!=0) FREE(zcros);
      if (nmod!=0) FREE(Mode_save);
      return;
    };
    if ( ng>0 ){
      if ((data->gwork = (double *) MALLOC(ng * sizeof(double)))==NULL){
	if (*neq>0) N_VDestroy_Serial(data->ewt);
	if (*neq>0)FREE(data);
	if (*neq>0)IDAFree(&ida_mem);
	if (*neq>0)N_VDestroy_Serial(IDx);
	if (*neq>0)N_VDestroy_Serial(yp);
	if (*neq>0)N_VDestroy_Serial(yy);
	if (ng!=0) FREE(jroot);
	if (ng!=0) FREE(zcros);
	if (nmod!=0) FREE(Mode_save);
	return;
      };
    }
    /*Jacobian_Flag=0; */
    if (Jacobian_Flag==1){/* set by the block with A-Jac in flag-4 using Set_Jacobian_flag(1); */
      Jn=*neq;
      Jnx=Blocks[AJacobian_block-1].nx;
      Jno=Blocks[AJacobian_block-1].nout;
      Jni=Blocks[AJacobian_block-1].nin;
      Jactaille= 2*Jn+(Jn+Jni)*(Jn+Jno)+Jnx*(Jni+2*Jn+Jno)+(Jn-Jnx)*(2*(Jn-Jnx)+Jno+Jni)+2*Jni*Jno;

      if ((data->rwork = (double *) MALLOC(Jactaille * sizeof(double)))==NULL){
	if ( ng>0 ) FREE(data->gwork);
	if (*neq>0)N_VDestroy_Serial(data->ewt);
	if (*neq>0)FREE(data);
	if (*neq>0)IDAFree(&ida_mem);
	if (*neq>0)N_VDestroy_Serial(IDx);
	if (*neq>0)N_VDestroy_Serial(yp);
	if (*neq>0)N_VDestroy_Serial(yy);
	if (ng!=0) FREE(jroot);
	if (ng!=0) FREE(zcros);
	if (nmod!=0) FREE(Mode_save);
	*ierr =10000;
	return;
      }

      flag = IDADenseSetJacFn(ida_mem, Jacobians, data);
      if(check_flag(&flag, "IDADenseSetJacFn", 1)) {
      *ierr=200+(-flag);
	freeallx
	  return;
      };
    }

    flag = IDASetRdata(ida_mem, data);
    if (check_flag(&flag, "IDASetRdata", 1)) {
      *ierr=200+(-flag);
      freeallx
	return;
    };


    if(hmax>0){
      flag=IDASetMaxStep(ida_mem, (realtype) hmax);
      if (check_flag(&flag, "IDASetMaxStep", 1)) {
	*ierr=200+(-flag);
	freeallx
	return;
      };
    }

    maxnj=100; /* setting the maximum number of Jacobian evaluation during a Newton step */
    flag=IDASetMaxNumJacsIC(ida_mem, maxnj);
    if (check_flag(&flag, "IDASetMaxNumJacsIC", 1)) {
      *ierr=200+(-flag);
      freeallx
      return;
    };

    maxnit=1; /* setting the maximum number of Newton iterations in any one attemp to solve CIC */
    flag=IDASetMaxNumItersIC(ida_mem, maxnit);
    if (check_flag(&flag, "IDASetMaxNumItersIC", 1)) {
      *ierr=200+(-flag);
      freeallx
      return;
    };


  }/* testing if neq>0 */

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
  C2F(realtimeinit)(told, &C2F(rtfactor).scale);
  /*     ATOL and RTOL are scalars */

  jj = 0;
  for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun) {
    if (Blocks[C2F(curblk).kfun-1].ng >= 1) {
      zcros[jj] = C2F(curblk).kfun;
      ++jj;
    }
  }
  /*     . Il faut:  ng >= jj */
  if (jj != ng) {
    zcros[jj] = -1;
  }
  /*     initialisation (propagation of constant blocks outputs) */
  idoit(told);
  if (*ierr != 0) {
    freeallx;
    return;
  }
  /*--discrete zero crossings----dzero--------------------*/
  if (ng>0){ /* storing ZC signs just after a solver call*/
    zdoit(g, x, x, told);
    if (*ierr != 0) {freeallx;return;  }
    for (jj = 0; jj < ng; ++jj)
      if(g[jj]>=0) jroot[jj]=5;else jroot[jj]=-5;
  }

  //**
  //** MODELICA SIMULATION LOOP : IMPLICIT SOLVER
  //**
  /* main loop on time */
  while (*told < *tf) { //** util the end of simulation time
      // removed in scilab 5
        /*
          if (inxsci == 1 && scilab_timer_check() == 1) {
              C2F(sxevents)();
              //     .     sxevents can modify halt
            }
      */

     //** BRUNO/SIMONE : patch for scicos halt (STOP) simulation
     while (ismenu()) //** if the user has done something, do the actions
          {
   	    SeqSync = GetCommand(CommandToUnstack); //** get at the action
            CommandLength = (int)strlen(CommandToUnstack);
            syncexec(CommandToUnstack, &CommandLength, &zero, &one, CommandLength); //** execute it
          }
     //** Typically the user click over the "STOP" button; the "haltscicos" callback macros is executed
     //*  then the built in "sci_haltscicos()" put set "C2F(coshlt).halt = 1", the the following if is true :)

    if (C2F(coshlt).halt != 0)
      {
        if (C2F(coshlt).halt ==2)
          *told = *tf;  /* end simulation */

        C2F(coshlt).halt = 0;
        freeallx;
        return; //** exit from the simulation
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
	  N_VConst(ONE, IDx); /* Initialize id to 1's. */
	  scicos_xproperty=NV_DATA_S(IDx);
	  reinitdoit(told,scicos_xproperty);
	  if(*ierr >0){
	    freeallx;
	    return;
	  }
	}
	rhotmp = *tf + ttol;
	if (*pointi!=0){
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
	  if (rhotmp < tstop) {
	    hot = 0;/* Do cold-restat the solver:if the new TSTOP isn't beyong the previous one*/
	  }
	}
	tstop = rhotmp;
	t = min(*told + deltat,min(t,*tf + ttol));

	if (hot == 0){ /* CIC calculation when hot==0 */

	  /* Setting the stop time*/
	  flag = IDASetStopTime(ida_mem, (realtype)tstop);
	  if (check_flag(&flag, "IDASetStopTime", 1)) {
	    *ierr=200+(-flag);
	    freeallx;
	    return;
	  };

	  if (ng>0&&nmod>0){
	    phase=1;
	    zdoit(g, xd, x,told);
	    if (*ierr != 0) {
	      freeallx;
	      return;
	    }
	  }


	  for(j=0;j<=(nmod+*neq);j++){/* counter to reevaluate the
				  modes in  mode->CIC->mode->CIC-> loop
				  do it once in the absence of mode (nmod=0)*/
	    /* updating the modes through Flag==9, Phase==1 */
	    N_VConst(ONE, IDx); /* Initialize id to 1's. */
	    scicos_xproperty=NV_DATA_S(IDx);
	    reinitdoit(told,scicos_xproperty);/* filling up the scicos_xproperties */
 	    if(*ierr >0){
	      freeallx;
	      return;
	    }

	    flag=IDASetId(ida_mem,IDx);
	    if (check_flag(&flag, "IDASetId", 1)) {
	      *ierr=200+(-flag);
	      freeallx
	      return;
	    }

	    /* yy->PH */
	    flag =IDAReInit(ida_mem, simblkdaskr, (realtype)(*told), yy, yp, IDA_SS, reltol, &abstol);
	    if (check_flag(&flag, "CVodeReInit", 1)) {
	      *ierr=200+(-flag);
	      freeallx;
	      return;
	    };

	    phase=2; /* IDACalcIC: PHI-> yy0: if (ok) yy0_cic-> PHI*/
	    copy_IDA_mem->ida_kk=1;

	    flagr=IDACalcIC(ida_mem, IDA_YA_YDP_INIT, (realtype)(t));
	    phase=1;
	    flag = IDAGetConsistentIC(ida_mem, yy, yp); /* PHI->YY */

	    if (*ierr > 5) {  /* *ierr>5 => singularity in block */
	      freeallx;
	      return;
	    }

	    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
            {
	      if (flagr>=0) {
		sciprint("**** SUNDIALS.IDA succesfully initialized *****/r/n" );
	      }
	      else{
		sciprint("**** SUNDIALS.IDA failed to initialize ->try again *****/r/n" );
	      }
	    }
	    /*-------------------------------------*/
	    /* saving the previous modes*/
	    for (jj = 0; jj < nmod; ++jj) {
	      Mode_save[jj] = mod[jj];
	    }
	    if (ng>0&&nmod>0){
	      phase=1;
	      zdoit(g, xd, x,told);
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
	    if ( Mode_change==0){
	      break;
	      /* I saw some examples,  even with IDACalcIC failure,
		 IDASolve can continue integrating; So I decided to
		 ignore failure in IDACalcIC and try one more tie
		 with IDASolve. /Masoud

	      if (flagr>=0) break;
	      else{
		*ierr=200+(-flagr);
		freeallx;
		return;
	      }*/

	    }
	  }/* mode-CIC  counter*/
	  if(Mode_change==1){
	    /* In tghis case, we try again by relaxing all modes and calling IDA_calc again
	       /Masoud */
	    phase=1;
	    copy_IDA_mem->ida_kk=1;
	    flagr=IDACalcIC(ida_mem, IDA_YA_YDP_INIT, (realtype)(t));
	    phase=1;
	    flag = IDAGetConsistentIC(ida_mem, yy, yp); /* PHI->YY */
	    if ((flagr<0)||(*ierr>5)) {  /* *ierr>5 => singularity in block */
	      *ierr = 23;
	      freeallx;
	      return;
	    }
	  }
	} /* CIC calculation when hot==0 */

	if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
        {
	  sciprint("****daskr from: %f to %f hot= %d  \r\n", *told,t,hot);
	}

	/*--discrete zero crossings----dzero--------------------*/
	/*--check for Dzeros after Mode settings or ddoit()----*/
	Discrete_Jump=0;
	if (ng>0 && hot==0){
	  zdoit(g, xd, x,told);
	  if (*ierr != 0) {freeallx;return;  }
	  for (jj = 0; jj < ng; ++jj) {
	    if((g[jj]>=0.0)&&( jroot[jj]==-5)) {Discrete_Jump=1;jroot[jj]=1;}
	    else if((g[jj]<0.0)&&( jroot[jj]==5)) {Discrete_Jump=1;jroot[jj]=-1;}
	    else jroot[jj]=0;
	  }
	}

	/*--discrete zero crossings----dzero--------------------*/
	if (Discrete_Jump==0){/* if there was a dzero, its event should be activated*/
	  phase=2;
	  flagr = IDASolve(ida_mem, t, told, yy, yp, IDA_NORMAL_TSTOP);
	  phase=1;
	  if (*ierr != 0) {freeallx;return;}
	}else{
	  flagr = IDA_ROOT_RETURN; /* in order to handle discrete jumps */
	}
	if (flagr>=0){
	  if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
	    sciprint("****SUNDIALS.Ida reached: %f\r\n",*told);
	  hot = 1;
          cnt = 0;
	} else if ( flagr==IDA_TOO_MUCH_WORK ||  flagr == IDA_CONV_FAIL || flagr==IDA_ERR_FAIL) {
	  sciprint("**** SUNDIALS.Ida: too much work at time=%g (stiff region, change RTOL and ATOL)\r\n",*told);
	  hot = 0;
          cnt++;
          if (cnt>5) {
            *ierr=200+(-flagr);
	    freeallx;
	    return;
          }
	}else{
	  if (flagr<0) *ierr=200+(-flagr);    /* raising errors due to internal errors, other wise erros due to flagr*/
	  freeallx;
	  return;
	};

	/*     update outputs of 'c' type  blocks if we are at the end*/
	if (*told >= *tf) {
	  cdoit(told);
	  freeallx;
	  return;
	}

	if (flagr == IDA_ZERO_DETACH_RETURN){hot=0;}; /* new feature of sundials, detects unmasking */
	if (flagr == IDA_ROOT_RETURN) {
	  /*     .        at a least one root has been found */
	  hot = 0;
	  if (Discrete_Jump==0){
	    flagr = IDAGetRootInfo(ida_mem, jroot);
	    if (check_flag(&flagr, "IDAGetRootInfo", 1)) {
 	      *ierr=200+(-flagr);
	      freeallx;
	      return;
	    };
	  }

	  if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
          {
	    sciprint("root found at t=: %f\r\n",*told);
	  }
	  /*     .        update outputs affecting ztyp blocks  ONLY FOR OLD BLOCKS*/
	  zdoit(g, xd, x,told);
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
		  callf(told, xd, x, x,g,&flag__);
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
		if ((Blocks[C2F(curblk).kfun-1].nx > 0) || (*Blocks[C2F(curblk).kfun-1].work != NULL) ) {
		  /*     .call corresponding block to update state */
		  flag__ = 2;
		  nclock = -kev;
		  pointer_xproperty=
		    &scicos_xproperty[-1+xptr[C2F(curblk).kfun]];
		  n_pointer_xproperty=Blocks[C2F(curblk).kfun-1].nx;
		  callf(told, xd, x, x,g,&flag__);
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
	// removed
	/*
	if (inxsci == 1 && scilab_timer_check() == 1) {
	  C2F(sxevents)();
	  otimer = ntimer;
	  //     .     sxevents can modify halt
	}
	*/
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
	  zdoit(g, xd, x, told); if (*ierr != 0) {freeallx;return;  }
	  for (jj = 0; jj < ng; ++jj)
	    if(g[jj]>=0)jroot[jj]=5;else jroot[jj]=-5;
	}
	/*--discrete zero crossings----dzero--------------------*/
      }
      C2F(realtime)(told);
    } else {
      /*     .  t==told */
      if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
      {
	sciprint("Event: %d activated at t=%f\r\n",*pointi,*told);
      }

      ddoit(told);
      if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
      {
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
  static int flag__;

  static int kfune;

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
  int i,i2;

  /* Local variables */
  static int flag__, nord;

  static int ierr1;
  int ii, kever;
  int cond;

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
    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
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

        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
          if (outtbdptr[0] <= 0.) {
	    i= 2;
	  } else {
	    i= 1;
	  }*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbcptr <= 0);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbsptr <= 0);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtblptr <= 0);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbucptr <= 0);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbusptr <= 0);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbulptr <= 0);
                               break;

           default  : /* Add a message here */
                      break;
          }
          if (cond) {
            i=2;
          } else {
            i=1;
          }

        /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
          i=max(min((int)  outtbdptr[0],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbcptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbsptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtblptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbucptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbusptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbulptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           default  : /* Add a message here */
                      break;
          }
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
  int i2;

  /* Local variables */
  static int flag__;
  static int ierr1;
  static int i,jj;
  static int cond;

  /* Function Body */
  for (jj = 1; jj <= ncord; ++jj) {
    C2F(curblk).kfun = cord[jj];
    nclock = cord[jj + ncord];
    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
      flag__ = 1;
      callf(told, xd, x, x,x,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
          if (outtbdptr[0] <= 0.) {
	    i = 2;
	  } else {
	    i = 1;
	  }*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbcptr <= 0);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbsptr <= 0);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtblptr <= 0);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbucptr <= 0);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbusptr <= 0);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbulptr <= 0);
                               break;

           default  : /* Add a message here */
                      break;
          }
          if (cond) {
            i=2;
          } else {
            i=1;
          }
        /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	  i= max(min((int)  outtbdptr[0],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbcptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbsptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtblptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbucptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbusptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbulptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           default  : /* Add a message here */
                      break;
          }
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
  int i2,j;

  /* Local variables */
  static int flag__, kiwa;

  static int i,i3,ierr1;
  static int  ii, keve;
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
	if (Blocks[C2F(curblk).kfun-1].nx+Blocks[C2F(curblk).kfun-1].nz+Blocks[C2F(curblk).kfun-1].noz > 0||
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
     int *kiwa;
{
  /* System generated locals */
  int i2;

  /* Local variables */
  static int flag__;
  static int nord;

  static int ierr1, i;
  int kever, ii;
  int cond;

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

    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
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

        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	  if (outtbdptr[0] <= 0.) {
	    i = 2;
	  } else {
	    i = 1;
	  }*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbdptr <= 0.);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbcptr <= 0);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbsptr <= 0);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtblptr <= 0);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbucptr <= 0);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbusptr <= 0);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               cond = (*outtbulptr <= 0);
                               break;

           default  : /* Add a message here */
                      break;
          }
          if (cond) {
            i=2;
          } else {
            i=1;
          }
        /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	  i= max(min((int)  outtbdptr[0],
		    Blocks[C2F(curblk).kfun - 1].nevout),1);*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbdptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbcptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbsptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtblptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbucptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbusptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               i=max(min((int)  *outtbulptr,
                                         Blocks[C2F(curblk).kfun - 1].nevout),1);
                               break;

           default  : /* Add a message here */
                      break;
          }
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
  int i2;

  /* Local variables */
  static int flag__, keve, kiwa;
  static int ierr1, i;
  static int ii, jj;
  static int cond;

  /* Function Body */
  kiwa = 0;

  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (funtyp[C2F(curblk).kfun] > -1){ /* if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) {*/
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
          /* if-then-else blk */
	  if (funtyp[C2F(curblk).kfun] == -1) {
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	    if (outtbdptr[0] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbcptr <= 0);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbsptr <= 0);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtblptr <= 0);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbucptr <= 0);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbusptr <= 0);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbulptr <= 0);
                                 break;

             default  : /* Add a message here */
                        break;
            }
            if (cond) {
              i=2;
            } else {
              i=1;
            }
          /* eselect blk */
	  } else if (funtyp[C2F(curblk).kfun] == -2) {
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            i=max(min((int)  outtbdptr[0],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);*/

            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbcptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbsptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtblptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbucptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbusptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbulptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             default  : /* Add a message here */
                        break;
            }
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
     realtype *scicos_xproperty;
{
  /* System generated locals */
  int i2;

  /* Local variables */
  static int flag__, keve, kiwa;

  static int ierr1, i;
  static int ii, jj;
  static int cond;

  /* Function Body */
  kiwa = 0;
  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
      flag__ = 1;
      callf(told, xd, x, x,x,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0 && funtyp[C2F(curblk).kfun] < 0) {
      /* if-then-else blk */
      if (funtyp[C2F(curblk).kfun] == -1) {
/*        outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	if (outtbdptr[0] <= 0.) {
	  i=2;
	} else {
	  i=1;
	}*/

        switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
        {
         case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbdptr <= 0.);
                             break;

         case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbdptr <= 0.);
                             break;

         case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbcptr <= 0);
                             break;

         case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbsptr <= 0);
                             break;

         case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtblptr <= 0);
                             break;

         case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbucptr <= 0);
                             break;

         case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbusptr <= 0);
                             break;

         case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             cond = (*outtbulptr <= 0);
                             break;

         default  : /* Add a message here */
                    break;
        }
        if (cond) {
          i=2;
        } else {
          i=1;
        }

      /* eselect blk */
      } else if (funtyp[C2F(curblk).kfun] == -2) {
/*        outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	i= max(min((int)  outtbdptr[0],
		  Blocks[C2F(curblk).kfun - 1].nevout),1);*/
        switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
        {
         case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbdptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbdptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbcptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbsptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtblptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbucptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbusptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                             i=max(min((int)  *outtbulptr,
                                       Blocks[C2F(curblk).kfun - 1].nevout),1);
                             break;

         default  : /* Add a message here */
                    break;
        }
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
     int *kiwa;
{
  /* System generated locals */
  int i2;

  /* Local variables */
  static int flag__, nord;

  static int ierr1, i;
  int ii, kever;
  int cond;

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
    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
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

        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (phase==1 || Blocks[C2F(curblk).kfun - 1].nmode==0){
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            if (outtbdptr[0] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    } */
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbcptr <= 0);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbsptr <= 0);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtblptr <= 0);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbucptr <= 0);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbusptr <= 0);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbulptr <= 0);
                                 break;

             default  : /* Add a message here */
                        break;
              }
              if (cond) {
                i=2;
              } else {
                i=1;
              }
	    }else{
	      i=Blocks[C2F(curblk).kfun - 1].mode[0];
	    }
        /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  if (phase==1 || Blocks[C2F(curblk).kfun - 1].nmode==0){
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	    i= max(min((int) 
                       outtbdptr[0],
		       Blocks[C2F(curblk).kfun - 1].nevout),1);*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbcptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbsptr,
                                            Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtblptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbucptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbusptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbulptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             default  : /* Add a message here */
                        break;
            }
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
  int i2;

  /* Local variables */
  static int flag__, keve, kiwa;
  static int ierr1, i,j;
  static int ii, jj;
  static int cond;

  /* Function Body */
  C2F(dset)(&ng, &c_b14,g , &c__1);

  kiwa = 0;
  for (jj = 1; jj <= nzord; ++jj) {
    C2F(curblk).kfun = zord[jj];
    nclock = zord[jj + nzord];
    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
      flag__ = 1;
      callf(told, xtd, xt, xt,xt,&flag__);

      if (flag__ < 0) {
	*ierr = 5 - flag__;
	return;
      }
    }

    if (Blocks[C2F(curblk).kfun - 1].nevout > 0) {
      if (funtyp[C2F(curblk).kfun] < 0) {

        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
	  if (phase==1|| Blocks[C2F(curblk).kfun - 1].nmode==0){
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            if (outtbdptr[0] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbcptr <= 0);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbsptr <= 0);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtblptr <= 0);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbucptr <= 0);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbusptr <= 0);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbulptr <= 0);
                                 break;

             default  : /* Add a message here */
                        break;
            }
            if (cond) {
              i=2;
            } else {
              i=1;
            }

	  }else{
	    i=Blocks[C2F(curblk).kfun - 1].mode[0];
	  }
        /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  if (phase==1|| Blocks[C2F(curblk).kfun - 1].nmode==0){
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	    i=max(min((int) 
                      outtbdptr[0],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbcptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbsptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtblptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbucptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbusptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbulptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             default  : /* Add a message here */
                        break;
            }
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
        /* if-then-else blk */
	if (funtyp[C2F(curblk).kfun] == -1) {
/*          outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
          g[zcptr[C2F(curblk).kfun]-1]=outtbdptr[0];*/
          switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
          {
           case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=*outtbdptr;
                               break;

           case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=*outtbdptr;
                               break;

           case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbcptr;
                               break;

           case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbsptr;
                               break;

           case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=(double) *outtblptr;
                               break;

           case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbucptr;
                               break;

           case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbusptr;
                               break;

           case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                               g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbulptr;
                               break;

           default  : /* Add a message here */
                      break;
          }
	  if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
	    if (g[zcptr[C2F(curblk).kfun]-1] <= 0.) {
	      Blocks[C2F(curblk).kfun - 1].mode[0] = 2;
	    }
	    else {
	      Blocks[C2F(curblk).kfun - 1].mode[0] = 1;
	    }
	  }
          /* eselect blk */
	} else if (funtyp[C2F(curblk).kfun] == -2) {
	  for (jj=0;jj<Blocks[C2F(curblk).kfun - 1].nevout-1;++jj) {
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
	    g[zcptr[C2F(curblk).kfun]-1+jj]=outtbdptr[0]-(double)(jj+2);*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=*outtbdptr-(double)(jj+2);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=*outtbdptr-(double)(jj+2);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbcptr-(double)(jj+2);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbsptr-(double)(jj+2);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtblptr-(double)(jj+2);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbucptr-(double)(jj+2);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbusptr-(double)(jj+2);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbulptr-(double)(jj+2);
                                 break;

             default  : /* Add a message here */
                        break;
            }
	  }
	  if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            j=max(min((int)  outtbdptr[0],Blocks[C2F(curblk).kfun - 1].nevout),1);*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbdptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbdptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbcptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbsptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtblptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbucptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbusptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 j=max(min((int)  *outtbulptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             default  : /* Add a message here */
                        break;
            }
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
          /* if-then-else blk */
	  if (funtyp[C2F(curblk).kfun] == -1) {
/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            g[zcptr[C2F(curblk).kfun]-1]=outtbdptr[0];*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=*outtbdptr;
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=*outtbdptr;
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbcptr;
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbsptr;
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=(double) *outtblptr;
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbucptr;
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbusptr;
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 g[zcptr[C2F(curblk).kfun]-1]=(double) *outtbulptr;
                                 break;

             default  : /* Add a message here */
                        break;
            }
	    if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
	      if (g[zcptr[C2F(curblk).kfun]-1] <= 0.) {
		Blocks[C2F(curblk).kfun - 1].mode[0] = 2;
	      } else {
		Blocks[C2F(curblk).kfun - 1].mode[0] = 1;
	      }
	    }
          /* eselect blk */
	  } else if (funtyp[C2F(curblk).kfun] == -2) {
/*	    for (jj=0;jj<Blocks[C2F(curblk).kfun - 1].nevout-1;++jj) {
              outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
              g[zcptr[C2F(curblk).kfun]-1+jj]=outtbdptr[0]-(double)(jj+2);
	    }
	    if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0){
              outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
              j=max(min((int)  outtbdptr[0],Blocks[C2F(curblk).kfun - 1].nevout),1);*/
            for (jj=0;jj<Blocks[C2F(curblk).kfun - 1].nevout-1;++jj) {
             switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
             {
              case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=*outtbdptr-(double)(jj+2);
                                  break;

              case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=*outtbdptr-(double)(jj+2);
                                  break;

              case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbcptr-(double)(jj+2);
                                  break;

              case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbsptr-(double)(jj+2);
                                  break;

              case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtblptr-(double)(jj+2);
                                  break;

              case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbucptr-(double)(jj+2);
                                  break;

              case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbusptr-(double)(jj+2);
                                  break;

              case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  g[zcptr[C2F(curblk).kfun]-1+jj]=(double) *outtbulptr-(double)(jj+2);
                                  break;

              default  : /* Add a message here */
                         break;
             }
            }
            if(phase==1&&Blocks[C2F(curblk).kfun - 1].nmode>0) {
             switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
             {
              case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbdptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbdptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbcptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbsptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtblptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbucptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbusptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                  j=max(min((int)  *outtbulptr,Blocks[C2F(curblk).kfun - 1].nevout),1);
                                  break;

              default  : /* Add a message here */
                         break;
             }
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
     int *flag;
     double *t,*xtd,*xt,*residual,*g;

{
  voidf loc ;
  double* args[SZ_SIZE];
  int sz[SZ_SIZE];
  double intabl[TB_SIZE],outabl[TB_SIZE];
  int ii,kf,in,out,ki,ko,no,k,j;
  int ni = 0 ;
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

  if ( cosd > 1)
  {
    if (cosd != 3)
    {
     sciprint("block %d is called ",kf);
     sciprint("with flag %d ",*flag);
     sciprint("at time %f \r\n",*t);
    }
    if(debug_block>-1)
    {
      if (cosd != 3) sciprint("Entering the block \r\n");
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
	if (cosd != 3) sciprint("Leaving block %d \r\n",kf);
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
    ni=0;
    /* catenate inputs if necessary */

    if (Blocks[kf-1].nin>1) {
      ki=0;
      for (in=0;in<Blocks[kf-1].nin;in++) {
	lprt=inplnk[inpptr[kf]+in];
        szi=outtbsz[lprt-1]*outtbsz[(lprt-1)+nlnk];
        outtbdptr=(double *)outtbptr[lprt-1];
	for (ii=0;ii<szi;ii++) intabl[ki++]=outtbdptr[ii];
	ni=ni+szi;
      }
      args[0]=&(intabl[0]);
    }
    else {
      if (Blocks[kf-1].nin==0) {
	ni=0;
        /*outtbdptr=(double *)outtbptr[0];
        args[0]=&(outtbdptr[0]);*/
        args[0]=NULL;
      }
      else {
	lprt=inplnk[inpptr[kf]];
        outtbdptr=(double *)outtbptr[lprt-1];
        args[0]=&(outtbdptr[0]);
        ni=outtbsz[lprt-1]*outtbsz[(lprt-1)+nlnk];
      }
    }
    in=Blocks[kf-1].nin;

    /* catenate outputs if necessary */
	no=0;
    if (Blocks[kf-1].nout>1) {
      ko=0;
      for (out=0;out<Blocks[kf-1].nout;out++) {
	lprt=outlnk[outptr[kf]+out];
        szi=outtbsz[lprt-1]*outtbsz[(lprt-1)+nlnk];
        outtbdptr=(double *)outtbptr[lprt-1];
	for (ii=0;ii<szi;ii++) outabl[ko++]=outtbdptr[ii];
	no=no+szi;
      }
      args[1]=&(outabl[0]);
    }
    else {
      if (Blocks[kf-1].nout==0) {
	/*no=0;
        outtbdptr=(double *)outtbptr[0];
        args[1]=&(outtbdptr[0]);*/
        args[1]=NULL;
      }
      else {
	lprt=outlnk[outptr[kf]];
        outtbdptr=(double *)outtbptr[lprt-1];
        args[1]=&(outtbdptr[0]);
        no=outtbsz[lprt-1]*outtbsz[(lprt-1)+nlnk];
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
        szi=outtbsz[lprt-1]*outtbsz[(lprt-1)+nlnk];
        outtbdptr=(double *)outtbptr[lprt-1];
	for (ii=0;ii<szi;ii++) outtbdptr[ii]=outabl[ko++];
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
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double **)Blocks[kf-1].inptr,
		Blocks[kf-1].insz,&Blocks[kf-1].nin,
		(double **)Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
      }
      else{
	loc2z = (ScicosF2z) loc;
	(*loc2z)(flag,&nclock,t,Blocks[kf-1].res,Blocks[kf-1].x,&Blocks[kf-1].nx,
		 Blocks[kf-1].z,&Blocks[kf-1].nz,
		 Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		 Blocks[kf-1].ipar,&Blocks[kf-1].nipar,
		 (double **)Blocks[kf-1].inptr,Blocks[kf-1].insz,&Blocks[kf-1].nin,
		 (double **)Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout,
		 Blocks[kf-1].g,&Blocks[kf-1].ng);
      }
    }
    else {
      if (Blocks[kf-1].ztyp==0){
	loc2 = (ScicosF2) loc;
	(*loc2)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		Blocks[kf-1].z,&Blocks[kf-1].nz,
		Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double **)Blocks[kf-1].inptr,
		Blocks[kf-1].insz,&Blocks[kf-1].nin,
		(double **)Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
      }
      else{
	loc2z = (ScicosF2z) loc;
	(*loc2z)(flag,&nclock,t,Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		 Blocks[kf-1].z,&Blocks[kf-1].nz,
		 Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		 Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double **)Blocks[kf-1].inptr,
		 Blocks[kf-1].insz,&Blocks[kf-1].nin,
		 (double **)Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout,
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
	       Blocks[kf-1].ipar,&Blocks[kf-1].nipar,(double **)Blocks[kf-1].inptr,
	       Blocks[kf-1].insz,&Blocks[kf-1].nin,
	       (double **)Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout);
    }
    else {
      loci2z = (ScicosFi2z) loc;

      (*loci2z)(flag,&nclock,t,Blocks[kf-1].res,
		Blocks[kf-1].xd,Blocks[kf-1].x,&Blocks[kf-1].nx,
		Blocks[kf-1].z,&Blocks[kf-1].nz,
		Blocks[kf-1].evout,&Blocks[kf-1].nevout,Blocks[kf-1].rpar,&Blocks[kf-1].nrpar,
		Blocks[kf-1].ipar,&Blocks[kf-1].nipar,
		(double **)Blocks[kf-1].inptr,Blocks[kf-1].insz,&Blocks[kf-1].nin,
		(double **)Blocks[kf-1].outptr,Blocks[kf-1].outsz,&Blocks[kf-1].nout,
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
      if (cosd != 3) sciprint("Leaving block %d \r\n",kf);
      call_debug_scicos(t,xtd,xt,residual,g,flag,kf,flagi,debug_block);
    }
  }
}


void call_debug_scicos(t,xtd,xt,residual,g,flag,kf,flagi,deb_blk)
     int *flag,kf,flagi,deb_blk;
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
    /* special case for type 10004 */
    if(Blocks[kf-1].type==10004) {
      Blocks[kf-1].xd=&xtd[xptr[kf]-1];
      Blocks[kf-1].res=&residual[xptr[kf]-1];
      (*loc4)(&Blocks[kf-1],*flag);
    }
    else {
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
  }
  if (*flag<0) sciprint("Error in the Debug block \r\n");
}





int C2F(funnum)(fname)
     char * fname;
{
  int i=0,ln;
  int loc=-1;
  while ( tabsim[i].name != (char *) NULL) {
    if ( strcmp(fname,tabsim[i].name) == 0 ) return(i+1);
    i++;
  }
  ln=(int)strlen(fname);
  C2F(iislink)(fname,&loc);C2F(iislink)(fname,&loc);
  if (loc >= 0) return(ntabsim+(int)loc+1);
  return(0);
}



int simblk(realtype t,N_Vector yy,N_Vector yp, void *f_data)
{
  double tx, *x, *xd;
  int i;

  tx= (double) t;
  x=  (double *) NV_DATA_S(yy);
  xd= (double *) NV_DATA_S(yp);

  for(i=0;i<*neq;i++)   xd[i]=0;   /* à la place de "C2F(dset)(neq, &c_b14,xcdot , &c__1);"*/
  C2F(ierode).iero = 0;
  *ierr= 0;
  odoit(xd, x, xd, &tx);
  C2F(ierode).iero = *ierr;
  return 0;
}

int grblk(realtype t, N_Vector yy, realtype *gout, void *g_data)
{
  double tx, *x;
  tx= (double) t;
  x=  (double *) NV_DATA_S(yy);

  C2F(ierode).iero = 0;
  *ierr= 0;
  zdoit((double*) gout, x, x, &tx);
  C2F(ierode).iero = *ierr;

 return 0;
}

int simblkdaskr(realtype tres, N_Vector yy, N_Vector yp, N_Vector resval, void *rdata)

{
  double tx;
  double *xc, *xcdot, *residual;
  realtype alpha;

  UserData data;

  realtype hh;
  int qlast;
  int jj,flag;

  data = (UserData) rdata;

  if(get_phase_simulation()==1) {
    /* Just to update mode in a very special case, i.e., when initialization using modes fails.
       in this case, we relax all modes and try again one more time.
    */
    zdoit((double *)data->gwork, NV_DATA_S(yp), NV_DATA_S(yy),&tx);
  }

  hh=ZERO;
  flag=IDAGetCurrentStep(data->ida_mem, &hh);
  if (flag<0) {  *ierr=200+(-flag); return (*ierr);};

  qlast=0;
  flag=IDAGetCurrentOrder(data->ida_mem, &qlast);
  if (flag<0) {  *ierr=200+(-flag); return (*ierr);};

  alpha=ZERO;
  for (jj=0;jj<qlast;jj++)
    alpha=alpha -ONE/(jj+1);
  if (hh!=0) CJJ=-alpha/hh; else {*ierr= 1;return (*ierr);}
  xc=(double *)  NV_DATA_S(yy);
  xcdot= (double *) NV_DATA_S(yp);
  residual=(double *) NV_DATA_S(resval);
  tx=(double) tres;

  C2F(dcopy)(neq, xcdot, &c__1, residual, &c__1);
  *ierr= 0;
  C2F(ierode).iero = 0;
  odoit(residual, xc, xcdot,&tx);

  C2F(ierode).iero = *ierr;

  return (*ierr); /* ierr>0 recoverable error; ierr>0 unrecoverable error; ierr=0: ok*/
}


int grblkdaskr(realtype t, N_Vector yy, N_Vector yp, realtype *gout, void *g_data)
{
  double tx;
  tx=(double) t;

  *ierr= 0;
  C2F(ierode).iero = 0;
  zdoit((double *)gout, NV_DATA_S(yp), NV_DATA_S(yy),&tx);
  C2F(ierode).iero = *ierr;
  return (*ierr);
}


/* Subroutine */ void addevs(t, evtnb, ierr1)
     double t;
     int *evtnb, *ierr1;
{
  static int i, j;

  /* Function Body */
  *ierr1 = 0;
  if (evtspt[*evtnb] != -1) {
    if (evtspt[*evtnb] == 0) {
      tevts[*evtnb] = t;
    }else{
      if (*pointi != 0) {
	/* find parent of evtnb    */
	if (*pointi == *evtnb) {
	  *pointi =evtspt[*evtnb]; /* remove from chain */
	}else{
	  i= *pointi;
	  while (*evtnb != evtspt[i]){
	    i=evtspt[i];
	  }
	  evtspt[i]=evtspt[*evtnb]; /* remove old evtnb from chain */
	}
      }
      evtspt[*evtnb] = 0;
      tevts[*evtnb] = t;
    }
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
     int *evtnb, *ierr1;
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



void FREE_blocks(void)

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
    if (Blocks[kf].oparsz!=NULL){
      FREE(Blocks[kf].oparsz);
    }else {
      break;
    }
    if (Blocks[kf].ozsz!=NULL){
      FREE(Blocks[kf].ozsz);
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

  if(nmod>0) FREE(mod);

  if(ng>0) FREE(g);

  return;
}



int setmode(W,x,told,jroot,ttol)
     /* work space W needs to be ng+*neq*2 */
     double *W,*x,*told,ttol;
     int *jroot;
{
  int k,j,jj,diff,ii;
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
      /*---------à la place de old simblk--------*/
      /*   C2F(simblk)(neq, &ttmp, W, &W[*neq]);  */
      for(ii=0;ii<*neq;++ii) W[*neq+ii]=0;
      C2F(ierode).iero = 0; *ierr= 0;
      odoit(&W[*neq], W,&W[*neq],&ttmp);
      C2F(ierode).iero = *ierr;
      /*--------------------------------------*/
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


int get_phase_simulation(void)
{
  return phase;
}


void do_cold_restart(void)
{
  hot=0;
  return;
}

double get_scicos_time(void)
{
  return scicos_time;
}

int get_block_number(void)
{
  return C2F(curblk).kfun;
}

void set_block_error(int err)
{
  *block_error=err;
  return;
}

void end_scicos_sim(void)
{
  C2F(coshlt).halt = 2;
   return;
}

void set_pointer_xproperty(int* pointer)
{
  int i;
  for (i=0;i<n_pointer_xproperty;i++){
    /* pointer_xproperty[i]=pointer[i]; */
    if (pointer[i]==1)  pointer_xproperty[i]=ONE;
    if (pointer[i]==-1) pointer_xproperty[i]=ZERO;
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
  int i2;

  /* Local variables */
  static int flag__, keve, kiwa;
  static int ierr1, i;
  static int ii, jj;
  static int cond;

  /* Function Body */
  kiwa = 0;
  for (jj = 1; jj <= noord; ++jj) {
    C2F(curblk).kfun = oord[jj];
    nclock = oord[jj + noord];
    if (funtyp[C2F(curblk).kfun] > -1){ /*    if (outptr[C2F(curblk).kfun + 1] - outptr[C2F(curblk).kfun] > 0) { */
      flag__ = 1;

      if ((*job==2)&&(oord[jj]==AJacobian_block)) {/* applying desired output */
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
          /* if-then-else blk */
	  if (funtyp[C2F(curblk).kfun] == -1) {
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbdptr <= 0.);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbcptr <= 0);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbsptr <= 0);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtblptr <= 0);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbucptr <= 0);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbusptr <= 0);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 cond = (*outtbulptr <= 0);
                                 break;

             default  : /* Add a message here */
                        break;
           }
           if (cond) {
             i=2;
           } else {
             i=1;
           }

/*            outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            if (outtbdptr[0] <= 0.) {
	      i=2;
	    } else {
	      i=1;
	    }*/
          /* eselect blk */
	  } else if (funtyp[C2F(curblk).kfun] == -2) {
            /*outtbdptr=(double *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
            i=max(min((int)  outtbdptr[0],
		      Blocks[C2F(curblk).kfun - 1].nevout),1);*/
            switch(outtbtyp[-1+inplnk[inpptr[C2F(curblk).kfun]]])
            {
             case SCSREAL_N    : outtbdptr=(SCSREAL_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSCOMPLEX_N : outtbdptr=(SCSCOMPLEX_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbdptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT8_N    : outtbcptr=(SCSINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbcptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT16_N   : outtbsptr=(SCSINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbsptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSINT32_N   : outtblptr=(SCSINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtblptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT8_N   : outtbucptr=(SCSUINT8_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbucptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT16_N  : outtbusptr=(SCSUINT16_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbusptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             case SCSUINT32_N  : outtbulptr=(SCSUINT32_COP *)outtbptr[-1+inplnk[inpptr[C2F(curblk).kfun]]];
                                 i=max(min((int)  *outtbulptr,
                                           Blocks[C2F(curblk).kfun - 1].nevout),1);
                                 break;

             default  : /* Add a message here */
                        break;
            }
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
      if (((*job==1)&&(oord[ii]==AJacobian_block))||(*job!=1)){
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
	if (((*job==1)&&(oord[ii]==AJacobian_block))||(*job!=1)){
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

int Jacobians(long int Neq, realtype tt, N_Vector yy, N_Vector yp,
           N_Vector resvec, realtype cj, void *jdata, DenseMat Jacque,
           N_Vector tempv1, N_Vector tempv2, N_Vector tempv3)
{
  double  ttx;
  double *xc, *xcdot, *residual;
  /*  char chr;*/
  int i,j,n, nx,ni,no,nb,m,flag;
  double *RX, *Fx, *Fu, *Gx, *Gu, *ERR1,*ERR2;
  double *Hx, *Hu,*Kx,*Ku,*HuGx,*FuKx,*FuKuGx,*HuGuKx;
  double ysave;
  int job;
  double **y = (double **)Blocks[AJacobian_block-1].outptr; /*for compatibility */
  double **u = (double **)Blocks[AJacobian_block-1].inptr; /*warning pointer of y and u have changed to void ***/
   /*  taill1= 2*n+(n+ni)*(n+no)+nx(2*nx+ni+2*m+no)+m*(2*m+no+ni)+2*ni*no*/
  double inc, inc_inv, xi, xpi, srur;
  realtype *Jacque_col;

  UserData data;
  realtype hh;
  N_Vector ewt;
  double *ewt_data;

  *ierr= 0;
  xc   =(double *) N_VGetArrayPointer(yy);
  xcdot  =(double *) N_VGetArrayPointer(yp);
  residual=(double *) NV_DATA_S(resvec);
  ttx=(double)tt;
  CJJ=(double)cj;

  data = (UserData) jdata;
  ewt = data->ewt;
  flag=IDAGetCurrentStep(data->ida_mem, &hh);
  if (flag<0) {  *ierr=200+(-flag); return (*ierr);};

  flag=IDAGetErrWeights(data->ida_mem, ewt);
  if (flag<0) {  *ierr=200+(-flag); return (*ierr);};

  ewt_data=NV_DATA_S(ewt);
  srur =(double) RSqrt(UNIT_ROUNDOFF);

  n=Neq;
  nb=nblk;
  nx=Blocks[AJacobian_block-1].nx; /* quant on est là cela signifie que AJacobian_block>0 */
  m=n-nx;
  no=Blocks[AJacobian_block-1].nout;
  ni=Blocks[AJacobian_block-1].nin;

  ERR1=(double *)data->rwork;
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
  /* HuGuKx+m*m; =>  m*m=size of HuGuKx */
  /* ------------------ Numerical Jacobian--->> Hx,Kx */

  /* read residuals;*/
  /* job=0;
     Jdoit(ERR1, xc, xcdot,&ttx,&job);
     if (*ierr < 0) return -1; */
  /* "residual" already contains the current residual,
     so Masoud removed the first call to Jdoit*/

  for (i=0;i<m;i++)
    for (j=0;j<ni;j++)
      Kx[j+i*ni]=u[j][0];

  for(i=0;i<m;i++){
    xi = xc[i];
    xpi = xcdot[i];
    inc = MAX( srur * MAX( ABS(xi),ABS(hh*xpi)),ONE/ewt_data[i] );
    if (hh*xpi < ZERO) inc = -inc;
    inc = (xi + inc) - xi;
    xc[i] += inc;
    xcdot[i] += CJJ*inc;
    /*a= max(abs(H[0]*xcdot[i]),abs(1.0/Ewt[i]));
      b= max(1.0,abs(xc[i]));
      del=SQUR[0]*max(a,b);    */
    job=0;/* read residuals */
    Jdoit(ERR2, xc, xcdot,&ttx,&job);
    if (*ierr < 0) return -1;
    inc_inv = ONE/inc;
    for(j=0;j<m;j++)
      Hx[m*i+j]=(ERR2[j]-residual[j])*inc_inv;
    for (j=0;j<ni;j++)
      Kx[j+i*ni]=(u[j][0]-Kx[j+i*ni])*inc_inv;
    xc[i]=xi;
    xcdot[i]=xpi;
  }
  /*----- Numerical Jacobian--->> Hu,Ku */

  job=0;
  Jdoit(ERR1, xc, xcdot,&ttx,&job);
  for (i=0;i<no;i++)
    for (j=0;j<ni;j++)
      Ku[j+i*ni]=u[j][0];

  for(i=0;i<no;i++){
    ysave=y[i][0];
    inc =srur * MAX( ABS(ysave),1);
    inc = (ysave + inc) - ysave;
    /*del=SQUR[0]* max(1.0,abs(y[i][0]));
      del=(y[i][0]+del)-y[i][0];*/
    y[i][0] += inc;
    job=2;/* applying y[i][0] to the output of imp block*/
    Jdoit(ERR2, xc, xcdot,&ttx,&job);
    if (*ierr < 0) return -1;
    inc_inv = ONE/inc;
    for(j=0;j<m;j++)
      Hu[m*i+j]=(ERR2[j]-ERR1[j])*inc_inv;
    for (j=0;j<ni;j++)
      Ku[j+i*ni]=(u[j][0]-Ku[j+i*ni])*inc_inv;
    y[i][0]=ysave;
  }
  /*----------------------------------------------*/
  job=1;/* read jacobian through flag=10; */
  Jdoit(&Fx[-m], xc, xcdot,&ttx,&job);/* Filling up the FX:Fu:Gx:Gu*/
  if (*block_error!=0) sciprint("\n\r error in Jacobian");
  /*-------------------------------------------------*/

  Multp(Fu,Ku,RX,nx,ni,ni,no);Multp(RX,Gx,FuKuGx,nx,no,no,nx);

  for (j=0;j<nx;j++){
    Jacque_col=DENSE_COL(Jacque,j+m);
    for (i=0;i<nx;i++){
      Jacque_col[i+m]=Fx[i+j*nx]+FuKuGx[i+j*nx];
    }
  }

  Multp(Hu,Gx,HuGx,m, no, no,nx);

  for (i=0;i<nx;i++){
    Jacque_col=DENSE_COL(Jacque,i+m);
    for (j=0;j<m;j++){
      Jacque_col[j]=HuGx[j+i*m];
    }
  }

  Multp(Fu,Kx,FuKx,nx,ni,ni,m);

  for (i=0;i<m;i++){
    Jacque_col=DENSE_COL(Jacque,i);
    for (j=0;j<nx;j++){
      Jacque_col[j+m]=FuKx[j+i*nx];
    }
  }


  Multp(Hu,Gu,RX,m,no,no,ni); Multp(RX,Kx,HuGuKx,m,ni,ni,m);

  for (j=0;j<m;j++){
    Jacque_col=DENSE_COL(Jacque,j);
    for (i=0;i<m;i++){
      Jacque_col[i]=Hx[i+j*m]+HuGuKx[i+j*m];
    }
  }

  /*  chr='Z';   printf("\n\r t=%g",ttx); DISP(Z,n,n,chr);*/
  C2F(ierode).iero = *ierr;
 return 0;

}
/*----------------------------------------------------*/
void Multp(A,B,R,ra ,ca, rb,cb)
     double *A, *B, *R;
     int ra,rb,ca,cb;
{
  int i,j,k;
  /*if (ca!=rb) sciprint("\n\r Error in matrix multiplication");*/
  for (i = 0; i<ra; i++)
    for (j = 0; j<cb; j++){
      R[i+ra*j]=0.0;
      for (k = 0; k<ca; k++)
	R[i+ra*j]+=A[i+k*ra]*B[k+j*rb];
    }
  return;
}
/*----------------------------------------------------*/
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


