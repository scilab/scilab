/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif

#include <string.h>

#include "machine.h"
#include "stack-c.h"
#include "sciprint.h"

typedef int (*Differential_Equations5_Interf) _PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct 
{
	Differential_Equations5_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations5Table;
/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations5)(void);
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_feval) _PARAMS((char *fname,unsigned long fname_len))
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
int C2F(gw_differential_equations5)(void)
{  
#if _MSC_VER
#ifndef _DEBUG
	_try
	{
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
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
