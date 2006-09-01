#include "gw_data_structures2.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
/*-----------------------------------------------------------------------------------*/
extern C2F(error)();
/*-----------------------------------------------------------------------------------*/	
static int C2F(sci_getfield) _PARAMS((char *fname))
{
	extern int C2F(intgetfield)();
	
	C2F(intgetfield)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static int C2F(sci_setfield) _PARAMS((char *fname))
{
	extern int C2F(intsetfield)();
	
	C2F(intsetfield)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
static DataStructures2Table Tab[]=
{
	{C2F(sci_getfield),"getfield"},
	{C2F(sci_setfield),"setfield"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gwdatastructures2)()
{ 
	Rhs=Max(Rhs,0);
	if (C2F(vstk).top - C2F(com).rhs + C2F(com).lhs + 1 >= C2F(vstk).bot) 
	{
		static integer codeerror = 18;
		C2F(error)(&codeerror);
		return 0;
	}

#if _MSC_VER
#ifndef _DEBUG
	_try
	{
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
	}
	_except (EXCEPTION_EXECUTE_HANDLER)
	{	
		char *ExceptionString=GetExceptionString(GetExceptionCode());
		sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
		if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
	}
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
#endif
#else
	(*(Tab[Fin-1].f)) (Tab[Fin-1].name);
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
