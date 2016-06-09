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

#include <vector>
#include <map>
#include <string>
#include "list.hxx"
#include "listundefined.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "pointer.hxx"
#include "polynom.hxx"
#include "gatewaystruct.hxx"
#include "double.hxx"
#include "string.hxx"
#include "int.hxx"
#include "context.hxx"
#include "void.hxx"
#include "sparse.hxx"

extern "C"
{
#include <stdio.h>
#include <string.h>
#include "call_scilab.h"
#include "doublecomplex.h"
#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_double.h"
#include "api_internal_poly.h"
#include "api_internal_int.h"
#include "api_internal_sparse.h"
#include "api_internal_boolean_sparse.h"
#include "api_internal_pointer.h"
#include "localization.h"
}

//internal functions
static SciErr checkListItemPosition(void* _pvCtx, int* _piParent, int _iItemPos, int _apiError, const char* _functionName);
static SciErr createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress);
static SciErr createCommonListInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress);
static SciErr createCommonNamedList(void* _pvCtx, const char* _pstName, int _iListType, int _iNbItem, int** _piAddress);
static SciErr getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static SciErr getCommomListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static SciErr readCommonNamedList(void* _pvCtx, const char* _pstName, int _iListType, int* _piNbItem, int** _piAddress);
static SciErr allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
static SciErr getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
static SciErr createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);
static SciErr createCommonMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg);
static SciErr readCommonMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);
static SciErr getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static SciErr createCommonMatrixOfPolyInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg);
static SciErr readCommonMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static SciErr createCommonMatrixOfStringInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings);
static SciErr createCommonPointerInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr);
static SciErr readCommonSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static SciErr createCommonBooleanSparseMatrixInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos);
static SciErr createCommonSparseMatrixInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg);

const char* getListTypeName(int _iType)
{
    switch (_iType)
    {
        case sci_list :
            return "list";
            break;
        case sci_tlist :
            return "tlist";
            break;
        case sci_mlist :
            return "mlist";
            break;
        default:
            break;
    }
    return "";
}

SciErr getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem)
{
    int iType = 0;

    SciErr sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_LIST_ITEM_NUMBER, _("%s: Unable to get item number of list"), "getListItemNumber");
        return sciErr;
    }

    types::List* pL = (types::List*)_piAddress;
    switch (iType)
    {
        case sci_list :
        case sci_mlist :
        case sci_tlist :
            *_piNbItem = pL->getSize();
            break;
        default :
            addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "getListItemNumber", _("list"));
            return sciErr;
    }
    return sciErr;
}

SciErr getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress)
{
    int iItem           = 0;
    int* piOffset       = NULL;
    int* piItemAddress  = NULL;

    SciErr sciErr = sciErrInit();

    sciErr = checkListItemPosition(_pvCtx, _piAddress, _iItemNum, API_ERROR_GET_ITEM_ADDRESS, "getListItemAddress");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pL = (types::List*)_piAddress;
    //get offset of item array
    types::InternalType* pIT = pL->get(_iItemNum - 1);
    if (pIT->isListUndefined())
    {
        *_piItemAddress = NULL;
    }
    else
    {
        *_piItemAddress = (int*)pIT;
    }
    return sciErr;
}

SciErr checkListItemPosition(void* _pvCtx, int* _piParent, int _iItemPos, int _apiError, const char* _functionName)
{
    SciErr sciErr = sciErrInit();
    int iItem = 0;

    // get a number of items
    sciErr = getListItemNumber(_pvCtx, _piParent, &iItem);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to get address of item #%d in argument #%d"), _functionName, _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }
    // check a valid item
    if (_iItemPos > iItem)
    {
        addErrorMessage(&sciErr, _apiError, _("%s: Unable to create list item #%d in Scilab memory"), _functionName, _iItemPos + 1);
        return sciErr;
    }
    return sciErr;
}

SciErr createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
    return createCommonList(_pvCtx, _iVar, sci_list, _iNbItem, _piAddress);
}

SciErr createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
    return createCommonList(_pvCtx, _iVar, sci_mlist, _iNbItem, _piAddress);
}

SciErr createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
    return createCommonList(_pvCtx, _iVar, sci_tlist, _iNbItem, _piAddress);
}

SciErr createNamedList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress)
{
    return createCommonNamedList(_pvCtx, _pstName, sci_list, _iNbItem, _piAddress);
}

SciErr createNamedTList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress)
{
    return createCommonNamedList(_pvCtx, _pstName, sci_tlist, _iNbItem, _piAddress);
}

SciErr createNamedMList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress)
{
    return createCommonNamedList(_pvCtx, _pstName, sci_mlist, _iNbItem, _piAddress);
}

SciErr createNamedList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress, bool useStack)
{

    return createCommonNamedList(_pvCtx, _pstName, sci_list, _iNbItem, _piAddress);
}

SciErr createNamedTList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress, bool useStack)
{
    return createCommonNamedList(_pvCtx, _pstName, sci_tlist, _iNbItem, _piAddress);
}

SciErr createNamedMList(void* _pvCtx, const char* _pstName, int _iNbItem, int** _piAddress, bool useStack)
{
    return createCommonNamedList(_pvCtx, _pstName, sci_mlist, _iNbItem, _piAddress);
}

static SciErr createCommonNamedList(void* _pvCtx, const char* _pstName, int _iListType, int _iNbItem, int** _piAddress)
{
    SciErr sciErr = sciErrInit();

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createCommonNamedList", _pstName);
        return sciErr;
    }

    types::List* pL = NULL;
    try
    {
        if (_iListType == sci_list)
        {
            pL = new types::List();
        }
        else if (_iListType == sci_mlist)
        {
            pL = new types::MList();
        }
        else if (_iListType == sci_tlist)
        {
            pL = new types::TList();
        }
        else
        {
            addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "createNamedList", _("list"));
            return sciErr;
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "createNamedList", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pL == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createNamedList");
        return sciErr;
    }

    *_piAddress = (int*)pL;
    // create a list with the user number of items
    // it helps to check a wrong item, for example in createCommonListInList
    pL->set(_iNbItem - 1, new types::ListUndefined());

    wchar_t* pwstName = to_wide_string(_pstName);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pL);
    }
    else
    {
        delete pL;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }
    return sciErr;
}

static SciErr createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress)
{
    SciErr sciErr = sciErrInit();
    if (_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "createList");
        return sciErr;
    }

    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::InternalType** out = pStr->m_pOut;

    types::List* pL = NULL;
    try
    {
        if (_iListType == sci_list)
        {
            pL = new types::List();
        }
        else if (_iListType == sci_mlist)
        {
            pL = new types::MList();
        }
        else if (_iListType == sci_tlist)
        {
            pL = new types::TList();
        }
        else
        {
            addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "createList", _("list"));
            return sciErr;
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "createList", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pL == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createList");
        return sciErr;
    }

    // create a list with the user number of items
    // it helps to check a wrong item, for example in createCommonListInList
    pL->set(_iNbItem - 1, new types::ListUndefined());

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pL;
    *_piAddress = (int*)pL;
    return sciErr;
}

SciErr readNamedList(void* _pvCtx, const char* _pstName, int* _piNbItem, int** _piAddress)
{
    return readCommonNamedList(_pvCtx, _pstName, sci_list, _piNbItem, _piAddress);
}

SciErr readNamedTList(void* _pvCtx, const char* _pstName, int* _piNbItem, int** _piAddress)
{
    return readCommonNamedList(_pvCtx, _pstName, sci_tlist, _piNbItem, _piAddress);
}

SciErr readNamedMList(void* _pvCtx, const char* _pstName, int* _piNbItem, int** _piAddress)
{
    return readCommonNamedList(_pvCtx, _pstName, sci_mlist, _piNbItem, _piAddress);
}

static SciErr readCommonNamedList(void* _pvCtx, const char* _pstName, int _iListType, int* _piNbItem, int** _piAddress)
{
    int* piAddr  = NULL;
    int iNbItem  = 0;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_LIST, _("%s: Unable to get variable \"%s\""), "readNamedList", _pstName);
        return sciErr;
    }

    if (piAddr[0] != _iListType)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "readNamedList", getListTypeName(_iListType));
        return sciErr;
    }

    sciErr = getListItemNumber(_pvCtx, piAddr, &iNbItem);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_LIST, _("%s: Unable to get variable \"%s\""), "readNamedList", _pstName);
        return sciErr;
    }

    *_piNbItem = iNbItem;
    *_piAddress = piAddr;

    return sciErr;
}

SciErr getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
    return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_list, _piAddress);
}

SciErr getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
    return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_tlist, _piAddress);
}

SciErr getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
    return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_mlist, _piAddress);
}

SciErr getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, _piAddress);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_LIST_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getListInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    if ((*_piAddress)[0] != _iListType)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "getListInList", getListTypeName(_iListType));
        return sciErr;
    }
    return sciErr;
}

SciErr getListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
    return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_list, _piAddress);
}

SciErr getTListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
    return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_tlist, _piAddress);
}

SciErr getMListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
    return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_mlist, _piAddress);
}

SciErr getCommomListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
    SciErr sciErr = sciErrInit();
    int* piAddr  = NULL;

    sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_LIST_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "getListInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (piAddr[0] != _iListType)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), "getListInNamedList", getListTypeName(_iListType));
        return sciErr;
    }

    *_piAddress = piAddr;
    return sciErr;
}

SciErr createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return createCommonListInList(_pvCtx, NULL, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress);
}

SciErr createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return createCommonListInList(_pvCtx, NULL, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress);
}

SciErr createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return createCommonListInList(_pvCtx, NULL, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress);
}

SciErr createListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return createCommonListInList(_pvCtx, _pstName, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress);
}

SciErr createTListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return createCommonListInList(_pvCtx, _pstName, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress);
}

SciErr createMListInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
    return createCommonListInList(_pvCtx, _pstName, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress);
}

static SciErr createCommonListInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress)
{
    SciErr sciErr        = sciErrInit();
    const char* funcName = NULL;
    int apiError         = API_ERROR_CREATE_LIST_IN_LIST;

    if (_piParent != NULL)
    {
        funcName = "createListInList";
    }
    else
    {
        funcName = "createListInNamedList";
        apiError = API_ERROR_CREATE_LIST_IN_NAMED_LIST;
    }

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, apiError, funcName);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pChild = NULL;
    try
    {
        if (_iListType == sci_list)
        {
            pChild = new types::List();
        }
        else if (_iListType == sci_mlist)
        {
            pChild = new types::MList();
        }
        else if (_iListType == sci_tlist)
        {
            pChild = new types::TList();
        }
        else
        {
            addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s expected"), funcName, _("list"));
            return sciErr;
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), funcName, ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pChild == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), funcName);
        return sciErr;
    }
    // create a list in the list with a user number of items
    pChild->set(_iNbItem - 1, new types::ListUndefined());

    types::List* pParent = (types::List*)_piParent;
    pParent->set(_iItemPos - 1, pChild);
    *_piAddress = (int*)pChild;
    return sciErr;
}

/******************************
* Void and defined functions *
******************************/
SciErr createVoidInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos)
{
    SciErr sciErr   = sciErrInit();

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, API_ERROR_CREATE_VOID_IN_LIST, "createVoidInNamedList");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pL = (types::List*)_piParent;
    if (pL == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createVoidInNamedList");
        return sciErr;
    }

    pL->set(_iItemPos - 1, new types::Void());
    return sciErr;
}

SciErr createUndefinedInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos)
{
    SciErr sciErr   = sciErrInit();

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, API_ERROR_CREATE_VOID_IN_LIST, "createUndefinedInNamedList");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pL = (types::List*)_piParent;
    if (pL == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createUndefinedInNamedList");
        return sciErr;
    }

    pL->set(_iItemPos - 1, new types::ListUndefined());
    return sciErr;
}

/*********************
* Double functions *
*********************/

SciErr getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal)
{
    return getCommonMatrixOfDoubleInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    return getCommonMatrixOfDoubleInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static SciErr getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "getMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getCommonMatrixOfDouble(_pvCtx, piAddr, '$', _iComplex, _piRows, _piCols, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "getMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }
    return sciErr;
}

SciErr allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal)
{
    return allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    return allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

static SciErr allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr   = sciErrInit();
    int iItem       = 0;

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, API_ERROR_ALLOC_DOUBLE_IN_LIST, _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pParent = (types::List*)_piParent;
    if (_iRows == 0 || _iCols == 0)
    {
        types::Double *pDbl = types::Double::Empty();
        if (pDbl == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }
        pParent->set(_iItemPos - 1, pDbl);
        return sciErr;
    }

    types::Double* pDbl = NULL;
    try
    {
        pDbl = new types::Double(_iRows, _iCols, _iComplex == 1);
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pDbl == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList");
        return sciErr;
    }

    *_pdblReal = pDbl->getReal();
    if (*_pdblReal == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList");
        return sciErr;
    }

    if (_iComplex && _pdblImg != NULL)
    {
        *_pdblImg = pDbl->getImg();
        if (*_pdblImg == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList");
            return sciErr;
        }
    }

    pParent->set(_iItemPos - 1, pDbl);
    return sciErr;
}

SciErr createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal)
{
    return createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const doublecomplex* _pdblData)
{
    SciErr sciErr       = sciErrInit();
    double *pdblReal    = NULL;
    double *pdblImg     = NULL;

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, API_ERROR_CREATE_ZDOUBLE_IN_LIST, "createComplexZMatrixOfDoubleInList");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    sciErr = allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_ZDOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createComplexZMatrixOfDoubleInList", _iItemPos + 1);
        return sciErr;
    }

    vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

    return sciErr;
}

SciErr createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    double *pdblReal = NULL;
    double *pdblImg  = NULL;

    SciErr sciErr = allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
        return sciErr;
    }

    if (_pdblReal != NULL)
    {
        memcpy(pdblReal, _pdblReal, _iRows * _iCols * sizeof(double));
    }

    if (_iComplex && _pdblImg != NULL)
    {
        memcpy(pdblImg, _pdblImg, _iRows * _iCols * sizeof(double));
    }

    return sciErr;
}

SciErr createMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal)
{
    return createCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr createComplexMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr createComplexZMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const doublecomplex* _pdblData)
{
    SciErr sciErr       = sciErrInit();
    int iVar            = 0;
    double *pdblReal    = NULL;
    double *pdblImg     = NULL;

    sciErr = allocCommonMatrixOfDoubleInList(_pvCtx, iVar, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_ZDOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createComplexZMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

    return sciErr;
}

SciErr createCommonMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    SciErr sciErr    = sciErrInit();
    double* pdblReal = NULL;
    double* pdblImg  = NULL;
    int iVar         = 0;

    sciErr = allocCommonMatrixOfDoubleInList(_pvCtx, iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInNamedList" : "createMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_pdblReal != NULL)
    {
        memcpy(pdblReal, _pdblReal, _iRows * _iCols * sizeof(double));
    }

    if (_iComplex && _pdblImg != NULL)
    {
        memcpy(pdblImg, _pdblImg, _iRows * _iCols * sizeof(double));
    }

    return sciErr;
}

SciErr readMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal)
{
    return readCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr readComplexMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
    return readCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static SciErr readCommonMatrixOfDoubleInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iNbItem    = 0;
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    double* pdblReal = NULL;
    double* pdblImg  = NULL;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getCommonMatrixOfDouble(_pvCtx, piAddr, '$', _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
    {
        return sciErr;
    }

    memcpy(_pdblReal, pdblReal, sizeof(double) **_piRows **_piCols);
    if (_iComplex)
    {
        memcpy(_pdblImg, pdblImg, sizeof(double) **_piRows **_piCols);
    }
    return sciErr;
}


/*********************
* Strings functions *
*********************/

SciErr getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfStringInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfStringInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr createMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings)
{
    return createCommonMatrixOfStringInList(_pvCtx, NULL, _piParent, _iItemPos, _iRows, _iCols, _pstStrings);
}

SciErr createMatrixOfStringInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings)
{
    return createCommonMatrixOfStringInList(_pvCtx, _pstName, _piParent, _iItemPos, _iRows, _iCols, _pstStrings);
}

SciErr createCommonMatrixOfStringInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* const* _pstStrings)
{
    SciErr sciErr = sciErrInit();
    const char* funcName = NULL;
    int apiError         = API_ERROR_CREATE_STRING_IN_LIST;

    if (_pstName == NULL)
    {
        funcName = "createMatrixOfStringInList";
    }
    else
    {
        funcName = "createMatrixOfStringInNamedList";
        apiError = API_ERROR_CREATE_STRING_IN_NAMED_LIST;
    }

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, apiError, funcName);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pParent = (types::List*)_piParent;
    if (_iRows == 0 || _iCols == 0)
    {
        types::Double *pDbl = types::Double::Empty();
        if (pDbl == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }
        pParent->set(_iItemPos - 1, pDbl);
        return sciErr;
    }

    types::String* pS = NULL;
    try
    {
        pS = new types::String(_iRows, _iCols);
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), funcName, ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pS == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), funcName);
        return sciErr;
    }

    for (int i = 0 ; i < _iRows * _iCols ; i++)
    {
        wchar_t* pwst = to_wide_string(_pstStrings[i]);
        pS->set(i, pwst);
        FREE(pwst);
    }

    pParent->set(_iItemPos - 1, pS);
    return sciErr;
}

SciErr readMatrixOfStringInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
    SciErr sciErr = sciErrInit();
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    int iNbItem    = 0;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    return sciErr;
}

/*********************
* boolean functions *
*********************/

SciErr getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfBooleanInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, _piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfBooleanInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piBool)
{
    SciErr sciErr   = sciErrInit();
    int *piBool     = NULL;
    int iVar        = 0;

    sciErr = allocMatrixOfBooleanInList(_pvCtx, iVar, _piParent, _iItemPos, _iRows, _iCols, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
        return sciErr;
    }

    if (_piBool != NULL)
    {
        memcpy(piBool, _piBool, _iRows * _iCols * sizeof(int));
    }

    return sciErr;
}

SciErr allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
    SciErr sciErr   = sciErrInit();

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, API_ERROR_ALLOC_BOOLEAN_IN_LIST, "allocMatrixOfBooleanInList");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pParent = (types::List*)_piParent;
    if (_iRows == 0 || _iCols == 0)
    {
        types::Double *pDbl = types::Double::Empty();
        if (pDbl == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }
        pParent->set(_iItemPos - 1, pDbl);
        return sciErr;
    }

    types::Bool* pBool = NULL;
    try
    {
        pBool = new types::Bool(_iRows, _iCols);
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "allocMatrixOfBooleanInList", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pBool == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "allocMatrixOfBooleanInList");
        return sciErr;
    }

    *_piBool = pBool->get();
    if (*_piBool == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "allocMatrixOfBooleanInList");
        return sciErr;
    }

    pParent->set(_iItemPos - 1, pBool);
    return sciErr;
}

SciErr createMatrixOfBooleanInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piBool)
{
    SciErr sciErr   = sciErrInit();
    int* piBool     = NULL;
    int iVar        = 0;

    sciErr = allocMatrixOfBooleanInList(_pvCtx, iVar, _piParent, _iItemPos, _iRows, _iCols, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_piBool != NULL)
    {
        memcpy(piBool, _piBool, _iRows * _iCols * sizeof(int));
    }

    return sciErr;
}

SciErr readMatrixOfBooleanInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool)
{
    SciErr sciErr = sciErrInit();
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    int iNbItem    = 0;
    int* piBool    = NULL;


    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, &piBool);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_piBool == NULL)
    {
        return sciErr;
    }

    memcpy(_piBool, piBool, *_piRows **_piCols * sizeof(int));
    return sciErr;
}


/*************************
* polynomials functions *
*************************/

SciErr getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
    return getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POLY_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfPolyInList" : "getMatrixOfPolyInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POLY_IN_LIST, _("API_ERROR_GET_POLY_IN_LIST"));
        return sciErr;
    }

    return sciErr;
}

SciErr createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal)
{
    return createCommonMatrixOfPolyInList(_pvCtx, NULL, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    return createCommonMatrixOfPolyInList(_pvCtx, NULL, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal)
{
    return createCommonMatrixOfPolyInList(_pvCtx, _pstName, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createComplexMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    return createCommonMatrixOfPolyInList(_pvCtx, _pstName, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonMatrixOfPolyInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    SciErr sciErr        = sciErrInit();
    const char* funcName = NULL;
    int apiError         = API_ERROR_CREATE_POLY_IN_LIST;

    if (_pstName == NULL)
    {
        funcName = _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList";
    }
    else
    {
        funcName = _iComplex ? "createComplexMatrixOfPolyInNamedList" : "createMatrixOfPolyInNamedList";
        apiError = API_ERROR_CREATE_POLY_IN_NAMED_LIST;
    }

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, apiError, funcName);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pParent = (types::List*)_piParent;
    //return empty matrix
    if (_iRows == 0 || _iCols == 0)
    {
        types::Double *pDbl = types::Double::Empty();
        if (pDbl == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }
        pParent->set(_iItemPos - 1, pDbl);
        return sciErr;
    }

    wchar_t* pstTemp = to_wide_string(_pstVarName);
    std::wstring wstTemp(pstTemp);
    types::Polynom* pP = new types::Polynom(wstTemp, _iRows, _iCols, _piNbCoef);
    FREE(pstTemp);
    if (pP == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), funcName);
        return sciErr;
    }

    if (_iComplex)
    {
        pP->setComplex(true);
    }

    for (int i = 0; i < pP->getSize(); i++)
    {
        types::Double* pD = new types::Double(_piNbCoef[i], 1, _iComplex == 1);
        pD->set(_pdblReal[i]);
        if (_iComplex)
        {
            pD->setImg(_pdblImg[i]);
        }
        pP->setCoef(i, pD);
        delete pD;
    }

    pParent->set(_iItemPos - 1, pP);
    return sciErr;
}

SciErr readMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
    return readCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr readComplexMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return readCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr readCommonMatrixOfPolyInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    int iNbItem    = 0;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    return sciErr;
}

/**********************
* integers functions *
**********************/

static SciErr allocCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
    SciErr sciErr = sciErrInit();

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, API_ERROR_ALLOC_INT_IN_LIST, "allocMatrixOfIntegerInList");
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::List* pParent = (types::List*)_piParent;
    if (_iRows == 0 || _iCols == 0)
    {
        types::Double *pDbl = types::Double::Empty();
        if (pDbl == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createEmptyMatrix");
            return sciErr;
        }
        pParent->set(_iItemPos - 1, pDbl);
        return sciErr;
    }

    types::InternalType* pIT = NULL;
    try
    {
        switch (_iPrecision)
        {
            case SCI_INT8 :
            {
                types::Int8* pi = new types::Int8(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_UINT8 :
            {
                types::UInt8* pi = new types::UInt8(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_INT16 :
            {
                types::Int16* pi = new types::Int16(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_UINT16 :
            {
                types::UInt16* pi = new types::UInt16(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_INT32 :
            {
                types::Int32* pi = new types::Int32(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_UINT32 :
            {
                types::UInt32* pi = new types::UInt32(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_INT64 :
            {
                types::Int64* pi = new types::Int64(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
            case SCI_UINT64 :
            {
                types::UInt64* pi = new types::UInt64(_iRows, _iCols);
                *_pvData = pi->get();
                pIT = pi;
            }
            break;
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "allocMatrixOfIntegerInList", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pIT == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "allocMatrixOfIntegerInList");
        return sciErr;
    }

    pParent->set(_iItemPos - 1, pIT);
    return sciErr;
}

SciErr allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, (void **)_pucData);
}

SciErr allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, (void**)_pusData);
}

SciErr allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, (void**)_puiData);
}

#ifdef __SCILAB_INT64__
SciErr allocMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned long long** _pullData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT64, _iRows, _iCols, (void**)_pullData);
}
#endif

SciErr allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, (void**)_pcData);
}

SciErr allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, (void**)_psData);
}

SciErr allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, (void**)_piData);
}

#ifdef __SCILAB_INT64__
SciErr allocMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, long long** _pllData)
{
    return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT64, _iRows, _iCols, (void**)_pllData);
}
#endif

static SciErr createCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, const void* _pvData)
{
    void *pvData = NULL;

    SciErr sciErr = allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _pstName, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, &pvData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
        return sciErr;
    }

    if (pvData != NULL)
    {
        memcpy(pvData, _pvData, _iRows * _iCols * (_iPrecision % 10));
    }
    return sciErr;
}

SciErr createMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned char* _pucData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

SciErr createMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned short* _pusData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

SciErr createMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned int* _puiData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

#ifdef __SCILAB_INT64__
SciErr createMatrixOfUnsignedInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned long long* _pullData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_UINT64, _iRows, _iCols, _pullData);
}
#endif

SciErr createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* _pcData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

SciErr createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const short* _psData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

SciErr createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

#ifdef __SCILAB_INT64__
SciErr createMatrixOfInteger64InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, const long long* _pllData)
{
    return createCommonMatrixOfIntegerInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, SCI_INT64, _iRows, _iCols, _pllData);
}
#endif

static SciErr getCommonMatrixOfIntegerInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void** _pvData)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfIntegerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, _pvData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfIntegerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, (void**)_pucData);
}

SciErr getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, (void**)_pusData);
}

SciErr getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, (void**)_puiData);
}

#ifdef __SCILAB_INT64__
SciErr getMatrixOfUnsignedInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long** _pullData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT64, _piRows, _piCols, (void**)_pullData);
}
#endif

SciErr getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, (void**)_pcData);
}

SciErr getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, (void**)_psData);
}

SciErr getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, (void**)_piData);
}

#ifdef __SCILAB_INT64__
SciErr getMatrixOfInteger64InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long** _pllData)
{
    return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT64, _piRows, _piCols, (void**)_pllData);
}
#endif

static SciErr createCommonMatrixOfIntegerInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, const void* _pvData)
{
    SciErr sciErr   = sciErrInit();
    void* pvData    = NULL;
    int iVar        = 0;

    sciErr = allocCommonMatrixOfIntegerInList(_pvCtx, iVar, _pstName, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, &pvData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_INT_IN_NAMED_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (pvData != NULL)
    {
        memcpy(pvData, _pvData, _iRows * _iCols * (_iPrecision % 10));
    }

    return sciErr;
}

SciErr createMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned char* _pucData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

SciErr createMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned short* _pusData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

SciErr createMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned int* _puiData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

#ifdef __SCILAB_INT64__
SciErr createMatrixOfUnsignedInteger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const unsigned long long* _pullData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT64, _iRows, _iCols, _pullData);
}
#endif

SciErr createMatrixOfInteger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const char* _pcData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

SciErr createMatrixOfInteger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const short* _psData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

SciErr createMatrixOfInteger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const int* _piData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

#ifdef __SCILAB_INT64__
SciErr createMatrixOfInteger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, const long long* _pllData)
{
    return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT64, _iRows, _iCols, _pllData);
}
#endif

static SciErr readCommonMatrixOfIntgerInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
    SciErr sciErr = sciErrInit();
    int iNbItem    = 0;
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    void* pvData   = NULL;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, &pvData);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_pvData == NULL)
    {
        return sciErr;
    }

    memcpy(_pvData, pvData, (_iPrecision % 10 ) **_piRows **_piCols);
    return sciErr;
}

SciErr readMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, _pucData);
}

SciErr readMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, _pusData);
}

SciErr readMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, _puiData);
}

#ifdef __SCILAB_INT64__
SciErr readMatrixOfUnsignedInteger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned long long* _pullData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT64, _piRows, _piCols, _pullData);
}
#endif

SciErr readMatrixOfIntger8InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, _pcData);
}

SciErr readMatrixOfIntger16InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, _psData);
}

SciErr readMatrixOfIntger32InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, _piData);
}

#ifdef __SCILAB_INT64__
SciErr readMatrixOfIntger64InNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, long long* _pllData)
{
    return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT64, _piRows, _piCols, _pllData);
}
#endif

/*********************
* sparses functions *
*********************/

static SciErr fillCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    SciErr sciErr = sciErrInit();

    types::Sparse* pSparse = (types::Sparse*)_piParent;

    if (_iComplex)
    {
        for (int i = 0; i < _iRows; i++)
        {
            for (int j = 0; j < _piNbItemRow[i]; j++)
            {
                int iIndex = (*_piColPos++ - 1) * _iRows + i;
                std::complex<double> cplx(*_pdblReal++, *_pdblImg++);
                pSparse->set(iIndex, cplx);
            }
        }
    }
    else
    {
        for (int i = 0; i < _iRows; i++)
        {
            for (int j = 0; j < _piNbItemRow[i]; j++)
            {
                int iIndex = (*_piColPos++ - 1) * _iRows + i;
                pSparse->set(iIndex, *_pdblReal++);
            }
        }
    }

    return sciErr;
}

static SciErr createCommonSparseMatrixInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    SciErr sciErr        = sciErrInit();
    const char* funcName = NULL;
    int apiError         = API_ERROR_CREATE_SPARSE_IN_LIST;

    if (_pstName == NULL)
    {
        funcName = _iComplex ? "createComplexSparseMatrixInList" : "createSparseMatrixInList";
    }
    else
    {
        funcName = _iComplex ? "createComplexSparseMatrixInNamedList" : "createSparseMatrixInNamedList";
        apiError = API_ERROR_CREATE_SPARSE_IN_NAMED_LIST;
    }

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, apiError, funcName);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::Sparse* pSparse = NULL;
    try
    {
        pSparse = new types::Sparse(_iRows, _iCols, _iComplex == 1);
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), funcName, ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pSparse == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), funcName);
        return sciErr;
    }

    sciErr = fillCommonSparseMatrixInList(_pvCtx, _iVar, (int*)pSparse, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, apiError, _("%s: Unable to create list item #%d in Scilab memory"), funcName, _iItemPos + 1);
        return sciErr;
    }

    types::List* pParent = (types::List*)_piParent;
    pParent->set(_iItemPos - 1, pSparse);

    return sciErr;
}

SciErr createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal)
{
    return createCommonSparseMatrixInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonSparseMatrixInList(_pvCtx, _iVar, NULL, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr createSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal)
{
    return createCommonSparseMatrixInList(_pvCtx, 0, _pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr createComplexSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonSparseMatrixInList(_pvCtx, 0, _pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static SciErr getCommonSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
    return getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
    return getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static SciErr readCommonSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iNbItem    = 0;
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    int* piNbItemRow = NULL;
    int* piColPos   = NULL;
    double* pdblReal = NULL;
    double* pdblImg  = NULL;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_piNbItemRow == NULL)
    {
        return sciErr;
    }
    memcpy(_piNbItemRow, piNbItemRow, *_piRows * sizeof(int));

    if (_piColPos == NULL)
    {
        return sciErr;
    }
    memcpy(_piColPos, piColPos, *_piNbItem * sizeof(int));

    if (_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
    {
        return sciErr;
    }

    memcpy(_pdblReal, pdblReal, sizeof(double) **_piNbItem);
    if (_iComplex)
    {
        memcpy(_pdblImg, pdblImg, sizeof(double) **_piNbItem);
    }

    return sciErr;
}

SciErr readSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
    return readCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr readComplexSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
    return readCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}


/*****************************
* boolean sparses functions *
*****************************/
static SciErr fillBooleanSparseMatrixInList(void* _pvCtx, int _iVar, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    SciErr sciErr = sciErrInit();
    types::SparseBool* pSparse = (types::SparseBool*)_piParent;

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

SciErr createCommonBooleanSparseMatrixInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    SciErr sciErr = sciErrInit();
    const char* funcName = NULL;
    int apiError = API_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST;

    if (_pstName == NULL)
    {
        funcName = "createBooleanSparseMatrixInList";
    }
    else
    {
        funcName = "createBooleanSparseMatrixInNamedList";
        apiError = API_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST;
    }

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, apiError, funcName);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::SparseBool* pSparse = NULL;
    try
    {
        pSparse = new types::SparseBool(_iRows, _iCols);
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), funcName, ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pSparse == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), funcName);
        return sciErr;
    }

    sciErr = fillBooleanSparseMatrixInList(_pvCtx, 0, NULL, (int*)pSparse, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in Scilab memory"), funcName, _iItemPos + 1);
        return sciErr;
    }

    types::List* piParent = (types::List*)_piParent;
    piParent->set(_iItemPos - 1, pSparse);

    return sciErr;
}

SciErr createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    return createCommonBooleanSparseMatrixInList(_pvCtx, NULL, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
}

SciErr createBooleanSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, const int* _piNbItemRow, const int* _piColPos)
{
    return createCommonBooleanSparseMatrixInList(_pvCtx, _pstName, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
}

SciErr getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr readBooleanSparseMatrixInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
    SciErr sciErr = sciErrInit();
    int iNbItem    = 0;
    int* piAddr    = NULL;
    int* piRoot    = NULL;
    int* piNbItemRow = NULL;
    int* piColPos   = NULL;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    if (_piNbItemRow == NULL)
    {
        freeAllocatedBooleanSparse(piNbItemRow, piColPos);
        return sciErr;
    }
    memcpy(_piNbItemRow, piNbItemRow, *_piRows * sizeof(int));

    if (_piColPos == NULL)
    {
        freeAllocatedBooleanSparse(piNbItemRow, piColPos);
        return sciErr;
    }
    memcpy(_piColPos, piColPos, *_piNbItem * sizeof(int));

    freeAllocatedBooleanSparse(piNbItemRow, piColPos);
    return sciErr;
}

/*********************
* Pointer functions *
*********************/
SciErr getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr)
{
    int* piAddr  = NULL;

    SciErr sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    sciErr = getPointer(_pvCtx, piAddr, _pvPtr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
        return sciErr;
    }

    return sciErr;
}

SciErr createPointerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, void* _pvPtr)
{
    return createCommonPointerInList(_pvCtx, NULL, _piParent, _iItemPos, _pvPtr);
}

SciErr createPointerInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr)
{
    return createCommonPointerInList(_pvCtx, _pstName, _piParent, _iItemPos, _pvPtr);
}

SciErr createCommonPointerInList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr)
{
    SciErr sciErr        = sciErrInit();
    const char* funcName = NULL;
    int apiError         = API_ERROR_CREATE_POINTER_IN_LIST;

    if (_pstName == NULL)
    {
        funcName = "createPointerInList";
    }
    else
    {
        funcName = "createPointerInNamedList";
        apiError = API_ERROR_CREATE_POINTER_IN_NAMED_LIST;
    }

    sciErr = checkListItemPosition(_pvCtx, _piParent, _iItemPos, apiError, funcName);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::Pointer* pP = NULL;
    try
    {
        pP = new types::Pointer(_pvPtr);
        if (pP == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), "createPointerInList");
            return sciErr;
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), "createPointerInList", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    types::List* pParent   = (types::List*)_piParent;
    pParent->set(_iItemPos - 1, pP);

    return sciErr;
}

SciErr readPointerInNamedList(void* _pvCtx, const char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr)
{
    SciErr sciErr = sciErrInit();
    int iNbItem    = 0;
    int* piAddr    = NULL;
    int* piRoot    = NULL;

    if (_piParent == NULL)
    {
        sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
        if (sciErr.iErr)
        {
            addErrorMessage(&sciErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
            return sciErr;
        }

        sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
    }
    else
    {
        sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    sciErr = getPointer(_pvCtx, piAddr, _pvPtr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
        return sciErr;
    }

    return sciErr;
}

/********************
* tools  functions *
********************/

//internal tool functions
/*--------------------------------------------------------------------------*/

int isListType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_list);
}

/*--------------------------------------------------------------------------*/
int isNamedListType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_list);
}

/*--------------------------------------------------------------------------*/
int isTListType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_tlist);
}

/*--------------------------------------------------------------------------*/
int isNamedTListType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_tlist);
}

/*--------------------------------------------------------------------------*/
int isMListType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_mlist);
}

/*--------------------------------------------------------------------------*/
int isNamedMListType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_mlist);
}
/*--------------------------------------------------------------------------*/
