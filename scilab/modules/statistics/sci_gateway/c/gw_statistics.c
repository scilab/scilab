#include "gw_statistics.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int cdfbetI(char* fname,unsigned long l);
extern int cdfbinI(char* fname,unsigned long l);
extern int cdfchiI(char* fname,unsigned long l);
extern int cdfchnI(char* fname,unsigned long l);
extern int cdffI(char* fname,unsigned long l);
extern int cdffncI(char* fname,unsigned long l);
extern int cdfgamI(char* fname,unsigned long l);
extern int cdfnbnI(char* fname,unsigned long l);
extern int cdfnorI(char* fname,unsigned long l);
extern int cdfpoiI(char* fname,unsigned long l);
extern int cdftI(char* fname,unsigned long l);
/*-----------------------------------------------------------------------------------*/
static StatisticsTable Tab[]=
{ 
	{cdfbetI, "cdfbet"},
	{cdfbinI, "cdfbin"},
	{cdfchiI, "cdfchi"},
	{cdfchnI, "cdfchn"},
	{cdffI, "cdff"},
	{cdffncI, "cdffnc"},
	{cdfgamI, "cdfgam"},
	{cdfnbnI, "cdfnbn"},
	{cdfnorI, "cdfnor"},
	{cdfpoiI, "cdfpoi"},
	{cdftI, "cdft"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_statistics)()
{  
	Rhs = Max(0, Rhs);
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
