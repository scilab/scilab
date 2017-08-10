/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include <list>
#include <map>
#include <hdf5.h>
#include "hdf5_gw.hxx"
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"
#include "struct.hxx"
#include "cell.hxx"
#include "int.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "macrofile.hxx"
#include "graphichandle.hxx"
#include "user.hxx"
#include "overload.hxx"
#include "handle_properties.hxx"
#include "context.hxx"
#include "serializervisitor.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "os_string.h"
#include "deleteafile.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "h5_readDataFromFile.h"
#include "h5_attributeConstants.h"
#include "HandleManagement.h"
}

/*--------------------------------------------------------------------------*/
static bool isVarExist(int _iFile, const char* _pstVarName);
static int extractVarNameList(int* pvCtx, int _iStart, int _iEnd, char** _pstNameList);

int export_data(int parent, const std::string& name, types::InternalType* data);
static int export_double(int parent, const std::string& name, types::Double* data);
static int export_string(int parent, const std::string& name, types::String* data);
static int export_boolean(int parent, const std::string& name, types::Bool* data);
static int export_list(int parent, const std::string& name, types::List* data);
static int export_struct(int parent, const std::string& name, types::Struct* data, const char* type = g_SCILAB_CLASS_STRUCT);
template <class T> static int export_int(int parent, const std::string& name, int type, const char* prec, T* data);
static int export_poly(int parent, const std::string& name, types::Polynom* data);
static int export_sparse(int parent, const std::string& name, types::Sparse* data);
static int export_cell(int parent, const std::string& name, types::Cell* data);
static int export_macro(int parent, const std::string& name, types::Macro* data);
static int export_usertype(int parent, const std::string& name, types::UserType* data);

static int export_boolean_sparse(int parent, const std::string& name, types::SparseBool* data);
static int export_handles(int parent, const std::string& name, types::GraphicHandle* data);
static int export_void(int parent, const std::string& name);
static int export_undefined(int parent, const std::string& name);

/*--------------------------------------------------------------------------*/
static const std::string fname("save");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_hdf5_save(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iH5File = 0;
    bool bAppendMode = false;
    int rhs = static_cast<int>(in.size());
    std::string filename;
    std::map<std::string, types::InternalType*> vars;
    symbol::Context* ctx = symbol::Context::getInstance();

    /* Check the number of input argument */
    if (in.size() < 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 2);
        return types::Function::Error;
    }

    if (in[0]->getId() != types::InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    wchar_t* wfilename = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wfilename);
    filename = cfilename;
    FREE(wfilename);
    FREE(cfilename);

    if (rhs == 1)
    {
        //save environment
        //get variables in scope 1
        std::list<std::wstring> lst;
        int size = ctx->getConsoleVarsName(lst);

        if (size == 0)
        {
            return types::Function::OK;
        }

        for (const auto & wvar : lst)
        {
            types::InternalType* pIT = ctx->getAtLevel(symbol::Symbol(wvar), SCOPE_CONSOLE);

            //do not save macrofile
            if (pIT->isMacroFile() || pIT->isFunction() || pIT->isLibrary())
            {
                continue;
            }

            char* cvar = wide_string_to_UTF8(wvar.data());
            std::string var(cvar);
            FREE(cvar);

            //check var exists
            vars[var] = pIT;
        }
    }
    else
    {
        for (int i = 1; i < rhs; ++i)
        {
            if (in[i]->getId() != types::InternalType::IdScalarString)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), fname.data(), i+1);
                return types::Function::Error;
            }

            wchar_t* wvar = in[i]->getAs<types::String>()->get()[0];
            if (wcscmp(wvar, L"-append") == 0)
            {
                bAppendMode = true;
                continue;
            }

            types::InternalType* pIT = ctx->get(symbol::Symbol(wvar));
            if (pIT == NULL)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Defined variable expected.\n"), fname.data(), i + 1);
                return types::Function::Error;
            }

            char* cvar = wide_string_to_UTF8(wvar);
            std::string var(cvar);
            FREE(cvar);

            //check var exists
            vars[var] = pIT;
        }
    }
    //check append option
    if (bAppendMode)
    {
        // open hdf5 file
        iH5File = openHDF5File(filename.data(), bAppendMode);
        if (iH5File < 0)
        {
            iH5File = createHDF5File(filename.data());
        }
        else
        {
            int iVersion = getSODFormatAttribute(iH5File);
            if (iVersion != SOD_FILE_VERSION)
            {
                //to update version must be the same
                closeHDF5File(iH5File);
                Scierror(999, _("%s: Wrong SOD file format version. Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, iVersion);
                return types::Function::Error;
            }
        }
    }
    else
    {
        iH5File = createHDF5File(filename.data());
    }


    if (iH5File < 0)
    {
        if (iH5File == -2)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: \"%s\" is a directory"), fname.data(), 1, filename.data());
        }
        else
        {
            Scierror(999, _("%s: Cannot open file %s.\n"), fname.data() , filename.data());
        }

        return types::Function::Error;
    }

    // export data
    for (const auto var : vars)
    {
        if (isVarExist(iH5File, var.first.data()))
        {
            if (bAppendMode)
            {
                if (deleteHDF5Var(iH5File, var.first.data()))
                {
                    closeHDF5File(iH5File);
                    Scierror(999, _("%s: Unable to delete existing variable \"%s\".\n"), fname.data(), var.first.data());
                    return types::Function::Error;
                }
            }
            else
            {
                closeHDF5File(iH5File);
                Scierror(999, _("%s: Variable \'%s\' already exists in file \'%s\'\nUse -append option to replace existing variable.\n"), fname.data(), var.first.data(), filename.data());
                return types::Function::Error;
            }
        }

        int iDataset = export_data(iH5File, var.first, var.second);
        if (iDataset == -1)
        {
            closeHDF5File(iH5File);
            deleteafile(filename.data());
            Scierror(999, _("%s: Unable to export variable \'%s\' in file \'%s\'.\n"), fname.data(), var.first.data(), filename.data());
            return types::Function::Error;
        }
    }

    //add or update scilab version and file version in hdf5 file
    if (updateScilabVersion(iH5File) < 0)
    {
        closeHDF5File(iH5File);
        Scierror(999, _("%s: Unable to update Scilab version in \"%s\"."), fname.data(), filename.data());
        return types::Function::Error;
    }

    if (updateFileVersion(iH5File) < 0)
    {
        closeHDF5File(iH5File);
        Scierror(999, _("%s: Unable to update HDF5 format version in \"%s\"."), fname.data(), filename.data());
        return types::Function::Error;
    }

    //close hdf5 file
    closeHDF5File(iH5File);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
static bool isVarExist(int _iFile, const char* _pstVarName)
{
    //check if variable already exists
    int iNbItem = getVariableNames6(_iFile, NULL);
    if (iNbItem)
    {
        char **pstVarNameList = (char **)MALLOC(sizeof(char *) * iNbItem);

        iNbItem = getVariableNames6(_iFile, pstVarNameList);

        //import all data
        for (int i = 0; i < iNbItem; i++)
        {
            if (strcmp(pstVarNameList[i], _pstVarName) == 0)
            {
                freeArrayOfString(pstVarNameList, iNbItem);
                return true;
            }
        }

        freeArrayOfString(pstVarNameList, iNbItem);
    }

    return false;
}
/*--------------------------------------------------------------------------*/
int export_data(int parent, const std::string& name, types::InternalType* data)
{
    int dataset = -1;
    switch (data->getType())
    {
        case types::InternalType::ScilabDouble:
            dataset = export_double(parent, name, data->getAs<types::Double>());
            break;
        case types::InternalType::ScilabString:
            dataset = export_string(parent, name, data->getAs<types::String>());
            break;
        case types::InternalType::ScilabBool:
            dataset = export_boolean(parent, name, data->getAs<types::Bool>());
            break;
        case types::InternalType::ScilabTList:
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
            dataset = export_list(parent, name, data->getAs<types::List>());
            break;
        case types::InternalType::ScilabInt8:
            dataset = export_int(parent, name, H5T_NATIVE_INT8, "8", data->getAs<types::Int8>());
            break;
        case types::InternalType::ScilabInt16:
            dataset = export_int(parent, name, H5T_NATIVE_INT16, "16", data->getAs<types::Int16>());
            break;
        case types::InternalType::ScilabInt32:
            dataset = export_int(parent, name, H5T_NATIVE_INT32, "32", data->getAs<types::Int32>());
            break;
        case types::InternalType::ScilabInt64:
            dataset = export_int(parent, name, H5T_NATIVE_INT64, "64", data->getAs<types::Int64>());
            break;
        case types::InternalType::ScilabUInt8:
            dataset = export_int(parent, name, H5T_NATIVE_UINT8, "u8", data->getAs<types::UInt8>());
            break;
        case types::InternalType::ScilabUInt16:
            dataset = export_int(parent, name, H5T_NATIVE_UINT16, "u16", data->getAs<types::UInt16>());
            break;
        case types::InternalType::ScilabUInt32:
            dataset = export_int(parent, name, H5T_NATIVE_UINT32, "u32", data->getAs<types::UInt32>());
            break;
        case types::InternalType::ScilabUInt64:
            dataset = export_int(parent, name, H5T_NATIVE_UINT64, "u64", data->getAs<types::UInt64>());
            break;
        case types::InternalType::ScilabStruct:
            dataset = export_struct(parent, name, data->getAs<types::Struct>());
            break;
        case types::InternalType::ScilabPolynom:
            dataset = export_poly(parent, name, data->getAs<types::Polynom>());
            break;
        case types::InternalType::ScilabSparse:
            dataset = export_sparse(parent, name, data->getAs<types::Sparse>());
            break;
        case types::InternalType::ScilabSparseBool :
            dataset = export_boolean_sparse(parent, name, data->getAs<types::SparseBool>());
            break;
        case types::InternalType::ScilabCell:
            dataset = export_cell(parent, name, data->getAs<types::Cell>());
            break;
        case types::InternalType::ScilabVoid:
            dataset = export_void(parent, name);
            break;
        case types::InternalType::ScilabListUndefinedOperation:
            dataset = export_undefined(parent, name);
            break;
        case types::InternalType::ScilabMacro:
            dataset = export_macro(parent, name, data->getAs<types::Macro>());
            break;
        case types::InternalType::ScilabMacroFile:
        {
            types::MacroFile* pMF = data->getAs<types::MacroFile>();
            dataset = export_macro(parent, name, pMF->getMacro());
            break;
        }
        case types::InternalType::ScilabHandle:
            dataset = export_handles(parent, name, data->getAs<types::GraphicHandle>());
            break;
        case types::InternalType::ScilabUserType:
            dataset = export_usertype(parent, name, data->getAs<types::UserType>());
            break;
        default:
        {
            break;
        }
    }

    return dataset;
}

/*--------------------------------------------------------------------------*/
static int export_list(int parent, const std::string& name, types::List* data)
{
    int size = data->getSize();

    const char* type = nullptr;
    switch (data->getType())
    {
        case types::InternalType::ScilabMList:
            type = g_SCILAB_CLASS_MLIST;
            break;
        case types::InternalType::ScilabTList:
            type = g_SCILAB_CLASS_TLIST;
            break;
        case types::InternalType::ScilabList:
            type = g_SCILAB_CLASS_LIST;
            break;
        default:
            return -1;
    }

    //create node with list name
    int dset = openList6(parent, name.data(), type);

    for (int i = 0; i < size; ++i)
    {
        if (export_data(dset, std::to_string(i).data(), data->get(i)) == -1)
        {
            closeList6(dset);
            return -1;
        }
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }
    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_double(int parent, const std::string& name, types::Double* data)
{
    int dataset = -1;

    if (data->isComplex())
    {
        dataset = writeDoubleComplexMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get(), data->getImg());
    }
    else
    {
        dataset = writeDoubleMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get());
    }

    return dataset;
}
/*--------------------------------------------------------------------------*/
template <class T>
static int export_int(int parent, const std::string& name, int type, const char* prec, T* data)
{
    return writeIntegerMatrix6(parent, name.data(), type, prec, data->getDims(), data->getDimsArray(), data->get());
}
/*--------------------------------------------------------------------------*/
static int export_string(int parent, const std::string& name, types::String* data)
{
    int size = data->getSize();
    wchar_t** s = data->get();
    std::vector<char*> v(size);

    //convert UTF16 strings to UTF8
    for (int i = 0; i < size; ++i)
    {
        v[i] = wide_string_to_UTF8(s[i]);
    }

    int dset = writeStringMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), v.data());

    //release memory
    for (int i = 0; i < size; ++i)
    {
        FREE(v[i]);
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_boolean(int parent, const std::string& name, types::Bool* data)
{
    return writeBooleanMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get());
}
/*--------------------------------------------------------------------------*/
static int export_struct(int parent, const std::string& name, types::Struct* data, const char* type)
{
    //create a group with struct name
    int dset = openList6(parent, name.data(), type);
    //store struct dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    int size = data->getSize();

    if (size == 0)
    {
        if (closeList6(dset) == -1)
        {
            return -1;
        }

        return dset;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_STRUCT);
    if (refs < 0)
    {
        return -1;
    }

    types::String* fields = data->getFieldNames();
    int fieldCount = fields->getSize();
    wchar_t** pfields = fields->get();

    //save fields list in vector to keep order
    export_string(dset, "__fields__", fields);

    std::vector<hobj_ref_t> vrefs(size);
    //fill main group with struct field name
    for (int i = 0; i < fieldCount; ++i)
    {
        char* cfield = wide_string_to_UTF8(pfields[i]);
        for (int j = 0; j < size; ++j)
        {
            //get data
            types::InternalType* val = data->get(j)->get(pfields[i]);
            //create ref name
            std::string refname(cfield);
            refname += "_" + std::to_string(j);
            //export data in refs group
            int ref = export_data(refs, refname, val);
            //create reference of data
            ret = addItemStruct6(refs, vrefs.data(), j, refname.data());
            if (ret)
            {
                delete fields;
                FREE(cfield);
                return -1;
            }
        }

        ret = writeStructField6(dset, cfield, data->getDims(), data->getDimsArray(), vrefs.data());
        FREE(cfield);
        if (ret < 0)
        {
            delete fields;
            return -1;
        }
    }

    delete fields;

    if (closeList6(refs) == -1)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_void(int parent, const std::string& name)
{
    return writeVoid6(parent, name.data());
}
/*--------------------------------------------------------------------------*/
static int export_undefined(int parent, const std::string& name)
{
    return writeUndefined6(parent, name.data());
}
/*--------------------------------------------------------------------------*/
static int export_poly(int parent, const std::string& name, types::Polynom* data)
{
    //create a group with struct name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_POLY);
    //store struct dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //store variable name
    std::vector<int> vardims = {1, 1};
    char* varname = wide_string_to_UTF8(data->getVariableName().data());
    ret = writeStringMatrix6(dset, "__varname__", 2, vardims.data(), &varname);
    FREE(varname);
    if (ret < 0)
    {
        return -1;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_POLY);
    if (refs < 0)
    {
        return -1;
    }

    bool complex = data->isComplex();
    int size = data->getSize();
    std::vector<hobj_ref_t> vrefs(size);
    types::SinglePoly** ss = data->get();
    //fill main group with struct field name
    for (int j = 0; j < size; ++j)
    {
        //get data
        types::SinglePoly* val = ss[j];
        //export data in refs group
        std::vector<int> ssdims = {1, val->getSize()};
        std::string polyname(std::to_string(j));
        if (complex)
        {
            writeDoubleComplexMatrix6(refs, polyname.data(), 2, ssdims.data(), val->get(), val->getImg());
        }
        else
        {
            writeDoubleMatrix6(refs, polyname.data(), 2, ssdims.data(), val->get());
        }

        //create reference of data
        ret = addItemStruct6(refs, vrefs.data(), j, polyname.data());
        if (ret)
        {
            return -1;
        }
    }


    if (closeList6(refs) == -1)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_sparse(int parent, const std::string& name, types::Sparse* data)
{
    int nnz = static_cast<int>(data->nonZeros());
    int row = data->getRows();
    //create a group with sparse name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_SPARSE);
    //store sparse dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //store numbers of non zero by rows.
    std::vector<int> dimsnnz = {1, 1};
    ret = writeIntegerMatrix6(dset, "__nnz__", H5T_NATIVE_INT32, "32", 2, dimsnnz.data(), &nnz);
    if (ret < 0)
    {
        return -1;
    }

    //store inner vector
    int innercount = 0;
    int* inner = data->getInnerPtr(&innercount);
    std::vector<int> dimsinner = {1, nnz};
    ret = writeIntegerMatrix6(dset, "__inner__", H5T_NATIVE_INT32, "32", 2, dimsinner.data(), inner);
    if (ret < 0)
    {
        return -1;
    }

    int outercount = 0;
    int* outer = data->getOuterPtr(&outercount);
    std::vector<int> dimsouter = {1, outercount + 1};
    ret = writeIntegerMatrix6(dset, "__outer__", H5T_NATIVE_INT32, "32", 2, dimsouter.data(), outer);
    if (ret < 0)
    {
        return -1;
    }

    if (data->isComplex())
    {
        double* real = new double[nnz];
        double* img = new double[nnz];
        std::complex<double>* d = data->getImg();
        for (int i = 0; i < nnz; ++i)
        {
            real[i] = d[i].real();
            img[i] = d[i].imag();
        }


        std::vector<int> dimsdata = {1, nnz};
        ret = writeDoubleComplexMatrix6(dset, "__data__", 2, dimsdata.data(), real, img);
        delete[] real;
        delete[] img;
        if (ret < 0)
        {
            return -1;
        }
    }
    else
    {
        std::vector<int> dimsdata = {1, nnz};
        ret = writeDoubleMatrix6(dset, "__data__", 2, dimsdata.data(), data->get());
        if (ret < 0)
        {
            return -1;
        }
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;

}
/*--------------------------------------------------------------------------*/
static int export_boolean_sparse(int parent, const std::string& name, types::SparseBool* data)
{
    int nnz = static_cast<int>(data->nbTrue());
    int row = data->getRows();
    //create a group with sparse name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_BSPARSE);
    //store sparse dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //store numbers of non zero by rows.
    std::vector<int> dimsnnz = {1, 1};
    ret = writeIntegerMatrix6(dset, "__nnz__", H5T_NATIVE_INT32, "32", 2, dimsnnz.data(), &nnz);
    if (ret < 0)
    {
        return -1;
    }

    //store inner vector
    int innercount = 0;
    int* inner = data->getInnerPtr(&innercount);
    std::vector<int> dimsinner = {1, nnz};
    ret = writeIntegerMatrix6(dset, "__inner__", H5T_NATIVE_INT32, "32", 2, dimsinner.data(), inner);
    if (ret < 0)
    {
        return -1;
    }

    int outercount = 0;
    int* outer = data->getOuterPtr(&outercount);
    std::vector<int> dimsouter = {1, outercount + 1};
    ret = writeIntegerMatrix6(dset, "__outer__", H5T_NATIVE_INT32, "32", 2, dimsouter.data(), outer);
    if (ret < 0)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_cell(int parent, const std::string& name, types::Cell* data)
{
    //create a group with cell name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_CELL);
    //store cell dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_CELL);
    if (refs < 0)
    {
        return -1;
    }

    int size = data->getSize();
    types::InternalType** it = data->get();
    std::vector<hobj_ref_t> vrefs(size);
    for (int i = 0; i < size; ++i)
    {
        std::string refname(std::to_string(i));
        int ref = export_data(refs, refname, it[i]);
    }


    if (closeList6(refs) == -1)
    {
        return -1;
    }

    if (closeList6(dset) == -1)
    {
        return -1;
    }

    return dset;
}

static int export_handles(int parent, const std::string& name, types::GraphicHandle* data)
{
    //create a group with cell name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_HANDLE);
    //store cell dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray());
    if (ret < 0)
    {
        return -1;
    }

    //create a node for fields references
    int refs = openList6(dset, "__refs__", g_SCILAB_CLASS_HANDLE);
    if (refs < 0)
    {
        closeList6(dset);
        return -1;
    }

    int size = data->getSize();
    long long* ll = data->get();
    std::vector<hobj_ref_t> vrefs(size);
    for (int i = 0; i < size; ++i)
    {
        //get handle uid
        int hl = getObjectFromHandle(static_cast<long>(ll[i]));
        std::string refname(std::to_string(i));
        if (export_handle(refs, refname, hl) == false)
        {
            closeList6(refs);
            closeList6(dset);
            return -1;
        }
    }

    closeList6(refs);
    closeList6(dset);
    return dset;
}

static int export_macro(int parent, const std::string& name, types::Macro* data)
{
    int dims[2];

    //create a group with macro name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_MACRO);

    //inputs
    std::vector<char*> inputNames;
    auto inputs = data->getInputs();
    for (auto & input : *inputs)
    {
        inputNames.push_back(wide_string_to_UTF8(input->getSymbol().getName().data()));
    }

    dims[0] = 1;
    dims[1] = static_cast<int>(inputNames.size());
    writeStringMatrix6(dset, "inputs", 2, dims, inputNames.data());

    for (auto & in : inputNames)
    {
        FREE(in);
    }

    //outputs
    std::vector<char*> outputNames;
    auto outputs = data->getOutputs();
    for (auto & output : *outputs)
    {
        outputNames.push_back(wide_string_to_UTF8(output->getSymbol().getName().data()));
    }

    dims[0] = 1;
    dims[1] = static_cast<int>(outputNames.size());
    writeStringMatrix6(dset, "outputs", 2, dims, outputNames.data());

    for (auto & in : outputNames)
    {
        FREE(in);
    }

    //body
    ast::Exp* pExp = data->getBody();
    ast::SerializeVisitor serialMacro(pExp);

    unsigned char* serialAst = serialMacro.serialize();
    //size if the buffer ( unsigned int )
    unsigned int size = *((unsigned int*)serialAst);

    dims[0] = 1;
    dims[1] = size;
    writeIntegerMatrix6(dset, "body", H5T_NATIVE_UINT8, "u8", 2, dims, serialAst);
    free(serialAst);

    closeList6(dset);
    return dset;
}

static int export_usertype(int parent, const std::string& name, types::UserType* data)
{
    types::InternalType* it = data->save();
    if (it == nullptr)
    {
        types::typed_list in;
        in.push_back(data);

        types::typed_list out;
        //overload
        // rational case
        std::wstring wstFuncName = L"%" + data->getShortTypeStr() + L"_save";

        try
        {
            types::Callable::ReturnValue ret = Overload::call(wstFuncName, in, 1, out);

            if (ret != types::Callable::OK)
            {
                return -1;
            }

            if (out.size() != 1)
            {
                for (auto & i : out)
                {
                    i->killMe();
                }
                return -1;
            }

            it = out[0];
        }
        catch (const ast::InternalError& /*ie*/)
        {
            //overload does not exist
            return -1;
        }

    }

    if (it->isUserType())
    {
        it->killMe();
        return -1;
    }

    //create a struct around "usertype" to be able to restore it.
    types::Struct* str = new types::Struct(1, 1);
    types::SingleStruct* ss = str->get()[0];

    //add fields
    ss->addField(L"type");
    ss->addField(L"data");

    //assign values to new fields
    ss->set(L"type", new types::String(data->getShortTypeStr().data()));
    ss->set(L"data", it);

    int ret = export_struct(parent, name, str, g_SCILAB_CLASS_USERTYPE);

    //protect data against delete
    it->IncreaseRef();
    delete str;
    it->DecreaseRef();

    return ret;
}
