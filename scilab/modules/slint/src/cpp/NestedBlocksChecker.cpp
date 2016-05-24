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

#include "checkers/NestedBlocksChecker.hxx"

namespace slint
{

void NestedBlocksChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (max >= 0)
    {
        if (e.isIfExp() && !stack.empty())
        {
            std::pair<unsigned int, unsigned int> pos;
            if (context.getPosition(e.getLocation(), pos))
            {
                const wchar_t * kw = context.getCode() + pos.first;
                if (*kw == L'e')
                {
                    // we are in a elseif
                    stack.erase(std::prev(stack.end()));
                }
            }
        }

        stack.emplace_back(&e);
        if (stack.size() == (1 + (unsigned int)max))
        {
            result.report(context, stack.back()->getLocation(), *this, _("Too many nested blocks: %d max."), max);
        }
    }
}

void NestedBlocksChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (max >= 0)
    {
        if (!stack.empty())
        {
            stack.erase(std::prev(stack.end()));
        }
    }
}

const std::string NestedBlocksChecker::getName() const
{
    return "NestedBlocksChecker";
}
}
