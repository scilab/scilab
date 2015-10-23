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
