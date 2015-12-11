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

#include "H5VlenData.hxx"
#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

H5VlenData::H5VlenData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, hid_t vlenType, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), cumprod(H5Object::getCumProd(_ndims, dims))
{
    type = H5Tget_super(vlenType);
    baseSize = H5Tget_size(type);
    if (H5Tget_class(type) == H5T_STRING && !H5Tis_variable_str(type))
    {
        // We have a C-string so it is null terminated
        baseSize++;
    }
}

H5VlenData::~H5VlenData()
{
    delete[] cumprod;
    H5Tclose(type);
}

bool H5VlenData::isVlen() const
{
    return true;
}

H5Object & H5VlenData::getData(const unsigned int size, const unsigned int * index) const
{
    unsigned int pos = 0;
    hsize_t * _dims;
    hvl_t * x = 0;

    for (unsigned int i = 0; i < size; i++)
    {
        pos += (int)cumprod[i] * index[i];
    }

    if (pos >= totalSize)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid index."));
    }

    x = reinterpret_cast<hvl_t *>(static_cast<char *>(data) + offset + pos * (stride ? stride : dataSize));
    _dims = new hsize_t[1];
    *_dims = (hsize_t)x->len;

    return H5DataFactory::getObjectData(*const_cast<H5VlenData *>(this), *_dims, baseSize, type, 1, _dims, x->p, 0, 0, false);
}

std::string H5VlenData::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);

    os << H5Object::getIndentString(indentLevel) << "HDF5 Variable length data" << std::endl
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

std::string H5VlenData::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this, false);
}

void H5VlenData::printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
{
    hvl_t * x = reinterpret_cast<hvl_t *>(static_cast<char *>(data) + offset + pos * (stride ? stride : dataSize));

    if (!x || !x->p)
    {
        os << "()";
        return;
    }

    hsize_t * _dims = new hsize_t[1];

    *_dims = (hsize_t)x->len;
    H5Data & hdata = H5DataFactory::getObjectData(*const_cast<H5VlenData *>(this), *_dims, baseSize, type, 1, _dims, x->p, 0, 0, false);

    os << "(";
    for (unsigned int i = 0; i < *_dims - 1; i++)
    {
        hdata.printData(os, i, indentLevel + 1);
        os << ", ";
    }
    hdata.printData(os, (int)*_dims - 1, indentLevel + 1);
    os << ")";

    delete &hdata;
}

void H5VlenData::toScilab(void * pvApiCtx, const int lhsPosition, int * parentList, const int listPosition, const bool flip) const
{
    H5Object::toScilab(pvApiCtx, lhsPosition, parentList, listPosition, flip);
}
}
