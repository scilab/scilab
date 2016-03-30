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

#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

H5Data & H5DataFactory::getData(H5Object & parent, const hid_t obj, H5Dataspace * space, hsize_t * selectdims, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t * dims = 0;
    hsize_t totalSize;
    hsize_t dataSize;
    void * data = 0;
    const hid_t spaceId = space ? space->getH5Id() : -1;
    const hid_t type = isAttribute ? H5Aget_type(obj) : H5Dget_type(obj);
    const hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);
    if (type < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the data type"));
    }

    try
    {
        getNativeData(obj, spaceId, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);
        H5Data & ret = getObjectData(parent, totalSize, dataSize, nativeType, ndims, dims, data, 0, 0, true);
        H5Tclose(type);
        H5Tclose(nativeType);

        return ret;
    }
    catch (const H5Exception & /*e*/)
    {
        H5Tclose(type);
        H5Tclose(nativeType);
        if (dims)
        {
            delete[] dims;
        }

        if (data)
        {
            delete[] static_cast<char *>(data);
        }
        throw;
    }
}

H5Data & H5DataFactory::getObjectData(H5Object & parent, const hsize_t totalSize, const hsize_t dataSize, const hid_t type, const hsize_t ndims, const hsize_t * dims, void * data, const hsize_t stride, const size_t offset, const bool dataOwner)
{
    switch (H5Tget_class(type))
    {
        case H5T_INTEGER:
            if (H5Tequal(type, H5T_NATIVE_SCHAR))
            {
                return *new H5CharData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_UCHAR))
            {
                return *new H5UnsignedCharData(parent, totalSize, dataSize, ndims, dims, (unsigned char *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_SHORT))
            {
                return *new H5BasicData<short>(parent, totalSize, dataSize, ndims, dims, (short *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_USHORT))
            {
                return *new H5BasicData<unsigned short>(parent, totalSize, dataSize, ndims, dims, (unsigned short *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_INT))
            {
                return *new H5BasicData<int>(parent, totalSize, dataSize, ndims, dims, (int *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT))
            {
                return *new H5BasicData<unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned int *)data, stride, offset, dataOwner);
            }

#ifdef __SCILAB_INT64__

            else if (H5Tequal(type, H5T_NATIVE_LONG))
            {
                return *new H5BasicData<long long>(parent, totalSize, dataSize, ndims, dims, (long long *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG))
            {
                return *new H5BasicData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data, stride, offset, dataOwner);
            }
#else

            else if (H5Tequal(type, H5T_NATIVE_LONG))
            {
                return *new H5TransformedData<long long, int>(parent, totalSize, dataSize, ndims, dims, (long long *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG))
            {
                return *new H5TransformedData<unsigned long long, unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data, stride, offset, dataOwner);
            }

#endif // __SCILAB_INT64__

            else
            {
                throw H5Exception(__LINE__, __FILE__, _("Unknown integer datatype."));
            }
            break;
        case H5T_FLOAT:
            if (H5Tequal(type, H5T_NATIVE_FLOAT))
            {
                return *new H5TransformedData<float, double>(parent, totalSize, dataSize, ndims, dims, (float *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_DOUBLE))
            {
                return *new H5BasicData<double>(parent, totalSize, dataSize, ndims, dims, (double *)data, stride, offset, dataOwner);
            }
            else
            {
                throw H5Exception(__LINE__, __FILE__, _("Unknown floating-point datatype."));
            }
            break;
        case H5T_TIME:
            return *new H5TimeData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, dataOwner);
        case H5T_STRING:
            if (H5Tis_variable_str(type))
            {
                return *new H5StringData(parent, totalSize, dataSize, ndims, dims, (char **)data, stride, offset, dataOwner);
            }
            else
            {
                return *new H5StringData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, dataOwner);
            }
        case H5T_BITFIELD:
            switch (dataSize)
            {
                case 1:
                    return *new H5Bitfield1Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned char *>(data), stride, offset, dataOwner);
                case 2:
                    return *new H5Bitfield2Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned short *>(data), stride, offset, dataOwner);
                case 4:
                    return *new H5Bitfield4Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned int *>(data), stride, offset, dataOwner);
                case 8:
                //return *new H5BitfieldData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned long long *>(data), stride, offset, false);
                default:
                    throw H5Exception(__LINE__, __FILE__, _("Bitfield is too big"));
            }

        case H5T_OPAQUE:
            return *new H5OpaqueData(parent, totalSize, dataSize, ndims, dims, (unsigned char *)data, stride, offset, dataOwner);
        case H5T_COMPOUND:
            return *new H5CompoundData(parent, totalSize, dataSize, ndims, dims, (char *)data, H5Tcopy(type), stride, offset, dataOwner);
        case H5T_REFERENCE:
            // TODO: virer le false
            return *new H5ReferenceData(parent, H5Tequal(type, H5T_STD_REF_DSETREG) ? H5R_DATASET_REGION : H5R_OBJECT, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, dataOwner);
        case H5T_ENUM:
        {
            int nmembers = H5Tget_nmembers(type);
            std::string * names = nmembers > 0 ? new std::string[nmembers] : 0;

            for (int i = 0; i < nmembers; i++)
            {
                char * mname = H5Tget_member_name(type, i);
                names[i] = std::string(mname);
                //HDF5 version > 1.8.13
                //H5free_memory(mnale);

                //freed memory allocated by H5Tget_member_name trigger a segfault on Windows.
                //http://lists.hdfgroup.org/pipermail/hdf-forum_lists.hdfgroup.org/2014-September/008061.html
                //little memory leaks are better then crashs :x
#ifndef _MSC_VER
                free(mname);
#endif
            }

            if (H5Tget_sign(type) == H5T_SGN_NONE)
            {
                switch (dataSize)
                {
                    case 1:
                        return *new H5EnumData<unsigned char>(parent, totalSize, dataSize, ndims, dims, (unsigned char *)data, type, H5T_NATIVE_UCHAR, nmembers, names, stride, offset, dataOwner);
                    case 2:
                        return *new H5EnumData<unsigned short>(parent, totalSize, dataSize, ndims, dims, (unsigned short *)data, type, H5T_NATIVE_USHORT, nmembers, names, stride, offset, dataOwner);
                    case 4:
                        return *new H5EnumData<unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned int *)data, type, H5T_NATIVE_UINT, nmembers, names, stride, offset, dataOwner);
#ifdef __SCILAB_INT64__
                    case 8:
                        return *new H5EnumData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data, type, H5T_NATIVE_ULLONG, nmembers, names, stride, offset, dataOwner);
#endif
                }
            }
            else
            {
                switch (dataSize)
                {
                    case 1:
                        return *new H5EnumData<char>(parent, totalSize, dataSize, ndims, dims, (char *)data, type, H5T_NATIVE_CHAR, nmembers, names, stride, offset, dataOwner);
                    case 2:
                        return *new H5EnumData<short>(parent, totalSize, dataSize, ndims, dims, (short *)data, type, H5T_NATIVE_SHORT, nmembers, names, stride, offset, dataOwner);
                    case 4:
                        return *new H5EnumData<int>(parent, totalSize, dataSize, ndims, dims, (int *)data, type, H5T_NATIVE_INT, nmembers, names, stride, offset, dataOwner);
#ifdef __SCILAB_INT64__
                    case 8:
                        return *new H5EnumData<long long>(parent, totalSize, dataSize, ndims, dims, (long long *)data, type, H5T_NATIVE_LLONG, nmembers, names, stride, offset, dataOwner);
#endif
                }
            }

            return *new H5EnumData<char>(parent, totalSize, dataSize, ndims, dims, (char *)data, type, H5T_NATIVE_CHAR, nmembers, names, stride, offset, dataOwner);
        }
        case H5T_VLEN:
            return *new H5VlenData(parent, totalSize, dataSize, ndims, dims, static_cast<char *>(data), type, stride, offset, dataOwner);
        case H5T_ARRAY:
            return *new H5ArrayData(parent, totalSize, dataSize, ndims, dims, static_cast<char *>(data), type, stride, offset, dataOwner);
        default:
            throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
    }

    throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
}

void H5DataFactory::getNativeData(const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, hsize_t * totalSize, hsize_t * dataSize, hsize_t * ndims, hsize_t ** dims, void ** data, const bool isAttribute)
{
    hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);
    hid_t _space = space < 0 ? (isAttribute ? H5Aget_space(obj) : H5Dget_space(obj)) : space;
    hsize_t size = H5Tget_size(nativeType);
    H5S_sel_type sel;
    hid_t targetspace;
    herr_t err;
    hsize_t * blockbuf = 0;
    bool hyperslab = false;
    bool isString = false;

    *totalSize = 1;
    if (H5Tget_class(nativeType) == H5T_STRING && !H5Tis_variable_str(nativeType))
    {
        // We have a C-string so it is null terminated
        size++;
        isString = true;
    }

    *dataSize = size;
    *ndims = H5Sget_simple_extent_dims(_space, 0, 0);
    *dims = new hsize_t[*ndims];

    if (isAttribute)
    {
        H5Sget_simple_extent_dims(_space, *dims, 0);
        for (unsigned int i = 0; i < *ndims; i++)
        {
            *totalSize *= (*dims)[i];
        }
    }
    else
    {
        sel = H5Sget_select_type(_space);
        switch (sel)
        {
            case H5S_SEL_NONE:
            case H5S_SEL_ALL:
                H5Sget_simple_extent_dims(_space, *dims, 0);
                for (unsigned int i = 0; i < *ndims; i++)
                {
                    *totalSize *= (*dims)[i];
                }
                break;
            case H5S_SEL_POINTS:
                break;
            case H5S_SEL_HYPERSLABS:
                for (unsigned int i = 0; i < *ndims; i++)
                {
                    (*dims)[i] = selectdims[i];
                    *totalSize *= (*dims)[i];
                }
                hyperslab = true;
                break;
            default:
                break;
        }
    }

    size *= *totalSize;

    if ((hsize_t)((size_t)size) != size)
    {
        H5Tclose(nativeType);
        if (space < 0)
        {
            H5Sclose(_space);
        }
        delete[] *dims;
        throw H5Exception(__LINE__, __FILE__, _("Memory to allocate is too big"));
    }

    try
    {
        if (isString)
        {
            *data = static_cast<void *>(new char[(size_t)size]());
        }
        else
        {
            // No need to initialize the array
            *data = static_cast<void *>(new char[(size_t)size]);
        }
    }
    catch (const std::bad_alloc & /*e*/)
    {
        H5Tclose(nativeType);
        if (space < 0)
        {
            H5Sclose(_space);
        }
        *data = 0;
        delete[] *dims;
        *dims = 0;
        throw H5Exception(__LINE__, __FILE__, _("Cannot allocate memory to get the data"));
    }

    if (!*data)
    {
        H5Tclose(nativeType);
        if (space < 0)
        {
            H5Sclose(_space);
        }
        delete[] *dims;
        *dims = 0;
        throw H5Exception(__LINE__, __FILE__, _("Cannot allocate memory to get the data"));
    }

    if (hyperslab)
    {
        targetspace =  H5Screate_simple((int) * ndims, *dims, 0);
        err = H5Dread(obj, nativeType, targetspace, _space, H5P_DEFAULT, *data);
        H5Sclose(targetspace);
    }
    else
    {
        if (isAttribute)
        {
            err = H5Aread(obj, nativeType, *data);
        }
        else
        {
            err = H5Dread(obj, nativeType, H5S_ALL, H5S_ALL, H5P_DEFAULT, *data);
        }
    }

    if (err < 0)
    {
        H5Tclose(nativeType);
        if (space < 0)
        {
            H5Sclose(_space);
        }
        delete[] static_cast<char *>(*data);
        *data = 0;
        delete[] *dims;
        *dims = 0;
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve the data from the attribute"));
    }

    H5Tclose(nativeType);
    if (space < 0)
    {
        H5Sclose(_space);
    }
}
}
