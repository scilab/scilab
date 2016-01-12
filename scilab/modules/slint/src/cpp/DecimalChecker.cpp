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

#include "checkers/DecimalChecker.hxx"

namespace slint
{

void DecimalChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::DoubleExp & de = static_cast<const ast::DoubleExp &>(e);
    std::pair<unsigned int, unsigned int> out;
    if (context.getPosition(de.getLocation(), out))
    {
        const char * const code = context.getCode();
        if (character != '\0')
        {
            for (const char * c = code + out.first; c < code + out.second; ++c)
            {
                if ((*c == 'e' || *c == 'E' || *c == 'd' || *c == 'D') && *c != character)
                {
                    result.report(context, e.getLocation(), *this, _("Bad decimal exponent: %s was expected and %s was found."), character, *c);
                }
            }
        }
        if (checkDot)
        {
            if (*(code + out.first) == '.')
            {
                result.report(context, e.getLocation(), *this, _("Decimal numbers musn\'t begin by a dot."));
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
