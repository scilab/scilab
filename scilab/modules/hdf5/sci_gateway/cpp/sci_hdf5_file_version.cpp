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
#include "double.hxx"
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

static const std::string fname("hdf5_file_version");

types::Function::ReturnValue sci_hdf5_file_version(types::typed_list &in, int _iRetCount, types::typed_list &out)
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

    if (version == -1)
    {
        version = 1;
    }

    out.push_back(new types::Double(static_cast<double>(version)));
    return types::Function::OK;
}