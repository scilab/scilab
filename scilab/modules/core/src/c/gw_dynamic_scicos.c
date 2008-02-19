/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
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
int gw_dynamic_scicos(void)
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
