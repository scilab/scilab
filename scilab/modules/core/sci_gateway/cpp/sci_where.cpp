/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

    std::list<std::pair<int, std::wstring> > lWhereAmI = ConfigVariable::getWhere();
    if (lWhereAmI.size() == 1)
    {
        out.push_back(types::Double::Empty());
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    std::list<std::pair<int, std::wstring> >::const_iterator it;
    types::Double* pDblLines = new types::Double(lWhereAmI.size() - 1, 1);
    types::String* pStrNames = new types::String(lWhereAmI.size() - 1, 1);

    it = lWhereAmI.begin();
    pDblLines->set(0, (double)it->first);
    it++;
    for (int i = 0; it != lWhereAmI.end(); it++, i++)
    {
        pDblLines->set(i + 1, (double)(it->first));
        pStrNames->set(i, it->second.c_str());
    }

    pStrNames->set(lWhereAmI.size() - 1, lWhereAmI.front().second.c_str());

    out.push_back(pDblLines);
    out.push_back(pStrNames);
    return types::Function::OK;
}
