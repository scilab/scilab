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

void AnalysisVisitor::visit(ast::AssignExp & e)
{
    logger.log(L"AssignExp", e.getLocation());
    if (e.getLeftExp().isSimpleVar()) // A = ...
    {
        ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getLeftExp());
        const symbol::Symbol & sym = var.getSymbol();

        if (e.getRightExp().isSimpleVar())
        {
            // We have A = B (so the data associated to b is shared with a)
            const symbol::Symbol & symR = static_cast<ast::SimpleVar &>(e.getRightExp()).getSymbol();
            Info & info = getSymInfo(symR);
            const TIType & Rtype = info.getType();
            Result & resL = e.getLeftExp().getDecorator().setResult(Rtype);
            resL.setConstant(info.getConstant());
            resL.setRange(info.getRange());
            Result & resR = e.getRightExp().getDecorator().setResult(Rtype);
            resR.setConstant(info.getConstant());
            resR.setRange(info.getRange());
            getDM().share(sym, symR, Rtype, resR.isAnInt(), &e);
        }
        else
        {
            // apply the ConstantVisitor
            cv.setLHS(1);
            e.getRightExp().accept(cv);

            if (e.getRightExp().isCallExp()) // A = foo(...)
            {
                if (e.getRightExp().isCallExp())
                {
                    visit(static_cast<ast::CallExp &>(e.getRightExp()), /* LHS */ 1);
                }
                else
                {
                    e.getRightExp().accept(*this);
                }
            }
            else // A = 1 + 2
            {
                e.getRightExp().accept(*this);
            }

            Result & RR = getResult();
            var.getDecorator().res = RR;
            Info & info = getDM().define(sym, RR.getType(), RR.isAnInt(), &e);
            info.getConstant() = RR.getConstant();
            e.getDecorator().safe = true;

            // Don't remove temp: because the value is transfered to LHS
            getDM().releaseTmp(RR.getTempId(), nullptr);//&e.getRightExp());
        }
    }
    else if (e.getLeftExp().isCallExp()) // A(12) = ...
    {
        // apply the ConstantVisitor
        cv.setLHS(1);
        e.getRightExp().accept(cv);

        ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
        if (ce.getName().isSimpleVar())
        {
            const symbol::Symbol & symL = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
            e.getRightExp().accept(*this);
            Result & RR = e.getRightExp().getDecorator().getResult();
            ce.getDecorator().res = RR;
            Info & info = getDM().write(symL, RR.getType(), &ce.getName());
            ce.getName().getDecorator().setResult(info.type);
            if (analyzeIndices(info.type, ce))
            {
                e.getDecorator().safe = (RR.getType() == getResult().getType());
            }
            getDM().releaseTmp(RR.getTempId(), &e.getRightExp());
        }
    }
    else if (e.getLeftExp().isAssignListExp()) // [A, B] = ...
    {
        ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(e.getLeftExp());
        if (e.getRightExp().isCallExp())
        {
            const ast::exps_t & exps = ale.getExps();
            visit(static_cast<ast::CallExp &>(e.getRightExp()), /* LHS */ exps.size());
            std::vector<Result>::iterator j = multipleLHS.begin();
            for (const auto exp : exps)
            {
                // TODO: handle fields...
                if (exp->isSimpleVar() && j != multipleLHS.end())
                {
                    ast::SimpleVar & var = *static_cast<ast::SimpleVar *>(exp);
                    const symbol::Symbol & sym = var.getSymbol();
                    Info & info = getDM().define(sym, j->getType(), j->isAnInt(), exp);
                    info.setConstant(j->getConstant());
                    var.getDecorator().res = *j;
                    ++j;
                }
            }
        }
    }
}
}
