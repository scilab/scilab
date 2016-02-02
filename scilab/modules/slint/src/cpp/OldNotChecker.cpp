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
