/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
/*-----------------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "stack-c.h"
#include "sciprint.h"
#include "gw_arnoldi.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Arpack_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Arpack_Interf f;    /** function **/
	char *name;      /** its name **/
} ArpackTable;
/*-----------------------------------------------------------------------------------*/
static ArpackTable Tab[]={
  {C2F(intdsaupd),"dsaupd"},
  {C2F(intdnaupd),"dnaupd"},
  {C2F(intznaupd),"znaupd"},
  {C2F(intdseupd),"dseupd"},
  {C2F(intdneupd),"dneupd"},
  {C2F(intzneupd),"zneupd"},
};
/*-----------------------------------------------------------------------------------*/ 
int C2F(gw_arnoldi)(void)
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


