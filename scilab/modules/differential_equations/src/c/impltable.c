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
/***********************************
* impl   (  fres, fadda, fj2 )
***********************************/

#define ARGS_fres int*,double *,double *,double *,double*,int*
typedef void (*fresf)(ARGS_fres);


#define ARGS_fadda int*,double *,double *,int*,int*,double*,int*
typedef void (*faddaf)(ARGS_fadda);

#define ARGS_fj2 int *,double *,double *,double *,int *,int *,double*,int *
typedef void (*fj2f)(ARGS_fj2);

/**************** fres ***************/
extern void C2F(resid)(ARGS_fres);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fres)(ARGS_fres);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfres)(char *name, int *rep);

FTAB FTab_fres[] =
{
    {"resid", (voidf)  C2F(resid)},
    {(char *) 0, (voidf) 0}
};
/**************** fadda ***************/
extern void C2F(aplusp)(ARGS_fadda);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fadda)(ARGS_fadda);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfadda)(char *name, int *rep);

FTAB FTab_fadda[] =
{
    {"aplusp", (voidf)  C2F(aplusp)},
    {(char *) 0, (voidf) 0}
};

/**************** fj2 ***************/
extern void C2F(dgbydy)(ARGS_fj2);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fj2)(ARGS_fj2);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfj2)(char *name, int *rep);

FTAB FTab_fj2[] =
{
    {"dgbydy", (voidf)  C2F(dgbydy)},
    {(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for impl
***********************************/

/** the current function fixed by setfres **/

static fresf fresfonc ;

/** function call **/

void C2F(fres)(int *ny, double *t, double *y, double *s, double *r, int *ires)
{
    (*fresfonc)(ny, t, y, s, r, ires);
}

/** fixes the function associated to name **/

void C2F(setfres)(char *name, int *rep)
{
    fresfonc = (fresf) GetFunctionByName(name, rep, FTab_fres);
}


/** the current function fixed by setfadda **/

static faddaf faddafonc ;

void C2F(fadda)(int *ny, double *t, double *y, int *ml, int *mu, double *p, int *nrowp)
{
    (*faddafonc)(ny, t, y, ml, mu, p, nrowp);
}

/** fixes the function associated to name **/

void C2F(setfadda)(char *name, int *rep)
{
    faddafonc = (faddaf) GetFunctionByName(name, rep, FTab_fadda);
}


/** the current function fixed by setfj2 **/

static fj2f fj2fonc ;

/** function call **/

void C2F(fj2)(int *ny, double *t, double *y, double *s, int *ml, int *mu, double *p, int *nrowp)
{
    (*fj2fonc)(ny, t, y, s, ml, mu, p, nrowp);
}

/** fixes the function associated to name **/

void C2F(setfj2)(char *name, int *rep)
{
    fj2fonc = (fj2f) GetFunctionByName(name, rep, FTab_fj2);
}
