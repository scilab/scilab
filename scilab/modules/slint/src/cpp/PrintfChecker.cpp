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

#include "checkers/PrintfChecker.hxx"

namespace slint
{
void PrintfChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    if (ce.getName().isSimpleVar())
    {
        const std::wstring & name = static_cast<const ast::SimpleVar &>(ce.getName()).getSymbol().getName();
        if (name == L"printf" || name == L"mprintf" || name  == L"sprintf" || name == L"msprintf")
        {
            ast::exps_t args = ce.getArgs();
            if (!args.empty())
            {
                const ast::Exp & first = *args.front();
                if (first.isStringExp())
                {
                    const std::wstring & format = static_cast<const ast::StringExp &>(first).getValue();
                    unsigned int percents = 0;
                    for (std::wstring::const_iterator i = format.begin(), end = format.end(); i != end; ++i)
                    {
                        if (*i == L'%')
                        {
                            if (std::next(i) != end)
                            {
                                if (*std::next(i) != L'%')
                                {
                                    ++percents;
                                }
                                ++i;
                            }
                        }
                    }
                    if (percents != args.size() - 1)
                    {
                        result.report(context, e.getLocation(), *this, _("Invalid number of arguments in %s: data don\'t correspond to the format."), name);
                    }
                }
            }
        }
    }
}

void PrintfChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string PrintfChecker::getName() const
{
    return "PrintfChecker";
}
}
