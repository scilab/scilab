/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

#include <hdf5.h>
#include "hdf5_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "loadlib.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "FileExist.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
#include "h5_attributeConstants.h"
#include "h5_readDataFromFile.h"
}

static const std::string fname("import_from_hdf5");

Function::ReturnValue sci_import_from_hdf5(typed_list &in, int _iRetCount, typed_list& out)
{
    int rhs = static_cast<int>(in.size());
    if (rhs < 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 1);
        return Function::Error;
    }

    if (in[0]->getId() != InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname.data(), 1);
        return Function::Error;
    }

    wchar_t* wcfilename = expandPathVariableW(in[0]->getAs<String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wcfilename);
    std::string filename(cfilename);
    std::wstring wfilename(wcfilename);
    FREE(cfilename);
    FREE(wcfilename);

    if (FileExistW(wfilename.data()) == FALSE)
    {
        Scierror(999, _("%s: Unable to open file: '%s'.\n"), fname.data(), filename.data());
        return Function::Error;
    }

    //library ?
    if (isHDF5File(filename.data()) == false)
    {
        //lib file
        int err = 0;
        Library* lib = loadlib(wfilename, &err);

        switch (err)
        {
            case 1:
                Scierror(999, _("%s: %s is not a valid module file.\n"), fname.data(), filename.data());
                return Function::Error;
            case 2:
                Scierror(999, "%s: %s", fname.data(), _("Redefining permanent variable.\n"));
                return Function::Error;
            default:
                break;
        }

        lib->killMe();
        return Function::OK;
    }

    int iFile = openHDF5File(filename.data(), 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname.data(), filename.data());
        return Function::Error;
    }

    std::wstring wstFuncName;
    //manage version information
    int version = getSODFormatAttribute(iFile);
    closeHDF5File(iFile);

    bool needReprocess = false;
    switch (version)
    {
        case -1:
        case 1:
        {
            wstFuncName = L"import_from_hdf5_v1";
            needReprocess = true;
            break;
        }
        case 2:
        {
            wstFuncName = L"import_from_hdf5_v2";
            needReprocess = true;
            break;
        }
        case 3:
        {
            wstFuncName = L"import_from_hdf5_v3";
            break;
        }
        default :
        {
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, version);
            return Function::Error;
        }
    }


    ast::ExecVisitor exec;
    typed_list out1;
    Function::ReturnValue ret = Overload::call(wstFuncName, in, _iRetCount, out1, &exec);

    if (ret != Function::OK)
    {
        Scierror(999, _("%s: Unable to load '%s'\n"), fname.data(), filename.data());
        return Function::Error;
    }

    if (needReprocess)
    {
        //call %sodload
        String* vars = out1[0]->getAs<String>();
        vars->IncreaseRef();
        int size = vars->getSize();
        typed_list in2(1, vars);
        typed_list out2;
        std::wstring wstFuncName = L"%_sodload";
        Callable::ReturnValue Ret = Callable::Error;
        Ret = Overload::call(wstFuncName, in2, _iRetCount, out2, &exec);
        vars->DecreaseRef();

        if (out2.size() != size)
        {
            Scierror(999, _("%s: Unable to load '%s'\n"), fname.data(), filename.data());
            return Function::Error;
        }

        symbol::Context* ctx = symbol::Context::getInstance();
        wchar_t** names = vars->get();

        //update context with values return by %_sodload
        for (int i = 0; i < size; ++i)
        {
            ctx->put(symbol::Symbol(names[i]), out2[i]);
        }

        vars->killMe();
    }

    return ret;
}