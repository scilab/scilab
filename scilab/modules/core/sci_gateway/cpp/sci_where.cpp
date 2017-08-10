/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - scilab Enterprises - Cedric Delamarre
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

#include <vector>

#include "core_gw.hxx"
#include "function.hxx"
#include "configvariable.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_where(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 0)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected."), "where", 0);
        return types::Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected."), "where", 2);
        return types::Function::Error;
    }

    const ConfigVariable::WhereVector& where = ConfigVariable::getWhere();
    if (where.size() <= 1)
    {
        out.push_back(types::Double::Empty());
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    types::Double* pDblLines = new types::Double((int)where.size() - 1, 1);
    types::String* pStrNames = new types::String((int)where.size() - 1, 1);

    auto it = where.rbegin();
    pDblLines->set(0, it->m_line);
    it++;
    for (int i = 0; it != where.rend(); it++, i++)
    {
        pDblLines->set(i + 1, it->m_line);
        pStrNames->set(i, it->m_name.c_str());
    }

    pStrNames->set(static_cast<int>(where.size()) - 1, where.back().m_name.c_str());

    out.push_back(pDblLines);
    out.push_back(pStrNames);
    return types::Function::OK;
}
