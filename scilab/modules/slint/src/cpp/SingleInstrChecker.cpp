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

#include "checkers/SingleInstrChecker.hxx"

namespace slint
{

void SingleInstrChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isSeqExp())
    {
        const ast::exps_t & exps = static_cast<const ast::SeqExp &>(e).getExps();
        if (!exps.empty())
        {
            int prevline = exps.front()->getLocation().last_line;
            for (ast::exps_t::const_iterator i = std::next(exps.begin()), end = exps.end(); i != end; ++i)
            {
                if (!(*i)->isCommentExp() && (*i)->getLocation().first_line == prevline)
                {
                    result.report(context, (*i)->getLocation(), *this, _("Two instructions on the same line."));
                }
                prevline = (*i)->getLocation().last_line;
            }
        }
    }
}

void SingleInstrChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string SingleInstrChecker::getName() const
{
    return "SingleInstrChecker";
}
}
