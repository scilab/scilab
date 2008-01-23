/*--------------------------------------------------------------------------*/
/* INRIA */
/*--------------------------------------------------------------------------*/
#include "AddFunctionInTable.h"

#define ARGS_fintg double *

/**************** fintg ***************/
extern void C2F(intgex)(ARGS_fintg);
double *C2F(fintg)(ARGS_fintg);
void C2F(setfintg)(char *name, int *rep);

FTAB FTab_fintg[] ={
	{"intgex", (voidf)  C2F(intgex)},
	{(char *) 0, (voidf) 0}};

/***********************************
* Search Table for intg 
***********************************/

typedef double * (*fintgf)(ARGS_fintg);

/***********************************
* Search Table for intg 
*    uses : fintg 
***********************************/

/** the current function fixed by setfintg **/

static fintgf fintgfonc ;

/** function call : WARNING fintg returns a double  **/

double *C2F(fintg)(double *x)
{
	return((*fintgfonc)(x));
}

/** fixes the function associated to name **/

void C2F(setfintg)(char *name, int *rep)
{
	fintgfonc = (fintgf) AddFunctionInTable(name,rep,FTab_fintg);
}
