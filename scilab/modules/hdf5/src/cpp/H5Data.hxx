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

#ifndef __H5DATA_HXX__
#define __H5DATA_HXX__

#include "H5Data.hxx"
#include "H5Object.hxx"

namespace org_modules_hdf5
{
class H5Data : public H5Object
{

protected:

    void * data;
    const hsize_t totalSize;
    const hsize_t dataSize;
    const hsize_t ndims;
    const hsize_t * dims;
    const hsize_t stride;
    const size_t offset;
    const bool dataOwner;

public:

    H5Data(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, void * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5Object(_parent), totalSize(_totalSize), dataSize(_dataSize), ndims(_ndims), dims(_dims), data(_data), stride(_stride), offset(_offset), dataOwner(_dataOwner)
    {

    }

    virtual ~H5Data()
    {
        if (dataOwner)
        {
            delete[] dims;
            delete[] static_cast<char *>(data);
        }
    }

    inline virtual void * getData() const
    {
        return data;
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const {  }

protected:

    void createEmpty(void * pvApiCtx, const int position, int * parentList = 0, const int listPosition = 0) const
    {
        if (parentList)
        {
            createMatrixOfDoubleInList(pvApiCtx, position, parentList, listPosition, 0, 0, 0);
        }
        else
        {
            createEmptyMatrix(pvApiCtx, position);
        }
    }

    int * getHypermatrix(void * pvApiCtx, const int position, int * parentList = 0, const int listPosition = 0, const bool flip = true) const
    {
        static const char * hypermat[3] = {"hm", "dims", "entries"};

        int * list = 0;
        SciErr err;
        if (parentList)
        {
            err = createMListInList(pvApiCtx, position, parentList, listPosition, 3, &list);
        }
        else
        {
            err = createMList(pvApiCtx, position, 3, &list);
        }

        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }

        err = createMatrixOfStringInList(pvApiCtx, position, list, 1, 1, 3, hypermat);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }

        int * _dims = 0;
        err = allocMatrixOfInteger32InList(pvApiCtx, position, list, 2, 1, (int)ndims, &_dims);
        if (err.iErr)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create an hypermatrix on the stack"));
        }

        if (flip)
        {
            for (int i = 0; i < ndims; i++)
            {
                _dims[i] = (int)dims[ndims - 1 - i];
            }
        }
        else
        {
            for (int i = 0; i < ndims; i++)
            {
                _dims[i] = (int)dims[i];
            }
        }

        return list;
    }
};
}

#endif // __H5DATA_HXX__
