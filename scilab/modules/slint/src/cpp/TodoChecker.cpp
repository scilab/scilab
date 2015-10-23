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

#include "checkers/TodoChecker.hxx"

namespace slint
{

void TodoChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    const ast::CommentExp & ce = static_cast<const ast::CommentExp &>(e);
    if (todo.match(ce.getComment(), false))
    {
        result.report(context, e.getLocation(), *this, _("Comment contains a TODO."));
    }
}

void TodoChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string TodoChecker::getName() const
{
    return "TodoChecker";
}
}
