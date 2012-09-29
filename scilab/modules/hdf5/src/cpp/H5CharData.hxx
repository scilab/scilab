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

#ifndef __H5CHARDATA_HXX__
#define __H5CHARDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

class H5CharData : public H5BasicData<char>
{

public:

    H5CharData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, const hsize_t _arank, const hsize_t * _adims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData(_parent, _totalSize, _dataSize, _ndims, _dims, _arank, _adims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5CharData()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        if (adims)
        {
            os << "[ ";
            const hsize_t _pos = pos * dataSize;
            for (unsigned int i = 0; i < atotalSize - 1; i++)
            {
                os << (int)static_cast<char *>(getData())[_pos + i] << ", ";
            }
            os << (int)static_cast<char *>(getData())[_pos + atotalSize - 1] << " ]";
        }
        else
        {
            os << (int)static_cast<char *>(getData())[pos];
        }
    }
};
}

#endif // __H5CHARDATA_HXX__
