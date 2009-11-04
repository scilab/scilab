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

/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "gw_dynamic_generic.h"
/*--------------------------------------------------------------------------*/
int gw_dynamic_generic(char *moduleName,
					   char **dynlibName,
					   char **gatewayName,
					   DynLibHandle *hModuleLib,
					   PROC_GATEWAY *ptrGatewayFunction)
{
	dynamic_gateway_error_code err;
	if (*dynlibName == NULL)
	{
		*dynlibName = buildModuleDynLibraryName(moduleName, DYNLIB_NAME_FORMAT_AUTO);
	}

	if (*gatewayName == NULL)
	{
		*gatewayName = buildGatewayName(moduleName);
	}

	err = callDynamicGateway(moduleName,*dynlibName,
							*gatewayName,
							hModuleLib,
							ptrGatewayFunction);
	displayErrorGateway(err,*dynlibName,*gatewayName);
	
	return 0;
}
/*--------------------------------------------------------------------------*/
