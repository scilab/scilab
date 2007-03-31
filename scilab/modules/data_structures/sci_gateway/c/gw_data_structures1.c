/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_data_structures1.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Data_structures1_Interf) __PARAMS((char *fname,unsigned long fname_len));
typedef struct table_struct 
{
	Data_structures1_Interf f;    /** function **/
	char *name;      /** its name **/
} DataStructures1Table;
/*-----------------------------------------------------------------------------------*/
extern int C2F(error)();
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
