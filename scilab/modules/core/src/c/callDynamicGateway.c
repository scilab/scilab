/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Sylvest LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "callDynamicGateway.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
dynamic_gateway_error_code callDynamicGateway(char *dynLibName,
						char *gw_name,
						DynLibHandle *hlib,
						PROC_GATEWAY *ptrGateway)
{
	if (*hlib == NULL)
	{
		*hlib = LoadDynLibrary(dynLibName);
		if (*hlib == NULL) return DYN_GW_LOAD_LIBRARY_ERROR;
	}

	if (*ptrGateway == NULL)
	{
		*ptrGateway = (PROC_GATEWAY) GetDynLibFuncPtr(*hlib,gw_name);
		if (*ptrGateway == NULL) return DYN_GW_PTR_FUNCTION_ERROR ;
	}

	if ( (*hlib) && (*ptrGateway) )
	{
		(*ptrGateway)();
		return DYN_GW_NO_ERROR;
	}
	return DYN_GW_CALL_FUNCTION_ERROR;
}
/*--------------------------------------------------------------------------*/
char *buildModuleDynLibraryName(char *modulename)
{
	#ifdef _MSC_VER
		/* example scicos.dll */
		#define FORMATGATEWAYLIBNAME "%s%s"
	#else
		/* example libsciscicos.so */
		#define FORMATGATEWAYLIBNAME "libsci%s%s"
	#endif

	char *dynlibname = NULL;
	int lenName = (int)(strlen(modulename)+strlen(SHARED_LIB_EXT)+strlen(FORMATGATEWAYLIBNAME));
	dynlibname = (char*)MALLOC(sizeof(char)*(lenName+1));

	if (dynlibname)
	{
		sprintf(dynlibname,FORMATGATEWAYLIBNAME,modulename,SHARED_LIB_EXT);
	}
	
	return dynlibname;
}
/*--------------------------------------------------------------------------*/
char *buildGatewayName(char *modulename)
{
	/* example gw_scicos */
	#define FORMATGATEWAYNAME "gw_%s" 
	/* @TODO THIS MUST BE REWRITED !!! It is not portable at all */

	char *gatewayname = NULL;
	int lenName = (int)(strlen(modulename)+strlen(FORMATGATEWAYLIBNAME));
	gatewayname = (char*)MALLOC(sizeof(char)*(lenName+1));

	if (gatewayname)
	{
		sprintf(gatewayname,FORMATGATEWAYNAME,modulename);
	}

	return gatewayname;
}
/*--------------------------------------------------------------------------*/
void displayErrorGateway(dynamic_gateway_error_code err,char *libraryname,char *functionname)
{
	switch (err)
	{
	case DYN_GW_NO_ERROR :
		/* NO ERROR */
		break;
	case DYN_GW_LOAD_LIBRARY_ERROR :
		Scierror(999,_("Impossible to load %s library: %s\n"),libraryname,GetLastDynLibError());
		break;
	case DYN_GW_PTR_FUNCTION_ERROR :
		Scierror(999,_("Impossible to load %s function in %s library: %s\n"),functionname,libraryname, GetLastDynLibError());
		break;
	case DYN_GW_CALL_FUNCTION_ERROR :
		Scierror(999,_("Impossible to call %s in %s library: %s\n"),functionname,libraryname,GetLastDynLibError());
		break;
	}
}
/*--------------------------------------------------------------------------*/
