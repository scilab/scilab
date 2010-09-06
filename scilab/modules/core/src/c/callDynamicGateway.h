/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __CALLDYNAMICGATEWAY_H__
#define __CALLDYNAMICGATEWAY_H__

#include "BOOL.h"
#include "dynamiclibrary.h"

typedef int (*PROC_GATEWAY) (void);

typedef enum { DYN_GW_NO_ERROR = 0,
DYN_GW_LOAD_LIBRARY_ERROR = 1,
DYN_GW_PTR_FUNCTION_ERROR = 2,
DYN_GW_CALL_FUNCTION_ERROR =3

} dynamic_gateway_error_code;

typedef enum { DYNLIB_NAME_FORMAT_AUTO = 0, 
DYNLIB_NAME_FORMAT_1 = 1,
DYNLIB_NAME_FORMAT_2 = 2,
DYNLIB_NAME_FORMAT_3 = 3
} dynlib_name_format;

#define FORMATGATEWAYLIBNAME_1 "%s%s"
#define FORMATGATEWAYLIBNAME_2 "sci%s%s"
#define FORMATGATEWAYLIBNAME_3 "libsci%s%s"

/**
* load and call a scilab gateway
* @param[in] name of the Scilab module
* @param[in] name of dynamic library
* @param[in] gateway name
* @param[in, out] handle on dynamic library
* @param[in, out] pointer on gateway function
* @return error code (DYN_GW_NO_ERROR if it is ok)
*/
dynamic_gateway_error_code callDynamicGateway(char *moduleName,
                                              char *dynLibName,
                                              char *gw_name,
                                              DynLibHandle *hlib,
                                              PROC_GATEWAY *ptrGateway);

/**
* Build name of dynamic library based on module name
* @param[in] module name
* @param[in] generated dynamic lib name type
* @return name of dynamic library
*/
char *buildModuleDynLibraryName(char *modulename, dynlib_name_format iType);

/**
* Build gateway name based on module name
* @param[in] module name
* @return gateway name
*/
char *buildGatewayName(char *modulename);

/**
* Display error after a call to callDynamicGateway
* @param[in] error code returned by callDynamicGateway
* @param[in] name of dynamic library
* @param[in] gateway name
*/
void displayErrorGateway(dynamic_gateway_error_code err,
                         char *libraryname,
                         char *functionname);


/**
* free hlib and ptrGateway associated to a dynamic gateway
* @return TRUE or FALSE
*/
BOOL freeDynamicGateway(char **dynLibName,
                        char **gw_name,
                        DynLibHandle *hlib,
                        PROC_GATEWAY *ptrGateway);

#endif /* __CALLDYNAMICGATEWAY_H__ */
/*--------------------------------------------------------------------------*/
