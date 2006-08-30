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


typedef int (*Differential_Equations1_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct 
{
	Differential_Equations1_Interf f;    /** function **/
	char *name;      /** its name **/
} DifferentialEquations1Table;
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_ode) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_intg) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_feval) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_bvode) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_none_empty) _PARAMS((char *fname,unsigned long fname_len))
{
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static DifferentialEquations1Table Tab[]=
{
	{C2F(sci_ode),"ode"},
	{C2F(sci_none_empty),"sci_none_empty"},
	{C2F(sci_intg),"intg"},
	{C2F(sci_feval),"feval"},
	{C2F(sci_bvode),"bvode"}
};

/*-----------------------------------------------------------------------------------*/
int C2F(gw_differential_equations1)()
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
