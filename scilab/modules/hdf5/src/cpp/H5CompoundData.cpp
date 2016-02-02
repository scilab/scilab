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

#include "H5Options.hxx"
#include "H5CompoundData.hxx"
#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

H5CompoundData::H5CompoundData(H5Object & _parent, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, hid_t compoundType, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), type(compoundType), cumprod(H5Object::getCumProd(_ndims, dims))
{
    nfields = (unsigned int)H5Tget_nmembers(compoundType);
    infos = new std::map<std::string, FieldInfo *>();
    fieldinfos = new FieldInfo *[nfields];
    for (unsigned int i = 0; i < nfields; i++)
    {
        hid_t mtype = H5Tget_member_type(compoundType, i);
        hsize_t size = H5Tget_size(mtype);
        char * mname = H5Tget_member_name(compoundType, i);
        size_t offs = H5Tget_member_offset(compoundType, i);
        FieldInfo * info = 0;
        if (H5Tget_class(type) == H5T_STRING && !H5Tis_variable_str(type))
        {
            // We have a C-string so it is null terminated
            size++;
        }

        info = new FieldInfo(mtype, size, offs, std::string(mname));
        (*infos)[std::string(mname)] = info;
        fieldinfos[i] = info;
        free(mname);
    }
}

H5CompoundData::~H5CompoundData()
{
    infos->erase(infos->begin(), infos->end());
    delete infos;
    delete[] fieldinfos;
    delete[] cumprod;
    H5Tclose(type);
}

void H5CompoundData::toScilab(void * pvApiCtx, const int lhsPosition, int * parentList, const int listPosition, const bool flip) const
{
    H5Object::toScilab(pvApiCtx, lhsPosition, parentList, listPosition, flip);
}

bool H5CompoundData::isCompound() const
{
    return true;
}

void H5CompoundData::getAccessibleAttribute(const std::string & _name, const int pos, void * pvApiCtx) const
{
    H5Data & data = getData(_name);
    data.toScilab(pvApiCtx, pos, 0, 0, H5Options::isReadFlip());

    if (data.mustDelete())
    {
        delete &data;
    }
}

H5Data & H5CompoundData::getData(const std::string fieldname) const
{
    std::map<std::string, FieldInfo *>::const_iterator it = infos->find(fieldname);

    if (it != infos->end())
    {
        return H5DataFactory::getObjectData(*const_cast<H5CompoundData *>(this), totalSize, it->second->size, it->second->type, ndims, dims, data, stride ? stride : dataSize, offset + it->second->offset, false);
    }

    throw H5Exception(__LINE__, __FILE__, _("Invalid field name: %s"), fieldname.c_str());
}

H5Object & H5CompoundData::getData(const unsigned int size, const unsigned int * index) const
{
    unsigned int pos = 0;
    unsigned int cumprod = 1;
    hsize_t * _dims = 0;

    for (unsigned int i = 0; i < size; i++)
    {
        pos += cumprod * index[i];
    }

    if (pos >= totalSize)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid index."));
    }

    _dims = new hsize_t[1];
    _dims[0] = 1;

    return *new H5CompoundData(*const_cast<H5CompoundData *>(this), 1, dataSize, 1, _dims, static_cast<char *>(data) + offset + pos * (dataSize + stride), type, 0, 0, false);
}

void H5CompoundData::getFieldNames(const int position, void * pvApiCtx)
{
    std::vector<std::string> names;
    names.reserve(nfields);

    for (unsigned int i = 0; i < nfields; i++)
    {
        names.push_back(fieldinfos[i]->name);
    }
    H5BasicData<char>::putStringVectorOnStack(names, (int)names.size(), 1, position, pvApiCtx);
}

std::string H5CompoundData::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);

    os << H5Object::getIndentString(indentLevel) << "HDF5 Compound data" << std::endl
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

    os << indentString << _("Fields Names") << ": [";
    for (unsigned int i = 0; i < nfields - 1; i++)
    {
        os << fieldinfos[i]->name << ", ";
    }
    os << fieldinfos[nfields - 1]->name << "]";

    return os.str();
}

std::string H5CompoundData::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this, false);
}

void H5CompoundData::printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
{
    os << "{" << std::endl;
    std::string indent = H5Object::getIndentString(indentLevel + 2);

    for (unsigned int i = 0; i < nfields; i++)
    {
        H5Data & hdata = H5DataFactory::getObjectData(*const_cast<H5CompoundData *>(this), totalSize, fieldinfos[i]->size, fieldinfos[i]->type, ndims, dims, data, stride ? stride : dataSize, offset + fieldinfos[i]->offset, false);
        os << indent;
        hdata.printData(os, pos, indentLevel + 2);

        if (i != nfields - 1)
        {
            os << ", " << std::endl;
        }

        delete &hdata;
    }

    os << std::endl << H5Object::getIndentString(indentLevel + 1) << "}";
}
}
