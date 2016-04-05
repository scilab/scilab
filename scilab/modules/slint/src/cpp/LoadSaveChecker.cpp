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

#include <iostream>

#include "checkers/LoadSaveChecker.hxx"

namespace slint
{
void LoadSaveChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isCallExp())
    {
        const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
        if (ce.getName().isSimpleVar())
        {
            const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
            if (name == L"load" || name == L"save")
            {
                const ast::exps_t args = ce.getArgs();
                if (args.size() == 0)
                {
                    result.report(context, e.getLocation(), *this, _("Function %s has at least one argument."), name);
                }
                else if (args.size() == 1)
                {
                    result.report(context, e.getLocation(), *this, _("Function %s should be called with names of variables."), name);
                }
                else
                {
                    for (ast::exps_t::const_iterator i = std::next(args.begin()), end = args.end(); i != end; ++i)
                    {
                        const ast::Exp & arg = **i;
                        if (!arg.isStringExp())
                        {
                            result.report(context, e.getLocation(), *this, _("Function %s should be called with constant strings."), name);
                            break;
                        }
                    }
                }
            }
        }
    }
}

void LoadSaveChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string LoadSaveChecker::getName() const
{
    return "LoadSaveChecker";
}
}
