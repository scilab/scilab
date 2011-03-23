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
#include "GetFunctionByName.h"
#include "dynlib_differential_equations.h"
#include "arnol.h"
	/***********************************
	* ode   (fydot and fjac )
	***********************************/

/**
 ** @TODO : Wow !! Lot of things to kick out..
 **/


typedef void (*fydotf)(int*,double *,double *,double *);

#define ARGS_fjac int*,double *,double *,int*,int*,double*,int*
typedef void (*fjacf)(ARGS_fjac);

/**************** fydot ***************/
extern void C2F(fex)(int*,double *,double *,double *);
extern void C2F(fex2)(int*,double *,double *,double *);
extern void C2F(fex3)(int*,double *,double *,double *);
extern void C2F(fexab)(int*,double *,double *,double *);
extern void C2F(loren)(int*,double *,double *,double *);
extern void C2F(bcomp)(int*,double *,double *,double *);
extern void C2F(lcomp)(int*,double *,double *,double *);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fydot)(int*,double *,double *,double *);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfydot)(char *name, int *rep);

FTAB FTab_fydot[] ={
	{"arnol", (voidf)  C2F(arnol)},
	{"bcomp", (voidf)  C2F(bcomp)},
	{"fex", (voidf)  C2F(fex)},
	{"fex2", (voidf)  C2F(fex2)},
	{"fex3", (voidf)  C2F(fex3)},
	{"fexab", (voidf)  C2F(fexab)},
	{"lcomp", (voidf)  C2F(lcomp)},
	{"loren", (voidf)  C2F(loren)},
	{(char *) 0, (voidf) 0}};

/**************** fjac ***************/
extern void C2F(jex)(ARGS_fjac);

DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(fjac)(ARGS_fjac);
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(setfjac)(char *name, int *rep);

FTAB FTab_fjac[] =
{
	{"jex", (voidf)  C2F(jex)},
	{(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for fydot
***********************************/

/** the current function fixed by setfydot **/

static fydotf fydotfonc ;

/** function call **/

void C2F(fydot)(int *n, double *t, double *y, double *ydot)
{
	(*fydotfonc)(n,t,y,ydot);
}

/** fixes the function associated to name **/

void C2F(setfydot)(char *name, int *rep)
{
	fydotfonc = (fydotf) GetFunctionByName(name,rep,FTab_fydot);
}


/** the current function fixed by setfjac **/

static fjacf fjacfonc ;

/** function call **/

void C2F(fjac)(int *neq, double *t, double *y, int *ml, int *mu, double *pd, int *nrpd)
{
	(*fjacfonc)(neq, t, y, ml, mu, pd, nrpd);
}

/** fixes the function associated to name **/

void C2F(setfjac)(char *name, int *rep)
{
	fjacfonc = (fjacf) GetFunctionByName(name,rep,FTab_fjac);
}
