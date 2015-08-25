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

#include "checkers/UnreachableCodeChecker.hxx"

namespace slint
{

void UnreachableCodeChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::exps_t & exps = e.getExps();
    const ast::Exp * returnExp = nullptr;
    for (const auto exp : exps)
    {
        if (returnExp)
        {
            if (!exp->isCommentExp())
            {
                result.report(context, returnExp->getLocation(), *this, _("The code after the return statement is unreachable."));
                return;
            }
        }
        else if (exp->isReturnExp())
        {
            returnExp = exp;
        }
    }
}

void UnreachableCodeChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string UnreachableCodeChecker::getName() const
{
    return "UnreachableCodeChecker";
}
}
