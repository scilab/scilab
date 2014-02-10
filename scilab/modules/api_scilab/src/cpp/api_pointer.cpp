/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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
#include "localization.h"

#include "MALLOC.h"
#include "call_scilab.h"

SciErr getPointer(void* _pvCtx, int* _piAddress, void** _pvPtr)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;
    double *pdblTmp = NULL;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getPointer");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POINTER, _("%s: Unable to get argument #%d"), "getPointer", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (iType != sci_pointer)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), "getPointer", _("pointer"));
        return sciErr;
    }

    pdblTmp = (double*)(_piAddress + 4);

    *_pvPtr = (void*)((unsigned long int)(*pdblTmp));
    return sciErr;
}

SciErr fillPointer(void* _pvCtx, int *_piAddress, void** _pvPtr)
{
    SciErr sciErr = sciErrInit();
    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillPointer");
        return sciErr;
    }

    _piAddress[0] = sci_pointer;
    _piAddress[1] = 1;
    _piAddress[2] = 1;
    _piAddress[3] = 0;

    *_pvPtr = _piAddress + 4;

    return sciErr;
}

SciErr allocPointer(void* _pvCtx, int _iVar, void** _pvPtr)
{
    SciErr sciErr = sciErrInit();
    int iNewPos = Top - Rhs + _iVar;
    int iAddr = *Lstk(iNewPos);
    int* piAddr = NULL;
    void* pvPtr = NULL;

    int iMemSize = 2;
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
    if (iMemSize > iFreeSpace)
    {
        addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

    sciErr = fillPointer(_pvCtx, piAddr, &pvPtr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_POINTER, _("%s: Unable to create variable in Scilab memory"), "allocPointer");
        return sciErr;
    }

    *_pvPtr = pvPtr;
    updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
    updateLstk(iNewPos, sadr(iadr(iAddr) + 4), 2);

    return sciErr;
}

SciErr createPointer(void* _pvCtx, int _iVar, void* _pvPtr)
{
    void* pvPtr = NULL;

    SciErr sciErr = allocPointer(_pvCtx, _iVar, &pvPtr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_POINTER, _("%s: Unable to create variable in Scilab memory"), "createPointer");
        return sciErr;
    }

    ((double*)pvPtr)[0] = (double) ((unsigned long int)_pvPtr);

    return sciErr;
}

SciErr createNamedPointer(void* _pvCtx, const char* _pstName, int* _pvPtr)
{
    SciErr sciErr = sciErrInit();
    int iVarID[nsiz];
    int iSaveRhs = Rhs;
    int iSaveTop = Top;
    void* pvPtr = NULL;
    int *piAddr = NULL;

    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createNamedPointer", _pstName);
        return sciErr;
    }

    C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
    Top = Top + Nbvars + 1;

    int iMemSize = 1;
    int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(*Lstk(Top)));
    if (iMemSize > iFreeSpace)
    {
        addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
        return sciErr;
    }

    getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

    //write matrix information
    sciErr = fillPointer(_pvCtx, piAddr, &pvPtr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_POINTER, _("%s: Unable to create %s named \"%s\""), "createNamedPointer", _("pointer"), _pstName);
        return sciErr;
    }

    //copy data in stack
    ((double*)pvPtr)[0] = (double) ((unsigned long int)_pvPtr);

    updateLstk(Top, *Lstk(Top) + sadr(4), 2);

    Rhs = 0;
    //Add name in stack reference list
    createNamedVariable(iVarID);

    Top = iSaveTop;
    Rhs = iSaveRhs;
    return sciErr;
}

SciErr readNamedPointer(void* _pvCtx, const char* _pstName, void** _pvPtr)
{
    int* piAddr = NULL;
    void *pvPtr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_POINTER, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfBoolean", _pstName);
        return sciErr;
    }

    sciErr = getPointer(_pvCtx, piAddr, &pvPtr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_POINTER, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfBoolean", _pstName);
        return sciErr;
    }

    *_pvPtr = pvPtr;

    return sciErr;
}
/*--------------------------------------------------------------------------*/
int isPointerType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_pointer);
}
/*--------------------------------------------------------------------------*/
int isNamedPointerType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_pointer);
}
/*--------------------------------------------------------------------------*/

