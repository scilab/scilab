/*-----------------------------------------------------------------------------------*/
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include "FTables.h"

	/***********************************
	* ode   (fydot and fjac ) 
	***********************************/

#define ARGS_fydot integer*,double *,double *,double *
	typedef void (*fydotf)(ARGS_fydot);

#define ARGS_fjac integer*,double *,double *,integer*,integer*,double*,integer*
	typedef void (*fjacf)(ARGS_fjac);

/**************** fydot ***************/
extern void C2F(fex)(ARGS_fydot);
extern void C2F(fex2)(ARGS_fydot);
extern void C2F(fex3)(ARGS_fydot);
extern void C2F(fexab)(ARGS_fydot);
extern void C2F(loren)(ARGS_fydot);
extern void C2F(arnol)(ARGS_fydot);
extern void C2F(bcomp)(ARGS_fydot);
extern void C2F(lcomp)(ARGS_fydot);
void C2F(fydot)(ARGS_fydot);
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
	fydotfonc = (fydotf) SetFunction(name,rep,FTab_fydot);
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
	fjacfonc = (fjacf) SetFunction(name,rep,FTab_fjac);
}
