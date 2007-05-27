/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "dynamiclibrary_others.h"
/*-----------------------------------------------------------------------------------*/ 
DynLibHandle LoadDynLibrary(char *libname)
{
	return (DynLibHandle) dlopen(libname,  RTLD_NOW | RTLD_GLOBAL);
}
/*-----------------------------------------------------------------------------------*/ 
FBOOL FreeDynLibrary(DynLibHandle hInstance)
{
	BOOL bOK = FALSE;
	if (dlclose( hInstance)) bOK = TRUE;
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
DynLibFuncPtr GetFuncPtr(DynLibHandle hInstance,char *funcName)
{
	DynLibFuncPtr retFuncPtr = NULL ;
	
	if (hInstance)
	{
		retFuncPtr = dlsym(hInstance, funcName);
	}
	return retFuncPtr;
}
/*-----------------------------------------------------------------------------------*/ 
