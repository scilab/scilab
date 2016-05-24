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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
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
* CVODE (Sundials) replaced LSODAR
* IDA  (Sundials)  replaced DASKR
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>

/* Sundials includes */
#include <cvode/cvode.h>            /* prototypes for CVODES fcts. and consts. */
#include <cvode/cvode_dense.h>     /* prototype for CVDense */
#include <cvode/cvode_direct.h>    /* prototypes for various DlsMat operations */
#include <ida/ida.h>
#include <ida/ida_dense.h>
#include <ida/ida_direct.h>
#include <nvector/nvector_serial.h>   /* serial N_Vector types, fcts., and macros */
#include <sundials/sundials_dense.h>  /* prototypes for various DlsMat operations */
#include <sundials/sundials_direct.h> /* definitions of DlsMat and DENSE_ELEM */
#include <sundials/sundials_types.h>  /* definition of type realtype */
#include <sundials/sundials_math.h>
#include <kinsol/kinsol.h>
#include <kinsol/kinsol_dense.h>
#include <kinsol/kinsol_direct.h>
#include <sundials/sundials_extension.h> /* uses extension for scicos */
#include "ida_impl.h"

#include "machine.h" /* C2F */
#include "scicos-def.h"
#include "sciprint.h"
#include "scicos.h"
#include "import.h"
#include "scicos_internal.h"
#include "blocks.h"
#include "core_math.h"
#include "storeCommand.h"
#include "syncexec.h"
#include "realtime.h"
#include "sci_malloc.h"
#include "ezxml.h"

#include "sciblk2.h"
#include "sciblk4.h"
#include "dynlib_scicos.h"

#include "configvariable_interface.h" /* getEntryPointPosition() and getEntryPointFromPosition() */

#include "lsodar.h"           /* prototypes for lsodar fcts. and consts. */
#include "ddaskr.h"           /* prototypes for ddaskr fcts. and consts. */

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif

#include "localization.h"
#include "charEncoding.h"
#include "common_structure.h"
#include "Sciwarning.h"

/*--------------------------------------------------------------------------*/
typedef struct
{
    void *dae_mem;
    N_Vector ewt;
    double *rwork;
    int *iwork;
    double *gwork; /* just added for a very special use: a
				   space passing to grblkdakr for zero crossing surfaces
				   when updating mode variables during initialization */
} *UserData;

SCICOS_IMPEXP SCSPTR_struct C2F(scsptr);

enum Solver
{
    LSodar_Dynamic = 0,
    CVode_BDF_Newton,
    CVode_BDF_Functional,
    CVode_Adams_Newton,
    CVode_Adams_Functional,
    Dormand_Prince,
    Runge_Kutta,
    Implicit_Runge_Kutta,
    Crank_Nicolson,
    IDA_BDF_Newton = 100,
    DDaskr_BDF_Newton = 101,
    DDaskr_BDF_GMRes = 102
};
/*--------------------------------------------------------------------------*/

#define freeall					\
	if (*neq>0) ODEFree(&ode_mem); 		\
	if (*neq>0) N_VDestroy_Serial(y);		\
	if ( ng>0 ) FREE(jroot);			\
	if ( ng>0 ) FREE(zcros);


/* TJacque allocated by sundials */
#define freeallx				\
	if (*neq>0) free(TJacque);	\
	if (*neq>0) FREE(data->rwork);		\
	if (( ng>0 )&& (*neq>0)) FREE(data->gwork);	\
	if (*neq>0) N_VDestroy_Serial(data->ewt);	\
	if (*neq>0) FREE(data);			\
	if (*neq>0) DAEFree(&dae_mem); 	\
	if (*neq>0) N_VDestroy_Serial(IDx);		\
	if (*neq>0) N_VDestroy_Serial(yp);		\
	if (*neq>0) N_VDestroy_Serial(yy);		\
	if ( ng>0 ) FREE(jroot);			\
	if ( ng>0 ) FREE(zcros);			\
	if (nmod>0) FREE(Mode_save);

#define freeouttbptr				\
	FREE(outtbd);					\
	FREE(outtbc);					\
	FREE(outtbs);					\
	FREE(outtbl);					\
	FREE(outtbuc);				\
	FREE(outtbus);				\
	FREE(outtbul);

#define freekinsol				\
	FREE(Mode_save);				\
	N_VDestroy_Serial(y);				\
	N_VDestroy_Serial(fscale);			\
	N_VDestroy_Serial(yscale);			\
	KINFree(&kin_mem);


#define ONE   RCONST(1.0)
#define ZERO  RCONST(0.0)
#define T0    RCONST(0.0)

// Special values for elements of 'funtyp'
#define FORTRAN_GATEWAY   0
#define UNUSED1           1
#define UNUSED2           2
#define SCIFUNC_BLOCK     3
#define EXPRESSION_BLOCK  5
#define IFTHEL_BLOCK      11
#define ESELECT_BLOCK     12
#define DEBUG_BLOCK       99
#define OLD_SCI_BLOCK     10005
/*--------------------------------------------------------------------------*/
/* Table of constant values */
static int c__90 = 90;
static int c__91 = 91;
static int c__0 = 0;
static double c_b14 = 0.;
static int c__1 = 1;

int TCritWarning = 0;

static double *t0 = NULL, *tf = NULL;
static double *x = NULL, *tevts = NULL, *xd = NULL, *g = NULL;
static  double Atol = 0., rtol = 0., ttol = 0., deltat = 0., hmax = 0.;
static int *ierr = NULL;
static int *pointi = NULL;
static int *xptr = NULL, *modptr = NULL, *evtspt = NULL;
static int *funtyp = NULL, *inpptr = NULL, *outptr = NULL, *inplnk = NULL, *outlnk = NULL;
static int *clkptr = NULL, *ordptr = NULL, *ordclk = NULL, *cord = NULL, *iord = NULL, *oord = NULL,  *zord = NULL,  *critev = NULL,  *zcptr = NULL;
static int nblk = 0, nordptr = 0, nlnk = 0, nx = 0, ng = 0, ncord = 0, noord = 0, nzord = 0;
static int nordclk = 0, niord = 0, nmod = 0;
static int nelem = 0;
static int *mod = NULL;
static int *neq = NULL;
static int *xprop = NULL; /* xproperty */
static int debug_block = 0;
static int *iwa = NULL;
static int hot;

/* declaration of ptr for typed port */
static void **outtbptr = NULL; /*pointer array of object of outtb*/
static int *outtbsz = NULL;    /*size of object of outtb*/
static int *outtbtyp = NULL;   /*type of object of outtb*/

SCSREAL_COP *outtbdptr = NULL;     /*to store double of outtb*/
SCSINT8_COP *outtbcptr = NULL;     /*to store int8 of outtb*/
SCSINT16_COP *outtbsptr = NULL;    /*to store int16 of outtb*/
SCSINT32_COP *outtblptr = NULL;    /*to store int32 of outtb*/
SCSUINT8_COP *outtbucptr = NULL;   /*to store unsigned int8 of outtb */
SCSUINT16_COP *outtbusptr = NULL;  /*to store unsigned int16 of outtb */
SCSUINT32_COP *outtbulptr = NULL;  /*to store unsigned int32 of outtb */

static outtb_el *outtb_elem = NULL;

static scicos_block *Blocks = NULL;

/* pass to external variable for code generation */
/* reserved variable name */
int *block_error = NULL;
double scicos_time = 0.;
int phase = 0;
int Jacobian_Flag = 0;
// double CI = 0., CJ = 0.;  // doubles returned by Get_Jacobian_ci and Get_Jacobian_cj respectively
double  CJJ = 0.;            // returned by Get_Jacobian_parameter
double SQuround = 0.;
/* Jacobian*/
static int AJacobian_block = 0;


/* Variable declaration moved to scicos.c because it was in the scicos-def.h therefore
* multiple declaration of the variable and linkers were complaining about duplicate
* symbols
*/
SCICOS_IMPEXP COSDEBUGCOUNTER_struct C2F(cosdebugcounter);
SCICOS_IMPEXP RTFACTOR_struct C2F(rtfactor);
SCICOS_IMPEXP SOLVER_struct C2F(cmsolver);
SCICOS_IMPEXP CURBLK_struct C2F(curblk);
SCICOS_IMPEXP COSDEBUG_struct C2F(cosdebug);
SCICOS_IMPEXP COSHLT_struct C2F(coshlt);
SCICOS_IMPEXP DBCOS_struct C2F(dbcos);
SCICOS_IMPEXP COSTOL_struct C2F(costol);
SCICOS_IMPEXP COSERR_struct coserr;
/*--------------------------------------------------------------------------*/
static void FREE_blocks();
static void cosini(double *told);
static void cosend(double *told);
static void cossim(double *told);
static void cossimdaskr(double *told);
static void doit(double *told);
static void idoit(double *told);
static void zdoit(double *told, double *xt, double *xtd, double *g);
static void cdoit(double *told);
static void ozdoit(double *told, double *xt, double *xtd, int *kiwa);
static void odoit(double *told, double *xt, double *xtd, double *residual);
static void ddoit(double *told);
static void edoit(double *told, int *kiwa);
static void reinitdoit(double *told);
static int CallKinsol(double *told);
static int simblk(realtype t, N_Vector yy, N_Vector yp, void *f_data);
static int grblkdaskr(realtype t, N_Vector yy, N_Vector yp, realtype *gout, void *g_data);
static int grblk(realtype t, N_Vector yy, realtype *gout, void *g_data);
static void simblklsodar(int * nequations, realtype * tOld, realtype * actual, realtype * res);
static void grblklsodar(int * nequations, realtype * tOld, realtype * actual, int * ngc, realtype * res);
static void simblkddaskr(realtype *tOld, realtype *actual, realtype *actualP, realtype *res, int *flag, double *dummy1, int *dummy2);
static void grblkddaskr(int *nequations, realtype *tOld, realtype *actual, int *ngc, realtype *res, double *dummy1, int *dummy2);
static void jacpsol(realtype *res, int *ires, int *nequations, realtype *tOld, realtype *actual, realtype *actualP,
                    realtype *rewt, realtype *savr, realtype *wk, realtype *h, realtype *cj, realtype *wp,
                    int *iwp, int *ier, double *dummy1, int *dummy2);
static void psol(int *nequations, realtype *tOld, realtype *actual, realtype *actualP,
                 realtype *savr, realtype *wk, realtype *cj, realtype *wght, realtype *wp,
                 int *iwp, realtype *b, realtype *eplin, int *ier, double *dummy1, int *dummy2);
static void addevs(double t, int *evtnb, int *ierr1);
static int synchro_g_nev(ScicosImport *scs_imp, double *g, int kf, int *ierr);
static void Multp(double *A, double *B, double *R, int ra, int rb, int ca, int cb);
static int read_id(ezxml_t *elements, char *id, double *value);
static int simblkdaskr(realtype tres, N_Vector yy, N_Vector yp, N_Vector resval, void *rdata);
static void SundialsErrHandler(int error_code, const char *module, const char *function, char *msg, void *user_data);
static int Jacobians(long int Neq, realtype tt, realtype cj, N_Vector yy,
                     N_Vector yp, N_Vector resvec, DlsMat Jacque, void *jdata,
                     N_Vector tempv1, N_Vector tempv2, N_Vector tempv3);
static void call_debug_scicos(scicos_block *block, scicos_flag *flag, int flagi, int deb_blk);
static int synchro_nev(ScicosImport *scs_imp, int kf, int *ierr);
/*--------------------------------------------------------------------------*/
extern int C2F(dset)(int *n, double *dx, double *dy, int *incy);
extern int C2F(dcopy)(int *, double *, int *, double *, int *);
extern int C2F(dgefa)(double *A, int *lead_dim_A, int *n, int *ipivots, int *info);
extern int C2F(dgesl)(double *A, int *lead_dim_A, int *n, int *ipivots, double *B, int *job);
extern int C2F(msgs)();
extern void C2F(clearscicosimport)();
/*--------------------------------------------------------------------------*/
void putevs(double *t, int *evtnb, int *ierr1);
void Jdoit(double *told, double *xt, double *xtd, double *residual, int *job);
int simblkKinsol(N_Vector yy, N_Vector resval, void *rdata);
/*--------------------------------------------------------------------------*/
int C2F(scicos)(double *x_in, int *xptr_in, double *z__,
                void **work, int *zptr, int *modptr_in,
                void **oz, int *ozsz, int *oztyp, int *ozptr,
                char **iz, int *izptr, char **uid, int *uidptr, double *t0_in,
                double *tf_in, double *tevts_in, int *evtspt_in,
                int *nevts, int *pointi_in, void **outtbptr_in,
                int *outtbsz_in, int *outtbtyp_in,
                outtb_el *outtb_elem_in, int *nelem1, int *nlnk1,
                void** funptr, int *funtyp_in, int *inpptr_in,
                int *outptr_in, int *inplnk_in, int *outlnk_in,
                double *rpar, int *rpptr, int *ipar, int *ipptr,
                void **opar, int *oparsz, int *opartyp, int *opptr,
                int *clkptr_in, int *ordptr_in, int *nordptr1,
                int *ordclk_in, int *cord_in, int *ncord1,
                int *iord_in, int *niord1, int *oord_in,
                int *noord1, int *zord_in, int *nzord1,
                int *critev_in, int *nblk1, int *ztyp,
                int *zcptr_in, int *subscr, int *nsubs,
                double *simpar, int *flag__, int *ierr_out)
{
    int i1, kf, lprt, in, out, job = 1;


    static int mxtb = 0, ierr0 = 0, kfun0 = 0, i = 0, j = 0, k = 0, jj = 0;
    static int ni = 0, no = 0;
    static int nz = 0, noz = 0, nopar = 0;
    double *W = NULL;

    // Set FPU Flag to Extended for scicos simulation
    // in order to override Java setting it to Double.
#if defined(linux) && defined(__i386__)
    setFPUToExtended();
#endif
    /*     Copyright INRIA */
    /* iz,izptr are used to pass block labels */
    TCritWarning = 0;

    t0 = t0_in;
    tf = tf_in;
    ierr = ierr_out;

    /* Parameter adjustments */
    pointi = pointi_in;
    x = x_in;
    xptr = xptr_in - 1;
    modptr = modptr_in - 1;
    --zptr;
    //--izptr;
    --ozptr;
    evtspt = evtspt_in - 1;
    tevts = tevts_in - 1;
    outtbptr = outtbptr_in;
    outtbsz = outtbsz_in;
    outtbtyp = outtbtyp_in;
    outtb_elem = outtb_elem_in;
    funtyp = funtyp_in - 1;
    inpptr = inpptr_in - 1;
    outptr = outptr_in - 1;
    inplnk = inplnk_in - 1;
    outlnk = outlnk_in - 1;
    --rpptr;
    --ipptr;
    --opptr;
    clkptr = clkptr_in - 1;
    ordptr = ordptr_in - 1;
    ordclk = ordclk_in - 1;
    cord = cord_in - 1;
    iord = iord_in - 1;
    oord = oord_in - 1;
    zord = zord_in - 1;

    critev = critev_in - 1;
    --ztyp;
    zcptr = zcptr_in - 1;

    /* Function Body */
    Atol = simpar[0];
    rtol = simpar[1];
    ttol = simpar[2];
    deltat = simpar[3];
    C2F(rtfactor).scale = simpar[4];
    C2F(cmsolver).solver = (int) simpar[5];
    hmax = simpar[6];

    nordptr = *nordptr1;
    nblk  = *nblk1;
    ncord = *ncord1;
    noord = *noord1;
    nzord = *nzord1;
    niord = *niord1;
    nlnk  = *nlnk1;
    nelem = *nelem1;
    *ierr = 0;

    nordclk = ordptr[nordptr] - 1;  /* number of rows in ordclk is ordptr(nclkp1)-1 */
    ng      = zcptr[nblk + 1] - 1;  /* computes number of zero crossing surfaces */
    nmod    = modptr[nblk + 1] - 1; /* computes number of modes */
    nz      = zptr[nblk + 1] - 1;   /* number of discrete real states */
    noz     = ozptr[nblk + 1] - 1;  /* number of discrete object states */
    nopar   = opptr[nblk + 1] - 1;  /* number of object parameters */
    nx      = xptr[nblk + 1] - 1;   /* number of object parameters */
    neq     = &nx;

    xd = &x[xptr[nblk + 1] - 1];

    /* check for hard coded maxsize */
    for (i = 1; i <= nblk; ++i)
    {
        if (funtyp[i] < 10000)
        {
            funtyp[i] %= 1000;
        }
        else
        {
            funtyp[i] = funtyp[i] % 1000 + 10000;
        }
        ni = inpptr[i + 1] - inpptr[i];
        no = outptr[i + 1] - outptr[i];
        if (funtyp[i] == 1)
        {
            if (ni + no > 11)
            {
                /*     hard coded maxsize in callf.c */
                C2F(msgs)(&c__90, &c__0);
                C2F(curblk).kfun = i;
                *ierr = i + 1005;
                return 0;
            }
        }
        else if (funtyp[i] == 2 || funtyp[i] == 3)
        {
            /*     hard coded maxsize in scicos.h */
            if (ni + no > SZ_SIZE)
            {
                C2F(msgs)(&c__90, &c__0);
                C2F(curblk).kfun = i;
                *ierr = i + 1005;
                return 0;
            }
        }
        mxtb = 0;
        if (funtyp[i] == 0)
        {
            if (ni > 1)
            {
                for (j = 1; j <= ni; ++j)
                {
                    k = inplnk[inpptr[i] - 1 + j];
                    mxtb = mxtb + (outtbsz[k - 1] * outtbsz[(k - 1) + nlnk]);
                }
            }
            if (no > 1)
            {
                for (j = 1; j <= no; ++j)
                {
                    k = outlnk[outptr[i] - 1 + j];
                    mxtb = mxtb + (outtbsz[k - 1] * outtbsz[(k - 1) + nlnk]);
                }
            }
            if (mxtb > TB_SIZE)
            {
                C2F(msgs)(&c__91, &c__0);
                C2F(curblk).kfun = i;
                *ierr = i + 1005;
                return 0;
            }
        }
    }

    if (nx > 0) /* xprop */
    {
        if ((xprop = MALLOC(sizeof(int) * nx)) == NULL )
        {
            *ierr = 5;
            return 0;
        }
    }
    for (i = 0; i < nx; i++) /* initialize */
    {
        xprop[i] = 1;
    }
    if (nmod > 0) /* mod */
    {
        if ((mod = MALLOC(sizeof(int) * nmod)) == NULL )
        {
            *ierr = 5;
            if (nx > 0)
            {
                FREE(xprop);
            }
            return 0;
        }
    }
    if (ng > 0) /* g becomes global */
    {
        if ((g = MALLOC(sizeof(double) * ng)) == NULL )
        {
            *ierr = 5;
            if (nmod > 0)
            {
                FREE(mod);
            }
            if (nx > 0)
            {
                FREE(xprop);
            }
            return 0;
        }
    }

    debug_block = -1; /* no debug block for start */
    C2F(cosdebugcounter).counter = 0;

    /** Create Block's array **/
    if ((Blocks = MALLOC(sizeof(scicos_block) * nblk)) == NULL )
    {
        *ierr = 5;
        if (nx > 0)
        {
            FREE(xprop);
        }
        if (nmod > 0)
        {
            FREE(mod);
        }
        if (ng > 0)
        {
            FREE(g);
        }
        return 0;
    }

    /** Setting blocks properties for each entry in Block's array **/

    /* 1 : type and pointer on simulation function */
    for (kf = 0; kf < nblk; ++kf)   /*for each block */
    {
        void* p = funptr[kf];
        C2F(curblk).kfun = kf + 1;
        Blocks[kf].type = funtyp[kf + 1];
        if (Blocks[kf].type == IFTHEL_BLOCK)
        {
            funtyp[kf + 1] = -1;
        }
        else if (Blocks[kf].type == ESELECT_BLOCK)
        {
            funtyp[kf + 1] = -2;
        }
        else if (Blocks[kf].type < 0)
        {
            //macros
            funtyp[kf + 1] *= -1; // Restore a positive 'funtyp' for later use
            switch (-Blocks[kf].type)
            {
                case FORTRAN_GATEWAY:
                    Blocks[kf].funpt = (voidg)F2C(sciblk);
                    break;
                case UNUSED1:
                    sciprint(_("type 1 function not allowed for scilab blocks\n"));
                    *ierr = 1000 + kf + 1;
                    FREE_blocks();
                    return 0;
                case UNUSED2:
                    sciprint(_("type 2 function not allowed for scilab blocks\n"));
                    *ierr = 1000 + kf + 1;
                    FREE_blocks();
                    return 0;
                case SCIFUNC_BLOCK:
                    Blocks[kf].funpt = (voidg)sciblk2;
                    Blocks[kf].type = 2;
                    break;
                case EXPRESSION_BLOCK:
                    Blocks[kf].funpt = (voidg)sciblk4;
                    Blocks[kf].type = 4;
                    break;
                case DEBUG_BLOCK: /* debugging block */
                    Blocks[kf].funpt = (voidg)sciblk4;
                    Blocks[kf].type *= -1;
                    debug_block = kf;
                    break;

                case OLD_SCI_BLOCK:
                    Blocks[kf].funpt = (voidg)sciblk4;
                    Blocks[kf].type = 10004;
                    break;
                default:
                    sciprint(_("Undefined Function type\n"));
                    *ierr = 1000 + kf + 1;
                    FREE_blocks();
                    return 0;
            }
            Blocks[kf].scsptr = p; /* set scilab function adress for sciblk */
        }
        else
        {
            //linked functions (internal or external)
            Blocks[kf].funpt = (voidf)p;
            Blocks[kf].scsptr = NULL;   /* this is done for being able to test if a block
                                        is a scilab block in the debugging phase when
                                        sciblk4 is called */
        }

        /* 2 : Dimension properties */
        Blocks[kf].ztyp = ztyp[kf + 1];
        Blocks[kf].nx = xptr[kf + 2] - xptr[kf + 1]; /* continuous state dimension*/
        Blocks[kf].ng = zcptr[kf + 2] - zcptr[kf + 1]; /* number of zero crossing surface*/
        Blocks[kf].nz = zptr[kf + 2] - zptr[kf + 1]; /* number of double discrete state*/
        Blocks[kf].noz = ozptr[kf + 2] - ozptr[kf + 1]; /* number of other discrete state*/
        Blocks[kf].nrpar = rpptr[kf + 2] - rpptr[kf + 1]; /* size of double precision parameter vector*/
        Blocks[kf].nipar = ipptr[kf + 2] - ipptr[kf + 1]; /* size of integer precision parameter vector*/
        Blocks[kf].nopar = opptr[kf + 2] - opptr[kf + 1]; /* number of other parameters (matrix, data structure,..)*/
        Blocks[kf].nin = inpptr[kf + 2] - inpptr[kf + 1]; /* number of input ports */
        Blocks[kf].nout = outptr[kf + 2] - outptr[kf + 1]; /* number of output ports */

        /* 3 : input port properties */
        /* in insz, we store :
        *  - insz[0..nin-1] : first dimension of input ports
        *  - insz[nin..2*nin-1] : second dimension of input ports
        *  - insz[2*nin..3*nin-1] : type of data of input ports
        */
        /* allocate size and pointer arrays (number of input ports)*/
        Blocks[kf].insz = NULL;
        Blocks[kf].inptr = NULL;
        if (Blocks[kf].nin != 0)
        {
            if ((Blocks[kf].insz = MALLOC(Blocks[kf].nin * 3 * sizeof(int))) == NULL )
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
            if ((Blocks[kf].inptr = MALLOC(Blocks[kf].nin * sizeof(double*))) == NULL )
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
        }
        for (in = 0; in < Blocks[kf].nin; in++)
        {
            lprt = inplnk[inpptr[kf + 1] + in];
            Blocks[kf].inptr[in] = outtbptr[lprt - 1]; /* pointer on the data*/
            Blocks[kf].insz[in] = outtbsz[lprt - 1]; /* row dimension of the input port*/
            Blocks[kf].insz[Blocks[kf].nin + in] = outtbsz[(lprt - 1) + nlnk]; /* column dimension of the input port*/
            Blocks[kf].insz[2 * Blocks[kf].nin + in] = outtbtyp[lprt - 1]; /*type of data of the input port*/
        }
        /* 4 : output port properties */
        /* in outsz, we store :
        *  - outsz[0..nout-1] : first dimension of output ports
        *  - outsz[nout..2*nout-1] : second dimension of output ports
        *  - outsz[2*nout..3*nout-1] : type of data of output ports
        */
        /* allocate size and pointer arrays (number of output ports)*/
        Blocks[kf].outsz = NULL;
        Blocks[kf].outptr = NULL;
        if (Blocks[kf].nout != 0)
        {
            if ((Blocks[kf].outsz = MALLOC(Blocks[kf].nout * 3 * sizeof(int))) == NULL )
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
            if ((Blocks[kf].outptr = MALLOC(Blocks[kf].nout * sizeof(double*))) == NULL )
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
        }
        /* set the values */
        for (out = 0; out < Blocks[kf].nout; out++) /*for each output port */
        {
            lprt = outlnk[outptr[kf + 1] + out];
            Blocks[kf].outptr[out] = outtbptr[lprt - 1]; /*pointer on data */
            Blocks[kf].outsz[out] = outtbsz[lprt - 1]; /*row dimension of output port*/
            Blocks[kf].outsz[Blocks[kf].nout + out] = outtbsz[(lprt - 1) + nlnk]; /*column dimension of output ports*/
            Blocks[kf].outsz[2 * Blocks[kf].nout + out] = outtbtyp[lprt - 1]; /*type of data of output port */
        }

        /* 5 : event output port properties */
        Blocks[kf].evout = NULL;
        Blocks[kf].nevout = clkptr[kf + 2] - clkptr[kf + 1];
        if (Blocks[kf].nevout != 0)
        {
            if ((Blocks[kf].evout = CALLOC(Blocks[kf].nevout, sizeof(double))) == NULL )
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
        }

        /* 6 : pointer on the begining of the double discrete state array ( z) */
        Blocks[kf].z = &(z__[zptr[kf + 1] - 1]);

        /* 7 : type, size and pointer on the other discrete states  data structures (oz) */
        Blocks[kf].ozsz = NULL;
        if (Blocks[kf].noz == 0)
        {
            Blocks[kf].ozptr = NULL;
            Blocks[kf].oztyp = NULL;
        }
        else
        {
            Blocks[kf].ozptr = &(oz[ozptr[kf + 1] - 1]);
            if ((Blocks[kf].ozsz = MALLOC(Blocks[kf].noz * 2 * sizeof(int))) == NULL )
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
            for (i = 0; i < Blocks[kf].noz; i++)
            {
                Blocks[kf].ozsz[i] = ozsz[(ozptr[kf + 1] - 1) + i];
                Blocks[kf].ozsz[i + Blocks[kf].noz] = ozsz[(ozptr[kf + 1] - 1 + noz) + i];
            }
            Blocks[kf].oztyp = &(oztyp[ozptr[kf + 1] - 1]);
        }

        /* 8 : pointer on the begining of the double parameter array ( rpar ) */
        Blocks[kf].rpar = &(rpar[rpptr[kf + 1] - 1]);

        /* 9 : pointer on the begining of the integer parameter array ( ipar ) */
        Blocks[kf].ipar = &(ipar[ipptr[kf + 1] - 1]);

        /* 10 : type, size and pointer on the other parameters  data structures (opar) */
        Blocks[kf].oparsz = NULL;
        if (Blocks[kf].nopar == 0)
        {
            Blocks[kf].oparptr = NULL;
            Blocks[kf].opartyp = NULL;
        }
        else
        {
            Blocks[kf].oparptr = &(opar[opptr[kf + 1] - 1]);
            if ((Blocks[kf].oparsz = MALLOC(Blocks[kf].nopar * 2 * sizeof(int))) == NULL)
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
            for (i = 0; i < Blocks[kf].nopar; i++)
            {
                Blocks[kf].oparsz[i] = oparsz[(opptr[kf + 1] - 1) + i];
                Blocks[kf].oparsz[i + Blocks[kf].nopar] = oparsz[(opptr[kf + 1] - 1 + nopar) + i];
            }
            Blocks[kf].opartyp = &(opartyp[opptr[kf + 1] - 1]);
        }

        /* 10 : pointer on the beginning of the residual array (res) */
        Blocks[kf].res = NULL;
        if (Blocks[kf].nx != 0)
        {
            if ((Blocks[kf].res = MALLOC(Blocks[kf].nx * sizeof(double))) == NULL)
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
        }

        /* 11 : block label (label) */
        i1 = izptr[kf];
        if ((Blocks[kf].label = MALLOC(sizeof(char) * (i1 + 1))) == NULL)
        {
            FREE_blocks();
            *ierr = 5;
            return 0;
        }
        Blocks[kf].label[i1] = '\0';
        strcpy(Blocks[kf].label, iz[kf]);

        /* block uid (uid) */
        i1 = uidptr[kf];
        if ((Blocks[kf].uid = MALLOC(sizeof(char) * (i1 + 1))) == NULL)
        {
            FREE_blocks();
            *ierr = 5;
            return 0;
        }
        Blocks[kf].uid[i1] = '\0';
        strcpy(Blocks[kf].uid, uid[kf]);

        /* 12 : block array of crossed surfaces (jroot) */
        Blocks[kf].jroot = NULL;
        if (Blocks[kf].ng > 0)
        {
            if ((Blocks[kf].jroot = CALLOC(Blocks[kf].ng, sizeof(int))) == NULL)
            {
                FREE_blocks();
                *ierr = 5;
                return 0;
            }
        }

        /* 13 : block work array  (work) */
        Blocks[kf].work = (void **)(((double *)work) + kf);

        /* 14 : block modes  array  (mode) */
        Blocks[kf].nmode = modptr[kf + 2] - modptr[kf + 1];
        if (Blocks[kf].nmode != 0)
        {
            Blocks[kf].mode = &(mod[modptr[kf + 1] - 1]);
        }

        /* 15 : block xprop  array  (xprop) */
        Blocks[kf].xprop = NULL;
        if (Blocks[kf].nx != 0)
        {
            Blocks[kf].xprop = &(xprop[xptr[kf + 1] - 1]);
        }

        /* 16 : pointer on the zero crossing surface computation function of the block (g) */
        Blocks[kf].g = NULL;
        if (Blocks[kf].ng != 0)
        {
            Blocks[kf].g = &(g[zcptr[kf + 1] - 1]);
        }
    }
    /** all block properties are stored in the Blocks array **/

    /* iwa */
    iwa = NULL;
    if ((*nevts) != 0)
    {
        if ((iwa = MALLOC(sizeof(int) * (*nevts))) == NULL)
        {
            FREE_blocks();
            *ierr = 5;
            return 0;
        }
    }

    /* save ptr of scicos in import structure */
    makescicosimport(x, &nx, &xptr[1], &zcptr[1], z__, &nz, &zptr[1],
                     &noz, oz, ozsz, oztyp, &ozptr[1],
                     g, &ng, mod, &nmod, &modptr[1], iz, &izptr[1],
                     uid, uidptr,
                     &inpptr[1], &inplnk[1], &outptr[1], &outlnk[1],
                     outtbptr, outtbsz, outtbtyp,
                     outtb_elem, &nelem,
                     &nlnk, rpar, &rpptr[1], ipar, &ipptr[1],
                     opar, oparsz, opartyp, &opptr[1],
                     &nblk, subscr, nsubs,
                     &tevts[1], &evtspt[1], nevts, pointi,
                     &iord[1], &niord, &oord[1], &noord, &zord[1], &nzord,
                     funptr, &funtyp[1], &ztyp[1],
                     &cord[1], &ncord, &ordclk[1], &nordclk, &clkptr[1],
                     &ordptr[1], &nordptr, &critev[1], iwa, Blocks,
                     t0, tf, &Atol, &rtol, &ttol, &deltat, &hmax,
                     xprop, xd);

    if (*flag__ == 1)   /*start*/
    {
        /*      blocks initialization */
        for (kf = 0; kf < nblk; ++kf)
        {
            *(Blocks[kf].work) = NULL;
        }
        cosini(t0);
        if (*ierr != 0)
        {
            ierr0 = *ierr;
            kfun0 = C2F(curblk).kfun;
            cosend(t0);
            *ierr = ierr0;
            C2F(curblk).kfun = kfun0;
        }

    }
    else if (*flag__ == 2)     /*run*/
    {

        /*     integration */
        switch (C2F(cmsolver).solver)
        {
            case LSodar_Dynamic:
            case CVode_BDF_Newton:
            case CVode_BDF_Functional:
            case CVode_Adams_Newton:
            case CVode_Adams_Functional:
            case Dormand_Prince:
            case Runge_Kutta:
            case Implicit_Runge_Kutta:
            case Crank_Nicolson:
                cossim(t0);
                break;
            case IDA_BDF_Newton:
            case DDaskr_BDF_Newton:
            case DDaskr_BDF_GMRes:
                cossimdaskr(t0);
                break;
            default: // Unknown solver number
                *ierr = 1000;
                return 0;
        }
        if (*ierr != 0)
        {
            ierr0 = *ierr;
            kfun0 = C2F(curblk).kfun;
            cosend(t0);
            *ierr = ierr0;
            C2F(curblk).kfun = kfun0;
        }

    }
    else if (*flag__ == 3)     /*finish*/
    {
        /*     blocks closing */
        cosend(t0);
    }
    else if (*flag__ == 4)     /*linear*/
    {
        phase = 1;
        idoit(t0);
        if (*ierr == 0)
        {
            if ((W = MALLOC(sizeof(double) * (Max(nx, ng)))) == NULL )
            {
                FREE(iwa);
                FREE_blocks();
                *ierr = 5;
                return 0;
            }

            /*---------instead of old simblk--------*/
            /*  C2F(simblk)(&nx, t0, x, W);  */

            if (ng > 0 && nmod > 0)
            {
                zdoit(t0, x, x + nx, W); /* updating modes as a function of state values; this was necessary in iGUI*/
            }
            for (jj = 0; jj < nx; jj++)
            {
                W[jj] = 0.0;
            }
            C2F(ierode).iero = 0;
            *ierr = 0;
            if (C2F(cmsolver).solver < 100)
            {
                odoit(t0, x, W, W);
            }
            else
            {
                odoit(t0, x, x + nx, W);
            }
            C2F(ierode).iero = *ierr;
            /*-----------------------------------------*/
            for (i = 0; i < nx; ++i)
            {
                x[i] = W[i];
            }
            FREE(W);
        }
    }
    else if (*flag__ == 5)     /* initial_KINSOL= "Kinsol" */
    {
        C2F(ierode).iero = 0;
        *ierr = 0;
        idoit(t0);
        CallKinsol(t0);
        *ierr = C2F(ierode).iero;
    }


    FREE(iwa);
    FREE_blocks();

    C2F(clearscicosimport)();
    return 0;
} /* scicos_ */
/*--------------------------------------------------------------------------*/
/* check_flag */
static int check_flag(void *flagvalue, char *funcname, int opt)
{
    int *errflag = NULL;

    /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
    if (opt == 0 && flagvalue == NULL)
    {
        sciprint(_("\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n"), funcname);
        return (1);
    }
    /* Check if flag < 0 */
    else if (opt == 1)
    {
        errflag = (int *) flagvalue;
        if (*errflag < 0)
        {
            sciprint(_("\nSUNDIALS_ERROR: %s() failed with flag = %d\n\n"),
                     funcname, *errflag);
            return (1);
        }
    }
    /* Check if function returned NULL pointer - no memory allocated */
    else if (opt == 2 && flagvalue == NULL)
    {
        sciprint(_("\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n"), funcname);
        return (1);
    }

    return (0);
} /* check_flag */

/*--------------------------------------------------------------------------*/
static void cosini(double *told)
{
    static scicos_flag flag__ = 0;
    static int i = 0;

    static int kfune = 0;
    static int jj = 0;

    SCSREAL_COP *outtbd = NULL;    /*to save double of outtb*/
    SCSINT8_COP *outtbc = NULL;    /*to save int8 of outtb*/
    SCSINT16_COP *outtbs = NULL;   /*to save int16 of outtb*/
    SCSINT32_COP *outtbl = NULL;   /*to save int32 of outtb*/
    SCSUINT8_COP *outtbuc = NULL;  /*to save unsigned int8 of outtb*/
    SCSUINT16_COP *outtbus = NULL; /*to save unsigned int16 of outtb*/
    SCSUINT32_COP *outtbul = NULL; /*to save unsigned int32 of outtb*/
    int szouttbd = 0;  /*size of arrays*/
    int szouttbc = 0,  szouttbs = 0,  szouttbl = 0;
    int szouttbuc = 0, szouttbus = 0, szouttbul = 0;
    int curouttbd = 0; /*current position in arrays*/
    int curouttbc = 0,  curouttbs = 0,  curouttbl = 0;
    int curouttbuc = 0, curouttbus = 0, curouttbul = 0;

    int ii = 0, kk = 0; /*local counters*/
    int sszz = 0;  /*local size of element of outtb*/
    /*Allocation of arrays for outtb*/
    for (ii = 0; ii < nlnk; ii++)
    {
        switch (outtbtyp[ii])
        {
            case SCSREAL_N    :
                szouttbd += outtbsz[ii] * outtbsz[ii + nlnk]; /*double real matrix*/
                outtbd = (SCSREAL_COP *) REALLOC (outtbd, szouttbd * sizeof(SCSREAL_COP));
                memset(outtbd, 0, szouttbd * sizeof(SCSREAL_COP));
                break;

            case SCSCOMPLEX_N :
                szouttbd += 2 * outtbsz[ii] * outtbsz[ii + nlnk]; /*double complex matrix*/
                outtbd = (SCSCOMPLEX_COP *) REALLOC (outtbd, szouttbd * sizeof(SCSCOMPLEX_COP));
                memset(outtbd, 0, szouttbd * sizeof(SCSCOMPLEX_COP));
                break;

            case SCSINT8_N    :
                szouttbc += outtbsz[ii] * outtbsz[ii + nlnk]; /*int8*/
                outtbc = (SCSINT8_COP *) REALLOC (outtbc, szouttbc * sizeof(SCSINT8_COP));
                memset(outtbc, 0, szouttbc * sizeof(SCSINT8_COP));
                break;

            case SCSINT16_N   :
                szouttbs += outtbsz[ii] * outtbsz[ii + nlnk]; /*int16*/
                outtbs = (SCSINT16_COP *) REALLOC (outtbs, szouttbs * sizeof(SCSINT16_COP));
                memset(outtbs, 0, szouttbs * sizeof(SCSINT16_COP));
                break;

            case SCSINT32_N   :
                szouttbl += outtbsz[ii] * outtbsz[ii + nlnk]; /*int32*/
                outtbl = (SCSINT32_COP *) REALLOC (outtbl, szouttbl * sizeof(SCSINT32_COP));
                memset(outtbl, 0, szouttbl * sizeof(SCSINT32_COP));
                break;

            case SCSUINT8_N   :
                szouttbuc += outtbsz[ii] * outtbsz[ii + nlnk]; /*uint8*/
                outtbuc = (SCSUINT8_COP *) REALLOC (outtbuc, szouttbuc * sizeof(SCSUINT8_COP));
                memset(outtbuc, 0, szouttbuc * sizeof(SCSUINT8_COP));
                break;

            case SCSUINT16_N  :
                szouttbus += outtbsz[ii] * outtbsz[ii + nlnk]; /*uint16*/
                outtbus = (SCSUINT16_COP *) REALLOC (outtbus, szouttbus * sizeof(SCSUINT16_COP));
                memset(outtbus, 0, szouttbus * sizeof(SCSUINT16_COP));
                break;

            case SCSUINT32_N  :
                szouttbul += outtbsz[ii] * outtbsz[ii + nlnk]; /*uint32*/
                outtbul = (SCSUINT32_COP *) REALLOC (outtbul, szouttbul * sizeof(SCSUINT32_COP));
                memset(outtbul, 0, szouttbul * sizeof(SCSUINT32_COP));
                break;

            default  : /* Add a message here */
                break;
        }
    }

    /* Jacobian*/
    AJacobian_block = 0;

    /* Function Body */
    *ierr = 0;

    /*     initialization (flag 4) */
    /*     loop on blocks */
    C2F(dset)(&ng, &c_b14, g, &c__1);

    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun)
    {
        flag__ = 4;
        if (Blocks[C2F(curblk).kfun - 1].nx > 0)
        {
            Blocks[C2F(curblk).kfun - 1].x  = &x[xptr[C2F(curblk).kfun] - 1];
            Blocks[C2F(curblk).kfun - 1].xd = &xd[xptr[C2F(curblk).kfun] - 1];
        }
        Blocks[C2F(curblk).kfun - 1].nevprt = 0;
        if (funtyp[C2F(curblk).kfun] >= 0)   /* debug_block is not called here */
        {
            /*callf(told, xd, x, x,g,&flag__);*/
            Jacobian_Flag = 0;
            callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
            if (flag__ < 0 && *ierr == 0)
            {
                *ierr = 5 - flag__;
                kfune = C2F(curblk).kfun;
            }
            if ((Jacobian_Flag == 1) && (AJacobian_block == 0))
            {
                AJacobian_block = C2F(curblk).kfun;
            }
        }
    }
    if (*ierr != 0)
    {
        C2F(curblk).kfun = kfune;
        freeouttbptr;
        return;
    }

    /*     initialization (flag 6) */
    flag__ = 6;
    for (jj = 1; jj <= ncord; ++jj)
    {
        C2F(curblk).kfun = cord[jj];
        Blocks[C2F(curblk).kfun - 1].nevprt = 0;
        if (funtyp[C2F(curblk).kfun] >= 0)
        {
            /*callf(told, xd, x, x,g,&flag__);*/
            callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
            if (flag__ < 0)
            {
                *ierr = 5 - flag__;
                freeouttbptr;
                return;
            }
        }
    }

    /*     point-fix iterations */
    flag__ = 6;
    for (i = 1; i <= nblk + 1; ++i)   /*for each block*/
    {
        /*     loop on blocks */
        for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun)
        {
            Blocks[C2F(curblk).kfun - 1].nevprt = 0;
            if (funtyp[C2F(curblk).kfun] >= 0)
            {
                /*callf(told, xd, x, x,g,&flag__);*/
                callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
                if (flag__ < 0)
                {
                    *ierr = 5 - flag__;
                    freeouttbptr;
                    return;
                }
            }
        }

        flag__ = 6;
        for (jj = 1; jj <= ncord; ++jj)   /*for each continuous block*/
        {
            C2F(curblk).kfun = cord[jj];
            if (funtyp[C2F(curblk).kfun] >= 0)
            {
                /*callf(told, xd, x, x,g,&flag__);*/
                callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
                if (flag__ < 0)
                {
                    *ierr = 5 - flag__;
                    freeouttbptr;
                    return;
                }
            }
        }

        /*comparison between outtb and arrays*/
        curouttbd = 0;
        curouttbc = 0;
        curouttbs = 0;
        curouttbl = 0;
        curouttbuc = 0;
        curouttbus = 0;
        curouttbul = 0;
        for (jj = 0; jj < nlnk; jj++)
        {
            switch (outtbtyp[jj]) /*for each type of ports*/
            {
                case SCSREAL_N    :
                    outtbdptr = (SCSREAL_COP *)outtbptr[jj]; /*double real matrix*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        int outtbdptr_isnan = outtbdptr[kk] != outtbdptr[kk];
                        int outtbd_isnan = (SCSREAL_COP)outtbd[curouttbd + kk] != (SCSREAL_COP)outtbd[curouttbd + kk];

                        if (outtbdptr_isnan && outtbd_isnan)
                        {
                            continue;
                        }
                        if (outtbdptr[kk] != (SCSREAL_COP)outtbd[curouttbd + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbd += sszz;
                    break;

                case SCSCOMPLEX_N :
                    outtbdptr = (SCSCOMPLEX_COP *)outtbptr[jj]; /*double complex matrix*/
                    sszz = 2 * outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        int outtbdptr_isnan = outtbdptr[kk] != outtbdptr[kk];
                        int outtbd_isnan = (SCSCOMPLEX_COP)outtbd[curouttbd + kk] != (SCSCOMPLEX_COP)outtbd[curouttbd + kk];

                        if (outtbdptr_isnan && outtbd_isnan)
                        {
                            continue;
                        }
                        if (outtbdptr[kk] != (SCSCOMPLEX_COP)outtbd[curouttbd + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbd += sszz;
                    break;

                case SCSINT8_N    :
                    outtbcptr = (SCSINT8_COP *)outtbptr[jj]; /*int8*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        if (outtbcptr[kk] != (SCSINT8_COP)outtbc[curouttbc + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbc += sszz;
                    break;

                case SCSINT16_N   :
                    outtbsptr = (SCSINT16_COP *)outtbptr[jj]; /*int16*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        if (outtbsptr[kk] != (SCSINT16_COP)outtbs[curouttbs + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbs += sszz;
                    break;

                case SCSINT32_N   :
                    outtblptr = (SCSINT32_COP *)outtbptr[jj]; /*int32*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        if (outtblptr[kk] != (SCSINT32_COP)outtbl[curouttbl + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbl += sszz;
                    break;

                case SCSUINT8_N   :
                    outtbucptr = (SCSUINT8_COP *)outtbptr[jj]; /*uint8*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        if (outtbucptr[kk] != (SCSUINT8_COP)outtbuc[curouttbuc + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbuc += sszz;
                    break;

                case SCSUINT16_N  :
                    outtbusptr = (SCSUINT16_COP *)outtbptr[jj]; /*uint16*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        if (outtbusptr[kk] != (SCSUINT16_COP)outtbus[curouttbus + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbus += sszz;
                    break;

                case SCSUINT32_N  :
                    outtbulptr = (SCSUINT32_COP *)outtbptr[jj]; /*uint32*/
                    sszz = outtbsz[jj] * outtbsz[jj + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        if (outtbulptr[kk] != (SCSUINT32_COP)outtbul[curouttbul + kk])
                        {
                            goto L30;
                        }
                    }
                    curouttbul += sszz;
                    break;

                default  : /* Add a message here */
                    break;
            }
        }
        freeouttbptr;
        return;

L30:
        /*Save data of outtb in arrays*/
        curouttbd = 0;
        curouttbc = 0;
        curouttbs = 0;
        curouttbl = 0;
        curouttbuc = 0;
        curouttbus = 0;
        curouttbul = 0;
        for (ii = 0; ii < nlnk; ii++) /*for each link*/
        {
            switch (outtbtyp[ii])  /*switch to type of outtb object*/
            {
                case SCSREAL_N    :
                    outtbdptr = (SCSREAL_COP *)outtbptr[ii]; /*double real matrix*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    C2F(dcopy)(&sszz, outtbdptr, &c__1, &outtbd[curouttbd], &c__1);
                    curouttbd += sszz;
                    break;

                case SCSCOMPLEX_N :
                    outtbdptr = (SCSCOMPLEX_COP *)outtbptr[ii]; /*double complex matrix*/
                    sszz = 2 * outtbsz[ii] * outtbsz[ii + nlnk];
                    C2F(dcopy)(&sszz, outtbdptr, &c__1, &outtbd[curouttbd], &c__1);
                    curouttbd += sszz;
                    break;

                case SCSINT8_N    :
                    outtbcptr = (SCSINT8_COP *)outtbptr[ii];  /*int8*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        outtbc[curouttbc + kk] = (SCSINT8_COP)outtbcptr[kk];
                    }
                    curouttbc += sszz;
                    break;

                case SCSINT16_N   :
                    outtbsptr = (SCSINT16_COP *)outtbptr[ii]; /*int16*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        outtbs[curouttbs + kk] = (SCSINT16_COP)outtbsptr[kk];
                    }
                    curouttbs += sszz;
                    break;

                case SCSINT32_N   :
                    outtblptr = (SCSINT32_COP *)outtbptr[ii];  /*int32*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        outtbl[curouttbl + kk] = (SCSINT32_COP)outtblptr[kk];
                    }
                    curouttbl += sszz;
                    break;

                case SCSUINT8_N   :
                    outtbucptr = (SCSUINT8_COP *)outtbptr[ii]; /*uint8*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        outtbuc[curouttbuc + kk] = (SCSUINT8_COP)outtbucptr[kk];
                    }
                    curouttbuc += sszz;
                    break;

                case SCSUINT16_N  :
                    outtbusptr = (SCSUINT16_COP *)outtbptr[ii]; /*uint16*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        outtbus[curouttbus + kk] = (SCSUINT16_COP)outtbusptr[kk];
                    }
                    curouttbus += sszz;
                    break;

                case SCSUINT32_N  :
                    outtbulptr = (SCSUINT32_COP *)outtbptr[ii]; /*uint32*/
                    sszz = outtbsz[ii] * outtbsz[ii + nlnk];
                    for (kk = 0; kk < sszz; kk++)
                    {
                        outtbul[curouttbul + kk] = (SCSUINT32_COP)outtbulptr[kk];
                    }
                    curouttbul += sszz;
                    break;

                default  : /* Add a message here */
                    break;
            }
        }
    }
    *ierr = 20;
    freeouttbptr;
} /* cosini_ */

/*--------------------------------------------------------------------------*/
static void cossim(double *told)
{
    /* System generated locals */
    int i3 = 0;

    //** used for the [stop] button
    char* CommandToUnstack;
    static int CommandLength = 0;
    static int SeqSync = 0;
    static int one = 1;

    /* Local variables */
    static scicos_flag flag__ = 0;
    static int ierr1 = 0;
    static int j = 0, k = 0;
    static double t = 0.;
    static int jj = 0;
    static double rhotmp = 0., tstop = 0.;
    static int kpo = 0, kev = 0;
    int Discrete_Jump = 0;
    int *jroot = NULL, *zcros = NULL;
    realtype reltol = 0., abstol = 0.;
    N_Vector y = NULL;
    void *ode_mem = NULL;
    int flag = 0, flagr = 0;
    int cnt = 0;
    /* Saving solver number */
    int solver = C2F(cmsolver).solver;
    /* Defining function pointers, for more readability */
    void(* ODEFree) (void**);
    int (* ODE) (void*, realtype, N_Vector, realtype*, int);
    int (* ODEReInit) (void*, realtype, N_Vector);
    int (* ODESetMaxStep) (void*, realtype);
    int (* ODESetStopTime) (void*, realtype);
    int (* ODEGetRootInfo) (void*, int*);
    int (* ODESStolerances) (void*, realtype, realtype);
    /* Generic flags for stop mode */
    int ODE_NORMAL   = 1;  /* ODE_NORMAL   = CV_NORMAL   = LS_NORMAL   = 1 */
    int ODE_ONE_STEP = 2;  /* ODE_ONE_STEP = CV_ONE_STEP = LS_ONE_STEP = 2 */
    switch (solver)
    {
        case LSodar_Dynamic:
            ODEFree = &LSodarFree;
            ODE = &LSodar;
            ODEReInit = &LSodarReInit;
            ODESetMaxStep = &LSodarSetMaxStep;
            ODESetStopTime = &LSodarSetStopTime;
            ODEGetRootInfo = &LSodarGetRootInfo;
            ODESStolerances = &LSodarSStolerances;
            break;
        case CVode_BDF_Newton:
        case CVode_BDF_Functional:
        case CVode_Adams_Newton:
        case CVode_Adams_Functional:
        case Dormand_Prince:
        case Runge_Kutta:
        case Implicit_Runge_Kutta:
        case Crank_Nicolson:
            ODEFree = &CVodeFree;
            ODE = &CVode;
            ODEReInit = &CVodeReInit;
            ODESetMaxStep = &CVodeSetMaxStep;
            ODESetStopTime = &CVodeSetStopTime;
            ODEGetRootInfo = &CVodeGetRootInfo;
            ODESStolerances = &CVodeSStolerances;
            break;
        default: // Unknown solver number
            *ierr = 1000;
            return;
    }

    jroot = NULL;
    if (ng > 0)
    {
        if ((jroot = MALLOC(sizeof(int) * ng)) == NULL )
        {
            *ierr = 10000;
            return;
        }
    }

    for ( jj = 0 ; jj < ng ; jj++ )
    {
        jroot[jj] = 0 ;
    }

    zcros = NULL;
    if (ng > 0)
    {
        if ((zcros = MALLOC(sizeof(int) * ng)) == NULL )
        {
            *ierr = 10000;
            if (ng > 0)
            {
                FREE(jroot);
            }
            return;
        }
    }

    reltol = (realtype) rtol;
    abstol = (realtype) Atol;  /* Ith(abstol,1) = realtype) Atol;*/

    if (*neq > 0) /* Unfortunately CVODE does not work with NEQ==0 */
    {
        y = N_VNewEmpty_Serial(*neq);
        if (check_flag((void *)y, "N_VNewEmpty_Serial", 0))
        {
            *ierr = 10000;
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            return;
        }

        NV_DATA_S(y) = x;

        ode_mem = NULL;

        /* Set extension of Sundials for scicos */
        set_sundials_with_extension(TRUE);

        switch (solver)
        {
            case LSodar_Dynamic:
                ode_mem = LSodarCreate(neq, ng); /* Create the lsodar problem */
                break;
            case CVode_BDF_Newton:
                ode_mem = CVodeCreate(CV_BDF, CV_NEWTON);
                break;
            case CVode_BDF_Functional:
                ode_mem = CVodeCreate(CV_BDF, CV_FUNCTIONAL);
                break;
            case CVode_Adams_Newton:
                ode_mem = CVodeCreate(CV_ADAMS, CV_NEWTON);
                break;
            case CVode_Adams_Functional:
                ode_mem = CVodeCreate(CV_ADAMS, CV_FUNCTIONAL);
                break;
            case Dormand_Prince:
                ode_mem = CVodeCreate(CV_DOPRI, CV_FUNCTIONAL);
                break;
            case Runge_Kutta:
                ode_mem = CVodeCreate(CV_ExpRK, CV_FUNCTIONAL);
                break;
            case Implicit_Runge_Kutta:
                ode_mem = CVodeCreate(CV_ImpRK, CV_FUNCTIONAL);
                break;
            case Crank_Nicolson:
                ode_mem = CVodeCreate(CV_CRANI, CV_FUNCTIONAL);
                break;
        }

        /*    ode_mem = CVodeCreate(CV_ADAMS, CV_FUNCTIONAL);*/

        if (check_flag((void *)ode_mem, "CVodeCreate", 0))
        {
            *ierr = 10000;
            N_VDestroy_Serial(y);
            FREE(jroot);
            FREE(zcros);
            return;
        }

        if (solver == LSodar_Dynamic)
        {
            flag = LSodarSetErrHandlerFn(ode_mem, SundialsErrHandler, NULL);
        }
        else
        {
            flag = CVodeSetErrHandlerFn(ode_mem, SundialsErrHandler, NULL);
        }
        if (check_flag(&flag, "CVodeSetErrHandlerFn", 1))
        {
            *ierr = 300 + (-flag);
            freeall
            return;
        }

        if (solver == LSodar_Dynamic)
        {
            flag = LSodarInit(ode_mem, simblklsodar, T0, y);
        }
        else
        {
            flag = CVodeInit (ode_mem, simblk, T0, y);
        }
        if (check_flag(&flag, "CVodeInit", 1))
        {
            *ierr = 300 + (-flag);
            freeall
            return;
        }

        flag = ODESStolerances(ode_mem, reltol, abstol);
        if (check_flag(&flag, "CVodeSStolerances", 1))
        {
            *ierr = 300 + (-flag);
            freeall
            return;
        }

        if (solver == LSodar_Dynamic)
        {
            flag = LSodarRootInit(ode_mem, ng, grblklsodar);
        }
        else
        {
            flag = CVodeRootInit(ode_mem, ng, grblk);
        }
        if (check_flag(&flag, "CVodeRootInit", 1))
        {
            *ierr = 300 + (-flag);
            freeall
            return;
        }

        /* Call CVDense to specify the CVDENSE dense linear solver, only for solvers needing CVode's Newton method */
        if (solver == CVode_BDF_Newton || solver == CVode_Adams_Newton)
        {
            flag = CVDense(ode_mem, *neq);
        }
        if (check_flag(&flag, "CVDense", 1))
        {
            *ierr = 300 + (-flag);
            freeall
            return;
        }

        if (hmax > 0)
        {
            flag = ODESetMaxStep(ode_mem, (realtype) hmax);
            if (check_flag(&flag, "CVodeSetMaxStep", 1))
            {
                *ierr = 300 + (-flag);
                freeall;
                return;
            }
        }
        /* Set the Jacobian routine to Jac (user-supplied)
        flag = CVDlsSetDenseJacFn(ode_mem, Jac);
        if (check_flag(&flag, "CVDlsSetDenseJacFn", 1)) return(1);  */

    }/* testing if neq>0 */

    /* Function Body */
    C2F(coshlt).halt = 0;
    *ierr = 0;

    /*     initialization */
    C2F(realtimeinit)(told, &C2F(rtfactor).scale);

    phase = 1;
    hot = 0;

    jj = 0;
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun)
    {
        if (Blocks[C2F(curblk).kfun - 1].ng >= 1)
        {
            zcros[jj] = C2F(curblk).kfun;
            ++jj;
        }
    }
    /*     . ng >= jj required */
    if (jj != ng)
    {
        zcros[jj] = -1;
    }
    /*     initialization (propagation of constant blocks outputs) */
    idoit(told);
    if (*ierr != 0)
    {
        freeall;
        return;
    }
    /*--discrete zero crossings----dzero--------------------*/
    if (ng > 0) /* storing ZC signs just after a solver call*/
    {
        /*zdoit(told, g, x, x);*/
        zdoit(told, x, x, g);
        if (*ierr != 0)
        {
            freeall;
            return;
        }
        for (jj = 0; jj < ng; ++jj)
            if (g[jj] >= 0)
            {
                jroot[jj] = 5;
            }
            else
            {
                jroot[jj] = -5;
            }
    }
    /*--discrete zero crossings----dzero--------------------*/

    /*     main loop on time */
    while (*told < *tf)
    {
        while (ismenu()) //** if the user has done something, do the actions
        {
            int ierr2 = 0;
            int iUnused;
            GetCommand(&CommandToUnstack, &SeqSync, &iUnused, NONE); //** get to the action
            CommandLength = (int)strlen(CommandToUnstack);
            //syncexec(CommandToUnstack, &CommandLength, &ierr2, &one, CommandLength); //** execute it
            FREE(CommandToUnstack);
        }
        if (C2F(coshlt).halt != 0)
        {
            if (C2F(coshlt).halt == 2)
            {
                *told = *tf;    /* end simulation */
            }
            C2F(coshlt).halt = 0;
            freeall;
            return;
        }
        if (*pointi == 0)
        {
            t = *tf;
        }
        else
        {
            t = tevts[*pointi];
        }
        if (fabs(t - *told) < ttol)
        {
            t = *told;
            /*     update output part */
        }
        if (*told > t)
        {
            /*     !  scheduling problem */
            *ierr = 1;
            freeall;
            return;
        }
        if (*told != t)
        {
            if (xptr[nblk + 1] == 1)
            {
                /*     .     no continuous state */
                if (*told + deltat + ttol > t)
                {
                    *told = t;
                }
                else
                {
                    *told += deltat;
                }
                /*     .     update outputs of 'c' type blocks with no continuous state */
                if (*told >= *tf)
                {
                    /*     .     we are at the end, update continuous part before leaving */
                    if (ncord > 0)
                    {
                        cdoit(told);
                        freeall;
                        return;
                    }
                }
            }
            else
            {
                /*     integrate */
                rhotmp = *tf + ttol;
                if (*pointi != 0)
                {
                    kpo = *pointi;
L20:
                    if (critev[kpo] == 1)
                    {
                        rhotmp = tevts[kpo];
                        goto L30;
                    }
                    kpo = evtspt[kpo];
                    if (kpo != 0)
                    {
                        goto L20;
                    }
L30:
                    if (rhotmp < tstop)
                    {
                        hot = 0;
                    }
                }
                tstop = rhotmp;
                t = Min(*told + deltat, Min(t, *tf + ttol));

                if (ng > 0 &&  hot == 0 && nmod > 0)
                {
                    zdoit(told, x, x, g);
                    if (*ierr != 0)
                    {
                        freeall;
                        return;
                    }
                }

                if (hot == 0) /* hot==0 : cold restart*/
                {
                    flag = ODESetStopTime(ode_mem, (realtype)tstop);  /* Setting the stop time*/
                    if (check_flag(&flag, "CVodeSetStopTime", 1))
                    {
                        *ierr = 300 + (-flag);
                        freeall;
                        return;
                    }

                    flag = ODEReInit(ode_mem, (realtype)(*told), y);
                    if (check_flag(&flag, "CVodeReInit", 1))
                    {
                        *ierr = 300 + (-flag);
                        freeall;
                        return;
                    }
                }

                if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                {
                    sciprint(_("****SUNDIALS.Cvode from: %f to %f hot= %d  \n"), *told, t, hot);
                }

                /*--discrete zero crossings----dzero--------------------*/
                /*--check for Dzeros after Mode settings or ddoit()----*/
                Discrete_Jump = 0;

                if (ng > 0 && hot == 0)
                {
                    zdoit(told, x, x, g);
                    if (*ierr != 0)
                    {
                        freeall;
                        return;
                    }
                    for (jj = 0; jj < ng; ++jj)
                    {
                        if ((g[jj] >= 0.0) && (jroot[jj] == -5))
                        {
                            Discrete_Jump = 1;
                            jroot[jj] = 1;
                        }
                        else if ((g[jj] < 0.0) && (jroot[jj] == 5))
                        {
                            Discrete_Jump = 1;
                            jroot[jj] = -1;
                        }
                        else
                        {
                            jroot[jj] = 0;
                        }
                    }
                }
                /*--discrete zero crossings----dzero--------------------*/

                if (Discrete_Jump == 0) /* if there was a dzero, its event should be activated*/
                {
                    phase = 2;
                    flag = ODE(ode_mem, t, y, told, ODE_NORMAL);
                    if (*ierr != 0)
                    {
                        freeall;
                        return;
                    }
                    phase = 1;
                }
                else
                {
                    flag = CV_ROOT_RETURN; /* in order to handle discrete jumps */
                }

                /*     .     update outputs of 'c' type  blocks if we are at the end*/
                if (*told >= *tf)
                {
                    if (ncord > 0)
                    {
                        cdoit(told);
                        freeall;
                        return;
                    }
                }

                if (flag >= 0)
                {
                    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                    {
                        sciprint(_("****SUNDIALS.Cvode reached: %f\n"), *told);
                    }
                    hot = 1;
                    cnt = 0;
                }
                else if ( flag == CV_TOO_MUCH_WORK ||  flag == CV_CONV_FAILURE || flag == CV_ERR_FAILURE)
                {
                    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                    {
                        sciprint(_("****SUNDIALS.Cvode: too much work at time=%g (stiff region, change RTOL and ATOL)\n"), *told);
                    }
                    hot = 0;
                    cnt++;
                    if (cnt > 5)
                    {
                        *ierr = 300 + (-flag);
                        freeall;
                        return;
                    }
                }
                else
                {
                    if (flag < 0)
                    {
                        *ierr = 300 + (-flag);    /* raising errors due to internal errors, otherwise error due to flagr*/
                    }
                    freeall;
                    return;
                }

                if (flag == CV_ZERO_DETACH_RETURN)
                {
                    hot = 0;
                };  /* new feature of sundials, detects zero-detaching */

                if (flag == CV_ROOT_RETURN)
                {
                    /*     .        at a least one root has been found */
                    hot = 0;
                    if (Discrete_Jump == 0)
                    {
                        flagr = ODEGetRootInfo(ode_mem, jroot);
                        if (check_flag(&flagr, "CVodeGetRootInfo", 1))
                        {
                            *ierr = 300 + (-flagr);
                            freeall;
                            return;
                        }
                    }
                    /*     .        at a least one root has been found */
                    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                    {
                        sciprint(_("root found at t=: %f\n"), *told);
                    }
                    /*     .        update outputs affecting ztyp blocks ONLY FOR OLD BLOCKS */
                    zdoit(told, x, xd, g);
                    if (*ierr != 0)
                    {
                        freeall;
                        return;
                    }
                    for (jj = 0; jj < ng; ++jj)
                    {
                        C2F(curblk).kfun = zcros[ jj];
                        if (C2F(curblk).kfun == -1)
                        {
                            break;
                        }
                        kev = 0;

                        for (j = zcptr[C2F(curblk).kfun] - 1 ;
                                j < zcptr[C2F(curblk).kfun + 1] - 1 ; ++j)
                        {
                            if (jroot[j] != 0)
                            {
                                kev = 1;
                                break;
                            }
                        }
                        /*   */
                        if (kev != 0)
                        {
                            Blocks[C2F(curblk).kfun - 1].jroot = &jroot[zcptr[C2F(curblk).kfun] - 1];
                            if (funtyp[C2F(curblk).kfun] > 0)
                            {

                                if (Blocks[C2F(curblk).kfun - 1].nevout > 0)
                                {
                                    flag__ = 3;
                                    if (Blocks[C2F(curblk).kfun - 1].nx > 0)
                                    {
                                        Blocks[C2F(curblk).kfun - 1].x  = &x[xptr[C2F(curblk).kfun] - 1];
                                        Blocks[C2F(curblk).kfun - 1].xd = &xd[xptr[C2F(curblk).kfun] - 1];
                                    }
                                    /* call corresponding block to determine output event (kev) */
                                    Blocks[C2F(curblk).kfun - 1].nevprt = -kev;
                                    /*callf(told, xd, x, x,g,&flag__);*/
                                    callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
                                    if (flag__ < 0)
                                    {
                                        *ierr = 5 - flag__;
                                        freeall;
                                        return;
                                    }
                                    /*     .              update event agenda */
                                    for (k = 0; k < Blocks[C2F(curblk).kfun - 1].nevout; ++k)
                                    {
                                        if (Blocks[C2F(curblk).kfun - 1].evout[k] >= 0.)
                                        {
                                            i3 = k + clkptr[C2F(curblk).kfun] ;
                                            addevs(Blocks[C2F(curblk).kfun - 1].evout[k] + (*told), &i3, &ierr1);
                                            if (ierr1 != 0)
                                            {
                                                /*     .                       nevts too small */
                                                *ierr = 3;
                                                freeall;
                                                return;
                                            }
                                        }
                                    }
                                }
                                /*     .              update state */
                                if (Blocks[C2F(curblk).kfun - 1].nx > 0)
                                {
                                    /*     .              call corresponding block to update state */
                                    flag__ = 2;
                                    Blocks[C2F(curblk).kfun - 1].x  = &x[xptr[C2F(curblk).kfun] - 1];
                                    Blocks[C2F(curblk).kfun - 1].xd = &xd[xptr[C2F(curblk).kfun] - 1];
                                    Blocks[C2F(curblk).kfun - 1].nevprt = -kev;
                                    /*callf(told, xd, x, x,g,&flag__);*/
                                    callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);

                                    if (flag__ < 0)
                                    {
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
            if (ng > 0) /* storing ZC signs just after a sundials call*/
            {
                zdoit(told, x, x, g);
                if (*ierr != 0)
                {
                    freeall;
                    return;
                }
                for (jj = 0; jj < ng; ++jj)
                {
                    if (g[jj] >= 0)
                    {
                        jroot[jj] = 5;
                    }
                    else
                    {
                        jroot[jj] = -5;
                    }
                }
            }
            /*--discrete zero crossings----dzero--------------------*/

            C2F(realtime)(told);
        }
        else
        {
            /*     .  t==told */
            if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
            {
                sciprint(_("Event: %d activated at t=%f\n"), *pointi, *told);
                for (kev = 0; kev < nblk; kev++)
                {
                    if (Blocks[kev].nmode > 0)
                    {
                        sciprint(_("mode of block %d=%d, "), kev, Blocks[kev].mode[0]);
                    }
                }
                sciprint(_("**mod**\n"));
            }

            ddoit(told);
            if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
            {
                sciprint(_("End of activation\n"));
            }
            if (*ierr != 0)
            {
                freeall;
                return;
            }

        }
        /*     end of main loop on time */
    }
    freeall;
} /* cossim_ */

/*--------------------------------------------------------------------------*/
static void cossimdaskr(double *told)
{
    /* System generated locals */
    int i3;
    //** used for the [stop] button
    char* CommandToUnstack;
    static int CommandLength = 0;
    static int SeqSync = 0;
    static int one = 1;

    /* Local variables */
    static scicos_flag flag__ = 0;
    static int ierr1 = 0;
    static int j = 0, k = 0;
    static double t = 0.;
    static int jj = 0;
    static double rhotmp = 0., tstop = 0.;
    static int kpo = 0, kev = 0;

    int *jroot = NULL, *zcros = NULL;
    int *Mode_save = NULL;
    int Mode_change = 0;

    int flag = 0, flagr = 0;
    N_Vector   yy = NULL, yp = NULL;
    realtype reltol = 0., abstol = 0.;
    int Discrete_Jump = 0;
    N_Vector IDx = NULL;
    realtype *scicos_xproperty = NULL;
    DlsMat TJacque = NULL;

    void *dae_mem = NULL;
    UserData data = NULL;
    IDAMem copy_IDA_mem = NULL;
    int maxnj = 0, maxnit = 0, maxnh = 0;
    /*-------------------- Analytical Jacobian memory allocation ----------*/
    int  Jn = 0, Jnx = 0, Jno = 0, Jni = 0, Jactaille = 0;
    double uround = 0.;
    int cnt = 0, N_iters = 0;
    /* Saving solver number */
    int solver = C2F(cmsolver).solver;
    /* Flags for initial values calculation */
    int DAE_YA_YDP_INIT = 1;
    int DAE_Y_INIT      = 2;
    /* Defining function pointers, for more readability*/
    void(* DAEFree) (void**);
    int (* DAESolve) (void*, realtype, realtype*, N_Vector, N_Vector, int);
    int (* DAEReInit) (void*, realtype, N_Vector, N_Vector);
    int (* DAESetId) (void*, N_Vector);
    int (* DAECalcIC) (void*, int, realtype);
    int (* DAESetMaxStep) (void*, realtype);
    int (* DAESetUserData) (void*, void*);
    int (* DAESetStopTime) (void*, realtype);
    int (* DAEGetRootInfo) (void*, int*);
    int (* DAESStolerances) (void*, realtype, realtype);
    int (* DAEGetConsistentIC) (void*, N_Vector, N_Vector);
    int (* DAESetMaxNumSteps) (void*, long int);
    int (* DAESetMaxNumJacsIC) (void*, int);
    int (* DAESetMaxNumItersIC) (void*, int);
    int (* DAESetMaxNumStepsIC) (void*, int);
    int (* DAESetLineSearchOffIC) (void*, int);
    /* For DAEs, the generic flags for stop mode depend on the used solver */
    int DAE_NORMAL = 0, DAE_ONE_STEP = 0;
    DAE_NORMAL   = (solver == IDA_BDF_Newton) ? 1 : 0;  /* IDA_NORMAL   = 1, DDAS_NORMAL   = 0 */
    DAE_ONE_STEP = (solver == IDA_BDF_Newton) ? 2 : 1;  /* IDA_ONE_STEP = 2, DDAS_ONE_STEP = 1 */
    switch (solver)
    {
        case IDA_BDF_Newton:
            DAEFree = &IDAFree;
            DAESolve = &IDASolve;
            DAESetId = &IDASetId;
            DAEReInit = &IDAReInit;
            DAECalcIC = &IDACalcIC;
            DAESetMaxStep = &IDASetMaxStep;
            DAESetUserData = &IDASetUserData;
            DAESetStopTime = &IDASetStopTime;
            DAEGetRootInfo = &IDAGetRootInfo;
            DAESStolerances = &IDASStolerances;
            DAESetMaxNumSteps = &IDASetMaxNumSteps;
            DAEGetConsistentIC = &IDAGetConsistentIC;
            DAESetMaxNumJacsIC = &IDASetMaxNumJacsIC;
            DAESetMaxNumItersIC = &IDASetMaxNumItersIC;
            DAESetMaxNumStepsIC = &IDASetMaxNumStepsIC;
            DAESetLineSearchOffIC = &IDASetLineSearchOffIC;
            break;
        case DDaskr_BDF_Newton:
        case DDaskr_BDF_GMRes:
            DAEFree = &DDaskrFree;
            DAESolve = &DDaskrSolve;
            DAESetId = &DDaskrSetId;
            DAEReInit = &DDaskrReInit;
            DAECalcIC = &DDaskrCalcIC;
            DAESetMaxStep = &DDaskrSetMaxStep;
            DAESetUserData = &DDaskrSetUserData;
            DAESetStopTime = &DDaskrSetStopTime;
            DAEGetRootInfo = &DDaskrGetRootInfo;
            DAESStolerances = &DDaskrSStolerances;
            DAESetMaxNumSteps = &DDaskrSetMaxNumSteps;
            DAEGetConsistentIC = &DDaskrGetConsistentIC;
            DAESetMaxNumJacsIC = &DDaskrSetMaxNumJacsIC;
            DAESetMaxNumItersIC = &DDaskrSetMaxNumItersIC;
            DAESetMaxNumStepsIC = &DDaskrSetMaxNumStepsIC;
            DAESetLineSearchOffIC = &DDaskrSetLineSearchOffIC;
            break;
        default: // Unknown solver number
            *ierr = 1000;
            return;
    }

    /* Set extension of Sundials for scicos */
    set_sundials_with_extension(TRUE);

    // CI=1.0;   /* for function Get_Jacobian_ci */
    jroot = NULL;
    if (ng > 0)
    {
        if ((jroot = MALLOC(sizeof(int) * ng)) == NULL )
        {
            *ierr = 10000;
            return;
        }
    }
    for ( jj = 0 ; jj < ng ; jj++ )
    {
        jroot[jj] = 0 ;
    }

    zcros = NULL;
    if (ng > 0)
    {
        if ((zcros = MALLOC(sizeof(int) * ng)) == NULL )
        {
            *ierr = 10000;
            if (ng > 0)
            {
                FREE(jroot);
            }
            return;
        }
    }

    Mode_save = NULL;
    if (nmod > 0)
    {
        if ((Mode_save = MALLOC(sizeof(int) * nmod)) == NULL )
        {
            *ierr = 10000;
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            return;
        }
    }

    reltol = (realtype) rtol;
    abstol = (realtype) Atol;  /*  Ith(abstol,1) = (realtype) Atol;*/

    if (*neq > 0)
    {
        yy = NULL;
        yy = N_VNewEmpty_Serial(*neq);
        if (check_flag((void *)yy, "N_VNew_Serial", 0))
        {
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
        }
        NV_DATA_S(yy) = x;

        yp = NULL;
        yp = N_VNewEmpty_Serial(*neq);
        if (check_flag((void *)yp, "N_VNew_Serial", 0))
        {
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }
        NV_DATA_S(yp) = xd;

        IDx = NULL;
        IDx = N_VNew_Serial(*neq);
        if (check_flag((void *)IDx, "N_VNew_Serial", 0))
        {
            *ierr = 10000;
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }

        /* Call the Create and Init functions to initialize DAE memory */
        dae_mem = NULL;
        if (solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes)
        {
            dae_mem = DDaskrCreate(neq, ng, solver);
        }
        else
        {
            dae_mem = IDACreate();
        }
        if (check_flag((void *)dae_mem, "IDACreate", 0))
        {
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }
        if (C2F(cmsolver).solver == 100)
        {
            copy_IDA_mem = (IDAMem) dae_mem;
        }

        if (solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes)
        {
            flag = DDaskrSetErrHandlerFn(dae_mem, SundialsErrHandler, NULL);
        }
        else
        {
            flag = IDASetErrHandlerFn(dae_mem, SundialsErrHandler, NULL);
        }
        if (check_flag(&flag, "IDASetErrHandlerFn", 1))
        {
            *ierr = 200 + (-flag);
            if (*neq > 0)
            {
                DAEFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }

        if (solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes)
        {
            flag = DDaskrInit(dae_mem, simblkddaskr, T0, yy, yp, jacpsol, psol);
        }
        else
        {
            flag = IDAInit(dae_mem, simblkdaskr, T0, yy, yp);
        }
        if (check_flag(&flag, "IDAInit", 1))
        {
            *ierr = 200 + (-flag);
            if (*neq > 0)
            {
                DAEFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }

        flag = DAESStolerances(dae_mem, reltol, abstol);
        if (check_flag(&flag, "IDASStolerances", 1))
        {
            *ierr = 200 + (-flag);
            if (*neq > 0)
            {
                DAEFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }

        if (solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes)
        {
            flag = DDaskrRootInit(dae_mem, ng, grblkddaskr);
        }
        else
        {
            flag = IDARootInit(dae_mem, ng, grblkdaskr);
        }
        if (check_flag(&flag, "IDARootInit", 1))
        {
            *ierr = 200 + (-flag);
            if (*neq > 0)
            {
                DAEFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }

        if (solver == IDA_BDF_Newton)
        {
            flag = IDADense(dae_mem, *neq);
        }
        if (check_flag(&flag, "IDADense", 1))
        {
            *ierr = 200 + (-flag);
            if (*neq > 0)if (solver == IDA_BDF_Newton)
                {
                    IDAFree(&dae_mem);
                }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }

        data = NULL;
        if ((data = (UserData) MALLOC(sizeof(*data))) == NULL)
        {
            *ierr = 10000;
            if (*neq > 0)
            {
                IDAFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }
        data->dae_mem = dae_mem;
        data->ewt   = NULL;
        data->iwork = NULL;
        data->rwork = NULL;
        data->gwork = NULL;

        data->ewt   = N_VNew_Serial(*neq);
        if (check_flag((void *)data->ewt, "N_VNew_Serial", 0))
        {
            *ierr = 200 + (-flag);
            if (*neq > 0)
            {
                FREE(data);
            }
            if (*neq > 0)
            {
                DAEFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            return;
        }
        if ( ng > 0 )
        {
            if ((data->gwork = (double *) MALLOC(ng * sizeof(double))) == NULL)
            {
                if (*neq > 0)
                {
                    N_VDestroy_Serial(data->ewt);
                }
                if (*neq > 0)
                {
                    FREE(data);
                }
                if (*neq > 0)
                {
                    DAEFree(&dae_mem);
                }
                if (*neq > 0)
                {
                    N_VDestroy_Serial(IDx);
                }
                if (*neq > 0)
                {
                    N_VDestroy_Serial(yp);
                }
                if (*neq > 0)
                {
                    N_VDestroy_Serial(yy);
                }
                if (ng > 0)
                {
                    FREE(jroot);
                }
                if (ng > 0)
                {
                    FREE(zcros);
                }
                if (nmod > 0)
                {
                    FREE(Mode_save);
                }
                return;
            }
        }
        /*Jacobian_Flag=0; */
        if (AJacobian_block > 0) /* set by the block with A-Jac in flag-4 using Set_Jacobian_flag(1); */
        {
            Jn = *neq;
            Jnx = Blocks[AJacobian_block - 1].nx;
            Jno = Blocks[AJacobian_block - 1].nout;
            Jni = Blocks[AJacobian_block - 1].nin;
        }
        else
        {
            Jn = *neq;
            Jnx = 0;
            Jno = 0;
            Jni = 0;
        }
        Jactaille = 3 * Jn + (Jn + Jni) * (Jn + Jno) + Jnx * (Jni + 2 * Jn + Jno) + (Jn - Jnx) * (2 * (Jn - Jnx) + Jno + Jni) + 2 * Jni * Jno;

        if ((data->rwork = (double *) MALLOC(Jactaille * sizeof(double))) == NULL)
        {
            if ( ng > 0 )
            {
                FREE(data->gwork);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(data->ewt);
            }
            if (*neq > 0)
            {
                FREE(data);
            }
            if (*neq > 0)
            {
                DAEFree(&dae_mem);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(IDx);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yp);
            }
            if (*neq > 0)
            {
                N_VDestroy_Serial(yy);
            }
            if (ng > 0)
            {
                FREE(jroot);
            }
            if (ng > 0)
            {
                FREE(zcros);
            }
            if (nmod > 0)
            {
                FREE(Mode_save);
            }
            *ierr = 10000;
            return;
        }

        if (solver == IDA_BDF_Newton)
        {
            flag = IDADlsSetDenseJacFn(dae_mem, Jacobians);
        }
        if (check_flag(&flag, "IDADlsSetDenseJacFn", 1))
        {
            *ierr = 200 + (-flag);
            freeallx
            return;
        }

        TJacque = (DlsMat) NewDenseMat(*neq, *neq);

        flag = DAESetUserData(dae_mem, data);
        if (check_flag(&flag, "IDASetUserData", 1))
        {
            *ierr = 200 + (-flag);
            freeallx
            return;
        }

        if (hmax > 0)
        {
            flag = DAESetMaxStep(dae_mem, (realtype) hmax);
            if (check_flag(&flag, "IDASetMaxStep", 1))
            {
                *ierr = 200 + (-flag);
                freeallx
                return;
            }
        }

        maxnj = 100; /* setting the maximum number of Jacobian evaluations during a Newton step */
        flag = DAESetMaxNumJacsIC(dae_mem, maxnj);
        if (check_flag(&flag, "IDASetMaxNumJacsIC", 1))
        {
            *ierr = 200 + (-flag);
            freeallx
            return;
        }

        maxnit = 10; /* setting the maximum number of Newton iterations in any attempt to solve CIC */
        if (C2F(cmsolver).solver == 102)
        {
            maxnit = 15;    /* By default, the Krylov max iterations should be 15 */
        }
        flag = DAESetMaxNumItersIC(dae_mem, maxnit);
        if (check_flag(&flag, "IDASetMaxNumItersIC", 1))
        {
            *ierr = 200 + (-flag);
            freeallx
            return;
        }

        /* setting the maximum number of steps in an integration interval */
        maxnh = 2000;
        flag = DAESetMaxNumSteps(dae_mem, maxnh);
        if (check_flag(&flag, "IDASetMaxNumSteps", 1))
        {
            *ierr = 200 + (-flag);
            freeallx
            return;
        }

    } /* testing if neq>0 */

    uround = 1.0;
    do
    {
        uround = uround * 0.5;
    }
    while ( 1.0 + uround != 1.0);
    uround = uround * 2.0;
    SQuround = sqrt(uround);
    /* Function Body */

    C2F(coshlt).halt = 0;
    *ierr = 0;
    /*     hot = .false. */
    phase = 1;
    hot = 0;

    /*     initialization */
    C2F(realtimeinit)(told, &C2F(rtfactor).scale);
    /*     ATOL and RTOL are scalars */

    jj = 0;
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun)
    {
        if (Blocks[C2F(curblk).kfun - 1].ng >= 1)
        {
            zcros[jj] = C2F(curblk).kfun;
            ++jj;
        }
    }
    /*     . ng >= jj required */
    if (jj != ng)
    {
        zcros[jj] = -1;
    }
    /*     initialization (propagation of constant blocks outputs) */
    idoit(told);
    if (*ierr != 0)
    {
        freeallx;
        return;
    }

    /*--discrete zero crossings----dzero--------------------*/
    if (ng > 0) /* storing ZC signs just after a solver call*/
    {
        zdoit(told, x, x, g);
        if (*ierr != 0)
        {
            freeallx;
            return;
        }
        for (jj = 0; jj < ng; ++jj)
            if (g[jj] >= 0)
            {
                jroot[jj] = 5;
            }
            else
            {
                jroot[jj] = -5;
            }
    }
    /*     main loop on time */
    while (*told < *tf)
    {
        while (ismenu()) //** if the user has done something, do the actions
        {
            int ierr2 = 0;
            int iUnused;
            GetCommand(&CommandToUnstack, &SeqSync, &iUnused, NONE); //** get to the action
            CommandLength = (int)strlen(CommandToUnstack);
            //syncexec(CommandToUnstack, &CommandLength, &ierr2, &one, CommandLength); //** execute it
            FREE(CommandToUnstack);
        }
        if (C2F(coshlt).halt != 0)
        {
            if (C2F(coshlt).halt == 2)
            {
                *told = *tf;    /* end simulation */
            }
            C2F(coshlt).halt = 0;
            freeallx;
            return;
        }
        if (*pointi == 0)
        {
            t = *tf;
        }
        else
        {
            t = tevts[*pointi];
        }
        if (fabs(t - *told) < ttol)
        {
            t = *told;
            /*     update output part */
        }
        if (*told > t)
        {
            /*     !  scheduling problem */
            *ierr = 1;
            freeallx;
            return;
        }
        if (*told != t)
        {
            if (xptr[nblk + 1] == 1)
            {
                /*     .     no continuous state */
                if (*told + deltat + ttol > t)
                {
                    *told = t;
                }
                else
                {
                    *told += deltat;
                }
                /*     .     update outputs of 'c' type blocks with no continuous state */
                if (*told >= *tf)
                {
                    /*     .     we are at the end, update continuous part before leaving */
                    cdoit(told);
                    freeallx;
                    return;
                }
            }
            else
            {
                rhotmp = *tf + ttol;
                if (*pointi != 0)
                {
                    kpo = *pointi;
L20:
                    if (critev[kpo] == 1)
                    {
                        rhotmp = tevts[kpo];
                        goto L30;
                    }
                    kpo = evtspt[kpo];
                    if (kpo != 0)
                    {
                        goto L20;
                    }
L30:
                    if (rhotmp < tstop)
                    {
                        hot = 0;/* Cold-restart the solver if the new TSTOP isn't beyong the previous one*/
                    }
                }
                tstop = rhotmp;
                t = Min(*told + deltat, Min(t, *tf + ttol));

                if (hot == 0)  /* CIC calculation when hot==0 */
                {

                    /* Setting the stop time*/
                    flag = DAESetStopTime(dae_mem, (realtype)tstop);
                    if (check_flag(&flag, "IDASetStopTime", 1))
                    {
                        *ierr = 200 + (-flag);
                        freeallx;
                        return;
                    }

                    if (ng > 0 && nmod > 0)
                    {
                        phase = 1;
                        zdoit(told, x, xd, g);
                        if (*ierr != 0)
                        {
                            freeallx;
                            return;
                        }
                    }

                    /*----------ID setting/checking------------*/
                    N_VConst(ONE, IDx); /* Initialize id to 1's. */
                    scicos_xproperty = NV_DATA_S(IDx);
                    reinitdoit(told);
                    if (*ierr > 0)
                    {
                        freeallx;
                        return;
                    }
                    for (jj = 0; jj < *neq; jj++)
                    {
                        if (xprop[jj] ==  1)
                        {
                            scicos_xproperty[jj] = ONE;
                        }
                        if (xprop[jj] == -1)
                        {
                            scicos_xproperty[jj] = ZERO;
                        }
                    }
                    /* CI=0.0;CJ=100.0; // for functions Get_Jacobian_ci and Get_Jacobian_cj
                    Jacobians(*neq, (realtype) (*told), yy, yp,	bidon, (realtype) CJ, data, TJacque, tempv1, tempv2, tempv3);
                    for (jj=0;jj<*neq;jj++){
                    Jacque_col=DENSE_COL(TJacque,jj);
                    CI=ZERO;
                    for (kk=0;kk<*neq;kk++){
                    if ((Jacque_col[kk]-Jacque_col[kk]!=0)) {
                    CI=-ONE;
                    break;
                    }else{
                    if (Jacque_col[kk]!=0){
                    CI=ONE;
                    break;
                    }
                    }
                    }
                    if (CI>=ZERO){  scicos_xproperty[jj]=CI;}else{fprintf(stderr,"\nWarinng! Xproperties are not match for i=%d!",jj);}
                    } */
                    /* printf("\n"); for(jj=0;jj<*neq;jj++) { printf("x%d=%g ",jj,scicos_xproperty[jj]); }*/

                    flag = DAESetId(dae_mem, IDx);
                    if (check_flag(&flag, "IDASetId", 1))
                    {
                        *ierr = 200 + (-flag);
                        freeallx
                        return;
                    }
                    // CI=1.0;  // for function Get_Jacobian_ci
                    /*--------------------------------------------*/
                    // maxnj=100; /* setting the maximum number of Jacobian evaluation during a Newton step */
                    // flag=DAESetMaxNumJacsIC(dae_mem, maxnj);
                    // if (check_flag(&flag, "IDASetMaxNumJacsIC", 1)) {
                    //   *ierr=200+(-flag);
                    //   freeallx;
                    //   return;
                    // };
                    // flag=DAESetLineSearchOffIC(dae_mem,FALSE);  /* (def=false)  */
                    // if (check_flag(&flag, "IDASetLineSearchOffIC", 1)) {
                    //   *ierr=200+(-flag);
                    //   freeallx;
                    //   return;
                    // };
                    // flag=DAESetMaxNumItersIC(dae_mem, 10);/* (def=10) setting the maximum number of Newton iterations in any attempt to solve CIC */
                    // if (check_flag(&flag, "IDASetMaxNumItersIC", 1)) {
                    //   *ierr=200+(-flag);
                    //   freeallx;
                    //   return;
                    // };

                    N_iters = 4 + nmod * 4;
                    for (j = 0; j <= N_iters; j++)
                    {
                        /* counter to reevaluate the
                        						modes in  mode->CIC->mode->CIC-> loop
                        						do it once in the absence of mode (nmod=0) */
                        /* updating the modes through Flag==9, Phase==1 */

                        /* Serge Steer 29/06/2009 */
                        while (ismenu()) //** if the user has done something, do the actions
                        {
                            int ierr2 = 0;
                            int iUnused;
                            GetCommand(&CommandToUnstack, &SeqSync, &iUnused, NONE); //** get to the action
                            CommandLength = (int)strlen(CommandToUnstack);
                            //syncexec(CommandToUnstack, &CommandLength, &ierr2, &one, CommandLength); //** execute it
                            FREE(CommandToUnstack);
                        }
                        if (C2F(coshlt).halt != 0)
                        {
                            if (C2F(coshlt).halt == 2)
                            {
                                *told = *tf;    /* end simulation */
                            }
                            C2F(coshlt).halt = 0;
                            freeallx;
                            return;
                        }

                        /* yy->PH */
                        flag = DAEReInit(dae_mem, (realtype)(*told), yy, yp);
                        if (check_flag(&flag, "IDAReInit", 1))
                        {
                            *ierr = 200 + (-flag);
                            freeallx;
                            return;
                        }

                        phase = 2; /* IDACalcIC: PHI-> yy0: if (ok) yy0_cic-> PHI*/
                        if (C2F(cmsolver).solver == 100)
                        {
                            copy_IDA_mem->ida_kk = 1;
                        }
                        // the initial conditons y0 and yp0 do not satisfy the DAE
                        flagr = DAECalcIC(dae_mem, DAE_YA_YDP_INIT, (realtype)(t));
                        phase = 1;
                        flag = DAEGetConsistentIC(dae_mem, yy, yp); /* PHI->YY */
                        if (*ierr > 5)    /* *ierr>5 => singularity in block */
                        {
                            freeallx;
                            return;
                        }

                        if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                        {
                            if (flagr >= 0)
                            {
                                sciprint(_("**** SUNDIALS.IDA successfully initialized *****\n") );
                            }
                            else
                            {
                                sciprint(_("**** SUNDIALS.IDA failed to initialize ->try again *****\n") );
                            }
                        }
                        /*-------------------------------------*/
                        /* saving the previous modes*/
                        for (jj = 0; jj < nmod; ++jj)
                        {
                            Mode_save[jj] = mod[jj];
                        }
                        if (ng > 0 && nmod > 0)
                        {
                            phase = 1;
                            zdoit(told, x, xd, g);
                            if (*ierr != 0)
                            {
                                freeallx;
                                return;
                            }
                        }
                        /*------------------------------------*/
                        Mode_change = 0;
                        for (jj = 0; jj < nmod; ++jj)
                        {
                            if (Mode_save[jj] != mod[jj])
                            {
                                Mode_change = 1;
                                break;
                            }
                        }
                        if (Mode_change == 0)
                        {
                            if (flagr >= 0 )
                            {
                                break;  /*   if (flagr>=0) break;  else{ *ierr=200+(-flagr); freeallx;  return; }*/
                            }
                            else if (j >= (int)( N_iters / 2))
                            {
                                /* DAESetMaxNumStepsIC(mem,10); */     /* maxnh (def=5) */
                                DAESetMaxNumJacsIC(dae_mem, 10);      /* maxnj 100 (def=4)*/
                                /* DAESetMaxNumItersIC(mem,100000); */ /* maxnit in IDANewtonIC (def=10) */
                                DAESetLineSearchOffIC(dae_mem, TRUE); /* (def=false)  */
                                /* DAESetNonlinConvCoefIC(mem,1.01);*/ /* (def=0.01-0.33*/
                                flag = DAESetMaxNumItersIC(dae_mem, 1000);
                                if (check_flag(&flag, "IDASetMaxNumItersIC", 1))
                                {
                                    *ierr = 200 + (-flag);
                                    freeallx;
                                    return;
                                };
                            }
                        }
                    }/* mode-CIC  counter*/
                    if (Mode_change == 1)
                    {
                        /* In this case, we try again by relaxing all modes and calling DAECalcIC again
                        /Masoud */
                        phase = 1;
                        if (C2F(cmsolver).solver == 100)
                        {
                            copy_IDA_mem->ida_kk = 1;
                        }
                        flagr = DAECalcIC(dae_mem, DAE_YA_YDP_INIT, (realtype)(t));
                        phase = 1;
                        flag = DAEGetConsistentIC(dae_mem, yy, yp); /* PHI->YY */
                        if ((flagr < 0) || (*ierr > 5)) /* *ierr>5 => singularity in block */
                        {
                            *ierr = 23;
                            freeallx;
                            return;
                        }
                    }
                    /*-----If flagr<0 the initialization solver has not converged-----*/
                    if (flagr < 0)
                    {
                        *ierr = 237;
                        freeallx;
                        return;
                    }

                } /* CIC calculation when hot==0 */

                if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                {
                    sciprint(_("****daskr from: %f to %f hot= %d  \n"), *told, t, hot);
                }

                /*--discrete zero crossings----dzero--------------------*/
                /*--check for Dzeros after Mode settings or ddoit()----*/
                Discrete_Jump = 0;
                if (ng > 0 && hot == 0)
                {
                    zdoit(told, x, xd, g);
                    if (*ierr != 0)
                    {
                        freeallx;
                        return;
                    }
                    for (jj = 0; jj < ng; ++jj)
                    {
                        if ((g[jj] >= 0.0) && ( jroot[jj] == -5))
                        {
                            Discrete_Jump = 1;
                            jroot[jj] = 1;
                        }
                        else if ((g[jj] < 0.0) && ( jroot[jj] == 5))
                        {
                            Discrete_Jump = 1;
                            jroot[jj] = -1;
                        }
                        else
                        {
                            jroot[jj] = 0;
                        }
                    }
                }

                /*--discrete zero crossings----dzero--------------------*/
                if (Discrete_Jump == 0) /* if there was a dzero, its event should be activated*/
                {
                    phase = 2;
                    flagr = DAESolve(dae_mem, t, told, yy, yp, DAE_NORMAL);
                    phase = 1;
                    if (*ierr != 0)
                    {
                        freeallx;
                        return;
                    }
                }
                else
                {
                    flagr = IDA_ROOT_RETURN; /* in order to handle discrete jumps */
                }
                if (flagr >= 0)
                {
                    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                    {
                        sciprint(_("****SUNDIALS.Ida reached: %f\n"), *told);
                    }
                    hot = 1;
                    cnt = 0;
                }
                else if ( flagr == IDA_TOO_MUCH_WORK ||  flagr == IDA_CONV_FAIL || flagr == IDA_ERR_FAIL)
                {
                    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                    {
                        sciprint(_("**** SUNDIALS.Ida: too much work at time=%g (stiff region, change RTOL and ATOL)\n"), *told);
                    }
                    hot = 0;
                    cnt++;
                    if (cnt > 5)
                    {
                        *ierr = 200 + (-flagr);
                        freeallx;
                        return;
                    }
                }
                else
                {
                    if (flagr < 0)
                    {
                        *ierr = 200 + (-flagr);    /* raising errors due to internal errors, otherwise error due to flagr*/
                    }
                    freeallx;
                    return;
                }

                /*     update outputs of 'c' type  blocks if we are at the end*/
                if (*told >= *tf)
                {
                    cdoit(told);
                    freeallx;
                    return;
                }

                if (flagr == IDA_ZERO_DETACH_RETURN)
                {
                    hot = 0;
                }; /* new feature of sundials, detects unmasking */
                if (flagr == IDA_ROOT_RETURN)
                {
                    /*     .        at least one root has been found */
                    hot = 0;
                    if (Discrete_Jump == 0)
                    {
                        flagr = DAEGetRootInfo(dae_mem, jroot);
                        if (check_flag(&flagr, "IDAGetRootInfo", 1))
                        {
                            *ierr = 200 + (-flagr);
                            freeallx;
                            return;
                        }
                    }

                    if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
                    {
                        sciprint(_("root found at t=: %f\n"), *told);
                    }
                    /*     .        update outputs affecting ztyp blocks  ONLY FOR OLD BLOCKS*/
                    zdoit(told, x, xd, g);
                    if (*ierr != 0)
                    {
                        freeallx;
                        return;
                    }
                    for (jj = 0; jj < ng; ++jj)
                    {
                        C2F(curblk).kfun = zcros[jj];
                        if (C2F(curblk).kfun == -1)
                        {
                            break;
                        }
                        kev = 0;
                        for (j = zcptr[C2F(curblk).kfun] - 1 ;
                                j < zcptr[C2F(curblk).kfun + 1] - 1 ; ++j)
                        {
                            if (jroot[j] != 0)
                            {
                                kev = 1;
                                break;
                            }
                        }
                        if (kev != 0)
                        {
                            Blocks[C2F(curblk).kfun - 1].jroot = &jroot[zcptr[C2F(curblk).kfun] - 1];
                            if (funtyp[C2F(curblk).kfun] > 0)
                            {
                                if (Blocks[C2F(curblk).kfun - 1].nevout > 0)
                                {
                                    flag__ = 3;
                                    if (Blocks[C2F(curblk).kfun - 1].nx > 0)
                                    {
                                        Blocks[C2F(curblk).kfun - 1].x  = &x[xptr[C2F(curblk).kfun] - 1];
                                        Blocks[C2F(curblk).kfun - 1].xd = &xd[xptr[C2F(curblk).kfun] - 1];
                                    }
                                    /*     call corresponding block to determine output event (kev) */
                                    Blocks[C2F(curblk).kfun - 1].nevprt = -kev;
                                    /*callf(told, xd, x, x,g,&flag__);*/
                                    callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
                                    if (flag__ < 0)
                                    {
                                        *ierr = 5 - flag__;
                                        freeallx;
                                        return;
                                    }
                                    /*     update event agenda */
                                    for (k = 0; k < Blocks[C2F(curblk).kfun - 1].nevout; ++k)
                                    {
                                        if (Blocks[C2F(curblk).kfun - 1].evout[k] >= 0)
                                        {
                                            i3 = k + clkptr[C2F(curblk).kfun] ;
                                            addevs(Blocks[C2F(curblk).kfun - 1].evout[k] + (*told), &i3, &ierr1);
                                            if (ierr1 != 0)
                                            {
                                                /*     .                       nevts too small */
                                                *ierr = 3;
                                                freeallx;
                                                return;
                                            }
                                        }
                                    }
                                }
                                /* update state */
                                if ((Blocks[C2F(curblk).kfun - 1].nx > 0) || (*Blocks[C2F(curblk).kfun - 1].work != NULL) )
                                {
                                    /* call corresponding block to update state */
                                    flag__ = 2;
                                    if (Blocks[C2F(curblk).kfun - 1].nx > 0)
                                    {
                                        Blocks[C2F(curblk).kfun - 1].x  = &x[xptr[C2F(curblk).kfun] - 1];
                                        Blocks[C2F(curblk).kfun - 1].xd = &xd[xptr[C2F(curblk).kfun] - 1];
                                    }
                                    Blocks[C2F(curblk).kfun - 1].nevprt = -kev;

                                    Blocks[C2F(curblk).kfun - 1].xprop = &xprop[-1 + xptr[C2F(curblk).kfun]];
                                    /*callf(told, xd, x, x,g,&flag__);*/
                                    callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);

                                    if (flag__ < 0)
                                    {
                                        *ierr = 5 - flag__;
                                        freeallx;
                                        return;
                                    }
                                    for (j = 0; j < *neq; j++) /* Adjust xprop for IDx */
                                    {
                                        if (xprop[j] ==  1)
                                        {
                                            scicos_xproperty[j] = ONE;
                                        }
                                        if (xprop[j] == -1)
                                        {
                                            scicos_xproperty[j] = ZERO;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                /* Serge Steer 29/06/2009 */
                while (ismenu()) //** if the user has done something, do the actions
                {
                    int ierr2 = 0;
                    int iUnused;
                    GetCommand(&CommandToUnstack, &SeqSync, &iUnused, NONE); //** get to the action
                    CommandLength = (int)strlen(CommandToUnstack);
                    //syncexec(CommandToUnstack, &CommandLength, &ierr2, &one, CommandLength); //** execute it
                    FREE(CommandToUnstack);
                }

                if (C2F(coshlt).halt != 0)
                {
                    if (C2F(coshlt).halt == 2)
                    {
                        *told = *tf;    /* end simulation */
                    }
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
                if (ng > 0) /* storing ZC signs just after a ddaskr call*/
                {
                    zdoit(told, x, xd, g);
                    if (*ierr != 0)
                    {
                        freeallx;
                        return;
                    }
                    for (jj = 0; jj < ng; ++jj)
                    {
                        if (g[jj] >= 0)
                        {
                            jroot[jj] = 5;
                        }
                        else
                        {
                            jroot[jj] = -5;
                        }
                    }
                }
                /*--discrete zero crossings----dzero--------------------*/
            }
            C2F(realtime)(told);
        }
        else
        {
            /*     .  t==told */
            if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
            {
                sciprint(_("Event: %d activated at t=%f\n"), *pointi, *told);
            }

            ddoit(told);
            if ((C2F(cosdebug).cosd >= 1) && (C2F(cosdebug).cosd != 3))
            {
                sciprint(_("End of activation\n"));
            }
            if (*ierr != 0)
            {
                freeallx;
                return;
            }
        }
        /*     end of main loop on time */
    }
    freeallx;
} /* cossimdaskr_ */
/*--------------------------------------------------------------------------*/
/* Subroutine cosend */
static void cosend(double *told)
{
    /* Local variables */
    static scicos_flag flag__ = 0;

    static int kfune = 0;

    /* Function Body */
    *ierr = 0;
    /*     loop on blocks */
    for (C2F(curblk).kfun = 1; C2F(curblk).kfun <= nblk; ++C2F(curblk).kfun)
    {
        flag__ = 5;
        Blocks[C2F(curblk).kfun - 1].nevprt = 0;
        if (funtyp[C2F(curblk).kfun] >= 0)
        {
            if (Blocks[C2F(curblk).kfun - 1].nx > 0)
            {
                Blocks[C2F(curblk).kfun - 1].x  = &x[xptr[C2F(curblk).kfun] - 1];
                Blocks[C2F(curblk).kfun - 1].xd = &xd[xptr[C2F(curblk).kfun] - 1];
            }
            /*callf(told, xd, x, x,x,&flag__);*/
            callf(told, &Blocks[C2F(curblk).kfun - 1], &flag__);
            if (flag__ < 0 && *ierr == 0)
            {
                *ierr = 5 - flag__;
                kfune = C2F(curblk).kfun;
            }
        }
    }
    if (*ierr != 0)
    {
        C2F(curblk).kfun = kfune;
        return;
    }
} /* cosend_ */
/*--------------------------------------------------------------------------*/
/* callf */
void callf(double *t, scicos_block *block, scicos_flag *flag)
{
    double* args[SZ_SIZE];
    int sz[SZ_SIZE];
    double intabl[TB_SIZE];
    double outabl[TB_SIZE];

    int ii = 0, in = 0, out = 0, ki = 0, ko = 0, no = 0, ni = 0, k = 0, j = 0;
    int szi = 0, flagi = 0;
    double *ptr_d = NULL;

    /* function pointers type def */
    voidf loc ;
    ScicosF0 loc0;
    ScicosF loc1;
    /*  ScicosFm1 loc3;*/
    ScicosF2 loc2;
    ScicosF2z loc2z;
    ScicosFi loci1;
    ScicosFi2 loci2;
    ScicosFi2z loci2z;
    ScicosF4 loc4;

    int solver = C2F(cmsolver).solver;
    int cosd   = C2F(cosdebug).cosd;
    /*int kf     = C2F(curblk).kfun;*/
    scicos_time     = *t;
    block_error     = (int*) flag;

    /* debug block is never called */
    /*if (kf==(debug_block+1)) return;*/
    if (block->type == DEBUG_BLOCK)
    {
        return;
    }

    /* flag 7 implicit initialization */
    flagi = (int) * flag;
    /* change flag to zero if flagi==7 for explicit block */
    if (flagi == 7 && block->type < 10000)
    {
        *flag = 0;
    }

    /* display information for debugging mode */
    if (cosd > 1)
    {
        if (cosd != 3)
        {
            sciprint(_("block %d [%s] is called "), C2F(curblk).kfun, block->uid);
            sciprint(_("with flag %d "), *flag);
            sciprint(_("at time %f \n"), *t);
        }
        if (debug_block > -1)
        {
            if (cosd != 3)
            {
                sciprint(_("Entering the block \n"));
            }
            call_debug_scicos(block, flag, flagi, debug_block);
            if (*flag < 0)
            {
                return;    /* error in debug block */
            }
        }
    }

    C2F(scsptr).ptr = block->scsptr;

    /* get pointer of the function */
    loc = block->funpt;

    /* continuous state */
    if ((solver == IDA_BDF_Newton || solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes) && block->type < 10000 && *flag == 0)
    {
        ptr_d = block->xd;
        block->xd  = block->res;
    }

    /* switch loop */
    //sciprint("callf type=%d flag=%d\n",block->type,flagi);
    switch (block->type)
    {
            /*******************/
            /* function type 0 */
            /*******************/
        case 0 :
        {
            /* This is for compatibility */
            /* jroot is returned in g for old type */
            if (block->nevprt < 0)
            {
                for (j = 0; j < block->ng; ++j)
                {
                    block->g[j] = (double)block->jroot[j];
                }
            }

            /* concatenated entries and concatened outputs */
            /* catenate inputs if necessary */
            ni = 0;
            if (block->nin > 1)
            {
                ki = 0;
                for (in = 0; in < block->nin; in++)
                {
                    szi = block->insz[in] * block->insz[in + block->nin];
                    for (ii = 0; ii < szi; ii++)
                    {
                        intabl[ki++] = *((double *)(block->inptr[in]) + ii);
                    }
                    ni = ni + szi;
                }
                args[0] = &(intabl[0]);
            }
            else
            {
                if (block->nin == 0)
                {
                    args[0] = NULL;
                }
                else
                {
                    args[0] = (double *)(block->inptr[0]);
                    ni = block->insz[0] * block->insz[1];
                }
            }

            /* catenate outputs if necessary */
            no = 0;
            if (block->nout > 1)
            {
                ko = 0;
                for (out = 0; out < block->nout; out++)
                {
                    szi = block->outsz[out] * block->outsz[out + block->nout];
                    for (ii = 0; ii < szi; ii++)
                    {
                        outabl[ko++] = *((double *)(block->outptr[out]) + ii);
                    }
                    no = no + szi;
                }
                args[1] = &(outabl[0]);
            }
            else
            {
                if (block->nout == 0)
                {
                    args[1] = NULL;
                }
                else
                {
                    args[1] = (double *)(block->outptr[0]);
                    no = block->outsz[0] * block->outsz[1];
                }
            }

            loc0 = (ScicosF0) loc;

            (*loc0)(flag, &block->nevprt, t, block->xd, block->x, &block->nx,
                    block->z, &block->nz,
                    block->evout, &block->nevout, block->rpar, &block->nrpar,
                    block->ipar, &block->nipar, (double *)args[0], &ni,
                    (double *)args[1], &no);

            /* split output vector on each port if necessary */
            if (block->nout > 1)
            {
                ko = 0;
                for (out = 0; out < block->nout; out++)
                {
                    szi = block->outsz[out] * block->outsz[out + block->nout];
                    for (ii = 0; ii < szi; ii++)
                    {
                        *((double *)(block->outptr[out]) + ii) = outabl[ko++];
                    }
                }
            }

            /* adjust values of output register */
            for (in = 0; in < block->nevout; ++in)
            {
                block->evout[in] = block->evout[in] - *t;
            }

            break;
        }

        /*******************/
        /* function type 1 */
        /*******************/
        case 1 :
        {
            /* This is for compatibility */
            /* jroot is returned in g for old type */
            if (block->nevprt < 0)
            {
                for (j = 0; j < block->ng; ++j)
                {
                    block->g[j] = (double)block->jroot[j];
                }
            }

            /* one entry for each input or output */
            for (in = 0 ; in < block->nin ; in++)
            {
                args[in] = block->inptr[in];
                sz[in] = block->insz[in];
            }
            for (out = 0; out < block->nout; out++)
            {
                args[in + out] = block->outptr[out];
                sz[in + out] = block->outsz[out];
            }
            /* with zero crossing */
            if (block->ztyp > 0)
            {
                args[block->nin + block->nout] = block->g;
                sz[block->nin + block->nout] = block->ng;
            }

            loc1 = (ScicosF) loc;

            (*loc1)(flag, &block->nevprt, t, block->xd, block->x, &block->nx,
                    block->z, &block->nz,
                    block->evout, &block->nevout, block->rpar, &block->nrpar,
                    block->ipar, &block->nipar,
                    (double *)args[0], &sz[0],
                    (double *)args[1], &sz[1], (double *)args[2], &sz[2],
                    (double *)args[3], &sz[3], (double *)args[4], &sz[4],
                    (double *)args[5], &sz[5], (double *)args[6], &sz[6],
                    (double *)args[7], &sz[7], (double *)args[8], &sz[8],
                    (double *)args[9], &sz[9], (double *)args[10], &sz[10],
                    (double *)args[11], &sz[11], (double *)args[12], &sz[12],
                    (double *)args[13], &sz[13], (double *)args[14], &sz[14],
                    (double *)args[15], &sz[15], (double *)args[16], &sz[16],
                    (double *)args[17], &sz[17]);

            /* adjust values of output register */
            for (in = 0; in < block->nevout; ++in)
            {
                block->evout[in] = block->evout[in] - *t;
            }

            break;
        }

        /*******************/
        /* function type 2 */
        /*******************/
        case 2 :
        {
            /* This is for compatibility */
            /* jroot is returned in g for old type */
            if (block->nevprt < 0)
            {
                for (j = 0; j < block->ng; ++j)
                {
                    block->g[j] = (double)block->jroot[j];
                }
            }

            /* no zero crossing */
            if (block->ztyp == 0)
            {
                loc2 = (ScicosF2) loc;
                (*loc2)(flag, &block->nevprt, t, block->xd, block->x, &block->nx,
                        block->z, &block->nz,
                        block->evout, &block->nevout, block->rpar, &block->nrpar,
                        block->ipar, &block->nipar, (double **)block->inptr,
                        block->insz, &block->nin,
                        (double **)block->outptr, block->outsz, &block->nout,
                        block->scsptr);
            }
            /* with zero crossing */
            else
            {
                loc2z = (ScicosF2z) loc;
                (*loc2z)(flag, &block->nevprt, t, block->xd, block->x, &block->nx,
                         block->z, &block->nz,
                         block->evout, &block->nevout, block->rpar, &block->nrpar,
                         block->ipar, &block->nipar, (double **)block->inptr,
                         block->insz, &block->nin,
                         (double **)block->outptr, block->outsz, &block->nout,
                         block->scsptr,
                         block->g, &block->ng);
            }

            /* adjust values of output register */
            for (in = 0; in < block->nevout; ++in)
            {
                block->evout[in] = block->evout[in] - *t;
            }

            break;
        }

        /*******************/
        /* function type 4 */
        /*******************/
        case 4 :
        {
            /* get pointer of the function type 4*/
            loc4 = (ScicosF4) loc;

            (*loc4)(block, *flag);

            break;
        }

        /***********************/
        /* function type 10001 */
        /***********************/
        case 10001 :
        {
            /* This is for compatibility */
            /* jroot is returned in g for old type */
            if (block->nevprt < 0)
            {
                for (j = 0; j < block->ng; ++j)
                {
                    block->g[j] = (double)block->jroot[j];
                }
            }

            /* implicit block one entry for each input or output */
            for (in = 0 ; in < block->nin ; in++)
            {
                args[in] = block->inptr[in];
                sz[in] = block->insz[in];
            }
            for (out = 0; out < block->nout; out++)
            {
                args[in + out] = block->outptr[out];
                sz[in + out] = block->outsz[out];
            }
            /* with zero crossing */
            if (block->ztyp > 0)
            {
                args[block->nin + block->nout] = block->g;
                sz[block->nin + block->nout] = block->ng;
            }

            loci1 = (ScicosFi) loc;
            (*loci1)(flag, &block->nevprt, t, block->res, block->xd, block->x,
                     &block->nx, block->z, &block->nz,
                     block->evout, &block->nevout, block->rpar, &block->nrpar,
                     block->ipar, &block->nipar,
                     (double *)args[0], &sz[0],
                     (double *)args[1], &sz[1], (double *)args[2], &sz[2],
                     (double *)args[3], &sz[3], (double *)args[4], &sz[4],
                     (double *)args[5], &sz[5], (double *)args[6], &sz[6],
                     (double *)args[7], &sz[7], (double *)args[8], &sz[8],
                     (double *)args[9], &sz[9], (double *)args[10], &sz[10],
                     (double *)args[11], &sz[11], (double *)args[12], &sz[12],
                     (double *)args[13], &sz[13], (double *)args[14], &sz[14],
                     (double *)args[15], &sz[15], (double *)args[16], &sz[16],
                     (double *)args[17], &sz[17]);

            /* adjust values of output register */
            for (in = 0; in < block->nevout; ++in)
            {
                block->evout[in] = block->evout[in] - *t;
            }

            break;
        }

        /***********************/
        /* function type 10002 */
        /***********************/
        case 10002 :
        {
            /* This is for compatibility */
            /* jroot is returned in g for old type */
            if (block->nevprt < 0)
            {
                for (j = 0; j < block->ng; ++j)
                {
                    block->g[j] = (double)block->jroot[j];
                }
            }

            /* implicit block, inputs and outputs given by a table of pointers */
            /* no zero crossing */
            if (block->ztyp == 0)
            {
                loci2 = (ScicosFi2) loc;
                (*loci2)(flag, &block->nevprt, t, block->res,
                         block->xd, block->x, &block->nx,
                         block->z, &block->nz,
                         block->evout, &block->nevout, block->rpar, &block->nrpar,
                         block->ipar, &block->nipar, (double **)block->inptr,
                         block->insz, &block->nin,
                         (double **)block->outptr, block->outsz, &block->nout);
            }
            /* with zero crossing */
            else
            {
                loci2z = (ScicosFi2z) loc;
                (*loci2z)(flag, &block->nevprt, t, block->res,
                          block->xd, block->x, &block->nx,
                          block->z, &block->nz,
                          block->evout, &block->nevout, block->rpar, &block->nrpar,
                          block->ipar, &block->nipar,
                          (double **)block->inptr, block->insz, &block->nin,
                          (double **)block->outptr, block->outsz, &block->nout,
                          block->g, &block->ng);
            }

            /* adjust values of output register */
            for (in = 0; in < block->nevout; ++in)
            {
                block->evout[in] = block->evout[in] - *t;
            }

            break;
        }

        /***********************/
        /* function type 10004 */
        /***********************/
        case 10004 :
        {
            /* get pointer of the function type 4*/
            loc4 = (ScicosF4) loc;

            (*loc4)(block, *flag);

            break;
        }

        /***********/
        /* default */
        /***********/
        default :
        {
            sciprint(_("Undefined Function type\n"));
            *flag = -1000;
            return; /* exit */
        }
    }
    // sciprint("callf end  flag=%d\n",*flag);
    /* Implicit Solver & explicit block & flag==0 */
    /* adjust continuous state vector after call */
    if ((solver == IDA_BDF_Newton || solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes) && block->type < 10000 && *flag == 0)
    {
        block->xd  = ptr_d;
        if (flagi != 7)
        {
            for (k = 0; k < block->nx; k++)
            {
                block->res[k] = block->res[k] - block->xd[k];
            }
        }
        else
        {
            for (k = 0; k < block->nx; k++)
            {
                block->xd[k] = block->res[k];
            }
        }
    }

    /* debug block */
    if (cosd > 1)
    {
        if (debug_block > -1)
        {
            if (*flag < 0)
            {
                return;    /* error in block */
            }
            if (cosd != 3)
            {
                sciprint(_("Leaving block %d \n"), C2F(curblk).kfun);
            }
            call_debug_scicos(block, flag, flagi, debug_block);
            /*call_debug_scicos(flag,kf,flagi,debug_block);*/
        }
    }
} /* callf */
/*--------------------------------------------------------------------------*/
/* call_debug_scicos */
static void call_debug_scicos(scicos_block *block, scicos_flag *flag, int flagi, int deb_blk)
{
    voidf loc ;
    int solver = C2F(cmsolver).solver, k = 0;
    ScicosF4 loc4;
    double *ptr_d = NULL;

    C2F(cosdebugcounter).counter = C2F(cosdebugcounter).counter + 1;
    C2F(scsptr).ptr = block->scsptr;

    loc  = Blocks[deb_blk].funpt; /* GLOBAL */
    loc4 = (ScicosF4) loc;

    /* continuous state */
    if ((solver == IDA_BDF_Newton || solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes) && block->type < 10000 && *flag == 0)
    {
        ptr_d = block->xd;
        block->xd  = block->res;
    }

    // Temporarily replacing the block's computational function with DEBUG_BLOCK's so that sciblk4 will call %debug_scicos()
    block->scsptr = Blocks[deb_blk].scsptr;
    (*loc4)(block, *flag);
    block->scsptr = C2F(scsptr).ptr;

    /* Implicit Solver & explicit block & flag==0 */
    /* adjust continuous state vector after call */
    if ((solver == IDA_BDF_Newton || solver == DDaskr_BDF_Newton || solver == DDaskr_BDF_GMRes) && block->type < 10000 && *flag == 0)
    {
        block->xd  = ptr_d;
        if (flagi != 7)
        {
            for (k = 0; k < block->nx; k++)
            {
                block->res[k] = block->res[k] - block->xd[k];
            }
        }
        else
        {
            for (k = 0; k < block->nx; k++)
            {
                block->xd[k] = block->res[k];
            }
        }
    }

    if (*flag < 0)
    {
        sciprint(_("Error in the Debug block \n"));
    }
} /* call_debug_scicos */
/*--------------------------------------------------------------------------*/
/* simblk */
static int simblk(realtype t, N_Vector yy, N_Vector yp, void *f_data)
{
    double tx = 0., *x = NULL, *xd = NULL;
    int i = 0, nantest = 0;

    tx = (double) t;
    x =  (double *) NV_DATA_S(yy);
    xd = (double *) NV_DATA_S(yp);

    for (i = 0; i < *neq; i++)
    {
        xd[i] = 0;    /* à la place de "C2F(dset)(neq, &c_b14,xcdot , &c__1);"*/
    }
    C2F(ierode).iero = 0;
    *ierr = 0;
    odoit(&tx, x, xd, xd);
    C2F(ierode).iero = *ierr;

    if (*ierr == 0)
    {
        nantest = 0;
        for (i = 0; i < *neq; i++) /* NaN checking */
        {
            if ((xd[i] - xd[i] != 0))
            {
                Sciwarning(_("\nWarning: The computing function #%d returns a NaN/Inf"), i);
                nantest = 1;
                break;
            }
        }
        if (nantest == 1)
        {
            return 349;    /* recoverable error; */
        }
    }

    return (abs(*ierr)); /* ierr>0 recoverable error; ierr>0 unrecoverable error; ierr=0: ok*/

} /* simblk */
/*--------------------------------------------------------------------------*/
/* grblk */
static int grblk(realtype t, N_Vector yy, realtype *gout, void *g_data)
{
    double tx = 0., *x = NULL;
    int jj = 0, nantest = 0;

    tx = (double) t;
    x =  (double *) NV_DATA_S(yy);

    C2F(ierode).iero = 0;
    *ierr = 0;

    zdoit(&tx, x, x, (double*) gout);

    if (*ierr == 0)
    {
        nantest = 0;
        for (jj = 0; jj < ng; jj++)
            if (gout[jj] - gout[jj] != 0)
            {
                Sciwarning(_("\nWarning: The zero_crossing function #%d returns a NaN/Inf"), jj);
                nantest = 1;
                break;
            } /* NaN checking */
        if (nantest == 1)
        {
            return 350;    /* recoverable error; */
        }
    }
    C2F(ierode).iero = *ierr;

    return 0;
} /* grblk */
/*--------------------------------------------------------------------------*/
/* simblklsodar */
static void simblklsodar(int * nequations, realtype * tOld, realtype * actual, realtype * res)
{
    double tx = 0.;
    int i = 0;

    tx = (double) * tOld;

    for (i = 0; i < *nequations; ++i)
    {
        res[i] = 0;    /* à la place de "C2F(dset)(neq, &c_b14,xcdot , &c__1);"*/
    }
    C2F(ierode).iero = 0;
    *ierr = 0;
    odoit(&tx, actual, res, res);
    C2F(ierode).iero = *ierr;

    if (*ierr == 0)
    {
        for (i = 0; i < *nequations; i++) /* NaN checking */
        {
            if ((res[i] - res[i] != 0))
            {
                Sciwarning(_("\nWarning: The computing function #%d returns a NaN/Inf"), i);
            }
        }
    }
} /* simblklsodar */
/*--------------------------------------------------------------------------*/
/* grblklsodar */
static void grblklsodar(int * nequations, realtype * tOld, realtype * actual, int * ngc, realtype * res)
{
    double tx = 0.;
    int jj = 0;

    tx = (double) * tOld;

    C2F(ierode).iero = 0;
    *ierr = 0;

    zdoit(&tx, actual, actual, res);

    if (*ierr == 0)
    {
        for (jj = 0; jj < *ngc; jj++)
        {
            if (res[jj] - res[jj] != 0)
            {
                Sciwarning(_("\nWarning: The zero_crossing function #%d returns a NaN/Inf"), jj);
            } /* NaN checking */
        }
    }
} /* grblklsodar */
/*--------------------------------------------------------------------------*/
/* simblkdaskr */
static int simblkdaskr(realtype tres, N_Vector yy, N_Vector yp, N_Vector resval, void *rdata)
{
    double tx = 0.;
    double *xc = NULL, *xcdot = NULL, *residual = NULL;
    realtype alpha = 0.;

    UserData data;

    realtype hh = 0.;
    int qlast = 0;
    int jj = 0, flag = 0, nantest = 0;

    data = (UserData) rdata;

    if (get_phase_simulation() == 1)
    {
        /* Just to update mode in a very special case, i.e., when initialization using modes fails.
        in this case, we relax all modes and try again one more time.
        */
        zdoit(&tx, NV_DATA_S(yy), NV_DATA_S(yp), NULL);
    }

    hh = ZERO;
    flag = IDAGetCurrentStep(data->dae_mem, &hh);
    if (flag < 0)
    {
        *ierr = 200 + (-flag);
        return (*ierr);
    };

    qlast = 0;
    flag = IDAGetCurrentOrder(data->dae_mem, &qlast);
    if (flag < 0)
    {
        *ierr = 200 + (-flag);
        return (*ierr);
    };

    alpha = ZERO;
    for (jj = 0; jj < qlast; jj++)
    {
        alpha = alpha - ONE / (jj + 1);
    }
    if (hh != 0)
        // CJ=-alpha/hh;  // For function Get_Jacobian_cj
    {
        CJJ = -alpha / hh;
    }
    else
    {
        *ierr = 217;
        return (*ierr);
    }
    xc = (double *)  NV_DATA_S(yy);
    xcdot = (double *) NV_DATA_S(yp);
    residual = (double *) NV_DATA_S(resval);
    tx = (double) tres;

    C2F(dcopy)(neq, xcdot, &c__1, residual, &c__1);
    *ierr = 0;
    C2F(ierode).iero = 0;
    odoit(&tx, xc, xcdot, residual);

    C2F(ierode).iero = *ierr;

    if (*ierr == 0)
    {
        nantest = 0;
        for (jj = 0; jj < *neq; jj++)
            if (residual[jj] - residual[jj] != 0) /* NaN checking */
            {
                //sciprint(_("\nWarning: The residual function #%d returns a NaN"),jj);
                nantest = 1;
                break;
            }
        if (nantest == 1)
        {
            return 257;    /* recoverable error; */
        }
    }

    return (abs(*ierr)); /* ierr>0 recoverable error; ierr>0 unrecoverable error; ierr=0: ok*/
}/* simblkdaskr */
/*--------------------------------------------------------------------------*/
/* grblkdaskr */
static int grblkdaskr(realtype t, N_Vector yy, N_Vector yp, realtype *gout, void *g_data)
{
    double tx = 0.;
    int jj = 0, nantest = 0;

    tx = (double) t;

    *ierr = 0;
    C2F(ierode).iero = 0;
    zdoit(&tx, NV_DATA_S(yy), NV_DATA_S(yp), (double *) gout);
    if (*ierr == 0)
    {
        nantest = 0; /* NaN checking */
        for (jj = 0; jj < ng; jj++)
        {
            if (gout[jj] - gout[jj] != 0)
            {
                Sciwarning(_("\nWarning: The zero-crossing function #%d returns a NaN"), jj);
                nantest = 1;
                break;
            }
        }
        if (nantest == 1)
        {
            return 258; /* recoverable error; */
        }
    }
    C2F(ierode).iero = *ierr;
    return (*ierr);
}/* grblkdaskr */
/*--------------------------------------------------------------------------*/
/* simblkddaskr */
static void simblkddaskr(realtype *tOld, realtype *actual, realtype *actualP, realtype *res, int *flag, double *dummy1, int *dummy2)
{
    double tx = 0.;

    int jj = 0;

    if (get_phase_simulation() == 1)
    {
        /* Just to update mode in a very special case, i.e., when initialization using modes fails.
        in this case, we relax all modes and try again one more time.
        */
        zdoit(&tx, actual, actualP, NULL);
    }

    CJJ = 6;

    tx = (double) * tOld;
    *flag = 0;

    C2F(dcopy)(neq, actualP, &c__1, res, &c__1);
    *ierr = 0;
    C2F(ierode).iero = 0;
    odoit(&tx, actual, actualP, res);
    C2F(ierode).iero = *ierr;

    if (*ierr == 0)
    {
        for (jj = 0; jj < *neq; jj++)
            if (res[jj] - res[jj] != 0) /* NaN checking */
            {
                Sciwarning(_("\nWarning: The residual function #%d returns a NaN"), jj);
                *flag = -1; /* recoverable error; */
                return;
            }
    }
    else
    {
        *flag = -2;
        return;
    }
    /* *flag=-1 recoverable error; *flag=-2 unrecoverable error; *flag=0: ok*/

}/* simblkddaskr */
/*--------------------------------------------------------------------------*/
/* grblkddaskr */
static void grblkddaskr(int *nequations, realtype *tOld, realtype *actual, int *ngc, realtype *res, double *dummy1, int *dummy2)
{
    double tx = 0.;
    int jj = 0;

    tx = (double) * tOld;

    *ierr = 0;
    C2F(ierode).iero = 0;
    zdoit(&tx, actual, actual, res);
    C2F(ierode).iero = *ierr;

    if (*ierr == 0)
    {
        /* NaN checking */
        for (jj = 0; jj < *ngc; jj++)
        {
            if (res[jj] - res[jj] != 0)
            {
                Sciwarning(_("\nWarning: The zero-crossing function #%d returns a NaN"), jj);
                return;
            }
        }
    }
    else
    {
        sciprint(_("\nError: Problem in the evaluation of a root function"));
        return;
    }

}/* grblkddaskr */
/*--------------------------------------------------------------------------*/
/* jacpsol */
static void jacpsol(realtype *res, int *ires, int *neq, realtype *tOld, realtype *actual, realtype *actualP,
                    realtype *rewt, realtype *savr, realtype *wk, realtype *h, realtype *cj, realtype *wp, int *iwp,
                    int *ier, double *dummy1, int *dummy2)
{
    /* Here, we compute the system preconditioner matrix P, which is actually the jacobian matrix,
       so P(i,j) = dres(i)/dactual(j) + cj*dres(i)/dactualP(j), and we LU-decompose it. */
    int i = 0, j = 0, nrow = 0, info = 0;
    realtype tx = 0, del = 0, delinv = 0, ysave = 0, ypsave = 0;
    realtype * e = NULL;

    tx = *tOld;

    /* Work array used to evaluate res(*tOld, actual + small_increment, actualP + small_increment).
       savr already contains res(*tOld, actual, actualP). */
    e = (realtype *) calloc(*neq, sizeof(realtype));

    for (i = 0; i < *neq; ++i)
    {
        del =  max (SQuround * max(fabs(actual[i]), fabs(*h * actualP[i])), 1. / rewt[i]);
        del *= (*h * actualP[i] >= 0) ? 1 : -1;
        del =  (actual[i] + del) - actual[i];
        ysave  = actual[i];
        ypsave = actualP[i];
        actual[i]  += del;
        actualP[i] += *cj * del;
        *ierr = 0;
        C2F(ierode).iero = 0;
        simblkddaskr(tOld, actual, actualP, e, ires, dummy1, dummy2);
        C2F(ierode).iero = *ierr;
        if (*ires == 0)
        {
            delinv = 1. / del;
            for (j = 0; j < *neq; ++j)
            {
                wp[nrow + j] = (e[j] - savr[j]) * delinv;
                /* NaN test */
                if (wp[nrow + j] - wp[nrow + j] != 0)
                {
                    Sciwarning(_("\nWarning: The preconditioner evaluation function returns a NaN at index #%d."), nrow + j);
                    *ier = -1;
                }
            }
            nrow       += *neq;
            actual[i]  =  ysave;
            actualP[i] =  ypsave;
        }
        else
        {
            sciprint(_("\nError: The preconditioner evaluation function failed."));
            *ier = -1;
            free(e);
            return;
        }
    }

    /* Proceed to LU factorization of P. */
    C2F(dgefa) (wp, neq, neq, iwp, &info);
    if (info != 0)
    {
        sciprint(_("\nError: Failed to factor the preconditioner."));
        *ier = -1;
    }

    free(e);
}/* jacpsol */
/*--------------------------------------------------------------------------*/
/* psol */
static void psol(int *neq, realtype *tOld, realtype *actual, realtype *actualP,
                 realtype *savr, realtype *wk, realtype *cj, realtype *wght, realtype *wp,
                 int *iwp, realtype *b, realtype *eplin, int *ier, double *dummy1, int *dummy2)
{
    /* This function "applies" the inverse of the preconditioner to 'b' (computes P^-1*b).
       It is done by solving P*x = b using the linpack routine 'dgesl'. */
    int i = 0, job = 0;

    C2F(dgesl) (wp, neq, neq, iwp, b, &job);

    /* NaN test */
    for (i = 0; i < *neq; ++i)
    {
        if (b[i] - b[i] != 0)
        {
            Sciwarning(_("\nWarning: The preconditioner application function returns a NaN at index #%d."), i);
            /* Indicate a recoverable error, meaning that the step will be retried with the same step size
               but with a call to 'jacpsol' to update necessary data, unless the Jacobian data is current,
               in which case the step will be retried with a smaller step size. */
            *ier = 1;
        }
    }
}/* psol */
/*--------------------------------------------------------------------------*/
/* Subroutine addevs */
static void addevs(double t, int *evtnb, int *ierr1)
{
    static int i = 0, j = 0;

    /* Function Body */
    *ierr1 = 0;
    if (evtspt[*evtnb] != -1)
    {
        if ((evtspt[*evtnb] == 0) && (*pointi == *evtnb))
        {
            tevts[*evtnb] = t;
            return;
        }
        else
        {
            if (*pointi == *evtnb)
            {
                *pointi = evtspt[*evtnb]; /* remove from chain */
            }
            else
            {
                i = *pointi;
                while (*evtnb != evtspt[i])
                {
                    i = evtspt[i];
                }
                evtspt[i] = evtspt[*evtnb]; /* remove old evtnb from chain */
                if (TCritWarning == 0)
                {
                    sciprint(_("\n Warning: an event is reprogrammed at t=%g by removing another"), t );
                    sciprint(_("\n         (already programmed) event. There may be an error in"));
                    Sciwarning(_("\n         your model. Please check your model\n"));
                    TCritWarning = 1;
                }
                do_cold_restart(); /* the erased event could be a critical
								   event, so do_cold_restart is added to
								   refresh the critical event table */
            }
            evtspt[*evtnb] = 0;
            tevts[*evtnb] = t;
        }
    }
    else
    {
        evtspt[*evtnb] = 0;
        tevts[*evtnb] = t;
    }
    if (*pointi == 0)
    {
        *pointi = *evtnb;
        return;
    }
    if (t < tevts[*pointi])
    {
        evtspt[*evtnb] = *pointi;
        *pointi = *evtnb;
        return;
    }
    i = *pointi;

L100:
    if (evtspt[i] == 0)
    {
        evtspt[i] = *evtnb;
        return;
    }
    if (t >= tevts[evtspt[i]])
    {
        j = evtspt[i];
        if (evtspt[j] == 0)
        {
            evtspt[j] = *evtnb;
            return;
        }
        i = j;
        goto L100;
    }
    else
    {
        evtspt[*evtnb] = evtspt[i];
        evtspt[i] = *evtnb;
    }
} /* addevs */
/*--------------------------------------------------------------------------*/
/* Subroutine putevs */
void putevs(double *t, int *evtnb, int *ierr1)
{
    /* Function Body */
    *ierr1 = 0;
    if (evtspt[*evtnb] != -1)
    {
        *ierr1 = 1;
        return;
    }
    else
    {
        evtspt[*evtnb] = 0;
        tevts[*evtnb] = *t;
    }
    if (*pointi == 0)
    {
        *pointi = *evtnb;
        return;
    }
    evtspt[*evtnb] = *pointi;
    *pointi = *evtnb;
} /* putevs */
/*--------------------------------------------------------------------------*/
/* Subroutine idoit */
static void idoit(double *told)
{
    /* initialisation (propagation of constant blocks outputs) */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int i = 0, j = 0;
    int ierr1 = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    flag = 1;
    for (j = 0; j < * (scs_imp->niord); j++)
    {
        kf = &scs_imp->iord[j];
        C2F(curblk).kfun = *kf; /* */
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            /* continuous state */
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
            }
            scs_imp->blocks[*kf - 1].nevprt = scs_imp->iord[j + * (scs_imp->niord)];
            /*callf(told, xd, x, x,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }
        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                i = synchro_nev(scs_imp, *kf, ierr);
                if (*ierr != 0)
                {
                    return;
                }
                i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                doit(told);
                if (*ierr != 0)
                {
                    return;
                }
            }
        }
    }
} /* idoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine doit */
static void doit(double *told)
{
    /* propagation of blocks outputs on discrete activations */
    /*     Copyright INRIA */

    int i = 0, i2 = 0;
    scicos_flag flag = 0;
    int nord = 0;
    int ierr1 = 0;
    int ii = 0, kever = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    kever = *pointi;
    *pointi = evtspt[kever];
    evtspt[kever] = -1;

    nord = scs_imp->ordptr[kever] - scs_imp->ordptr[kever - 1];
    if (nord == 0)
    {
        return;
    }

    for (ii = scs_imp->ordptr[kever - 1]; ii <= scs_imp->ordptr[kever] - 1 ; ii++)
    {
        kf = &scs_imp->ordclk[ii - 1];
        C2F(curblk).kfun = *kf;
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            /* continuous state */
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
            }
            scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);
            flag = 1;
            /*callf(told, xd, x, x,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        /* Initialize tvec */
        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                i = synchro_nev(scs_imp, *kf, ierr);
                if (*ierr != 0)
                {
                    return;
                }
                i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                doit(told);
                if (*ierr != 0)
                {
                    return;
                }
            }
        }
    }
} /* doit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine cdoit */
static void cdoit(double *told)
{
    /* propagation of continuous blocks outputs */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int ierr1 = 0;
    int i = 0, j = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    for (j = 0; j < * (scs_imp->ncord); j++)
    {
        kf = &scs_imp->cord[j];
        C2F(curblk).kfun = *kf;
        /* continuous state */
        if (scs_imp->blocks[*kf - 1].nx > 0)
        {
            scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
        }
        scs_imp->blocks[*kf - 1].nevprt = scs_imp->cord[j + * (scs_imp->ncord)];
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            flag = 1;
            /*callf(told, xd, x, x,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        /* Initialize tvec */
        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                i = synchro_nev(scs_imp, *kf, ierr);
                if (*ierr != 0)
                {
                    return;
                }
                i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                doit(told);
                if (*ierr != 0)
                {
                    return;
                }
            }
        }
    }
} /* cdoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine ddoit */
static void ddoit(double *told)
{
    /* update states & event out on discrete activations */
    /*     Copyright INRIA */

    int i2 = 0, j = 0;
    scicos_flag flag = 0;
    int kiwa = 0;
    int i = 0, i3 = 0, ierr1 = 0;
    int ii = 0, keve = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    kiwa = 0;
    edoit(told, &kiwa);
    if (*ierr != 0)
    {
        return;
    }

    /* update continuous and discrete states on event */
    if (kiwa == 0)
    {
        return;
    }
    for (i = 0; i < kiwa; i++)
    {
        keve = iwa[i];
        if (critev[keve] != 0)
        {
            hot = 0;
        }
        i2 = scs_imp->ordptr[keve] - 1;
        for (ii = scs_imp->ordptr[keve - 1]; ii <= i2; ii++)
        {
            kf = &scs_imp->ordclk[ii - 1];
            C2F(curblk).kfun = *kf;
            /* continuous state */
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
            }

            scs_imp->blocks[*kf - 1].nevprt = scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1];

            if (scs_imp->blocks[*kf - 1].nevout > 0)
            {
                if (scs_imp->funtyp[*kf - 1] >= 0)
                {
                    /* initialize evout */
                    for (j = 0; j < scs_imp->blocks[*kf - 1].nevout; j++)
                    {
                        scs_imp->blocks[*kf - 1].evout[j] = -1;
                    }
                    flag = 3;

                    if (scs_imp->blocks[*kf - 1].nevprt > 0) /* if event has continuous origin don't call*/
                    {
                        /*callf(told, xd, x, x ,x,&flag);*/
                        callf(told, &scs_imp->blocks[*kf - 1], &flag);
                        if (flag < 0)
                        {
                            *ierr = 5 - flag;
                            return;
                        }
                    }

                    for (j = 0; j < scs_imp->blocks[*kf - 1].nevout; j++)
                    {
                        if (scs_imp->blocks[*kf - 1].evout[j] >= 0.)
                        {
                            i3 = j + scs_imp->clkptr[*kf - 1] ;
                            addevs(scs_imp->blocks[*kf - 1].evout[j] + (*told), &i3, &ierr1);
                            if (ierr1 != 0)
                            {
                                /* event conflict */
                                *ierr = 3;
                                return;
                            }
                        }
                    }
                }
            }

            if (scs_imp->blocks[*kf - 1].nevprt > 0)
            {
                if (scs_imp->blocks[*kf - 1].nx + scs_imp->blocks[*kf - 1].nz + scs_imp->blocks[*kf - 1].noz > 0 || \
                        *scs_imp->blocks[*kf - 1].work != NULL)
                {
                    /*  if a hidden state exists, must also call (for new scope eg)  */
                    /*  to avoid calling non-real activations */
                    flag = 2;
                    /*callf(told, xd, x, x,x,&flag);*/
                    callf(told, &scs_imp->blocks[*kf - 1], &flag);
                    if (flag < 0)
                    {
                        *ierr = 5 - flag;
                        return;
                    }
                }
            }
            else
            {
                if (*scs_imp->blocks[*kf - 1].work != NULL)
                {
                    flag = 2;
                    scs_imp->blocks[*kf - 1].nevprt = 0; /* in case some hidden continuous blocks need updating */
                    /*callf(told, xd, x, x,x,&flag);*/
                    callf(told, &scs_imp->blocks[*kf - 1], &flag);
                    if (flag < 0)
                    {
                        *ierr = 5 - flag;
                        return;
                    }
                }
            }
        }
    }
} /* ddoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine edoit */
static void edoit(double *told, int *kiwa)
{
    /* update blocks output on discrete activations */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int ierr1 = 0, i = 0;
    int kever = 0, ii = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;
    int nord = 0;

    scs_imp = getscicosimportptr();

    /* Function Body */
    kever = *pointi;

    *pointi = evtspt[kever];
    evtspt[kever] = -1;

    nord = scs_imp->ordptr[kever] - scs_imp->ordptr[kever - 1];
    if (nord == 0)
    {
        return;
    }
    iwa[*kiwa] = kever;
    ++(*kiwa);
    for (ii = scs_imp->ordptr[kever - 1]; ii <= scs_imp->ordptr[kever] - 1; ii++)
    {
        kf = &scs_imp->ordclk[ii - 1];
        C2F(curblk).kfun = *kf;

        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            /* continuous state */
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
            }

            scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);

            flag = 1;
            /*callf(told, xd, x, x,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        /* Initialize tvec */
        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                i = synchro_nev(scs_imp, *kf, ierr);
                if (*ierr != 0)
                {
                    return;
                }
                i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                edoit(told, kiwa);
                if (*ierr != 0)
                {
                    return;
                }
            }
        }
    }
} /* edoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine odoit */
static void odoit(double *told, double *xt, double *xtd, double *residual)
{
    /* update blocks derivative of continuous time block */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int keve = 0, kiwa = 0;
    int ierr1 = 0, i = 0;
    int ii = 0, jj = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    kiwa = 0;
    for (jj = 0; jj < * (scs_imp->noord); jj++)
    {
        kf = &scs_imp->oord[jj];
        C2F(curblk).kfun = *kf;
        /* continuous state */
        if (scs_imp->blocks[*kf - 1].nx > 0)
        {
            scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].res = &residual[scs_imp->xptr[*kf - 1] - 1];
        }

        scs_imp->blocks[*kf - 1].nevprt = scs_imp->oord[jj + * (scs_imp->noord)];
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            flag = 1;
            /*callf(told, xtd, xt, residual,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                if (scs_imp->blocks[*kf - 1].nmode > 0)
                {
                    i2 = scs_imp->blocks[*kf - 1].mode[0] + scs_imp->clkptr[*kf - 1] - 1;
                }
                else
                {
                    i = synchro_nev(scs_imp, *kf, ierr);
                    if (*ierr != 0)
                    {
                        return;
                    }
                    i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                }
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                ozdoit(told, xt, xtd, &kiwa);
                if (*ierr != 0)
                {
                    return;
                }
            }
        }
    }

    /*  update states derivatives */
    for (ii = 0; ii < * (scs_imp->noord); ii++)
    {
        kf = &scs_imp->oord[ii];
        C2F(curblk).kfun = *kf;
        if (scs_imp->blocks[*kf - 1].nx > 0 || \
                *scs_imp->blocks[*kf - 1].work != NULL)
        {
            /* work tests if a hidden state exists, used for delay block */
            flag = 0;
            /* continuous state */
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].res = &residual[scs_imp->xptr[*kf - 1] - 1];
            }
            scs_imp->blocks[*kf - 1].nevprt = scs_imp->oord[ii + * (scs_imp->noord)];
            /*callf(told, xtd, xt, residual,xt,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);

            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }
    }

    for (i = 0; i < kiwa; i++)
    {
        keve = iwa[i];
        for (ii = scs_imp->ordptr[keve - 1]; ii <= scs_imp->ordptr[keve] - 1; ii++)
        {
            kf = &scs_imp->ordclk[ii - 1];
            C2F(curblk).kfun = *kf;
            if (scs_imp->blocks[*kf - 1].nx > 0 || \
                    *scs_imp->blocks[*kf - 1].work != NULL)
            {
                /* work tests if a hidden state exists */
                flag = 0;
                /* continuous state */
                if (scs_imp->blocks[*kf - 1].nx > 0)
                {
                    scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].res = &residual[scs_imp->xptr[*kf - 1] - 1];
                }
                scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);
                /*callf(told, xtd, xt, residual,xt,&flag);*/
                callf(told, &scs_imp->blocks[*kf - 1], &flag);

                if (flag < 0)
                {
                    *ierr = 5 - flag;
                    return;
                }
            }
        }
    }
} /* odoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine reinitdoit */
static void reinitdoit(double *told)
{
    /* update blocks xproperties of continuous time block */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int keve = 0, kiwa = 0;
    int ierr1 = 0, i = 0;
    int ii = 0, jj = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    kiwa = 0;
    for (jj = 0; jj < * (scs_imp->noord); jj++)
    {
        kf = &scs_imp->oord[jj];
        C2F(curblk).kfun = *kf;
        /* continuous state */
        if (scs_imp->blocks[*kf - 1].nx > 0)
        {
            scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
        }
        scs_imp->blocks[*kf - 1].nevprt = scs_imp->oord[jj + * (scs_imp->noord)];
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            flag = 1;
            /*callf(told, xd, x, x,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        if (scs_imp->blocks[*kf - 1].nevout > 0 && scs_imp->funtyp[*kf - 1] < 0)
        {
            i = synchro_nev(scs_imp, *kf, ierr);
            if (*ierr != 0)
            {
                return;
            }
            if (scs_imp->blocks[*kf - 1].nmode > 0)
            {
                scs_imp->blocks[*kf - 1].mode[0] = i;
            }
            i2 = i + scs_imp->clkptr[*kf - 1] - 1;
            putevs(told, &i2, &ierr1);
            if (ierr1 != 0)
            {
                /* event conflict */
                *ierr = 3;
                return;
            }
            doit(told);
            if (*ierr != 0)
            {
                return;
            }
        }
    }

    /* reinitialize */
    for (ii = 0; ii < * (scs_imp->noord); ii++)
    {
        kf = &scs_imp->oord[ii];
        C2F(curblk).kfun = *kf;
        if (scs_imp->blocks[*kf - 1].nx > 0)
        {
            flag = 7;
            scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].res = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].nevprt = scs_imp->oord[ii + * (scs_imp->noord)];
            scs_imp->blocks[*kf - 1].xprop = &scs_imp->xprop[-1 + scs_imp->xptr[*kf - 1]];
            /*callf(told, xd, x, xd,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }
    }

    for (i = 0; i < kiwa; i++)
    {
        keve = iwa[i];
        for (ii = scs_imp->ordptr[keve - 1]; ii <= scs_imp->ordptr[keve] - 1; ii++)
        {
            kf = &scs_imp->ordclk[ii - 1];
            C2F(curblk).kfun = *kf;
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                flag = 7;
                scs_imp->blocks[*kf - 1].x  = &scs_imp->x[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].res = &scs_imp->xd[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);
                scs_imp->blocks[*kf - 1].xprop = &scs_imp->xprop[-1 + scs_imp->xptr[*kf - 1]];
                /*callf(told, xd, x, xd,x,&flag);*/
                callf(told, &scs_imp->blocks[*kf - 1], &flag);
                if (flag < 0)
                {
                    *ierr = 5 - flag;
                    return;
                }
            }
        }
    }
} /* reinitdoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine ozdoit */
static void ozdoit(double *told, double *xt, double *xtd, int *kiwa)
{
    /* update blocks output of continuous time block on discrete activations */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int nord = 0;
    int ierr1 = 0, i = 0;
    int ii = 0, kever = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    kever = *pointi;
    *pointi = evtspt[kever];
    evtspt[kever] = -1;

    nord = scs_imp->ordptr[kever] - scs_imp->ordptr[kever - 1];
    if (nord == 0)
    {
        return;
    }
    iwa[*kiwa] = kever;
    ++(*kiwa);

    for (ii = scs_imp->ordptr[kever - 1]; ii <= scs_imp->ordptr[kever] - 1; ii++)
    {
        kf = &scs_imp->ordclk[ii - 1];
        C2F(curblk).kfun = *kf;
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            /* continuous state */
            if (scs_imp->blocks[*kf - 1].nx > 0)
            {
                scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
            }
            scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);
            flag = 1;
            /*callf(told, xtd, xt, xt,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        /* Initialize tvec */
        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                if (phase == 1 || scs_imp->blocks[*kf - 1].nmode == 0)
                {
                    i = synchro_nev(scs_imp, *kf, ierr);
                    if (*ierr != 0)
                    {
                        return;
                    }
                }
                else
                {
                    i = scs_imp->blocks[*kf - 1].mode[0];
                }
                i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                ozdoit(told, xt, xtd, kiwa);
            }
        }
    }
} /* ozdoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine zdoit */
static void zdoit(double *told, double *xt, double *xtd, double *g)
{
    /* update blocks zcross of continuous time block  */
    /*     Copyright INRIA */
    int i2 = 0;
    scicos_flag flag = 0;
    int keve = 0, kiwa = 0;
    int ierr1 = 0, i = 0, j = 0;
    int ii = 0, jj = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    for (i = 0; i < * (scs_imp->ng); i++)
    {
        g[i] = 0.;
    }

    kiwa = 0;
    for (jj = 0; jj < * (scs_imp->nzord); jj++)
    {
        kf = &scs_imp->zord[jj];
        C2F(curblk).kfun = *kf;
        /* continuous state */
        if (scs_imp->blocks[*kf - 1].nx > 0)
        {
            scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
            scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
        }
        scs_imp->blocks[*kf - 1].nevprt = scs_imp->zord[jj + * (scs_imp->nzord)];

        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            flag = 1;
            /*callf(told, xtd, xt, xt,xt,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        /* Initialize tvec */
        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                if (phase == 1 || scs_imp->blocks[*kf - 1].nmode == 0)
                {
                    i = synchro_nev(scs_imp, *kf, ierr);
                    if (*ierr != 0)
                    {
                        return;
                    }
                }
                else
                {
                    i = scs_imp->blocks[*kf - 1].mode[0];
                }
                i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                ozdoit(told, xt, xtd, &kiwa);
                if (*ierr != 0)
                {
                    return;
                }
            }
        }
    }

    /* update zero crossing surfaces */
    for (ii = 0; ii < * (scs_imp->nzord); ii++)
    {
        kf = &scs_imp->zord[ii];
        C2F(curblk).kfun = *kf;
        if (scs_imp->blocks[*kf - 1].ng > 0)
        {
            /* update g array ptr */
            scs_imp->blocks[*kf - 1].g = &g[scs_imp->zcptr[*kf - 1] - 1];
            if (scs_imp->funtyp[*kf - 1] > 0)
            {
                flag = 9;
                /* continuous state */
                if (scs_imp->blocks[*kf - 1].nx > 0)
                {
                    scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                }
                scs_imp->blocks[*kf - 1].nevprt = scs_imp->zord[ii + * (scs_imp->nzord)];
                /*callf(told, xtd, xt, xtd,g,&flag);*/
                callf(told, &scs_imp->blocks[*kf - 1], &flag);
                if (flag < 0)
                {
                    *ierr = 5 - flag;
                    return;
                }
            }
            else
            {
                j = synchro_g_nev(scs_imp, g, *kf, ierr);
                if (*ierr != 0)
                {
                    return;
                }
                if ( (phase == 1) && (scs_imp->blocks[*kf - 1].nmode > 0) )
                {
                    scs_imp->blocks[*kf - 1].mode[0] = j;
                }
            }

            // scs_imp->blocks[*kf-1].g = &scs_imp->g[scs_imp->zcptr[*kf]-1];

        }
    }

    for (i = 0; i < kiwa; i++)
    {
        keve = iwa[i];
        for (ii = scs_imp->ordptr[keve - 1]; ii <= scs_imp->ordptr[keve] - 1; ii++)
        {
            kf = &scs_imp->ordclk[ii - 1];
            C2F(curblk).kfun = *kf;
            if (scs_imp->blocks[*kf - 1].ng > 0)
            {
                /* update g array ptr */
                scs_imp->blocks[*kf - 1].g = &g[scs_imp->zcptr[*kf - 1] - 1];
                if (scs_imp->funtyp[*kf - 1] > 0)
                {
                    flag = 9;
                    /* continuous state */
                    if (scs_imp->blocks[*kf - 1].nx > 0)
                    {
                        scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                        scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                    }
                    scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);
                    /*callf(told, xtd, xt, xtd,g,&flag);*/
                    callf(told, &scs_imp->blocks[*kf - 1], &flag);
                    if (flag < 0)
                    {
                        *ierr = 5 - flag;
                        return;
                    }
                }
                else
                {
                    j = synchro_g_nev(scs_imp, g, *kf, ierr);
                    if (*ierr != 0)
                    {
                        return;
                    }
                    if ((phase == 1) && (scs_imp->blocks[*kf - 1].nmode > 0))
                    {
                        scs_imp->blocks[*kf - 1].mode[0] = j;
                    }
                }

                //scs_imp->blocks[*kf-1].g = &scs_imp->g[scs_imp->zcptr[*kf]-1];
            }
        }
    }
} /* zdoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine Jdoit */
void Jdoit(double *told, double *xt, double *xtd, double *residual, int *job)
{
    /* update blocks jacobian of continuous time block  */
    /*     Copyright INRIA */

    int i2 = 0;
    scicos_flag flag = 0;
    int keve = 0, kiwa = 0;
    int ierr1 = 0, i = 0;
    int ii = 0, jj = 0;

    ScicosImport *scs_imp = NULL;
    int *kf = NULL;

    scs_imp = getscicosimportptr();

    /* Function Body */
    kiwa = 0;
    for (jj = 0; jj < * (scs_imp->noord); jj++)
    {
        kf = &scs_imp->oord[jj];
        C2F(curblk).kfun = *kf;
        scs_imp->blocks[*kf - 1].nevprt = scs_imp->oord[jj + * (scs_imp->noord)];
        if (scs_imp->funtyp[*kf - 1] > -1)
        {
            flag = 1;
            /* applying desired output */
            if ((*job == 2) && (scs_imp->oord[jj] == AJacobian_block))
            {
            }
            else
                /* continuous state */
                if (scs_imp->blocks[*kf - 1].nx > 0)
                {
                    scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].res = &residual[scs_imp->xptr[*kf - 1] - 1];
                }

            /*callf(told, xtd, xt, residual,x,&flag);*/
            callf(told, &scs_imp->blocks[*kf - 1], &flag);
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }

        if (scs_imp->blocks[*kf - 1].nevout > 0)
        {
            if (scs_imp->funtyp[*kf - 1] < 0)
            {
                if (scs_imp->blocks[*kf - 1].nmode > 0)
                {
                    i2 = scs_imp->blocks[*kf - 1].mode[0] + scs_imp->clkptr[*kf - 1] - 1;
                }
                else
                {
                    i = synchro_nev(scs_imp, *kf, ierr);
                    if (*ierr != 0)
                    {
                        return;
                    }
                    i2 = i + scs_imp->clkptr[*kf - 1] - 1;
                }
                putevs(told, &i2, &ierr1);
                if (ierr1 != 0)
                {
                    /* event conflict */
                    *ierr = 3;
                    return;
                }
                ozdoit(told, xt, xtd, &kiwa);
            }
        }
    }

    /* update states derivatives */
    for (ii = 0; ii < * (scs_imp->noord); ii++)
    {
        kf = &scs_imp->oord[ii];
        C2F(curblk).kfun = *kf;
        if (scs_imp->blocks[*kf - 1].nx > 0 || \
                *scs_imp->blocks[*kf - 1].work != NULL)
        {
            /* work tests if a hidden state exists, used for delay block */
            flag = 0;
            if (((*job == 1) && (scs_imp->oord[ii] == AJacobian_block)) || (*job != 1))
            {
                if (*job == 1)
                {
                    flag = 10;
                }
                /* continuous state */
                if (scs_imp->blocks[*kf - 1].nx > 0)
                {
                    scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                    scs_imp->blocks[*kf - 1].res = &residual[scs_imp->xptr[*kf - 1] - 1];
                }
                scs_imp->blocks[*kf - 1].nevprt = scs_imp->oord[ii + * (scs_imp->noord)];
                /*callf(told, xtd, xt, residual,xt,&flag);*/
                callf(told, &scs_imp->blocks[*kf - 1], &flag);
            }
            if (flag < 0)
            {
                *ierr = 5 - flag;
                return;
            }
        }
    }

    for (i = 0; i < kiwa; i++)
    {
        keve = iwa[i];
        for (ii = scs_imp->ordptr[keve - 1]; ii <= scs_imp->ordptr[keve] - 1;  ii++)
        {
            kf = &scs_imp->ordclk[ii - 1];
            C2F(curblk).kfun = *kf;
            if (scs_imp->blocks[*kf - 1].nx > 0 || \
                    *scs_imp->blocks[*kf - 1].work != NULL)
            {
                /* work tests if a hidden state exists */
                flag = 0;
                if (((*job == 1) && (scs_imp->oord[ii - 1] == AJacobian_block)) || (*job != 1))
                {
                    if (*job == 1)
                    {
                        flag = 10;
                    }
                    /* continuous state */
                    if (scs_imp->blocks[*kf - 1].nx > 0)
                    {
                        scs_imp->blocks[*kf - 1].x  = &xt[scs_imp->xptr[*kf - 1] - 1];
                        scs_imp->blocks[*kf - 1].xd = &xtd[scs_imp->xptr[*kf - 1] - 1];
                        scs_imp->blocks[*kf - 1].res = &residual[scs_imp->xptr[*kf - 1] - 1];
                    }
                    scs_imp->blocks[*kf - 1].nevprt = abs(scs_imp->ordclk[ii + * (scs_imp->nordclk) - 1]);
                    /*callf(told, xtd, xt, residual,xt,&flag);*/
                    callf(told, &scs_imp->blocks[*kf - 1], &flag);
                }
                if (flag < 0)
                {
                    *ierr = 5 - flag;
                    return;
                }
            }
        }
    }
} /* Jdoit_ */
/*--------------------------------------------------------------------------*/
/* Subroutine synchro_nev */
static int synchro_nev(ScicosImport *scs_imp, int kf, int *ierr)
{
    /* synchro blocks computation  */
    /*     Copyright INRIA */
    SCSREAL_COP *outtbdptr = NULL;     /*to store double of outtb*/
    SCSINT8_COP *outtbcptr = NULL;     /*to store int8 of outtb*/
    SCSINT16_COP *outtbsptr = NULL;    /*to store int16 of outtb*/
    SCSINT32_COP *outtblptr = NULL;    /*to store int32 of outtb*/
    SCSUINT8_COP *outtbucptr = NULL;   /*to store unsigned int8 of outtb */
    SCSUINT16_COP *outtbusptr = NULL;  /*to store unsigned int16 of outtb */
    SCSUINT32_COP *outtbulptr = NULL;  /*to store unsigned int32 of outtb */

    int cond = 0;
    int i = 0; /* return 0 by default */

    /* variable for param */
    int *outtbtyp = 0;
    void **outtbptr = NULL;
    int *funtyp = 0;
    int *inplnk = 0;
    int *inpptr = 0;

    /* get param ptr */
    outtbtyp = scs_imp->outtbtyp;
    outtbptr = scs_imp->outtbptr;
    funtyp = scs_imp->funtyp;
    inplnk = scs_imp->inplnk;
    inpptr = scs_imp->inpptr;

    /* if-then-else blk */
    if (funtyp[kf - 1] == -1)
    {
        switch (outtbtyp[-1 + inplnk[inpptr[kf - 1] - 1]])
        {
            case SCSREAL_N    :
                outtbdptr = (SCSREAL_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbdptr <= 0.);
                break;

            case SCSCOMPLEX_N :
                outtbdptr = (SCSCOMPLEX_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbdptr <= 0.);
                break;

            case SCSINT8_N    :
                outtbcptr = (SCSINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbcptr <= 0);
                break;

            case SCSINT16_N   :
                outtbsptr = (SCSINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbsptr <= 0);
                break;

            case SCSINT32_N   :
                outtblptr = (SCSINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtblptr <= 0);
                break;

            case SCSUINT8_N   :
                outtbucptr = (SCSUINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbucptr <= 0);
                break;

            case SCSUINT16_N  :
                outtbusptr = (SCSUINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbusptr <= 0);
                break;

            case SCSUINT32_N  :
                outtbulptr = (SCSUINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                cond = (*outtbulptr <= 0);
                break;

            default  : /* Add a message here */
                *ierr = 25;
                return 0;
        }
        if (cond)
        {
            i = 2;
        }
        else
        {
            i = 1;
        }
    }
    /* eselect blk */
    else if (funtyp[kf - 1] == -2)
    {
        switch (outtbtyp[-1 + inplnk[inpptr[kf - 1] - 1]])
        {
            case SCSREAL_N    :
                outtbdptr = (SCSREAL_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbdptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSCOMPLEX_N :
                outtbdptr = (SCSCOMPLEX_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbdptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSINT8_N    :
                outtbcptr = (SCSINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbcptr,
                            scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSINT16_N   :
                outtbsptr = (SCSINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbsptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSINT32_N   :
                outtblptr = (SCSINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtblptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSUINT8_N   :
                outtbucptr = (SCSUINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbucptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSUINT16_N  :
                outtbusptr = (SCSUINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbusptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSUINT32_N  :
                outtbulptr = (SCSUINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                i = Max(Min((int) * outtbulptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            default  : /* Add a message here */
                *ierr = 25;
                return 0;
        }
    }
    return i;
} /* synchro_nev */
/*--------------------------------------------------------------------------*/
/* Subroutine synchro_g_nev */
static int synchro_g_nev(ScicosImport *scs_imp, double *g, int kf, int *ierr)
{
    /* synchro blocks with zcross computation  */
    /*     Copyright INRIA */
    SCSREAL_COP *outtbdptr = NULL;     /*to store double of outtb*/
    SCSINT8_COP *outtbcptr = NULL;     /*to store int8 of outtb*/
    SCSINT16_COP *outtbsptr = NULL;    /*to store int16 of outtb*/
    SCSINT32_COP *outtblptr = NULL;    /*to store int32 of outtb*/
    SCSUINT8_COP *outtbucptr = NULL;   /*to store unsigned int8 of outtb */
    SCSUINT16_COP *outtbusptr = NULL;  /*to store unsigned int16 of outtb */
    SCSUINT32_COP *outtbulptr = NULL;  /*to store unsigned int32 of outtb */

    int cond = 0;
    int i = 0; /* return 0 by default */
    int jj = 0;

    /* variable for param */
    int *outtbtyp = NULL;
    void **outtbptr = NULL;
    int *funtyp = NULL;
    int *inplnk = NULL;
    int *inpptr = NULL;
    int *zcptr = NULL;

    /* get param ptr */
    outtbtyp = scs_imp->outtbtyp;
    outtbptr = scs_imp->outtbptr;
    funtyp = scs_imp->funtyp;
    inplnk = scs_imp->inplnk;
    inpptr = scs_imp->inpptr;
    zcptr = scs_imp->zcptr;

    /* if-then-else blk */
    if (funtyp[kf - 1] == -1)
    {
        switch (outtbtyp[-1 + inplnk[inpptr[kf - 1] - 1]])
        {
            case SCSREAL_N    :
                outtbdptr = (SCSREAL_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = *outtbdptr;
                cond = (*outtbdptr <= 0.);
                break;

            case SCSCOMPLEX_N :
                outtbdptr = (SCSCOMPLEX_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = *outtbdptr;
                cond = (*outtbdptr <= 0.);
                break;

            case SCSINT8_N    :
                outtbcptr = (SCSINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = (double) * outtbcptr;
                cond = (*outtbcptr <= 0);
                break;

            case SCSINT16_N   :
                outtbsptr = (SCSINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = (double) * outtbsptr;
                cond = (*outtbsptr <= 0);
                break;

            case SCSINT32_N   :
                outtblptr = (SCSINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = (double) * outtblptr;
                cond = (*outtblptr <= 0);
                break;

            case SCSUINT8_N   :
                outtbucptr = (SCSUINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = (double) * outtbucptr;
                cond = (*outtbucptr <= 0);
                break;

            case SCSUINT16_N  :
                outtbusptr = (SCSUINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = (double) * outtbusptr;
                cond = (*outtbusptr <= 0);
                break;

            case SCSUINT32_N  :
                outtbulptr = (SCSUINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                g[zcptr[kf - 1] - 1] = (double) * outtbulptr;
                cond = (*outtbulptr <= 0);
                break;

            default  : /* Add a message here */
                *ierr = 25;
                return 0;
        }
        if (cond)
        {
            i = 2;
        }
        else
        {
            i = 1;
        }
    }
    /* eselect blk */
    else if (funtyp[kf - 1] == -2)
    {
        switch (outtbtyp[-1 + inplnk[inpptr[kf - 1] - 1]])
        {
            case SCSREAL_N    :
                outtbdptr = (SCSREAL_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = *outtbdptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbdptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSCOMPLEX_N :
                outtbdptr = (SCSCOMPLEX_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = *outtbdptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbdptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSINT8_N    :
                outtbcptr = (SCSINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = (double) * outtbcptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbcptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSINT16_N   :
                outtbsptr = (SCSINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = (double) * outtbsptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbsptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSINT32_N   :
                outtblptr = (SCSINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = (double) * outtblptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtblptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSUINT8_N   :
                outtbucptr = (SCSUINT8_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = (double) * outtbucptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbucptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSUINT16_N  :
                outtbusptr = (SCSUINT16_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = (double) * outtbusptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbusptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            case SCSUINT32_N  :
                outtbulptr = (SCSUINT32_COP *)outtbptr[-1 + inplnk[inpptr[kf - 1] - 1]];
                for (jj = 0; jj < scs_imp->blocks[kf - 1].nevout - 1; jj++)
                {
                    g[zcptr[kf - 1] - 1 + jj] = (double) * outtbulptr - (double)(jj + 2);
                }
                i = Max(Min((int) * outtbulptr, scs_imp->blocks[kf - 1].nevout), 1);
                break;

            default  : /* Add a message here */
                *ierr = 25;
                return 0;
        }
    }
    return i;
} /* synchro_g_nev */
/*--------------------------------------------------------------------------*/
/* FREE_blocks */
static void FREE_blocks()
{
    int kf = 0;
    for (kf = 0; kf < nblk; ++kf)
    {
        if (Blocks[kf].insz != NULL)
        {
            FREE(Blocks[kf].insz);
        }
        else
        {
            break;
        }
        if (Blocks[kf].inptr != NULL)
        {
            FREE(Blocks[kf].inptr);
        }
        else
        {
            break;
        }
        if (Blocks[kf].outsz != NULL)
        {
            FREE(Blocks[kf].outsz);
        }
        else
        {
            break;
        }
        if (Blocks[kf].outptr != NULL)
        {
            FREE(Blocks[kf].outptr);
        }
        else
        {
            break;
        }
        if (Blocks[kf].oparsz != NULL)
        {
            FREE(Blocks[kf].oparsz);
        }
        else
        {
            break;
        }
        if (Blocks[kf].ozsz != NULL)
        {
            FREE(Blocks[kf].ozsz);
        }
        else
        {
            break;
        }
        if (Blocks[kf].label != NULL)
        {
            FREE(Blocks[kf].label);
        }
        else
        {
            break;
        }
        if (Blocks[kf].uid != NULL)
        {
            FREE(Blocks[kf].uid);
        }
        else
        {
            break;
        }
        if (Blocks[kf].evout != NULL)
        {
            FREE(Blocks[kf].evout);
        }
        else
        {
            break;
        }
    }
    FREE(Blocks);

    if (nx > 0)
    {
        FREE(xprop);
    }

    if (nmod > 0)
    {
        FREE(mod);
    }

    if (ng > 0)
    {
        FREE(g);
    }

    return;
} /* FREE_blocks */
/*--------------------------------------------------------------------------*/
/* Subroutine funnum */
int C2F(funnum)(char * fname)
{
    int i = 0, ln = 0;
    int loc = -1;
    while (tabsim[i].name != (char *)NULL)
    {
        if (strcmp(fname, tabsim[i].name) == 0)
        {
            return (i + 1);
        }
        i++;
    }
    ln = (int)strlen(fname);
    //C2F(iislink)(fname, &loc);
    //C2F(iislink)(fname, &loc);
    if (loc >= 0)
    {
        return (ntabsim + (int)loc + 1);
    }
    return (0);
}/* funnum */
/*--------------------------------------------------------------------------*/
/* Subroutine funnum2 */
void* funnum2(char * fname)
{
    int i = 0;
    while (tabsim[i].name != (char *)NULL)
    {
        if (strcmp(fname, tabsim[i].name) == 0)
        {
            return (void*)tabsim[i].fonc;
        }
        i++;
    }
    return NULL;
}/* funnum2 */
/*--------------------------------------------------------------------------*/
int get_phase_simulation(void)
{
    return phase;
}
/*--------------------------------------------------------------------------*/
void do_cold_restart(void)
{
    hot = 0;
    return;
}
/*--------------------------------------------------------------------------*/
/* get_scicos_time : return the current
* simulation time
*/
double get_scicos_time(void)
{
    return scicos_time;
}
/*--------------------------------------------------------------------------*/
/*! \brief set the current simulation time before calling blocks
 *
 * As some of the blocks call get_scicos_time(), this is the only way to force
 * a local time for these blocks. This call does not modify the Xcos solver
 * time but is only used to step to a future point while calling blocks.
 */
void set_scicos_time(double t)
{
    scicos_time = t;
}
/*--------------------------------------------------------------------------*/
/* get_block_number : return the current
* block number
*/
int get_block_number(void)
{
    return C2F(curblk).kfun;
}
/*--------------------------------------------------------------------------*/
/* set_block_error : set an error number
* for block_error
*/
void set_block_error(int err)
{
    *block_error = err;
}
/*--------------------------------------------------------------------------*/
/* Coserror : copy an error message
* in coserr.buf an set block_error to
* -16
*/
#if _MSC_VER
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif
#endif


void Coserror(const char *fmt, ...)
{
    int retval;
    va_list ap;

    va_start(ap, fmt);

#ifdef vsnprintf
    retval = vsnprintf(coserr.buf, COSERR_len, fmt, ap);
#else
    retval = vsprintf(coserr.buf, fmt, ap);
#endif

    if (retval == -1)
    {
        coserr.buf[0] = '\0';
    }

    va_end(ap);

    /* coserror use error number 10 */
    *block_error = -5;
}
/*--------------------------------------------------------------------------*/
/* SundialsErrHandler: in case of a Sundials error,
* call Coserror() to write it in coserr.buf
*
* The unused parameters are there to square with Sundials' IDA error function, for better genericity.
*/
void SundialsErrHandler(int error_code, const char *module, const char *function, char *msg, void *user_data)
{
    Coserror("%s: %s", function, msg);
}
/*--------------------------------------------------------------------------*/
/* get_block_error : get the block error
* number
*/
int get_block_error()
{
    return *block_error;
}
/*--------------------------------------------------------------------------*/
void end_scicos_sim()
{
    C2F(coshlt).halt = 2;
    return;
}
/*--------------------------------------------------------------------------*/
/* get_pointer_xproperty */
int* get_pointer_xproperty()
{
    return &xprop[-1 + xptr[C2F(curblk).kfun]];
}
/*--------------------------------------------------------------------------*/
/* get_Npointer_xproperty */
int get_npointer_xproperty()
{
    return Blocks[C2F(curblk).kfun - 1].nx;
}
/*--------------------------------------------------------------------------*/
/* set_pointer_xproperty */
void set_pointer_xproperty(int* pointer)
{
    int i;
    for (i = 0; i < Blocks[C2F(curblk).kfun - 1].nx; i++)
    {
        Blocks[C2F(curblk).kfun - 1].xprop[i] = pointer[i];
    }
}
/*--------------------------------------------------------------------------*/
/* Jacobian */
void Set_Jacobian_flag(int flag)
{
    Jacobian_Flag = flag;
    return;
}
/*--------------------------------------------------------------------------*/
/* Get_Jacobian_ci et Get_Jacobian_cj were called by the C file only produced
by Modelicac v 1.11.2 */
/* double Get_Jacobian_ci(void)
{
return CI;
} */
/*--------------------------------------------------------------------------*/
/* double Get_Jacobian_cj(void)
{
return CJ;
} */
/*--------------------------------------------------------------------------*/
/* Function called by the C file produced by Modelicac 1.7.3 and 1.12.1 */
double Get_Jacobian_parameter(void)
{
    return CJJ;
}
/*--------------------------------------------------------------------------*/
double Get_Scicos_SQUR(void)
{
    return  SQuround;
}
/*--------------------------------------------------------------------------*/
static int Jacobians(long int Neq, realtype tt, realtype cj, N_Vector yy,
                     N_Vector yp, N_Vector resvec, DlsMat Jacque, void *jdata,
                     N_Vector tempv1, N_Vector tempv2, N_Vector tempv3)
{
    double  ttx = 0;
    double *xc = NULL, *xcdot = NULL, *residual = NULL;
    /*  char chr;*/
    int i = 0, j = 0, n = 0, nx = 0, ni = 0, no = 0, nb = 0, m = 0, flag = 0;
    double *RX = NULL, *Fx = NULL, *Fu = NULL, *Gx = NULL, *Gu = NULL, *ERR1 = NULL, *ERR2 = NULL;
    double *Hx = NULL, *Hu = NULL, *Kx = NULL, *Ku = NULL, *HuGx = NULL, *FuKx = NULL, *FuKuGx = NULL, *HuGuKx = NULL;
    double ysave = 0;
    int job = 0;
    double **y = NULL;
    double **u = NULL;
    /*  taill1= 3*n+(n+ni)*(n+no)+nx(2*nx+ni+2*m+no)+m*(2*m+no+ni)+2*ni*no*/
    double inc = 0., inc_inv = 0., xi = 0., xpi = 0., srur = 0.;
    realtype *Jacque_col = NULL;

    UserData data;
    realtype hh = 0.;
    N_Vector ewt;
    double *ewt_data = NULL;

    *ierr = 0;

    data = (UserData) jdata;
    ewt = data->ewt;

    flag = IDAGetCurrentStep(data->dae_mem, &hh);
    if (flag < 0)
    {
        *ierr = 200 + (-flag);
        return (*ierr);
    };

    flag = IDAGetErrWeights(data->dae_mem, ewt);
    if (flag < 0)
    {
        *ierr = 200 + (-flag);
        return (*ierr);
    };

    ewt_data = NV_DATA_S(ewt);
    xc   = (double *) N_VGetArrayPointer(yy);
    xcdot  = (double *) N_VGetArrayPointer(yp);
    /*residual=(double *) NV_DATA_S(resvec);*/
    ttx = (double)tt;
    // CJ=(double)cj;  // for fonction Get_Jacobian_cj
    CJJ = (double)cj;  // returned by Get_Jacobian_parameter

    srur = (double) RSqrt(UNIT_ROUNDOFF);

    if (AJacobian_block > 0)
    {
        nx = Blocks[AJacobian_block - 1].nx; /* quant on est là cela signifie que AJacobian_block>0 */
        no = Blocks[AJacobian_block - 1].nout;
        ni = Blocks[AJacobian_block - 1].nin;
        y = (double **)Blocks[AJacobian_block - 1].outptr; /*for compatibility */
        u = (double **)Blocks[AJacobian_block - 1].inptr; /*warning pointer of y and u have changed to void ***/
    }
    else
    {
        nx = 0;
        no = 0;
        ni = 0;
    }
    n = Neq;
    nb = nblk;
    m = n - nx;

    residual = (double *)data->rwork;
    ERR1 = residual + n;
    ERR2 = ERR1 + n;
    RX = ERR2 + n;
    Fx = RX + (n + ni) * (n + no); /* car (nx+ni)*(nx+no) peut etre > `a n*n*/
    Fu = Fx + nx * nx;
    Gx = Fu + nx * ni;
    Gu = Gx + no * nx;
    Hx = Gu + no * ni;
    Hu = Hx + m * m;
    Kx = Hu + m * no;
    Ku = Kx + ni * m;
    HuGx = Ku + ni * no;
    FuKx = HuGx + m * nx;
    FuKuGx = FuKx + nx * m;
    HuGuKx = FuKuGx + nx * nx;
    /* HuGuKx+m*m; =>  m*m=size of HuGuKx */
    /* ------------------ Numerical Jacobian--->> Hx,Kx */

    /* read residuals;*/
    job = 0;
    Jdoit(&ttx, xc, xcdot, residual, &job);
    if (*ierr < 0)
    {
        return -1;
    }

    /* "residual" already contains the current residual,
    so the first call to Jdoit can be removed */

    for (i = 0; i < m; i++)
        for (j = 0; j < ni; j++)
        {
            Kx[j + i * ni] = u[j][0];
        }

    for (i = 0; i < m; i++)
    {
        xi = xc[i];
        xpi = xcdot[i];
        inc = MAX( srur * MAX( ABS(xi), ABS(hh * xpi)), ONE / ewt_data[i] );
        if (hh * xpi < ZERO)
        {
            inc = -inc;
        }
        inc = (xi + inc) - xi;

        /* if (CI==0) {
        inc = MAX( srur * ABS(hh*xpi),ONE );
        if (hh*xpi < ZERO) inc = -inc;
        inc = (xpi + inc) - xi;
        } */
        // xc[i] += CI*inc;
        // xcdot[i] += CJ*inc;
        xc[i] += inc;
        xcdot[i] += CJJ * inc;
        /*a= Max(abs(H[0]*xcdot[i]),abs(1.0/Ewt[i]));
        b= Max(1.0,abs(xc[i]));
        del=SQUR[0]*Max(a,b);    */
        job = 0; /* read residuals */
        Jdoit(&ttx, xc, xcdot, ERR2, &job);
        if (*ierr < 0)
        {
            return -1;
        }
        inc_inv = ONE / inc;
        for (j = 0; j < m; j++)
        {
            Hx[m * i + j] = (ERR2[j] - residual[j]) * inc_inv;
        }
        for (j = 0; j < ni; j++)
        {
            Kx[j + i * ni] = (u[j][0] - Kx[j + i * ni]) * inc_inv;
        }
        xc[i] = xi;
        xcdot[i] = xpi;
    }
    /*----- Numerical Jacobian--->> Hu,Ku */

    if (AJacobian_block == 0)
    {
        for (j = 0; j < m; j++)
        {
            Jacque_col = DENSE_COL(Jacque, j);
            for (i = 0; i < m; i++)
            {
                Jacque_col[i] = Hx[i + j * m];
            }
        }
        C2F(ierode).iero = *ierr;
        return 0;
    }
    /****------------------***/
    job = 0;
    Jdoit(&ttx, xc, xcdot, ERR1, &job);
    for (i = 0; i < no; i++)
        for (j = 0; j < ni; j++)
        {
            Ku[j + i * ni] = u[j][0];
        }

    for (i = 0; i < no; i++)
    {
        ysave = y[i][0];
        inc = srur * MAX( ABS(ysave), 1);
        inc = (ysave + inc) - ysave;
        /*del=SQUR[0]* Max(1.0,abs(y[i][0]));
        del=(y[i][0]+del)-y[i][0];*/
        y[i][0] += inc;
        job = 2; /* applying y[i][0] to the output of imp block*/
        Jdoit(&ttx, xc, xcdot, ERR2, &job);
        if (*ierr < 0)
        {
            return -1;
        }
        inc_inv = ONE / inc;
        for (j = 0; j < m; j++)
        {
            Hu[m * i + j] = (ERR2[j] - ERR1[j]) * inc_inv;
        }
        for (j = 0; j < ni; j++)
        {
            Ku[j + i * ni] = (u[j][0] - Ku[j + i * ni]) * inc_inv;
        }
        y[i][0] = ysave;
    }
    /*----------------------------------------------*/
    job = 1; /* read jacobian through flag=10; */
    *block_error = 0;
    Jdoit(&ttx, xc, xcdot, &Fx[-m], &job);/* Filling up the FX:Fu:Gx:Gu*/
    if (*block_error != 0)
    {
        sciprint(_("\n error in Jacobian"));
    }
    /*-------------------------------------------------*/

    Multp(Fu, Ku, RX, nx, ni, ni, no);
    Multp(RX, Gx, FuKuGx, nx, no, no, nx);

    for (j = 0; j < nx; j++)
    {
        Jacque_col = DENSE_COL(Jacque, j + m);
        for (i = 0; i < nx; i++)
        {
            Jacque_col[i + m] = Fx[i + j * nx] + FuKuGx[i + j * nx];
        }
    }

    Multp(Hu, Gx, HuGx, m, no, no, nx);

    for (i = 0; i < nx; i++)
    {
        Jacque_col = DENSE_COL(Jacque, i + m);
        for (j = 0; j < m; j++)
        {
            Jacque_col[j] = HuGx[j + i * m];
        }
    }

    Multp(Fu, Kx, FuKx, nx, ni, ni, m);

    for (i = 0; i < m; i++)
    {
        Jacque_col = DENSE_COL(Jacque, i);
        for (j = 0; j < nx; j++)
        {
            Jacque_col[j + m] = FuKx[j + i * nx];
        }
    }


    Multp(Hu, Gu, RX, m, no, no, ni);
    Multp(RX, Kx, HuGuKx, m, ni, ni, m);

    for (j = 0; j < m; j++)
    {
        Jacque_col = DENSE_COL(Jacque, j);
        for (i = 0; i < m; i++)
        {
            Jacque_col[i] = Hx[i + j * m] + HuGuKx[i + j * m];
        }
    }

    /*  chr='Z';   printf("\n t=%g",ttx); DISP(Z,n,n,chr);*/
    C2F(ierode).iero = *ierr;
    return 0;

}
/*--------------------------------------------------------------------------*/
static void Multp(double *A, double *B, double *R, int ra, int rb, int ca, int cb)
{
    int i = 0, j = 0, k = 0;
    /*if (ca!=rb) sciprint(_("\n Error in matrix multiplication"));*/
    for (i = 0; i < ra; i++)
        for (j = 0; j < cb; j++)
        {
            R[i + ra * j] = 0.0;
            for (k = 0; k < ca; k++)
            {
                R[i + ra * j] += A[i + k * ra] * B[k + j * rb];
            }
        }
    return;
}
/*--------------------------------------------------------------------------*/
int read_xml_initial_states(int nvar, const char * xmlfile, char **ids, double *svars)
{
    ezxml_t model, elements;
    int result = 0, i = 0;
    double vr = 0.;

    if (nvar == 0)
    {
        return 0;
    }
    result = 0;
    for (i = 0; i < nvar; i++)
    {
        if (strcmp(ids[i], "") != 0)
        {
            result = 1;
            break;
        }
    }
    if (result == 0)
    {
        return 0;
    }

    model = ezxml_parse_file(xmlfile);

    if (model == NULL)
    {
        sciprint(_("Error: Cannot find file '%s'.\n"), xmlfile);
        return -1;/* file does not exist*/
    }

    elements = ezxml_child(model, "elements");
    for (i = 0; i < nvar; i++)
    {
        vr = 0.0;
        result = read_id(&elements, ids[i], &vr);
        if (result == 1)
        {
            svars[i] = vr;
        }
    }
    ezxml_free(model);
    return 0;
}
/*--------------------------------------------------------------------------*/
static int read_id(ezxml_t *elements, char *id, double *value)
{
    char V1[100], V2[100];
    int ok = 0, i = 0, ln = 0;

    if (strcmp(id, "") == 0)
    {
        return 0;
    }
    ok = search_in_child(elements, id, V1);
    if (ok == 0 )
    {
        /*sciprint(_("Cannot find: %s=%s  \n"),id,V1);      */
        return 0;
    }
    else
    {
        if (Convert_number(V1, value) != 0)
        {
            ln = (int)(strlen(V1));
            if (ln > 2)
            {
                for (i = 1; i <= ln - 2; i++)
                {
                    V2[i - 1] = V1[i];
                }
                V2[ln - 2] = '\0';
                ok = read_id(elements, V2, value);
                return ok;
            }
            else
            {
                return 0;
            }
        }
        else
        {
            /*      printf("\n ---->>>%s= %g",V1,*value);*/
            return 1;
        }
    }
}
/*--------------------------------------------------------------------------*/
int Convert_number(char *s, double *out)
{
    char *endp = NULL;
    double d = 0.;
    long int l = 0;
    d = strtod(s, &endp);
    if (s != endp && *endp == '\0')
    {
        /*    printf("  It's a float with value %g ", d); */
        *out = d;
        return 0;
    }
    else
    {
        l = strtol(s, &endp, 0);
        if (s != endp && *endp == '\0')
        {
            /*printf("  It's an int with value %ld ", 1); */
            *out = (double)l;
            return 0;
        }
        else
        {
            /*printf("  string "); */
            return -1;
        }
    }
}
/*--------------------------------------------------------------------------*/
int write_xml_states(int nvar, const char * xmlfile, char **ids, double *x)
{
    ezxml_t model, elements;
    int result = 0, i = 0, err = 0;
    FILE *fd = NULL;
    char *s = NULL;
    char **xv = NULL;

    if (nvar == 0)
    {
        return 0;
    }
    result = 0;
    for (i = 0; i < nvar; i++)
    {
        if (strcmp(ids[i], "") != 0)
        {
            result = 1;
            break;
        }
    }
    if (result == 0)
    {
        return 0;
    }

    xv = MALLOC(nvar * sizeof(char*));
    for (i = 0; i < nvar; i++)
    {
        xv[i] = MALLOC(nvar * 100 * sizeof(char));
        sprintf(xv[i], "%g", x[i]);
    }

    model = ezxml_parse_file(xmlfile);
    if (model == NULL)
    {
        sciprint(_("Error: Cannot find file '%s'.\n"), xmlfile);
        err = -1;
        goto err_free_xv;
    }

    elements = ezxml_child(model, "elements");

    for (i = 0; i < nvar; i++)
    {
        if (strcmp(ids[i], "") == 0)
        {
            continue;
        }
        result = write_in_child(&elements, ids[i], xv[i]);
        if (result == 0 )
        {
            /* sciprint(_("cannot find %s in '%s' \n"),ids[i],xmlfile);      */
            /* err= -1;*/ /* Variable does not exist*/
        }
    }

    s = ezxml_toxml(model);
    ezxml_free(model);


    wcfopen(fd, (char*)xmlfile, "wb");
    if (fd == NULL)
    {
        err = -3;/* cannot write to file*/
        goto err_free_s;
    }

    fputs (s, fd);
    fclose(fd);

err_free_s:
    free(s);
err_free_xv:
    for (i = 0; i < nvar; i++)
    {
        FREE(xv[i]);
    }
    FREE(xv);
    return err;
}
/*--------------------------------------------------------------------------*/
int C2F(fx)(double *x, double *residual) /* used for homotopy*/
{
    double  *xdot = NULL, t = 0;
    xdot = x + *neq;
    t = 0;
    *ierr = 0;
    C2F(ierode).iero = 0;
    odoit(&t, x, xdot, residual);
    C2F(ierode).iero = *ierr;
    return (*ierr);
}
/*--------------------------------------------------------------------------*/
int rho_(double *a, double *L, double *x, double *rho, double *rpar, int *ipar) /* used for homotopy*/
{
    int i = 0, N = 0;
    N = *neq;

    fx_(x, rho);
    for (i = 0; i < N; i++)
    {
        rho[i] += (-1 + *L) * a[i];
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int rhojac_(double *a, double *lambda, double  *x, double  *jac, int *col, double *rpar, int *ipar) /* used for homotopy*/
{
    /* MATRIX [d_RHO/d_LAMBDA, d_RHO/d_X_col] */
    int j = 0, N = 0;
    double *work = NULL;
    double inc = 0., inc_inv = 0., xi = 0., srur = 0.;
    N = *neq;
    if (*col == 1)
    {
        for (j = 0; j < N; j++)
        {
            jac[j] = a[j];
        }
    }
    else
    {
        if ((work = (double *) MALLOC(N * sizeof(double))) == NULL)
        {
            *ierr = 10000;
            return *ierr;
        }
        rho_(a, lambda, x, work, rpar, ipar);
        srur = 1e-10;
        xi = x[*col - 2];
        inc = srur * Max(fabs(xi), 1);
        inc = (xi + inc) - xi;
        x[*col - 2] += inc;

        rho_(a, lambda, x, jac, rpar, ipar);
        inc_inv = 1.0 / inc;

        for (j = 0; j < N; j++)
        {
            jac[j] = (jac[j] - work[j]) * inc_inv;
        }

        x[*col - 2] = xi;
        FREE(work);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(hfjac)(double *x, double *jac, int *col)
{
    int N = 0, j = 0;
    double *work = NULL;
    double  *xdot = NULL;
    double inc = 0., inc_inv = 0., xi = 0., srur = 0.;

    N = *neq;
    if ((work = (double *) MALLOC(N * sizeof(double))) == NULL)
    {
        *ierr = 10000;
        return *ierr;
    }
    srur = (double) RSqrt(UNIT_ROUNDOFF);

    fx_(x, work);

    xi = x[*col - 1];
    inc = srur * MAX (ABS(xi), 1);
    inc = (xi + inc) - xi;
    x[*col - 1] += inc;
    xdot = x + N;

    fx_(x, jac);
    if (*ierr < 0)
    {
        FREE(work);
        return *ierr;
    }

    inc_inv = ONE / inc;

    for (j = 0; j < N; j++)
    {
        jac[j] = (jac[j] - work[j]) * inc_inv;
    }

    x[*col - 1] = xi;

    FREE(work);
    return 0;
}
/*--------------------------------------------------------------------------*/
int simblkKinsol(N_Vector yy, N_Vector resval, void *rdata)
{
    double t = 0., *xc = NULL , *xcdot = NULL, *residual = NULL;
    UserData data;
    int jj = 0, nantest = 0, N = 0;
    N = *neq;

    t = 0;
    xc = (double *)  NV_DATA_S(yy);
    residual = (double *) NV_DATA_S(resval);
    data = (UserData) rdata;
    xcdot = xc;
    if (phase == 1) if ( ng > 0 && nmod > 0 )
        {
            zdoit(&t, xc, xcdot, g);
        }

    *ierr = 0;
    C2F(ierode).iero = 0;
    odoit(&t, xc, xcdot, residual);

    if (*ierr == 0)
    {
        nantest = 0; /* NaN checking */
        for (jj = 0; jj < N; jj++)
        {
            if (residual[jj] - residual[jj] != 0)
            {
                Sciwarning(_("\nWarning: The initialization system #%d returns a NaN/Inf"), jj);
                nantest = 1;
                break;
            }
        }
        if (nantest == 1)
        {
            return 258; /* recoverable error; */
        }
    }
    C2F(ierode).iero = *ierr;

    return (abs(*ierr)); /* ierr>0 recoverable error; ierr>0 unrecoverable error; ierr=0: ok*/
}
/*--------------------------------------------------------------------------*/
static int CallKinsol(double *told)
{
    //** used for the [stop] button
    char* CommandToUnstack;
    static int CommandLength = 0;
    static int SeqSync = 0;
    static int one = 1;

    N_Vector y = NULL, yscale = NULL, fscale = NULL;
    double *fsdata = NULL, *ysdata = NULL;
    int N = 0, strategy = 0, i = 0, j = 0, k = 0, status = 0;
    /* int mxiter, msbset, msbsetsub, etachoice, mxnbcf; */
    /* double eta, egamma, ealpha, mxnewtstep, relfunc, fnormtol, scsteptol; */
    /* booleantype noInitSetup, noMinEps; */
    void *kin_mem = NULL;
    realtype reltol = 0., abstol = 0.;
    int *Mode_save = NULL;
    int Mode_change = 0;
    static int PH = 0;
    int N_iters = 0;
    double ratio = 0.;

    N = *neq;
    if (N <= 0)
    {
        return 0;
    }

    reltol = (realtype) rtol;
    abstol = (realtype) Atol;

    Mode_save = NULL;
    if (nmod > 0)
    {
        if ((Mode_save = MALLOC(sizeof(int) * nmod)) == NULL )
        {
            *ierr = 10000;
            return -1;
        }
    }

    y = N_VNewEmpty_Serial(N);
    if (y == NULL)
    {
        FREE(Mode_save);
        return -1;
    }
    yscale = N_VNew_Serial(N);
    if (yscale == NULL)
    {
        FREE(Mode_save);
        N_VDestroy_Serial(y);
        return -1;
    }
    fscale = N_VNew_Serial(N);
    if (fscale == NULL)
    {
        FREE(Mode_save);
        N_VDestroy_Serial(y);
        N_VDestroy_Serial(yscale);
        return -1;
    }
    ysdata =  NV_DATA_S(yscale);
    fsdata = NV_DATA_S(fscale);

    NV_DATA_S(y) = x;
    kin_mem = KINCreate();
    if (kin_mem == NULL)
    {
        FREE(Mode_save);
        N_VDestroy_Serial(y);
        N_VDestroy_Serial(yscale);
        N_VDestroy_Serial(fscale);
        return -1;
    }

    status = KINInit(kin_mem, simblkKinsol, y);
    strategy = KIN_NONE; /*without LineSearch */
    status = KINDense(kin_mem, N);

    status = KINSetNumMaxIters(kin_mem, 2000);  /* MaxNumIter=200->2000 */
    status = KINSetRelErrFunc(kin_mem, reltol); /* FuncRelErr=eps->RTOL */
    status = KINSetMaxSetupCalls(kin_mem, 1);   /* MaxNumSetups=10->1=="Exact Newton" */
    status = KINSetMaxSubSetupCalls(kin_mem, 1); /* MaxNumSubSetups=5->1 */
    /* status = KINSetNoInitSetup(kin_mem,noInitSetup);  // InitialSetup=true  */
    /* status = KINSetNoMinEps(kin_mem,noMinEps);        // MinBoundEps=true   */
    /* status = KINSetMaxBetaFails(kin_mem,mxnbcf);      // MaxNumBetaFails=10 */
    /* status = KINSetEtaForm(kin_mem,etachoice);        // EtaForm=Type1      */
    /* status = KINSetEtaConstValue(kin_mem,eta);*/        // Eta=0.1            */
    /* status = KINSetEtaParams(kin_mem,egamma,ealpha);  // EtaGamma=0.9  EtaAlpha=2.0 */
    /* status = KINSetMaxNewtonStep(kin_mem,mxnewtstep); // MaxNewtonStep=0.0  */
    /* status = KINSetFuncNormTol(kin_mem,fnormtol);     // FuncNormTol=eps^(1/3) */
    /* status = KINSetScaledStepTol(kin_mem,scsteptol);  // ScaledStepTol={eps^(2/3) */
    for ( j = 0; j < N; j++)
    {
        ysdata[j] = 1;
        fsdata[j] = 1;
    }
    /*========================================================*/
    if (PH == 2)
    {
        PH = 1;
    }
    else
    {
        PH = 2;    /* remind that PH is a static variable*/
    }

    status = -1;
    N_iters = 10 + nmod * 3;
    for (k = 0; k <= N_iters; k++) /* loop for mode fixin*/
    {
        phase = PH;
        /*------------KINSOL calls-----------*/
        for (i = 0; i < 10; i++)
        {
            simblkKinsol(y, fscale, NULL);

            for (j = 0; j < N; j++)
                if (fsdata[j] - fsdata[j] != 0)
                {
                    sciprint(_("\nWarning: The residual function #%d returns a NaN/Inf"), j);
                    sciprint(_("\n The residual function returns NAN/Inf. \n Please verify your model:\n some functions might be called with illegal inputs."));
                    freekinsol;
                    *ierr = 400 - status;
                    C2F(ierode).iero = *ierr;
                    return -1;
                }
            ratio = 0.3;
            for ( j = 0; j < N; j++)
            {
                if (x[j] == 0)
                {
                    ysdata[j] += 1 * ratio;
                }
                else
                {
                    ysdata[j] += ratio / fabs(x[j]);
                }
                if (fsdata[j] == 0)
                {
                    fsdata[j] = 1;
                }
                else
                {
                    fsdata[j] = 1 / fabs(fsdata[j]);
                }
                ysdata[j] /= ratio + 1;
            }
            status = KINSol(kin_mem, y, strategy, yscale, fscale);/* Calling the Newton Solver */
            if (status >= 0)
            {
                break;
            }
            /* Serge Steer 29/06/2009 */
            while (ismenu()) //** if the user has done something, do the actions
            {
                int ierr2 = 0;
                int iUnused;
                GetCommand(&CommandToUnstack, &SeqSync, &iUnused, NONE); //** get to the action
                CommandLength = (int)strlen(CommandToUnstack);
                //syncexec(CommandToUnstack, &CommandLength, &ierr2, &one, CommandLength); //** execute it
                FREE(CommandToUnstack);
            }

            if (C2F(coshlt).halt != 0)
            {
                C2F(coshlt).halt = 0;
                freekinsol;
                return 0;
            }
        }
        /*---------end of KINSOL calls-----------*/
        if (PH == 2 )
        {
            for (j = 0; j < nmod; ++j)
            {
                Mode_save[j] = mod[j];
            }

            if (ng > 0 && nmod > 0)
            {
                phase = 1; // updating the modes
                zdoit(told, x, xd, g);
                if (*ierr != 0)
                {
                    freekinsol;
                    C2F(ierode).iero = *ierr;
                    return -1;
                }
            }

            Mode_change = 0;
            for (j = 0; j < nmod; ++j)
            {
                if (Mode_save[j] != mod[j])
                {
                    Mode_change = 1;
                    break;
                }
            }

            if (Mode_change == 0 && status >= 0 )
            {
                break;    /*Successful termination*/
            }

        }
        else
        {
            /* calling with phase=1*/
            if (status >= 0)
            {
                break;
            }
        }

    } /* end of the loop for mode fixing*/

    if (status < 0 )
    {
        *ierr = 400 - status;
        C2F(ierode).iero = *ierr;
    }
    freekinsol;
    return status;
} /* CallKinSol_ */
/*--------------------------------------------------------------------------*/

