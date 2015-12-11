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

#include "checkers/EqEqChecker.hxx"

namespace slint
{

void EqEqChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    if (oe.getParent()->isSeqExp() && oe.getOper() == ast::OpExp::eq && oe.getLeft().isSimpleVar())
    {
        result.report(context, e.getLocation(), *this, _("Possible confusion between assignment operator \'=' and equality operator \'==\'."));
    }
}

void EqEqChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string EqEqChecker::getName() const
{
    return "EqEqChecker";
}
}
