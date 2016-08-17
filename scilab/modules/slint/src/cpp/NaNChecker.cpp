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

#include "checkers/NaNChecker.hxx"

namespace slint
{

void NaNChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    const ast::OpExp::Oper oper = oe.getOper();

    if (oper == ast::OpExp::lt || oper == ast::OpExp::le || oper == ast::OpExp::gt || oper == ast::OpExp::ge || oper == ast::OpExp::ne || oper == ast::OpExp::eq)
    {
        bool hasNaN = false;

        if (oe.getLeft().isSimpleVar())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(oe.getLeft()).getSymbol().getName();
            if (name == L"%nan")
            {
                hasNaN = true;
            }
        }
        if (!hasNaN && oe.getRight().isSimpleVar())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(oe.getRight()).getSymbol().getName();
            if (name == L"%nan")
            {
                hasNaN = true;
            }
        }

        if (hasNaN)
        {
            switch (oper)
            {
                case ast::OpExp::lt:
                case ast::OpExp::le:
                case ast::OpExp::gt:
                case ast::OpExp::ge:
                {
                    result.report(context, e.getLocation(), *this, _("Use %nan in a comparison expression will return false."));
                    break;
                }
                case ast::OpExp::ne:
                case ast::OpExp::eq:
                {
                    result.report(context, e.getLocation(), *this, _("Use isnan() when comparing values with %nan."));
                    break;
                }
            }
        }
    }
}

void NaNChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string NaNChecker::getName() const
{
    return "NaNChecker";
}
}
