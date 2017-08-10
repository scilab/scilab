/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2012 - Paul Bignier
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
typedef void (*LSRhsFn) (int * neq, realtype * t, realtype * y, realtype * rwork);
typedef void (*LSRootFn) (int * neq, realtype * t, realtype * y, int * ng, realtype * rwork);
typedef void (*LSErrHandlerFn) (int error_code, const char *module, const char *function, char *msg, void *user_data);

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
    LSErrHandlerFn ehfun;
} *LSodarMem;

// Creating the problem
void * LSodarCreate (int * neq, int ng);

// Allocating the problem
int LSodarInit (void * lsodar_mem, LSRhsFn f, realtype t0, N_Vector y);

// Reinitializing the problem
int LSodarReInit (void * lsodar_mem, realtype tOld, N_Vector y);

// Specifying the tolerances
int LSodarSStolerances (void * lsodar_mem, realtype reltol, realtype abstol);

// Initializing the root-finding problem
int LSodarRootInit (void * lsodar_mem, int ng, LSRootFn g);

// Specifying the maximum step size
int LSodarSetMaxStep (void * lsodar_mem, realtype hmax);

// Specifying the time beyond which the integration is not to proceed
int LSodarSetStopTime (void * lsodar_mem, realtype tcrit);

// Solving the problem
int LSodar (void * lsodar_mem, realtype tOut, N_Vector yVec, realtype * tOld, int itask);

// Update rootsfound to the computed jroots
int LSodarGetRootInfo (void * lsodar_mem, int * rootsfound);

// Freeing the problem memory allocated by lsodarMalloc
void LSodarFree (void ** lsodar_mem);

// Freeing the lsodar vectors allocated in lsodarAllocVectors
void LSFreeVectors (LSodarMem lsodar_mem);

// Specifies the error handler function
int LSodarSetErrHandlerFn (void * lsodar_mem, LSErrHandlerFn ehfun, void * eh_data);

// Error handling function
void LSProcessError (LSodarMem ls_mem, int error_code, const char *module, const char *fname, const char *msgfmt, ...);

#endif
