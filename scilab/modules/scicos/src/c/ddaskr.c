/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2013 - Paul Bignier
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <machine.h>
#include <float.h>   // DBL_EPSILON, to define UNIT_ROUNDOFF
#include <math.h>    // fabs() and pow() functions

#include "ddaskr.h"

#define NO_FPRINTF_OUTPUT 1

/*
 * Control constant for tolerances
 *
 * Scicos only uses scalar tolerances, so we only need the scalar-scalar (SS) value for info[1].
 * --------------------------------
 */
#define DDAS_SS           0

/* Flags for stop mode */
#define DDAS_NORMAL       0
#define DDAS_ONE_STEP     1

/*
 * Flags for initial values calculation.
 *
 * Indicating whether the components have been qualified (differential / algebraic).
 * --------------------------------
 */
#define DDAS_YA_YDP_INIT  1
#define DDAS_Y_INIT       2

#define UNIT_ROUNDOFF     DBL_EPSILON

/* =============================
 *
 *           ddaskr
 *
 * =============================
 *
 * Actual solving function, from 'ODEPACK' in 'differential_equations' module.
 * Since we use ddaskr's built-in jacobian function, set jacpsol type to DDasJacPsolFn.
 */

extern void C2F(ddaskr) (DDASResFn res, int *neq, realtype *t, realtype *y, realtype *yp, realtype *tout, int *info, realtype *reltol, realtype *abstol, int *istate, struct DDrWork_t *rwork, int *lrw, int *iwork, int *liw, double *dummy1, int *dummy2, DDASJacPsolFn jacpsol, DDASPsolFn psol, DDASRootFn grblk, int *ng, int *jroot);

/* =============================
 *
 *         DDaskrCreate
 *
 * =============================
 *
 * DDaskrCreate creates an internal memory block for a problem to be solved by DDASKR.
 * If successful, DDaskrCreate returns a pointer to the problem memory.
 * This pointer should be passed to DDaskrInit.
 * If an initialization error occurs,
 * DDaskrCreate prints an error message to standard err and returns NULL.
 */

void * DDaskrCreate (int * neq, int ng, int solverIndex)
{
    int lIw = 0, lRw = 0, LENWP = 0, LENIWP = 0;
    DDaskrMem ddaskr_mem = NULL;

    /* Allocate the problem memory space */
    ddaskr_mem = NULL;
    ddaskr_mem = (DDaskrMem) malloc(sizeof(struct DDaskrMemRec));
    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, 0, "DDASKR", "DDaskrCreate", MSG_MEM_FAIL);
        return (NULL);
    }

    /* Zero out ddas_mem */
    memset(ddaskr_mem, 0, sizeof(struct DDaskrMemRec));

    /* Set the 'rwork' and 'iwork' workspaces lengths by default
       LENWP and LENIWP are lentghts of segments of rwork and iwork (respectively),
       that will contain factored preconditioner matrix information */
    LENWP  = (*neq) * (*neq);
    LENIWP = (*neq);
    lRw    = 60 + (*neq) * (max(MAXORD_DEFAULT + 4, 7) + (*neq)) + 3 * ng;
    lIw    = 40 + 2 * (*neq);

    /* If we are going to use the Krylov method, resize the workspaces adequately */
    if (solverIndex == 102)
    {
        lRw = 101 + 18 * (*neq) + 3 * ng + LENWP;
        lIw = 40 + (*neq) + LENIWP;
    }

    /* Copy the variables into the problem memory space */
    ddaskr_mem->nEquations = neq;
    ddaskr_mem->user_data  = NULL;
    ddaskr_mem->iState     = 0;
    ddaskr_mem->info       = NULL;
    ddaskr_mem->rwork      = NULL;
    ddaskr_mem->lrw        = lRw;
    ddaskr_mem->iwork      = NULL;
    ddaskr_mem->liw        = lIw;
    ddaskr_mem->ehfun      = NULL;
    ddaskr_mem->g_fun      = NULL;
    ddaskr_mem->ng_fun     = ng;
    ddaskr_mem->jroot      = NULL;
    ddaskr_mem->solver     = solverIndex;
    ddaskr_mem->jacpsol    = NULL;
    ddaskr_mem->psol       = NULL;
    ddaskr_mem->rpar       = NULL;
    ddaskr_mem->ipar       = NULL;

    return ((void *) ddaskr_mem);
}

/* Shortcuts to problem memory space parameters */
# define res        ddas_mem->res
# define nEq        ddas_mem->nEquations
# define user_data  ddas_mem->user_data
# define yVec       ddas_mem->yVector
# define ypVec      ddas_mem->yPrimeVector
# define tStart     ddas_mem->tStart
# define info       ddas_mem->info
# define relTol     ddas_mem->relTol
# define absTol     ddas_mem->absTol
# define iState     ddas_mem->iState
# define rwork      ddas_mem->rwork
# define lrw        ddas_mem->lrw
# define iwork      ddas_mem->iwork
# define liw        ddas_mem->liw
# define maxnhIC    ddas_mem->maxnhIC
# define g_fun      ddas_mem->g_fun
# define ng_fun     ddas_mem->ng_fun
# define jroot      ddas_mem->jroot
# define solver     ddas_mem->solver
# define jacpsol    ddas_mem->jacpsol
# define psol       ddas_mem->psol
# define rpar       ddas_mem->rpar
# define ipar       ddas_mem->ipar

/* =============================
 *
 *          DDaskrInit
 *
 * =============================
 *
 * DDaskrInit allocates and initializes memory for a problem.
 * All problem inputs are checked for errors. If any error occurs during initialization,
 * it is reported to the file whose file pointer is errfp and an error flag is returned.
 * Otherwise, it returns IDA_SUCCESS.
 */

int DDaskrInit (void * ddaskr_mem, DDASResFn Res, realtype t0, N_Vector yy0, N_Vector yp0, DDASJacPsolFn Jacpsol, DDASPsolFn Psol)
{
    DDaskrMem ddas_mem = NULL;

    /* Check the input arguments */

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrInit", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (yy0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrInit", MSG_Y0_NULL);
        return (IDA_ILL_INPUT);
    }

    if (yp0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrInit", MSG_YP0_NULL);
        return (IDA_ILL_INPUT);
    }

    if (Res == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrInit", MSG_RES_NULL);
        return (IDA_ILL_INPUT);
    }

    /* Jacpsol = NULL or Psol = NULL is a problem only if the user decided to use the GMRes solver */
    if (solver == 102)
    {
        if (Jacpsol == NULL || Psol == NULL)
        {
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrInit", MSG_BAD_KRY_INPUT);
            return (IDA_ILL_INPUT);
        }
    }

    /* Copy the arguments into the problem memory space */
    res     = Res;
    yVec    = NV_DATA_S(yy0);
    ypVec   = NV_DATA_S(yp0);
    tStart  = t0;
    jacpsol = Jacpsol;
    psol    = Psol;

    /* Allocate the info[20] tab to zero, used to store parameters (zero is default value for mostof them) */
    info = calloc(20, sizeof(int));

    /* info[11] = 1 => Krylov method selected
       info[14] = 1 => providing jacobian function (to evaluate and LU-factor the preconditioner) */
    if (solver == 102)
    {
        info[11] = 1;
        info[14] = 1;
    }

    /* Allocate rwork and iwork workspaces and set them to zero.
       Their size is lrw and liw, respectively */
    rwork = (struct DDrWork_t *) calloc(lrw, sizeof(realtype));
    iwork = calloc(liw, sizeof(int));

    /* Save their lengths in iwork */
    iwork[16] = lrw;
    iwork[17] = liw;

    /* Solve the problem without invoking any special inequality constraints */
    info[9] = 0;

    /* Default values for heuristic control quantities in the initial values calculation */
    iwork[31] = (info[11] == 0) ? 5 : 15;		// maxnit, depends on dense / Krylov method
    iwork[32] = (info[11] == 0) ? 6  : 2;		// maxnj, depends on dense / Krylov method
    iwork[33] = 5;								// maxnh
    iwork[34] = 0;								// lsoff, flag to turn linesearch on / off (0 = on)
    rwork->steptol = pow(UNIT_ROUNDOFF, 2. / 3);	// steptol
    rwork->epinit = 0.01;						// epinit, swing factor in the Newton iteration convergence test.
    maxnhIC = 5;								// maxnh for IC calculation

    return (IDA_SUCCESS);
}

/* =============================
 *
 *         DDaskrReInit
 *
 * =============================
 *
 * DDaskrReInit reinitializes DDASKR's memory for a problem,
 * assuming it has already been allocated in a prior DDaskrInit call.
 * All problem specification inputs are checked for errors.
 * If any error occurs during initialization, it is reported to the file whose file pointer is errfp.
 * The return value is IDA_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int DDaskrReInit (void * ddaskr_mem, realtype tOld, N_Vector yy0, N_Vector yp0)
{
    DDaskrMem ddas_mem = NULL;

    /* Check the input arguments */

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrReInit", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (yy0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrInit", MSG_Y0_NULL);
        return (IDA_ILL_INPUT);
    }

    if (yp0 == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrInit", MSG_YP0_NULL);
        return (IDA_ILL_INPUT);
    }

    /* Reset the problem memory space variables to the arguments */
    *nEq    = NV_LENGTH_S(yy0);
    yVec    = NV_DATA_S(yy0);
    ypVec   = NV_DATA_S(yp0);
    tStart  = tOld;
    iState  = 0;
    info[0] = 0;

    /* Tell DDaskr to get new consitent values */
    info[10] = 1;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDaskrSStolerances
 *
 * =============================
 *
 * This function specifies the scalar integration tolerances.
 * It MUST be called before the first call to DDaskr.
 */

int DDaskrSStolerances (void * ddaskr_mem, realtype reltol, realtype abstol)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDaskr", "DDaskrSStolerances", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    /* Check inputs */

    if (reltol < 0.)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSStolerances", MSG_BAD_RTOL);
        return (IDA_ILL_INPUT);
    }

    if (abstol < 0.)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSStolerances", MSG_BAD_ATOL);
        return (IDA_ILL_INPUT);
    }

    /* Copy tolerances into memory */

    relTol  = reltol;
    absTol  = abstol;
    info[1] = DDAS_SS;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *         DDaskrRootInit
 *
 * =============================
 *
 * DDaskrRootInit initializes a rootfinding problem to be solved during the integration of the ODE system.
 * It loads the root function pointer and the number of root functions, and allocates workspace memory.
 * The return value is IDA_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int DDaskrRootInit (void * ddaskr_mem, int ng, DDASRootFn g)
{
    DDaskrMem ddas_mem = NULL;
    int nrt = 0;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrRootInit", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (g == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrRootInit", MSG_ROOT_FUNC_NULL);
        return (IDA_ILL_INPUT);
    }

    g_fun  = g;
    nrt    = (ng < 0) ? 0 : ng;
    ng_fun = nrt;

    /* Allocate jroot and set it to zero */
    if (ng > 0)
    {
        jroot = calloc(ng, sizeof(int));
    }

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDaskrSetUserData
 *
 * =============================
 *
 * Sets a pointer to user_data that will be passed to the user's res function
 * every time a user-supplied function is called.
 */

int DDaskrSetUserData (void * ddaskr_mem, void * User_data)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetUserData", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    user_data = User_data;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDaskrSetMaxStep
 *
 * =============================
 *
 * Sets the maximum step size, stocked in rwork->hmax.
 * Sets info[6] to 1 for rwork->hmax to be taken in consideration by ddaskr().
 */

int DDaskrSetMaxStep (void * ddaskr_mem, realtype hMax)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetMaxStep", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (info[6] == 0)
    {
        info[6] = 1;
    }
    rwork->hmax = hMax;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDaskrSetStopTime
 *
 * =============================
 *
 * Specifies the time beyond which the integration is not to proceed, stocked in rwork->tcrit.
 * Sets info[3] to 1 for rwork->tcrit to be taken in consideration by ddaskr().
 */

int DDaskrSetStopTime (void * ddaskr_mem, realtype tCrit)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetStopTime", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (info[3] == 0)
    {
        info[3] = 1;
    }

    rwork->tcrit = tCrit;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *     DDaskrSetMaxNumSteps
 *
 * =============================
 *
 * Sets the maximum number of steps in an integration interval.
 * Ensure that ddaskr will consider it via flag info[16], and stock it in iwork[33].
 */

int DDaskrSetMaxNumSteps (void * ddaskr_mem, long int maxnh)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetMaxNumSteps", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (maxnh <= 0)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "IDA", "DDaskrSetMaxNumSteps", MSG_BAD_MAXNH);
        return (IDA_ILL_INPUT);
    }

    if (info[16] == 0)
    {
        info[16] = 1;
    }

    iwork[33] = maxnh;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *     DDaskrSetMaxNumJacsIC
 *
 * =============================
 *
 * Sets the maximum number of Jacobian or preconditioner evaluations.
 * Ensure that ddaskr will consider it via flag info[16], and stock it in iwork[32].
 */

int DDaskrSetMaxNumJacsIC (void * ddaskr_mem, int maxnj)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetMaxNumJacsIC", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (maxnj <= 0)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "IDA", "DDaskrSetMaxNumJacsIC", MSG_BAD_MAXNJ);
        return (IDA_ILL_INPUT);
    }

    if (info[16] == 0)
    {
        info[16] = 1;
    }

    iwork[32] = maxnj;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *    DDaskrSetMaxNumItersIC
 *
 * =============================
 *
 * Sets the maximum number of Newton iterations per Jacobian or preconditioner evaluation.
 * Ensure that ddaskr will consider it via flag info[16], and stock it in iwork[31].
 */

int DDaskrSetMaxNumItersIC (void * ddaskr_mem, int maxnit)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetMaxNumItersIC", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (maxnit <= 0)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "IDA", "DDaskrSetMaxNumItersIC", MSG_BAD_MAXNIT);
        return (IDA_ILL_INPUT);
    }

    if (info[16] == 0)
    {
        info[16] = 1;
    }

    iwork[31] = maxnit;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *    DDaskrSetMaxNumStepsIC
 *
 * =============================
 *
 * Sets the maximum number of values of the artificial stepsize parameter H to be tried if info[10] = 1,
 * during the Initial Conditions calculation.
 * Ensure that ddaskr will consider it via flag info[16], and stock it in iwork[33].
 */

int DDaskrSetMaxNumStepsIC (void * ddaskr_mem, int MaxnhIC)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetMaxNumStepsIC", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (MaxnhIC <= 0)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "IDA", "DDaskrSetMaxNumStepsIC", MSG_BAD_MAXNH);
        return (IDA_ILL_INPUT);
    }

    if (info[16] == 0)
    {
        info[16] = 1;
    }

    maxnhIC = MaxnhIC;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *   DDaskrSetLineSearchOffIC
 *
 * =============================
 *
 * Sets the flag to turn off the linesearch algorithm.
 * lsoff = 0 means linesearch is on, lsoff = 1 means it is turned off.
 * Ensure that ddaskr will consider it via flag info[16], and stock it in iwork[34].
 */

int DDaskrSetLineSearchOffIC (void * ddaskr_mem, int lsoff)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetLineSearchOffIC", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (info[16] == 0)
    {
        info[16] = 1;
    }

    if (lsoff)
    {
        iwork[34] = 1;
    }

    return (IDA_SUCCESS);
}

/* =============================
 *
 *          DDaskrSetID
 *
 * =============================
 *
 * Specifies which components are differential and which ones are algrebraic, in order to get consistent initial values.
 * They are stocked in xproperty[neq] in the form:
 *  - xproperty[i] = 1 => differential component
 *  - xproperty[i] = 0 => algebraic component
 */

int DDaskrSetId (void * ddaskr_mem, N_Vector xproperty)
{
    DDaskrMem ddas_mem = NULL;
    realtype * temp = NULL;
    int i = 0, LID = 0;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetID", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    /* Copy xproperty data */
    temp = NULL;
    temp = NV_DATA_S(xproperty);

    /* Inform ddaskr that we are going to define components
       This is also a flag to compute consistent initial values */
    if (info[10] == 0)
    {
        info[10] = 1;
    }

    /* Since we don't use the non-negative constraints in scicos, LID = 40 (otherwise, LID = 40+neq) */
    LID = (info[9] == 0) ? 40 : 40 + *nEq;

    /* Stock xproperty in a segment of iwork
       temp[i] = 0 or 1, but that segment needs to be -1 or 1 (respectively) */
    for (i = 0; i < *nEq; ++i)
    {
        iwork[i + LID] = (temp[i] == 0) ? -1 : 1;
    }

    return (IDA_SUCCESS);
}

/* =============================
 *
 *          DDaskrSolve
 *
 * =============================
 *
 * This routine is the main driver of DDASKR.
 *
 * It integrates and looks for roots over a time interval defined by the user.
 *
 * The first time that DDaskr is called for a successfully initialized problem,
 * it computes a tentative initial step size h.
 *
 * DDaskr supports five modes, specified by itask: DDAS_NORMAL, DDAS_ONE_STEP, DDAS_MESH_STEP, DDAS_NORMAL_TSTOP, and DDAS_ONE_STEP_TSTOP.
 *
 * In the DDAS_NORMAL mode, the solver steps until it reaches or passes tout and then interpolates to obtain y(tOut).
 * In the DDAS_ONE_STEP mode, it takes one internal step and returns.
 * DDAS_MESH_STEP means stop at the first internal mesh point at or beyond t = tout and return.
 * DDAS_NORMAL_TSTOP and DDAS_ONE_STEP_TSTOP are similar to DDAS_NORMAL and DDAS_ONE_STEP, respectively,
 * but the integration never proceeds past tcrit (which must have been defined through a call to DDaskrSetStopTime).
 *
 * It returns IDA_ROOT_RETURN if a root was detected, IDA_SUCCESS if the integration went correctly,
 * or a corresponding error flag.
 */

int DDaskrSolve (void * ddaskr_mem, realtype tOut, realtype * tOld, N_Vector yOut, N_Vector ypOut, int itask)
{
    DDaskrMem ddas_mem = NULL;

    /* Check the input arguments */

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSolve", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (yOut == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_YRET_NULL);
        return (IDA_ILL_INPUT);
    }

    if (ypOut == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_YPRET_NULL);
        return (IDA_ILL_INPUT);
    }

    if (tOld == NULL)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_TRET_NULL);
        return (IDA_ILL_INPUT);
    }

    if ((itask != DDAS_NORMAL) && (itask != DDAS_ONE_STEP))
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_BAD_ITASK);
        return (IDA_ILL_INPUT);
    }

    /* Retrieve nEq if it has changed, use a copy of the solution vector and stock the simulation times */
    *nEq   = NV_LENGTH_S(yOut);
    yVec   = NV_DATA_S(yOut);
    ypVec  = NV_DATA_S(ypOut);
    tStart = *tOld;

    /* Save the task mode in info[2] */
    info[2] = itask;

    /* Launch the simulation with the memory space parameters.
       ddaskr() will update yVec, iState, rwork, iwork and jroot */
    C2F(ddaskr) (res, nEq, &tStart, yVec, ypVec, &tOut, info, &relTol, &absTol, &iState, rwork, &lrw, iwork, &liw, rpar, ipar, jacpsol, psol, g_fun, &ng_fun, jroot);

    /* Increment the start time */
    *tOld  = tStart;

    /* For continuation calls, avoiding recomputation of consistent values (if info[10] used to be 1) */
    info[10] = 0;

    /* ddaskr() stocked the completion status in iState; return accordingly */
    switch (iState)
    {
        case 5:
            return (IDA_ROOT_RETURN);
        case 6:
            return (IDA_ZERO_DETACH_RETURN);
        case -1:
            DDASProcessError(ddas_mem, IDA_TOO_MUCH_WORK, "DDASKR", "DDaskrSolve", MSG_MAX_STEPS, tStart);
            return (IDA_TOO_MUCH_WORK);
        case -2:
            DDASProcessError(ddas_mem, IDA_TOO_MUCH_ACC, "DDASKR", "DDaskrSolve", MSG_TOO_MUCH_ACC, tStart);
            return (IDA_TOO_MUCH_ACC);
        case -3:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_BAD_ATOL, tStart);
            return (IDA_ILL_INPUT);
        case -6:
            DDASProcessError(ddas_mem, IDA_ERR_FAIL, "DDASKR", "DDaskrSolve", MSG_ERR_FAILS, tStart);
            return (IDA_ERR_FAIL);
        case -7:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASKR", "DDaskrSolve", MSG_CONV_FAILS, tStart);
            return (IDA_CONV_FAIL);
        case -8:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_SINGULAR);
            return (IDA_ILL_INPUT);
        case -9:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASKR", "DDaskrSolve", MSG_CONV_FAILS, tStart);
            return (IDA_CONV_FAIL);
        case -10:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASKR", "DDaskrSolve", MSG_CONV_FAILS, tStart);
            return (IDA_CONV_FAIL);
        case -11:
            DDASProcessError(ddas_mem, IDA_RES_FAIL, "DDASKR", "DDaskrSolve", MSG_RES_NONRECOV, tStart);
            return (IDA_RES_FAIL);
        case -12:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_IC_FAIL_CONSTR);
            return (IDA_ILL_INPUT);
        case -33:
            DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrSolve", MSG_BAD_INPUT);
            return (IDA_ILL_INPUT);
        default:
            return (IDA_SUCCESS);
    }
}

/* =============================
 *
 *         DDaskrCalcIC
 *
 * =============================
 *
 * Computing consistent initial values for the problem.
 * This is done by launching the solver with a "stop after consistent initial values computation" flag
 * (info[10] and info[13]).
 */

int DDaskrCalcIC (void * ddaskr_mem, int icopt, realtype tout1)
{
    DDaskrMem ddas_mem = NULL;
    double tdist = 0, troundoff = 0, maxnhTemp = 0;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrCalcIC", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    /* icopt is a flag to determine whether the DDaskrSetID has been called,
       in which case it is now known which components are differential / algebraic.
       Here, we only use DDAS_YA_YDP_INIT (DDaskr has been called) */
    if (icopt != DDAS_YA_YDP_INIT && icopt != DDAS_Y_INIT)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrCalcIC", MSG_IC_BAD_ICOPT);
        return (IDA_ILL_INPUT);
    }

    /* Checking for valid tout1, not too close to tStart */
    tdist = fabs(tout1 - tStart);
    troundoff = 2 * UNIT_ROUNDOFF * (fabs(tStart) + fabs(tout1));
    if (tdist < troundoff)
    {
        DDASProcessError(ddas_mem, IDA_ILL_INPUT, "DDASKR", "DDaskrCalcIC", MSG_IC_TOO_CLOSE);
        return (IDA_ILL_INPUT);
    }

    /* info[10] = icopt => Flag on which initial values to compute (differential, algebraic or both)
       info[13] = 1 => Do not proceed to integration after calculation */
    info[10] = icopt;
    if (info[13] == 0)
    {
        info[13] = 1;
    }

    /* Giving maxnh a specific value for IC calculation, switching back right after */
    if (info[16] == 1)
    {
        maxnhTemp = iwork[33];
        iwork[33] = maxnhIC;
    }

    C2F(ddaskr) (res, nEq, &tStart, yVec, ypVec, &tout1, info, &relTol, &absTol, &iState, rwork, &lrw, iwork, &liw, rpar, ipar, jacpsol, psol, g_fun, &ng_fun, jroot);

    if (info[16] == 1)
    {
        iwork[33] = maxnhTemp;
    }

    /* The continuation of the program will not need initial values computation again, unless ReInit */
    info[10] = 0;
    info[13] = 0;

    switch (iState)
    {
        case 4:
            return (IDA_SUCCESS);
        default:
            DDASProcessError(ddas_mem, IDA_CONV_FAIL, "DDASKR", "DDaskrCalcIC", MSG_IC_CONV_FAILED);
            return (IDA_CONV_FAIL);
    }
}

/* =============================
 *
 *     DDaskrGetConsistentIC
 *
 * =============================
 *
 * Following on DDasCalcIC, copying yy0 and yp0 (computed consistent values) into the memory space.
 */

int DDaskrGetConsistentIC (void * ddaskr_mem, N_Vector yy0, N_Vector yp0)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrGetConsistentIC", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    if (yy0 != NULL)
    {
        NV_DATA_S(yy0) = yVec;
    }
    if (yp0 != NULL)
    {
        NV_DATA_S(yp0) = ypVec;
    }

    return (IDA_SUCCESS);
}

/* =============================
 *
 *       DDaskrGetRootInfo
 *
 * =============================
 *
 * Updates rootsfound[] to the computed roots stocked in jroot[].
 */

int DDaskrGetRootInfo (void * ddaskr_mem, int * rootsfound)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrGetRootInfo", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }
    ddas_mem = (DDaskrMem) ddaskr_mem;

    /* Copy jroot to rootsfound */
    memcpy(rootsfound, jroot, ng_fun * sizeof(int));

    return (IDA_SUCCESS);
}

/* =============================
 *
 *            DDASFree
 *
 * =============================
 *
 * This routine frees the problem memory allocated by DDaskrInit.
 */

void DDaskrFree (void ** ddaskr_mem)
{
    DDaskrMem ddas_mem = NULL;

    if (*ddaskr_mem == NULL)
    {
        return;
    }
    ddas_mem = (DDaskrMem) (*ddaskr_mem);

    /* Free the inner vectors */
    DDASFreeVectors (ddas_mem);

    free (*ddaskr_mem);
    *ddaskr_mem = NULL;
}

/* =============================
 *
 *         DDASFreeVectors
 *
 * =============================
 *
 * Frees the problem memory space vectors.
 */

void DDASFreeVectors (DDaskrMem ddas_mem)
{
    /* info, rwork, iwork and jroot have been allocated; free them */
    free (info);
    free (rwork);
    free (iwork);
    free (jroot);
}

#define ehfun    ddas_mem->ehfun

/* =============================
 *
 *     DDaskrSetErrHandlerFn
 *
 * =============================
 *
 * Specifies the error handler function.
 */

int DDaskrSetErrHandlerFn (void * ddaskr_mem, DDASErrHandlerFn ehFun, void * eh_data)
{
    DDaskrMem ddas_mem = NULL;

    if (ddaskr_mem == NULL)
    {
        DDASProcessError(NULL, IDA_MEM_NULL, "DDASKR", "DDaskrSetErrHandlerFn", MSG_NO_MEM);
        return (IDA_MEM_NULL);
    }

    ddas_mem = (DDaskrMem) ddaskr_mem;

    ehfun = ehFun;

    return (IDA_SUCCESS);
}

/* =============================
 *
 *         DDASProcessError
 *
 * =============================
 *
 * Error handling function.
 */

void DDASProcessError (DDaskrMem ddas_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    /* Initialize the argument pointer variable
       (msgfmt is the last required argument to DDASProcessError) */
    va_start(ap, msgfmt);

    if (ddas_mem == NULL)      /* We write to stderr */
    {
#ifndef NO_FPRINTF_OUTPUT
        fprintf(stderr, "\n[%s ERROR]  %s\n  ", module, fname);
        fprintf(stderr, msgfmt);
        fprintf(stderr, "\n\n");
#endif
    }
    else                     /* We can call ehfun */
    {
        /* Compose the message */
        vsprintf(msg, msgfmt, ap);

        /* Call ehfun */
        ehfun(error_code, module, fname, msg, NULL);
    }

    /* Finalize argument processing */
    va_end(ap);
}
