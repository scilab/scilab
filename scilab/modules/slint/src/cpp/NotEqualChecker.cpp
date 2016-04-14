/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "checkers/NotEqualChecker.hxx"

namespace slint
{

void NotEqualChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    if (oe.getOper() == ast::OpExp::ne)
    {
        std::pair<unsigned int, unsigned int> pos;
        const Location & lloc = oe.getLeft().getLocation();
        const Location & rloc = oe.getRight().getLocation();
        Location loc(lloc.last_line, rloc.first_line, lloc.last_column, rloc.first_column - 1);
        if (context.getPosition(loc, pos))
        {
            const wchar_t * code = context.getCode();
            while (code[pos.first] == L' ' || code[pos.first] == L'\t')
            {
                ++pos.first;
            }
            while (code[pos.second] == L' ' || code[pos.second] == L'\t')
            {
                --pos.second;
            }
            const std::wstring _op(code + pos.first, code + pos.second + 1);
            if (pos.first < pos.second && _op != op)
            {
                result.report(context, e.getLocation(), *this, _("Operator \'%s\' should be used rather than \'%s\'."), op, _op);
            }
        }
    }
}

void NotEqualChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string NotEqualChecker::getName() const
{
    return "NotEqualChecker";
}
}
