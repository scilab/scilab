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

#include "H5ArrayData.hxx"
#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

H5ArrayData::H5ArrayData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, hid_t arrayType, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), cumprod(H5Object::getCumProd(_ndims, dims))
{
    type = H5Tget_super(arrayType);
    baseSize = H5Tget_size(type);
    andims = H5Tget_array_ndims(arrayType);
    adims = new hsize_t[andims];
    H5Tget_array_dims(arrayType, adims);
    atotalSize = 1;

    if (H5Tget_class(type) == H5T_STRING && !H5Tis_variable_str(type))
    {
        // We have a C-string so it is null terminated
        baseSize++;
    }

    for (unsigned int i = 0; i < andims; i++)
    {
        atotalSize *= adims[i];
    }

}

H5ArrayData::~H5ArrayData()
{
    delete[] cumprod;
    delete[] adims;
    H5Tclose(type);
}

bool H5ArrayData::isArray() const
{
    return true;
}

H5Object & H5ArrayData::getData(const unsigned int size, const unsigned int * index) const
{
    unsigned int pos = 0;
    hsize_t * _dims = 0;

    for (unsigned int i = 0; i < size; i++)
    {
        pos += (int)cumprod[i] * index[i];
    }

    if (pos >= totalSize)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid index."));
    }

    return H5DataFactory::getObjectData(*const_cast<H5ArrayData *>(this), atotalSize, baseSize, type, andims, adims, static_cast<char *>(data) + offset + pos * (stride ? stride : dataSize), 0, 0, false);
}

std::string H5ArrayData::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);

    os << H5Object::getIndentString(indentLevel) << "HDF5 Array data" << std::endl
       << indentString << _("Dimensions") << ": [";

    if (ndims == 0)
    {
        os << "1 x 1]";
    }
    else if (ndims == 1)
    {
        os << "1 x " << dims[0] << "]";
    }
    else
    {
        for (unsigned int i = 0; i < ndims - 1; i++)
        {
            os << dims[i] << " x ";
        }
        os << dims[ndims - 1] << "]" << std::endl;
    }

    return os.str();
}

std::string H5ArrayData::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this, false);
}

void H5ArrayData::printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
{
    os << "[ ";
    H5Data & hdata = H5DataFactory::getObjectData(*const_cast<H5ArrayData *>(this), atotalSize, baseSize, type, andims, adims, static_cast<char *>(data) + offset + pos * (stride ? stride : dataSize), 0, 0, false);

    for (unsigned int i = 0; i < atotalSize - 1; i++)
    {
        hdata.printData(os, i, indentLevel + 1);
        os << ", ";
    }
    hdata.printData(os, (int)atotalSize - 1, (int)indentLevel + 1);
    os << " ]";

    delete &hdata;
}

void H5ArrayData::toScilab(void * pvApiCtx, const int lhsPosition, int * parentList, const int listPosition, const bool flip) const
{
    H5Object::toScilab(pvApiCtx, lhsPosition, parentList, listPosition, flip);
}
}
