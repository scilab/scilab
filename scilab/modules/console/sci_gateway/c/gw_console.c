/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_console.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Console_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct
{
  Console_Interf f;    /** function **/
  char *name;      /** its name **/
} ConsoleTable;
/*-----------------------------------------------------------------------------------*/
static ConsoleTable Tab[]=
{
{C2F(sci_completion),"completion"},
{C2F(sci_clc),"clc"},
{C2F(sci_tohome),"tohome"}

};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_console)()
{  
	Rhs = Max(0, Rhs);

#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
