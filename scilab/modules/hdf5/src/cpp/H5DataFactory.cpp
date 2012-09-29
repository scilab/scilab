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

#include "H5DataFactory.hxx"

namespace org_modules_hdf5
{

H5Data & H5DataFactory::getData(H5Object & parent, const hid_t obj, H5Dataspace * space, hsize_t * selectdims, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t * dims = 0;
    hsize_t arank = 0;
    hsize_t * adims = 0;
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
        getNativeData(obj, spaceId, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &arank, &adims, &data, isAttribute);
        H5Data & ret = getObjectData(parent, totalSize, dataSize, nativeType, ndims, dims, arank, adims, data, -1, 0, true);
        H5Tclose(type);
        H5Tclose(nativeType);

        return ret;
    }
    catch (const H5Exception & e)
    {
        H5Tclose(type);
        H5Tclose(nativeType);
        if (dims)
        {
            delete[] dims;
        }
        if (adims)
        {
            delete[] adims;
        }
        if (data)
        {
            delete[] static_cast<char *>(data);
        }
        throw;
    }
}

H5Data & H5DataFactory::getObjectData(H5Object & parent, const hsize_t totalSize, const hsize_t dataSize, const hid_t type, hsize_t ndims, hsize_t * dims, const hsize_t arank, const hsize_t * adims, void * data, const hsize_t stride, const size_t offset, const bool dataOwner)
{
    switch (H5Tget_class(type))
    {
        case H5T_INTEGER:
            if (H5Tequal(type, H5T_NATIVE_SCHAR))
            {
                return *new H5CharData(parent, totalSize, dataSize, ndims, dims, arank, adims, (char *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_UCHAR))
            {
                return *new H5UnsignedCharData(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned char *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_SHORT))
            {
                return *new H5BasicData<short>(parent, totalSize, dataSize, ndims, dims, arank, adims, (short *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_USHORT))
            {
                return *new H5BasicData<unsigned short>(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned short *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_INT))
            {
                return *new H5BasicData<int>(parent, totalSize, dataSize, ndims, dims, arank, adims, (int *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT))
            {
                return *new H5BasicData<unsigned int>(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned int *)data, stride, offset, dataOwner);
            }

#ifdef __SCILAB_INT64__

            else if (H5Tequal(type, H5T_NATIVE_LONG))
            {
                return *new H5BasicData<long long>(parent, totalSize, dataSize, ndims, dims, arank, adims, (long long *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG))
            {
                return *new H5BasicData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned long long *)data, stride, offset, dataOwner);
            }
#else

            else if (H5Tequal(type, H5T_NATIVE_LONG))
            {
                return *new H5TransformedData<long long, int>(parent, totalSize, dataSize, ndims, dims, arank, adims, (long long *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG))
            {
                return *new H5TransformedData<unsigned long long, unsigned int>(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned long long *)data, stride, offset, dataOwner);
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
                return *new H5TransformedData<float, double>(parent, totalSize, dataSize, ndims, dims, arank, adims, (float *)data, stride, offset, dataOwner);
            }
            else if (H5Tequal(type, H5T_NATIVE_DOUBLE))
            {
                return *new H5BasicData<double>(parent, totalSize, dataSize, ndims, dims, arank, adims, (double *)data, stride, offset, dataOwner);
            }
            else
            {
                throw H5Exception(__LINE__, __FILE__, _("Unknown floating-point datatype."));
            }
            break;
        case H5T_TIME:
            return *new H5TimeData(parent, totalSize, dataSize, ndims, dims, arank, adims, (char *)data, stride, offset, dataOwner);
        case H5T_STRING:
            if (H5Tis_variable_str(type))
            {
                return *new H5StringData(parent, totalSize, dataSize, ndims, dims, arank, adims, (char **)data, stride, offset, dataOwner);
            }
            else
            {
                return *new H5StringData(parent, totalSize, dataSize, ndims, dims, arank, adims, (char *)data, stride, offset, dataOwner);
            }
        case H5T_BITFIELD:
            switch (dataSize)
            {
                case 1:
                    return *new H5Bitfield1Data(parent, totalSize, dataSize, ndims, dims, arank, adims, static_cast<unsigned char *>(data), stride, offset, dataOwner);
                case 2:
                    return *new H5Bitfield2Data(parent, totalSize, dataSize, ndims, dims, arank, adims, static_cast<unsigned short *>(data), stride, offset, dataOwner);
                case 4:
                    return *new H5Bitfield4Data(parent, totalSize, dataSize, ndims, dims, arank, adims, static_cast<unsigned int *>(data), stride, offset, dataOwner);
                case 8:
                    //return *new H5BitfieldData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned long long *>(data), stride, offset, false);
                default:
                    throw H5Exception(__LINE__, __FILE__, _("Bitfield is too big"));
            }

        case H5T_OPAQUE:
            return *new H5OpaqueData(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned char *)data, stride, offset, dataOwner);
        case H5T_COMPOUND:
        {
            const unsigned int nmembers = (unsigned int)H5Tget_nmembers(type);
            std::string * names = new std::string[nmembers];
            size_t offs;
            H5Data ** fields = new H5Data *[nmembers];

            for (unsigned int i = 0; i < nmembers; i++)
            {
                hid_t mtype = H5Tget_member_type(type, i);
                char * mname = H5Tget_member_name(type, i);
                size_t offs = H5Tget_member_offset(type, i);
                names[i] = std::string(mname);
                free(mname);
                fields[i] = &getObjectData(parent, totalSize, dataSize, mtype, ndims, dims, arank, adims, data, stride, offset + offs, false);
            }

            return *new H5CompoundData(parent, totalSize, dataSize, ndims, dims, arank, adims, nmembers, names, fields, (char *)data, dataOwner);
        }
        case H5T_REFERENCE:
            // TODO: virer le false
            return *new H5ReferenceData(parent, false, totalSize, dataSize, ndims, dims, arank, adims, (char *)data, stride, offset, dataOwner);
        case H5T_ENUM:
        {
            int nmembers = H5Tget_nmembers(type);
            std::string * names = nmembers > 0 ? new std::string[nmembers] : 0;

            for (unsigned int i = 0; i < nmembers; i++)
            {
                char * mname = H5Tget_member_name(type, i);
                names[i] = std::string(mname);
                free(mname);
            }

            return *new H5EnumData(parent, totalSize, dataSize, ndims, dims, arank, adims, (unsigned int *)data, nmembers, names, stride, offset, dataOwner);
        }
        case H5T_VLEN:
            //return *new H5VlenData(parent, totalSize, dataSize, ndims, dims, data, offset);
        case H5T_ARRAY:
        {
            hid_t super = H5Tget_super(type);
            try
            {
                H5Data & ret = getObjectData(parent, totalSize, dataSize, super, ndims, dims, arank, adims, data, stride, offset, dataOwner);
                H5Tclose(super);
                return ret;
            }
            catch (const H5Exception & e)
            {
                H5Tclose(super);
                throw;
            }
        }
        default:
            throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
    }

    throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
}

void H5DataFactory::getNativeData(const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, hsize_t * totalSize, hsize_t * dataSize, hsize_t * ndims, hsize_t ** dims, hsize_t * arank, hsize_t ** adims, void ** data, const bool isAttribute)
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

    if (H5Tget_class(nativeType) == H5T_ARRAY)
    {
        *arank = H5Tget_array_ndims(nativeType);
        *adims = new hsize_t[*arank];
        H5Tget_array_dims(nativeType, *adims);
    }

    *dataSize = size;
    *ndims = H5Sget_simple_extent_dims(_space, 0, 0);
    *dims = new hsize_t[*ndims];

    if (isAttribute)
    {
        H5Sget_simple_extent_dims(_space, *dims, 0);
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

    if (isString)
    {
        *data = static_cast<void *>(new char[(size_t)size]());
    }
    else
    {
        // No need to initialize the array
        *data = static_cast<void *>(new char[(size_t)size]);
    }

    if (!*data)
    {
        H5Tclose(nativeType);
        if (space < 0)
        {
            H5Sclose(_space);
        }
        delete[] *dims;
        throw H5Exception(__LINE__, __FILE__, _("Cannot allocate memory to get the data"));
    }

    if (hyperslab)
    {
        targetspace =  H5Screate_simple(*ndims, *dims, 0);
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
        delete[] *dims;
        throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve the data from the attribute"));
    }

    H5Tclose(nativeType);
    if (space < 0)
    {
        H5Sclose(_space);
    }
}
}
