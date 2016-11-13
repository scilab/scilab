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

#include "gvn/OpValue.hxx"

namespace analysis
{

std::ostream & operator<<(std::ostream & out, const OpValue & ov)
{
    switch (ov.kind)
    {
        case OpValue::UNARYMINUS:
            out << "-" << ov.lnum;
            break;
        case OpValue::UNARYNEG:
            out << "~" << ov.lnum;
            break;
        case OpValue::PLUS:
            out << ov.lnum << "+" << ov.rnum;
            break;
        case OpValue::MINUS:
            out << ov.lnum << "-" << ov.rnum;
            break;
        case OpValue::TIMES:
        case OpValue::DOTTIMES:
            out << ov.lnum << "*" << ov.rnum;
            break;
        case OpValue::RDIV:
        case OpValue::DOTRDIV:
            out << ov.lnum << "/" << ov.rnum;
            break;
        case OpValue::POWER:
        case OpValue::DOTPOWER:
            out << ov.lnum << "^" << ov.rnum;
            break;
        default:
            out << ov.lnum << "??" << ov.rnum;
            break;
    }

    return out;
}

} // namespace analysis
