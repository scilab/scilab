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

#include <hdf5.h>
#include "hdf5_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "overload.hxx"
#include "loadlib.hxx"
#include "context.hxx"

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

static const std::string fname("load");

types::Function::ReturnValue sci_hdf5_load(types::typed_list &in, int _iRetCount, types::typed_list& out)
{
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

    wchar_t* wcfilename = expandPathVariableW(in[0]->getAs<types::String>()->get()[0]);
    char* cfilename = wide_string_to_UTF8(wcfilename);
    std::string filename(cfilename);
    std::wstring wfilename(wcfilename);
    FREE(cfilename);
    FREE(wcfilename);

    if (FileExistW(wfilename.data()) == FALSE)
    {
        Scierror(999, _("%s: Unable to open file: '%s'.\n"), fname.data(), filename.data());
        return types::Function::Error;
    }

    //library ?
    if (isHDF5File(filename.data()) == false)
    {
        //lib file
        int err = 0;
        types::Library* lib = loadlib(in[0]->getAs<types::String>()->get()[0], &err);

        switch (err)
        {
            case 1:
                Scierror(999, _("%s: %s is not a valid lib file.\n"), fname.data(), filename.data());
                return types::Function::Error;
            case 2:
                Scierror(999, "%s: %s", fname.data(), _("Redefining permanent variable.\n"));
                return types::Function::Error;
            default:
                break;
        }

        lib->killMe();
        return types::Function::OK;
    }

    int iFile = openHDF5File(filename.data(), 0);
    if (iFile < 0)
    {
        Scierror(999, _("%s: Unable to open file: %s\n"), fname.data(), filename.data());
        return types::Function::Error;
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
            wstFuncName = L"hdf5_load_v1";
            needReprocess = true;
            break;
        }
        case 2:
        {
            wstFuncName = L"hdf5_load_v2";
            needReprocess = true;
            break;
        }
        case 3:
        {
            wstFuncName = L"hdf5_load_v3";
            break;
        }
        default :
        {
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, version);
            return types::Function::Error;
        }
    }

    types::typed_list out1;
    types::Function::ReturnValue ret = Overload::call(wstFuncName, in, _iRetCount, out1);

    if (ret != types::Function::OK)
    {
        Scierror(999, _("%s: Unable to load '%s'\n"), fname.data(), filename.data());
        return types::Function::Error;
    }

    if (needReprocess)
    {
        //call %sodload
        types::String* vars = out1[0]->getAs<types::String>();
        vars->IncreaseRef();
        int size = vars->getSize();
        types::typed_list in2(1, vars);
        types::typed_list out2;
        std::wstring wstFuncName = L"%_sodload";
        ret = Overload::call(wstFuncName, in2, size, out2);
        vars->DecreaseRef();

        symbol::Context* ctx = symbol::Context::getInstance();
        wchar_t** names = vars->get();

        //update context with values return by %_sodload
        for (int i = 0; i < size; ++i)
        {
            ctx->put(symbol::Symbol(names[i]), out2[i]);
        }

        vars->killMe();
    }
    else
    {
        out.push_back(out1.front());
    }

    return ret;
}
