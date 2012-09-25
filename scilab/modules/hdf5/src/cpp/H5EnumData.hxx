/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __H5ENUMDATA_HXX__
#define __H5ENUMDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

class H5EnumData : public H5BasicData<char>
{

public:

    H5EnumData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride = -1, const size_t _offset = 0, const bool _dataOwner = true) : H5BasicData(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {
        //dims[ndims - 1] = dataSize;
    }

    virtual ~H5EnumData()
    {

    }
};
}

#endif // __H5ENUMDATA_HXX__
