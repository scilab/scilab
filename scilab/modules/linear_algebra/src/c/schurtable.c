/*--------------------------------------------------------------------------*/
/* @author INRIA 
 * @TODO : a lot of code here could be factorized
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "linear_FTables.h"
#include "link.h"
#include "schur.h"
#include "FTables.h"

/***********************************
* Search Table for fschur
***********************************/

#define ARGS_fschur integer*,double *,double*,double*,double*
typedef integer * (*fschurf)(ARGS_fschur);


/***********************************
* Search Table for gshsel
***********************************/

#define ARGS_gshsel double *,double*,double*
typedef integer * (*gshself)(ARGS_gshsel);

/***********************************
* Search Table for gzhsel
***********************************/

#define ARGS_gzhsel doublecmplx *,doublecmplx *
typedef integer * (*gzhself)(ARGS_gzhsel);

/**************** fschur ***************/
extern void C2F(folhp)(ARGS_fschur);
extern void C2F(find)(ARGS_fschur);

FTAB FTab_fschur[] =
{
	{"find", (voidf)  C2F(find)},
	{"folhp", (voidf)  C2F(folhp)},
	{(char *) 0, (voidf) 0}
};
/**************** gshsel ***************/
extern void C2F(sb02ow)(ARGS_gshsel);
extern void C2F(sb02ox)(ARGS_gshsel);

FTAB FTab_gshsel[] =
{
	{"sb02ow", (voidf)  C2F(sb02ow)},
	{"sb02ox", (voidf)  C2F(sb02ox)},
	{(char *) 0, (voidf) 0}
};
/**************** gzhsel ***************/
extern void C2F(zb02ow)(ARGS_gzhsel);
extern void C2F(zb02ox)(ARGS_gzhsel);

FTAB FTab_gzhsel[] =
{
	{"zb02ow", (voidf)  C2F(zb02ow)},
	{"zb02ox", (voidf)  C2F(zb02ox)},
	{(char *) 0, (voidf) 0}
};


/***********************************
* Search Table for schur uses : schsel
***********************************/

/** the current function fixed by setschsel **/

static schself schselfonc ;

/** function call : schsel  **/

integer *C2F(schsel)(double *alpha, double *beta)
{
	return((*schselfonc)(alpha,beta));
}

/** fixes the function associated to name **/

void C2F(setschsel)(int *len, char *name, int *rep)
{
	if ( ((strncmp(name,"c",1)== 0 ) && (*len==1)) || strncmp(name,"cont",4)== 0 )
		schselfonc = (schself) SetFunction("sb02mv",rep,FTab_schsel);
	else if ( ((strncmp(name,"d",1)== 0) && (*len==1)) || strncmp(name,"disc",4)== 0 )
		schselfonc = (schself) SetFunction("sb02mw",rep,FTab_schsel);
	else 
		schselfonc = (schself) SetFunction(name,rep,FTab_schsel);
}

/***********************************
* Search Table for schur uses : zchsel
***********************************/

/** the current function fixed by setzschsel **/

static zchself zchselfonc ;

/** function call : zchsel  **/

integer *C2F(zchsel)(doublecmplx *alpha)
{
	return((*zchselfonc)(alpha));
}

/** fixes the function associated to name **/

void C2F(setzchsel)(int *len, char *name, int *rep)
{
	if ( ((strncmp(name,"c",1)== 0) && (*len==1)) || strncmp(name,"cont",3)== 0 )
		zchselfonc = (zchself) SetFunction("zb02mv",rep,FTab_zchsel);
	else if ( ( (strncmp(name,"d",1)== 0) && (*len==1) ) || strncmp(name,"disc",4)== 0 )
		zchselfonc = (zchself) SetFunction("zb02mw",rep,FTab_zchsel);
	else 
		zchselfonc = (zchself) SetFunction(name,rep,FTab_zchsel);
}

/***********************************
* Search Table for gschur uses : gshsel
***********************************/

/** the current function fixed by setgshsel **/

static gshself gshselfonc ;

/** function call : gshsel  **/

integer *C2F(gshsel)(double *alphar, double *alphai, double *beta)
{
	return((*gshselfonc)(alphar,alphai,beta));
}

/** fixes the function associated to name **/

void C2F(setgshsel)(int *len, char *name, int *rep)
{
	if ( ((strncmp(name,"c",1)== 0) && (*len==1)) || strncmp(name,"cont",3)== 0 )
		gshselfonc = (gshself) SetFunction("sb02ow",rep,FTab_gshsel);
	else if ( ( (strncmp(name,"d",1)== 0) && (*len==1) ) || strncmp(name,"disc",4)== 0 )
		gshselfonc = (gshself) SetFunction("sb02ox",rep,FTab_gshsel);
	else 
		gshselfonc = (gshself) SetFunction(name,rep,FTab_gshsel);
}

/***********************************
* Search Table for gschur uses : gzhsel
***********************************/

/** the current function fixed by setgzhsel **/

static gzhself gzhselfonc ;

/** function call : gzhsel  **/

integer *C2F(gzhsel)(doublecmplx *alpha, doublecmplx *beta)
{
	return((*gzhselfonc)(alpha,beta));
}

/** fixes the function associated to name **/

void C2F(setgzhsel)(int *len, char *name, int *rep)
{
	if ( ((strncmp(name,"c",1)== 0) && (*len==1)) || strncmp(name,"cont",3)== 0 )
		gzhselfonc = (gzhself) SetFunction("zb02ow",rep,FTab_gzhsel);
	else if ( ( (strncmp(name,"d",1)== 0) && (*len==1) ) || strncmp(name,"disc",4)== 0 )
		gzhselfonc = (gzhself) SetFunction("zb02ox",rep,FTab_gzhsel);
	else 
		gzhselfonc = (gzhself) SetFunction(name,rep,FTab_gzhsel);
}

/***********************************
* Search Table for schur uses : fschur 
***********************************/

/** the current function fixed by setsolvf **/

static fschurf fschurfonc ;

/** function call : fschur  **/

integer *C2F(fschur)(integer *lsize, double *alpha, double *beta, double *s, double *p)
{
	return((*fschurfonc)(lsize,alpha,beta,s,p));
}

/** fixes the function associated to name **/

void C2F(setfschur)(char *name, int *rep)
{
	if (strncmp(name,"c",1)== 0 || strncmp(name,"cont",3)== 0 )
		fschurfonc = (fschurf) SetFunction("folhp",rep,FTab_fschur);
	else if (strncmp(name,"d",1)== 0 || strncmp(name,"disc",4)== 0 )
		fschurfonc = (fschurf) SetFunction("find",rep,FTab_fschur);
	else 
		fschurfonc = (fschurf) SetFunction(name,rep,FTab_fschur);
}

