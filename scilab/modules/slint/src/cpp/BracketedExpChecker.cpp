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
            while (pos.first != -1 && (code[pos.first] == L' ' || code[pos.first] == L'\t'));
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
