/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2010 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "callDynamicGateway.h"
#include "gw_dynamic_generic.h"
#include "MALLOC.h"
#include "dynamic_parallel.h"
/*--------------------------------------------------------------------------*/
/* PARALLEL module */
#define PARALLEL_MODULE_NAME "parallel"
static DynLibHandle hParallelLib = NULL;
static PROC_GATEWAY ptr_gw_parallel = NULL;
static char* dynlibname_parallel = NULL;
static char* gatewayname_parallel = NULL;
/*--------------------------------------------------------------------------*/
int gw_dynamic_parallel(void)
{
    return gw_dynamic_generic(PARALLEL_MODULE_NAME,
        &dynlibname_parallel,
        &gatewayname_parallel,
        &hParallelLib,
        &ptr_gw_parallel);
}
/*--------------------------------------------------------------------------*/
#define PARALLEL_CONCURRENCY_NAME "parallelConcurrency"
typedef int (*PROC_PARALLEL_CONCURRENCY) (void);
static PROC_PARALLEL_CONCURRENCY ptr_parallel_concurrency = NULL;
/*--------------------------------------------------------------------------*/
int dynParallelConcurrency(void)
{
    if (hParallelLib)
    {
        if (ptr_parallel_concurrency == NULL)
        {
            ptr_parallel_concurrency = (PROC_PARALLEL_CONCURRENCY) GetDynLibFuncPtr(hParallelLib, 
                PARALLEL_CONCURRENCY_NAME);
            if (ptr_parallel_concurrency == NULL) return 0;
        }
        return (ptr_parallel_concurrency)();
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
#define PARALLEL_FORBIDDEN_NAME "parallelForbidden"
typedef BOOL (*PROC_PARALLEL_FORBIDDEN) (const char *);
static PROC_PARALLEL_FORBIDDEN ptr_parallel_forbidden = NULL;
/*--------------------------------------------------------------------------*/
int dynParallelForbidden(char const* fname)
{
    if (hParallelLib)
    {
        if (ptr_parallel_forbidden == NULL)
        {
            ptr_parallel_forbidden = (PROC_PARALLEL_FORBIDDEN) GetDynLibFuncPtr(hParallelLib, 
                PARALLEL_FORBIDDEN_NAME);
            if (ptr_parallel_forbidden == NULL) return 0;
        }
        return (ptr_parallel_forbidden)(fname);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
BOOL dynTerminateParallel(void)
{
    if (ptr_parallel_concurrency) ptr_parallel_concurrency = NULL;
    if (ptr_parallel_forbidden) ptr_parallel_forbidden = NULL;
    return freeDynamicGateway(&dynlibname_parallel,
        &gatewayname_parallel,
        &hParallelLib,
        &ptr_gw_parallel);
}
/*--------------------------------------------------------------------------*/

