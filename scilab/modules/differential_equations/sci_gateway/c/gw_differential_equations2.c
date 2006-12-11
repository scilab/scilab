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

typedef int (*Differential_Equations2_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations2_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations2Table;
/*-----------------------------------------------------------------------------------*/
extern int C2F(sciimpl) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(dassli) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(dasrti) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations2)(void);
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations2Table Tab[]=
{
	{C2F(sciimpl),"impl"},
	{C2F(dassli),"dassl"},
	{C2F(dasrti),"dasrt"},
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations2)(void)
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
