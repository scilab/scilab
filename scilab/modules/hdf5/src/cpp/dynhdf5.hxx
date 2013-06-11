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

#ifndef __DYNHDF5_HXX__
#define __DYNHDF5_HXX__

extern "C"
{
#include <hdf5.h>
#include "dynamiclibrary.h"
}


//static class to load HDF5 EntryPoint
class DynHDF5
{
private :
    static DynLibHandle m_hLib;
public :
    static hid_t dynH5T_NATIVE_INT8_g;
    static hid_t dynH5T_NATIVE_UINT8_g;
    static hid_t dynH5T_NATIVE_INT16_g;
    static hid_t dynH5T_NATIVE_UINT16_g;
    static hid_t dynH5T_NATIVE_INT32_g;
    static hid_t dynH5T_NATIVE_UINT32_g;
    static hid_t dynH5T_NATIVE_INT64_g;
    static hid_t dynH5T_NATIVE_UINT64_g;
    static hid_t dynH5T_NATIVE_DOUBLE_g;
    static hid_t dynH5P_CLS_FILE_ACCESS_g;
    static hid_t dynH5T_NATIVE_INT_g;
    static hid_t dynH5T_C_S1_g;
    static hid_t dynH5T_STD_REF_OBJ_g;

    static int dynOpenLib();
    static int dynCLoseLib();

    static herr_t dynH5Check()
    {
        return dynH5check_version(H5_VERS_MAJOR, H5_VERS_MINOR, H5_VERS_RELEASE);
    }
    static herr_t dynH5open(void);

    //File management
    static hid_t dynH5Pcreate(hid_t cls_id);
    static hid_t dynH5Fcreate(const char *filename, unsigned flags, hid_t create_plist, hid_t access_plist);
    static hid_t dynH5Fopen(const char *filename, unsigned flags, hid_t access_plist);
    static herr_t dynH5Fclose(hid_t file_id);
    static herr_t dynH5check_version(unsigned majnum, unsigned minnum, unsigned relnum);

    //read operations
    static hid_t dynH5Aopen_name(hid_t loc_id, const char *name);
    static herr_t dynH5Aread(hid_t attr_id, hid_t type_id, void *buf);
    static herr_t dynH5Aclose(hid_t attr_id);
    static herr_t dynH5Aiterate(hid_t loc_id, unsigned *attr_num, H5A_operator1_t op, void *op_data);
    static hid_t dynH5Aget_type(hid_t attr_id);
    static size_t dynH5Tget_size(hid_t type_id);
    static hid_t dynH5Aget_space(hid_t attr_id);
    static int dynH5Sget_simple_extent_dims(hid_t space_id, hsize_t dims[], hsize_t maxdims[]);
    static hid_t dynH5Tcopy(hid_t type_id);
    static herr_t dynH5Tset_size(hid_t type_id, size_t size);
    static herr_t dynH5Tclose(hid_t type_id);
    static herr_t dynH5Sclose(hid_t space_id);
    static herr_t dynH5Gget_num_objs(hid_t loc_id, hsize_t *num_objs);
    static H5G_obj_t dynH5Gget_objtype_by_idx(hid_t loc_id, hsize_t idx);
    static ssize_t dynH5Gget_objname_by_idx(hid_t loc_id, hsize_t idx, char* name, size_t size);
    static hid_t dynH5Dopen(hid_t file_id, const char *name);
    static herr_t dynH5Giterate(hid_t loc_id, const char *name, int *idx, H5G_iterate_t op, void *op_data);
    static herr_t dynH5Dclose(hid_t dset_id);
    static hid_t dynH5Rdereference(hid_t dataset, H5R_type_t ref_type, const void *ref);
    static hid_t dynH5Dget_type(hid_t dset_id);
    static hid_t dynH5Dget_space(hid_t dset_id);
    static hid_t dynH5Screate_simple(int rank, const hsize_t dims[], const hsize_t maxdims[]);
    static herr_t dynH5Sselect_hyperslab(hid_t space_id, H5S_seloper_t op, const hsize_t start[], const hsize_t _stride[], const hsize_t count[], const hsize_t _block[]);
    static herr_t dynH5Dread(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, void *buf/*out*/);

    //write functions
    static hid_t dynH5Acreate/*H5Acreate1*/(hid_t loc_id, const char *name, hid_t type_id, hid_t space_id, hid_t acpl_id);
    static herr_t  dynH5Awrite(hid_t attr_id, hid_t type_id, const void *buf);
    static hid_t dynH5Dcreate/*H5Dcreate1*/(hid_t file_id, const char *name, hid_t type_id, hid_t space_id, hid_t dcpl_id);
    static herr_t dynH5Dwrite(hid_t dset_id, hid_t mem_type_id, hid_t mem_space_id, hid_t file_space_id, hid_t plist_id, const void *buf);
    static herr_t dynH5Rcreate(void *ref, hid_t loc_id, const char *name, H5R_type_t ref_type, hid_t space_id);
    static hid_t dynH5Gcreate/*H5Gcreate1*/(hid_t loc_id, const char *name, size_t size_hint);
    static herr_t dynH5Gclose(hid_t group_id);

};

#undef H5F_ACC_TRUNC
#define H5F_ACC_TRUNC (DynHDF5::dynH5Check(), 0x0002u)

#undef H5F_ACC_RDONLY
#define H5F_ACC_RDONLY (DynHDF5::dynH5Check(), 0x0000u)

#undef H5P_FILE_ACCESS
#define H5P_FILE_ACCESS (DynHDF5::dynH5open(), DynHDF5::dynH5P_CLS_FILE_ACCESS_g)

#undef H5T_NATIVE_INT
#define H5T_NATIVE_INT (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_INT_g)

#undef H5T_C_S1
#define H5T_C_S1 (DynHDF5::dynH5open(), DynHDF5::dynH5T_C_S1_g)

#undef H5T_NATIVE_DOUBLE
#define H5T_NATIVE_DOUBLE (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_DOUBLE_g)

#undef H5T_STD_REF_OBJ
#define H5T_STD_REF_OBJ (DynHDF5::dynH5open(), DynHDF5::dynH5T_STD_REF_OBJ_g)

#undef H5T_NATIVE_INT8
#define H5T_NATIVE_INT8 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_INT8_g)

#undef H5T_NATIVE_UINT8
#define H5T_NATIVE_UINT8 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_UINT8_g)

#undef H5T_NATIVE_INT16
#define H5T_NATIVE_INT16 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_INT16_g)

#undef H5T_NATIVE_UINT16
#define H5T_NATIVE_UINT16 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_UINT16_g)

#undef H5T_NATIVE_INT32
#define H5T_NATIVE_INT32 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_INT32_g)

#undef H5T_NATIVE_UINT32
#define H5T_NATIVE_UINT32 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_UINT32_g)

#undef H5T_NATIVE_INT64
#define H5T_NATIVE_INT64 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_INT64_g)

#undef H5T_NATIVE_UINT64
#define H5T_NATIVE_UINT64 (DynHDF5::dynH5open(), DynHDF5::dynH5T_NATIVE_UINT64_g)

#endif /* ! __DYNHDF5_HXX__ */
