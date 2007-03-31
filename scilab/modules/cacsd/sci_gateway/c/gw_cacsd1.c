/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_cacsd1.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Cacsd1_Interf)  __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Cacsd1_Interf f;    /** function **/
	char *name;      /** its name **/

} Cacsd1Table;
/*-----------------------------------------------------------------------------------*/
static Cacsd1Table Tab[]=
{
	{C2F(scilyap),"lyap"},
	{C2F(scisylv),"sylv"},
	{C2F(intricc),"ricc"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_cacsd1)(void)
{  
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
