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
