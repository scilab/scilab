/*--------------------------------------------------------------------------*/
/* INRIA */
/*--------------------------------------------------------------------------*/
#include "FTables.h"
/***********************************
* Search Table for int3d
***********************************/

#define ARGS_fint3d double *,integer*,double *
typedef void (*fint3df)(ARGS_fint3d);

/**************** fint3d ***************/
extern void C2F(int3dex)(ARGS_fint3d);
void C2F(fint3d)(ARGS_fint3d);
void C2F(setfint3d)(char *name, int *rep);

FTAB FTab_fint3d[] =
{
	{"int3dex", (voidf)  C2F(int3dex)},
	{(char *) 0, (voidf) 0}
};


/***********************************
* Search Table for int3d
*    uses : fint3d
***********************************/

/** the current function fixed by setfint3d **/

static fint3df fint3dfonc ;

/** function call : WARNING fintg returns a double  **/

void C2F(fint3d)(double *xyz, integer *numfun, double *v)
{
	(*fint3dfonc)(xyz,numfun,v);
}

/** fixes the function associated to name **/

void C2F(setfint3d)(char *name, int *rep)
{
	fint3dfonc = (fint3df) SetFunction(name,rep,FTab_fint3d);
}
