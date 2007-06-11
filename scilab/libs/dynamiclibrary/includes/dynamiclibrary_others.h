/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __DYNAMICLIBRARY_OTHERS_H__
#define __DYNAMICLIBRARY_OTHERS_H__

#include <dlfcn.h>
#include "machine.h" /* BOOL */

typedef void * DynLibHandle;
typedef void * DynLibFuncPtr;

/**
* Maps the specified executable module into the address space of the calling process
* @param name of dynamic library
* @return Handle to the loaded library
*/
DynLibHandle LoadDynLibrary(char *libname);

/**
* Decrements the reference count of the loaded dynamic-link library
* @param Handle to the loaded library
* @return BOOL If the function succeeds, the return value is nonzero
*/
BOOL FreeDynLibrary(DynLibHandle hInstance);

/**
* Retrieves the address of an exported function 
* @param Handle to the loaded library
* @param string that specifies the function
* @return a pointer
*/
DynLibFuncPtr GetDynLibFuncPtr(DynLibHandle hInstance,char *funcName);

/**
* return last dynamic linking error 
* @return a string
*/
char * GetLastDynLibError(void);

#endif /* __DYNAMICLIBRARY_OTHERS_H__ */
/*-----------------------------------------------------------------------------------*/ 

