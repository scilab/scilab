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
