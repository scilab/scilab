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

#include "checkers/OldNotChecker.hxx"

namespace slint
{

void OldNotChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::NotExp & ne = static_cast<const ast::NotExp &>(e);
    std::pair<unsigned int, unsigned int> pos;
    if (context.getPosition(ne.getLocation(), pos))
    {
        if (pos.first < pos.second && *(context.getCode() + pos.first) == '@')
        {
            result.report(context, e.getLocation(), *this, _("Not operator \'~\' should be used rather than \'@\'."));
        }
    }
}

void OldNotChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string OldNotChecker::getName() const
{
    return "OldNotChecker";
}
}
