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

#include "checkers/SpacesInArgsChecker.hxx"

namespace slint
{

void SpacesInArgsChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    ast::exps_t args;
    if (e.isFunctionDec())
    {
        args = static_cast<const ast::FunctionDec &>(e).getArgs().getVars();
    }
    else if (e.isCallExp())
    {
        args = static_cast<const ast::CallExp &>(e).getArgs();
    }

    const char * code = context.getCode();
    const unsigned int len = context.getCodeLength();
    bool first = true;
    for (const auto arg : args)
    {
        std::pair<unsigned int, unsigned int> pos;
        if (context.getPosition(arg->getLocation(), pos))
        {
            const char next = code[pos.second];
            if (next == ' ' || next == '\t')
            {
                result.report(context, e.getLocation(), *this, _("No space after function argument."));
            }
            if (pos.first >= 2)
            {
                const char prev = code[pos.first - 1];
                if (first)
                {
                    if (prev == ' ' || prev == '\t')
                    {
                        result.report(context, e.getLocation(), *this, _("No space before first function argument."));
                    }
                }
                else
                {
                    const char pprev = code[pos.first - 2];
                    if (prev != ' ' || pprev != ',')
                    {
                        result.report(context, e.getLocation(), *this, _("A function argument must be preceded by a single space."));
                    }
                }
            }
            first = false;
        }
    }
}

void SpacesInArgsChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string SpacesInArgsChecker::getName() const
{
    return "SpacesInArgsChecker";
}
}
