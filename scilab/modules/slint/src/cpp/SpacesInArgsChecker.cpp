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

    const wchar_t * code = context.getCode();
    const unsigned int len = context.getCodeLength();
    bool first = true;
    for (const auto arg : args)
    {
        std::pair<unsigned int, unsigned int> pos;
        if (context.getPosition(arg->getLocation(), pos))
        {
            const wchar_t next = code[pos.second];
            if (next == L' ' || next == L'\t')
            {
                result.report(context, e.getLocation(), *this, _("No space after function argument."));
            }
            if (pos.first >= 2)
            {
                const wchar_t prev = code[pos.first - 1];
                if (first)
                {
                    if (prev == L' ' || prev == L'\t')
                    {
                        result.report(context, e.getLocation(), *this, _("No space before first function argument."));
                    }
                }
                else
                {
                    const wchar_t pprev = code[pos.first - 2];
                    if (prev != L' ' || pprev != L',')
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
