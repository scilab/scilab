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
#include "machine.h"
/***********************************
* Search Table for fsolve 
***********************************/

#define ARGS_fsolvf int*,double *,double*,int*
typedef void (*fsolvff)(ARGS_fsolvf);

#define ARGS_fsolvj int*,double*,double*,int*
typedef void (*fsolvjf)(ARGS_fsolvj);


/**************** fsolvf ***************/
extern void C2F(fsol1)(ARGS_fsolvf);
void C2F(fsolvf)(ARGS_fsolvf);
void C2F(setfsolvf)(char *name, int *rep);

FTAB FTab_fsolvf[] ={
{"fsol1", (voidf)  C2F(fsol1)},
{(char *) 0, (voidf) 0}};
/**************** fsolvj ***************/
extern void C2F(fsolj1)(ARGS_fsolvj);
void C2F(fsolvj)(ARGS_fsolvj);
void C2F(setfsolj)(char *name, int *rep);
void C2F(setfsolvj)(char *name, int *rep);

FTAB FTab_fsolvj[] ={
{"fsolj1", (voidf)  C2F(fsolj1)},
{(char *) 0, (voidf) 0}};

/***********************************
* Search Table for fsolve 
*    uses : fsolvf and fsolvj 
***********************************/

/** the current function fixed by setsolvf **/

static fsolvff fsolvffonc ;

/** function call : fsolvf  **/

void C2F(fsolvf)(int *n, double *x, double *fvec, int *iflag)
{
	(*fsolvffonc)(n,x,fvec,iflag);
}

/** fixes the function associated to name **/

void C2F(setfsolvf)(char *name, int *rep)
{
	fsolvffonc = (fsolvff) AddFunctionInTable(name,rep,FTab_fsolvf);
}

/** the current function fixed by setfsolvj **/

static fsolvjf fsolvjfonc ;

/** function call   **/

void C2F(fsolvj)(int *n, double *x, double *fjac, int *iflag)
{
	(*fsolvjfonc)(n,x,fjac,iflag);
}

/** fixes the function associated to name **/

void C2F(setfsolvj)(char *name, int *rep)
{
	fsolvjfonc = (fsolvjf) AddFunctionInTable(name,rep,FTab_fsolvj);
}
