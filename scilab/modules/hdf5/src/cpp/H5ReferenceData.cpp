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

#include "H5ReferenceData.hxx"

namespace org_modules_hdf5
{

H5ReferenceData::H5ReferenceData(H5Object & _parent, const bool _datasetReference, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride, const size_t _offset, const bool _dataOwner) : H5BasicData<char>(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), datasetReference(_datasetReference), cumprod(H5Object::getCumProd(_ndims, dims))
{

}

H5ReferenceData::~H5ReferenceData()
{
    delete[] cumprod;
}

void H5ReferenceData::toScilab(void * pvApiCtx, const int lhsPosition, int * parentList, const int listPosition, const bool flip) const
{
    H5Object::toScilab(pvApiCtx, lhsPosition, parentList, listPosition, flip);
}

const char ** H5ReferenceData::getReferencesName() const
{
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

H5Object & H5ReferenceData::getData(const unsigned int size, const unsigned int * index) const
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
    }

    if (pos >= totalSize)
    {
        throw H5Exception(__LINE__, __FILE__, _("Invalid index."));
    }

    cdata += pos * (stride ? stride : dataSize);

    file = getFile().getH5Id();
    ref = &(((void **)cdata)[0]);
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
    return H5DataConverter::dump(alreadyVisited, indentLevel, (int)ndims, dims, *this);
}

void H5ReferenceData::printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
{
    char * cdata = static_cast<char *>(data) + offset + pos * (stride ? stride : dataSize);
    void ** ref = &(((void **)cdata)[0]);
    hid_t file = getFile().getH5Id();
    hid_t obj = H5Rdereference(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref);
    if (obj < 0)
    {
        os << "NULL";
        return;
    }

    ssize_t size = H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, 0, 0);
    char * name = 0;
    H5O_info_t info;

    if (size != -1)
    {
        name = new char[size + 1];
        H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, name, size + 1);

        if (datasetReference == H5R_OBJECT)
        {
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
                    if (size != -1)
                    {
                        delete[] name;
                    }
                    throw H5Exception(__LINE__, __FILE__, _("Unknown HDF5 object"));
            }

            os << (haddr_t)(*ref) << " " << name;
        }
        else
        {
            hid_t space = H5Rget_region(file, H5R_DATASET_REGION, ref);
            hssize_t npoints = H5Sget_select_elem_npoints(space);
            hsize_t ndims = H5Sget_simple_extent_dims(space, 0, 0);
            H5Oclose(obj);

            os << "DATASET " << name << " {";

            if (npoints >= 0)
            {
                const hsize_t N = ndims * npoints;
                hsize_t * buf = new hsize_t[N];
                herr_t err =  H5Sget_select_elem_pointlist(space, 0, npoints, buf);
                for (hssize_t i = 0; i < (hssize_t)N; i += ndims)
                {
                    os << "(";
                    for (unsigned int j = 0; j < ndims - 1; j++)
                    {
                        os << buf[i + j] << ",";
                    }
                    os << buf[i + ndims - 1] << ")";

                    if (i != N - ndims)
                    {
                        os << ", ";
                    }
                    else
                    {
                        os << "}";
                    }
                }
                delete[] buf;
            }
            else
            {
                hssize_t nblocks = H5Sget_select_hyper_nblocks(space);
                if (nblocks >= 0)
                {
                    const hsize_t N = 2 * ndims * nblocks;
                    hsize_t * buf = new hsize_t[N];
                    herr_t err = H5Sget_select_hyper_blocklist(space, 0, nblocks, buf);
                    for (hssize_t i = 0; i < (hssize_t)N; i += 2 * ndims)
                    {
                        os << "(";
                        for (unsigned int j = 0; j < ndims - 1; j++)
                        {
                            os << buf[i + j] << ",";
                        }
                        os << buf[i + ndims - 1] << ")-(";
                        for (unsigned int j = 0; j < ndims - 1; j++)
                        {
                            os << buf[i + ndims + j] << ",";
                        }
                        os << buf[i + 2 * ndims - 1] << ")";

                        if (i != N - 2 * ndims)
                        {
                            os << ", ";
                        }
                        else
                        {
                            os << "}";
                        }
                    }

                    delete[] buf;
                }
            }

            H5Sclose(space);
        }

        delete[] name;
    }
}
}
