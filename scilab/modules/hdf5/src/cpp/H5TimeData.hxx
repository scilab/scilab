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

#ifndef __H5TIMEDATA_HXX__
#define __H5TIMEDATA_HXX__

#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5TimeData : public H5BasicData<char>
{

public:

    H5TimeData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {
    }

    virtual ~H5TimeData()
    {

    }
};
}

#endif // __H5TIMEDATA_HXX__
