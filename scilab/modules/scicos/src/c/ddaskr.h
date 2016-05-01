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

#ifndef _DDASKR_H
#define _DDASKR_H

#include "sundials_extension.h"
#include "sundials/sundials_types.h" // Definition of types 'realtype' and 'booleantype'
#include "nvector/nvector_serial.h"  // Type 'N_Vector'
#include "../scicos_sundials/src/ida/ida_impl.h" // Error handling

#ifndef max
#define max(A,B) ((A>B) ? A:B)  // 'max()' function
#endif

/* By default, we set the maximum order to 5 */
#define MAXORD_DEFAULT 5

// realtype workspace
struct DDrWork_t
{
    realtype tcrit;
    realtype hmax;
    realtype hnext;
    realtype tfarthest;
    realtype rwork5;
    realtype rwork6;
    realtype hlast;
    realtype rwork8;
    realtype rwork9;
    realtype rwork10;
    realtype rwork11;
    realtype rwork12;
    realtype rwork13;
    realtype steptol;
    realtype epinit;
    realtype rwork[1];
};

// Derivative computation, root functions, preconditioner calculation and application
typedef void (*DDASResFn) (realtype *tOld, realtype *y, realtype *yp, realtype *res, int *flag, realtype *dummy1, int *dummy2);
typedef void (*DDASRootFn) (int *neq, realtype *tOld, realtype *y, int *ng, realtype *groot, realtype *dummy1, int *dummy2);
typedef void (*DDASJacPsolFn) (realtype *res, int *ires, int *neq, realtype *tOld, realtype *actual, realtype *actualP,
                               realtype *rewt, realtype *savr, realtype *wk, realtype *h, realtype *cj, realtype *wp,
                               int *iwp, int *ier, realtype *dummy1, int *dummy2);
typedef void (*DDASPsolFn) (int *neq, realtype *tOld, realtype *actual, realtype *actualP,
                            realtype *savr, realtype *wk, realtype *cj, realtype *wght, realtype *wp,
                            int *iwp, realtype *b, realtype *eplin, int *ier, realtype *dummy1, int *dummy2);
typedef void (*DDASErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *user_data);

// DDaskr problem memory structure
typedef struct DDaskrMemRec
{
    DDASResFn res;
    int * nEquations;
    void * user_data;
    realtype tStart;
    realtype relTol;
    realtype absTol;
    realtype * yVector;
    realtype * yPrimeVector;
    int iState;
    int * info;
    struct DDrWork_t * rwork;
    int lrw;
    int * iwork;
    int liw;
    int maxnhIC;
    DDASErrHandlerFn ehfun;
    DDASRootFn g_fun;
    int ng_fun;
    int * jroot;
    int solver;
    DDASJacPsolFn jacpsol;
    DDASPsolFn psol;
    realtype * rpar;
    int * ipar;
} *DDaskrMem;

// Creating the problem
void * DDaskrCreate (int * neq, int ng, int solverIndex);

// Allocating the problem
int DDaskrInit (void * ddaskr_mem, DDASResFn Res, realtype t0, N_Vector yy0, N_Vector yp0, DDASJacPsolFn jacpsol, DDASPsolFn psol);

// Reinitializing the problem
int DDaskrReInit (void * ddaskr_mem, realtype tOld, N_Vector yy0, N_Vector yp0);

// Specifying the tolerances
int DDaskrSStolerances (void * ddaskr_mem, realtype reltol, realtype abstol);

// Initializing the root-finding problem
int DDaskrRootInit (void * ddaskr_mem, int ng, DDASRootFn g);

// Setting a pointer to user_data that will be passed to the user's res function every time a user-supplied function is called
int DDaskrSetUserData (void * ddaskr_mem, void * User_data);

// Specifying the maximum step size
int DDaskrSetMaxStep (void * ddaskr_mem, realtype hmax);

// Specifying the time beyond which the integration is not to proceed
int DDaskrSetStopTime (void * ddaskr_mem, realtype tcrit);

// Sets the maximum number of steps in an integration interval
int DDaskrSetMaxNumSteps (void * ddaskr_mem, long int maxnh);

// Sets the maximum number of Jacobian or preconditioner evaluations
int DDaskrSetMaxNumJacsIC (void * ddaskr_mem, int maxnj);

// Sets the maximum number of Newton iterations per Jacobian or preconditioner evaluation
int DDaskrSetMaxNumItersIC (void * ddaskr_mem, int maxnit);

// Sets the maximum number of values of the artificial stepsize parameter H to be tried
int DDaskrSetMaxNumStepsIC (void * ddaskr_mem, int MaxnhIC);

// Sets the flag to turn off the linesearch algorithm
int DDaskrSetLineSearchOffIC (void * ddaskr_mem, int lsoff);

// Specifying which components are differential and which ones are algrebraic, in order to get consistent initial values
int DDaskrSetId (void * ddaskr_mem, N_Vector xproperty);

// Solving the problem
int DDaskrSolve (void * ddaskr_mem, realtype tOut, realtype * tOld, N_Vector yOut, N_Vector ypOut, int itask);

// Computing consistent initial values for the problem
int DDaskrCalcIC (void * ddaskr_mem, int icopt, realtype tout1);

// Following on DDasCalcIC, copying yy0 and yp0 (computed consistent values) into the memory space
int DDaskrGetConsistentIC (void * ddaskr_mem, N_Vector yy0, N_Vector yp0);

// Update rootsfound to the computed jroots
int DDaskrGetRootInfo (void * ddaskr_mem, int * rootsfound);

// Freeing the problem memory allocated by ddaskrMalloc
void DDaskrFree (void ** ddaskr_mem);

// Freeing the ddaskr vectors allocated in ddaskrAllocVectors
void DDASFreeVectors (DDaskrMem ddaskr_mem);

// Specifies the error handler function
int DDaskrSetErrHandlerFn (void * ddaskr_mem, DDASErrHandlerFn ehfun, void * eh_data);

// Error handling function
void DDASProcessError (DDaskrMem ddas_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

#endif
