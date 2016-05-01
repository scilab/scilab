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

std::wostream & operator<<(std::wostream & out, const OpValue & ov)
{
    switch (ov.kind)
    {
        case OpValue::UNARYMINUS:
            out << L"-" << ov.lnum;
            break;
        case OpValue::UNARYNEG:
            out << L"~" << ov.lnum;
            break;
        case OpValue::PLUS:
            out << ov.lnum << L"+" << ov.rnum;
            break;
        case OpValue::MINUS:
            out << ov.lnum << L"-" << ov.rnum;
            break;
        case OpValue::TIMES:
        case OpValue::DOTTIMES:
            out << ov.lnum << L"*" << ov.rnum;
            break;
        case OpValue::RDIV:
        case OpValue::DOTRDIV:
            out << ov.lnum << L"/" << ov.rnum;
            break;
        case OpValue::POWER:
        case OpValue::DOTPOWER:
            out << ov.lnum << L"^" << ov.rnum;
            break;
        default:
            out << ov.lnum << L"??" << ov.rnum;
            break;
    }

    return out;
}

} // namespace analysis
