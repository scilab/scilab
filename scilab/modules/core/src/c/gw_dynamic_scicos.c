/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "gw_dynamic_scicos.h"
#include "callDynamicGateway.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
#define SCICOS_MODULE_NAME "scicos"
static DynLibHandle hScicosLib = NULL;
static PROC_GATEWAY ptr_gw_scicos = NULL;
static char* dynlibname_scicos = NULL;
static char* gatewayname_scicos = NULL;
/*--------------------------------------------------------------------------*/
int C2F(gw_dynamic_scicos)(void)
{
	dynamic_gateway_error_code err;
	if (dynlibname_scicos == NULL)
	{
		dynlibname_scicos = buildModuleDynLibraryName(SCICOS_MODULE_NAME);
	}

	if (gatewayname_scicos == NULL)
	{
		gatewayname_scicos = buildGatewayName(SCICOS_MODULE_NAME);
	}

	err = callDynamicGateway(dynlibname_scicos,
							gatewayname_scicos,
							&hScicosLib,
							&ptr_gw_scicos);
	displayErrorGateway(err,dynlibname_scicos,gatewayname_scicos);
	
	return 0;
}
/*--------------------------------------------------------------------------*/
