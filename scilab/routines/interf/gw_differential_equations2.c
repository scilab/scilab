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


typedef int (*Differential_Equations2_Interf) __PARAMS((char *fname));

typedef struct table_struct 
{
	Differential_Equations2_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations2Table;
/*-----------------------------------------------------------------------------------*/
extern int C2F(sciimpl) _PARAMS((char *fname));
extern int C2F(dassli) _PARAMS((char *fname));
extern int C2F(dasrti) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations2Table Tab[]=
{
	{C2F(sciimpl),"impl"},
	{C2F(dassli),"dassl"},
	{C2F(dasrti),"dasrt"},
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations2)()
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
