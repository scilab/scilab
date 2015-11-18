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

#include "checkers/ExpInCondChecker.hxx"

namespace slint
{
void ExpInCondChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    unsigned int num = 0;
    if (e.isIfExp())
    {
        num = checkCond(static_cast<const ast::IfExp &>(e).getTest());
    }
    else if (e.isWhileExp())
    {
        num = checkCond(static_cast<const ast::WhileExp &>(e).getTest());
    }

    if (num && num >= max)
    {
        result.report(context, e.getLocation(), *this, _("Number of expressions in condition is limited: %d max."), max);
    }
}

void ExpInCondChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string ExpInCondChecker::getName() const
{
    return "ExpInCondChecker";
}

unsigned int ExpInCondChecker::checkCond(const ast::Exp & e) const
{
    if (e.isOpExp())
    {
        const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
        const ast::OpExp::Oper oper = oe.getOper();
        if (oper == ast::OpExp::Oper::logicalAnd || oper == ast::OpExp::Oper::logicalOr ||
                oper == ast::OpExp::Oper::logicalShortCutAnd || oper == ast::OpExp::Oper::logicalShortCutOr)
        {
            return checkCond(oe.getLeft()) + checkCond(oe.getRight());
        }
    }
    return 1;
}
}
