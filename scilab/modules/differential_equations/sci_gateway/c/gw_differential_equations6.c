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

typedef int (*Differential_Equations6_Interf) __PARAMS((char *fname,unsigned long fname_len));

typedef struct table_struct 
{
	Differential_Equations6_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations6Table;
/*-----------------------------------------------------------------------------------*/
extern int C2F(scibvode) __PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations6)(void);
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations6Table Tab[]=
{
	{C2F(scibvode),"bvode"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations6)(void)
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
