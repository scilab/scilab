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

#include "gatewaystruct.hxx"
#include "double.hxx"
#include "context.hxx"

extern "C"
{
#include "doublecomplex.h"
#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_double.h"
#include "localization.h"
#include "elem_common.h"
#include "sci_tools.h"
    extern int C2F(entier)(int *n, double *d__, int *s);
    extern int C2F(icopy)(int *, int *, int *, int *, int *);
}

/*******************************/
/*   double matrix functions   */
/*******************************/

static int getCommonScalarDouble(void* _pvCtx, int* _piAddress, int _iComplex, double* _pdblReal, double* _pdblImg);
static int createCommonScalarDouble(void* _pvCtx, int _iVar, int _iComplex, double _dblReal, double _dblImg);
static int createCommonNamedScalarDouble(void* _pvCtx, const char* _pstName, int _iComplex, double _dblReal, double _dblImg);
static int getCommonNamedScalarDouble(void* _pvCtx, const char* _pstName, int _iComplex, double* _pdblReal, double* _pdblImg);
static int createCommonMatrixDoubleFromInteger(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int* _piReal, int* _piImg);
static int createCommonScalarDoubleFromInteger(void* _pvCtx, int _iVar, int _iComplex, int _iReal, int _iImg);

SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
{
    return getCommonMatrixOfDouble(_pvCtx, _piAddress, '$', 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    return getCommonMatrixOfDouble(_pvCtx, _piAddress, '$', 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

SciErr getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ)
{
    int iSize = 0;
    double *pdblReal = NULL;

    SciErr sciErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, 'z', 0, _piRows, _piCols, &pdblReal, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ZDOUBLE, _("%s: Unable to get argument #%d"), "getComplexZMatrixOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    types::Double* pDbl = (types::Double*)_piAddress;
    pDbl->convertToZComplex();
    *_pdblZ	= (doublecomplex*)(pDbl->get());
    return sciErr;
}

SciErr getMatrixOfDoubleAsInteger(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piReal)
{
    double* pdblReal = NULL;
    int iSize = 0;
    SciErr sciErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, 'i', 0, _piRows, _piCols, &pdblReal, NULL);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    iSize = *_piRows **_piCols;
    *_piReal = (int*)pdblReal;

    //Warning we overwrite double by int !!!!
    C2F(entier)(&iSize, pdblReal, *_piReal);

    types::Double* pD = (types::Double*)_piAddress;
    pD->setViewAsInteger();

    return sciErr;
}

SciErr getComplexMatrixOfDoubleAsInteger(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piReal, int** _piImg)
{
    double* pdblReal = NULL;
    double* pdblImg  = NULL;

    SciErr sciErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, 'i', 1, _piRows, _piCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        return sciErr;
    }

    types::Double* pD = (types::Double*)_piAddress;
    //convert values and view of data to int and int*
    pD->convertToInteger();

    *_piReal = (int*)pD->get();
    *_piImg = (int*)pD->getImg();

    return sciErr;
}

SciErr getCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, char _cType, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;
    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr || iType != sci_matrix)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble", _("double matrix"));
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (_pdblReal != NULL)
    {
        *_pdblReal = ((types::InternalType*)_piAddress)->getAs<types::Double>()->getReal();
    }

    if (_iComplex && _pdblImg != NULL)
    {
        *_pdblImg = ((types::InternalType*)_piAddress)->getAs<types::Double>()->getImg();
    }

    return sciErr;
}

SciErr allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal)
{
    double *pdblReal	= NULL;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, '$', 0, _iRows, _iCols, &pdblReal, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfDouble");
        return sciErr;
    }

    *_pdblReal = pdblReal;

    return sciErr;
}

SciErr allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, '$', 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    *_pdblReal = pdblReal;
    *_pdblImg  = pdblImg;
    return sciErr;
}

SciErr allocMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piReal)
{
    double *pdblReal	= NULL;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'i', 0, _iRows, _iCols, &pdblReal, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfDouble");
        return sciErr;
    }

    *_piReal = (int*)pdblReal;
    return sciErr;
}

SciErr allocComplexMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piReal, int** _piImg)
{
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'i', 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    *_piReal = (int*)pdblReal;
    *_piImg  = (int*)pdblImg;
    return sciErr;
}

SciErr allocCommonMatrixOfDouble(void* _pvCtx, int _iVar, char _cType, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();

    if (_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "allocComplexMatrixOfDouble" : "allocMatrixOfDouble");
        return sciErr;
    }

    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::InternalType** out = pStr->m_pOut;

    types::Double* pDbl = NULL;
    try
    {
        if (_cType == 'z')
        {
            pDbl = new types::Double(_iRows, _iCols, _iComplex == 1, true);
        }
        else
        {
            pDbl = new types::Double(_iRows, _iCols, _iComplex == 1);
            if (_cType == 'i')
            {
                pDbl->setViewAsInteger();
            }
        }
    }
    catch (const ast::InternalError& ie)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: %ls"), _iComplex ? "allocComplexMatrixOfDouble" : "allocMatrixOfDouble", ie.GetErrorMessage().c_str());
        return sciErr;
    }

    if (pDbl == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), _iComplex ? "allocComplexMatrixOfDouble" : "allocMatrixOfDouble");
        return sciErr;
    }

    int rhs = _iVar - *getNbInputArgument(_pvCtx);
    out[rhs - 1] = pDbl;
    *_pdblReal = pDbl->getReal();
    if (*_pdblReal == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), _iComplex ? "allocComplexMatrixOfDouble" : "allocexMatrixOfDouble");
        delete pDbl;
        return sciErr;
    }

    if (_iComplex && _pdblImg != NULL)
    {
        *_pdblImg	= pDbl->getImg();
        if (*_pdblImg == NULL)
        {
            addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocate variable"), _iComplex ? "allocComplexMatrixOfDouble" : "allocMatrixOfDouble");
            delete pDbl;
            return sciErr;
        }
    }
    
    return sciErr;
}

SciErr allocComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex** _pdblData)
{
    SciErr sciErr = sciErrInit();
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'z', 1, _iRows, _iCols, &pdblReal, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_ALLOC_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    *_pdblData = (doublecomplex*)pdblReal;
    return sciErr;
}

SciErr createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal)
{
    double *pdblReal = NULL;

    int iOne = 1;
    int iSize = _iRows * _iCols;

    SciErr sciErr = allocMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    C2F(dcopy)(&iSize, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
    return sciErr;
}

SciErr createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    int iOne = 1;
    int iSize = _iRows * _iCols;

    SciErr sciErr = allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    C2F(dcopy)(&iSize, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);
    C2F(dcopy)(&iSize, const_cast<double*>(_pdblImg), &iOne, pdblImg, &iOne);
    return sciErr;
}

SciErr createMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piReal)
{
    double *pdblReal	= NULL;

    int iOne = 1;
    int iSize = _iRows * _iCols;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'i', 0, _iRows, _iCols, &pdblReal, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    C2F(icopy)(&iSize, (int*)_piReal, &iOne, (int*)pdblReal, &iOne);
    return sciErr;
}

SciErr createComplexMatrixOfDoubleAsInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, const int* _piReal, const int* _piImg)
{
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    int iOne = 1;
    int iSize = _iRows * _iCols;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'i', 1, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    C2F(icopy)(&iSize, (int*)_piReal, &iOne, (int*)pdblReal, &iOne);
    C2F(icopy)(&iSize, (int*)_piImg, &iOne, (int*)pdblImg, &iOne);
    return sciErr;
}

SciErr createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, const doublecomplex* _pdblData)
{
    SciErr sciErr = sciErrInit();
    const doublecomplex *pdblZ = NULL;

    sciErr = allocComplexZMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblZ);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_ZDOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
        return sciErr;
    }

    memcpy((void*)pdblZ, _pdblData, _iRows * _iCols * sizeof(doublecomplex));
    return sciErr;
}

SciErr createNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const double* _pdblReal)
{
    return createCommonNamedMatrixOfDouble(_pvCtx, _pstName, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr createNamedComplexMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    return createCommonNamedMatrixOfDouble(_pvCtx, _pstName, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr createNamedComplexZMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iRows, int _iCols, const doublecomplex* _pdblData)
{
    SciErr sciErr = sciErrInit();

    int iOne					= 1;
    int iTwo					= 2;
    int iSize					= _iRows * _iCols;

    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createNamedComplexZMatrixOfDouble", _pstName);
        return sciErr;
    }

    types::Double* pDbl = new types::Double(_iRows, _iCols, true);

    double* pdblReal = pDbl->get();
    double* pdblImg = pDbl->getImg();
    C2F(dcopy)(&iSize, const_cast<double*>(&_pdblData->r), &iTwo, pdblReal, &iOne);
    C2F(dcopy)(&iSize, const_cast<double*>(&_pdblData->i), &iOne, pdblImg, &iOne);

    wchar_t* pwstName = to_wide_string(_pstName);
    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pDbl);
    }
    else
    {
        delete pDbl;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }

    return sciErr;
}

SciErr createCommonNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iComplex, int _iRows, int _iCols, const double* _pdblReal, const double* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    if (!checkNamedVarFormat(_pvCtx, _pstName))
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createCommonNamedMatrixOfDouble", _pstName);
        return sciErr;
    }

    wchar_t* pwstName           = to_wide_string(_pstName);
    int iOne					= 1;
    int iSize					= _iRows * _iCols;

    types::Double* pDbl = new types::Double(_iRows, _iCols, _iComplex == 1);

    double* pdblReal = pDbl->get();
    C2F(dcopy)(&iSize, const_cast<double*>(_pdblReal), &iOne, pdblReal, &iOne);

    if (_iComplex)
    {
        double* pdblImg = pDbl->getImg();
        C2F(dcopy)(&iSize, const_cast<double*>(_pdblImg), &iOne, pdblImg, &iOne);
    }

    symbol::Context* ctx = symbol::Context::getInstance();
    symbol::Symbol sym = symbol::Symbol(pwstName);
    FREE(pwstName);
    if (ctx->isprotected(sym) == false)
    {
        ctx->put(sym, pDbl);
    }
    else
    {
        delete pDbl;
        addErrorMessage(&sciErr, API_ERROR_REDEFINE_PERMANENT_VAR, _("Redefining permanent variable.\n"));
    }
    return sciErr;
}

SciErr readNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, double* _pdblReal)
{
    return readCommonNamedMatrixOfDouble(_pvCtx, _pstName, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr readNamedComplexMatrixOfDouble(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
    return readCommonNamedMatrixOfDouble(_pvCtx, _pstName, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

SciErr readCommonNamedMatrixOfDouble(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
    int* piAddr = NULL;
    double* pdblReal = NULL;
    double* pdblImg = NULL;
    int iSize = 0;
    int iOne = 1;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_DOUBLE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfDouble" : "readNamedMatrixOfDouble", _pstName);
        return sciErr;
    }

    sciErr = getCommonMatrixOfDouble(_pvCtx, piAddr, '$', _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_DOUBLE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfDouble" : "readNamedMatrixOfDouble", _pstName);
        return sciErr;
    }

    if (_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
    {
        return sciErr;
    }

    iSize = (*_piRows) * (*_piCols);
    /* dcopy source to dest */
    C2F(dcopy)(&iSize, pdblReal, &iOne, _pdblReal, &iOne);
    if (_iComplex)
    {
        C2F(dcopy)(&iSize, pdblImg, &iOne, _pdblImg, &iOne);
    }

    return sciErr;
}

/*shortcut functions*/

int isDoubleType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_matrix);
}
/*--------------------------------------------------------------------------*/
int isNamedDoubleType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_matrix);
}
/*--------------------------------------------------------------------------*/
int getScalarDouble(void* _pvCtx, int* _piAddress, double* _pdblReal)
{
    return getCommonScalarDouble(_pvCtx, _piAddress, 0, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getScalarComplexDouble(void* _pvCtx, int* _piAddress, double* _pdblReal, double* _pdblImg)
{
    return getCommonScalarDouble(_pvCtx, _piAddress, 1, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonScalarDouble(void* _pvCtx, int* _piAddress, int _iComplex, double* _pdblReal, double* _pdblImg)
{
    int iRows = 0;
    int iCols = 0;

    double* pdblReal = NULL;
    double* pdblImg = NULL;

    SciErr sciErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, '$', _iComplex, &iRows, &iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getScalarComplexDouble" : "getScalarDouble", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (isScalar(_pvCtx, _piAddress) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_DOUBLE, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _iComplex ? "getScalarComplexDouble" : "getScalarDouble", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (_pdblReal != NULL)
    {
        *_pdblReal	= pdblReal[0];
    }

    if (_iComplex == 1 && _pdblImg != NULL && pdblImg != NULL)
    {
        *_pdblImg		= pdblImg[0];
    }
    else if (_pdblImg != NULL)
    {
        *_pdblImg = 0;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarDouble(void* _pvCtx, const char* _pstName, double* _pdblReal)
{
    return getCommonNamedScalarDouble(_pvCtx, _pstName, 0, _pdblReal, 0);
}
/*--------------------------------------------------------------------------*/
int getNamedScalarComplexDouble(void* _pvCtx, const char* _pstName, double* _pdblReal, double* _pdblImg)
{
    return getCommonNamedScalarDouble(_pvCtx, _pstName, 1, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonNamedScalarDouble(void* _pvCtx, const char* _pstName, int _iComplex, double* _pdblReal, double* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iRows	= 0;
    int iCols	= 0;

    double dblReal = 0;
    double dblImg	 = 0;

    if (isNamedScalar(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_DOUBLE, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), _iComplex ? "getNamedScalarComplexDouble" : "getNamedScalarDouble", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = readCommonNamedMatrixOfDouble(_pvCtx, _pstName, _iComplex, &iRows, &iCols, &dblReal, &dblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_DOUBLE, _("%s: Unable to get argument \"%s\""), _iComplex ? "getNamedScalarComplexDouble" : "getNamedScalarDouble", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    if (_pdblReal != NULL)
    {
        _pdblReal[0]	= dblReal;
    }

    if (_pdblImg != NULL)
    {
        _pdblImg[0]		= dblImg;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarDouble(void* _pvCtx, int _iVar, double _dblReal)
{
    return createCommonScalarDouble(_pvCtx, _iVar, 0, _dblReal, 0);
}
/*--------------------------------------------------------------------------*/
int createScalarComplexDouble(void* _pvCtx, int _iVar, double _dblReal, double _dblImg)
{
    return createCommonScalarDouble(_pvCtx, _iVar, 1, _dblReal, _dblImg);
}
/*--------------------------------------------------------------------------*/
static int createCommonScalarDouble(void* _pvCtx, int _iVar, int _iComplex, double _dblReal, double _dblImg)
{
    double *pdblReal	= NULL;
    double *pdblImg		= NULL;

    SciErr sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, '$', _iComplex, 1, 1, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_DOUBLE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createScalarComplexDouble" : "createScalarDouble");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    pdblReal[0] = _dblReal;
    if (_iComplex)
    {
        pdblImg[0]	= _dblImg;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarDouble(void* _pvCtx, const char* _pstName, double _dblReal)
{
    return createCommonNamedScalarDouble(_pvCtx, _pstName, 0, _dblReal, 0);
}
/*--------------------------------------------------------------------------*/
int createNamedScalarComplexDouble(void* _pvCtx, const char* _pstName, double _dblReal, double _dblImg)
{
    return createCommonNamedScalarDouble(_pvCtx, _pstName, 1, _dblReal, _dblImg);
}
/*--------------------------------------------------------------------------*/
static int createCommonNamedScalarDouble(void* _pvCtx, const char* _pstName, int _iComplex, double _dblReal, double _dblImg)
{
    SciErr sciErr = createCommonNamedMatrixOfDouble(_pvCtx, _pstName, _iComplex, 1, 1, &_dblReal, &_dblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createNamedScalarComplexDouble" : "createNamedScalarDouble");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarDoubleFromInteger(void* _pvCtx, int _iVar, int _iReal)
{
    return createCommonScalarDoubleFromInteger(_pvCtx, _iVar, 0, _iReal, 0);
}
/*--------------------------------------------------------------------------*/
int createScalarComplexDoubleFromInteger(void* _pvCtx, int _iVar, int _iReal, int _iImg)
{
    return createCommonScalarDoubleFromInteger(_pvCtx, _iVar, 1, _iReal, _iImg);
}
/*--------------------------------------------------------------------------*/
static int createCommonScalarDoubleFromInteger(void* _pvCtx, int _iVar, int _iComplex, int _iReal, int _iImg)
{
    SciErr sciErr;
    double* pdblReal = NULL;
    double* pdblImg     = NULL;


    sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'i', _iComplex, 1, 1, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_FROM_INTEGER, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createScalarComplexDoubleFromInteger" : "createScalarDoubleFromInteger");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    pdblReal[0] = (double)_iReal;

    if (_iComplex)
    {
        pdblImg[0] = (double)_iImg;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int createMatrixOfDoubleFromInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, int* _piReal)
{
    return createCommonMatrixDoubleFromInteger(_pvCtx, _iVar, 0, _iRows, _iCols, _piReal, NULL);
}
/*--------------------------------------------------------------------------*/
int createMatrixOfComplexDoubleFromInteger(void* _pvCtx, int _iVar, int _iRows, int _iCols, int* _piReal, int* _piImg)
{
    return createCommonMatrixDoubleFromInteger(_pvCtx, _iVar, 1, _iRows, _iCols, _piReal, _piImg);
}
/*--------------------------------------------------------------------------*/
static int createCommonMatrixDoubleFromInteger(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int* _piReal, int* _piImg)
{
    SciErr sciErr;
    double* pdblReal = NULL;
    double* pdblImg     = NULL;


    sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 'i', _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_MATRIX_FROM_INTEGER, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createMatrixOfComplexDoubleFromInteger" : "createMatrixOfDoubleFromInteger");
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    memcpy(pdblReal, _piReal, _iRows * _iCols * sizeof(int));

    if (_iComplex)
    {
        memcpy(pdblImg, _piImg, _iRows * _iCols * sizeof(int));
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
