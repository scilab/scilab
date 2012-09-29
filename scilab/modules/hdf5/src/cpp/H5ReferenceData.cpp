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

#include "H5ReferenceData.hxx"

namespace org_modules_hdf5
{

H5ReferenceData::H5ReferenceData(H5Object & _parent, const bool _datasetReference, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, const hsize_t _arank, const hsize_t * _adims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData(_parent, _totalSize, _dataSize, _ndims, _dims, _arank, _adims, _data, _stride, _offset, _dataOwner), datasetReference(_datasetReference)
{

}

H5ReferenceData::~H5ReferenceData()
{

}

const char ** H5ReferenceData::getReferencesName() const
{
    // TODO: tenir compte du stride
    hid_t file = getFile().getH5Id();
    char * cdata = static_cast<char *>(data) + offset;
    const char ** names = new const char*[totalSize];

    for (int i = 0; i < totalSize; i++)
    {
        void * ref = &(((void **)cdata)[i]);
        hid_t obj = H5Rdereference(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref);
        H5O_info_t info;
        H5Oget_info(obj, &info);
        H5Oclose(obj);
        ssize_t size = H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, 0, 0);
        char * name = new char[size + 1];
        H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, name, size + 1);

        names[i] = name;
    }

    return names;
}

H5Object & H5ReferenceData::getReferencesObject(const unsigned int size, const double * index) const
{
    H5Object * obj = 0;
    unsigned int * iindex = new unsigned int[size];
    for (unsigned int i = 0; i < size; i++)
    {
        iindex[i] = (unsigned int)(index[i]);
    }

    try
    {
        obj = &getReferencesObject(size, iindex);
        delete[] iindex;
        return *obj;
    }
    catch (const H5Exception & e)
    {
        delete[] iindex;
        throw;
    }
}

H5Object & H5ReferenceData::getReferencesObject(const unsigned int size, const unsigned int * index) const
{
    char * cdata = static_cast<char *>(data) + offset;
    void ** ref = 0;
    unsigned int pos = 0;
    unsigned int cumprod = 1;
    H5O_info_t info;
    H5Object * hobj;
    hid_t obj;
    hid_t file;
    ssize_t ssize;
    char * name = 0;
    std::string _name;

    for (unsigned int i = 0; i < size; i++)
    {
        pos += cumprod * index[i];
        cumprod *= dims[i];
    }

    if (pos >= totalSize)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid index."));
    }

    file = getFile().getH5Id();
    ref = &(((void **)cdata)[pos]);
    obj = H5Rdereference(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref);
    if (obj < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot open object at the given position."));
    }

    ssize = H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, 0, 0);
    name = new char[ssize + 1];
    H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, name, ssize + 1);
    _name = std::string(name);
    delete[] name;

    H5Oget_info(obj, &info);
    switch (info.type)
    {
        case H5O_TYPE_GROUP:
            hobj = new H5Group(getParent(), obj, _name);
            break;
        case H5O_TYPE_DATASET:
            hobj = new H5Dataset(getParent(), obj, _name);
            break;
        case H5O_TYPE_NAMED_DATATYPE:
            hobj = new H5Type(getParent(), obj, _name);
            break;
        case H5O_TYPE_UNKNOWN:
        default:
            H5Oclose(obj);
            throw H5Exception(__LINE__, __FILE__, _("Unknown HDF5 object"));
    }

    return *hobj;
}

H5Object ** H5ReferenceData::getReferencesObject() const
{
    hid_t file = getFile().getH5Id();
    char * cdata = static_cast<char *>(data) + offset;
    H5Object ** objs = new H5Object *[totalSize];

    for (int i = 0; i < totalSize; i++)
    {
        void * ref = &(((void **)cdata)[i]);
        hid_t obj = H5Rdereference(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref);
        objs[i] = &H5Object::getObject(getParent(), obj);
    }

    return objs;
}

void H5ReferenceData::deleteReferencesObjects(H5Object ** objs, const unsigned int size)
{
    for (unsigned int i = 0; i < size; i++)
    {
        delete objs[i];
    }

    delete[] objs;
}

std::string H5ReferenceData::toString(const unsigned int indentLevel) const
{
    std::ostringstream os;
    std::string indentString = H5Object::getIndentString(indentLevel + 1);

    os << H5Object::getIndentString(indentLevel) << "HDF5 Reference data" << std::endl
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
        os << dims[ndims - 1] << "]";
    }

    return os.str();
}

std::string H5ReferenceData::dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
{
    return H5DataConverter::dump(alreadyVisited, indentLevel, ndims, dims, *this);
}

void H5ReferenceData::printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
{
    char * cdata = static_cast<char *>(data) + offset;
    void ** ref = &(((void **)cdata)[pos]);
    hid_t file = getFile().getH5Id();
    hid_t obj = H5Rdereference(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref);
    ssize_t size = H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, 0, 0);
    char * name = new char[size + 1];
    H5O_info_t info;

    H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, name, size + 1);
    H5Oget_info(obj, &info);
    H5Oclose(obj);

    switch (info.type)
    {
        case H5O_TYPE_GROUP:
            os << "GROUP ";
            break;
        case H5O_TYPE_DATASET:
            os << "DATASET ";
            break;
        case H5O_TYPE_NAMED_DATATYPE:
            os << "DATATYPE ";
            break;
        case H5O_TYPE_UNKNOWN:
        default:
            delete[] name;
            throw H5Exception(__LINE__, __FILE__, _("Unknown HDF5 object"));
    }

    os << (haddr_t)(*ref) << " " << name;
    delete[] name;
}

void H5ReferenceData::toScilab(void * pvApiCtx, const int lhsPosition, int * parentList, const int listPosition) const
{
    if (parentList)
    {
        createInScilabList(parentList, lhsPosition, listPosition, pvApiCtx);
    }
    else
    {
        createOnScilabStack(lhsPosition, pvApiCtx);
    }
}
}
