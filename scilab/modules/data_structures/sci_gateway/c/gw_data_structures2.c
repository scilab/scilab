/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_data_structures2.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(error)();
/*-----------------------------------------------------------------------------------*/
typedef int (*Data_structures2_Interf) __PARAMS((char *fname,unsigned long fname_len));
typedef struct table_struct 
{
	Data_structures2_Interf f;    /** function **/
	char *name;      /** its name **/
} DataStructures2Table;
/*-----------------------------------------------------------------------------------*/	
static DataStructures2Table Tab[]=
{
	{C2F(sci_getfield),"getfield"},
	{C2F(sci_setfield),"setfield"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gwdatastructures2)(void)
{ 
	Rhs=Max(Rhs,0);
	if (C2F(vstk).top - C2F(com).rhs + C2F(com).lhs + 1 >= C2F(vstk).bot) 
	{
		static integer codeerror = 18;
		C2F(error)(&codeerror);
		return 0;
	}

#ifdef _MSC_VER
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
