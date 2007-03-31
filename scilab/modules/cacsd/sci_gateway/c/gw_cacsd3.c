/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_cacsd3.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Cacsd3_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Cacsd3_Interf f;    /** function **/
	char *name;      /** its name **/
} Cacsd3Table;
/*-----------------------------------------------------------------------------------*/
static Cacsd3Table Tab[]=
{
	{C2F(sci_arl2),"arl2_ius"},
	{C2F(sci_residu),"residu"},
	{C2F(sci_ldiv),"ldiv"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd3)(void)
{  
	if ( Fin-1 > 2) return 0;
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
