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

#include "checkers/IllegalCallsChecker.hxx"

namespace slint
{
void IllegalCallsChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.getParent()->isCallExp() || e.getParent()->isSeqExp())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(e).getSymbol().getName();
        if (illegal.find(name) != illegal.end())
        {
            result.report(context, e.getLocation(), *this, _("Illegal call: %s."), name);
        }
    }
}

void IllegalCallsChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string IllegalCallsChecker::getName() const
{
    return "IllegalCallsChecker";
}
}
