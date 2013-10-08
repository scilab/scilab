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

#include "gatewaystruct.hxx"
#include "pointer.hxx"
#include "context.hxx"

extern "C"
{
#include "api_scilab.h"
#include "api_internal_common.h"
#include "localization.h"
}

using namespace types;

SciErr getPointer(void* _pvCtx, int* _piAddress, void** _pvPtr)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;

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

    *_pvPtr = ((InternalType*)_piAddress)->getAs<Pointer>()->get();
    return sciErr;
}

SciErr createPointer(void* _pvCtx, int _iVar, void* _pvPtr)
{
    SciErr sciErr = sciErrInit();

    if (_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "creatPointer");
        return sciErr;
    }

    GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
    InternalType** out = pStr->m_pOut;

    Pointer* pP = NULL;

    try
    {
        pP = new Pointer(_pvPtr);
        if (pP == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createPointer");
            return sciErr;
        }
    }
    catch (ast::ScilabError se)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "createPointer", se.GetErrorMessage().c_str());
        return sciErr;
    }

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pP;

    return sciErr;
}

SciErr createNamedPointer(void* _pvCtx, const char* _pstName, void* _pvPtr)
{
    SciErr sciErr = sciErrInit();
    wchar_t* pwstName = to_wide_string(_pstName);

    Pointer* pP = new Pointer(_pvPtr);
    symbol::Context::getInstance()->put(symbol::Symbol(pwstName), *pP);
    FREE(pwstName);

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

