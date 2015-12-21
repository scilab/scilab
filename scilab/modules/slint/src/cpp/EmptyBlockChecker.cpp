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
