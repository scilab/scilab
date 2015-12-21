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
