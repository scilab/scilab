/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be
 * still available and supported in Scilab 6.
 */

#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_handle.h"
#include "localization.h"

#include "Scierror.h"
#include "call_scilab.h"



SciErr getMatrixOfHandle(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, long long** _pllHandle)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfHandle");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr || iType != sci_handles)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), "getMatrixOfHandle", _("handle matrix"));
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_HANDLE, _("%s: Unable to get argument #%d"), "getMatrixOfHandle", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (_pllHandle)
    {
        *_pllHandle = (long long*)(_piAddress + 4);
    }
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr allocMatrixOfHandle(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long** _pllHandle)
{
    SciErr sciErr = sciErrInit();
    int iNewPos = Top - Rhs + _iVar;
    int iAddr = *Lstk(iNewPos);
    int* piAddr = NULL;

    int iMemSize = _iRows * _iCols + 2;
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
    if (iMemSize > iFreeSpace)
    {
        addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
    fillMatrixOfHandle(_pvCtx, piAddr, _iRows, _iCols, _pllHandle);

    int iSCIAddress = sadr(iadr(iAddr) + 4);
    updateInterSCI(_iVar, '$', iAddr, iSCIAddress);
    updateLstk(iNewPos, sadr(iadr(iAddr) + 4), _iRows * _iCols);
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createMatrixOfHandle(void* _pvCtx, int _iVar, int _iRows, int _iCols, long long* _pllHandle)
{
    long long* pllHandle = NULL;

    if (_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        SciErr sciErr = createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        }
        return sciErr;
    }

    SciErr sciErr = allocMatrixOfHandle(_pvCtx, _iVar, _iRows, _iCols, &pllHandle);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_HANDLE, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfHandle");
        return sciErr;
    }

    memcpy(pllHandle, _pllHandle, sizeof(long long) * _iRows * _iCols);
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr fillMatrixOfHandle(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, long long** _pllHandle)
{
    SciErr sciErr = sciErrInit();
    _piAddress[0]	= sci_handles;
    _piAddress[1] = Min(_iRows, _iRows * _iCols);
    _piAddress[2] = Min(_iCols, _iRows * _iCols);

    *_pllHandle = (long long*)(_piAddress + 4);
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
    int iRows	= 0;
    int iCols	= 0;

    long long* pllHandle = NULL;

    SciErr sciErr = getMatrixOfHandle(_pvCtx, _piAddress, &iRows, &iCols, &pllHandle);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_HANDLE, _("%s: Unable to get argument #%d"), "getScalarHandle", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (isScalar(_pvCtx, _piAddress) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_HANDLE, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "getScalarHandle", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (_pllHandle != NULL)
    {
        *_pllHandle	= pllHandle[0];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarHandle(void* _pvCtx, int _iVar, long long _llHandle)
{
    long long* pllHandle = NULL;

    SciErr sciErr = allocMatrixOfHandle(_pvCtx, _iVar, 1, 1, &pllHandle);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_HANDLE, _("%s: Unable to create variable in Scilab memory"), "createScalarHandle");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    pllHandle[0] = _llHandle;
    return 0;
}
