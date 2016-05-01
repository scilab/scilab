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

#include "checkers/VariableNameChecker.hxx"

namespace slint
{
void VariableNameChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    std::wstring name;
    if (e.isSimpleVar())
    {
        if (context.getLHSExp())
        {
            const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
            if (!context.isFunOut(var.getSymbol()) && context.isAssignedVar(var))
            {
                name = var.getSymbol().getName();
            }
        }
    }
    else if (e.isVarDec())
    {
        const ast::VarDec & vd = static_cast<const ast::VarDec &>(e);
        name = vd.getSymbol().getName();
    }

    if (!name.empty())
    {
        if (!matcher.match(name))
        {
            result.report(context, e.getLocation(), *this, _("Variable name doesn\'t match the pattern: %s, %s"), name, matcher.getPattern());
        }

        if (minimalLen > 0 && name.length() < minimalLen)
        {
            result.report(context, e.getLocation(), *this, _("Variable name length is lower than minimum: %d < %d"), name.length(), minimalLen);
        }

        if (maximalLen > 0 && name.length() > maximalLen)
        {
            result.report(context, e.getLocation(), *this, _("Variable name length is greater than maximum: %d > %d"), name.length(), minimalLen);
        }
    }
}

void VariableNameChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string VariableNameChecker::getName() const
{
    return "VariableNameChecker";
}
}
