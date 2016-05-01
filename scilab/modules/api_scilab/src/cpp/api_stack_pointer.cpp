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

    *_pvPtr = ((types::InternalType*)_piAddress)->getAs<types::Pointer>()->get();
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

    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::InternalType** out = pStr->m_pOut;

    types::Pointer* pP = NULL;

    try
    {
        pP = new types::Pointer(_pvPtr);
        if (pP == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createPointer");
            return sciErr;
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "createPointer", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pP;

    return sciErr;
}

SciErr createNamedPointer(void* _pvCtx, const char* _pstName, void* _pvPtr)
{
    SciErr sciErr = sciErrInit();

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createNamedPointer", _pstName);
        return sciErr;
    }

    wchar_t* pwstName = to_wide_string(_pstName);

    types::Pointer* pP = new types::Pointer(_pvPtr);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pP);
    }
    else
    {
        delete pP;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }

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

