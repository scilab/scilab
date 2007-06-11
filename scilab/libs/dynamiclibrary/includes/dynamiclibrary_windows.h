/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __DYNAMICLIBRARY_WINDOWS_H__
#define __DYNAMICLIBRARY_WINDOWS_H__

#include <Windows.h>

#include "machine.h" /* BOOL */

#ifdef EXPORT_DYNAMICLIBRARY_DLL
	#define IMPORT_EXPORT_DYNAMICLIBRARY_DLL __declspec(dllexport)
#else
	#define IMPORT_EXPORT_DYNAMICLIBRARY_DLL __declspec(dllimport)
#endif

typedef HINSTANCE DynLibHandle;
typedef FARPROC DynLibFuncPtr;

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library
* @return Handle to the loaded library
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibHandle LoadDynLibrary(char *libname);

/**
* Decrements the reference count of the loaded dynamic-link library
* @param Handle to the loaded library
* @return BOOL If the function succeeds, the return value is nonzero
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL BOOL FreeDynLibrary(DynLibHandle hInstance);

/**
* Retrieves the address of an exported function 
* @param Handle to the loaded library
* @param string that specifies the function
* @return
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance,char *funcName);

/**
* return last dynamic linking error 
* @return a string
*/
IMPORT_EXPORT_DYNAMICLIBRARY_DLL char * GetLastDynLibError(void);

#endif /* __DYNAMICLIBRARY_WINDOWS_H__ */
/*-----------------------------------------------------------------------------------*/ 
