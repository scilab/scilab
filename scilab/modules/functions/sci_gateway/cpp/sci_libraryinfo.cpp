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
/*--------------------------------------------------------------------------*/

#include "functions_gw.hxx"
#include "context.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C" {
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "charEncoding.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_libraryinfo(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    /* Check the number of input argument */
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "libraryinfo", 1);
        return types::Function::Error;
    }

    /* Check the number of output argument */
    if (_iRetCount != 1 && _iRetCount != 2)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "libraryinfo", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "libraryinfo", 1);
        return types::Function::Error;
    }

    types::String* pS = in[0]->getAs<types::String>();
    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A String expected.\n"), "libraryinfo", 1);
        return types::Function::Error;
    }

    types::InternalType* pIT = symbol::Context::getInstance()->get(symbol::Symbol(pS->get(0)));
    if (pIT == nullptr || pIT->isLibrary() == false)
    {
        char* libname = wide_string_to_UTF8(pS->get()[0]);
        Scierror(999, _("%s: Invalid library %s.\n"), "libraryinfo", libname);
        FREE(libname);
        return types::Function::Error;
    }

    types::Library* lib = pIT->getAs<types::Library>();

    std::list<std::wstring> names;
    int size = lib->getMacrosName(names);
    types::String* pNames = new types::String(size, 1);
    int i = 0;
    for (auto name : names)
    {
        pNames->set(i++, name.c_str());
    }

    out.push_back(pNames);

    if (_iRetCount == 2)
    {
        out.push_back(new types::String(lib->getPath().c_str()));
    }
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
