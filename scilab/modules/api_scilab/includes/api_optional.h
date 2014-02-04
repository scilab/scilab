/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 */

#ifndef __OPTIONAL_API__
#define __OPTIONAL_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_optional.h. Include api_scilab.h instead.
#endif

#include "stack-optional.h"

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Get optional variable
     * @param[in] name of function caller
     * @param[in/out] opts structure of optional arguments
     * @return if the operation succeeded ( 0 ) or not ( !0 )
     */
    int getOptionals(void* _pvCtx, char* fname, rhs_opts opts[]);

#ifdef __cplusplus
}
#endif
#endif /* __OPTIONAL_API__ */
