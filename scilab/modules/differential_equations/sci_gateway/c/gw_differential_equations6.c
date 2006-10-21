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
#include "sciprint.h"

typedef int (*Differential_Equations6_Interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct 
{
	Differential_Equations6_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations6Table;
/*-----------------------------------------------------------------------------------*/
extern int C2F(scibvode) __PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations6Table Tab[]=
{
	{C2F(scibvode),"bvode"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations6)()
{  
#if _MSC_VER
#ifndef _DEBUG
	_try
	{
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		char *ExceptionString=GetExceptionString(GetExceptionCode());
		sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
		if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
	}
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
