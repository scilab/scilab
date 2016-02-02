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
