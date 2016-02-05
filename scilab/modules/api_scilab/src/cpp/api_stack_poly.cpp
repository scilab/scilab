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
#include "polynom.hxx"
#include "context.hxx"

extern "C"
{
#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "call_scilab.h"
#include "api_scilab.h"
#include "api_internal_poly.h"
#include "api_internal_common.h"
#include "localization.h"
#include "sci_malloc.h"
#include "charEncoding.h"
}

static int getCommonAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int getCommonAllocatedNamedSinglePoly(void* _pvCtx, const char* _pstName, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int getCommonAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);
static int getCommonAllocatedNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg);


SciErr getPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen)
{
    SciErr sciErr = sciErrInit();

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getPolyVariableName");
        return sciErr;
    }

    if (!((types::InternalType*)_piAddress)->isPoly())
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), "getPolyVariableName", _("polynomial matrix"));
        return sciErr;
    }

    if (*_piVarNameLen == 0)
    {
        *_piVarNameLen = (int)((types::InternalType*)_piAddress)->getAs<types::Polynom>()->getVariableName().size();
        //No error
    }

    if (_pstVarName == NULL)
    {
        return sciErr;
    }

    char* pstTemp = wide_string_to_UTF8(((types::InternalType*)_piAddress)->getAs<types::Polynom>()->getVariableName().c_str());
    strcpy(_pstVarName, pstTemp);
    FREE(pstTemp);
    *_piVarNameLen = static_cast<int>(strlen(_pstVarName));
    return sciErr;
}

SciErr getMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
    return getCommonMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr getComplexMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return getCommonMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr getCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iType = 0;
    int iSize = 0;
    int *piOffset = NULL;
    double *pdblReal = NULL;
    double *pdblImg = NULL;

    if (_piAddress == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly");
        return sciErr;
    }

    sciErr = getVarType(_pvCtx, _piAddress, &iType);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    if (iType != sci_poly)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s expected"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", _("polynomial matrix"));
        return sciErr;
    }

    if (isVarComplex(_pvCtx, _piAddress) != _iComplex)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly");
        return sciErr;
    }

    sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    iSize	= *_piRows **_piCols;

    if (_piNbCoef == NULL)
    {
        return sciErr;
    }

    types::Polynom *pMP = ((types::InternalType*)_piAddress)->getAs<types::Polynom>();
    pMP->getSizes(_piNbCoef);

    if (_pdblReal == NULL)
    {
        return sciErr;
    }

    types::SinglePoly** pSP = pMP->get();
    if (_iComplex == 1)
    {
        for (int i = 0 ; i < iSize ; i++)
        {
            memcpy(_pdblReal[i], pSP[i]->get(),    sizeof(double) * pSP[i]->getSize());
            memcpy(_pdblImg[i],  pSP[i]->getImg(), sizeof(double) * _piNbCoef[i]);
        }
    }
    else
    {
        for (int i = 0 ; i < iSize ; i++)
        {
            memcpy(_pdblReal[i], pSP[i]->get(), sizeof(double) * pSP[i]->getSize());
        }
    }

    return sciErr;
}

SciErr createMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal)
{
    return createCommonMatrixOfPoly(_pvCtx, _iVar, 0, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createComplexMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    return createCommonMatrixOfPoly(_pvCtx, _iVar, 1, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonMatrixOfPoly(void* _pvCtx, int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    if (_pvCtx == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "createComplexMatrixOfPoly" : "createMatrixOfPoly");
        return sciErr;
    }

    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

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

    wchar_t* pstTemp = to_wide_string(_pstVarName);
    std::wstring wstTemp(pstTemp);
    types::Polynom* pP = new types::Polynom(wstTemp, _iRows, _iCols, _piNbCoef);
    FREE(pstTemp);
    if (pP == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), _iComplex ? "createComplexMatrixOfPoly" : "createMatrixOfPoly");
        return sciErr;
    }

    if (_iComplex)
    {
        pP->setComplex(true);
    }

    out[rhs - 1] = pP;

    for (int i = 0 ; i < pP->getSize() ; i++)
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

    return sciErr;
}

SciErr createNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal)
{
    return createCommonNamedMatrixOfPoly(_pvCtx, _pstName, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createNamedComplexMatrixOfPoly(void* _pvCtx, const char* _pstName, char* _pstVarName, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    return createCommonNamedMatrixOfPoly(_pvCtx, _pstName, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, char* _pstVarName, int _iComplex, int _iRows, int _iCols, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    SciErr sciErr = sciErrInit();

    // check variable name
    if (checkNamedVarFormat(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Invalid variable name: %s."), "createCommonNamedMatrixOfPoly", _pstName);
        return sciErr;
    }

    //return empty matrix
    if (_iRows == 0 && _iCols == 0)
    {
        if (createNamedEmptyMatrix(_pvCtx, _pstName))
        {
            addErrorMessage(&sciErr, API_ERROR_CREATE_EMPTY_MATRIX, _("%s: Unable to create variable in Scilab memory"), "createNamedEmptyMatrix");
            return sciErr;
        }

        return sciErr;
    }

    wchar_t* pstTemp = to_wide_string(_pstVarName);
    std::wstring wstTemp(pstTemp);
    types::Polynom* pP = new types::Polynom(wstTemp, _iRows, _iCols, _piNbCoef);
    FREE(pstTemp);
    if (pP == NULL)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Invalid variable name: %s."), "createCommonNamedMatrixOfPoly", _pstName);
        return sciErr;
    }

    if (_iComplex)
    {
        pP->setComplex(true);
    }

    for (int i = 0 ; i < pP->getSize() ; i++)
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

    wchar_t* pwstName = to_wide_string(_pstName);
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

SciErr readNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
    return readCommonNamedMatrixOfPoly(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr readNamedComplexMatrixOfPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return readCommonNamedMatrixOfPoly(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr readCommonNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    int* piAddr = NULL;

    SciErr sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_POLY, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfPoly" : "readNamedMatrixOfPoly", _pstName);
        return sciErr;
    }

    if (_iComplex == 1)
    {
        sciErr = getComplexMatrixOfPoly(_pvCtx, piAddr, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
    }
    else
    {
        sciErr = getMatrixOfPoly(_pvCtx, piAddr, _piRows, _piCols, _piNbCoef, _pdblReal);
    }

    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_READ_NAMED_POLY, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfPoly" : "readNamedMatrixOfPoly", _pstName);
        return sciErr;
    }

    return sciErr;
}

/*shortcut functions */

/*--------------------------------------------------------------------------*/
int isPolyType(void* _pvCtx, int* _piAddress)
{
    return checkVarType(_pvCtx, _piAddress, sci_poly);
}
/*--------------------------------------------------------------------------*/
int isNamedPolyType(void* _pvCtx, const char* _pstName)
{
    return checkNamedVarType(_pvCtx, _pstName, sci_poly);
}
/*--------------------------------------------------------------------------*/
int getAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal)
{
    return getCommonAllocatedSinglePoly(_pvCtx, _piAddress, 0, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedSingleComplexPoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return getCommonAllocatedSinglePoly(_pvCtx, _piAddress, 1, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iRows	= 0;
    int iCols	= 0;

    double* pdblReal = NULL;
    double* pdblImg	 = NULL;

    if (isScalar(_pvCtx, _piAddress) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, _piNbCoef, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pdblReal = (double*)MALLOC(sizeof(double) **_piNbCoef);

    if (_iComplex)
    {
        *_pdblImg	= (double*)MALLOC(sizeof(double) **_piNbCoef);
    }

    sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, _piNbCoef, _pdblReal, _pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedSinglePoly(void* _pvCtx, const char* _pstName, int* _piNbCoef, double** _pdblReal)
{
    return getCommonAllocatedNamedSinglePoly(_pvCtx, _pstName, 0, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedSingleComplexPoly(void* _pvCtx, const char* _pstName, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    return getCommonAllocatedNamedSinglePoly(_pvCtx, _pstName, 1, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedNamedSinglePoly(void* _pvCtx, const char* _pstName, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    int iRows	= 0;
    int iCols	= 0;

    double* pdblReal = NULL;
    double* pdblImg	 = NULL;

    if (isNamedScalar(_pvCtx, _pstName) == 0)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_POLY, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), _iComplex ? "getAllocatedNamedSingleComplexPoly" : "getAllocatedNamedSinglePoly", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, &iRows, &iCols, _piNbCoef, &pdblReal, &pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedSingleComplexPoly" : "getAllocatedNamedSinglePoly", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pdblReal = (double*)MALLOC(sizeof(double) **_piNbCoef);
    memcpy(*_pdblReal, pdblReal, sizeof(double) **_piNbCoef);

    if (_iComplex)
    {
        *_pdblImg	= (double*)MALLOC(sizeof(double) **_piNbCoef);
        memcpy(*_pdblImg, pdblImg, sizeof(double) **_piNbCoef);
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal)
{
    return getCommonAllocatedMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedMatrixOfComplexPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
    return getCommonAllocatedMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    double* pdblReal	= NULL;
    double* pdblImg		= NULL;

    sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, NULL, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedMatrixOfComplexPoly" : "getAllocatedMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_piNbCoef = (int*)MALLOC(sizeof(int) **_piRows **_piCols);

    sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, *_piNbCoef, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedMatrixOfComplexPoly" : "getAllocatedMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pdblReal = (double**)MALLOC(sizeof(double*) **_piRows **_piCols);
    for (int i = 0 ; i < *_piRows **_piCols ; i++)
    {
        (*_pdblReal)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
    }

    if (_iComplex)
    {
        *_pdblImg	= (double**)MALLOC(sizeof(double*) **_piRows **_piCols);
        for (int i = 0 ; i < *_piRows **_piCols ; i++)
        {
            (*_pdblImg)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
        }
    }

    sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, *_piNbCoef, *_pdblReal, _pdblImg == NULL ? NULL : *_pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal)
{
    return getCommonAllocatedNamedMatrixOfPoly(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedMatrixOfComplexPoly(void* _pvCtx, const char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
    return getCommonAllocatedNamedMatrixOfPoly(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedNamedMatrixOfPoly(void* _pvCtx, const char* _pstName, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
    SciErr sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, _piRows, _piCols, NULL, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_MATRIX_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedMatrixOfComplexPoly" : "getAllocatedNamedMatrixOfPoly", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_piNbCoef = (int*)MALLOC(sizeof(int) **_piRows **_piCols);

    sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, _piRows, _piCols, *_piNbCoef, NULL, NULL);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_MATRIX_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedMatrixOfComplexPoly" : "getAllocatedNamedMatrixOfPoly", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    *_pdblReal = (double**)MALLOC(sizeof(double*) **_piRows **_piCols);
    for (int i = 0 ; i < *_piRows **_piCols ; i++)
    {
        (*_pdblReal)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
    }

    if (_iComplex)
    {
        *_pdblImg	= (double**)MALLOC(sizeof(double*) **_piRows **_piCols);
        for (int i = 0 ; i < *_piRows **_piCols ; i++)
        {
            (*_pdblImg)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
        }
    }

    sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, _piRows, _piCols, *_piNbCoef, *_pdblReal, _pdblImg == NULL ? NULL : *_pdblImg);
    if (sciErr.iErr)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_MATRIX_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedMatrixOfComplexPoly" : "getAllocatedNamedMatrixOfPoly", _pstName);
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSinglePoly(double* _pdblReal)
{
    FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSingleComplexPoly(double* _pdblReal, double* _pdblImg)
{
    freeAllocatedSinglePoly(_pdblReal);
    FREE(_pdblImg);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedMatrixOfPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
    FREE(_piNbCoef);

    for (int i = 0 ; i < _iRows * _iCols ; i++)
    {
        FREE(_pdblReal[i]);
    }
    FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedMatrixOfComplexPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    freeAllocatedMatrixOfPoly(_iRows, _iCols, _piNbCoef, _pdblReal);

    for (int i = 0 ; i < _iRows * _iCols ; i++)
    {
        FREE(_pdblImg[i]);
    }
    FREE(_pdblImg);
}
/*--------------------------------------------------------------------------*/
