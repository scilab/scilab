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

#include "checkers/UselessArgChecker.hxx"

namespace slint
{
void UselessArgChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const ast::exps_t & args = static_cast<const ast::FunctionDec &>(e).getArgs().getVars();
        useless.push(std::map<symbol::Symbol, Location>());
        std::map<symbol::Symbol, Location> & map = useless.top();
        for (const auto arg : args)
        {
            const ast::SimpleVar & var = *static_cast<const ast::SimpleVar *>(arg);
            map.emplace(var.getSymbol(), var.getLocation());
        }
    }
    else if (e.isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
        if (context.isFunIn(var.getSymbol()) && !context.isAssignedVar(var))
        {
            useless.top().erase(var.getSymbol());
        }
    }
}

void UselessArgChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isFunctionDec())
    {
        const std::map<symbol::Symbol, Location> & map = useless.top();
        for (const auto & p : map)
        {
            result.report(context, p.second, *this, _("Function argument might be unused: %s."), p.first);
        }
        useless.pop();
    }
}

const std::string UselessArgChecker::getName() const
{
    return "UselessArgChecker";
}
}
