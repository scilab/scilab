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
/*--------------------------------------------------------------------------*/
#include "GetFunctionByName.h"
#include "machine.h"
#include "dynlib_optimization.h"
/***********************************
* Search Table for fsolve
***********************************/

#define ARGS_fsolvf int*,double *,double*,int*
typedef void (*fsolvff)(ARGS_fsolvf);

#define ARGS_fsolvj int*,double*,double*,int*
typedef void (*fsolvjf)(ARGS_fsolvj);


/**************** fsolvf ***************/
extern void C2F(fsol1)(ARGS_fsolvf);
OPTIMIZATION_IMPEXP void C2F(fsolvf)(ARGS_fsolvf);
OPTIMIZATION_IMPEXP void C2F(setfsolvf)(char *name, int *rep);

FTAB FTab_fsolvf[] =
{
    {"fsol1", (voidf)  C2F(fsol1)},
    {(char *) 0, (voidf) 0}
};
/**************** fsolvj ***************/
extern void C2F(fsolj1)(ARGS_fsolvj);
OPTIMIZATION_IMPEXP void C2F(fsolvj)(ARGS_fsolvj);
OPTIMIZATION_IMPEXP void C2F(setfsolj)(char *name, int *rep);
OPTIMIZATION_IMPEXP void C2F(setfsolvj)(char *name, int *rep);

FTAB FTab_fsolvj[] =
{
    {"fsolj1", (voidf)  C2F(fsolj1)},
    {(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for fsolve
*    uses : fsolvf and fsolvj
***********************************/

/** the current function fixed by setsolvf **/

static fsolvff fsolvffonc ;

/** function call : fsolvf  **/

void C2F(fsolvf)(int *n, double *x, double *fvec, int *iflag)
{
    (*fsolvffonc)(n, x, fvec, iflag);
}

/** fixes the function associated to name **/

void C2F(setfsolvf)(char *name, int *rep)
{
    fsolvffonc = (fsolvff) GetFunctionByName(name, rep, FTab_fsolvf);
}

/** the current function fixed by setfsolvj **/

static fsolvjf fsolvjfonc ;

/** function call   **/

void C2F(fsolvj)(int *n, double *x, double *fjac, int *iflag)
{
    (*fsolvjfonc)(n, x, fjac, iflag);
}

/** fixes the function associated to name **/

void C2F(setfsolvj)(char *name, int *rep)
{
    fsolvjfonc = (fsolvjf) GetFunctionByName(name, rep, FTab_fsolvj);
}
