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
