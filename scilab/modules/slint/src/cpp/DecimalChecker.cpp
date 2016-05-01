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

#include "checkers/DecimalChecker.hxx"

namespace slint
{

void DecimalChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::DoubleExp & de = static_cast<const ast::DoubleExp &>(e);
    std::pair<unsigned int, unsigned int> out;
    if (context.getPosition(de.getLocation(), out))
    {
        const wchar_t * const code = context.getCode();
        if (!character.empty())
        {
            for (const wchar_t * c = code + out.first; c < code + out.second; ++c)
            {
                if (*c == L'e' || *c == L'E' || *c == L'd' || *c == L'D')
                {
                    bool ok = false;
                    for (const wchar_t _c : character)
                    {
                        if (_c == *c)
                        {
                            ok = true;
                            break;
                        }
                    }
                    if (!ok)
                    {
                        result.report(context, e.getLocation(), *this, _("Bad decimal exponent: %s was expected and %s was found."), character, *c);
                    }
                }
            }
        }
        if (checkDot)
        {
            if (*(code + out.first) == L'.')
            {
                result.report(context, e.getLocation(), *this, _("Decimal numbers must not begin by a dot."));
            }
        }
    }
}

void DecimalChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string DecimalChecker::getName() const
{
    return "DecimalChecker";
}
}
