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

#include "checkers/ReturnsCountChecker.hxx"

namespace slint
{

void ReturnsCountChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        stack.push(0);
    }
    else if (e.isReturnExp())
    {
        ++stack.top();
    }
}

void ReturnsCountChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const unsigned int count = stack.top();
        if (maxReturns >= 0 && count > maxReturns)
        {
            result.report(context, e.getLocation(), *this, _("Too many return instructions: %d max."), maxReturns);
        }
        stack.pop();
    }
}

const std::string ReturnsCountChecker::getName() const
{
    return "ReturnsCountChecker";
}

} // namespace slint
