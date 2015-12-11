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

#ifndef __H5OPAQUEDATA_HXX__
#define __H5OPAQUEDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

class H5OpaqueData : public H5BasicData<unsigned char>
{

public:

    H5OpaqueData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, unsigned char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<unsigned char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5OpaqueData()
    {

    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        const unsigned char * x = &static_cast<unsigned char * >(getData())[pos * dataSize];
        for (unsigned int i = 0; i < dataSize - 1; i++)
        {
            os << std::hex << std::setfill('0') << std::setw(2) << (int)x[i] << ":";
        }
        os << std::hex << std::setfill('0') << std::setw(2) << (int)x[dataSize - 1];
    }

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const
    {
        unsigned char * newData = 0;

        if (ndims == 0)
        {
            alloc(pvApiCtx, lhsPosition, 1, (int)dataSize, parentList, listPosition, &newData);
            copyData(newData);
        }
        else if (ndims == 1)
        {
            alloc(pvApiCtx, lhsPosition, (int)*dims, (int)dataSize, parentList, listPosition, &newData);
            copyData(newData);
        }
        else
        {
            int * list = getHypermatrix(pvApiCtx, lhsPosition, parentList, listPosition, flip);
            hsize_t * _dims = new hsize_t[ndims + 1];
            memcpy(_dims, dims, ndims * sizeof(hsize_t));
            _dims[ndims] = (hsize_t)dataSize;

            try
            {
                alloc(pvApiCtx, lhsPosition, (int)(totalSize * dataSize), 1, list, 3, &newData);
                H5DataConverter::C2FHypermatrix((int)ndims + 1, _dims, (int)(totalSize * dataSize), static_cast<unsigned char *>(getData()), newData, flip);
            }
            catch (const H5Exception & /*e*/)
            {
                delete[] _dims;
                throw;
            }

            delete[] _dims;
        }
    }
};
}

#endif // __H5OPAQUEDATA_HXX__
