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
#include "gw_shell.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Shell_Interf) __PARAMS((char *fname,unsigned long l));

typedef struct table_struct
{
  Shell_Interf f;    /** function **/
  char *name;      /** its name **/
} ShellTable;
/*-----------------------------------------------------------------------------------*/
static ShellTable Tab[]=
{
{C2F(sci_completion),"completion"},
{C2F(sci_clc),"clc"},
{C2F(sci_tohome),"tohome"},
{C2F(sci_loadhistory),"loadhistory"},
{C2F(sci_savehistory),"savehistory"},
{C2F(sci_gethistory),"gethistory"},
{C2F(sci_resethistory),"resethistory"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_shell)()
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
