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
#include "machine.h"
/***********************************
* Search Table for int2d
***********************************/

#define ARGS_fint2d double *,double *
typedef double * (*fint2df)(ARGS_fint2d);

/**************** fint2d ***************/
extern void C2F(int2dex)(ARGS_fint2d);
double *C2F(fint2d)(ARGS_fint2d);
void C2F(setfint2d)(char *name, int *rep);

FTAB FTab_fint2d[] =
{
	{"int2dex", (voidf)  C2F(int2dex)},
	{(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for int2d
*    uses : fint2d
***********************************/

/** the current function fixed by setfint2d **/

static fint2df fint2dfonc ;

/** function call : WARNING fintg returns a double  **/

double *C2F(fint2d)(double *x, double *y)
{
	return((*fint2dfonc)(x,y));
}

/** fixes the function associated to name **/

void C2F(setfint2d)(char *name, int *rep)
{
	fint2dfonc = (fint2df) AddFunctionInTable(name,rep,FTab_fint2d);
}
