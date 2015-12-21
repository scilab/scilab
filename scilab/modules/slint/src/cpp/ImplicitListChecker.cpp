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

#include "core_math.h"

#include "checkers/ImplicitListChecker.hxx"

namespace slint
{

void ImplicitListChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::ListExp & le = static_cast<const ast::ListExp &>(e);
    if (le.getStart().isDoubleExp() && le.getStep().isDoubleExp() && le.getEnd().isDoubleExp())
    {
        const double start = static_cast<const ast::DoubleExp &>(le.getStart()).getValue();
        const double step = static_cast<const ast::DoubleExp &>(le.getStep()).getValue();
        const double end = static_cast<const ast::DoubleExp &>(le.getEnd()).getValue();
        if (ISNAN(start) || ISNAN(step) || ISNAN(end) || !finite(start) || !finite(step) || !finite(end))
        {
            result.report(context, e.getLocation(), *this, _("Invalid list, it contains NaN or Inf."));
        }
        else
        {
            if (start == end)
            {
                result.report(context, e.getLocation(), *this, _("List has the same start and end."));
            }
            if (step == 0)
            {
                result.report(context, e.getLocation(), *this, _("List has a null step."));
            }
            if ((start > end && step > 0) || (start < end && step < 0))
            {
                result.report(context, e.getLocation(), *this, _("List is empty."));
            }
        }
    }
    else if (le.getStart().isListExp() || le.getStep().isListExp() || le.getEnd().isListExp())
    {
        result.report(context, e.getLocation(), *this, _("Bad use of ':' operator."));
    }
}

void ImplicitListChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string ImplicitListChecker::getName() const
{
    return "ImplicitListChecker";
}
}
