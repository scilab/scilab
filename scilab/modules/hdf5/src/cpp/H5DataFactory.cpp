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
    const hid_t type = isAttribute ? H5Aget_type(obj) : H5Dget_type(obj);
    if (type < 0)
    {
        throw H5Exception(__LINE__, __FILE__, _("Cannot get the data type"));
    }
    H5Data * data = 0;
    hid_t spaceId = space ? space->getH5Id() : -1;

    try
    {
        switch (H5Tget_class(type))
        {
            case H5T_INTEGER:
                data = &getIntegerData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_FLOAT:
                data = &getFloatingData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_TIME:
                data = &getTimeData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_STRING:
                data = &getStringData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_BITFIELD:
                data = &getBitfieldData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_OPAQUE:
                data = &getOpaqueData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_COMPOUND:
                data = &getCompoundData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_REFERENCE:
                data = &getReferenceData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_ENUM:
                data = &getEnumData(parent, obj, spaceId, selectdims, type, isAttribute);
                break;
            case H5T_VLEN:
                //data = &getVlenData(parent, obj, spaceId, selectdims, type, isAttribute);
                //break;
            case H5T_ARRAY:
                std::cout << "ARRAY" << std::endl;
                //data = &getArrayData(parent, obj, type, isAttribute);
                //break;
            default:
                throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
        }
    }
    catch (const H5Exception & e)
    {
        H5Tclose(type);
        throw;
    }

    H5Tclose(type);

    return *data;
}

H5Data & H5DataFactory::getData(H5Object & parent, const hsize_t totalSize, const hid_t type, hsize_t ndims, hsize_t * dims, void * data, const hsize_t stride, const size_t offset, const bool dataOwner)
{
    hsize_t dataSize = H5Tget_size(type);
    if (H5Tget_class(type) == H5T_STRING && !H5Tis_variable_str(type))
    {
        // We have a C-string so it is null terminated
        dataSize++;
    }

    switch (H5Tget_class(type))
    {
        case H5T_INTEGER:
            if (H5Tequal(type, H5T_NATIVE_SCHAR))
            {
                return *new H5CharData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_UCHAR))
            {
                return *new H5UnsignedCharData(parent, totalSize, dataSize, ndims, dims, (unsigned char *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_SHORT))
            {
                return *new H5BasicData<short>(parent, totalSize, dataSize, ndims, dims, (short *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_USHORT))
            {
                return *new H5BasicData<unsigned short>(parent, totalSize, dataSize, ndims, dims, (unsigned short *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_INT))
            {
                return *new H5BasicData<int>(parent, totalSize, dataSize, ndims, dims, (int *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_UINT))
            {
                return *new H5BasicData<unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned int *)data, stride, offset, false);
            }

#ifdef __SCILAB_INT64__

            else if (H5Tequal(type, H5T_NATIVE_LONG))
            {
                return *new H5BasicData<long long>(parent, totalSize, dataSize, ndims, dims, (long long *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG))
            {
                return *new H5BasicData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data, stride, offset, false);
            }
#else

            else if (H5Tequal(type, H5T_NATIVE_LONG))
            {
                return *new H5TransformedData<long long, int>(parent, totalSize, dataSize, ndims, dims, (long long *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_ULONG))
            {
                return *new H5TransformedData<unsigned long long, unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data, stride, offset, false);
            }

#endif // __SCILAB_INT64__

            else
            {
                H5Tclose(type);
                throw H5Exception(__LINE__, __FILE__, _("Unknown integer datatype."));
            }
            break;
        case H5T_FLOAT:
            if (H5Tequal(type, H5T_NATIVE_FLOAT))
            {
                //return *new H5FloatData(parent, totalSize, dataSize, ndims, dims, (float *)data, stride, offset, false);
                return *new H5TransformedData<float, double>(parent, totalSize, dataSize, ndims, dims, (float *)data, stride, offset, false);
            }
            else if (H5Tequal(type, H5T_NATIVE_DOUBLE))
            {
                return *new H5BasicData<double>(parent, totalSize, dataSize, ndims, dims, (double *)data, stride, offset, false);
            }
            else
            {
                throw H5Exception(__LINE__, __FILE__, _("Unknown floating-point datatype."));
            }
            break;
        case H5T_TIME:
            return *new H5TimeData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, false);
        case H5T_STRING:
            if (H5Tis_variable_str(type))
            {
                return *new H5StringData(parent, totalSize, dataSize, ndims, dims, (char **)data, stride, offset, false);
            }
            else
            {
                return *new H5StringData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, false);
            }
        case H5T_BITFIELD:
            switch (dataSize)
            {
                case 1:
                    return *new H5Bitfield1Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned char *>(data), stride, offset, false);
                case 2:
                    return *new H5Bitfield2Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned short *>(data), stride, offset, false);
                case 4:
                    return *new H5Bitfield4Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned int *>(data), stride, offset, false);
                case 8:
                    //return *new H5BitfieldData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned long long *>(data), stride, offset, false);
                default:
                    throw H5Exception(__LINE__, __FILE__, _("Bitfield is too big"));
            }

            //return *new H5BitfieldData(parent, totalSize, dataSize, ndims, dims, (char *)data, stride, offset, false);
        case H5T_OPAQUE:
            return *new H5OpaqueData(parent, totalSize, dataSize, ndims, dims, (unsigned char *)data, stride, offset, false);
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
                fields[i] = &getData(parent, totalSize, mtype, ndims, dims, data, stride, offset + offs, false);
            }

            return *new H5CompoundData(parent, totalSize, dataSize, ndims, dims, nmembers, names, fields, (char *)data, dataOwner);
        }
        case H5T_REFERENCE:
            // TODO: virer le false
            return *new H5ReferenceData(parent, false, totalSize, dataSize, ndims, dims, (char *)data, offset);
        case H5T_ENUM:
        {
            /*int nmembers = H5Tget_nmembers(type);
              std::string * names = new std::string[nmembers];

              for (unsigned int i = 0; i < nmembers; i++)
              {
              char * mname = H5Tget_member_name(type, i);
              names[i] = std::string(mname);
              free(mname);
              }
              return *new H5EnumData(parent, totalSize, dataSize, ndims, dims, data, offset, names);*/
        }
        case H5T_VLEN:
            //return *new H5VlenData(parent, totalSize, dataSize, ndims, dims, data, offset);
        case H5T_ARRAY:
            //return *new H5ArrayData(parent, totalSize, dataSize, ndims, dims, data, offset);
        default:
            throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
    }

    throw H5Exception(__LINE__, __FILE__, _("Cannot get data from an unknown data type."));
}

H5Data & H5DataFactory::getIntegerData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t * dims = 0;
    hsize_t totalSize;
    hsize_t dataSize;
    void * data = 0;
    H5Data * dataObj = 0;
    const hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    if (H5Tequal(nativeType, H5T_NATIVE_SCHAR))
    {
        dataObj = new H5CharData(parent, totalSize, dataSize, ndims, dims, (char *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_UCHAR))
    {
        dataObj = new H5UnsignedCharData(parent, totalSize, dataSize, ndims, dims, (unsigned char *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_SHORT))
    {
        dataObj = new H5BasicData<short>(parent, totalSize, dataSize, ndims, dims, (short *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_USHORT))
    {
        dataObj = new H5BasicData<unsigned short>(parent, totalSize, dataSize, ndims, dims, (unsigned short *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_INT))
    {
        dataObj = new H5BasicData<int>(parent, totalSize, dataSize, ndims, dims, (int *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_UINT))
    {
        dataObj = new H5BasicData<unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned int *)data);
    }

#ifdef __SCILAB_INT64__

    else if (H5Tequal(nativeType, H5T_NATIVE_LONG))
    {
        dataObj = new H5BasicData<long long>(parent, totalSize, dataSize, ndims, dims, (long long *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_ULONG))
    {
        dataObj = new H5BasicData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data);
    }

#else

    else if (H5Tequal(nativeType, H5T_NATIVE_LONG))
    {
        dataObj = new H5TransformedData<long long, int>(parent, totalSize, dataSize, ndims, dims, (long long *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_ULONG))
    {
        dataObj = new H5TransformedData<unsigned long long, unsigned int>(parent, totalSize, dataSize, ndims, dims, (unsigned long long *)data);
    }

#endif // __SCILAB_INT64__

    else
    {
        H5Tclose(nativeType);
        delete[] dims;
        delete[] static_cast<char *>(data);
        throw H5Exception(__LINE__, __FILE__, _("Unknown integer datatype."));
    }

    H5Tclose(nativeType);

    return *dataObj;
}

H5Data & H5DataFactory::getFloatingData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;
    H5Data * dataObj = 0;
    const hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    if (H5Tequal(nativeType, H5T_NATIVE_FLOAT))
    {
        dataObj = new H5TransformedData<float, double>(parent, totalSize, dataSize, ndims, dims, (float *)data);
    }
    else if (H5Tequal(nativeType, H5T_NATIVE_DOUBLE))
    {
        dataObj = new H5BasicData<double>(parent, totalSize, dataSize, ndims, dims, (double *)data);
    }
    else
    {
        H5Tclose(nativeType);
        delete[] dims;
        delete[] static_cast<char *>(data);
        throw H5Exception(__LINE__, __FILE__, _("Unknown floating-point datatype."));
    }

    H5Tclose(nativeType);

    return *dataObj;
}

H5StringData & H5DataFactory::getStringData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);
    if (H5Tis_variable_str(type))
    {
        return *new H5StringData(parent, totalSize, dataSize, ndims, dims, (char **)data);
    }
    else
    {
        return *new H5StringData(parent, totalSize, dataSize, ndims, dims, (char *)data);
    }
}

H5TimeData & H5DataFactory::getTimeData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    return *new H5TimeData(parent, totalSize, dataSize, ndims, dims, static_cast<char *>(data));
}

H5Data & H5DataFactory::getBitfieldData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    switch (dataSize)
    {
        case 1:
            return *new H5Bitfield1Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned char *>(data));
        case 2:
            return *new H5Bitfield2Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned short *>(data));
        case 4:
            return *new H5Bitfield4Data(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned int *>(data));
        case 8:
            ///return *new H5BitfieldData<unsigned long long>(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned long long *>(data));
        default:
            delete[] dims;
            delete[] static_cast<char *>(data);
            throw H5Exception(__LINE__, __FILE__, _("Bitfield is too big"));
    }
}

H5OpaqueData & H5DataFactory::getOpaqueData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    return *new H5OpaqueData(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned char *>(data));
}

H5Data & H5DataFactory::getCompoundData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;
    const hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    try
    {
        return getData(parent, totalSize, nativeType, ndims, dims, data, dataSize, 0, true);
    }
    catch (const H5Exception & e)
    {
        H5Tclose(nativeType);
        throw;
    }
}

H5ReferenceData & H5DataFactory::getReferenceData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);
    return *new H5ReferenceData(parent, H5Tequal(type, H5T_STD_REF_DSETREG) > 0, totalSize, dataSize, ndims, dims, (char *)data);
}

H5EnumData & H5DataFactory::getEnumData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
{
    hsize_t ndims;
    hsize_t totalSize;
    hsize_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;
    int nmembers = H5Tget_nmembers(type);
    std::string * names = nmembers > 0 ? new std::string[nmembers] : 0;

    for (unsigned int i = 0; i < nmembers; i++)
    {
        char * mname = H5Tget_member_name(type, i);
        names[i] = std::string(mname);
        free(mname);
    }

    getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    return *new H5EnumData(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned int *>(data), nmembers, names);
}

/*    H5VlenData & H5DataFactory::getVlenData(H5Object & parent, const hid_t obj, const hid_t space, hsize_t * selectdims, const hid_t type, const bool isAttribute)
    {
        hsize_t ndims;
        hsize_t totalSize;
        hsize_t dataSize;
        hsize_t * dims = 0;
        void * data = 0;

        getNativeData(obj, space, selectdims, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

        return *new H5VlenData(parent, totalSize, dataSize, ndims, dims, static_cast<unsigned char *>(data));
    }

  H5ArrayData & H5DataFactory::getArrayData(H5Object & parent, const hid_t obj, const hid_t type, const bool isAttribute)
    {
    hsize_t ndims;
    hsize_t totalSize;
    hisze_t dataSize;
    hsize_t * dims = 0;
    void * data = 0;

    getNativeData(obj, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

    return *new H5ArrayData(parent, totalSize, dataSize, ndims, dims, data);
    }*/

//  getNativeData(obj, type, &totalSize, &dataSize, &ndims, &dims, &data, isAttribute);

//return *new H5StringData(parent, totalSize, dataSize, ndims, dims, data);

/*void H5DataFactory::getNativeData(const hid_t obj, const hid_t type, hsize_t * totalSize, hsize_t * dataSize, hsize_t * ndims, hsize_t ** dims, void ** data, const bool isAttribute)
  {
  hid_t nativeType = H5Tget_native_type(type, H5T_DIR_DEFAULT);
  const hid_t space = isAttribute ? H5Aget_space(obj) : H5Dget_space(obj);
  hsize_t size = H5Tget_size(nativeType);
  *totalSize = 1;
  *dims = new hsize_t[__SCILAB_HDF5_MAX_DIMS__]();
  *ndims = H5Sget_simple_extent_dims(space, *dims, 0);

  if (H5Tget_class(nativeType) == H5T_STRING && !H5Tis_variable_str(nativeType))
  {
  // We have a C-string so it is null terminated
  size++;
  }

  *dataSize = size;

  for (unsigned int i = 0; i < *ndims; i++)
  {
  *totalSize *= (*dims)[i];
  }

  size *= *totalSize;

  if ((hsize_t)((size_t)size) != size)
  {
  H5Tclose(type);
  H5Tclose(nativeType);
  H5Sclose(space);
  delete[] *dims;
  throw H5Exception(__LINE__, __FILE__, _("Memory to allocate is too big"));
  }

  *data = static_cast<void *>(new char[(size_t)size]());
  if (!*data)
  {
  H5Tclose(type);
  H5Tclose(nativeType);
  H5Sclose(space);
  delete[] *dims;
  throw H5Exception(__LINE__, __FILE__, _("Cannot allocate memory to get the data"));
  }

  if ((isAttribute && H5Aread(obj, nativeType, *data) < 0)
  || (!isAttribute && H5Dread(obj, nativeType, H5S_ALL, space, H5P_DEFAULT, *data) < 0))
  {
  H5Tclose(type);
  H5Tclose(nativeType);
  H5Sclose(space);
  delete[] static_cast<char *>(*data);
  delete[] *dims;
  throw H5Exception(__LINE__, __FILE__, _("Cannot retrieve the data from the attribute"));
  }

  H5Tclose(nativeType);
  H5Sclose(space);
  }
*/

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

    *totalSize = 1;
    if (H5Tget_class(nativeType) == H5T_STRING && !H5Tis_variable_str(nativeType))
    {
        // We have a C-string so it is null terminated
        size++;
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

    *data = static_cast<void *>(new char[(size_t)size]());
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
