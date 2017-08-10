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

#include "checkers/CommentRatioChecker.hxx"

namespace slint
{

void CommentRatioChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CommentExp & ce = static_cast<const ast::CommentExp &>(e);
    const Location & loc = ce.getLocation();
    count += loc.first_line - loc.last_line + 1;
}

void CommentRatioChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{

}

const std::string CommentRatioChecker::getName() const
{
    return "CommentRatioChecker";
}

bool CommentRatioChecker::isFileChecker() const
{
    return true;
}

void CommentRatioChecker::preCheckFile(SLintContext & context, SLintResult & result)
{
    count = 0;
}

void CommentRatioChecker::postCheckFile(SLintContext & context, SLintResult & result)
{
    const uint64_t lines = context.getSciFile()->countLines();
    double ratio;
    if (lines)
    {
        ratio = ((double)count) / ((double)lines);
    }
    else
    {
        ratio = 0;
    }
    if (ratio < ratioMin)
    {
        result.report(context, Location(), *this, _("The minimal ratio between commented lines and total lines is not reached: %d < %d."), ratio, ratioMin);
    }
}
}
