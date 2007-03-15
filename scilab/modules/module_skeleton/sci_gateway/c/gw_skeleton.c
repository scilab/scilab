/**
 * Skeleton for the gateway source file which defines links between the
 * native function (C, fortran, whatever) and the function offered into Scilab
 * @author Sylvestre LEDRU INRIA 2007
 */
#include "gw_skeleton.h"
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
static SkeletonTable Tab[]=
{
{C2F(sci_skeleton_c),"test_skeleton_c"},
{C2F(sci_skeleton_f),"test_skeleton_f"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_skeleton)()
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
