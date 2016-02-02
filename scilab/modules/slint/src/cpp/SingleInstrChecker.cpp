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
