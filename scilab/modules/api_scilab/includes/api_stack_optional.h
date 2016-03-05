/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __STACK_OPTIONAL_API__
#define __STACK_OPTIONAL_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_stack_optional.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rhs_opts__
{
    int iPos ; /** stack position : -1 if not present */
    char* pstName;
    int iType;// -1 not yet defined
    int iRows;
    int iCols;
    int* piAddr;
} rhs_opts;

/**
 * Get optional variable
 * @param[in] name of function caller
 * @param[in/out] opts structure of optional arguments
 * @return if the operation succeeded ( 0 ) or not ( !0 )
 */
int getOptionals(void* _pvCtx, char* fname, rhs_opts opts[]);
int FirstOpt(void* _pvCtx);
int NumOpt(void* _pvCtx);
int FindOpt(void* _pvCtx, char* pstProperty, rhs_opts opts[]);
#ifdef __cplusplus
}
#endif
#endif /* __STACK_OPTIONAL_API__ */
