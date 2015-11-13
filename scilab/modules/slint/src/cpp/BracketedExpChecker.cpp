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

#include "checkers/BracketedExpChecker.hxx"

namespace slint
{

void BracketedExpChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    if (oe.getParent()->isOpExp() || oe.getParent()->isListExp())
    {
        std::pair<unsigned int, unsigned int> pos;
        bool bracketed = true;
        if (context.getPosition(oe.getLocation(), pos))
        {
            const wchar_t * code = context.getCode();
            do
            {
                --pos.first;
            }
            while (pos.first >= 0 && (code[pos.first] == L' ' || code[pos.first] == L'\t'));
            if (pos.first == -1)
            {
                bracketed = false;
            }
            else
            {
                while (pos.second < context.getCodeLength() && (code[pos.second] == L' ' || code[pos.second] == L'\t'))
                {
                    ++pos.second;
                }

                if (pos.second == context.getCodeLength() || code[pos.first] != L'(' || code[pos.second] != L')')
                {
                    bracketed = false;
                }
            }
        }
        else
        {
            bracketed = false;
        }

        if (!bracketed)
        {
            result.report(context, oe.getLocation(), *this, _("Expression is not bracketed."));
        }
    }
}

void BracketedExpChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string BracketedExpChecker::getName() const
{
    return "BracketedExpChecker";
}
}
