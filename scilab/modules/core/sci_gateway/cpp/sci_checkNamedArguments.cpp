/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2017 - ESI - Antoine ELIAS
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
#include "macro.hxx"
#include "context.hxx"
#include "double.hxx"
#include "string.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "sciprint.h"
}

const char fname[] = "checkNamedArguments";
types::Function::ReturnValue sci_checkNamedArguments(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int size = 0;
    symbol::Context* ctx = symbol::Context::getInstance();
    int rhs = static_cast<int>(in.size());
    std::vector<std::wstring> proto;

    if (ctx->getScopeLevel() < 2)
    {
        Scierror(999, _("%s: must be call from a function.\n"), fname);
        return types::Function::Error;
    }

    if (rhs != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 0);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    //get before last where
    const ConfigVariable::WhereEntry& where = *(++ConfigVariable::getWhere().crbegin());
    if (where.call->isMacro())
    {
        types::Macro* m = (types::Macro*)where.call;
        auto inputs = m->getInputs();
        proto.reserve(inputs->size());
        for (auto i : *inputs)
        {
            std::wstring var(i->getSymbol().getName());
            if (var != L"varargin")
            {
                proto.push_back(var);
            }
        }

        size = (int)proto.size();
    }

    typedef std::pair<std::wstring, int> lst_entry_t;
    std::list<lst_entry_t> lst;
    int count = ctx->getCurrentScope(lst, true);

    // remove nargin/nargout//varargin
    lst.remove_if([](const lst_entry_t& p) -> bool { return p.first == L"nargin"; });
    lst.remove_if([](const lst_entry_t& p) -> bool { return p.first == L"nargout"; });
    lst.remove_if([](const lst_entry_t& p) -> bool { return p.first == L"varargin"; });

    for (int i = 0; i < size; ++i)
    {
        lst.remove_if([&](const lst_entry_t& p) -> bool { return p.first == proto[i]; });
    }

    count = (int)lst.size();
    if (count == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    types::String* pOut = new types::String(count, 1);
    int i = 0;
    for (auto v : lst)
    {
        pOut->set(i++, v.first.data());
    }

    out.push_back(pOut);
    return types::Function::OK;
}
