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
