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

#include "checkers/LinesCountChecker.hxx"

namespace slint
{
void LinesCountChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (max > 0)
    {
        if (context.isFirstLevelFn())
        {
            const unsigned int count = context.countLines(e.getLocation().first_line, e.getLocation().last_line);
            if (count > (unsigned int)max)
            {
                result.report(context, e.getLocation(), *this, _("Maximum lines number exceeded: %d > %d."), count, max);
            }
        }
    }
}

void LinesCountChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string LinesCountChecker::getName() const
{
    return "LinesCountChecker";
}
}
