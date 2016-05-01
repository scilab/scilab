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
#include <vector>

#include "checkers/LineLengthChecker.hxx"

namespace slint
{
void LineLengthChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

void LineLengthChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

bool LineLengthChecker::isFileChecker() const
{
    return true;
}

void LineLengthChecker::preCheckFile(SLintContext & context, SLintResult & result)
{
    if (max > 0)
    {
        std::vector<unsigned int> out;
        if (!context.checkLineLength((unsigned int)max, out))
        {
            std::wostringstream wos;
            for (std::vector<unsigned int>::const_iterator i = out.begin(), end = std::prev(out.end()); i != end; ++i)
            {
                wos << *i << L", ";
            }
            wos << *std::prev(out.end());
            result.report(context, Location(), *this, _("Maximum line length exceeded at lines: %s."), wos.str());
        }
    }
}

void LineLengthChecker::postCheckFile(SLintContext & context, SLintResult & result)
{

}

const std::string LineLengthChecker::getName() const
{
    return "LineLengthChecker";
}
}
