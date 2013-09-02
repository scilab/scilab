/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "GetFunctionByName.h"
#include "machine.h"
#include "dynlib_differential_equations.h"
#define ARGS_fintg double *

/**************** fintg ***************/
extern void C2F(intgex)(ARGS_fintg);

DIFFERENTIAL_EQUATIONS_IMPEXP double *C2F(fintg)(ARGS_fintg);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfintg)(char *name, int *rep);

FTAB FTab_fintg[] =
{
    {"intgex", (voidf)  C2F(intgex)},
    {(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for intg
***********************************/

typedef double * (*fintgf)(ARGS_fintg);

/***********************************
* Search Table for intg
*    uses : fintg
***********************************/

/** the current function fixed by setfintg **/

static fintgf fintgfonc ;

/** function call : WARNING fintg returns a double  **/

double *C2F(fintg)(double *x)
{
    return((*fintgfonc)(x));
}

/** fixes the function associated to name **/

void C2F(setfintg)(char *name, int *rep)
{
    fintgfonc = (fintgf) GetFunctionByName(name, rep, FTab_fintg);
}
