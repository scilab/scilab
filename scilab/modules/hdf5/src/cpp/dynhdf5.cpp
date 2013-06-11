/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "dynhdf5.hxx"
#include "MALLOC.h"

DynLibHandle DynHDF5::m_hLib = NULL;
hid_t DynHDF5::dynH5T_NATIVE_INT8_g;
hid_t DynHDF5::dynH5T_NATIVE_UINT8_g;
hid_t DynHDF5::dynH5T_NATIVE_INT16_g;
hid_t DynHDF5::dynH5T_NATIVE_UINT16_g;
hid_t DynHDF5::dynH5T_NATIVE_INT32_g;
hid_t DynHDF5::dynH5T_NATIVE_UINT32_g;
hid_t DynHDF5::dynH5T_NATIVE_INT64_g;
hid_t DynHDF5::dynH5T_NATIVE_UINT64_g;
hid_t DynHDF5::dynH5T_NATIVE_DOUBLE_g;
hid_t DynHDF5::dynH5P_CLS_FILE_ACCESS_g;
hid_t DynHDF5::dynH5T_NATIVE_INT_g;
hid_t DynHDF5::dynH5T_C_S1_g;
hid_t DynHDF5::dynH5T_STD_REF_OBJ_g;

int DynHDF5::dynOpenLib()
{
    if (m_hLib == NULL)
    {
#ifdef _MSC_VER
        m_hLib = LoadDynLibrary("hdf5dll.dll");
#else
        m_hLib = LoadDynLibrary("libhdf5.so");

#endif
        if (m_hLib == NULL)
        {
            return 1;
        }

        dynH5open();
        dynH5T_NATIVE_INT8_g        = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_INT8_g");
        dynH5T_NATIVE_UINT8_g       = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_UINT8_g");
        dynH5T_NATIVE_INT16_g       = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_INT16_g");
        dynH5T_NATIVE_UINT16_g      = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_UINT16_g");
        dynH5T_NATIVE_INT32_g       = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_INT32_g");
        dynH5T_NATIVE_UINT32_g      = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_UINT32_g");
        dynH5T_NATIVE_INT64_g       = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_INT64_g");
        dynH5T_NATIVE_UINT64_g      = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_UINT64_g");
        dynH5T_NATIVE_DOUBLE_g      = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_DOUBLE_g");
        dynH5P_CLS_FILE_ACCESS_g    = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5P_CLS_FILE_ACCESS_g");
        dynH5T_NATIVE_INT_g         = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_NATIVE_INT_g");
        dynH5T_C_S1_g               = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_C_S1_g");
        dynH5T_STD_REF_OBJ_g        = *(hid_t*)GetDynLibFuncPtr(m_hLib, "H5T_STD_REF_OBJ_g");
    }

    return 0;
}

int DynHDF5::dynCLoseLib()
{
    if (m_hLib)
    {
        FreeDynLibrary(m_hLib);
        m_hLib = NULL;
    }
    return 0;
}

herr_t DynHDF5::dynH5open(void)
{
    typedef herr_t (*HDF5Func)(void);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5open");
    return pFunc();

}

hid_t DynHDF5::dynH5Pcreate(hid_t cls_id)
{
    typedef hid_t (*HDF5Func)(hid_t cls_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Pcreate");
    return pFunc(cls_id);

}

hid_t DynHDF5::dynH5Fcreate(const char *filename, unsigned flags, hid_t create_plist, hid_t access_plist)
{
    typedef hid_t (*HDF5Func)(const char * filename, unsigned flags, hid_t create_plist, hid_t access_plist);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Fcreate");
    return pFunc(filename, flags, create_plist, access_plist);
}

hid_t DynHDF5::dynH5Fopen(const char *filename, unsigned flags, hid_t access_plist)
{
    typedef hid_t (*HDF5Func)(const char * filename, unsigned flags, hid_t access_plist);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Fopen");
    return pFunc(filename, flags, access_plist);
}

herr_t DynHDF5::dynH5Fclose(hid_t file_id)
{
    typedef herr_t (*HDF5Func)(hid_t file_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Fclose");
    return pFunc(file_id);
}

herr_t DynHDF5::dynH5check_version(unsigned majnum, unsigned minnum, unsigned relnum)
{
    typedef herr_t (*HDF5Func)(unsigned majnum, unsigned minnum, unsigned relnum);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5check_version");
    return pFunc(majnum, minnum, relnum);
}

hid_t DynHDF5::dynH5Aopen_name(hid_t loc_id, const char *name)
{
    typedef hid_t (*HDF5Func)(hid_t loc_id, const char * name);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Aopen_name");
    return pFunc(loc_id, name);
}

herr_t DynHDF5::dynH5Aread(hid_t attr_id, hid_t type_id, void *buf)
{
    typedef herr_t (*HDF5Func)(hid_t attr_id, hid_t type_id, void * buf);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Aread");
    return pFunc(attr_id, type_id, buf);
}

herr_t DynHDF5::dynH5Aclose(hid_t attr_id)
{
    typedef herr_t (*HDF5Func)(hid_t attr_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Aclose");
    return pFunc(attr_id);
}

herr_t DynHDF5::dynH5Aiterate(hid_t loc_id, unsigned *attr_num, H5A_operator1_t op, void *op_data)
{
    typedef herr_t (*HDF5Func)(hid_t loc_id, unsigned * attr_num, H5A_operator1_t op, void * op_data);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Aiterate1");
    return pFunc(loc_id, attr_num, op, op_data);
}

hid_t DynHDF5::dynH5Aget_type(hid_t attr_id)
{
    typedef hid_t (*HDF5Func)(hid_t attr_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Aget_type");
    return pFunc(attr_id);
}

size_t DynHDF5::dynH5Tget_size(hid_t type_id)
{
    typedef size_t (*HDF5Func)(hid_t type_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Tget_size");
    return pFunc(type_id);
}

hid_t DynHDF5::dynH5Aget_space(hid_t attr_id)
{
    typedef hid_t (*HDF5Func)(hid_t attr_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Aget_space");
    return pFunc(attr_id);
}

int DynHDF5::dynH5Sget_simple_extent_dims(hid_t space_id, hsize_t dims[], hsize_t maxdims[])
{
    typedef int (*HDF5Func)(hid_t space_id, hsize_t dims[], hsize_t maxdims[]);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Sget_simple_extent_dims");
    return pFunc(space_id, dims, maxdims);
}

hid_t DynHDF5::dynH5Tcopy(hid_t type_id)
{
    typedef hid_t (*HDF5Func)(hid_t type_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Tcopy");
    return pFunc(type_id);
}

herr_t DynHDF5::dynH5Tset_size(hid_t type_id, size_t size)
{
    typedef herr_t (*HDF5Func)(hid_t type_id, size_t size);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Tset_size");
    return pFunc(type_id, size);
}

herr_t DynHDF5::dynH5Tclose(hid_t type_id)
{
    typedef herr_t (*HDF5Func)(hid_t type_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Tclose");
    return pFunc(type_id);
}

herr_t DynHDF5::dynH5Sclose(hid_t space_id)
{
    typedef herr_t (*HDF5Func)(hid_t space_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Sclose");
    return pFunc(space_id);
}

herr_t DynHDF5::dynH5Gget_num_objs(hid_t loc_id, hsize_t *num_objs)
{
    typedef herr_t (*HDF5Func)(hid_t loc_id, hsize_t * num_objs);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Gget_num_objs");
    return pFunc(loc_id, num_objs);
}

H5G_obj_t DynHDF5::dynH5Gget_objtype_by_idx(hid_t loc_id, hsize_t idx)
{
    typedef H5G_obj_t (*HDF5Func)(hid_t loc_id, hsize_t idx);
    if (dynOpenLib())
    {
        return H5G_UNKNOWN;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Gget_objtype_by_idx");
    return pFunc(loc_id, idx);
}

ssize_t DynHDF5::dynH5Gget_objname_by_idx(hid_t loc_id, hsize_t idx, char* name, size_t size)
{
    typedef ssize_t (*HDF5Func)(hid_t loc_id, hsize_t idx, char * name, size_t size);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Gget_objname_by_idx");
    return pFunc(loc_id, idx, name, size);
}

hid_t DynHDF5::dynH5Dopen(hid_t file_id, const char *name)
{
    typedef hid_t (*HDF5Func)(hid_t file_id, const char * name);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dopen1");
    return pFunc(file_id, name);
}

herr_t DynHDF5::dynH5Dclose(hid_t dset_id)
{
    typedef herr_t (*HDF5Func)(hid_t dset_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dclose");
    return pFunc(dset_id);
}

hid_t DynHDF5::dynH5Rdereference(hid_t dataset, H5R_type_t ref_type, const void *ref)
{
    typedef hid_t (*HDF5Func)(hid_t dataset, H5R_type_t ref_type, const void * ref);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Rdereference");
    return pFunc(dataset, ref_type, ref);
}

hid_t DynHDF5::dynH5Dget_type(hid_t dset_id)
{
    typedef hid_t (*HDF5Func)(hid_t dset_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dget_type");
    return pFunc(dset_id);
}

hid_t DynHDF5::dynH5Dget_space(hid_t dset_id)
{
    typedef hid_t (*HDF5Func)(hid_t dset_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dget_space");
    return pFunc(dset_id);
}

hid_t DynHDF5::dynH5Screate_simple(int rank, const hsize_t dims[], const hsize_t maxdims[])
{
    typedef hid_t (*HDF5Func)(int rank, const hsize_t dims[], const hsize_t maxdims[]);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Screate_simple");
    return pFunc(rank, dims, maxdims);
}

herr_t DynHDF5::dynH5Sselect_hyperslab(hid_t space_id, H5S_seloper_t op, const hsize_t start[], const hsize_t _stride[], const hsize_t count[], const hsize_t _block[])
{
    typedef herr_t (*HDF5Func)(hid_t space_id, H5S_seloper_t op, const hsize_t start[], const hsize_t _stride[], const hsize_t count[], const hsize_t _block[]);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Sselect_hyperslab");
    return pFunc(space_id, op, start, _stride, count, _block);
}

herr_t DynHDF5::dynH5Dread(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, void *buf/*out*/)
{
    typedef herr_t (*HDF5Func)(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, void * buf);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dread");
    return pFunc(dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
}

hid_t DynHDF5::dynH5Acreate/*H5Acreate1*/(hid_t loc_id, const char *name, hid_t type_id, hid_t space_id, hid_t acpl_id)
{
    typedef hid_t (*HDF5Func)(hid_t loc_id, const char * name, hid_t type_id, hid_t space_id, hid_t acpl_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Acreate1");
    return pFunc(loc_id, name, type_id, space_id, acpl_id);
}

herr_t  DynHDF5::dynH5Awrite(hid_t attr_id, hid_t type_id, const void *buf)
{
    typedef herr_t (*HDF5Func)(hid_t attr_id, hid_t type_id, const void * buf);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Awrite");
    return pFunc(attr_id, type_id, buf);
}

hid_t DynHDF5::dynH5Dcreate/*H5Dcreate1*/(hid_t file_id, const char *name, hid_t type_id, hid_t space_id, hid_t dcpl_id)
{
    typedef hid_t (*HDF5Func)(hid_t file_id, const char * name, hid_t type_id, hid_t space_id, hid_t dcpl_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dcreate1");
    return pFunc(file_id, name, type_id, space_id, dcpl_id);
}

herr_t DynHDF5::dynH5Dwrite(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, const void *buf)
{
    typedef herr_t (*HDF5Func)(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, const void * buf);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Dwrite");
    return pFunc(dset_id, mem_type_id, mem_space_id, file_space_id, plist_id, buf);
}

herr_t DynHDF5::dynH5Rcreate(void *ref, hid_t loc_id, const char *name, H5R_type_t ref_type, hid_t space_id)
{
    typedef herr_t (*HDF5Func)(void * ref, hid_t loc_id, const char * name, H5R_type_t ref_type, hid_t space_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Rcreate");
    return pFunc(ref, loc_id, name, ref_type, space_id);
}

hid_t DynHDF5::dynH5Gcreate/*H5Gcreate1*/(hid_t loc_id, const char *name, size_t size_hint)
{
    typedef hid_t (*HDF5Func)(hid_t loc_id, const char * name, size_t size_hint);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Gcreate1");
    return pFunc(loc_id, name, size_hint);
}

herr_t DynHDF5::dynH5Gclose(hid_t group_id)
{
    typedef herr_t (*HDF5Func)(hid_t group_id);
    if (dynOpenLib())
    {
        return 1;
    }

    HDF5Func pFunc = (HDF5Func)GetDynLibFuncPtr(m_hLib, "H5Gclose");
    return pFunc(group_id);
}
