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

#include "AnalysisVisitor.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::SeqExp & e)
{
    logger.log("SeqExp", e.getLocation());
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
                exp->replace(new ast::CommentExp(exp->getLocation(), new std::string("useless break or continue")));
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
