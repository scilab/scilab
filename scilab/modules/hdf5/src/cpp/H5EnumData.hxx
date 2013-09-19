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

#ifndef __H5ENUMDATA_HXX__
#define __H5ENUMDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

class H5EnumData : public H5BasicData<unsigned int>
{

    const int nmembers;
    const std::string * names;

public:

    H5EnumData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned int * _data, const int _nmembers, const std::string * _names, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned int>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), nmembers(_nmembers), names(_names)
    {

    }

    virtual ~H5EnumData()
    {
        if (names)
        {
            delete[] names;
        }
    }
};
}

#endif // __H5ENUMDATA_HXX__
