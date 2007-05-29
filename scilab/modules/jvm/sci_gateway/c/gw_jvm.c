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
#include "gw_jvm.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*jvm_Interf) __PARAMS((char *fname,unsigned long l));
/*-----------------------------------------------------------------------------------*/
typedef struct table_struct
{
	jvm_Interf f;    /** function **/
	char *name;      /** its name **/
} jvmTable;
/*-----------------------------------------------------------------------------------*/
static jvmTable Tab[]=
{
	{C2F(sci_with_embedded_jre),"with_embedded_jre"},
	{C2F(sci_system_getproperty),"system_getproperty"},
	{C2F(sci_javaclasspath),"javaclasspath"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_jvm)(void)
{  
	Rhs = Max(0, Rhs);

	if ( (getScilabMode() != SCILAB_NWNI) )
	{
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
	}
	else
	{
		Scierror(999,"JVM interface disabled in -nogui or -nwni modes.\r\n");
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
