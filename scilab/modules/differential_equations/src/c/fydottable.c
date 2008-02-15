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
#include "arnol.h"
	/***********************************
	* ode   (fydot and fjac )
	***********************************/

/**
 ** @TODO : Wow !! Lot of things to kick out..
 **/


typedef void (*fydotf)(integer*,double *,double *,double *);

#define ARGS_fjac integer*,double *,double *,integer*,integer*,double*,integer*
typedef void (*fjacf)(ARGS_fjac);

/**************** fydot ***************/
extern void C2F(fex)(integer*,double *,double *,double *);
extern void C2F(fex2)(integer*,double *,double *,double *);
extern void C2F(fex3)(integer*,double *,double *,double *);
extern void C2F(fexab)(integer*,double *,double *,double *);
extern void C2F(loren)(integer*,double *,double *,double *);
extern void C2F(bcomp)(integer*,double *,double *,double *);
extern void C2F(lcomp)(integer*,double *,double *,double *);
void C2F(fydot)(integer*,double *,double *,double *);
void C2F(setfydot)(char *name, int *rep);

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
void C2F(fjac)(ARGS_fjac);
void C2F(setfjac)(char *name, int *rep);
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

void C2F(fydot)(integer *n, double *t, double *y, double *ydot)
{
	(*fydotfonc)(n,t,y,ydot);
}

/** fixes the function associated to name **/

void C2F(setfydot)(char *name, int *rep)
{
	fydotfonc = (fydotf) AddFunctionInTable(name,rep,FTab_fydot);
}


/** the current function fixed by setfjac **/

static fjacf fjacfonc ;

/** function call **/

void C2F(fjac)(integer *neq, double *t, double *y, integer *ml, integer *mu, double *pd, integer *nrpd)
{
	(*fjacfonc)(neq, t, y, ml, mu, pd, nrpd);
}

/** fixes the function associated to name **/

void C2F(setfjac)(char *name, int *rep)
{
	fjacfonc = (fjacf) AddFunctionInTable(name,rep,FTab_fjac);
}
