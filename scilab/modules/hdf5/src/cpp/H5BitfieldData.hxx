/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __H5BITFIELDDATA_HXX__
#define __H5BITFIELDDATA_HXX__

#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5Bitfield1Data : public H5BasicData<unsigned char>
{

public:

    H5Bitfield1Data(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5Bitfield1Data()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        os << "0x" << std::hex << std::setfill('0') << std::setw(2) << (int)static_cast<unsigned char * >(getData())[pos];
    }
};

class H5Bitfield2Data : public H5BasicData<unsigned short>
{

public:

    H5Bitfield2Data(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned short * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned short>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5Bitfield2Data()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        const unsigned short x = static_cast<unsigned short * >(getData())[pos];
        os << std::hex << std::setfill('0') << std::setw(2) << (x & 0xFF) << ":" << ((x >> 8) & 0xFF);
    }
};

class H5Bitfield4Data : public H5BasicData<unsigned int>
{

public:

    H5Bitfield4Data(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned int * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned int>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5Bitfield4Data()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        const unsigned int x = static_cast<unsigned int * >(getData())[pos];
        os << std::hex << std::setfill('0') << std::setw(2) << (x & 0xFF) << ":" << ((x >> 8) & 0xFF) << ":" << ((x >> 16) & 0xFF) << ":" << ((x >> 24) & 0xFF);
    }
};

#ifdef __SCILAB_INT64__

class H5Bitfield8Data : public H5BasicData<unsigned long>
{

public:

    H5Bitfield8Data(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned long * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned long>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5Bitfield8Data()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        const unsigned long long x = static_cast<unsigned long long * >(getData())[pos];
        os << std::hex << std::setfill('0') << std::setw(2) << (x & 0xFF) << ":" << ((x >> 8) & 0xFF) << ":" << ((x >> 16) & 0xFF) << ":" << ((x >> 24) & 0xFF) << ":" << ((x >> 32) & 0xFF) << ":" << ((x >> 40) & 0xFF) << ":" << ((x >> 48) & 0xFF) << ":" << ((x >> 56) & 0xFF);
    }
};

#endif // __SCILAB_INT64__
}

#endif // __H5BITFIELDDATA_HXX__
