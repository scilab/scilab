/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "dynamiclibrary_windows.h"
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibHandle LoadDynLibrary(char *libname)
{
	return (DynLibHandle) LoadLibrary(libname);
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_DYNAMICLIBRARY_DLL BOOL FreeDynLibrary(DynLibHandle hInstance)
{
	return (BOOL) FreeLibrary((HMODULE) hInstance);
}
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibFuncPtr GetFuncPtr(DynLibHandle hInstance,char *funcName)
{
	DynLibFuncPtr retFuncPtr = NULL ;
	
	if (hInstance)
	{
		retFuncPtr = GetProcAddress(hInstance, funcName);
	}
	
	return retFuncPtr;
}
/*-----------------------------------------------------------------------------------*/ 
