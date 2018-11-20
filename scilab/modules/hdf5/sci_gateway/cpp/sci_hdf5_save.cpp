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

int export_data(int parent, const std::string& name, types::InternalType* data, hid_t xfer_plist_id);
static int export_double(int parent, const std::string& name, types::Double* data, hid_t xfer_plist_id);
static int export_string(int parent, const std::string& name, types::String* data, hid_t xfer_plist_id);
static int export_boolean(int parent, const std::string& name, types::Bool* data, hid_t xfer_plist_id);
static int export_list(int parent, const std::string& name, types::List* data, hid_t xfer_plist_id);
static int export_struct(int parent, const std::string& name, types::Struct* data, const char* type, hid_t xfer_plist_id);
template <class T> static int export_int(int parent, const std::string& name, int type, const char* prec, T* data, hid_t xfer_plist_id);
static int export_poly(int parent, const std::string& name, types::Polynom* data, hid_t xfer_plist_id);
static int export_sparse(int parent, const std::string& name, types::Sparse* data, hid_t xfer_plist_id);
static int export_cell(int parent, const std::string& name, types::Cell* data, hid_t xfer_plist_id);
static int export_macro(int parent, const std::string& name, types::Macro* data, hid_t xfer_plist_id);
static int export_usertype(int parent, const std::string& name, types::UserType* data, hid_t xfer_plist_id);

static int export_boolean_sparse(int parent, const std::string& name, types::SparseBool* data, hid_t xfer_plist_id);
static int export_handles(int parent, const std::string& name, types::GraphicHandle* data, hid_t xfer_plist_id);
static int export_void(int parent, const std::string& name, hid_t xfer_plist_id);
static int export_undefined(int parent, const std::string& name, hid_t xfer_plist_id);

/*--------------------------------------------------------------------------*/
// Custom properties for raw data transfer
//
// Passing plist_id as XFER parameter will let H5Dwrite re-use the same
// internal buffers around.
struct CustomXFER
{
    // from HDF5 doc, "the default value for the maximum buffer is 1 Mb."
    const hsize_t size = 1024 * 1024;

    hid_t plist_id;
    uint8_t* tconv;
    uint8_t* bkg;

    CustomXFER() : plist_id(H5Pcreate(H5P_DATASET_XFER)), tconv(new uint8_t[size]), bkg(new uint8_t[size])
    {
        H5Pset_buffer(plist_id, size, tconv, bkg);
    }

    ~CustomXFER()
    {
        H5Pclose(plist_id);
        delete[] tconv;
        delete[] bkg;
    }
};
/*--------------------------------------------------------------------------*/
static const std::string fname("save");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_hdf5_save(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iH5File = 0;
    bool bAppendMode = false;
    bool isNewFile = true;
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
            if (!in[i]->isString())
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), fname.data(), i + 1);
                return types::Function::Error;
            }

            types::String *pS = in[i]->getAs<types::String>(); 
            wchar_t* wvar = pS->get(0);
            if (wcscmp(wvar, L"-append") == 0) {
              bAppendMode = true;
              continue;
            }

            for (int j = 0; j < pS->getSize(); j++)
            {
                wvar = pS->get(j);
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
            isNewFile = false;
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
            Scierror(999, _("%s: Cannot open file %s.\n"), fname.data(), filename.data());
        }

        return types::Function::Error;
    }

    // share common work buffers for all serialized data
    CustomXFER xfer;

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

        int iDataset = export_data(iH5File, var.first, var.second, xfer.plist_id);
        if (iDataset == -1)
        {
            closeHDF5File(iH5File);
            if (isNewFile)
            {
                deleteafile(filename.data());
            }
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
int export_data(int parent, const std::string& name, types::InternalType* data, hid_t xfer_plist_id)
{
    int dataset = -1;
    switch (data->getType())
    {
        case types::InternalType::ScilabDouble:
            dataset = export_double(parent, name, data->getAs<types::Double>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabString:
            dataset = export_string(parent, name, data->getAs<types::String>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabBool:
            dataset = export_boolean(parent, name, data->getAs<types::Bool>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabTList:
        case types::InternalType::ScilabList:
        case types::InternalType::ScilabMList:
            dataset = export_list(parent, name, data->getAs<types::List>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabInt8:
            dataset = export_int(parent, name, H5T_NATIVE_INT8, "8", data->getAs<types::Int8>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabInt16:
            dataset = export_int(parent, name, H5T_NATIVE_INT16, "16", data->getAs<types::Int16>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabInt32:
            dataset = export_int(parent, name, H5T_NATIVE_INT32, "32", data->getAs<types::Int32>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabInt64:
            dataset = export_int(parent, name, H5T_NATIVE_INT64, "64", data->getAs<types::Int64>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabUInt8:
            dataset = export_int(parent, name, H5T_NATIVE_UINT8, "u8", data->getAs<types::UInt8>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabUInt16:
            dataset = export_int(parent, name, H5T_NATIVE_UINT16, "u16", data->getAs<types::UInt16>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabUInt32:
            dataset = export_int(parent, name, H5T_NATIVE_UINT32, "u32", data->getAs<types::UInt32>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabUInt64:
            dataset = export_int(parent, name, H5T_NATIVE_UINT64, "u64", data->getAs<types::UInt64>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabStruct:
            dataset = export_struct(parent, name, data->getAs<types::Struct>(), g_SCILAB_CLASS_STRUCT, xfer_plist_id);
            break;
        case types::InternalType::ScilabPolynom:
            dataset = export_poly(parent, name, data->getAs<types::Polynom>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabSparse:
            dataset = export_sparse(parent, name, data->getAs<types::Sparse>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabSparseBool :
            dataset = export_boolean_sparse(parent, name, data->getAs<types::SparseBool>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabCell:
            dataset = export_cell(parent, name, data->getAs<types::Cell>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabVoid:
            dataset = export_void(parent, name, xfer_plist_id);
            break;
        case types::InternalType::ScilabListUndefinedOperation:
            dataset = export_undefined(parent, name, xfer_plist_id);
            break;
        case types::InternalType::ScilabMacro:
            dataset = export_macro(parent, name, data->getAs<types::Macro>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabMacroFile:
        {
            types::MacroFile* pMF = data->getAs<types::MacroFile>();
            dataset = export_macro(parent, name, pMF->getMacro(), xfer_plist_id);
            break;
        }
        case types::InternalType::ScilabHandle:
            dataset = export_handles(parent, name, data->getAs<types::GraphicHandle>(), xfer_plist_id);
            break;
        case types::InternalType::ScilabUserType:
            dataset = export_usertype(parent, name, data->getAs<types::UserType>(), xfer_plist_id);
            break;
        default:
        {
            break;
        }
    }

    return dataset;
}

/*--------------------------------------------------------------------------*/
static int export_list(int parent, const std::string& name, types::List* data, hid_t xfer_plist_id)
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
        if (export_data(dset, std::to_string(i).data(), data->get(i), xfer_plist_id) == -1)
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
static int export_double(int parent, const std::string& name, types::Double* data, hid_t xfer_plist_id)
{
    int dataset = -1;

    if (data->isComplex())
    {
        dataset = writeDoubleComplexMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get(), data->getImg(), xfer_plist_id);
    }
    else
    {
        dataset = writeDoubleMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get(), xfer_plist_id);
    }

    return dataset;
}
/*--------------------------------------------------------------------------*/
template <class T>
static int export_int(int parent, const std::string& name, int type, const char* prec, T* data, hid_t xfer_plist_id)
{
    return writeIntegerMatrix6(parent, name.data(), type, prec, data->getDims(), data->getDimsArray(), data->get(), xfer_plist_id);
}
/*--------------------------------------------------------------------------*/
static int export_string(int parent, const std::string& name, types::String* data, hid_t xfer_plist_id)
{
    int size = data->getSize();
    wchar_t** s = data->get();
    std::vector<char*> v(size);

    //convert UTF16 strings to UTF8
    for (int i = 0; i < size; ++i)
    {
        v[i] = wide_string_to_UTF8(s[i]);
    }

    int dset = writeStringMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), v.data(), xfer_plist_id);

    //release memory
    for (int i = 0; i < size; ++i)
    {
        FREE(v[i]);
    }

    return dset;
}
/*--------------------------------------------------------------------------*/
static int export_boolean(int parent, const std::string& name, types::Bool* data, hid_t xfer_plist_id)
{
    return writeBooleanMatrix6(parent, name.data(), data->getDims(), data->getDimsArray(), data->get(), xfer_plist_id);
}
/*--------------------------------------------------------------------------*/
static int export_struct(int parent, const std::string& name, types::Struct* data, const char* type, hid_t xfer_plist_id)
{
    //create a group with struct name
    int dset = openList6(parent, name.data(), type);
    //store struct dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray(), xfer_plist_id);
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
    export_string(dset, "__fields__", fields, xfer_plist_id);

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
            int ref = export_data(refs, refname, val, xfer_plist_id);
            //create reference of data
            ret = addItemStruct6(refs, vrefs.data(), j, refname.data());
            if (ret)
            {
                delete fields;
                FREE(cfield);
                return -1;
            }
        }

        ret = writeStructField6(dset, cfield, data->getDims(), data->getDimsArray(), vrefs.data(), xfer_plist_id);
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
static int export_void(int parent, const std::string& name, hid_t xfer_plist_id)
{
    return writeVoid6(parent, name.data(), xfer_plist_id);
}
/*--------------------------------------------------------------------------*/
static int export_undefined(int parent, const std::string& name, hid_t xfer_plist_id)
{
    return writeUndefined6(parent, name.data(), xfer_plist_id);
}
/*--------------------------------------------------------------------------*/
static int export_poly(int parent, const std::string& name, types::Polynom* data, hid_t xfer_plist_id)
{
    //create a group with struct name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_POLY);
    //store struct dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray(), xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    //store variable name
    std::vector<int> vardims = {1, 1};
    char* varname = wide_string_to_UTF8(data->getVariableName().data());
    ret = writeStringMatrix6(dset, "__varname__", 2, vardims.data(), &varname, xfer_plist_id);
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
            writeDoubleComplexMatrix6(refs, polyname.data(), 2, ssdims.data(), val->get(), val->getImg(), xfer_plist_id);
        }
        else
        {
            writeDoubleMatrix6(refs, polyname.data(), 2, ssdims.data(), val->get(), xfer_plist_id);
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
static int export_sparse(int parent, const std::string& name, types::Sparse* data, hid_t xfer_plist_id)
{
    int nnz = static_cast<int>(data->nonZeros());
    int row = data->getRows();
    //create a group with sparse name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_SPARSE);
    //store sparse dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray(), xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    //store numbers of non zero by rows.
    std::vector<int> dimsnnz = {1, 1};
    ret = writeIntegerMatrix6(dset, "__nnz__", H5T_NATIVE_INT32, "32", 2, dimsnnz.data(), &nnz, xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    //store inner vector
    int innercount = 0;
    int* inner = data->getInnerPtr(&innercount);
    std::vector<int> dimsinner = {1, nnz};
    ret = writeIntegerMatrix6(dset, "__inner__", H5T_NATIVE_INT32, "32", 2, dimsinner.data(), inner, xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    int outercount = 0;
    int* outer = data->getOuterPtr(&outercount);
    std::vector<int> dimsouter = {1, outercount + 1};
    ret = writeIntegerMatrix6(dset, "__outer__", H5T_NATIVE_INT32, "32", 2, dimsouter.data(), outer, xfer_plist_id);
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
        ret = writeDoubleComplexMatrix6(dset, "__data__", 2, dimsdata.data(), real, img, xfer_plist_id);
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
        ret = writeDoubleMatrix6(dset, "__data__", 2, dimsdata.data(), data->get(), xfer_plist_id);
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
static int export_boolean_sparse(int parent, const std::string& name, types::SparseBool* data, hid_t xfer_plist_id)
{
    int nnz = static_cast<int>(data->nbTrue());
    int row = data->getRows();
    //create a group with sparse name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_BSPARSE);
    //store sparse dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray(), xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    //store numbers of non zero by rows.
    std::vector<int> dimsnnz = {1, 1};
    ret = writeIntegerMatrix6(dset, "__nnz__", H5T_NATIVE_INT32, "32", 2, dimsnnz.data(), &nnz, xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    //store inner vector
    int innercount = 0;
    int* inner = data->getInnerPtr(&innercount);
    std::vector<int> dimsinner = {1, nnz};
    ret = writeIntegerMatrix6(dset, "__inner__", H5T_NATIVE_INT32, "32", 2, dimsinner.data(), inner, xfer_plist_id);
    if (ret < 0)
    {
        return -1;
    }

    int outercount = 0;
    int* outer = data->getOuterPtr(&outercount);
    std::vector<int> dimsouter = {1, outercount + 1};
    ret = writeIntegerMatrix6(dset, "__outer__", H5T_NATIVE_INT32, "32", 2, dimsouter.data(), outer, xfer_plist_id);
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
static int export_cell(int parent, const std::string& name, types::Cell* data, hid_t xfer_plist_id)
{
    //create a group with cell name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_CELL);
    //store cell dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray(), xfer_plist_id);
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
        int ref = export_data(refs, refname, it[i], xfer_plist_id);
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

static int export_handles(int parent, const std::string& name, types::GraphicHandle* data, hid_t xfer_plist_id)
{
    //create a group with cell name
    int dset = openList6(parent, name.data(), g_SCILAB_CLASS_HANDLE);
    //store cell dimensions
    std::vector<int> dims = {1, data->getDims()};
    int ret = writeIntegerMatrix6(dset, "__dims__", H5T_NATIVE_INT32, "32", 2, dims.data(), data->getDimsArray(), xfer_plist_id);
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
        if (export_handle(refs, refname, hl, xfer_plist_id) == false)
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

static int export_macro(int parent, const std::string& name, types::Macro* data, hid_t xfer_plist_id)
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
    writeStringMatrix6(dset, "inputs", 2, dims, inputNames.data(), xfer_plist_id);

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
    writeStringMatrix6(dset, "outputs", 2, dims, outputNames.data(), xfer_plist_id);

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
    writeIntegerMatrix6(dset, "body", H5T_NATIVE_UINT8, "u8", 2, dims, serialAst, xfer_plist_id);

    closeList6(dset);
    return dset;
}

static int export_usertype(int parent, const std::string& name, types::UserType* data, hid_t xfer_plist_id)
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

    int ret = export_struct(parent, name, str, g_SCILAB_CLASS_USERTYPE, xfer_plist_id);

    //protect data against delete
    it->IncreaseRef();
    delete str;
    it->DecreaseRef();

    return ret;
}
