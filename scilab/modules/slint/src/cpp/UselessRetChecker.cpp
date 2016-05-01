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

#include "checkers/UselessRetChecker.hxx"

namespace slint
{
void UselessRetChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const ast::exps_t & rets = static_cast<const ast::FunctionDec &>(e).getReturns().getVars();
        useless.push(std::map<symbol::Symbol, Location>());
        std::map<symbol::Symbol, Location> & map = useless.top();
        for (const auto ret : rets)
        {
            const ast::SimpleVar & var = *static_cast<const ast::SimpleVar *>(ret);
            map.emplace(var.getSymbol(), var.getLocation());
        }
    }
    else if (e.isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
        if (context.isFunOut(var.getSymbol()) && context.isAssignedVar(var))
        {
            useless.top().erase(var.getSymbol());
        }
    }
}

void UselessRetChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const std::map<symbol::Symbol, Location> & map = useless.top();
        for (const auto & p : map)
        {
            result.report(context, p.second, *this, _("Function returned value might be unused: %s."), p.first);
        }
        useless.pop();
    }
}

const std::string UselessRetChecker::getName() const
{
    return "UselessRetChecker";
}
}
