/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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

#include <stdio.h>
#include "double.hxx"
#include "bool.hxx"
#include "string.hxx"
#include "int.hxx"
#include "polynom.hxx"
#include "gatewaystruct.hxx"

extern "C"
{
#include "api_scilab.h"
#include "api_internal_common.h"
#include "api_internal_double.h"
#include "localization.h"

#include "call_scilab.h"
}

int isHypermatType(void* _pvCtx, int* _piAddress)
{
    types::InternalType* it = (types::InternalType*)_piAddress;
    if (it->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType* gt = it->getAs<types::GenericType>();
    if (gt->getDims() < 3)
    {
        return 0;
    }

    return 1;
}

int isHypermatComplex(void* _pvCtx, int* _piAddress)
{
    types::InternalType* it = (types::InternalType*)_piAddress;
    if (it->isGenericType() == false)
    {
        return 0;
    }

    types::GenericType* gt = it->getAs<types::GenericType>();
    if (gt->isComplex())
    {
        return 1;
    }

    return 0;
}

SciErr getHypermatType(void *_pvCtx, int *_piAddress, int *_piType)
{
    return getVarType(_pvCtx, _piAddress, _piType);
}

SciErr getHypermatOfIntegerPrecision(void *_pvCtx, int *_piAddress, int *_piPrecision)
{
    return getMatrixOfIntegerPrecision(_pvCtx, _piAddress, _piPrecision);
}

SciErr getHypermatDimensions(void *_pvCtx, int *_piAddress, int **_dims, int *_ndims)
{
    SciErr sciErr = sciErrInit();
    types::InternalType* it = (types::InternalType*)_piAddress;
    if (it->isGenericType() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_NOT_MATRIX_TYPE, _("%s: matrix argument expected"), "getHypermatDimensions");
        return sciErr;
    }

    types::GenericType* gt = it->getAs<types::GenericType>();
    *_ndims = gt->getDims();
    *_dims = gt->getDimsArray();
    return sciErr;
}

static int getHypermatEntries(void* _pvCtx, int* _piAddress, void** _piEntriesAddress)
{
    types::InternalType* it = (types::InternalType*)_piAddress;
    if (it->isGenericType() == false)
    {
        return 1;
    }

    switch (it->getType())
    {
        case types::InternalType::ScilabDouble:
        {
            *_piEntriesAddress = it->getAs<types::Double>();
            break;
        }
        case types::InternalType::ScilabBool:
        {
            *_piEntriesAddress = it->getAs<types::Bool>();
            break;
        }
        case types::InternalType::ScilabString:
        {
            *_piEntriesAddress = it->getAs<types::String>();
            break;
        }
        case types::InternalType::ScilabPolynom:
        {
            *_piEntriesAddress = it->getAs<types::Polynom>();
            break;
        }
        case types::InternalType::ScilabInt8:
        {
            *_piEntriesAddress = it->getAs<types::Int8>();
            break;
        }
        case types::InternalType::ScilabInt16:
        {
            *_piEntriesAddress = it->getAs<types::Int16>();
            break;
        }
        case types::InternalType::ScilabInt32:
        {
            *_piEntriesAddress = it->getAs<types::Int32>();
            break;
        }
        case types::InternalType::ScilabInt64:
        {
            *_piEntriesAddress = it->getAs<types::Int64>();
            break;
        }
        case types::InternalType::ScilabUInt8:
        {
            *_piEntriesAddress = it->getAs<types::UInt8>();
            break;
        }
        case types::InternalType::ScilabUInt16:
        {
            *_piEntriesAddress = it->getAs<types::UInt16>();
            break;
        }
        case types::InternalType::ScilabUInt32:
        {
            *_piEntriesAddress = it->getAs<types::UInt32>();
            break;
        }
        case types::InternalType::ScilabUInt64:
        {
            *_piEntriesAddress = it->getAs<types::UInt64>();
            break;
        }
        default:
        {
            *_piEntriesAddress = NULL;
            break;
        }
    }

    return 0;
}

SciErr getHypermatOfDouble(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, double** _pdblReal)
{
    SciErr sciErr = sciErrInit();
    void* entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (ret || entries == NULL || ((types::InternalType*)entries)->isDouble() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE, _("%s: Unable to get argument #%d"), "getHypermatOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    types::Double* d = (types::Double*)entries;
    *_dims = d->getDimsArray();
    *_ndims = d->getDims();
    *_pdblReal = d->get();
    return sciErr;
}

SciErr getComplexHypermatOfDouble(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    void* entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);
    if (ret || entries == NULL || ((types::InternalType*)entries)->isDouble() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE, _("%s: Unable to get argument #%d"), "getHypermatOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
        return sciErr;
    }

    types::Double* d = (types::Double*)entries;
    *_dims = d->getDimsArray();
    *_ndims = d->getDims();
    *_pdblReal = d->get();
    *_pdblImg = d->getImg();
    return sciErr;
}

SciErr getHypermatPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isPoly() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatPolyVariableName");
        return sciErr;
    }

    types::Polynom* p = (types::Polynom*)entries;
    std::wstring var = p->getVariableName();

    char* varname = wide_string_to_UTF8(var.data());
    *_piVarNameLen = static_cast<int>(strlen(varname));

    if (_pstVarName)
    {
        strcpy(_pstVarName, varname);
    }

    FREE(varname);
    return sciErr;
}

SciErr getHypermatOfPoly(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piNbCoef, double** _pdblReal)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isPoly() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfPoly");
        return sciErr;
    }

    types::Polynom* p = (types::Polynom*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();

    if (_piNbCoef == NULL)
    {
        return sciErr;
    }

    p->getSizes(_piNbCoef);

    if (_pdblReal == NULL)
    {
        return sciErr;
    }

    int size = p->getSize();
    types::SinglePoly** s = p->get();

    for (int i = 0; i < size; i++)
    {
        memcpy(_pdblReal[i], s[i]->get(), sizeof(double) * s[i]->getSize());
    }

    return sciErr;
}

SciErr getComplexHypermatOfPoly(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isPoly() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getComplexHypermatOfPoly");
        return sciErr;
    }

    types::Polynom* p = (types::Polynom*)entries;

    if (p->isComplex() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), "getComplexHypermatOfPoly");
        return sciErr;
    }

    *_dims = p->getDimsArray();
    *_ndims = p->getDims();

    if (_piNbCoef == NULL)
    {
        return sciErr;
    }

    p->getSizes(_piNbCoef);

    if (_pdblReal == NULL)
    {
        return sciErr;
    }

    int size = p->getSize();
    types::SinglePoly** s = p->get();

    for (int i = 0; i < size; i++)
    {
        memcpy(_pdblReal[i], s[i]->get(), sizeof(double) * s[i]->getSize());
        memcpy(_pdblImg[i], s[i]->getImg(), sizeof(double) * s[i]->getSize());
    }

    return sciErr;
}

SciErr getHypermatOfString(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piLength, char** _pstStrings)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isString() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfString");
        return sciErr;
    }

    types::String* p = (types::String*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    int size = p->getSize();

    if (_piLength == NULL)
    {
        return sciErr;
    }


    if (_pstStrings == NULL || *_pstStrings == NULL)
    {
        wchar_t** s = p->get();
        for (int i = 0; i < size; i++)
        {
            char* c = wide_string_to_UTF8(s[i]);
            _piLength[i] = (int)strlen(c);
            FREE(c);
        }
    }
    else
    {
        wchar_t** s = p->get();
        for (int i = 0; i < size; i++)
        {
            if (_pstStrings[i] == NULL)
            {
                addErrorMessage(&sciErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getHypermatOfString");
                return sciErr;
            }

            char* c = wide_string_to_UTF8(s[i]);
            strcpy(_pstStrings[i], c);
            FREE(c);
        }
    }

    return sciErr;
}

SciErr getHypermatOfWideString(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int* _piLength, wchar_t** _pwstStrings)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isString() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfWideString");
        return sciErr;
    }

    types::String* p = (types::String*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    int size = p->getSize();

    if (_piLength == NULL)
    {
        return sciErr;
    }


    if (_pwstStrings == NULL || *_pwstStrings == NULL)
    {
        wchar_t** s = p->get();
        for (int i = 0; i < size; i++)
        {
            _piLength[i] = (int)wcslen(s[i]);
        }
    }
    else
    {
        wchar_t** s = p->get();
        for (int i = 0; i < size; i++)
        {
            if (_pwstStrings[i] == NULL)
            {
                addErrorMessage(&sciErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getHypermatOfWideString");
                return sciErr;
            }

            wcscpy(_pwstStrings[i], s[i]);
        }
    }

    return sciErr;
}

SciErr getHypermatOfInteger8(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, char** _pcData8)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isInt8() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfInteger8");
        return sciErr;
    }

    types::Int8* p = (types::Int8*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_pcData8 = p->get();
    return sciErr;
}

SciErr getHypermatOfUnsignedInteger8(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned char** _pucData8)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isUInt8() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfUnsignedInteger8");
        return sciErr;
    }

    types::UInt8* p = (types::UInt8*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_pucData8 = p->get();
    return sciErr;
}

SciErr getHypermatOfInteger16(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, short** _psData16)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isInt16() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfInteger16");
        return sciErr;
    }

    types::Int16* p = (types::Int16*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_psData16 = p->get();
    return sciErr;
}

SciErr getHypermatOfUnsignedInteger16(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned short** _pusData16)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isUInt16() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfUnsignedInteger16");
        return sciErr;
    }

    types::UInt16* p = (types::UInt16*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_pusData16 = p->get();
    return sciErr;
}

SciErr getHypermatOfInteger32(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int** _piData32)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isInt32() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfInteger32");
        return sciErr;
    }

    types::Int32* p = (types::Int32*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_piData32 = p->get();
    return sciErr;
}

SciErr getHypermatOfUnsignedInteger32(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned int** _puiData32)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isUInt32() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfUnsignedInteger32");
        return sciErr;
    }

    types::UInt32* p = (types::UInt32*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_puiData32 = p->get();
    return sciErr;
}

SciErr getHypermatOfInteger64(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, long long** _pllData64)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isInt64() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfInteger64");
        return sciErr;
    }

    types::Int64* p = (types::Int64*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_pllData64 = p->get();
    return sciErr;
}

SciErr getHypermatOfUnsignedInteger64(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, unsigned long long** _pullData64)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isUInt64() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfUnsignedInteger64");
        return sciErr;
    }

    types::UInt64* p = (types::UInt64*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_pullData64 = p->get();
    return sciErr;
}

SciErr getHypermatOfBoolean(void* _pvCtx, int* _piAddress, int **_dims, int *_ndims, int** _piBool)
{
    SciErr sciErr = sciErrInit();
    void * entries = NULL;

    int ret = getHypermatEntries(_pvCtx, _piAddress, &entries);

    if (ret || entries == NULL || ((types::InternalType*)entries)->isBool() == false)
    {
        addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getHypermatOfBoolean");
        return sciErr;
    }

    types::Bool* p = (types::Bool*)entries;
    *_dims = p->getDimsArray();
    *_ndims = p->getDims();
    *_piBool = p->get();
    return sciErr;
}

SciErr createHypermatOfString(void *_pvCtx, int _iVar, int * _dims, int _ndims, const char* const* _pstStrings)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::String* p = new types::String(_ndims, _dims);
    int size = p->getSize();

    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    for (int i = 0; i < size; ++i)
    {
        wchar_t* w = to_wide_string(_pstStrings[i]);
        p->set(i, w);
        FREE(w);
    }

    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfPoly(void *_pvCtx, int _iVar, char* _pstVarName, int * _dims, int _ndims, const int* _piNbCoef, const double* const* _pdblReal)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    wchar_t* w = to_wide_string(_pstVarName);
    types::Polynom* p = new types::Polynom(w, _ndims, _dims, _piNbCoef);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        FREE(w);
        return sciErr;
    }

    types::SinglePoly** s = p->get();

    for (int i = 0; i < size; ++i)
    {
        s[i]->setCoef(_pdblReal[i], NULL);
    }

    out[rhs - 1] = p;
    FREE(w);
    return sciErr;
}

SciErr createComplexHypermatOfPoly(void *_pvCtx, int _iVar, char* _pstVarName, int * _dims, int _ndims, const int* _piNbCoef, const double* const* _pdblReal, const double* const* _pdblImg)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    wchar_t* w = to_wide_string(_pstVarName);
    types::Polynom* p = new types::Polynom(w, _ndims, _dims, _piNbCoef);
    p->setComplex(true);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        FREE(w);
        return sciErr;
    }

    types::SinglePoly** s = p->get();

    for (int i = 0; i < size; ++i)
    {
        s[i]->setCoef(_pdblReal[i], _pdblImg[i]);
    }

    out[rhs - 1] = p;
    FREE(w);
    return sciErr;
}

SciErr allocHypermatOfDouble(void *_pvCtx, int _iVar, int * _dims, int _ndims, double** _pdblReal)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Double* p = new types::Double(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    *_pdblReal = p->get();
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfDouble(void *_pvCtx, int _iVar, int * _dims, int _ndims, const double * _pdblReal)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Double* p = new types::Double(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pdblReal);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createComplexHypermatOfDouble(void *_pvCtx, int _iVar, int * _dims, int _ndims, const double * _pdblReal, const double * _pdblImg)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Double* p = new types::Double(_ndims, _dims, true);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pdblReal);
    p->setImg(_pdblImg);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfBoolean(void *_pvCtx, int _iVar, int * _dims, int _ndims, const int * _piBool)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Bool* p = new types::Bool(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_piBool);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfInteger8(void *_pvCtx, int _iVar, int * _dims, int _ndims, const char * _pcData8)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Int8* p = new types::Int8(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pcData8);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfUnsignedInteger8(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned char * _pucData8)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::UInt8* p = new types::UInt8(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pucData8);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfInteger16(void *_pvCtx, int _iVar, int * _dims, int _ndims, const short * _psData16)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Int16* p = new types::Int16(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_psData16);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfUnsignedInteger16(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned short * _pusData16)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::UInt16* p = new types::UInt16(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pusData16);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfInteger32(void *_pvCtx, int _iVar, int * _dims, int _ndims, const int * _piData32)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Int32* p = new types::Int32(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_piData32);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfUnsignedInteger32(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned int * _puiData32)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::UInt32* p = new types::UInt32(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_puiData32);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfInteger64(void *_pvCtx, int _iVar, int * _dims, int _ndims, const long long* _pllData64)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::Int64* p = new types::Int64(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pllData64);
    out[rhs - 1] = p;
    return sciErr;
}

SciErr createHypermatOfUnsignedInteger64(void *_pvCtx, int _iVar, int * _dims, int _ndims, const unsigned long long* _pullData64)
{
    SciErr sciErr = sciErrInit();
    types::GatewayStruct* pStr = (types::GatewayStruct*)_pvCtx;
    types::typed_list in = *pStr->m_pIn;
    types::InternalType** out = pStr->m_pOut;
    int rhs = _iVar - *getNbInputArgument(_pvCtx);

    types::UInt64* p = new types::UInt64(_ndims, _dims);

    int size = p->getSize();
    if (size == 0)
    {
        delete p;
        out[rhs - 1] = types::Double::Empty();
        return sciErr;
    }

    p->set(_pullData64);
    out[rhs - 1] = p;
    return sciErr;
}
