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

#ifndef __H5UNSIGNEDCHARDATA_HXX__
#define __H5UNSIGNEDCHARDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

class H5UnsignedCharData : public H5BasicData<unsigned char>
{

public:

    H5UnsignedCharData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5UnsignedCharData()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        os << (int)static_cast<unsigned char *>(getData())[pos];
    }
};
}

#endif // __H5UNSIGNEDCHARDATA_HXX__
