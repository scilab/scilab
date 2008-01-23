/*--------------------------------------------------------------------------*/
/* INRIA */
/*--------------------------------------------------------------------------*/
#include "AddFunctionInTable.h"
#include "feval.h"
/***********************************
* feval (ffeval)
***********************************/

#define ARGS_ffeval integer*,double *,double *,double *,integer*,char *
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

void C2F(ffeval)(integer *nn, double *x1, double *x2, double *xres, integer *itype, char *name)
{
	(*fevalfonc)(nn,x1,x2,xres,itype,name);
}

/** fixes the function associated to name **/

void C2F(setfeval)(char *name, int *rep)
{
	fevalfonc = (ffevalf) AddFunctionInTable(name,rep,FTab_ffeval);
}

