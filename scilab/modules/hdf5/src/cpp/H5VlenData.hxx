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

#ifndef __H5VLENDATA_HXX__
#define __H5VLENDATA_HXX__

#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5VlenData : public H5BasicData<char>
{

    const hsize_t * cumprod;
    hid_t type;
    hsize_t baseSize;

public:

    H5VlenData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, hid_t vlenType, const hsize_t stride, const size_t offset, const bool _dataOwner);

    virtual ~H5VlenData();

    virtual H5Object & getData(const unsigned int size, const unsigned int * index) const;

    virtual bool isVlen() const;

    virtual std::string toString(const unsigned int indentLevel) const;

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const;

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const;

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const;

    virtual bool mustDelete() const
    {
        return false;
    }
};
}

#endif // __H5VLENDATA_HXX__
