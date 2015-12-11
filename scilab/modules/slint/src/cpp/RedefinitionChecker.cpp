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
