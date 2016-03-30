/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __STACK_POINTER_API__
#define __STACK_POINTER_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_stack_pointer.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"

/**
* Get pointer variable
* @param[in] _piAddress variable address
* @param[out] _pvPtr return pointer
* @return if the operation succeeded ( 0 ) or not ( !0 )
*/
SciErr getPointer(void* _pvCtx, int* _piAddress, void** _pvPtr);

/**
* Create pointer variable
* @param[in] _iVar variable number
* @param[in] _pvPtr pointer
* @return if the operation succeeded ( 0 ) or not ( !0 )
*/
SciErr createPointer(void* _pvCtx, int _iVar, void* _pvPtr);

/**
* Create named pointer variable
* @param[in] _pstName variable name
* @param[in] _pvPtr pointer
* @return if the operation succeeded ( 0 ) or not ( !0 )
*/
SciErr createNamedPointer(void* _pvCtx, const char* _pstName, void* _pvPtr);

/**
* Read pointer named variable
* @param[in] _pstName variable name
* @param[out] _pvPtr return pointer
* @return if the operation succeeded ( 0 ) or not ( !0 )
*/
SciErr readNamedPointer(void* _pvCtx, const char* _pstName, void** _pvPtr);

/* shortcut functions */

/**
* Check if the variable is of pointer type
* @param[in] _piAddress variable address
* @return 1 for true and 0 for false
*/
int isPointerType(void* _pvCtx, int* _piAddress);

/**
* check if the named variable type of pointer type
* @param[in] _pstName variable name
* @return 1 for true and 0 for false
*/
int isNamedPointerType(void* _pvCtx, const char* _pstName);

#ifdef __cplusplus
}
#endif
#endif /* __STACK_POINTER_API__ */
