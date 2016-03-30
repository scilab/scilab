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

#include "AnalysisVisitor.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::SeqExp & e)
{
    logger.log(L"SeqExp", e.getLocation());
    ast::exps_t::iterator i = e.getExps().begin();
    ast::exps_t::iterator itEnd = e.getExps().end();
    for (; i != itEnd; ++i)
    {
        ast::Exp* exp = *i;
        if (exp->isCallExp())
        {
            visit(*static_cast<ast::CallExp *>(exp), /* LHS */ 0);
        }
        else if (exp->isBreakExp() || exp->isContinueExp())
        {
            exp->accept(*this);
            if (loops.empty())
            {
                // We are not in a loop so this break is useless.
                exp->replace(new ast::CommentExp(exp->getLocation(), new std::wstring(L"useless break or continue")));
            }
            else
            {
                // We are in a loop: all the code after the break in this SeqExp is useless
                break;
            }
        }
        else
        {
            exp->accept(*this);
        }
    }

    if (i != itEnd)
    {
        ++i;
        if (i != itEnd)
        {
            e.getExps().erase(i, itEnd);
        }
    }
}

} // namespace analysis
