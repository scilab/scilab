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
#include "overload.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "Scierror.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
#include "h5_attributeConstants.h"
#include "h5_readDataFromFile.h"
}

static const std::string fname("hdf5_listvar");

types::Function::ReturnValue sci_hdf5_listvar(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int rhs = static_cast<int>(in.size());
    if (rhs < 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): at least %d expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

    if (in[0]->getId() != types::InternalType::IdScalarString)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname.data(), 1);
        return types::Function::Error;
    }

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

    std::wstring wstFuncName;
    //manage version information
    int version = getSODFormatAttribute(iFile);
    closeHDF5File(iFile);
    switch (version)
    {
        case -1:
        case 1:
        case 2:
        {
            wstFuncName = L"hdf5_listvar_v2";
            break;
        }
        case 3:
        {
            wstFuncName = L"hdf5_listvar_v3";
            break;
        }
        default:
        {
            Scierror(999, _("%s: Wrong SOD file format version. Max Expected: %d Found: %d\n"), fname.data(), SOD_FILE_VERSION, version);
            return types::Function::Error;
        }
    }

    return Overload::call(wstFuncName, in, _iRetCount, out);
}
