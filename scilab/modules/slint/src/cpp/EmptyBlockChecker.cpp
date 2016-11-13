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

#include "checkers/EmptyBlockChecker.hxx"

namespace slint
{
void EmptyBlockChecker::preCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
    if (e.isSeqExp())
    {
        const ast::exps_t & exps = static_cast<const ast::SeqExp &>(e).getExps();
        bool empty = exps.empty();
        if (!empty)
        {
            empty = true;
            for (const auto exp : exps)
            {
                if (!exp->isCommentExp())
                {
                    empty = false;
                    break;
                }
            }
        }

        if (empty)
        {
            result.report(context, e.getLocation(), *this, _("Empty block."));
        }
    }
}

void EmptyBlockChecker::postCheckNode(const ast::Exp & e, SLintContext & context, SLintResult & result)
{
}

const std::string EmptyBlockChecker::getName() const
{
    return "EmptyBlockChecker";
}
}
