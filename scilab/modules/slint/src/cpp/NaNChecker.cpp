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

#include "checkers/NaNChecker.hxx"

namespace slint
{

void NaNChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    const ast::OpExp::Oper oper = oe.getOper();

    if (oper == ast::OpExp::lt || ast::OpExp::le || ast::OpExp::gt || ast::OpExp::ge || ast::OpExp::ne || ast::OpExp::eq)
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
