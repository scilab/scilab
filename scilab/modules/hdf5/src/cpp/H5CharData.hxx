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

#ifndef __H5CHARDATA_HXX__
#define __H5CHARDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

class H5CharData : public H5BasicData<char>
{

public:

    H5CharData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5CharData()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        os << (int)static_cast<char *>(getData())[pos];
    }
};
}

#endif // __H5CHARDATA_HXX__
