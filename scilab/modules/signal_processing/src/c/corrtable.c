/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#include "GetFunctionByName.h"
#include "dynlib_signal_processing.h"
#include "machine.h"
/***********************************
* corr ( dgetx dgety )
***********************************/

#define ARGS_dgetx double *,int*,int*
typedef void (*dgetxf)(ARGS_dgetx);

#define ARGS_dgety double *,int*,int*
typedef void (*dgetyf)(ARGS_dgety);


/**************** dgetx ***************/
extern void C2F(corexx)(ARGS_dgetx);
SIGNAL_PROCESSING_IMPEXP void C2F(dgetx)(ARGS_dgetx);
SIGNAL_PROCESSING_IMPEXP void C2F(setdgetx)(char *name, int *rep);

FTAB FTab_dgetx[] =
{
    {"corexx", (voidf)  C2F(corexx)},
    {(char *) 0, (voidf) 0}
};

/**************** dgety ***************/
extern void C2F(corexy)(ARGS_dgety);
SIGNAL_PROCESSING_IMPEXP void C2F(dgety)(ARGS_dgety);
SIGNAL_PROCESSING_IMPEXP void C2F(setdgety)(char *name, int *rep);

FTAB FTab_dgety[] =
{
    {"corexy", (voidf)  C2F(corexy)},
    {(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for corr
*   corr uses two externals : dgetx and dgety
***********************************/

/** the current function fixed by setdgetx **/


static dgetxf dgetxfonc ;

/** function call **/

void C2F(dgetx)(double *x, int *incr, int *istart)
{
    (*dgetxfonc)(x, incr, istart);
}

/** fixes the function associated to name **/

void C2F(setdgetx)(char *name, int *rep)
{
    dgetxfonc = (dgetxf) GetFunctionByName(name, rep, FTab_dgetx);
}


/** the current function fixed by setdgety **/

static dgetyf dgetyfonc ;

void C2F(dgety)(double *y, int *incr, int *istart)
{
    (*dgetyfonc)(y, incr, istart);
}


/** fixes the function associated to name **/

void C2F(setdgety)(char *name, int *rep)
{
    dgetyfonc = (dgetyf) GetFunctionByName(name, rep, FTab_dgety);
}

