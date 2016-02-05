/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GW_DYNAMIC_GENERIC_H__
#define __GW_DYNAMIC_GENERIC_H__

#include "dynamiclibrary.h"

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
                       void* ptrGatewayFunction);

#endif /* __GW_DYNAMIC_GENERIC_H__ */
/*--------------------------------------------------------------------------*/
