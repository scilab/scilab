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

#ifdef _MSC_VER
#pragma warning( disable : 4275 )
#endif

#ifndef __API_INT_HXX__
#define __API_INT_HXX__

#include "int.hxx"
#include "api_variable.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP Int8 : public MatrixT<char, types::Int8>
{
private :
    //internal constructor for existing variable
    Int8(types::Int8& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    Int8(char _cVal);
    Int8(int _iRows, int _iCols);
    Int8(int _iDims, int* _piDims);
    Int8(int _iRows, int _iCols, char** _pcData);
    virtual ~Int8()
    {
        if (created)
        {
            delete data;
        }
    };

    static Int8* getAsInt8(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabInt8;
    }
};

class API_SCILAB_IMPEXP UInt8 : public MatrixT<unsigned char, types::UInt8>
{
private :
    //internal constructor for existing variable
    UInt8(types::UInt8& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    UInt8(unsigned char _ucVal);
    UInt8(int _iRows, int _iCols);
    UInt8(int _iDims, int* _piDims);
    UInt8(int _iRows, int _iCols, unsigned char** _pucData);
    virtual ~UInt8()
    {
        if (created)
        {
            delete data;
        }
    };

    static UInt8* getAsUInt8(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabUInt8;
    }
};

class API_SCILAB_IMPEXP Int16 : public MatrixT<short, types::Int16>
{
private :
    //internal constructor for existing variable
    Int16(types::Int16& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    Int16(short _sVal);
    Int16(int _iRows, int _iCols);
    Int16(int _iDims, int* _piDims);
    Int16(int _iRows, int _iCols, short** _psData);
    virtual ~Int16()
    {
        if (created)
        {
            delete data;
        }
    };

    static Int16* getAsInt16(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabInt16;
    }
};

class API_SCILAB_IMPEXP UInt16 : public MatrixT<unsigned short, types::UInt16>
{
private :
    //internal constructor for existing variable
    UInt16(types::UInt16& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    UInt16(unsigned short _usVal);
    UInt16(int _iRows, int _iCols);
    UInt16(int _iDims, int* _piDims);
    UInt16(int _iRows, int _iCols, unsigned short** _pusData);
    virtual ~UInt16()
    {
        if (created)
        {
            delete data;
        }
    };

    static UInt16* getAsUInt16(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabUInt16;
    }
};

class API_SCILAB_IMPEXP Int32 : public MatrixT<int, types::Int32>
{
private :
    //internal constructor for existing variable
    Int32(types::Int32& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    Int32(int _iVal);
    Int32(int _iRows, int _iCols);
    Int32(int _iDims, int* _piDims);
    Int32(int _iRows, int _iCols, int** _piData);
    virtual ~Int32()
    {
        if (created)
        {
            delete data;
        }
    };

    static Int32* getAsInt32(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabInt32;
    }
};

class API_SCILAB_IMPEXP UInt32 : public MatrixT<unsigned int, types::UInt32>
{
private :
    //internal constructor for existing variable
    UInt32(types::UInt32& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    UInt32(unsigned int _uiVal);
    UInt32(int _iRows, int _iCols);
    UInt32(int _iDims, int* _piDims);
    UInt32(int _iRows, int _iCols, unsigned int** _puiData);
    virtual ~UInt32()
    {
        if (created)
        {
            delete data;
        }
    };

    static UInt32* getAsUInt32(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabUInt32;
    }
};

class API_SCILAB_IMPEXP Int64 : public MatrixT<long long, types::Int64>
{
private :
    //internal constructor for existing variable
    Int64(types::Int64& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    Int64(long long _llVal);
    Int64(int _iRows, int _iCols);
    Int64(int _iDims, int* _piDims);
    Int64(int _iRows, int _iCols, long long** _pllData);
    virtual ~Int64()
    {
        if (created)
        {
            delete data;
        }
    };

    static Int64* getAsInt64(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabInt64;
    }
};

class API_SCILAB_IMPEXP UInt64 : public MatrixT<unsigned long long, types::UInt64>
{
private :
    //internal constructor for existing variable
    UInt64(types::UInt64& _pdbl)
    {
        data = &_pdbl;
        created = false;
    }

public :
    UInt64(unsigned long long _ullVal);
    UInt64(int _iRows, int _iCols);
    UInt64(int _iDims, int* _piDims);
    UInt64(int _iRows, int _iCols, unsigned long long** _pullData);
    virtual ~UInt64()
    {
        if (created)
        {
            delete data;
        }
    };

    static UInt64* getAsUInt64(types::InternalType* _pdbl);

    types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabUInt64;
    }
};

}

#ifdef _MSC_VER
template class api_scilab::MatrixT<char, types::Int8>;
template class api_scilab::MatrixT<unsigned char, types::UInt8>;

template class api_scilab::MatrixT<short, types::Int16>;
template class api_scilab::MatrixT<unsigned short, types::UInt16>;

template class api_scilab::MatrixT<int, types::Int32>;
template class api_scilab::MatrixT<unsigned int, types::UInt32>;

template class api_scilab::MatrixT<long long, types::Int64>;
template class api_scilab::MatrixT<unsigned long long, types::UInt64>;
#endif

#endif /* !__API_DOUBLE_HXX__ */