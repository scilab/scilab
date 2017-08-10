/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include "context.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

enum ScopeRange
{
    Local,
    NoLocal,
    All,
    UnknownRange
};

static ScopeRange getScopeFromOption(const wchar_t *_psScope)
{
    if (wcscmp(_psScope, L"all") == 0 || wcscmp(_psScope, L"a") == 0)
    {
        return All;
    }
    if (wcscmp(_psScope, L"local") == 0 || wcscmp(_psScope, L"l") == 0)
    {
        return Local;
    }
    if (wcscmp(_psScope, L"nolocal") == 0 || wcscmp(_psScope, L"n") == 0)
    {
        return NoLocal;
    }

    return UnknownRange;
}

types::Function::ReturnValue sci_existsOrIsdef(types::typed_list &in, int _iRetCount, types::typed_list &out, const char* fname)
{
    types::String* pStrIn       = NULL;

    if (in.size() != 1 && in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), fname, 1, 2);
        return types::Function::Error;
    }

    if (!in[0]->isString())
    {
        Scierror(999, _("%s: Wrong type for argument #%d: Matrix of strings expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (in.size() == 2 && (!in[1]->isString() || in[1]->getAs<types::String>()->getSize() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), fname, 2);
        return types::Function::Error;
    }

    const wchar_t *psScope = L"all"; // Default option is "all"
    if (in.size() == 2)
    {
        psScope = in[1]->getAs<types::String>()->get(0);
    }

    pStrIn  = in[0]->getAs<types::String>();
    if (strcmp(fname, "exists") == 0)
    {
        types::Double* pDblOut = new types::Double(pStrIn->getDims(), pStrIn->getDimsArray());
        switch (getScopeFromOption(psScope))
        {
            case All:
                for (int i = 0; i < pStrIn->getSize(); i++)
                {
                    symbol::Context::getInstance()->get(symbol::Symbol(pStrIn->get(i))) ? pDblOut->set(i, 1) : pDblOut->set(i, 0);
                }
                break;
            case Local:
                for (int i = 0; i < pStrIn->getSize(); i++)
                {
                    symbol::Context::getInstance()->getCurrentLevel(symbol::Symbol(pStrIn->get(i))) ? pDblOut->set(i, 1) : pDblOut->set(i, 0);
                }
                break;
            case NoLocal:
                for (int i = 0; i < pStrIn->getSize(); i++)
                {
                    symbol::Context::getInstance()->getAllButCurrentLevel(symbol::Symbol(pStrIn->get(i))) ? pDblOut->set(i, 1) : pDblOut->set(i, 0);
                }
                break;
            default :
                Scierror(36, _("%s: Wrong input argument %d.\n"), fname, 2);
                return types::Function::Error;
        }

        out.push_back(pDblOut);
    }
    else
    {
        types::Bool* pBOut = new types::Bool(pStrIn->getDims(), pStrIn->getDimsArray());
        switch (getScopeFromOption(psScope))
        {
            case All:
                for (int i = 0; i < pStrIn->getSize(); i++)
                {
                    symbol::Context::getInstance()->get(symbol::Symbol(pStrIn->get(i))) ? pBOut->set(i, 1) : pBOut->set(i, 0);
                }
                break;
            case Local:
                for (int i = 0; i < pStrIn->getSize(); i++)
                {
                    symbol::Context::getInstance()->getCurrentLevel(symbol::Symbol(pStrIn->get(i))) ? pBOut->set(i, 1) : pBOut->set(i, 0);
                }
                break;
            case NoLocal:
                for (int i = 0; i < pStrIn->getSize(); i++)
                {
                    symbol::Context::getInstance()->getAllButCurrentLevel(symbol::Symbol(pStrIn->get(i))) ? pBOut->set(i, 1) : pBOut->set(i, 0);
                }
                break;
            default :
                Scierror(36, _("%s: Wrong input argument %d.\n"), fname, 2);
                return types::Function::Error;
        }

        out.push_back(pBOut);
    }

    return types::Function::OK;
}

types::Function::ReturnValue sci_exists(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return sci_existsOrIsdef(in, _iRetCount, out, "exists");
}

types::Function::ReturnValue sci_isdef(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return sci_existsOrIsdef(in, _iRetCount, out, "isdef");
}
