/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
        // Save 'os' format, fill it and restore
        char oldChar = os.fill();
        std::ios oldState(nullptr);
        oldState.copyfmt(os);
        os << "0x" << std::hex << std::setfill('0') << std::setw(2) << (int)static_cast<unsigned char * >(getData())[pos];
        os.copyfmt(oldState);
        os.fill(oldChar);
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
        // Save 'os' format, fill it and restore
        const unsigned short x = static_cast<unsigned short * >(getData())[pos];
        char oldChar = os.fill();
        std::ios oldState(nullptr);
        oldState.copyfmt(os);
        os << std::hex << std::setfill('0') << std::setw(2) << (x & 0xFF) << ":" << ((x >> 8) & 0xFF);
        os.copyfmt(oldState);
        os.fill(oldChar);
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
        // Save 'os' format, fill it and restore
        const unsigned int x = static_cast<unsigned int * >(getData())[pos];
        char oldChar = os.fill();
        std::ios oldState(nullptr);
        oldState.copyfmt(os);
        os << std::hex << std::setfill('0') << std::setw(2) << (x & 0xFF) << ":" << ((x >> 8) & 0xFF) << ":" << ((x >> 16) & 0xFF) << ":" << ((x >> 24) & 0xFF);
        os.copyfmt(oldState);
        os.fill(oldChar);
    }
};

#ifdef __SCILAB_INT64__

class H5Bitfield8Data : public H5BasicData<unsigned long long>
{

public:

    H5Bitfield8Data(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned long long * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned long long>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5Bitfield8Data()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        // Save 'os' format, fill it and restore
        const unsigned long long x = static_cast<unsigned long long * >(getData())[pos];
        char oldChar = os.fill();
        std::ios oldState(nullptr);
        oldState.copyfmt(os);
        os << std::hex << std::setfill('0') << std::setw(2) << (x & 0xFF) << ":" << ((x >> 8) & 0xFF) << ":" << ((x >> 16) & 0xFF) << ":" << ((x >> 24) & 0xFF) << ":" << ((x >> 32) & 0xFF) << ":" << ((x >> 40) & 0xFF) << ":" << ((x >> 48) & 0xFF) << ":" << ((x >> 56) & 0xFF);
        os.copyfmt(oldState);
        os.fill(oldChar);
    }
};

#endif // __SCILAB_INT64__
}

#endif // __H5BITFIELDDATA_HXX__
