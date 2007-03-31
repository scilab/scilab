/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
#include "gw_boolean.h"
#include "stack-c.h"
#include "sciprint.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Boolean_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Boolean_Interf f;    /** function **/
	char *name;      /** its name **/
} BooleanTable;
/*-----------------------------------------------------------------------------------*/
static BooleanTable Tab[]=
{
	{C2F(sci_find),"find"},
	{C2F(sci_bool2s),"bool2s"},
	{C2F(intor),"or"},
	{C2F(intand),"and"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_boolean)()
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
