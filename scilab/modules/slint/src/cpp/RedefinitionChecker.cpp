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

#include "checkers/RedefinitionChecker.hxx"

namespace slint
{

void RedefinitionChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
    if (context.isAssignedVar(var))
    {
        const symbol::Symbol & sym = var.getSymbol();
        const std::wstring & name = sym.getName();
        if (SLintChecker::isScilabConstant(name))
        {
            result.report(context, e.getLocation(), *this, _("Redefinition of Scilab\'s constant: %s."), name);
        }
        else
        {
            types::InternalType * pIT = symbol::Context::getInstance()->get(sym);
            if (pIT)
            {
                if (pIT->isFunction())
                {
                    result.report(context, e.getLocation(), *this, _("Redefinition of a built-in function: %s."), name);
                }
                else if (pIT->isMacroFile())
                {
                    result.report(context, e.getLocation(), *this, _("Redefinition of a library function: %s."), name);
                }
                else if (pIT->isMacro())
                {
                    result.report(context, e.getLocation(), *this, _("Redefinition of a macro: %s."), name);
                }
            }
            else if (context.isPrivateFunction(sym))
            {
                result.report(context, e.getLocation(), *this, _("Redefinition of a private macro: %s."), name);
            }
            else if (context.getPublicFunction(name))
            {
                result.report(context, e.getLocation(), *this, _("Redefinition of a macro: %s."), name);
            }
        }
    }
}

void RedefinitionChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string RedefinitionChecker::getName() const
{
    return "RedefinitionChecker";
}
}
