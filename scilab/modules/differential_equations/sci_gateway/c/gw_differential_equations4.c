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

typedef int (*Differential_Equations4_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations4_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations4Table;
/*-----------------------------------------------------------------------------------*/
int C2F(sci_intg) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int2d) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_int3d) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations4)(void);
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations4Table Tab[]=
{
	{C2F(sci_intg),"intg"},
	{C2F(sci_int2d),"int2d"},
	{C2F(sci_int3d),"int3d"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations4)(void)
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
