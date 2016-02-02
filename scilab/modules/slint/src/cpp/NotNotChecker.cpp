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

#include "checkers/NotNotChecker.hxx"

namespace slint
{

void NotNotChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::NotExp & ne = static_cast<const ast::NotExp &>(e);
    if (ne.getParent()->isNotExp())
    {
        result.report(context, e.getLocation(), *this, _("Double negation is not allowed."));
    }
}

void NotNotChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string NotNotChecker::getName() const
{
    return "NotNotChecker";
}
}
