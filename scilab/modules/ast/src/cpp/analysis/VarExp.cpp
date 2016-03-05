/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

#include "gvn/VarExp.hxx"

namespace analysis
{

const std::wstring VarExp::print(const std::map<uint64_t, std::wstring> & vars) const
{
    std::wostringstream wos;
    const auto i = vars.find(var);
    if (i != vars.end())
    {
        wos << i->second;
    }
    else
    {
        wos << L"$" << var;
    }

    if (exp > 1)
    {
        wos << L"^" << exp;
    }
    return wos.str();
}

std::wostream & operator<<(std::wostream & out, const VarExp & ve)
{
    out << (char)('a' + ve.var);
    if (ve.exp > 1)
    {
        out << L"^" << ve.exp;
    }
    return out;
}

}// namespace analysis
