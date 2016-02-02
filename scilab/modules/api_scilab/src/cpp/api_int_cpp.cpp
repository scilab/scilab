/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "api_int.hxx"

namespace api_scilab
{
Int8::Int8(char _cVal)
{
    data = new types::Int8(_cVal);
    created = true;
}

Int8::Int8(int _iRows, int _iCols)
{
    data = new types::Int8(_iRows, _iCols);
    created = true;
}

Int8::Int8(int _iDims, int* _piDims)
{
    data = new types::Int8(_iDims, _piDims);
    created = true;
}

Int8::Int8(int _iRows, int _iCols, char** _pcData)
{
    data = new types::Int8(_iRows, _iCols, _pcData);
    created = true;
}

Int8* Int8::getAsInt8(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isInt8())
    {
        return new Int8(*_pIT->getAs<types::Int8>());
    }

    return NULL;
}

UInt8::UInt8(unsigned char _ucVal)
{
    data = new types::UInt8(_ucVal);
    created = true;
}

UInt8::UInt8(int _iRows, int _iCols)
{
    data = new types::UInt8(_iRows, _iCols);
    created = true;
}

UInt8::UInt8(int _iDims, int* _piDims)
{
    data = new types::UInt8(_iDims, _piDims);
    created = true;
}

UInt8::UInt8(int _iRows, int _iCols, unsigned char** _pucData)
{
    data = new types::UInt8(_iRows, _iCols, _pucData);
    created = true;
}

UInt8* UInt8::getAsUInt8(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isUInt8())
    {
        return new UInt8(*_pIT->getAs<types::UInt8>());
    }

    return NULL;
}

Int16::Int16(short _sVal)
{
    data = new types::Int16(_sVal);
    created = true;
}

Int16::Int16(int _iRows, int _iCols)
{
    data = new types::Int16(_iRows, _iCols);
    created = true;
}

Int16::Int16(int _iDims, int* _piDims)
{
    data = new types::Int16(_iDims, _piDims);
    created = true;
}

Int16::Int16(int _iRows, int _iCols, short** _psData)
{
    data = new types::Int16(_iRows, _iCols, _psData);
    created = true;
}

Int16* Int16::getAsInt16(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isInt16())
    {
        return new Int16(*_pIT->getAs<types::Int16>());
    }

    return NULL;
}

UInt16::UInt16(unsigned short _usVal)
{
    data = new types::UInt16(_usVal);
    created = true;
}

UInt16::UInt16(int _iRows, int _iCols)
{
    data = new types::UInt16(_iRows, _iCols);
    created = true;
}

UInt16::UInt16(int _iDims, int* _piDims)
{
    data = new types::UInt16(_iDims, _piDims);
    created = true;
}

UInt16::UInt16(int _iRows, int _iCols, unsigned short** _pusData)
{
    data = new types::UInt16(_iRows, _iCols, _pusData);
    created = true;
}

UInt16* UInt16::getAsUInt16(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isUInt16())
    {
        return new UInt16(*_pIT->getAs<types::UInt16>());
    }

    return NULL;
}

Int32::Int32(int _iVal)
{
    data = new types::Int32(_iVal);
    created = true;
}

Int32::Int32(int _iRows, int _iCols)
{
    data = new types::Int32(_iRows, _iCols);
    created = true;
}

Int32::Int32(int _iDims, int* _piDims)
{
    data = new types::Int32(_iDims, _piDims);
    created = true;
}

Int32::Int32(int _iRows, int _iCols, int** _piData)
{
    data = new types::Int32(_iRows, _iCols, _piData);
    created = true;
}

Int32* Int32::getAsInt32(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isInt32())
    {
        return new Int32(*_pIT->getAs<types::Int32>());
    }

    return NULL;
}

UInt32::UInt32(unsigned int _uiVal)
{
    data = new types::UInt32(_uiVal);
    created = true;
}

UInt32::UInt32(int _iRows, int _iCols)
{
    data = new types::UInt32(_iRows, _iCols);
    created = true;
}

UInt32::UInt32(int _iDims, int* _piDims)
{
    data = new types::UInt32(_iDims, _piDims);
    created = true;
}

UInt32::UInt32(int _iRows, int _iCols, unsigned int** _puiData)
{
    data = new types::UInt32(_iRows, _iCols, _puiData);
    created = true;
}

UInt32* UInt32::getAsUInt32(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isUInt32())
    {
        return new UInt32(*_pIT->getAs<types::UInt32>());
    }

    return NULL;
}

Int64::Int64(long long _llVal)
{
    data = new types::Int64(_llVal);
    created = true;
}

Int64::Int64(int _iRows, int _iCols)
{
    data = new types::Int64(_iRows, _iCols);
    created = true;
}

Int64::Int64(int _iDims, int* _piDims)
{
    data = new types::Int64(_iDims, _piDims);
    created = true;
}

Int64::Int64(int _iRows, int _iCols, long long** _pllData)
{
    data = new types::Int64(_iRows, _iCols, _pllData);
    created = true;
}

Int64* Int64::getAsInt64(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isInt64())
    {
        return new Int64(*_pIT->getAs<types::Int64>());
    }

    return NULL;
}

UInt64::UInt64(unsigned long long _ullVal)
{
    data = new types::UInt64(_ullVal);
    created = true;
}

UInt64::UInt64(int _iRows, int _iCols)
{
    data = new types::UInt64(_iRows, _iCols);
    created = true;
}

UInt64::UInt64(int _iDims, int* _piDims)
{
    data = new types::UInt64(_iDims, _piDims);
    created = true;
}

UInt64::UInt64(int _iRows, int _iCols, unsigned long long** _pullData)
{
    data = new types::UInt64(_iRows, _iCols, _pullData);
    created = true;
}

UInt64* UInt64::getAsUInt64(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isUInt64())
    {
        return new UInt64(*_pIT->getAs<types::UInt64>());
    }

    return NULL;
}
}
