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

#ifndef __H5REFERENCEDATA_HXX__
#define __H5REFERENCEDATA_HXX__

#include "H5File.hxx"
#include "H5BasicData.hxx"

namespace org_modules_hdf5
{

class H5ReferenceData : public H5BasicData<char>
{

    const bool datasetReference;

public:

    H5ReferenceData(H5Object & _parent, const bool _datasetReference, const hsize_t _totalSize, const hsize_t _dataSize, const hsize_t _ndims, const hsize_t * _dims, char * _data, const hsize_t _stride = -1, const size_t _offset = 0, const bool _dataOwner = true) : H5BasicData(_parent, _totalSize, _dataSize, _ndims, _dims, _data, _stride, _offset, _dataOwner), datasetReference(_datasetReference)
    {

    }

    virtual ~H5ReferenceData()
    {

    }

    const char ** getReferencesName() const
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

    H5Object ** getReferencesObject() const
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

    static void deleteReferencesObjects(H5Object ** objs, const unsigned int size)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            delete objs[i];
        }

        delete[] objs;
    }

    std::string dump(std::map<haddr_t, std::string> & alreadyVisited, const unsigned int indentLevel) const
    {
        return H5DataConverter::dump(alreadyVisited, indentLevel, ndims, dims, *this);
    }

    virtual void printData(std::ostream & os, const unsigned int pos, const unsigned int indentLevel) const
    {
        char * cdata = static_cast<char *>(data) + offset;
        void ** ref = &(((void **)cdata)[pos]);
        hid_t file = getFile().getH5Id();
        hid_t obj = H5Rdereference(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref);
        ssize_t size = H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, 0, 0);
        char * name = new char[size + 1];
        H5O_info_t info;
        H5Oget_info(obj, &info);
        H5Oclose(obj);
        H5Rget_name(file, datasetReference ? H5R_DATASET_REGION : H5R_OBJECT, ref, name, size + 1);

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
};
}

#endif // __H5REFERENCEDATA_HXX__
