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

#ifndef __H5BASICDATA_HXX__
#define __H5BASICDATA_HXX__

#include "H5Data.hxx"
#include "H5Object.hxx"
#include "H5DataConverter.hxx"

#define __SCILAB_STACK_CREATOR__(U,NAME) static void create(void * pvApiCtx, const int position, const int rows, const int cols, U * ptr, int * list, const int listPosition) \
    {                                                                   \
        SciErr err;                                                     \
        if (list)                                                       \
        {                                                               \
            if (rows == 0 || cols == 0)                                 \
            {                                                           \
                createMatrixOfDoubleInList(pvApiCtx, position, list, listPosition, 0, 0, 0); \
                return;                                                 \
            }                                                           \
            err = createMatrixOf##NAME##InList(pvApiCtx, position, list, listPosition, rows, cols, ptr); \
        }                                                               \
        else                                                            \
        {                                                               \
            if (rows == 0 || cols == 0)                                 \
            {                                                           \
                createEmptyMatrix(pvApiCtx, position);                  \
                return;                                                 \
            }                                                           \
            err = createMatrixOf##NAME(pvApiCtx, position, rows, cols, ptr); \
        }                                                               \
        if (err.iErr)                                                   \
        {                                                               \
            throw H5Exception(__LINE__, __FILE__, "Cannot allocate memory"); \
        }                                                               \
    }

#define __SCILAB_STACK_ALLOCATOR__(U,NAME) static void alloc(void * pvApiCtx, const int position, const int rows, const int cols, int * list, const int listPosition, U ** ptr) \
    {                                                                   \
        SciErr err;                                                     \
        if (list)                                                       \
        {                                                               \
            err = allocMatrixOf##NAME##InList(pvApiCtx, position, list, listPosition, rows, cols, ptr); \
        }                                                               \
        else                                                            \
        {                                                               \
            err = allocMatrixOf##NAME(pvApiCtx, position, rows, cols, ptr); \
        }                                                               \
        if (err.iErr)                                                   \
        {                                                               \
            throw H5Exception(__LINE__, __FILE__, "Cannot allocate memory"); \
        }                                                               \
    }

#define __SCILAB_ALLOCATORS_CREATORS__(U,NAME) __SCILAB_STACK_CREATOR__(U,NAME) \
    __SCILAB_STACK_ALLOCATOR__(U,NAME)


namespace org_modules_hdf5
{
template<typename T>
class H5BasicData : public H5Data
{

protected:

    T * transformedData;

public:

    H5BasicData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, T * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5Data(_parent, _totalSize, _dataSize, _ndims, _dims, static_cast<void *>(_data), _stride, _offset, _dataOwner), transformedData(0)
    {
        //std::cout << totalSize << ", " << stride << ", " << offset << std::endl;
    }

    virtual ~H5BasicData()
    {
        if (transformedData)
        {
            delete[] transformedData;
        }
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        os << static_cast<T *>(getData())[pos];
    }

    virtual void copyData(T * dest) const
    {
        if (dest)
        {
            if (stride == 0)
            {
                memcpy(static_cast<void *>(dest), data, totalSize * dataSize);
            }
            else if (transformedData)
            {
                memcpy(static_cast<void *>(dest), static_cast<void *>(transformedData), totalSize * dataSize);
            }
            else
            {
                char * cdata = static_cast<char *>(data) + offset;
                if (sizeof(T) == dataSize)
                {
                    for (int i = 0; i < totalSize; i++)
                    {
                        dest[i] = *((T *)cdata);
                        cdata += stride;
                    }
                }
                else
                {
                    char * _dest = reinterpret_cast<char *>(dest);
                    for (int i = 0; i < totalSize; i++)
                    {
                        memcpy(_dest, cdata, dataSize);
                        cdata += stride;
                        _dest += dataSize;
                    }
                }
            }
        }
        else
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot copy data to an empty pointer"));
        }
    }

    inline virtual void * getData() const
    {
        if (stride == 0)
        {
            return data;
        }
        else
        {
            if (!transformedData)
            {
                char * dest = new char[totalSize * dataSize];
                copyData(reinterpret_cast<T *>(dest));
                const_cast<H5BasicData *>(this)->transformedData = reinterpret_cast<T *>(dest);
            }

            return static_cast<void *>(transformedData);
        }
    }

    virtual void toScilab(void * pvApiCtx, const int lhsPosition, int * parentList = 0, const int listPosition = 0, const bool flip = true) const
    {
        T * newData = 0;
        hsize_t _ndims = ndims;
        hsize_t _totalSize = totalSize;
        hsize_t * _dims = const_cast<hsize_t *>(dims);

        if (_ndims == 0)
        {
            create(pvApiCtx, lhsPosition, 1, 1, static_cast<T *>(getData()), parentList, listPosition);
        }
        else if (_ndims == 1)
        {
            alloc(pvApiCtx, lhsPosition, 1, (int)*_dims, parentList, listPosition, &newData);
            copyData(newData);
        }
        else
        {
            if (_ndims == 2)
            {
                if (flip)
                {
                    alloc(pvApiCtx, lhsPosition, (int)_dims[1], (int)_dims[0], parentList, listPosition, &newData);
                }
                else
                {
                    alloc(pvApiCtx, lhsPosition, (int)_dims[0], (int)_dims[1], parentList, listPosition, &newData);
                }

                H5DataConverter::C2FHypermatrix(2, _dims, 0, static_cast<T *>(getData()), newData, flip);
            }
            else
            {
                int * list = getHypermatrix(pvApiCtx, lhsPosition, parentList, listPosition, flip);
                alloc(pvApiCtx, lhsPosition, (int)_totalSize, 1, list, 3, &newData);
                H5DataConverter::C2FHypermatrix((int)_ndims, _dims, _totalSize, static_cast<T *>(getData()), newData, flip);
            }
        }
    }

    virtual std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
    {
        return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this);
    }

    static void putStringVectorOnStack(std::vector<std::string> & strs, const int rows, const int cols, const int pos, void * pvApiCtx)
    {
        if (rows * cols != strs.size())
        {
            throw H5Exception(__LINE__, __FILE__, _("Wrong dimensions."));
        }

        if (strs.size() == 0)
        {
            create(pvApiCtx, pos, 0, 0, "", 0, 0);
        }
        else
        {
            std::vector<const char *> _strs;
            _strs.reserve(strs.size());
            for (unsigned int i = 0; i < strs.size(); i++)
            {
                _strs.push_back(strs[i].c_str());
            }
            create(pvApiCtx, pos, rows, cols, const_cast<char **>(&(_strs[0])), 0, 0);
        }
    }

    __SCILAB_ALLOCATORS_CREATORS__(double, Double)
    __SCILAB_ALLOCATORS_CREATORS__(char, Integer8)
    __SCILAB_ALLOCATORS_CREATORS__(unsigned char, UnsignedInteger8)
    __SCILAB_ALLOCATORS_CREATORS__(short, Integer16)
    __SCILAB_ALLOCATORS_CREATORS__(unsigned short, UnsignedInteger16)
    __SCILAB_ALLOCATORS_CREATORS__(int, Integer32)
    __SCILAB_ALLOCATORS_CREATORS__(unsigned int, UnsignedInteger32)

#ifdef  _SCILAB_INT64__
    __SCILAB_ALLOCATORS_CREATORS__(long long, Integer64)
    __SCILAB_ALLOCATORS_CREATORS__(unsigned long long, UnsignedInteger64)
#endif

    __SCILAB_STACK_CREATOR__(char *, String)
    static void alloc(void * pvApiCtx, const int position, const int rows, const int cols, int * list, const int listPosition, char*** ptr) {}
};
}


#undef __SCILAB_STACK_CREATOR__
#undef __SCILAB_STACK_ALLOCATOR__
#undef __SCILAB_LIST_CREATOR__
#undef __SCILAB_LIST_ALLOCATOR__
#undef __SCILAB_ALLOCATORS_CREATORS__

#endif // __H5BASICDATA_HXX__
