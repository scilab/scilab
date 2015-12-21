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
    const double ratio = ((double)count) / ((double)lines);
    if (ratio < ratioMin)
    {
        result.report(context, Location(), *this, _("The minimal ratio between commented lines and total lines is not reached: %d < %d."), ratio, ratioMin);
    }
}
}
