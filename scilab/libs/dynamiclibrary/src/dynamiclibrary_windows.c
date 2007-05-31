/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
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
IMPORT_EXPORT_DYNAMICLIBRARY_DLL char * GetLastDynLibError(void)
{
	static char buffer[512];
	DWORD dw = GetLastError(); 
	DWORD source = 0;

	if (FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS, &source, dw, 0,
			buffer, 512, NULL) == 0) 
	{
			strcpy(buffer, "Unknown Error");
	}

	return buffer;
}
/*-----------------------------------------------------------------------------------*/ 