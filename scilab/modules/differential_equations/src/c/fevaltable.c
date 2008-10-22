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
#include "feval.h"
/***********************************
* feval (ffeval)
***********************************/

#define ARGS_ffeval int*,double *,double *,double *,int*,char *
typedef void (*ffevalf)(ARGS_ffeval);

/**************** ffeval ***************/
void C2F(ffeval)(ARGS_ffeval);
void C2F(setfeval)(char *name, int *rep);

FTAB FTab_ffeval[] ={
	{"parab", (voidf)  C2F(parab)},
	{"parabc", (voidf)  C2F(parabc)},
	{(char *) 0, (voidf) 0}};


/***********************************
* Search Table for feval 
***********************************/

/** the current function fixed by setfeval **/

static ffevalf fevalfonc ;

/** function call **/

void C2F(ffeval)(int *nn, double *x1, double *x2, double *xres, int *itype, char *name)
{
	(*fevalfonc)(nn,x1,x2,xres,itype,name);
}

/** fixes the function associated to name **/

void C2F(setfeval)(char *name, int *rep)
{
	fevalfonc = (ffevalf) AddFunctionInTable(name,rep,FTab_ffeval);
}

