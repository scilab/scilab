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

#include "sparse.hxx"
#include "context.hxx"
#include "gatewaystruct.hxx"

extern "C"
{
#include <string.h>
#include "sci_malloc.h"
#include "api_scilab.h"
#include "call_scilab.h"
#include "core_math.h"
#include "api_internal_common.h"
#include "api_stack_boolean_sparse.h"
#include "api_internal_boolean_sparse.h"
#include "localization.h"
}

SciErr getBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getBooleanSparseMatrix");
        return sciErr;
    }

    sciErr =  getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr || iType != sci_boolean_sparse)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getBooleanSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getBooleanSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    types::SparseBool* pSpBool = ((types::InternalType*)_piAddress)->getAs<types::SparseBool>();
    *_piNbItem = (int)pSpBool->nbTrue();

    if (_piNbItemRow == NULL)
    {
        return sciErr;
    }

    int* piNbItemRows = (int*)MALLOC(sizeof(int) **_piRows);
    *_piNbItemRow = pSpBool->getNbItemByRow(piNbItemRows);

    if (_piColPos == NULL)
    {
        return sciErr;
    }

    int* piColPos = (int*)MALLOC(sizeof(int) **_piNbItem);
    *_piColPos = pSpBool->getColPos(piColPos);

    return sciErr;
}

SciErr allocBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
    SciErr sciErr = sciErrInit();
    // We cant rewrite this function in Scilab 6
    // because sparses are not stored like scilab 5.
    // We cant return pointer to _piNbItemRow and
    // _piColPos and let user fill it.
    return sciErr;
}

SciErr fillBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    SciErr sciErr = sciErrInit();

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillBooleanSparseMatrix");
        return sciErr;
    }

    types::SparseBool* pSparse = (types::SparseBool*)_piAddress;

    for (int i = 0; i < _iRows; i++)
    {
        for (int j = 0; j < _piNbItemRow[i]; j++)
        {
            int iIndex = (*_piColPos++ - 1) * _iRows + i;
            pSparse->set(iIndex, true);
        }
    }

    return sciErr;
}

SciErr createBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    SciErr sciErr = sciErrInit();

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

    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::InternalType** out = pStr->m_pOut;

    types::SparseBool* pSparse = new types::SparseBool(_iRows, _iCols);
    if (pSparse == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE, _("%s: Unable to create variable in Scilab memory"), "createBooleanSparseMatrix");
        return sciErr;
    }

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pSparse;

    sciErr = fillBooleanSparseMatrix(_pvCtx, (int*)pSparse, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
    return sciErr;
}

SciErr createNamedBooleanSparseMatrix(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    SciErr sciErr = sciErrInit();

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createNamedMatrixOfBoolean", _pstName);
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

    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createNamedBooleanSparseMatrix", _pstName);
        return sciErr;
    }

    types::SparseBool* pSparse = new types::SparseBool(_iRows, _iCols);
    if (pSparse == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_BOOLEAN_SPARSE, _("%s: Unable to create %s named \"%s\""), "createNamedBooleanSparseMatrix", _("boolean sparse matrix"), _pstName);
        return sciErr;
    }

    sciErr = fillBooleanSparseMatrix(_pvCtx, (int*)pSparse, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);

    wchar_t* pwstName = to_wide_string(_pstName);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pSparse);
    }
    else
    {
        delete pSparse;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }
    return sciErr;
}

SciErr readNamedBooleanSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
    SciErr sciErr       = sciErrInit();
    struct Attr
    {
        public:
            int *piAddr;
            int *piNbItemRow;
            int *piColPos;
            Attr() : piAddr(NULL), piNbItemRow(NULL), piColPos(NULL) {}
            ~Attr()
            {
                if(piNbItemRow)
                    FREE(piNbItemRow);
                if(piColPos)
                    FREE(piColPos);
                if(piAddr)
                    FREE(piAddr);
            }
    };
    struct Attr attr;
    sciErr = getVarAddressFromName(_pvCtx, _pstName, &attr.piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_BOOLEAN_SPARSE, _("%s: Unable to get variable \"%s\""), "readNamedBooleanSparseMatrix", _pstName);
        return sciErr;
    }

    sciErr = getBooleanSparseMatrix(_pvCtx, attr.piAddr, _piRows, _piCols, _piNbItem, &attr.piNbItemRow, &attr.piColPos);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_BOOLEAN_SPARSE, _("API_ERROR_READ_NAMED_BOOLEAN_SPARSE"));
        return sciErr;
    }

    if (_piNbItemRow == NULL)
    {
        return sciErr;
    }

    memcpy(_piNbItemRow, attr.piNbItemRow, *_piRows * sizeof(int));

    if (_piColPos == NULL)
    {
        return sciErr;
    }

    memcpy(_piColPos, attr.piColPos, *_piNbItem * sizeof(int));
    return sciErr;
}
/*--------------------------------------------------------------------------*/
int isBooleanSparseType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_boolean_sparse);
}
/*--------------------------------------------------------------------------*/
int isNamedBooleanSparseType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_boolean_sparse);
}
/*--------------------------------------------------------------------------*/
int getAllocatedBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
    SciErr sciErr = sciErrInit();
    int* piNbItemRow	= NULL;
    int* piColPos			= NULL;

    sciErr = getBooleanSparseMatrix(_pvCtx, _piAddress, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getAllocatedBooleanSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        FREE(piNbItemRow);
        FREE(piColPos);
        return sciErr.iErr;
    }

    *_piNbItemRow		= (int*)MALLOC(sizeof(int) **_piRows);
    memcpy(*_piNbItemRow, piNbItemRow, sizeof(int) **_piRows);

    *_piColPos			= (int*)MALLOC(sizeof(int) **_piNbItem);
    memcpy(*_piColPos, piColPos, sizeof(int) **_piNbItem);

    FREE(piNbItemRow);
    FREE(piColPos);
    return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedAllocatedBooleanSparseMatrix(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
    SciErr sciErr = sciErrInit();
    sciErr = readNamedBooleanSparseMatrix(_pvCtx, _pstName, _piRows, _piCols, _piNbItem, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument \"%s\""), "getNamedAllocatedBooleanSparseMatrix", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_piNbItemRow		= (int*)MALLOC(sizeof(int) **_piRows);
    *_piColPos			= (int*)MALLOC(sizeof(int) **_piNbItem);

    sciErr = readNamedBooleanSparseMatrix(_pvCtx, _pstName, _piRows, _piCols, _piNbItem, *_piNbItemRow, *_piColPos);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument \"%s\""), "getNamedAllocatedBooleanSparseMatrix", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllocatedBooleanSparse(int* _piNbItemRow, int* _piColPos)
{
    FREE(_piNbItemRow);
    FREE(_piColPos);
}
