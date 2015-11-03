/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
    SciErr getPointer(void* _pvCtx, int* _piAddress, void const** _pvPtr);

    /**
     * Create pointer variable
     * @param[in] _iVar variable number
     * @param[in] _pvPtr pointer
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createPointer(void* _pvCtx, int _iVar, const void* _pvPtr);

    /**
     * Create named pointer variable
     * @param[in] _pstName variable name
     * @param[in] _pvPtr pointer
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr createNamedPointer(void* _pvCtx, const char* _pstName, const void* _pvPtr);

    /**
     * Read pointer named variable
     * @param[in] _pstName variable name
     * @param[out] _pvPtr return pointer
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    SciErr readNamedPointer(void* _pvCtx, const char* _pstName, void const ** _pvPtr);

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
