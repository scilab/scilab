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

#include <string.h>
#include <stdlib.h>

#include "bool.hxx"
#include "context.hxx"
#include "gatewaystruct.hxx"

extern "C"
{
#include "machine.h"
#include "core_math.h"
#include "call_scilab.h"
#include "api_scilab.h"
#include "api_internal_common.h"
#include "localization.h"
}

/********************************/
/*   boolean matrix functions   */
/********************************/
SciErr getMatrixOfBoolean(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piBool)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfBoolean");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr || iType != sci_boolean)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), "getMatrixOfBoolean", _("boolean matrix"));
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN, _("%s: Unable to get argument #%d"), "getMatrixOfBoolean", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (_piBool)
    {
        *_piBool = ((types::InternalType*)_piAddress)->getAs<types::Bool>()->get();
    }
    return sciErr;
}

SciErr allocMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piBool)
{
    SciErr sciErr = sciErrInit();

    if (_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "allocMatrixOfBoolean");
        return sciErr;
    }

    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;

    types::Bool *pBool = new types::Bool(_iRows, _iCols);
    if (pBool == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), "allocMatrixOfBoolean");
        return sciErr;
    }

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pBool;
    *_piBool = pBool->get();
    if (*_piBool == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), "allocMatrixOfBoolean");
        delete pBool;
        return sciErr;
    }

    return sciErr;
}

SciErr createMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piBool)
{
    SciErr sciErr = sciErrInit();
    int* piBool		= NULL;

    if (_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        sciErr = createMatrixOfDouble(_pvCtx, _iVar, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
        }
        return sciErr;
    }

    sciErr = allocMatrixOfBoolean(_pvCtx, _iVar, _iRows, _iCols, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfBoolean");
        return sciErr;
    }

    memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);
    return sciErr;
}

SciErr createNamedMatrixOfBoolean(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const int* _piBool)
{
    SciErr sciErr = sciErrInit();

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createNamedMatrixOfBoolean", _pstName);
        return sciErr;
    }

    if (_iRows == 0 && _iCols == 0)
    {
        double dblReal = 0;
        sciErr = createNamedMatrixOfDouble(_pvCtx, _pstName, 0, 0, &dblReal);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createNamedEmptyMatrix");
        }
        return sciErr;
    }

    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createNamedMatrixOfBoolean", _pstName);
        return sciErr;
    }

    types::Bool* pBool = new types::Bool(_iRows, _iCols);
    if (pBool == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_BOOLEAN, _("%s: Unable to create %s named \"%s\""), "createNamedMatrixOfBoolean", _("matrix of boolean"), _pstName);
        return sciErr;
    }

    wchar_t* pwstName = to_wide_string(_pstName);
    pBool->set(_piBool);

    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pBool);
    }
    else
    {
        delete pBool;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }

    return sciErr;
}

SciErr readNamedMatrixOfBoolean(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piBool)
{
    SciErr sciErr = sciErrInit();
    int* piAddr				= NULL;
    int* piBool				= NULL;

    sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfBoolean", _pstName);
        return sciErr;
    }

    sciErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfBoolean", _pstName);
        return sciErr;
    }

    if (_piBool)
    {
        memcpy(_piBool, piBool, sizeof(int) **_piRows **_piCols);
    }

    return sciErr;
}

/*shortcut functions*/

/*--------------------------------------------------------------------------*/
int isBooleanType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_boolean);
}
/*--------------------------------------------------------------------------*/
int isNamedBooleanType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_boolean);
}
/*--------------------------------------------------------------------------*/
int getScalarBoolean(void* _pvCtx, int* _piAddress, int* _piBool)
{
    SciErr sciErr = sciErrInit();
    int iRows	= 0;
    int iCols	= 0;

    int* piBool = NULL;

    sciErr = getMatrixOfBoolean(_pvCtx, _piAddress, &iRows, &iCols, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_BOOLEAN, _("%s: Unable to get argument #%d"), "getScalarBoolean", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (isScalar(_pvCtx, _piAddress) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_BOOLEAN, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "getScalarBoolean", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (_piBool != NULL)
    {
        *_piBool	= piBool[0];
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarBoolean(void* _pvCtx, const char* _pstName, int* _piBool)
{
    SciErr sciErr = sciErrInit();
    int iRows	= 0;
    int iCols	= 0;

    int iBool = 0;

    sciErr = readNamedMatrixOfBoolean(_pvCtx, _pstName, &iRows, &iCols, &iBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_BOOLEAN, _("%s: Unable to get argument \"%s\""), "getNamedScalarBoolean", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (isNamedScalar(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_BOOLEAN, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), "getNamedScalarBoolean", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (_piBool != NULL)
    {
        *_piBool	= iBool;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarBoolean(void* _pvCtx, int _iVar, int _iBool)
{
    SciErr sciErr = sciErrInit();
    int* piBool = NULL;

    sciErr = allocMatrixOfBoolean(_pvCtx, _iVar, 1, 1, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createScalarBoolean");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    piBool[0] = _iBool;
    return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarBoolean(void* _pvCtx, const char* _pstName, int _iBool)
{
    SciErr sciErr = sciErrInit();
    sciErr = createNamedMatrixOfBoolean(_pvCtx, _pstName, 1, 1, &_iBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarBoolean");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
