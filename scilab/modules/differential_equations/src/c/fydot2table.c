/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "AddFunctionInTable.h"

extern int C2F(getcodc)(integer *nd1, integer *iflag1);

/***********************************
* Search Table for odedc
***********************************/

#define ARGS_fydot2 integer*, integer*,integer*,double *,double*,double* 
#define ARGS_fydot2f integer *, double *, double *, double *
typedef integer * (*fydot2f)(ARGS_fydot2);


/**************** fydot2 ***************/
extern void C2F(fexcd)(ARGS_fydot2);
extern void C2F(fcd)(ARGS_fydot2);
extern void C2F(fcd1)(ARGS_fydot2);
extern void C2F(phis)(ARGS_fydot2);
extern void C2F(phit)(ARGS_fydot2);
void C2F(fydot2)(ARGS_fydot2f);
void C2F(setfydot2)(char *name, int *rep);

FTAB FTab_fydot2[] =
{
	{"fcd", (voidf)  C2F(fcd)},
	{"fcd1", (voidf)  C2F(fcd1)},
	{"fexcd", (voidf)  C2F(fexcd)},
	{"phis", (voidf)  C2F(phis)},
	{"phit", (voidf)  C2F(phit)},
	{(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for fydot2
***********************************/

/** the current function fixed by setfydot2 **/

static fydot2f fydot2fonc ;

/** function call **/

void C2F(fydot2)(integer *n, double *t, double *y, double *ydot)
{
	integer nd1,iflag1;
	C2F(getcodc)(&nd1,&iflag1);
	(*fydot2fonc)(&iflag1,n,&nd1,t,y,ydot);
}

/** fixes the function associated to name **/

void C2F(setfydot2)(char *name, int *rep)
{
	fydot2fonc = (fydot2f) AddFunctionInTable(name,rep,FTab_fydot2);
}
