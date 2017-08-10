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

#include "H5StringData.hxx"

namespace org_modules_hdf5
{

H5StringData::H5StringData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _stringSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5Data(_parent, _totalSize, _stringSize, _ndims, _dims, _data, _stride == 0 ? _stringSize : _stride, _offset, _dataOwner)
{
    char * __data = static_cast<char *>(_data);
    transformedData = new char*[totalSize];

    if (stride == 0)
    {
        *transformedData = __data;
        for (unsigned int i = 1; i < (unsigned int)totalSize; i++)
        {
            transformedData[i] = transformedData[i - 1] + dataSize;
        }
    }
    else
    {
        for (unsigned int i = 0; i < (unsigned int)totalSize; i++)
        {
            transformedData[i] = __data + offset;
            __data += stride;
        }
    }
}

H5StringData::H5StringData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _stringSize, const hsize_t _ndims, const hsize_t * _dims, char ** _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5Data(_parent, _totalSize, _stringSize, _ndims, _dims, _data, _stride == 0 ? _stringSize : _stride, _offset, _dataOwner), transformedData(0)
{

}

H5StringData::~H5StringData()
{

    if (transformedData)
    {
        delete[] transformedData;
    }
    else
    {
        char ** _data = reinterpret_cast<char **>(getData());
        hid_t space = H5Screate_simple(1, &totalSize, 0);
        hid_t type = H5Tcopy(H5T_C_S1);
        H5Tset_size(type, H5T_VARIABLE);
        H5Tset_strpad(type, H5T_STR_NULLTERM);

        herr_t err = H5Dvlen_reclaim(type, space, H5P_DEFAULT, _data);
        if (err < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot free the memory associated with String data"));
        }

        H5Tclose(type);
        H5Sclose(space);
    }
}

void * H5StringData::getData() const
{
    if (transformedData)
    {
        return transformedData;
    }
    else
    {
        return H5Data::getData();
    }
}

void H5StringData::printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
{
    char * str = static_cast<char **>(getData())[pos];
    if (str)
    {
        os << "\"" << str << "\"";
    }
    else
    {
        os << "NULL";
    }
}

void H5StringData::toScilab(void * pvApiCtx, const int lhsPosition, int * parentList, const int listPosition, const bool flip) const
{
    static char EMPTY[] = { '\0' };

    char ** _tdata = 0;
    char ** _data = static_cast<char **>(getData());

    if (!transformedData)
    {
        // It is possible to have a nil pointer (Scilab doesn't like that)
        // so we replace nil ptr by an empty string...

        _tdata = new char *[totalSize];
        for (hsize_t i = 0; i < totalSize; i++)
        {
            if (_data[i])
            {
                _tdata[i] = _data[i];
            }
            else
            {
                _tdata[i] = static_cast<char *>(EMPTY);
            }
        }
        _data = _tdata;
    }

    std::cout << ndims << std::endl;

    if (ndims == 0)
    {
        H5BasicData<char *>::create(pvApiCtx, lhsPosition, 1, 1, _data, parentList, listPosition);
    }
    else if (ndims == 1)
    {
        H5BasicData<char *>::create(pvApiCtx, lhsPosition, 1, (int)*dims, _data, parentList, listPosition);
    }
    else
    {
        char ** newData = new char *[totalSize];
        if (ndims == 2)
        {
            H5DataConverter::C2FHypermatrix(2, dims, 0, _data, newData, flip);
            if (flip)
            {
                H5BasicData<char *>::create(pvApiCtx, lhsPosition, (int)dims[1], (int)dims[0], newData, parentList, listPosition);
            }
            else
            {
                H5BasicData<char *>::create(pvApiCtx, lhsPosition, (int)dims[0], (int)dims[1], newData, parentList, listPosition);
            }
        }
        else
        {
            int * list = getHypermatrix(pvApiCtx, lhsPosition, parentList, listPosition, flip);
            H5DataConverter::C2FHypermatrix((int)ndims, dims, totalSize, _data, newData, flip);
            H5BasicData<char *>::create(pvApiCtx, lhsPosition, (int)totalSize, 1, newData, list, 3);
        }
        delete[] newData;
    }

    if (_tdata)
    {
        delete[] _tdata;
    }
}

std::string H5StringData::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this);
}
}
