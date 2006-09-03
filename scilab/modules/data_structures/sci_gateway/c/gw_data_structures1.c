#include "gw_data_structures1.h"
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
extern int C2F(sci_glist) _PARAMS((char *fname));
extern int C2F(sci_scilist) _PARAMS((char *fname));
extern int C2F(sci_lsslist) _PARAMS((char *fname));
extern int C2F(sci_rlist) _PARAMS((char *fname));
extern int C2F(sci_scitlist) _PARAMS((char *fname));
extern int C2F(sci_lstcat) _PARAMS((char *fname));
extern int C2F(sci_scimlist) _PARAMS((char *fname));
extern int C2F(sci_definedfields) _PARAMS((char *fname));
extern int C2F(sci_lstsize) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/	
extern C2F(error)();
/*-----------------------------------------------------------------------------------*/
static DataStructures1Table Tab[]=
{
	{C2F(sci_scilist),"list"},
	{C2F(sci_scitlist),"tlist"},
	{C2F(sci_rlist),"rlist"},
	{C2F(sci_lsslist),"lsslist"},
	{C2F(sci_glist),"glist"},
	{C2F(sci_lstcat),"lstcat"},
	{C2F(sci_scimlist),"mlist"},
	{C2F(sci_definedfields),"definedfields"},
	{C2F(sci_lstsize),"lstsize"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gwdatastructures1)()
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
