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

#ifndef _LSODAR_H
#define _LSODAR_H

#include "sundials_extension.h"
#include "sundials/sundials_types.h" // Definition of types 'realtype' and 'booleantype'
#include "nvector/nvector_serial.h"  // Type 'N_Vector'
#include "../scicos_sundials/src/cvode/cvode_impl.h" // Error handling

#ifndef max
#define max(A,B) ((A>B) ? A:B)  // 'max()' function
#endif

// realtype workspace
struct rWork_t
{
    realtype tcrit;
    realtype rwork2;
    realtype rwork3;
    realtype rwork4;
    realtype h0;
    realtype hmax;
    realtype hmin;
    realtype rwork[1];
};

// Derivative computation and Root functions
typedef int (*LSRhsFn) (int * neq, realtype * t, realtype * y, realtype * rwork);
typedef int (*LSRootFn) (int * neq, realtype * t, realtype * y, int * ng, realtype * rwork);

// Potential tasks
enum iTask_t
{
    LS_NORMAL = 1,
    LS_ONE_STEP = 2,
    LS_MESH_STEP = 3,
    LS_NORMAL_TSTOP = 4,
    LS_ONE_STEP_TSTOP = 5
};

// LSodar problem memory structure
typedef struct LSodarMemRec
{
    LSRhsFn func;
    int * nEquations;
    realtype * yVector;
    realtype tStart;
    realtype tEnd;
    int iTol;
    realtype relTol;
    realtype absTol;
    int iState;
    int iOpt;
    struct rWork_t * rwork;
    int lrw;
    int * iwork;
    int liw;
    int jacobian;
    int jacType;
    LSRootFn g_fun;
    int ng_fun;
    int * jroot;
} *LSodarMem;

// Creating the problem
void * LSodarCreate (int * neq, int ng);

// Allocating the problem
int LSodarMalloc (void * lsodar_mem, LSRhsFn f, realtype t0, N_Vector y, int itol, realtype reltol, void * abstol);

// Reinitializing the problem
int LSodarReInit (void * lsodar_mem, LSRhsFn f, realtype tOld, N_Vector y, int itol, realtype reltol, void * abstol);

// Initializing the root-finding problem
int LSodarRootInit (void * lsodar_mem, int ng, LSRootFn g, void *gdata);

// Specifying the maximum step size
int LSodarSetMaxStep (void * lsodar_mem, realtype hmax);

// Specifying the time beyond which the integration is not to proceed
int LSodarSetStopTime (void * lsodar_mem, realtype tcrit);

// Solving the problem
int LSodar (void * lsodar_mem, realtype tOut, N_Vector yVec, realtype * tOld, enum iTask_t itask);

// Update rootsfound to the computed jroots
int LSodarGetRootInfo (void * lsodar_mem, int * rootsfound);

// Freeing the problem memory allocated by lsodarMalloc
void LSodarFree (void ** lsodar_mem);

// Freeing the lsodar vectors allocated in lsodarAllocVectors
void LSFreeVectors (LSodarMem lsodar_mem);

// Error handling function
void LSProcessError (LSodarMem lsodar_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

// Default error handling function
void LSErrHandler (int error_code, const char *module, const char *function, char *msg, void *data);

#endif
