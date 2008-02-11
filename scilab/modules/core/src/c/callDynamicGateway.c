/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
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
	/* example gw_scicos_ --> C2F(gw_scicos) */
	#define FORMATGATEWAYNAME "gw_%s_"
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
		Scierror(999,_("Impossible to load %s library.\n"),libraryname);
		break;
	case DYN_GW_PTR_FUNCTION_ERROR :
		Scierror(999,_("Impossible to load %s function in %s library.\n"),functionname,libraryname);
		break;
	case DYN_GW_CALL_FUNCTION_ERROR :
		Scierror(999,_("Impossible to call %s in %s library.\n"),functionname,libraryname);
		break;
	}
}
/*--------------------------------------------------------------------------*/
