/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
 * Copyright (C) 2015 - Scilab Enterprises - Anais AUBERT
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

/*--------------------------------------------------------------------------*/
#include "gatewaystruct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "context.hxx"

extern "C"
{
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "api_scilab.h"
#include "api_internal_common.h"
#include "call_scilab.h"
#include "localization.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "os_string.h"
}

/*--------------------------------------------------------------------------*/

/*******************************/
/*   string matrix functions   */
/*******************************/

SciErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
    SciErr sciErr = sciErrInit();
    int *piOffset = NULL;
    int *piData	= NULL;
    int iType = 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfString", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (iType != sci_strings)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), "getMatrixOfString", _("string matrix"));
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfString", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (_piLength == NULL)
    {
        return sciErr;
    }

    types::String *pS = ((types::InternalType*)_piAddress)->getAs<types::String>();

    //non cummulative length
    int iSize = pS->getSize();
    if (_pstStrings == NULL || *_pstStrings == NULL)
    {
        for (int i = 0 ; i < iSize; i++)
        {
            char* pstTemp = wide_string_to_UTF8(pS->get(i));
            _piLength[i] = (int)strlen(pstTemp);
            FREE(pstTemp);
        }
    }
    else
    {
        for (int i = 0 ; i < iSize; i++)
        {
            if (_pstStrings[i] == NULL)
            {
                addErrorMessage(&sciErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
                return sciErr;
            }

            char* c = wide_string_to_UTF8(pS->get(i));
            strcpy(_pstStrings[i], c);
            FREE(c);
        }
    }

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const char* const * _pstStrings)
{
    SciErr sciErr = sciErrInit();

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::InternalType** out = pStr->m_pOut;

    //return empty matrix
    if (_iRows == 0 && _iCols == 0)
    {
        types::Double *pDbl = new types::Double(_iRows, _iCols);
        if (pDbl == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }

        out[rhs - 1] = pDbl;
        return sciErr;
    }

    types::String* pS = new types::String(_iRows, _iCols);
    if (pS == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), "createMatrixOfString");
        return sciErr;
    }

    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t* pstTemp = to_wide_string(_pstStrings[i]);
        pS->set(i, pstTemp);
        FREE(pstTemp);
    }

    out[rhs - 1] = pS;

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createNamedMatrixOfString(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const char* const* _pstStrings)
{
    SciErr sciErr = sciErrInit();

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createNamedMatrixOfString", _pstName);
        return sciErr;
    }

    //return empty matrix
    if (_iRows == 0 && _iCols == 0)
    {
        if (createNamedEmptyMatrix(_pvCtx, _pstName))
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }

        return sciErr;
    }

    types::String* pS = new types::String(_iRows, _iCols);
    if (pS == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createNamedMatrixOfString", _pstName);
        return sciErr;
    }

    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        wchar_t* pstTemp = to_wide_string(_pstStrings[i]);
        pS->set(i, pstTemp);
        FREE(pstTemp);
    }

    wchar_t* pwstName = to_wide_string(_pstName);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pS);
    }
    else
    {
        delete pS;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr readNamedMatrixOfString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
    int* piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfString", _pstName);
        return sciErr;
    }

    sciErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfString", _pstName);
        return sciErr;
    }

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;
    char **pstStrings = NULL;
    int *piLenStrings = NULL;
    int strSize = 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfWideString");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (iType != sci_strings)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), "getMatrixOfWideString", _("string matrix"));
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (_piwLength == NULL)
    {
        return sciErr;
    }

    types::String *pS = ((types::InternalType*)_piAddress)->getAs<types::String>();

    int iSize = pS->getSize();
    if (_pwstStrings == NULL || *_pwstStrings == NULL)
    {
        for (int i = 0 ; i < iSize; i++)
        {
            _piwLength[i] = (int)wcslen(pS->get(i));
        }
    }
    else
    {
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            if (_pwstStrings[i] == NULL)
            {
                addErrorMessage(&sciErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
                return sciErr;
            }

            wcscpy(_pwstStrings[i], pS->get(i));
        }
    }

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, const wchar_t* const* _pstwStrings)
{
    char **pStrings = NULL;

    //return empty matrix
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

    pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );

    for (int i = 0; i < (_iRows * _iCols) ; i++)
    {
        pStrings[i] = wide_string_to_UTF8(_pstwStrings[i]);
    }

    SciErr sciErr = createMatrixOfString(_pvCtx, _iVar, _iRows, _iCols, pStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_WIDE_STRING, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfWideString");
    }

    freeArrayOfString(pStrings, _iRows * _iCols);

    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createNamedMatrixOfWideString(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const wchar_t* const* _pwstStrings)
{
    SciErr sciErr = sciErrInit();
    char **pStrings = NULL;

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createNamedMatrixOfWideString", _pstName);
        return sciErr;
    }

    //return named empty matrix
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

    types::String* pS = new types::String(_iRows, _iCols);
    if (pS == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createNamedMatrixOfWideString", _pstName);
        return sciErr;
    }

    for (int i = 0 ; i < pS->getSize() ; i++)
    {
        pS->set(i, _pwstStrings[i]);
    }

    wchar_t* pwstName = to_wide_string(_pstName);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pS);
    }
    else
    {
        delete pS;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }
    return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr readNamedMatrixOfWideString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
    int* piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_WIDE_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfWideString", _pstName);
        return sciErr;
    }

    sciErr = getMatrixOfWideString(_pvCtx, piAddr, _piRows, _piCols, _piwLength, _pwstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_WIDE_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfWideString", _pstName);
        return sciErr;
    }

    return sciErr;
}

/*shortcut functions*/
/*--------------------------------------------------------------------------*/
int isStringType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_strings);
}
/*--------------------------------------------------------------------------*/
int isNamedStringType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_strings);
}
/*--------------------------------------------------------------------------*/
int getAllocatedSingleString(void* _pvCtx, int* _piAddress, char** _pstData)
{
    SciErr sciErr = sciErrInit();
    int iRows = 0;
    int iCols = 0;
    int iLen = 0;

    if (isScalar(_pvCtx, _piAddress) == 0 || isStringType(_pvCtx, _piAddress) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_STRING, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "getAllocatedSingleString", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = getMatrixOfString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pstData = (char*)MALLOC(sizeof(char) * (iLen + 1)); //+1 for null termination

    sciErr = getMatrixOfString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, _pstData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleString");
        printError(&sciErr, 0);
        FREE(*_pstData);
        return sciErr.iErr;
    }

    return 0;
}

/*--------------------------------------------------------------------------*/
int getAllocatedSingleWideString(void* _pvCtx, int* _piAddress, wchar_t** _pwstData)
{
    SciErr sciErr = sciErrInit();
    int iRows = 0;
    int iCols = 0;
    int iLen = 0;

    if (isScalar(_pvCtx, _piAddress) == 0 || isStringType(_pvCtx, _piAddress) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_WIDE_STRING, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "getAllocatedSingleWideString", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = getMatrixOfWideString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleWideString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pwstData = (wchar_t*)MALLOC(sizeof(wchar_t) * (iLen + 1)); //+1 for null termination

    sciErr = getMatrixOfWideString(_pvCtx, _piAddress, &iRows, &iCols, &iLen, _pwstData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "getAllocatedSingleWideString");
        printError(&sciErr, 0);
        FREE(*_pwstData);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, char*** _pstData)
{
    SciErr sciErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    int* piLen = (int*)MALLOC(sizeof(int) **_piRows **_piCols);

    sciErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfString");
        printError(&sciErr, 0);
        if (piLen)
        {
            FREE(piLen);
            piLen = NULL;
        }
        return sciErr.iErr;
    }

    *_pstData = (char**)MALLOC(sizeof(char*) **_piRows **_piCols);
    for (int i = 0 ; i < *_piRows **_piCols ; i++)
    {
        (*_pstData)[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+1 for null termination
    }

    sciErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLen, *_pstData);
    if (piLen)
    {
        FREE(piLen);
        piLen = NULL;
    }
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfString");
        printError(&sciErr, 0);
        for (int i = 0 ; i < *_piRows **_piCols ; i++)
        {
            FREE((*_pstData)[i]);
        }
        FREE(*_pstData);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, wchar_t*** _pwstData)
{
    SciErr sciErr = getMatrixOfWideString(_pvCtx, _piAddress, _piRows, _piCols, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfWideString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    int* piLen = (int*)MALLOC(sizeof(int) **_piRows **_piCols);

    sciErr = getMatrixOfWideString(_pvCtx, _piAddress, _piRows, _piCols, piLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfWideString");
        if (piLen)
        {
            FREE(piLen);
            piLen = NULL;
        }
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pwstData = (wchar_t**)MALLOC(sizeof(wchar_t*) **_piRows **_piCols);
    for (int i = 0 ; i < *_piRows **_piCols ; i++)
    {
        (*_pwstData)[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (piLen[i] + 1));//+1 for null termination
    }

    sciErr = getMatrixOfWideString(_pvCtx, _piAddress, _piRows, _piCols, piLen, *_pwstData);

    if (piLen)
    {
        FREE(piLen);
        piLen = NULL;
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedMatrixOfWideString");
        printError(&sciErr, 0);
        for (int i = 0 ; i < *_piRows **_piCols ; i++)
        {
            FREE((*_pwstData)[i]);
        }
        FREE(*_pwstData);
        return sciErr.iErr;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedSingleString(void* _pvCtx, const char* _pstName, char** _pstData)
{
    SciErr sciErr = sciErrInit();
    int iRows = 0;
    int iCols = 0;
    int iLen = 0;

    if (isNamedScalar(_pvCtx, _pstName) == 0 || isNamedStringType(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_STRING, _("%s: Wrong type for input argument \"%s\": A single string expected.\n"), "getAllocatedNamedSingleString", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = readNamedMatrixOfString(_pvCtx, _pstName, &iRows, &iCols, &iLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_STRING, _("%s: Unable to get argument data"), "getAllocatedNamedSingleString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pstData = (char*)MALLOC(sizeof(char) * (iLen + 1)); //+1 for null termination

    sciErr = readNamedMatrixOfString(_pvCtx, _pstName, &iRows, &iCols, &iLen, _pstData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_STRING, _("%s: Unable to get argument data"), "getAllocatedNamedSingleString");
        printError(&sciErr, 0);
        FREE(*_pstData);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedSingleWideString(void* _pvCtx, const char* _pstName, wchar_t** _pwstData)
{
    SciErr sciErr = sciErrInit();
    int iRows = 0;
    int iCols = 0;
    int iLen = 0;

    if (isNamedScalar(_pvCtx, _pstName) == 0 || isNamedStringType(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_WIDE_STRING, _("%s: Wrong type for input argument \"%s\": A single string expected.\n"), "getAllocatedNamedSingleWideString", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = readNamedMatrixOfWideString(_pvCtx, _pstName, &iRows, &iCols, &iLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "getAllocatedNamedSingleWideString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pwstData = (wchar_t*)MALLOC(sizeof(wchar_t) * (iLen + 1)); //+1 for null termination

    sciErr = readNamedMatrixOfWideString(_pvCtx, _pstName, &iRows, &iCols, &iLen, _pwstData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "getAllocatedNamedSingleWideString");
        printError(&sciErr, 0);
        FREE(*_pwstData);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedMatrixOfString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, char*** _pstData)
{
    SciErr sciErr = readNamedMatrixOfString(_pvCtx, _pstName, _piRows, _piCols, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedNamedMatrixOfString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    int* piLen = (int*)MALLOC(sizeof(int) **_piRows **_piCols);

    sciErr = readNamedMatrixOfString(_pvCtx, _pstName, _piRows, _piCols, piLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedNamedMatrixOfString");
        if (piLen)
        {
            FREE(piLen);
            piLen = NULL;
        }
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pstData = (char**)MALLOC(sizeof(char*) **_piRows **_piCols);
    for (int i = 0 ; i < *_piRows **_piCols ; i++)
    {
        (*_pstData)[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));//+1 for null termination
    }

    sciErr = readNamedMatrixOfString(_pvCtx, _pstName, _piRows, _piCols, piLen, *_pstData);
    if (piLen)
    {
        FREE(piLen);
        piLen = NULL;
    }
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedNamedMatrixOfString");
        printError(&sciErr, 0);
        for (int i = 0 ; i < *_piRows **_piCols ; i++)
        {
            FREE((*_pstData)[i]);
        }
        FREE(*_pstData);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedMatrixOfWideString(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, wchar_t*** _pwstData)
{
    int iRows = 0;
    int iCols = 0;
    int* piLen = 0;

    SciErr sciErr = readNamedMatrixOfWideString(_pvCtx, _pstName, &iRows, &iCols, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedNamedMatrixOfWideString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    piLen = (int*)MALLOC(sizeof(int) **_piRows **_piCols);

    sciErr = readNamedMatrixOfWideString(_pvCtx, _pstName, &iRows, &iCols, piLen, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedNamedMatrixOfWideString");
        if (piLen)
        {
            FREE(piLen);
            piLen = NULL;
        }
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pwstData = (wchar_t**)MALLOC(sizeof(wchar_t*) **_piRows **_piCols);

    for (int i = 0 ; i < *_piRows **_piCols ; i++)
    {
        *_pwstData[i] = (wchar_t*)MALLOC(sizeof(wchar_t) * (piLen[i] + 1));//+1 for null termination
    }

    sciErr = readNamedMatrixOfWideString(_pvCtx, _pstName, &iRows, &iCols, piLen, *_pwstData);
    if (piLen)
    {
        FREE(piLen);
        piLen = NULL;
    }
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_WIDE_STRING_MATRIX, _("%s: Unable to get argument data"), "getAllocatedNamedMatrixOfWideString");
        printError(&sciErr, 0);
        for (int i = 0 ; i < *_piRows **_piCols ; i++)
        {
            FREE((*_pwstData)[i]);
        }
        FREE(*_pwstData);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createSingleString(void* _pvCtx, int _iVar, const char* _pstStrings)
{
    SciErr sciErr = createMatrixOfString(_pvCtx, _iVar, 1, 1,	&_pstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SINGLE_STRING, _("%s: Unable to get argument data"), "createSingleString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}

int allocSingleString(void* _pvCtx, int _iVar, int _iLen, const char** _pstStrings)
{
    SciErr sciErr = sciErrInit();

    types::GatewayStruct* pGstr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pGstr->m_pIn;
    types::InternalType** out = pGstr->m_pOut;
    types::String *pStr = NULL;

    char* pstStrings;

    if (_pstStrings == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "allocSingleString");
        return sciErr.iErr;
    }

    pstStrings = new char[_iLen];
    memset(pstStrings, ' ', _iLen);
    _pstStrings[0] = pstStrings;

    pStr = new types::String(pstStrings);

    if (pStr == NULL)
    {
        delete[] pstStrings;
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "allocSingleString");
        return sciErr.iErr;
    }

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pStr;


    return sciErr.iErr;
}

/*--------------------------------------------------------------------------*/
int createSingleWideString(void* _pvCtx, int _iVar, const wchar_t* _pwstStrings)
{
    SciErr sciErr = createMatrixOfWideString(_pvCtx, _iVar, 1, 1,	&_pwstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "createSingleWideString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedSingleString(void* _pvCtx, const char* _pstName, const char* _pstStrings)
{
    SciErr sciErr = createNamedMatrixOfString(_pvCtx, _pstName, 1, 1,	&_pstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SINGLE_STRING, _("%s: Unable to get argument data"), "createSingleString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedSingleWideString(void* _pvCtx, const char* _pstName, const wchar_t* _pwstStrings)
{
    SciErr sciErr = createNamedMatrixOfWideString(_pvCtx, _pstName, 1, 1,	&_pwstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SINGLE_WIDE_STRING, _("%s: Unable to get argument data"), "createSingleWideString");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSingleString(char* _pstData)
{
    FREE(_pstData);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSingleWideString(wchar_t* _pwstData)
{
    FREE(_pwstData);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedMatrixOfString(int _iRows, int _iCols, char** _pstData)
{
    for (int i = 0 ; i < _iRows * _iCols ; i++)
    {
        FREE(_pstData[i]);
    }
    FREE(_pstData);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedMatrixOfWideString(int _iRows, int _iCols, wchar_t** _pwstData)
{
    for (int i = 0 ; i < _iRows * _iCols ; i++)
    {
        FREE(_pwstData[i]);
    }
    FREE(_pwstData);
}
