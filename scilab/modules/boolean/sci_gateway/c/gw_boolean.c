#include "gw_boolean.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_find) _PARAMS((char *fname,unsigned long fname_len));
static int C2F(sci_bool2s) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
extern int C2F(intor) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intand) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
int C2F(gw_boolean)(void);
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
static int C2F(sci_find) _PARAMS((char *fname,unsigned long fname_len))
{
	extern int C2F(intfind)();
	C2F(intfind)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_bool2s) _PARAMS((char *fname,unsigned long fname_len))
{
	extern int C2F(intsbool2s)();
	C2F(intsbool2s)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
