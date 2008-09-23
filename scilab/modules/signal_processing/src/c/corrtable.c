/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AddFunctionInTable.h"
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
void C2F(dgetx)(ARGS_dgetx);
void C2F(setdgetx)(char *name, int *rep);

FTAB FTab_dgetx[] =
{
	{"corexx", (voidf)  C2F(corexx)},
	{(char *) 0, (voidf) 0}
};

/**************** dgety ***************/
extern void C2F(corexy)(ARGS_dgety);
void C2F(dgety)(ARGS_dgety);
void C2F(setdgety)(char *name, int *rep);

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
	dgetxfonc = (dgetxf) AddFunctionInTable(name,rep,FTab_dgetx);
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
	dgetyfonc = (dgetyf) AddFunctionInTable(name,rep,FTab_dgety);
}

