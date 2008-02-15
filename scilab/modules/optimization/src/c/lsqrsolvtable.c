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
* Search Table for lsqrsolve 
***********************************/

#define ARGS_lsqrsolvf integer*,integer*,double *,double*,integer*
typedef void (*lsqrsolvff)(ARGS_lsqrsolvf);

#define ARGS_lsqrsolvj integer*,integer*,double*,double*,integer*,integer*
typedef void (*lsqrsolvjf)(ARGS_lsqrsolvj);

/**************** lsqrsolvf ***************/
extern void C2F(lsqrsol1)(ARGS_lsqrsolvf);
void C2F(lsqrsolvf)(ARGS_lsqrsolvf);
void C2F(setlsqrsolvf)(char *name, int *rep);


FTAB FTab_lsqrsolvf[] =
{
{"lsqrsol1", (voidf)  C2F(lsqrsol1)},
{(char *) 0, (voidf) 0}
};
/**************** lsqrsolvj ***************/
extern void C2F(lsqrsolj1)(ARGS_lsqrsolvj);
void C2F(lsqrsolvj)(ARGS_lsqrsolvj);
void C2F(setlsqrsolvj)(char *name, int *rep);

FTAB FTab_lsqrsolvj[] ={
{"lsqrsolj1", (voidf)  C2F(lsqrsolj1)},
{(char *) 0, (voidf) 0}
};

/***********************************
* Search Table for fsolve 
*    uses : lsqrsolvf and lsqrsolvj 
***********************************/

/** the current function fixed by setsolvf **/

static lsqrsolvff lsqrsolvffonc ;

/** function call : lsqrsolvf  **/

void C2F(lsqrsolvf)(integer *m, integer *n, double *x, double *fvec, integer *iflag)
{
	(*lsqrsolvffonc)(m,n,x,fvec,iflag);
}

/** fixes the function associated to name **/

void C2F(setlsqrsolvf)(char *name, int *rep)
{
	lsqrsolvffonc = (lsqrsolvff) AddFunctionInTable(name,rep,FTab_lsqrsolvf);
}

/** the current function fixed by setfsolvj **/

static lsqrsolvjf lsqrsolvjfonc ;

/** function call   **/

void C2F(lsqrsolvj)(integer *m, integer *n, double *x, double *fjac, integer *ldfjac, integer *iflag)
{
	(*lsqrsolvjfonc)(m,n,x,fjac,ldfjac,iflag);
}

/** fixes the function associated to name **/

void C2F(setlsqrsolvj)(char *name, int *rep)
{
	lsqrsolvjfonc = (lsqrsolvjf) AddFunctionInTable(name,rep,FTab_lsqrsolvj);
}
