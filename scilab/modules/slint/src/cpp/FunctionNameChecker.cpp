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

#include "checkers/FunctionNameChecker.hxx"

namespace slint
{
void FunctionNameChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::FunctionDec & fdec = static_cast<const ast::FunctionDec &>(e);
    const std::wstring & name = fdec.getSymbol().getName();
    if (!matcher.match(name))
    {
        result.report(context, e.getLocation(), *this, _("Function name doesn\'t match the pattern: %s, %s"), name, matcher.getPattern());
    }

    if (minimalLen > 0 && name.length() < minimalLen)
    {
        result.report(context, e.getLocation(), *this, _("Function name length is lower than minimum: %d < %d"), name.length(), minimalLen);
    }

    if (maximalLen > 0 && name.length() > maximalLen)
    {
        result.report(context, e.getLocation(), *this, _("Function name length is greater than maximum: %d > %d"), name.length(), minimalLen);
    }
}

void FunctionNameChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string FunctionNameChecker::getName() const
{
    return "FunctionNameChecker";
}
}
