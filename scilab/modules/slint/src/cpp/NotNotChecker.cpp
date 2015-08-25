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

#include "checkers/NotNotChecker.hxx"

namespace slint
{

void NotNotChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::NotExp & ne = static_cast<const ast::NotExp &>(e);
    if (ne.getParent()->isNotExp())
    {
        result.report(context, e.getLocation(), *this, _("Double negation is not allowed."));
    }
}

void NotNotChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string NotNotChecker::getName() const
{
    return "NotNotChecker";
}
}
