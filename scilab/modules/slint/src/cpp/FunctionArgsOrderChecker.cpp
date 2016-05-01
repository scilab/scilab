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

#include <map>

#include "checkers/FunctionArgsOrderChecker.hxx"

namespace slint
{
void FunctionArgsOrderChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        ast::exps_t args = ce.getArgs();
        std::map<symbol::Symbol, unsigned int> assignments;
        unsigned int pos = 1;
        for (const auto arg : args)
        {
            if (arg->isAssignExp())
            {
                const ast::AssignExp & ae = *static_cast<const ast::AssignExp *>(arg);
                if (ae.getLeftExp().isSimpleVar())
                {
                    assignments.emplace(static_cast<ast::SimpleVar &>(ae.getLeftExp()).getSymbol(), pos);
                }
            }
            else if (!assignments.empty())
            {
                result.report(context, e.getLocation(), *this, _("Argument at position %d must be an assignment."), pos);
            }
            ++pos;
        }
        if (!assignments.empty())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            const ast::FunctionDec * fundec = context.getPublicFunction(name);
            if (!fundec)
            {
                fundec = context.getPrivateFunction(name);
            }

            if (fundec)
            {
                const ast::exps_t & funargs = fundec->getArgs().getVars();
                pos = 1;
                for (const auto arg : funargs)
                {
                    const symbol::Symbol & sym = static_cast<const ast::SimpleVar *>(arg)->getSymbol();
                    auto i = assignments.find(sym);
                    if (i != assignments.end())
                    {
                        if (pos != i->second)
                        {
                            result.report(context, e.getLocation(), *this, _("Argument %s declared at position %d and assigned at position %d."), sym.getName(), pos, i->second);
                        }
                    }
                    ++pos;
                }
            }
        }
    }
}

void FunctionArgsOrderChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string FunctionArgsOrderChecker::getName() const
{
    return "FunctionArgsOrderChecker";
}
}
