/*-----------------------------------------------------------------------------------*/
/**
* @author Allan CORNET INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_jvm.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "Scierror.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table  Tab[]=
{
	{C2F(sci_with_embedded_jre),"with_embedded_jre"},
	{C2F(sci_system_setproperty),"system_setproperty"},
	{C2F(sci_system_getproperty),"system_getproperty"},
	{C2F(sci_javaclasspath),"javaclasspath"},
	{C2F(sci_javalibrarypath),"javalibrarypath"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_jvm)(void)
{  
	Rhs = Max(0, Rhs);

	if ( (getScilabMode() != SCILAB_NWNI) )
	{
		callFunctionFromGateway(Tab);	
	}
	else
	{
		Scierror(999,"JVM interface disabled in -nogui or -nwni modes.\r\n");
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
