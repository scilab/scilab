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
#include <unordered_map>

#include <hdf5.h>
#include "hdf5_gw.hxx"
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"
#include "struct.hxx"
#include "cell.hxx"
#include "int.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "polynom.hxx"
#include "sparse.hxx"
#include "graphichandle.hxx"
#include "macro.hxx"
#include "void.hxx"
#include "listundefined.hxx"
#include "context.hxx"
#include "handle_properties.hxx"
#include "deserializervisitor.hxx"
#include "overload.hxx"

std::unordered_map<int, Links::PathList> Links::paths;

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
static bool import_variable(int file, std::string& name);
types::InternalType* import_data(int dataset);
static types::InternalType* import_double(int dataset);
static types::InternalType* import_string(int dataset);
static types::InternalType* import_boolean(int dataset);
static types::InternalType* import_int(int dataset);
static types::InternalType* import_list(int dataset, types::List* lst);
static types::InternalType* import_struct(int dataset);
static types::InternalType* import_poly(int dataset);
static types::InternalType* import_cell(int dataset);
static types::InternalType* import_handles(int dataset);
static types::InternalType* import_sparse(int dataset);
static types::InternalType* import_boolean_sparse(int dataset);
static types::InternalType* import_macro(int dataset);
static types::InternalType* import_usertype(int dataset);


/*--------------------------------------------------------------------------*/
static const std::string fname("load");
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_hdf5_load_v3(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::string filename;
    int rhs = static_cast<int>(in.size());

    if (rhs < 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->getId() != types::InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    wchar_t* wfilename = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wfilename);
    filename = cfilename;
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
        Scierror(999, _("%s: Wrong SOD file format version. Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, iVersion);
        return types::Function::Error;
    }

    if (rhs > 1)
    {
        for (int i = 1; i < rhs; ++i)
        {
            std::string var;
            char* cvar = wide_string_to_UTF8(in[i]->getAs<types::String>()->get()[0]);
            var = cvar;
            FREE(cvar);

            if (import_variable(iFile, var) == false)
            {
                Scierror(999, _("%s: Unable to load \'%s\'.\n"), fname.data(), var.data());
                return types::Function::Error;
            }
        }
    }
    else
    {
        //restore all variables
        int iNbItem = 0;
        iNbItem = getVariableNames6(iFile, NULL);
        if (iNbItem != 0)
        {
            std::vector<char*> vars(iNbItem);
            iNbItem = getVariableNames6(iFile, vars.data());
            for (auto & var : vars)
            {
                std::string s(var);
                FREE(var);
                if (import_variable(iFile, s) == false)
                {
                    Scierror(999, _("%s: Unable to load \'%s\'.\n"), fname.data(), s.data());
                    return types::Function::Error;
                }
            }
        }
    }

    //close the file
    closeHDF5File(iFile);

    out.push_back(new types::Bool(1));
    return types::Function::OK;
}

static bool import_variable(int file, std::string& name)
{
    int dataset = getDataSetIdFromName(file, name.data());
    if (dataset <= 0)
    {
        return false;
    }

    types::InternalType* data = import_data(dataset);
    if (data != nullptr)
    {
        wchar_t* var = to_wide_string(name.data());
        //update macro name
        if (data->isMacro())
        {
            types::Macro* macro = data->getAs<types::Macro>();
            macro->setName(var);
            symbol::Context::getInstance()->addMacro(macro);
        }
        else
        {
            //add to context
            symbol::Context::getInstance()->put(symbol::Symbol(var), data);
        }
        FREE(var);
        return true;
    }

    return false;
}

types::InternalType* import_data(int dataset)
{
    //get var type
    char* ctype = getScilabTypeFromDataSet6(dataset);
    std::string type(ctype);
    FREE(ctype);

    if (type == g_SCILAB_CLASS_DOUBLE)
    {
        return import_double(dataset);
    }

    if (type == g_SCILAB_CLASS_STRING)
    {
        return import_string(dataset);
    }

    if (type == g_SCILAB_CLASS_INT)
    {
        return import_int(dataset);
    }

    if (type == g_SCILAB_CLASS_BOOLEAN)
    {
        return import_boolean(dataset);
    }

    if (type == g_SCILAB_CLASS_LIST)
    {
        return import_list(dataset, new types::List());
    }

    if (type == g_SCILAB_CLASS_TLIST)
    {
        return import_list(dataset, new types::TList());
    }

    if (type == g_SCILAB_CLASS_MLIST)
    {
        return import_list(dataset, new types::MList());
    }

    if (type == g_SCILAB_CLASS_STRUCT)
    {
        return import_struct(dataset);
    }

    if (type == g_SCILAB_CLASS_POLY)
    {
        return import_poly(dataset);
    }

    if (type == g_SCILAB_CLASS_SPARSE)
    {
        return import_sparse(dataset);
    }

    if (type == g_SCILAB_CLASS_BSPARSE)
    {
        return import_boolean_sparse(dataset);
    }

    if (type == g_SCILAB_CLASS_CELL)
    {
        return import_cell(dataset);
    }

    if (type == g_SCILAB_CLASS_HANDLE)
    {
        return import_handles(dataset);
    }

    if (type == g_SCILAB_CLASS_MACRO)
    {
        return import_macro(dataset);
    }

    if (type == g_SCILAB_CLASS_VOID)
    {
        closeDataSet(dataset);
        return new types::Void();
    }

    if (type == g_SCILAB_CLASS_UNDEFINED)
    {
        closeDataSet(dataset);
        return new types::ListUndefined();
    }

    if (type == g_SCILAB_CLASS_USERTYPE)
    {
        return import_usertype(dataset);
    }


    return nullptr;
}

static types::InternalType* import_double(int dataset)
{
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    std::vector<int> d(dims);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size <= 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    types::Double* dbl = new types::Double(dims, d.data(), complex == 1);

    double* real = dbl->get();
    double* img = dbl->getImg();

    if (complex)
    {
        readDoubleComplexMatrix(dataset, real, img);
    }
    else
    {
        readDoubleMatrix(dataset, real);
    }
    return dbl;
}

static types::InternalType* import_string(int dataset)
{
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }


    std::vector<int> d(dims);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size <= 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    std::vector<char*> s(size);
    ret = readStringMatrix(dataset, s.data());

    types::String* str = new types::String(dims, d.data());
    wchar_t** pstr = str->get();

    for (int i = 0; i < size; ++i)
    {
        pstr[i] = to_wide_string(s[i]);
    }

    freeStringMatrix(dataset, s.data());

    return str;
}

static types::InternalType* import_boolean(int dataset)
{
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    std::vector<int> d(dims);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size <= 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    types::Bool* bools = new types::Bool(dims, d.data());

    int* b = bools->get();
    readBooleanMatrix(dataset, b);
    return bools;
}

static types::InternalType* import_int(int dataset)
{
    types::InternalType* pOut = nullptr;
    int complex = 0;
    int dims = 0;
    int ret = getDatasetInfo(dataset, &complex, &dims, NULL);
    if (ret < 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    std::vector<int> d(dims);
    int size = getDatasetInfo(dataset, &complex, &dims, d.data());


    if (dims == 0 || size <= 0)
    {
        closeDataSet(dataset);
        return types::Double::Empty();
    }

    int prec = 0;
    ret = getDatasetPrecision(dataset, &prec);
    if (ret != 0)
    {
        closeDataSet(dataset);
        return nullptr;
    }

    switch (prec)
    {
        case SCI_INT8:
        {
            types::Int8* pi = new types::Int8(dims, d.data());
            ret = readInteger8Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_INT16:
        {
            types::Int16* pi = new types::Int16(dims, d.data());
            ret = readInteger16Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_INT32:
        {
            types::Int32* pi = new types::Int32(dims, d.data());
            ret = readInteger32Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_INT64:
        {
            types::Int64* pi = new types::Int64(dims, d.data());
            ret = readInteger64Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT8:
        {
            types::UInt8* pi = new types::UInt8(dims, d.data());
            ret = readUnsignedInteger8Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT16:
        {
            types::UInt16* pi = new types::UInt16(dims, d.data());
            ret = readUnsignedInteger16Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT32:
        {
            types::UInt32* pi = new types::UInt32(dims, d.data());
            ret = readUnsignedInteger32Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        case SCI_UINT64:
        {
            types::UInt64* pi = new types::UInt64(dims, d.data());
            ret = readUnsignedInteger64Matrix(dataset, pi->get());
            pOut = pi;
            break;
        }
        default:
            return nullptr;
            break;
    }

    return pOut;
}

static types::InternalType* import_list(int dataset, types::List* lst)
{
    int count = 0;
    int ret  = getListDims6(dataset, &count);
    if (ret)
    {
        closeList6(dataset);
        delete lst;
        return nullptr;
    }

    if (count == 0)
    {
        closeList6(dataset);
        return lst;
    }

    //get children

    for (int i = 0; i < count; ++i)
    {
        int data = getDataSetIdFromName(dataset, std::to_string(i).data());
        if (data <= 0)
        {
            closeList6(dataset);
            delete lst;
            return nullptr;
        }

        types::InternalType* child = import_data(data);
        if (child == nullptr)
        {
            closeList6(dataset);
            delete lst;
            return nullptr;
        }

        lst->append(child);
        if (child->isList())
        {
            child->killMe();
        }
    }

    closeList6(dataset);
    return lst;
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

static types::InternalType* import_struct(int dataset)
{
    //get struct dims node
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    types::Struct* str = new types::Struct(static_cast<int>(pdims.size()), pdims.data());
    size = str->getSize();
    if (size == 0)
    {
        //empty struct
        closeList6(dataset);
        delete str;
        return new types::Struct();
    }

    types::SingleStruct** sstr = str->get();

    int fieldCount = 0;
    int ret = getListDims6(dataset, &fieldCount);
    if (ret < 0)
    {
        closeList6(dataset);
        return str;
    }

    //get fields name
    int dfield = getDataSetIdFromName(dataset, "__fields__");
    int dim = 0;
    getDatasetInfo(dfield, &complex, &dim, NULL);
    std::vector<int> d(dim);
    size = getDatasetInfo(dfield, &complex, &dim, d.data());
    if (size < 0)
    {
        closeList6(dataset);
        delete str;
        return nullptr;
    }

    //get dims value
    std::vector<char*> fields(size);
    readStringMatrix(dfield, fields.data());

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");

    for (const auto & name : fields)
    {
        wchar_t* field = to_wide_string(name);
        str->addField(field);

        int dataref = getDataSetIdFromName(dataset, name);
        if (dataref < 0)
        {
            closeList6(dataset);
            freeStringMatrix(dfield, fields.data());
            FREE(field);
            delete str;
            return nullptr;
        }

        int refdim = 0;
        getDatasetInfo(dataref, &complex, &refdim, NULL);
        std::vector<int> refdims(refdim);
        int refcount = getDatasetInfo(dataref, &complex, &refdim, refdims.data());
        std::vector<hobj_ref_t> vrefs(refcount);
        ret = H5Dread(dataref, H5T_STD_REF_OBJ, H5S_ALL, H5S_ALL, H5P_DEFAULT, vrefs.data());
        if (ret < 0)
        {
            freeStringMatrix(dfield, fields.data());
            FREE(field);
            delete str;
            return nullptr;
        }


        //import field
        for (int j = 0; j < refcount; ++j)
        {
            int data = H5Rdereference(refs, H5R_OBJECT, &vrefs[j]);
            if (data < 0)
            {
                freeStringMatrix(dfield, fields.data());
                FREE(field);
                delete str;
                return nullptr;
            }

            types::InternalType* val = import_data(data);
            if (val == nullptr)
            {
                freeStringMatrix(dfield, fields.data());
                FREE(field);
                delete str;
                return nullptr;
            }

            sstr[j]->set(field, val);

        }

        FREE(field);
        closeDataSet(dataref);
    }

    freeStringMatrix(dfield, fields.data());
    closeList6(refs);
    closeList6(dataset);
    return str;
}

static types::InternalType* import_poly(int dataset)
{
    //get poly dims node
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get variable name
    char* var = NULL;
    int varname = getDataSetIdFromName(dataset, "__varname__");
    readStringMatrix(varname, &var);
    wchar_t* wvar = to_wide_string(var);
    std::wstring wvarname(wvar);
    FREE(wvar);
    freeStringMatrix(varname, &var);

    types::Polynom* p = new types::Polynom(wvarname, static_cast<int>(pdims.size()), pdims.data());
    types::SinglePoly** pss = p->get();

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    size = p->getSize();

    //loop on children
    for (int i = 0; i < size; ++i)
    {
        //forge name
        std::string polyname(std::to_string(i));
        int poly = getDataSetIdFromName(refs, polyname.data());

        //get dims
        complex = 0;
        int dims = 0;
        int ret = getDatasetInfo(poly, &complex, &dims, NULL);
        if (ret < 0)
        {
            return nullptr;
        }

        std::vector<int> d(dims);
        int datasize = getDatasetInfo(poly, &complex, &dims, d.data());

        types::SinglePoly* ss = NULL;

        //get coef
        if (dims == 0 || datasize <= 0)
        {
            ss = new types::SinglePoly();
        }
        else if (complex)
        {
            double* real = NULL;
            double* img = NULL;
            //create singlepoly
            ss = new types::SinglePoly(&real, &img, datasize - 1);
            readDoubleComplexMatrix(poly, real, img);
        }
        else
        {
            double* real = NULL;
            ss = new types::SinglePoly(&real, datasize - 1);
            readDoubleMatrix(poly, real);
        }

        pss[i] = ss;
    }

    closeList6(refs);
    closeList6(dataset);
    return p;
}

static types::InternalType* import_sparse(int dataset)
{
    types::Sparse* sp = nullptr;
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get non zeros count
    int nnz = 0;
    int datannz = getDataSetIdFromName(dataset, "__nnz__");
    readInteger32Matrix(datannz, &nnz);

    if (nnz == 0)
    {
        closeList6(dataset);
        return new types::Sparse(pdims[0], pdims[1]);
    }

    //get inner vector
    int datain = getDataSetIdFromName(dataset, "__inner__");
    int dimin = 0;
    int sizein = getDatasetInfo(datain, &complex, &dimin, NULL);
    std::vector<int> dimsin(dimin);
    sizein = getDatasetInfo(datain, &complex, &dimin, dimsin.data());
    if (sizein < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    std::vector<int> in(sizein);
    int ret = readInteger32Matrix(datain, in.data());
    if (ret < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //get outer vector
    int dataout = getDataSetIdFromName(dataset, "__outer__");
    int dimout = 0;
    int sizeout = getDatasetInfo(dataout, &complex, &dimout, NULL);
    std::vector<int> dimsout(dimout);
    sizeout = getDatasetInfo(dataout, &complex, &dimout, dimsout.data());
    if (sizeout < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    std::vector<int> out(sizeout);
    ret = readInteger32Matrix(dataout, out.data());
    if (ret < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //get data
    int ddata = getDataSetIdFromName(dataset, "__data__");
    int dimdata = 0;
    int sizedata = getDatasetInfo(ddata, &complex, &dimdata, NULL);
    std::vector<int> dimsdata(dimdata);
    sizedata = getDatasetInfo(ddata, &complex, &dimdata, dimsdata.data());
    if (sizedata < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    std::vector<double> real(sizedata);

    if (complex)
    {
        std::vector<double> img(sizedata);
        ret = readDoubleComplexMatrix(ddata, real.data(), img.data());
        if (ret < 0)
        {
            closeList6(dataset);
            return nullptr;
        }

        sp = new types::Sparse(pdims[0], pdims[1], nnz, in.data(), out.data(), real.data(), img.data());
    }
    else
    {
        ret = readDoubleMatrix(ddata, real.data());
        if (ret < 0)
        {
            closeList6(dataset);
            return nullptr;
        }

        sp = new types::Sparse(pdims[0], pdims[1], nnz, in.data(), out.data(), real.data(), nullptr);
    }

    closeList6(dataset);
    return sp;
}

static types::InternalType* import_boolean_sparse(int dataset)
{
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    //get non zeros count
    int nnz = 0;
    int datannz = getDataSetIdFromName(dataset, "__nnz__");
    readInteger32Matrix(datannz, &nnz);

    if (nnz == 0)
    {
        closeList6(dataset);
        return new types::SparseBool(pdims[0], pdims[1]);
    }

    //get inner vector
    int datain = getDataSetIdFromName(dataset, "__inner__");
    int dimin = 0;
    int sizein = getDatasetInfo(datain, &complex, &dimin, NULL);
    std::vector<int> dimsin(dimin);
    sizein = getDatasetInfo(datain, &complex, &dimin, dimsin.data());
    if (sizein < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    std::vector<int> in(sizein);
    int ret = readInteger32Matrix(datain, in.data());
    if (ret < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //get outer vector
    int dataout = getDataSetIdFromName(dataset, "__outer__");
    int dimout = 0;
    int sizeout = getDatasetInfo(dataout, &complex, &dimout, NULL);
    std::vector<int> dimsout(dimout);
    sizeout = getDatasetInfo(dataout, &complex, &dimout, dimsout.data());
    if (sizeout < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    std::vector<int> out(sizeout);
    ret = readInteger32Matrix(dataout, out.data());
    if (ret < 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    closeList6(dataset);

    return new types::SparseBool(pdims[0], pdims[1], nnz, in.data(), out.data());
}

static types::InternalType* import_cell(int dataset)
{
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    if (size == 0)
    {
        closeList6(dataset);
        return new types::Cell();
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    std::vector<types::InternalType*> data(size);
    for (int i = 0; i < size; ++i)
    {
        int ref = getDataSetIdFromName(refs, std::to_string(i).data());
        types::InternalType* val = import_data(ref);
        if (val == nullptr)
        {
            return nullptr;
        }

        data[i] = val;
    }

    types::Cell* cell = new types::Cell(static_cast<int>(pdims.size()), pdims.data(), data.data());

    closeList6(refs);
    closeList6(dataset);
    return cell;
}

static types::InternalType* import_handles(int dataset)
{
    //get sparse dimensions
    int complex = 0;
    std::vector<int> pdims;
    int size = getDimsNode(dataset, &complex, pdims);

    if (size == 0)
    {
        closeList6(dataset);
        return nullptr;
    }

    //open __refs__ node
    int refs = getDataSetIdFromName(dataset, "__refs__");
    types::GraphicHandle* handles = new types::GraphicHandle(static_cast<int>(pdims.size()), pdims.data());
    long long* h = handles->get();

    if (size == 1)
    {
        //%h_copy
        int ref = getDataSetIdFromName(refs, std::to_string(0).data());
        int val = add_current_entity(ref);
        if (val < 0)
        {
            return nullptr;
        }

        h[0] = getHandle(val);
    }
    else
    {
        for (int i = 0; i < size; ++i)
        {
            int ref = getDataSetIdFromName(refs, std::to_string(i).data());
            int val = import_handle(ref, -1);
            if (val < 0)
            {
                return nullptr;
            }

            h[i] = getHandle(val);
        }
    }
    closeList6(refs);
    closeList6(dataset);


    //update links property of legend handle
    if (Links::count())
    {
        std::list<int> legends = Links::legends();
        for (auto & i : legends)
        {
            Links::PathList paths = Links::get(i);
            update_link_path(i, paths);
        }
    }

    return handles;
}

static types::InternalType* import_macro(int dataset)
{
    int complex = 0;
    int dims = 0;
    int size = 0;
    std::vector<int> d(2);

    std::list<symbol::Variable*>* inputList = new std::list<symbol::Variable*>();
    std::list<symbol::Variable*>* outputList = new std::list<symbol::Variable*>();
    ast::Exp* body = nullptr;

    symbol::Context* ctx = symbol::Context::getInstance();

    //inputs
    int inputNode = getDataSetIdFromName(dataset, "inputs");
    size = getDatasetInfo(inputNode, &complex, &dims, d.data());
    if (size < 0)
    {
        delete inputList;
        delete outputList;
        closeList6(dataset);
        return nullptr;
    }
    std::vector<char*> inputNames(size);

    if (size != 0)
    {
        readStringMatrix(inputNode, inputNames.data());

        for (auto & input : inputNames)
        {
            wchar_t* winput = to_wide_string(input);
            symbol::Variable* var = ctx->getOrCreate(symbol::Symbol(winput));
            FREE(winput);
            inputList->push_back(var);
        }

        freeStringMatrix(inputNode, inputNames.data());
    }
    else
    {
        closeDataSet(inputNode);
    }

    //outputs
    int outputNode = getDataSetIdFromName(dataset, "outputs");
    size = getDatasetInfo(outputNode, &complex, &dims, d.data());
    if (size < 0)
    {
        delete inputList;
        delete outputList;
        closeList6(dataset);
        return nullptr;
    }
    std::vector<char*> outputNames(size);

    if (size != 0)
    {
        readStringMatrix(outputNode, outputNames.data());

        for (auto & output : outputNames)
        {
            wchar_t* woutput = to_wide_string(output);
            symbol::Variable* var = ctx->getOrCreate(symbol::Symbol(woutput));
            FREE(woutput);
            outputList->push_back(var);
        }

        freeStringMatrix(outputNode, outputNames.data());
    }
    else
    {
        closeDataSet(outputNode);
    }

    //body
    int bodyNode = getDataSetIdFromName(dataset, "body");
    size = getDatasetInfo(bodyNode, &complex, &dims, d.data());
    if (size < 0)
    {
        delete inputList;
        delete outputList;
        closeList6(dataset);
        return nullptr;
    }
    std::vector<unsigned char> bodybin(size);
    readUnsignedInteger8Matrix(bodyNode, bodybin.data());

    ast::DeserializeVisitor ds(bodybin.data());
    body = ds.deserialize();

    //wname+1 is to remove "/" at the start of the var name from HDF5
    types::Macro* macro = new types::Macro(L"", *inputList, *outputList, *body->getAs<ast::SeqExp>(), L"script");
    delete body;
    closeList6(dataset);
    return macro;
}

static types::InternalType* import_usertype(int dataset)
{
    types::InternalType* it = import_struct(dataset);
    if (it == nullptr || it->isStruct() == false)
    {
        delete it;
        return nullptr;
    }


    types::Struct* str = it->getAs<types::Struct>();

    if (str->isScalar() == false)
    {
        delete it;
        return nullptr;
    }

    types::SingleStruct* ss = str->get()[0];

    //extract type from struct
    types::InternalType* itType  = ss->get(L"type");
    if (itType == nullptr || itType->getId() != types::InternalType::IdScalarString)
    {
        delete it;
        return nullptr;
    }

    types::String* s = it->getAs<types::String>();
    wchar_t* type = s->get()[0];

    types::InternalType* data = ss->get(L"data");
    if (data == nullptr)
    {
        delete it;
        return nullptr;
    }

    //call %yourtype_load overload
    types::typed_list in;
    in.push_back(data);

    types::typed_list out;
    //overload
    // rational case
    std::wstring wstFuncName = L"%" + data->getShortTypeStr() + L"_load";
    types::Callable::ReturnValue ret = Overload::call(wstFuncName, in, 1, out);

    //clean temporary variables
    delete it; //included type and data

    if (ret != types::Callable::OK)
    {
        return nullptr;
    }

    if (out.size() != 1)
    {
        for (auto & i : out)
        {
            i->killMe();
        }

        return nullptr;
    }

    return out[0];
}
