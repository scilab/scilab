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
/***********************************
* impl   (  fres, fadda, fj2 )
***********************************/

#define ARGS_fres integer*,double *,double *,double *,double*,integer*
typedef void (*fresf)(ARGS_fres);


#define ARGS_fadda integer*,double *,double *,integer*,integer*,double*,integer*
typedef void (*faddaf)(ARGS_fadda);

#define ARGS_fj2 integer *,double *,double *,double *,integer *,integer *,double*,integer *
typedef void (*fj2f)(ARGS_fj2);

/**************** fres ***************/
extern void C2F(resid)(ARGS_fres);
void C2F(fres)(ARGS_fres);
void C2F(setfres)(char *name, int *rep);

FTAB FTab_fres[] ={
	{"resid", (voidf)  C2F(resid)},
	{(char *) 0, (voidf) 0}};
/**************** fadda ***************/
extern void C2F(aplusp)(ARGS_fadda);
void C2F(fadda)(ARGS_fadda);
void C2F(setfadda)(char *name, int *rep);

FTAB FTab_fadda[] ={
{"aplusp", (voidf)  C2F(aplusp)},
{(char *) 0, (voidf) 0}};

/**************** fj2 ***************/
extern void C2F(dgbydy)(ARGS_fj2);
void C2F(fj2)(ARGS_fj2);
void C2F(setfj2)(char *name, int *rep);

FTAB FTab_fj2[] ={
	{"dgbydy", (voidf)  C2F(dgbydy)},
	{(char *) 0, (voidf) 0}};

/***********************************
* Search Table for impl 
***********************************/

/** the current function fixed by setfres **/

static fresf fresfonc ;

/** function call **/

void C2F(fres)(integer *ny, double *t, double *y, double *s, double *r, integer *ires)
{
	(*fresfonc)(ny,t,y,s,r,ires);
}

/** fixes the function associated to name **/

void C2F(setfres)(char *name, int *rep)
{
	fresfonc = (fresf) AddFunctionInTable(name,rep,FTab_fres);
}


/** the current function fixed by setfadda **/

static faddaf faddafonc ;

void C2F(fadda)(integer *ny, double *t, double *y, integer *ml, integer *mu, double *p, integer *nrowp)
{
	(*faddafonc)(ny,t,y,ml,mu,p,nrowp);
}

/** fixes the function associated to name **/

void C2F(setfadda)(char *name, int *rep)
{
	faddafonc = (faddaf) AddFunctionInTable(name,rep,FTab_fadda);
}


/** the current function fixed by setfj2 **/

static fj2f fj2fonc ;

/** function call **/

void C2F(fj2)(integer *ny, double *t, double *y, double *s, integer *ml, integer *mu, double *p, integer *nrowp)
{
	(*fj2fonc)(ny,t,y,s,ml,mu,p,nrowp);
}

/** fixes the function associated to name **/

void C2F(setfj2)(char *name, int *rep)
{
	fj2fonc = (fj2f) AddFunctionInTable(name,rep,FTab_fj2);
}
