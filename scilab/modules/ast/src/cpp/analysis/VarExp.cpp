/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
