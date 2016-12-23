/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2012 - DIGITEO - Antoine ELIAS
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

#include <vector>
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"
#include "list.hxx"
#include "hdf5_gw.hxx"
#include "configvariable.hxx"

extern "C"
{
#include <hdf5.h>
#include "sci_types.h" //sci_matrix, sci_ints, ...
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile.h"
#include "h5_writeDataToFile.h"
#include "h5_attributeConstants.h"
#include "expandPathVariable.h"
}

typedef struct __VAR_INFO6__
{
    char info[128];
    std::string name;
    std::string ctype;
    int type;
    int size;
    int dims;
    std::vector<int> pdims;
} VarInfo6;

static bool read_data(int dataset, VarInfo6& info);
static bool read_short_data(int dataset, VarInfo6& info);
static bool read_double(int dataset, VarInfo6& info);
static bool read_string(int dataset, VarInfo6& info);
static bool read_boolean(int dataset, VarInfo6& info);
static bool read_integer(int dataset, VarInfo6& info);
static bool read_sparse(int dataset, VarInfo6& info);
static bool read_boolean_sparse(int dataset, VarInfo6& info);
static bool read_poly(int dataset, VarInfo6& info);
static bool read_list(int dataset, VarInfo6& info, std::string type);
static bool read_void(int dataset, VarInfo6& info);
static bool read_undefined(int dataset, VarInfo6& info);
static bool read_struct(int dataset, VarInfo6& info);
static bool read_cell(int dataset, VarInfo6& info);
static bool read_handles(int dataset, VarInfo6& info);
static bool read_macro(int dataset, VarInfo6& info);

static void generateInfo(VarInfo6& info);
static int getDimsNode(int dataset, int* complex, std::vector<int>& dims);

static const std::string fname("hdf5_listvar");

types::Function::ReturnValue sci_hdf5_listvar_v3(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int rhs = static_cast<int>(in.size());
    if (rhs != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount < 1 || _iRetCount > 4)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), fname.data(), 1, 4);
        return types::Function::Error;
    }

    //get filename
    if (in[0]->getId() != types::InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    //open hdf5 file
    wchar_t* wfilename = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wfilename);
    std::string filename = cfilename;
    FREE(wfilename);
    FREE(cfilename);

    int iFile = openHDF5File(filename.data(), 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname.data(), filename.data());
        return types::Function::Error;
    }

    //manage version information
    int iVersion = getSODFormatAttribute(iFile);
    if (iVersion != SOD_FILE_VERSION)
    {
        //can't read file with version newer that me !
        closeHDF5File(iFile);
        Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, iVersion);
        return types::Function::Error;
    }

    int items = getVariableNames6(iFile, nullptr);
    std::vector<VarInfo6> info(items);
    if (items != 0)
    {
        std::vector<char*> vars(items);
        items = getVariableNames6(iFile, vars.data());

        if (_iRetCount == 1)
        {
            sciprint("Name                     Type           Size            Bytes\n");
            sciprint("-------------------------------------------------------------\n");
        }

        for (int i = 0; i < items; i++)
        {
            info[i].name = vars[i];
            FREE(vars[i]);
            info[i].size = 0;

            int dset = getDataSetIdFromName(iFile, info[i].name.data());
            if (dset == 0)
            {
                break;
            }


            if (_iRetCount != 2)
            {
                if (read_data(dset, info[i]) == false)
                {
                    break;
                }

                if (_iRetCount == 1)
                {
                    sciprint("%s\n", info[i].info);
                }
            }
            else // == 2
            {
                if (read_short_data(dset, info[i]) == false)
                {
                    break;
                }
            }
        }
    }
    else
    {
        //no variable returms [] for each Lhs
        for (int i = 0; i < _iRetCount; i++)
        {
            out.push_back(types::Double::Empty());
        }

        return types::Function::OK;
    }

    closeHDF5File(iFile);

    //1st Lhs
    types::String* out1 = new types::String(items, 1);
    for (int i = 0; i < items; i++)
    {
        out1->set(i, info[i].name.data());
    }

    out.push_back(out1);

    //2nd
    if (_iRetCount > 1)
    {
        types::Double* out2 = new types::Double(items, 1);
        double* pout2 = out2->get();
        for (int i = 0; i < items; i++)
        {
            pout2[i] = info[i].type;
        }

        out.push_back(out2);
    }

    if (_iRetCount > 2)
    {
        //3rd Lhs
        types::List* out3 = new types::List();
        for (int i = 0; i < items; i++)
        {
            int dims = info[i].dims;
            types::Double* item = new types::Double(1, dims);
            double* pitem = item->get();
            for (int j = 0; j < dims; j++)
            {
                pitem[j] = static_cast<double>(info[i].pdims[j]);
            }

            out3->append(item);
        }

        out.push_back(out3);
    }

    if (_iRetCount > 3)
    {
        //4th Lhs
        types::Double* out4 = new types::Double(items, 1);
        double* pout4 = out4->get();
        for (int i = 0; i < items; i++)
        {
            pout4[i] = info[i].size;
        }

        out.push_back(out4);
    }

    return types::Function::OK;
}

static bool read_short_data(int dataset, VarInfo6& info)
{
    char* ctype = getScilabTypeFromDataSet6(dataset);
    std::string type(ctype);
    FREE(ctype);
    info.ctype = type;

    if (type == g_SCILAB_CLASS_DOUBLE)
    {
        info.type = sci_matrix;
    }
    else if (type == g_SCILAB_CLASS_STRING)
    {
        info.type = sci_strings;
    }
    else if (type == g_SCILAB_CLASS_LIST)
    {
        info.type = sci_list;
    }
    else if (type == g_SCILAB_CLASS_TLIST)
    {
        info.type = sci_tlist;
    }
    else if (type == g_SCILAB_CLASS_MLIST)
    {
        info.type = sci_mlist;
    }
    else if (type == g_SCILAB_CLASS_BOOLEAN)
    {
        info.type = sci_boolean;
    }
    else if (type == g_SCILAB_CLASS_POLY)
    {
        info.type = sci_poly;
    }
    else if (type == g_SCILAB_CLASS_INT)
    {
        info.type = sci_ints;
    }
    else if (type == g_SCILAB_CLASS_SPARSE)
    {
        info.type = sci_sparse;
    }
    else if (type == g_SCILAB_CLASS_BSPARSE)
    {
        info.type = sci_boolean_sparse;
    }
    else if (type == g_SCILAB_CLASS_VOID)
    {
        info.type = sci_void;
    }
    else if (type == g_SCILAB_CLASS_UNDEFINED)
    {
        info.type = sci_undefined;
    }
    else if (type == g_SCILAB_CLASS_STRUCT)
    {
        info.type = sci_mlist;
    }
    else if (type == g_SCILAB_CLASS_CELL)
    {
        info.type = sci_mlist;
    }
    else if (type == g_SCILAB_CLASS_HANDLE)
    {
        info.type = sci_handles;
    }
    else if (type == g_SCILAB_CLASS_MACRO)
    {
        info.type = sci_c_function;
    }
    else if (type == g_SCILAB_CLASS_USERTYPE)
    {
        info.type = sci_pointer;
    }
    else
    {
        Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), "listvar_in_hdf5");
        return false;
    }

    return true;
}

static bool read_data(int dataset, VarInfo6& info)
{
    bool bRet = false;

    char* ctype = getScilabTypeFromDataSet6(dataset);
    std::string type(ctype);
    FREE(ctype);
    info.ctype = type;

    if (type == g_SCILAB_CLASS_DOUBLE)
    {
        info.type = sci_matrix;
        return read_double(dataset, info);
    }

    if (type == g_SCILAB_CLASS_STRING)
    {
        info.type = sci_strings;
        return read_string(dataset, info);
    }

    if (type == g_SCILAB_CLASS_LIST)
    {
        info.type = sci_list;
        return read_list(dataset, info, "list");
    }

    if (type == g_SCILAB_CLASS_TLIST)
    {
        info.type = sci_tlist;
        return read_list(dataset, info, "tlist");
    }

    if (type == g_SCILAB_CLASS_MLIST)
    {
        info.type = sci_mlist;
        return read_list(dataset, info, "mlist");
    }

    if (type == g_SCILAB_CLASS_BOOLEAN)
    {
        info.type = sci_boolean;
        return read_boolean(dataset, info);
    }

    if (type == g_SCILAB_CLASS_POLY)
    {
        info.type = sci_poly;
        return read_poly(dataset, info);
    }

    if (type == g_SCILAB_CLASS_INT)
    {
        info.type = sci_ints;
        return read_integer(dataset, info);
    }

    if (type == g_SCILAB_CLASS_SPARSE)
    {
        info.type = sci_sparse;
        return read_sparse(dataset, info);
    }

    if (type == g_SCILAB_CLASS_BSPARSE)
    {
        info.type = sci_boolean_sparse;
        return read_boolean_sparse(dataset, info);
    }

    if (type == g_SCILAB_CLASS_VOID)
    {
        info.type = sci_void;
        return read_void(dataset, info);
    }

    if (type == g_SCILAB_CLASS_UNDEFINED)
    {
        info.type = sci_undefined;
        return read_undefined(dataset, info);
    }

    if (type == g_SCILAB_CLASS_STRUCT)
    {
        info.type = sci_mlist;
        return read_struct(dataset, info);
    }

    if (type == g_SCILAB_CLASS_CELL)
    {
        info.type = sci_mlist;
        return read_cell(dataset, info);
    }

    if (type == g_SCILAB_CLASS_HANDLE)
    {
        info.type = sci_handles;
        return read_handles(dataset, info);
    }

    if (type == g_SCILAB_CLASS_MACRO)
    {
        info.type = sci_c_function;
        return read_macro(dataset, info);
    }

    Scierror(999, _("%s: Invalid HDF5 Scilab format.\n"), "listvar_in_hdf5");
    return false;
}

static bool read_double(int dataset, VarInfo6& info)
{
    int complex = 0;
    int ret = getDatasetInfo(dataset, &complex, &info.dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return false;
    }

    info.pdims.resize(info.dims);
    int size = getDatasetInfo(dataset, &complex, &info.dims, info.pdims.data());
    if (size < 0)
    {
        closeDataSet(dataset);
        return false;
    }
    info.size = size * (complex + 1) * sizeof(double);

    generateInfo(info);
    closeDataSet(dataset);
    return true;
}

static bool read_string(int dataset, VarInfo6& info)
{
    int complex = 0;
    int ret = getDatasetInfo(dataset, &complex, &info.dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return false;
    }

    info.pdims.resize(info.dims);
    int size = getDatasetInfo(dataset, &complex, &info.dims, info.pdims.data());
    if (size < 0)
    {
        closeDataSet(dataset);
        return false;
    }

    std::vector<char*> str(size);
    ret = readStringMatrix(dataset, str.data());

    for (int i = 0; i < size; i++)
    {
        info.size += (int)strlen(str[i]) * sizeof(wchar_t);
    }

    freeStringMatrix(dataset, str.data());

    generateInfo(info);
    return true;
}

static bool read_boolean(int dataset, VarInfo6& info)
{
    int complex = 0;
    int ret = getDatasetInfo(dataset, &complex, &info.dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return false;
    }

    info.pdims.resize(info.dims);
    int size = getDatasetInfo(dataset, &complex, &info.dims, info.pdims.data());
    if (size < 0)
    {
        closeDataSet(dataset);
        return false;
    }
    info.size = size * sizeof(int);

    generateInfo(info);
    closeDataSet(dataset);
    return true;
}

static bool read_integer(int dataset, VarInfo6& info)
{
    int complex = 0;
    int ret = getDatasetInfo(dataset, &complex, &info.dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return false;
    }

    info.pdims.resize(info.dims);
    int size = getDatasetInfo(dataset, &complex, &info.dims, info.pdims.data());
    if (size < 0)
    {
        closeDataSet(dataset);
        return false;
    }

    int prec = 0;
    getDatasetPrecision(dataset, &prec);

    info.size = size * (prec % 10);

    generateInfo(info);
    closeDataSet(dataset);
    return true;
}

static bool read_sparse(int dataset, VarInfo6& info)
{
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get non zeros count
    int nnz = 0;
    int datannz = getDataSetIdFromName(dataset, "__nnz__");
    readInteger32Matrix(datannz, &nnz);

    info.dims = 2;
    info.pdims.resize(2);
    info.pdims[0] = pdims[0];
    info.pdims[1] = pdims[1];
    //nnz(double) + rows(int) + nnz(int)
    info.size = sizeof(double) * nnz * (complex + 1) + info.pdims[0] * sizeof(int) + nnz * sizeof(int);

    generateInfo(info);
    closeList6(dataset);
    return true;
}

static bool read_boolean_sparse(int dataset, VarInfo6& info)
{
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get non zeros count
    int nnz = 0;
    int datannz = getDataSetIdFromName(dataset, "__nnz__");
    readInteger32Matrix(datannz, &nnz);

    info.dims = 2;
    info.pdims[0] = pdims[0];
    info.pdims[1] = pdims[1];
    //rows(int) + nnz(int)
    info.size = info.pdims[0] * sizeof(int) + nnz * sizeof(int);

    generateInfo(info);
    closeList6(dataset);
    return true;
}

static bool read_poly(int dataset, VarInfo6& info)
{
    int complex = 0;
    int size = getDimsNode(dataset, &complex, info.pdims);
    info.size = 0;

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");

    for (int i = 0; i < size; ++i)
    {
        std::string polyname(std::to_string(i));
        int poly = getDataSetIdFromName(refs, polyname.data());

        //get dims
        complex = 0;
        int dims = 0;
        int ret = getDatasetInfo(poly, &complex, &dims, NULL);
        if (ret < 0)
        {
            return false;
        }

        std::vector<int> d(dims);
        int datasize = getDatasetInfo(poly, &complex, &dims, d.data());
        if (datasize < 0)
        {
            return false;
        }
        info.size += datasize * sizeof(double) * (complex + 1);
    }

    closeList6(refs);
    closeList6(dataset);

    generateInfo(info);
    return true;
}

static bool read_list(int dataset, VarInfo6& info, std::string type)
{
    int ret = 0;
    int items = 0;

    ret = getListDims6(dataset, &items);
    if (ret)
    {
        return false;
    }

    info.dims = 1;
    info.pdims.resize(1);
    info.pdims[0] = items;
    info.size = 0;

    for (int i = 0; i < items; i++)
    {
        int data = getDataSetIdFromName(dataset, std::to_string(i).data());
        if (data <= 0)
        {
            closeList6(dataset);
            return false;
        }


        VarInfo6 info2;
        bool bRet = read_data(data, info2);
        if (bRet == false)
        {
            return false;
        }

        info.size += info2.size;
    }

    closeList6(dataset);
    generateInfo(info);
    return true;
}

static bool read_void(int dataset, VarInfo6& info)
{
    info.size = 0;
    closeDataSet(dataset);
    return true;
}

static bool read_undefined(int dataset, VarInfo6& info)
{
    info.size = 0;
    closeDataSet(dataset);
    return true;
}

static bool read_struct(int dataset, VarInfo6& info)
{
    int complex = 0;
    int size = getDimsNode(dataset, &complex, info.pdims);
    info.dims = static_cast<int>(info.pdims.size());
    info.size = 0;

    if (size == 0)
    {
        generateInfo(info);
        closeList6(dataset);
        return true;
    }
    int fieldCount = 0;
    int ret = getListDims6(dataset, &fieldCount);
    if (ret < 0)
    {
        closeList6(dataset);
        return false;
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    H5O_info_t oinfo;
    for (int i = 0; i < fieldCount; ++i)
    {
        H5Oget_info_by_idx(dataset, ".", H5_INDEX_NAME, H5_ITER_NATIVE, i, &oinfo, H5P_DEFAULT);
        ssize_t len = H5Lget_name_by_idx(dataset, ".", H5_INDEX_NAME, H5_ITER_INC, i, 0, 0, H5P_DEFAULT) + 1;
        char* name = (char*)MALLOC(sizeof(char) * len);
        H5Lget_name_by_idx(dataset, ".", H5_INDEX_NAME, H5_ITER_INC, i, name, len, H5P_DEFAULT);
        std::string cname(name);
        FREE(name);

        if (cname != "__dims__" && cname != "__refs__")
        {
            int dataref = getDataSetIdFromName(dataset, cname.data());
            if (dataref < 0)
            {
                closeList6(dataset);
                return false;
            }

            int refdim = 0;
            getDatasetInfo(dataref, &complex, &refdim, NULL);
            std::vector<int> refdims(refdim);
            int refcount = getDatasetInfo(dataref, &complex, &refdim, refdims.data());
            if (refcount < 0)
            {
                closeList6(dataset);
                return false;
            }
            std::vector<hobj_ref_t> vrefs(refcount);
            ret = H5Dread(dataref, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, vrefs.data());
            if (ret < 0)
            {
                return false;
            }


            //import field
            for (int j = 0; j < refcount; ++j)
            {
                int data = H5Rdereference(refs, H5R_OBJECT, &vrefs[j]);
                if (data < 0)
                {
                    return false;
                }

                VarInfo6 info2;
                ret = read_data(data, info2);
                if (ret == false)
                {
                    return false;
                }

                info.size += info2.size;

            }

            closeDataSet(dataref);
        }
    }

    generateInfo(info);
    closeList6(refs);
    closeList6(dataset);
    return true;
}

static bool read_cell(int dataset, VarInfo6& info)
{
    //get cell dimensions
    int complex = 0;
    int size = getDimsNode(dataset, &complex, info.pdims);
    info.dims = static_cast<int>(info.pdims.size());

    if (size == 0)
    {
        info.size = 0;
        generateInfo(info);
        closeList6(dataset);
        return true;
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    for (int i = 0; i < size; ++i)
    {
        int ref = getDataSetIdFromName(refs, std::to_string(i).data());
        VarInfo6 info2;
        if (read_data(ref, info2) == false)
        {
            closeList6(refs);
            closeList6(dataset);
            return false;
        }

        info.size += info2.size;
    }

    closeList6(refs);
    closeList6(dataset);

    generateInfo(info);
    return true;
}

static bool read_handles(int dataset, VarInfo6& info)
{
    //get cell dimensions
    int complex = 0;
    int size = getDimsNode(dataset, &complex, info.pdims);
    info.dims = static_cast<int>(info.pdims.size());

    if (size == 0)
    {
        info.size = 0;
        generateInfo(info);
        closeList6(dataset);
        return true;
    }

    closeList6(dataset);

    generateInfo(info);
    return true;
}

static bool read_macro(int dataset, VarInfo6& info)
{
    info.size = 0;
    info.dims = 2;
    info.pdims = {1, 1};
    closeList6(dataset);
    generateInfo(info);
    return true;
}


static void generateInfo(VarInfo6& info)
{
    std::ostringstream ostr;

    if (info.dims != 0)
    {
        ostr << info.pdims[0];
        for (int i = 1; i < info.dims; ++i)
        {
            ostr << " by " << info.pdims[i];
        }
    }

    sprintf(info.info, "%-*s%-*s%-*s%-*d", 25, info.name.data(), 15, info.ctype.data(), 16, ostr.str().data(), 10, info.size);
}

static int getDimsNode(int dataset, int* complex, std::vector<int>& dims)
{
    dims.clear();
    int id = getDataSetIdFromName(dataset, "__dims__");
    if (id < 0)
    {
        return 0;
    }

    //get dims dimension count
    int dim = 0;
    getDatasetInfo(id, complex, &dim, NULL);
    //get dims dimension
    std::vector<int> d(dim);
    int size = getDatasetInfo(id, complex, &dim, d.data());
    if (size < 0)
    {
        return 0;
    }

    //get dims value
    dims.resize(size);
    readInteger32Matrix(id, dims.data());

    size = dims[0];
    for (int i = 1; i < dims.size(); ++i)
    {
        size *= dims[i];
    }
    return size;
}

