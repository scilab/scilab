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
/*--------------------------------------------------------------------------*/
#include "AddFunctionInTable.h"
/***********************************
* Search Table for foptim 
***********************************/
#define ARGS_foptim integer*,integer*,double *,double*,double*,integer*,float*,double*
typedef void (*foptimf)(ARGS_foptim);

/**************** foptim ***************/
extern void C2F(genros)(ARGS_foptim);
extern void C2F(topt2)(ARGS_foptim);
extern void C2F(icsemc)(ARGS_foptim);

void C2F(foptim)(ARGS_foptim);
void C2F(setfoptim)(char *name, int *rep);

FTAB FTab_foptim[] =
{
	{"genros", (voidf)  C2F(genros)},
	{"icsemc", (voidf)  C2F(icsemc)},
	{"topt2", (voidf)  C2F(topt2)},
	{(char *) 0, (voidf) 0}
};
/***********************************
* Search Table for optim 
*    uses : foptim 
***********************************/

/** the current function fixed by setsolvf **/

static foptimf foptimfonc ;

/** function call : foptim  **/

void C2F(foptim)(integer *indsim, integer *n, double *x, double *f, double *g, integer *izs, float *rzs, double *dzs)
{
	(*foptimfonc)(indsim,n,x,f,g,izs,rzs,dzs);
}

/** fixes the function associated to name **/

void C2F(setfoptim)(char *name, int *rep)
{
	foptimfonc = (foptimf) AddFunctionInTable(name,rep,FTab_foptim);
}

