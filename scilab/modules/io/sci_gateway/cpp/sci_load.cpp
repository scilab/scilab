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
#include "api_scilab.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"
#include "io_gw.hxx"
#include "string.hxx"
#include "library.hxx"
#include "loadlib.hxx"

extern "C"
{
#include "sci_malloc.h"
#include "expandPathVariable.h"
#include "h5_fileManagement.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_load(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    InternalType* pIT = in[0];

    if (pIT->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "load", 1);
        return Function::Error;
    }

    String *pS = pIT->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Scalar string expected.\n"), "load", 1);
        return Function::Error;
    }

    wchar_t* pwstPathLib = expandPathVariableW(pS->get(0));
    char* pstPath = wide_string_to_UTF8(pwstPathLib);

    if (isHDF5File(pstPath))
    {
        FREE(pstPath);
        FREE(pwstPathLib);

        //call overload
        std::wstring wstFuncName = L"%_sodload";
        ast::ExecVisitor* exec = new ast::ExecVisitor();
        Callable::ReturnValue Ret = Callable::Error;
        Ret = Overload::call(wstFuncName, in, _iRetCount, out, exec);
        delete exec;
        return Ret;
    }
    else
    {
        Library* lib = loadlib(pwstPathLib);
        FREE(pstPath);
        FREE(pwstPathLib);

        if (lib == NULL)
        {
            Scierror(999, _("%s: Wrong file type.\n"), "load");
            return Function::Error;
        }
    }

    return Function::OK;
}


/*--------------------------------------------------------------------------*/
