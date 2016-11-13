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

#include "checkers/UselessOpChecker.hxx"

namespace slint
{

void UselessOpChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    if (oe.getParent()->isSeqExp())
    {
        result.report(context, e.getLocation(), *this, _("Possible useless operation."));
    }
}

void UselessOpChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string UselessOpChecker::getName() const
{
    return "UselessOpChecker";
}
}
