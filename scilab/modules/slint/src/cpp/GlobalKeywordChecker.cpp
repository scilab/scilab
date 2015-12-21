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

#include "checkers/GlobalKeywordChecker.hxx"

namespace slint
{

void GlobalKeywordChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
        if (name == L"global")
        {
            result.report(context, ce.getName().getLocation(), *this, _("Function 'global' not allowed."));
        }
    }
}

void GlobalKeywordChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{

}

const std::string GlobalKeywordChecker::getName() const
{
    return "GlobalKeywordChecker";
}
}
