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

#ifndef __HDF5SCILAB_HXX__
#define __HDF5SCILAB_HXX__

#include <vector>
#include <map>

#include "H5Options.hxx"
#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Dataspace.hxx"
#include "H5Attribute.hxx"
#include "H5BasicData.hxx"
#include "H5Data.hxx"
#include "H5VariableScope.hxx"

extern "C"
{
#include "doublecomplex.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "HDF5Objects.h"
#include "sciprint.h"
#include "localization.h"
}

namespace org_modules_hdf5
{

class HDF5Scilab
{

public:

    enum H5ObjectType
    {
        H5FILE,
        H5GROUP,
        H5DATASET,
        H5ATTRIBUTE,
        H5SPACE,
        H5TYPE,
        H5REFERENCE,
        H5LIST,
        H5COMPOUND,
        H5ARRAY,
        H5VLEN
    };

    static std::map<std::string, H5Object::FilterType> filtersName;

    static int getH5ObjectId(int * mlist, void * pvApiCtx);

    static H5Object * getH5Object(int * mlist, void * pvApiCtx);

    static bool isH5Object(int * mlist, void * pvApiCtx);

    static void scilabPrint(const std::string & str);

    static void split(const std::string & str, std::vector<std::string> & v, const char c = '\n');

    static void readData(const std::string & filename, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx);

    static void readData(H5Object & obj, const std::string & name, const unsigned int size, const double * start, const double * stride, const double * count, const double * block, int pos, void * pvApiCtx);

    static void readAttributeData(H5Object & obj, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx);

    static void readAttributeData(const std::string & filename, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx);

    static void deleteObject(const std::string & file, const std::string & name);

    static void deleteObject(const std::string & file, const int size, const char ** names);

    static void deleteObject(const H5Object & parent, const std::string & name);

    static void deleteObject(const H5Object & parent, const int size, const char ** names);

    static void getObject(H5Object & parent, const std::string & location, const bool isAttr, int position, void * pvApiCtx);

    static void createLink(H5Object & parent, const std::string & name, const std::string & targetPath, const bool hard);

    static void createLink(H5Object & parent, const std::string & name, H5Object & targetObject, const bool hard);

    static void createLink(H5Object & parent, const std::string & name, const std::string & targetFile, const std::string & targetPath);

    static void createLink(H5Object & parent, const std::string & name, H5Object & targetObject);

    static void createLink(const std::string & file, const std::string & location, const std::string & name, const std::string & destName, const bool hard);

    static void createLink(const std::string & file, const std::string & location, const std::string & name, const std::string & destFile, const std::string & destName);

    static void copy(H5Object & src, const std::string & slocation, H5Object & dest, const std::string & dlocation);

    static void copy(H5Object & src, const std::string & slocation, const std::string & dfile, const std::string & dlocation);

    static void copy(const std::string & sfile, const std::string & slocation, H5Object & dest, const std::string & dlocation);

    static void copy(const std::string & sfile, const std::string & slocation, const std::string & dfile, const std::string & dlocation);

    static void ls(H5Object & obj, const std::string & name, int position, void * pvApiCtx);

    static void ls(const std::string & path, const std::string & name, int position, void * pvApiCtx);

    static void ls(H5Object & obj, const std::string & name, const std::string & type, int position, void * pvApiCtx);

    static void ls(const std::string & path, const std::string & name, const std::string & type, int position, void * pvApiCtx);

    static bool checkType(const H5Object & obj, const H5ObjectType type);

    static void mount(H5Object & obj, const std::string & location, H5Object & file);

    static void umount(H5Object & obj, const std::string & location);

    static void createGroup(H5Object & parent, const std::string & name);

    static void createGroup(const std::string & file, const std::string & name);

    static void createGroup(H5Object & parent, const int size, const char ** names);

    static void createGroup(const std::string & file, const int size, const char ** names);

    static void label(H5Object & obj, const std::string & location, const unsigned int size, const unsigned int * dim, const char ** names);

    static void label(const std::string & filename, const std::string & location, const unsigned int size, const unsigned int * dim, const char ** names);

    static int * exists(H5Object & obj, const unsigned int size, const char ** locations, const char ** attrNames);

    static int * exists(const std::string & filename, const unsigned int size, const char ** locations, const char ** attrNames);

    template <typename T>
    static T & create(H5Object & parent, const std::string & name, const unsigned int srank, const hsize_t * sdims, const hsize_t * sstart, const hsize_t * sstride, const hsize_t * scount, const hsize_t * sblock, const hid_t sourceType, void * data, const unsigned int drank, const hsize_t * ddims, const hsize_t * dmaxdims, const hsize_t * dstart, const hsize_t * dstride, const hsize_t * dcount, const hsize_t * dblock, const hid_t targetType)
    {
        hid_t obj;
        hid_t srcspace;
        hid_t targetspace;
        hid_t targettype;
        hsize_t * newdims = 0;
        bool mustDelete = false;
        H5T_cdata_t * pcdata = 0;
        bool chunked = false;

        if (srank > __SCILAB_HDF5_MAX_DIMS__ || drank > __SCILAB_HDF5_MAX_DIMS__)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid rank, must be in the interval [0, %d]."), __SCILAB_HDF5_MAX_DIMS__);
        }

        if (targetType == (hid_t) - 1)
        {
            targettype = H5Tcopy(sourceType);
        }
        else
        {
            targettype = H5Tcopy(targetType);
        }

        if (!H5Tfind(sourceType, targettype, &pcdata))
        {
            H5Tclose(targettype);
            throw H5Exception(__LINE__, __FILE__, _("No converter found for the specified target datatype."));
        }

        srcspace = H5Screate_simple(srank, sdims, 0);
        if (srcspace < 0)
        {
            H5Tclose(targettype);
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a new dataspace."));
        }

        try
        {
            newdims = H5Dataspace::select(srcspace, srank, sstart, sstride, scount, sblock);
        }
        catch (const H5Exception & /*e*/)
        {
            H5Tclose(targettype);
            H5Sclose(srcspace);
            throw;
        }

        if (ddims)
        {
            targetspace = H5Screate_simple(drank, ddims, dmaxdims);
            if (targetspace < 0)
            {
                if (newdims)
                {
                    delete[] newdims;
                }
                H5Sclose(srcspace);
                H5Tclose(targettype);
                throw H5Exception(__LINE__, __FILE__, _("Invalid target dataspace."));
            }
            if (dmaxdims)
            {
                for (unsigned int i = 0; i < drank; i++)
                {
                    if (ddims[i] != dmaxdims[i])
                    {
                        chunked = true;
                        break;
                    }
                }
            }
        }
        else if (newdims)
        {
            targetspace = H5Screate_simple(srank, newdims, 0);
            if (targetspace < 0)
            {
                delete[] newdims;
                H5Sclose(srcspace);
                H5Tclose(targettype);
                throw H5Exception(__LINE__, __FILE__, _("Cannot create a new dataspace."));
            }
        }
        else
        {
            targetspace = (hid_t) - 1;
        }

        if (newdims)
        {
            delete[] newdims;
        }

        if (targetspace != -1 && dstart)
        {
            try
            {
                hsize_t * _newdims = H5Dataspace::select(targetspace, drank, dstart, dstride, dcount, dblock);
                if (_newdims)
                {
                    delete[] _newdims;
                }
            }
            catch (const H5Exception & /*e*/)
            {
                H5Tclose(targettype);
                H5Sclose(targetspace);
                H5Sclose(srcspace);
                throw;
            }
        }

        try
        {
            obj = T::create(parent, name, sourceType, targettype, srcspace, targetspace, data, chunked);
            H5Sclose(srcspace);
            if (targetspace >= 0)
            {
                H5Sclose(targetspace);
            }
        }
        catch (const H5Exception & /*e*/)
        {
            H5Tclose(targettype);
            H5Sclose(srcspace);
            if (targetspace >= 0)
            {
                H5Sclose(targetspace);
            }
            throw;
        }

        H5Tclose(targettype);

        return *new T(parent, obj, name);
    }

    static void getScilabData(hid_t * type, unsigned int * ndims, hsize_t ** dims, void ** data, bool * mustDelete, bool * mustDeleteContent, const bool flip, int rhsPosition, void * pvApiCtx);

    static void getScilabData(hid_t * type, unsigned int * ndims, hsize_t ** dims, void ** data, bool * mustDelete, bool * mustDeleteContent, const bool flip, int * addr, int rhsPosition, void * pvApiCtx);

    template <typename T>
    static void createObjectFromStack(const std::string & file, const std::string & location, const std::string & name, const bool flip, void * pvApiCtx, const int rhsPosition, const unsigned int srank, const hsize_t * sdims, const hsize_t * sstart, const hsize_t * sstride, const hsize_t * scount, const hsize_t * sblock, const std::string & targetType, const unsigned int drank, const hsize_t * ddims, const hsize_t * dmaxdims, const hsize_t * dstart, const hsize_t * dstride, const hsize_t * dcount, const hsize_t * dblock)
    {
        H5File & src = *new H5File(file, "/", "r+");

        try
        {
            createObjectFromStack<T>(src, location, name, flip, pvApiCtx, rhsPosition, srank, sdims, sstart, sstride, scount, sblock, targetType, drank, ddims, dmaxdims, dstart, dstride, dcount, dblock);
            delete &src;
        }
        catch (const H5Exception & /*e*/)
        {
            delete &src;
            throw;
        }
    }

    template <typename T>
    static void createObjectFromStack(H5Object & obj, const std::string & location, const std::string & name, const bool flip, void * pvApiCtx, const int rhsPosition, const unsigned int srank, const hsize_t * sdims, const hsize_t * sstart, const hsize_t * sstride, const hsize_t * scount, const hsize_t * sblock, const std::string & targetType, const unsigned int drank, const hsize_t * ddims, const hsize_t * dmaxdims, const hsize_t * dstart, const hsize_t * dstride, const hsize_t * dcount, const hsize_t * dblock)
    {
        hid_t sourceType = -1;;
        hid_t targettype;
        unsigned int rank;
        hsize_t * dims = 0;
        void * data = 0;
        bool mustDelete = false;
        bool mustDeleteContent = false;
        H5Object * hobj = 0;
        T * newobj = 0;
        bool isReference = false;
        hsize_t total = 1;

        if (targetType.empty())
        {
            targettype = (hid_t) - 1;
        }
        else
        {
            targettype = H5Type::getBaseType(targetType);
            if (targettype < 0)
            {
                throw H5Exception(__LINE__, __FILE__, _("Cannot create the target type."));
            }

            if (H5Tequal(targettype, H5T_STD_REF_OBJ))
            {
                isReference = true;
            }
        }

        try
        {
            hobj = H5Object::isEmptyPath(location) ? &obj : &H5Object::getObject(obj, location);
            getScilabData(&sourceType, &rank, &dims, &data, &mustDelete, &mustDeleteContent, flip, rhsPosition, pvApiCtx);

            if (sdims)
            {
                hsize_t p1 = 1;
                hsize_t p2 = 1;
                for (unsigned int i = 0; i < srank; i++)
                {
                    p1 *= sdims[i];
                }
                for (unsigned int i = 0; i < rank; i++)
                {
                    p2 *= dims[i];
                }

                total = p1;

                if (p1 != p2)
                {
                    throw H5Exception(__LINE__, __FILE__, _("Incompatible dimensions"));
                }
                rank = srank;
            }
            else
            {
                sdims = dims;
                for (unsigned int i = 0; i < rank; i++)
                {
                    total *= sdims[i];
                }
            }

            if (isReference)
            {
                herr_t err;
                hid_t loc;
                hobj_ref_t * newData = 0;

                if (H5Tget_class(sourceType) != H5T_STRING)
                {
                    throw H5Exception(__LINE__, __FILE__, _("References must be given as strings"));
                }

                loc = hobj->getFile().getH5Id();
                newData = (hobj_ref_t *)MALLOC(sizeof(hobj_ref_t) * total);

                for (unsigned int i = 0; i < total; i++)
                {
                    char * _name = static_cast<char **>(data)[i];
                    err = H5Rcreate(newData + i, loc, _name, H5R_OBJECT, -1);
                    if (err < 0)
                    {
                        FREE(newData);
                        throw H5Exception(__LINE__, __FILE__, _("Invalid path: %s."), _name);
                    }
                }

                if (mustDeleteContent)
                {
                    for (unsigned int i = 0; i < total; i++)
                    {
                        FREE(static_cast<void **>(data)[i]);
                    }
                }
                if (mustDelete)
                {
                    FREE(data);
                }

                data = newData;
                mustDeleteContent = false;
                mustDelete = false;

                if (sourceType > 0)
                {
                    H5Tclose(sourceType);
                }
                sourceType = H5Tcopy(targettype);
            }

            newobj = &create<T>(*hobj, name, rank, sdims, sstart, sstride, scount, sblock, sourceType, data, drank, ddims, dmaxdims, dstart, dstride, dcount, dblock, targettype);
        }
        catch (const H5Exception & /*e*/)
        {
            if (mustDeleteContent)
            {
                for (unsigned int i = 0; i < total; i++)
                {
                    FREE(static_cast<void **>(data)[i]);
                }
            }
            if (mustDelete)
            {
                FREE(data);
            }
            if (dims)
            {
                delete[] dims;
            }
            if (!H5Object::isEmptyPath(location))
            {
                delete hobj;
            }
            if (targettype > 0)
            {
                H5Tclose(targettype);
            }

            if (sourceType > 0)
            {
                H5Tclose(sourceType);
            }

            throw;
        }

        if (newobj)
        {
            delete newobj;
        }

        if (mustDeleteContent)
        {
            for (unsigned int i = 0; i < total; i++)
            {
                FREE(static_cast<void **>(data)[i]);
            }
        }
        if (mustDelete)
        {
            FREE(data);
        }
        if (dims)
        {
            delete[] dims;
        }
        if (!H5Object::isEmptyPath(location))
        {
            delete hobj;
        }
        if (targettype > 0)
        {
            H5Tclose(targettype);
        }
        if (sourceType > 0)
        {
            H5Tclose(sourceType);
        }
    }

    template <typename T>
    static void flip(const unsigned int size, T * data)
    {
        if (!data)
        {
            return;
        }

        for (unsigned int i = 0; i < size / 2; i++)
        {
            T x = data[i];
            data[i] = data[size - 1 - i];
            data[size - 1 - i] = x;
        }
    }

    template <typename T>
    static hsize_t * flipAndConvert(const unsigned int size, T * data)
    {
        if (!data)
        {
            return 0;
        }

        hsize_t * arr = new hsize_t[size];
        for (unsigned int i = 0; i < size; i++)
        {
            arr[i] = (hsize_t)data[size - 1 - i];
        }

        return arr;
    }

private:
    static std::map<std::string, H5Object::FilterType> initFilterNames();
};
}

#endif // __HDF5SCILAB_HXX__
