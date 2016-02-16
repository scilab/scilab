/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/

#include "gatewaystruct.hxx"
#include "int.hxx"

extern "C"
{
#include "api_scilab.h"
}

int API_PROTO(getIntegerPrecision)(scilabEnv env, scilabVar var)
{
    switch (((types::InternalType*)var)->getType())
    {
        case types::InternalType::ScilabInt8:
            return SCI_INT8;
        case types::InternalType::ScilabUInt8:
            return SCI_UINT8;
        case types::InternalType::ScilabInt16:
            return SCI_INT16;
        case types::InternalType::ScilabUInt16:
            return SCI_UINT16;
        case types::InternalType::ScilabInt32:
            return SCI_INT32;
        case types::InternalType::ScilabUInt32:
            return SCI_UINT32;
        case types::InternalType::ScilabInt64:
            return SCI_INT64;
        case types::InternalType::ScilabUInt64:
            return SCI_UINT64;
        default:
            return 0;
    }
}

int API_PROTO(isInt8)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt8() ? 1 : 0;
}

int API_PROTO(isInt16)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt16() ? 1 : 0;
}

int API_PROTO(isInt32)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt32() ? 1 : 0;
}

int API_PROTO(isInt64)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt64() ? 1 : 0;
}

int API_PROTO(isUnsignedInt8)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isUInt8() ? 1 : 0;
}

int API_PROTO(isUnsignedInt16)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isInt16() ? 1 : 0;
}

int API_PROTO(isUnsignedInt32)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isUInt32() ? 1 : 0;
}

int API_PROTO(isUnsignedInt64)(scilabEnv env, scilabVar var)
{
    return ((types::InternalType*)var)->isUInt64() ? 1 : 0;
}

scilabVar API_PROTO(createInteger8)(scilabEnv env, char val)
{
    return (scilabVar)new types::Int8(val);
}

scilabVar API_PROTO(createInteger16)(scilabEnv env, short val)
{
    return (scilabVar)new types::Int16(val);
}

scilabVar API_PROTO(createInteger32)(scilabEnv env, int val)
{
    return (scilabVar)new types::Int32(val);
}

scilabVar API_PROTO(createInteger64)(scilabEnv env, long long val)
{
    return (scilabVar)new types::Int64(val);
}

scilabVar API_PROTO(createUnsignedInteger8)(scilabEnv env, unsigned char val)
{
    return (scilabVar)new types::UInt8(val);
}

scilabVar API_PROTO(createUnsignedInteger16)(scilabEnv env, unsigned short val)
{
    return (scilabVar)new types::UInt16(val);
}

scilabVar API_PROTO(createUnsignedInteger32)(scilabEnv env, unsigned int val)
{
    return (scilabVar)new types::UInt32(val);
}

scilabVar API_PROTO(createUnsignedInteger64)(scilabEnv env, unsigned long long val)
{
    return (scilabVar)new types::UInt64(val);
}

scilabVar API_PROTO(createIntegerMatrix)(scilabEnv env, int prec, int dim, const int* dims)
{
    switch (prec)
    {
        case SCI_INT8:
            return scilab_createInteger8Matrix(env, dim, dims);
        case SCI_INT16:
            return scilab_createInteger16Matrix(env, dim, dims);
        case SCI_INT32:
            return scilab_createInteger32Matrix(env, dim, dims);
        case SCI_INT64:
            return scilab_createInteger64Matrix(env, dim, dims);
        case SCI_UINT8:
            return scilab_createUnsignedInteger8Matrix(env, dim, dims);
        case SCI_UINT16:
            return scilab_createUnsignedInteger16Matrix(env, dim, dims);
        case SCI_UINT32:
            return scilab_createUnsignedInteger32Matrix(env, dim, dims);
        case SCI_UINT64:
            return scilab_createUnsignedInteger64Matrix(env, dim, dims);
    }

    return NULL;
}

scilabVar API_PROTO(createInteger8Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createIntegerMatrix8", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createIntegerMatrix8", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::Int8(dim, dims);
}

scilabVar API_PROTO(createInteger16Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createIntegerMatrix16", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createIntegerMatrix16", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::Int16(dim, dims);
}

scilabVar API_PROTO(createInteger32Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createIntegerMatrix32", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createIntegerMatrix32", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::Int32(dim, dims);
}

scilabVar API_PROTO(createInteger64Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createIntegerMatrix64", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createIntegerMatrix64", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::Int64(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger8Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createUnsignedIntegerMatrix8", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createUnsignedIntegerMatrix8", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::UInt8(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger16Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createUnsignedIntegerMatrix16", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createUnsignedIntegerMatrix16", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::UInt16(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger32Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createUnsignedIntegerMatrix32", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createUnsignedIntegerMatrix32", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::UInt32(dim, dims);
}

scilabVar API_PROTO(createUnsignedInteger64Matrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createUnsignedIntegerMatrix64", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createUnsignedIntegerMatrix64", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif

    return (scilabVar)new types::UInt64(dim, dims);
}

scilabVar API_PROTO(createIntegerMatrix2d)(scilabEnv env, int prec, int row, int col)
{
    int dims[2] = {row, col};
    return scilab_createIntegerMatrix(env, prec, 2, dims);
}

scilabVar API_PROTO(createInteger8Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_INT8, row, col);
}

scilabVar API_PROTO(createInteger16Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_INT16, row, col);
}

scilabVar API_PROTO(createInteger32Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_INT32, row, col);
}

scilabVar API_PROTO(createInteger64Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_INT64, row, col);
}

scilabVar API_PROTO(createUnsignedInteger8Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_UINT8, row, col);
}

scilabVar API_PROTO(createUnsignedInteger16Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_UINT16, row, col);
}

scilabVar API_PROTO(createUnsignedInteger32Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_UINT32, row, col);
}

scilabVar API_PROTO(createUnsignedInteger64Matrix2d)(scilabEnv env, int row, int col)
{
    return scilab_createIntegerMatrix2d(env, SCI_UINT64, row, col);
}

scilabStatus API_PROTO(getInteger8)(scilabEnv env, scilabVar var, char* val)
{
    types::Int8* i = (types::Int8*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt8() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getInteger8", _W("var must be a scalar int8 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getInteger16)(scilabEnv env, scilabVar var, short* val)
{
    types::Int16* i = (types::Int16*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt16() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getInteger16", _W("var must be a scalar int16 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getInteger32)(scilabEnv env, scilabVar var, int* val)
{
    types::Int32* i = (types::Int32*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt32() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getInteger32", _W("var must be a scalar int32 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getInteger64)(scilabEnv env, scilabVar var, long long* val)
{
    types::Int64* i = (types::Int64*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt64() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getInteger64", _W("var must be a scalar int64 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger8)(scilabEnv env, scilabVar var, unsigned char* val)
{
    types::UInt8* i = (types::UInt8*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt8() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger8", _W("var must be a scalar uint8 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger16)(scilabEnv env, scilabVar var, unsigned short* val)
{
    types::UInt16* i = (types::UInt16*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt16() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger16", _W("var must be a scalar uint16 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger32)(scilabEnv env, scilabVar var, unsigned int* val)
{
    types::UInt32* i = (types::UInt32*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt32() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger32", _W("var must be a scalar uint32 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger64)(scilabEnv env, scilabVar var, unsigned long long* val)
{
    types::UInt64* i = (types::UInt64*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt64() == false || i->isScalar() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger64", _W("var must be a scalar uint64 variable"));
        return STATUS_ERROR;
    }
#endif
    *val = i->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getIntegerArray)(scilabEnv env, scilabVar var, void** vals)
{
    switch (((types::InternalType*)var)->getType())
    {
        case types::InternalType::ScilabInt8:
            return scilab_getInteger8Array(env, var, (char**)vals);
        case types::InternalType::ScilabInt16:
            return scilab_getInteger16Array(env, var, (short**)vals);
        case types::InternalType::ScilabInt32:
            return scilab_getInteger32Array(env, var, (int**)vals);
        case types::InternalType::ScilabInt64:
            return scilab_getInteger64Array(env, var, (long long**)vals);
        case types::InternalType::ScilabUInt8:
            return scilab_getUnsignedInteger8Array(env, var, (unsigned char**)vals);
        case types::InternalType::ScilabUInt16:
            return scilab_getUnsignedInteger16Array(env, var, (unsigned short**)vals);
        case types::InternalType::ScilabUInt32:
            return scilab_getUnsignedInteger32Array(env, var, (unsigned int**)vals);
        case types::InternalType::ScilabUInt64:
            return scilab_getUnsignedInteger64Array(env, var, (unsigned long long**)vals);
        default:
            return STATUS_ERROR;
    }
}

scilabStatus API_PROTO(getInteger8Array)(scilabEnv env, scilabVar var, char** vals)
{
    types::Int8* i = (types::Int8*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt8() == false)
    {
        scilab_setInternalError(env, L"getInteger8Array", _W("var must be a int8 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getInteger16Array)(scilabEnv env, scilabVar var, short** vals)
{
    types::Int16* i = (types::Int16*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt16() == false)
    {
        scilab_setInternalError(env, L"getInteger16Array", _W("var must be a int16 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getInteger32Array)(scilabEnv env, scilabVar var, int** vals)
{
    types::Int32* i = (types::Int32*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt32() == false)
    {
        scilab_setInternalError(env, L"getInteger32Array", _W("var must be a int32 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getInteger64Array)(scilabEnv env, scilabVar var, long long** vals)
{
    types::Int64* i = (types::Int64*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt64() == false)
    {
        scilab_setInternalError(env, L"getInteger64Array", _W("var must be a int64 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger8Array)(scilabEnv env, scilabVar var, unsigned char** vals)
{
    types::UInt8* i = (types::UInt8*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt8() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger8Array", _W("var must be a uint8 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger16Array)(scilabEnv env, scilabVar var, unsigned short** vals)
{
    types::UInt16* i = (types::UInt16*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt16() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger16Array", _W("var must be a uint16 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger32Array)(scilabEnv env, scilabVar var, unsigned int** vals)
{
    types::UInt32* i = (types::UInt32*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt32() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger32Array", _W("var must be a uint32 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getUnsignedInteger64Array)(scilabEnv env, scilabVar var, unsigned long long** vals)
{
    types::UInt64* i = (types::UInt64*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt64() == false)
    {
        scilab_setInternalError(env, L"getUnsignedInteger64Array", _W("var must be a uint64 variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = i->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(setIntegerArray)(scilabEnv env, scilabVar var, const void* vals)
{
    switch (((types::InternalType*)var)->getType())
    {
        case types::InternalType::ScilabInt8:
            return scilab_setInteger8Array(env, var, (const char*)vals);
        case types::InternalType::ScilabInt16:
            return scilab_setInteger16Array(env, var, (const short*)vals);
        case types::InternalType::ScilabInt32:
            return scilab_setInteger32Array(env, var, (const int*)vals);
        case types::InternalType::ScilabInt64:
            return scilab_setInteger64Array(env, var, (const long long*)vals);
        case types::InternalType::ScilabUInt8:
            return scilab_setUnsignedInteger8Array(env, var, (const unsigned char*)vals);
        case types::InternalType::ScilabUInt16:
            return scilab_setUnsignedInteger16Array(env, var, (const unsigned short*)vals);
        case types::InternalType::ScilabUInt32:
            return scilab_setUnsignedInteger32Array(env, var, (const unsigned int*)vals);
        case types::InternalType::ScilabUInt64:
            return scilab_setUnsignedInteger64Array(env, var, (const unsigned long long*)vals);
        default:
            return STATUS_ERROR;

    }
}

scilabStatus API_PROTO(setInteger8Array)(scilabEnv env, scilabVar var, const char* vals)
{
    types::Int8* i = (types::Int8*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt8() == false)
    {
        scilab_setInternalError(env, L"setInteger8Array", _W("var must be a int8 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setInteger16Array)(scilabEnv env, scilabVar var, const short* vals)
{
    types::Int16* i = (types::Int16*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt16() == false)
    {
        scilab_setInternalError(env, L"setInteger16Array", _W("var must be a int16 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setInteger32Array)(scilabEnv env, scilabVar var, const int* vals)
{
    types::Int32* i = (types::Int32*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt32() == false)
    {
        scilab_setInternalError(env, L"setInteger32Array", _W("var must be a int32 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setInteger64Array)(scilabEnv env, scilabVar var, const long long* vals)
{
    types::Int64* i = (types::Int64*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isInt32() == false)
    {
        scilab_setInternalError(env, L"setInteger64Array", _W("var must be a int32 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setUnsignedInteger8Array)(scilabEnv env, scilabVar var, const unsigned char* vals)
{
    types::UInt8* i = (types::UInt8*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt8() == false)
    {
        scilab_setInternalError(env, L"setUnsignedInteger8Array", _W("var must be a uint8 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setUnsignedInteger16Array)(scilabEnv env, scilabVar var, const unsigned short* vals)
{
    types::UInt16* i = (types::UInt16*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt16() == false)
    {
        scilab_setInternalError(env, L"setUnsignedInteger16Array", _W("var must be a uint16 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setUnsignedInteger32Array)(scilabEnv env, scilabVar var, const unsigned int* vals)
{
    types::UInt32* i = (types::UInt32*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt32() == false)
    {
        scilab_setInternalError(env, L"setUnsignedInteger32Array", _W("var must be a uint32 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}

scilabStatus API_PROTO(setUnsignedInteger64Array)(scilabEnv env, scilabVar var, const unsigned long long* vals)
{
    types::UInt64* i = (types::UInt64*)var;
#ifdef __API_SCILAB_SAFE__
    if (i->isUInt64() == false)
    {
        scilab_setInternalError(env, L"setUnsignedInteger64Array", _W("var must be a uint64 variable"));
        return STATUS_ERROR;
    }
#endif
    i->set(vals);
    return STATUS_OK;
}
