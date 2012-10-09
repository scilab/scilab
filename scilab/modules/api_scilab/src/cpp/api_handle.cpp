/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

extern "C"
{
#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_handle.h"
#include "localization.h"

#include "Scierror.h"
#include "call_scilab.h"
}

SciErr getMatrixOfHandle(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, long long** _pllHandle)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    //FIX ME

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr allocMatrixOfHandle(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _pllHandle)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    //FIX ME

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createMatrixOfHandle(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long* _pllHandle)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    //FIX ME

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr fillMatrixOfHandle(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, long long** _pllHandle)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    //FIX ME

    return sciErr;
}
/*--------------------------------------------------------------------------*/
int isHandleType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_handles);
}
/*--------------------------------------------------------------------------*/
int getScalarHandle(void* _pvCtx, int* _piAddress, long long* _pllHandle)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    //FIX ME

    return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarHandle(void* _pvCtx, int _iVar, long long _llHandle)
{
    SciErr sciErr;
    sciErr.iErr = 0;
    sciErr.iMsgCount = 0;

    //FIX ME

    return 0;
}
