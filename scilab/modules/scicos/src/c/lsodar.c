/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2012 - Paul Bignier
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <machine.h>

#include "lsodar.h"

/* =============================
 *
 *            lsodar
 *
 * =============================
 *
 * Actual solving function, from 'ODEPACK' in 'differential_equations' module
 */

extern void C2F(lsodar) (LSRhsFn f, int *neq, realtype *y, realtype *t, realtype *tout, int *itol, realtype *reltol, realtype *abstol, enum iTask_t *itask, int *istate, int *iopt, struct rWork_t *rwork, int *lrw, int *iwork, int *liw,  int *jacobian, int *jacType, LSRootFn grblk, int *ng, int *jroot);

/* =============================
 *
 *         LSodarCreate
 *
 * =============================
 *
 * LSodarCreate creates an internal memory block for a problem to be solved by LSODAR.
 * If successful, LSodarCreate returns a pointer to the problem memory.
 * This pointer should be passed to LSodarMalloc.
 * If an initialization error occurs,
 * LSodarCreate prints an error message to standard err and returns NULL.
 */

void * LSodarCreate (int * neq, int ng)
{
    int lRn, lRs, lIw, lRw;

    /* Allocate the problem memory space */
    LSodarMem lsodar_mem = NULL;
    lsodar_mem = (LSodarMem) malloc(sizeof(struct LSodarMemRec));
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, 0, "LSODAR", "LSodarCreate", MSGCV_CVMEM_FAIL);
        return (NULL);
    }

    /* Set the 'rwork' and 'iwork' workspaces lengths */
    lRn = 20 + 16 * (*neq) + 3 * ng;
    lRs = 22 + ((*neq) * (9 + (*neq))) + 3 * ng;
    lIw = 20 + (*neq);
    lRw = max(lRn, lRs);

    /* Copy the variables into the problem memory space */
    lsodar_mem->nEquations = neq;
    lsodar_mem->iState = 1;
    lsodar_mem->iOpt = 0;
    lsodar_mem->rwork = NULL;
    lsodar_mem->lrw = lRw;
    lsodar_mem->iwork = NULL;
    lsodar_mem->liw = lIw;
    lsodar_mem->jacobian = 0;
    lsodar_mem->jacType = 2;
    lsodar_mem->g_fun = NULL;
    lsodar_mem->ng_fun = ng;
    lsodar_mem->jroot = NULL;

    return ((void *) lsodar_mem);
}

/* Shortcuts to problem memory space parameters */
# define func    ls_mem->func
# define nEq     ls_mem->nEquations
# define yVec    ls_mem->yVector
# define tStart  ls_mem->tStart
# define tEnd    ls_mem->tEnd
# define iTol    ls_mem->iTol
# define relTol  ls_mem->relTol
# define absTol  ls_mem->absTol
# define iState  ls_mem->iState
# define iOpt    ls_mem->iOpt
# define rwork   ls_mem->rwork
# define lrw     ls_mem->lrw
# define iwork   ls_mem->iwork
# define liw     ls_mem->liw
# define jac     ls_mem->jacobian
# define jacType ls_mem->jacType
# define g_fun   ls_mem->g_fun
# define ng_fun  ls_mem->ng_fun
# define jroot   ls_mem->jroot

/* =============================
 *
 *         LSodarMalloc
 *
 * =============================
 *
 * LSodarMalloc allocates and initializes memory for a problem.
 * All problem inputs are checked for errors. If any error occurs during initialization,
 * it is reported to the file whose file pointer is errfp and an error flag is returned.
 * Otherwise, it returns CV_SUCCESS.
 */

int LSodarMalloc (void * lsodar_mem, LSRhsFn f, realtype t0, N_Vector y, int itol, realtype reltol, void * abstol)
{
    /* Check the input arguments */

    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarMalloc", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    if (f == NULL)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarMalloc", MSGCV_NULL_F);
        return (CV_ILL_INPUT);
    }
    if (y == NULL)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarMalloc", MSGCV_NULL_Y0);
        return (CV_ILL_INPUT);
    }
    if (reltol < 0.)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarMalloc", MSGCV_BAD_RELTOL);
        return (CV_ILL_INPUT);
    }
    if (*((realtype *) abstol) < 0.)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarMalloc", MSGCV_BAD_ABSTOL);
        return (CV_ILL_INPUT);
    }

    /* Copy the arguments into the problem memory space */
    func   =  f;
    yVec   =  NV_DATA_S(y);
    tStart =  t0;
    iTol   =  itol;
    relTol =  reltol;
    absTol =  *((realtype *) abstol);

    /* Allocate rwork and iwork workspaces and set them to zero.
       Their size is lrw and liw, respectively */
    rwork = (struct rWork_t *) calloc(lrw, sizeof(realtype));
    iwork = calloc(liw, sizeof(int));

    return (CV_SUCCESS);
}

/* =============================
 *
 *         LSodarReInit
 *
 * =============================
 *
 * LSodarReInit re-initializes LSODAR's memory for a problem,
 * assuming it has already been allocated in a prior LSodarMalloc call.
 * All problem specification inputs are checked for errors.
 * If any error occurs during initialization, it is reported to the file whose file pointer is errfp.
 * The return value is CV_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int LSodarReInit (void * lsodar_mem, LSRhsFn f, realtype tOld, N_Vector y, int itol, realtype reltol, void * abstol)
{
    double rwork0, rwork5;

    /* Check the input arguments */

    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarReInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    if (y == NULL)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarReInit", MSGCV_NULL_Y0);
        return (CV_ILL_INPUT);
    }
    if (f == NULL)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarReInit", MSGCV_NULL_F);
        return (CV_ILL_INPUT);
    }
    if (reltol < 0.)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarReInit", MSGCV_BAD_RELTOL);
        return (CV_ILL_INPUT);
    }
    if (*((realtype *) abstol) < 0.)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarReInit", MSGCV_BAD_ABSTOL);
        return (CV_ILL_INPUT);
    }

    /* Reset the problem memory space variables to the arguments */
    func   =  f;
    *nEq   =  NV_LENGTH_S(y);
    yVec   =  NV_DATA_S(y);
    tStart =  tOld;
    iTol   =  itol;
    relTol =  reltol;
    absTol =  *((realtype *) abstol);
    iState =  1;

    /* Reinitialize rwork and iwork, leave rwork->tcrit and rwork->hmax unchanged, containing tcrit and hmax */
    rwork0 = rwork->tcrit;
    rwork5 = rwork->hmax;
    memset(rwork, 0, lrw);
    memset(iwork, 0, liw);
    rwork->tcrit = rwork0;
    rwork->hmax = rwork5;

    return (CV_SUCCESS);
}

/* =============================
 *
 *         LSodarRootInit
 *
 * =============================
 *
 * LSodarRootInit initializes a rootfinding problem to be solved during the integration of the ODE system.
 * It loads the root function pointer and the number of root functions, and allocates workspace memory.
 * The return value is CV_SUCCESS = 0 if no errors occurred, or a negative value otherwise.
 */

int LSodarRootInit (void * lsodar_mem, int ng, LSRootFn g, void *gdata)
{
    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarRootInit", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    if (g == NULL)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodarRootInit", MSGCV_NULL_G);
        return (CV_MEM_NULL);
    }

    g_fun = g;
    ng_fun = ng;

    /* Allocate jroot and set it to zero */
    if (ng > 0)
    {
        jroot = calloc(ng, sizeof(int));
    }

    return (CV_SUCCESS);
}


/* =============================
 *
 *       LSodarSetMaxStep
 *
 * =============================
 *
 * Sets the maximum step size, stocked in rwork->hmax.
 * Sets iOpt to 1 for rwork->hmax to be taken in consideration by lsodar().
 */

int LSodarSetMaxStep (void * lsodar_mem, realtype hMax)
{
    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarSetMaxStep", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    if (iOpt == 0)
    {
        iOpt = 1;
    }
    rwork->hmax = hMax;

    return (CV_SUCCESS);
}

/* =============================
 *
 *       LSodarSetStopTime
 *
 * =============================
 *
 * Specifies the time beyond which the integration is not to proceed, stocked in rwork->tcrit.
 * Sets iOpt to 1 for rwork->tcrit to be taken in consideration by lsodar().
 */

int LSodarSetStopTime (void * lsodar_mem, realtype tCrit)
{
    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarSetStopTime", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    if (iOpt == 0)
    {
        iOpt = 1;
    }
    rwork->tcrit = tCrit;

    return (CV_SUCCESS);
}

/* =============================
 *
 *            LSodar
 *
 * =============================
 *
 * This routine is the main driver of LSODAR.
 *
 * It integrates and looks for roots over a time interval defined by the user.
 *
 * The first time that LSodar is called for a successfully initialized problem,
 * it computes a tentative initial step size h.
 *
 * LSodar supports five modes, specified by itask: LS_NORMAL, LS_ONE_STEP, LS_MESH_STEP, LS_NORMAL_TSTOP, and LS_ONE_STEP_TSTOP.
 *
 * In the LS_NORMAL mode, the solver steps until it reaches or passes tout and then interpolates to obtain y(tOut).
 * In the LS_ONE_STEP mode, it takes one internal step and returns.
 * LS_MESH_STEP means stop at the first internal mesh point at or beyond t = tout and return.
 * LS_NORMAL_TSTOP and LS_ONE_STEP_TSTOP are similar to LS_NORMAL and LS_ONE_STEP, respectively,
 * but the integration never proceeds past tcrit (which must have been defined through a call to LSodarSetStopTime).
 *
 * It returns CV_ROOT_RETURN if a root was detected, CV_SUCCESS if the integration went correctly,
 * or a corresponding error flag.
 */

int LSodar (void * lsodar_mem, realtype tOut, N_Vector yOut, realtype * tOld, enum iTask_t itask)
{
    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodar", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    if (yOut == NULL)
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_YOUT_NULL);
        return (CV_ILL_INPUT);
    }
    if ((itask != LS_NORMAL) && (itask != LS_ONE_STEP) && (itask != LS_MESH_STEP) &&
            (itask != LS_NORMAL_TSTOP) && (itask != CV_ONE_STEP_TSTOP))
    {
        LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_BAD_ITASK);
        return(CV_ILL_INPUT);
    }

    /* Retrieve nEq if it has changed, use a copy of the solution vector and stock the simulation times */
    *nEq = NV_LENGTH_S(yOut);
    yVec = NV_DATA_S(yOut);
    tStart = *tOld;
    tEnd = tOut;

    /* Launch the simulation with the memory space parameters.
       lsodar() will update yVec, iState, rwork, iwork and jroot */
    C2F(lsodar) (func, nEq, yVec, &tStart, &tEnd, &iTol, &relTol, &absTol, &itask, &iState, &iOpt, rwork, &lrw, iwork, &liw, &jac, &jacType, g_fun, &ng_fun, jroot);

    /* Increment the start times */
    *tOld = tOut;
    tStart = tEnd;

    /* lsodar() stocked the completion status in iState; return accordingly  */
    switch (iState)
    {
        case 3:
            return (CV_ROOT_RETURN);
        case -1:
            LSProcessError(ls_mem, CV_TOO_MUCH_WORK, "LSODAR", "LSodar", MSGCV_MAX_STEPS, tStart);
            return (CV_TOO_MUCH_WORK);
        case -2:
            LSProcessError(ls_mem, CV_TOO_MUCH_ACC, "LSODAR", "LSodar", MSGCV_TOO_MUCH_ACC, tStart);
            return (CV_TOO_MUCH_ACC);
        case -3:
            LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_BAD_INPUT, tStart);
            return (CV_ILL_INPUT);
        case -4:
            LSProcessError(ls_mem, CV_ERR_FAILURE, "LSODAR", "LSodar", MSGCV_ERR_FAILS, tStart);
            return (CV_ERR_FAILURE);
        case -5:
            LSProcessError(ls_mem, CV_CONV_FAILURE, "LSODAR", "LSodar", MSGCV_CONV_FAILS, tStart);
            return (CV_CONV_FAILURE);
        case -6:
            LSProcessError(ls_mem, CV_ILL_INPUT, "LSODAR", "LSodar", MSGCV_EWT_NOW_BAD, tStart);
            return (CV_ILL_INPUT);
        default:
            return (CV_SUCCESS);
    }
}

/* =============================
 *
 *       LSodarGetRootInfo
 *
 * =============================
 *
 * Updates rootsfound[] to the computed roots stocked in jroot[].
 */

int LSodarGetRootInfo (void * lsodar_mem, int * rootsfound)
{
    LSodarMem ls_mem = NULL;
    if (lsodar_mem == NULL)
    {
        LSProcessError(NULL, CV_MEM_NULL, "LSODAR", "LSodarGetRootInfo", MSGCV_NO_MEM);
        return (CV_MEM_NULL);
    }
    ls_mem = (LSodarMem) lsodar_mem;

    /* Copy jroot to rootsfound */
    memcpy(rootsfound, jroot, ng_fun * sizeof(int));

    return (CV_SUCCESS);
}

/* =============================
 *
 *            LSFree
 *
 * =============================
 *
 * This routine frees the problem memory allocated by LSodarMalloc.
 */

void LSodarFree (void ** lsodar_mem)
{
    LSodarMem ls_mem = NULL;
    if (*lsodar_mem == NULL)
    {
        return;
    }
    ls_mem = (LSodarMem) (*lsodar_mem);

    /* Free the inner vectors */
    LSFreeVectors (ls_mem);

    free (*lsodar_mem);
    *lsodar_mem = NULL;
}

/* =============================
 *
 *         LSFreeVectors
 *
 * =============================
 *
 * Frees the problem memory space vectors.
 */

void LSFreeVectors (LSodarMem ls_mem)
{
    /* rwork, iwork and jroot have been allocated; free them */
    free (rwork);
    free (iwork);
    free (jroot);
}

#define ehfun    LSErrHandler
#define eh_data  (void *) lsodar_mem

/* =============================
 *
 *         LSProcessError
 *
 * =============================
 *
 * Error handling function.
 */

void LSProcessError (LSodarMem lsodar_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...)
{
    va_list ap;
    char msg[256];

    /* Initialize the argument pointer variable
       (msgfmt is the last required argument to LSProcessError) */
    va_start(ap, msgfmt);

    if (lsodar_mem == NULL)      /* We write to stderr */
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
        ehfun(error_code, module, fname, msg, eh_data);
    }

    /* Finalize argument processing */
    va_end(ap);

    return;
}

#define errfp    stderr

/* =============================
 *
 *          LSErrHandler
 *
 * =============================
 *
 * Default error handling function.
 */

void LSErrHandler (int error_code, const char *module, const char *function, char *msg, void *data)
{
    char err_type[10];

    if (error_code == CV_WARNING)
    {
        sprintf(err_type, "WARNING");
    }
    else
    {
        sprintf(err_type, "ERROR");
    }

#ifndef NO_FPRINTF_OUTPUT
    if (errfp != NULL)
    {
        fprintf(errfp, "\n[%s %s]  %s\n", module, err_type, function);
        fprintf(errfp, "  %s\n\n", msg);
    }
#endif

    return;
}

