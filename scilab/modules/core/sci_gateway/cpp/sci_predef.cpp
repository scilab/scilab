/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
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

#include "core_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C"
{
#include "charEncoding.h"
#include "localization.h"
#include "Scierror.h"
}

static const std::string fname("predef");

types::Function::ReturnValue sci_predef(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int rhs = in.size();
    symbol::Context* ctx = symbol::Context::getInstance();
    if (rhs > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), fname.data(), 0, 1);
        return types::Function::Error;
    }

    if (rhs == 0)
    {
        //return size
        std::list<std::wstring> lst;
        int size = ctx->protectedVars(lst);
        out.push_back(new types::Double(size));
        return types::Function::OK;
    }

    types::InternalType* pIT = in[0];

    if (pIT->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected."), fname.data(),  1);
        return types::Function::Error;
    }

    types::String* pS = pIT->getAs<types::String>();

    if (pS->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected."), fname.data(), 1);
        return types::Function::Error;
    }

    std::wstring opt(pS->get()[0]);

    if (opt == L"all" || opt == L"a")
    {
        symbol::Context::getInstance()->protect();
    }
    else if (opt == L"clear" || opt == L"c")
    {
        symbol::Context::getInstance()->unprotect();
    }
    else if (opt == L"names" || opt == L"n")
    {
        std::list<std::wstring> lst;
        int size = ctx->protectedVars(lst);

        //create output string
        if (size == 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        types::String* pS = new types::String(size, 1);
        int i = 0;
        wchar_t** ws = pS->get();
        for (auto var : lst)
        {
            pS->set(i++, var.c_str());
        }

        out.push_back(pS);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s' expected.\n"), fname.data(), 1, "all", "clear", "names");
        return types::Function::Error;
    }

    return types::Function::OK;
}
