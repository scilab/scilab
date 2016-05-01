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

#ifndef __H5TRANSFORMEDDATA_HXX__
#define __H5TRANSFORMEDDATA_HXX__

#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

template <typename T, typename U>
class H5TransformedData : public H5Data
{

protected:

    U * transformedData;

public:

    H5TransformedData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, T * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5Data(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner)
    {
        transformedData = new U[totalSize];

        if (stride == 0)
        {
            for (unsigned int i = 0; i < (unsigned int)totalSize; i++)
            {
                transformedData[i] = (U)(_data[i]);
            }
        }
        else
        {
            char * __data = (char *)_data;
            for (unsigned int i = 0; i < (unsigned int)totalSize; i++)
            {
                transformedData[i] = (U)(*((T *)(__data + offset)));
                __data += stride;
            }
        }
    }

    virtual ~H5TransformedData()
    {
        delete[] transformedData;
    }

    virtual void * getData() const
    {
        return transformedData;
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        os << transformedData[pos];
    }

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const
    {
        U * newData = 0;

        if (ndims == 0)
        {
            H5BasicData<U>::create(pvApiCtx, lhsPosition, 1, 1, static_cast<U *>(getData()), parentList, listPosition);
        }
        else if (ndims == 1)
        {
            H5BasicData<U>::alloc(pvApiCtx, lhsPosition, 1, (int)*dims, parentList, listPosition, &newData);
            memcpy(static_cast<void *>(newData), static_cast<void *>(transformedData), totalSize * sizeof(U));
        }
        else
        {
            if (ndims == 2)
            {
                if (flip)
                {
                    H5BasicData<U>::alloc(pvApiCtx, lhsPosition, (int)dims[1], (int)dims[0], parentList, listPosition, &newData);
                }
                else
                {
                    H5BasicData<U>::alloc(pvApiCtx, lhsPosition, (int)dims[0], (int)dims[1], parentList, listPosition, &newData);
                }

                H5DataConverter::C2FHypermatrix(2, dims, 0, static_cast<U *>(getData()), newData);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, lhsPosition, parentList, listPosition, flip);
                H5BasicData<U>::alloc(pvApiCtx, lhsPosition, (int)totalSize, 1, list, 3, &newData);
                H5DataConverter::C2FHypermatrix((int)ndims, dims, totalSize, static_cast<U *>(getData()), newData, flip);
            }
        }
    }

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
    {
        return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this);
    }
};
}

#endif // __H5TRANSFORMEDDATA_HXX__
