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

#ifndef __H5VLENDATA_HXX__
#define __H5VLENDATA_HXX__

#include "H5Data.hxx"

namespace org_modules_hdf5
{

template <typename T>
class H5VlenData : public H5Data
{

public:

    H5VlenData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, hvl_t * _data, const hsize_t _stride = -1, const size_t _offset = 0, const bool _dataOwner = true) : H5Data(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {

    }

    virtual ~H5VlenData()
    {
        for (unsigned int i = 0; i < totalSize; i++)
        {
            free(data[i].p);
        }
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        const hvl_t & x = *static_cast<hvl_t *>(getData())[pos];

        os << "(";
        for (unsigned int i = 0; i < x.len - 1; i++)
        {
            os << static_cast<T *>(x.p)[i] << ", ";
        }
        os << static_cast<T *>(x.p)[x.len - 1] << ")";
    }

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0) const
    {
        SciErr err;
        unsigned char * newData = 0;

        if (ndims == 0)
        {
            alloc(pvApiCtx, lhsPosition, 1, dataSize, parentList, listPosition, &newData);
            copyData(newData);
        }
        else if (ndims == 1)
        {
            alloc(pvApiCtx, lhsPosition, *dims, dataSize, parentList, listPosition, &newData);
            copyData(newData);
        }
        else
        {
            int * list = getHypermatrix(pvApiCtx, lhsPosition, parentList, listPosition);
            hsize_t * _dims = new hsize_t[ndims + 1];
            memcpy(_dims, dims, ndims * sizeof(hsize_t));
            _dims[ndims] = (hsize_t)dataSize;

            try
            {
                alloc(pvApiCtx, lhsPosition, 1, totalSize * dataSize, list, 3, &newData);
                H5DataConverter::C2FHypermatrix(ndims + 1, _dims, totalSize * dataSize, static_cast<unsigned char *>(getData()), newData);
            }
            catch (const H5Exception & e)
            {
                delete[] _dims;
                throw;
            }

            delete[] _dims;
        }
    }

    class VlenInfo
    {
        const hid_t space;
        const hid_t type;
        const bool isString;

    public:
        VlenInfo(const hid_t _space, const hid_t _type, const bool _isString) : space(_space), type(_type), isString(_isString) { }

        virtual ~VlenInfo()
        {
            if (space)
            {
                H5Sclose(space);
            }
            if (type)
            {
                H5Tclose(type);
            }
        }

        hid_t getSpace() const
        {
            return space;
        }
        hid_t getType() const
        {
            return type;
        }
        bool isString() const
        {
            return isString;
        }
    };

};
}

#endif // __H5VLENDATA_HXX__
