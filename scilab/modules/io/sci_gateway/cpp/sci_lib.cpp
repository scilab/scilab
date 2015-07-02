/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "library.hxx"
#include "loadlib.hxx"

extern "C"
{
#include "Scierror.h"
#include "sci_malloc.h"
#include "expandPathVariable.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_lib(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iXMLFileLen = 0;
    if (in.size() != 1)
    {
        Scierror(78, _("%s: Wrong number of input argument(s): %d expected.\n"), "lib", 1);
        return Function::Error;
    }

    InternalType* pIT = in[0];

    if (pIT->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "lib", 1);
        return Function::Error;
    }

    String *pS = pIT->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), "lib", 1);
        return Function::Error;
    }

    wchar_t* pstPath = pS->get(0);
    wchar_t* pwstPath = expandPathVariableW(pstPath);
    Library* lib = loadlib(pwstPath, false, false);
    FREE(pwstPath);

    if (lib == NULL)
    {
        char* path = wide_string_to_UTF8(pstPath);
        Scierror(999, "File %s does not exist or read access denied.", path);
        FREE(path);
        return Function::Error;
    }

    out.push_back(lib);
    return Function::OK;
}


/*--------------------------------------------------------------------------*/
