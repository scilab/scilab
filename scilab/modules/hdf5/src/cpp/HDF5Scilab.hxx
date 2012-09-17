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

#ifndef __HDF5SCILAB_HXX__
#define __HDF5SCILAB_HXX__

#include "H5Object.hxx"
#include "H5File.hxx"
#include "H5Group.hxx"
#include "H5Dataset.hxx"
#include "H5Attribute.hxx"
#include "H5Data.hxx"
#include "H5VariableScope.hxx"

extern "C"
{
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

    static int getH5ObjectId(int * mlist, void * pvApiCtx);

    static H5Object * getH5Object(int * mlist, void * pvApiCtx);

    static bool isH5Object(int * mlist, void * pvApiCtx);

    static void scilabPrint(const std::string & str);

    static void readData(const std::string & filename, const std::string & name, int pos, void * pvApiCtx);

    static void readData(H5Object & obj, const std::string & name, int pos, void * pvApiCtx);

    static void readAttributeData(H5Object & obj, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx);

    static void readAttributeData(const std::string & filename, const std::string & path, const std::string & attrName, int pos, void * pvApiCtx);

    static void deleteLink(H5Object & parent, const std::string & name);

    static void createLink(H5Object & parent, const std::string & name, const std::string & targetPath, const bool hard);

    static void createLink(H5Object & parent, const std::string & name, H5Object & targetObject, const bool hard);

    static void createLink(H5Object & parent, const std::string & name, const std::string & targetFile, const std::string & targetPath);

    static void createLink(H5Object & parent, const std::string & name, H5Object & targetObject);

    template <typename T, typename U>
    static U & create(H5Object & parent, const std::string & name, const unsigned int rank, const hsize_t * dims, const hsize_t * maxdims, T * data, const hid_t targetType)
    {
        hid_t obj;
        hid_t space;
        hid_t type;
        hid_t targettype;
        herr_t err;
        H5Object * loc = 0;
        hsize_t * _maxdims = 0;
        bool mustDelete = false;
        H5T_conv_t conv;
        H5T_cdata_t * pcdata = 0;

        if (rank > __SCILAB_HDF5_MAX_DIMS__)
        {
            throw H5Exception(__LINE__, __FILE__, _("Invalid rank, must be in the interval [0, %d]."), __SCILAB_HDF5_MAX_DIMS__);
        }

        if (parent.isFile())
        {
            loc = &reinterpret_cast<H5File *>(&parent)->getRoot();
        }
        else
        {
            loc = &parent;
        }

        type = H5Type::getBaseType(data);
        if (type < 0)
        {
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a data type."));
        }

        if (targetType == (hid_t) - 1)
        {
            targettype = type;
        }
        else
        {
            targettype = targetType;
        }

        if (!H5Tfind(type, targettype, &pcdata))
        {
            H5Tclose(type);
            throw H5Exception(__LINE__, __FILE__, _("No converter found for the specified target datatype."));
        }

        if (!maxdims)
        {
            _maxdims = new hsize_t[rank];
            memcpy(_maxdims, dims, rank * sizeof(hsize_t));
            mustDelete = true;
        }
        else
        {
            _maxdims = const_cast<hsize_t *>(maxdims);
        }

        space = H5Screate_simple(rank, dims, maxdims);
        if (mustDelete)
        {
            delete[] _maxdims;
        }

        if (space < 0)
        {
            H5Tclose(type);
            throw H5Exception(__LINE__, __FILE__, _("Cannot create a new dataspace."));
        }

        try
        {
            obj = U::create(*loc, name, type, targettype, space, data);
            H5Tclose(type);
            H5Sclose(space);
        }
        catch (H5Exception & e)
        {
            H5Tclose(type);
            H5Sclose(space);
            throw;
        }

        return *new U(*loc, obj, name);
    }

    template <typename T, typename U>
    static U & create(H5Object & parent, const std::string & name, const unsigned int rank, const hsize_t * dims, const hsize_t * maxdims, T * data, const std::string & targetType)
    {
        hid_t targettype;
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
        }

        try
        {
            U & obj = create<T, U>(parent, name, rank, dims, maxdims, data, targettype);
            if (targettype >= 0)
            {
                H5Tclose(targettype);
            }
            return obj;
        }
        catch (H5Exception & e)
        {
            if (targettype >= 0)
            {
                H5Tclose(targettype);
            }
            throw;
        }
    }
};
}

#endif // __HDF5SCILAB_HXX__
