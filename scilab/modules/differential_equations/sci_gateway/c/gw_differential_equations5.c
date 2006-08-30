/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"


typedef int (*Differential_Equations5_Interf) __PARAMS((char *fname));

typedef struct table_struct 
{
	Differential_Equations5_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations5Table;
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_feval) _PARAMS((char *fname))
{
	extern int C2F(feval)();
	C2F(feval)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations5Table Tab[]=
{
	{C2F(sci_feval),"feval"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations5)()
{  
#if _MSC_VER
#ifndef _DEBUG
	_try
	{
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		char *ExceptionString=GetExceptionString(GetExceptionCode());
		sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
		if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
	}
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
#endif
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
