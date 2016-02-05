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
