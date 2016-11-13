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

const std::string VarExp::print(const std::map<uint64_t, std::string> & vars) const
{
    std::ostringstream os;
    const auto i = vars.find(var);
    if (i != vars.end())
    {
        os << i->second;
    }
    else
    {
        os << "$" << var;
    }

    if (exp > 1)
    {
        os << "^" << exp;
    }
    return os.str();
}

std::ostream & operator<<(std::ostream & out, const VarExp & ve)
{
    out << (char)('a' + ve.var);
    if (ve.exp > 1)
    {
        out << "^" << ve.exp;
    }
    return out;
}

}// namespace analysis
