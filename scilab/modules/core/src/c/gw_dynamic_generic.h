/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_DYNAMIC_GENERIC_H__
#define __GW_DYNAMIC_GENERIC_H__

#include "callDynamicGateway.h"

/**
* generic function to call a gateway dynamically loaded
* @param[in] module name , ex : scicos
* @param[in,out] dynamic library name, ex : libsciscicos.so
* @param[in,out] gateway function name , exe : scicos
* @param[in,out] dynamic handle on library
* @param[in,out] pointer on function named by gatewayName
* @return 0
*/
int gw_dynamic_generic(char *moduleName,
                       char **dynlibName,
                       char **gatewayName,
                       DynLibHandle *hModuleLib,
                       PROC_GATEWAY *ptrGatewayFunction);

#endif /* __GW_DYNAMIC_GENERIC_H__ */
/*--------------------------------------------------------------------------*/
