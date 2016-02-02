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

void AnalysisVisitor::visit(ast::IfExp & e)
{
    logger.log(L"IfExp", e.getLocation());
    ast::Exp * shortcutExp = nullptr;

    // we apply the ConstantVisitor
    e.getTest().accept(cv);
    ast::Exp & test = e.getTest();

    if (cv.getResult() && test.isConstExp())
    {
        e.accept(cv);
        // we have a constant !
        ast::ConstExp & ce = static_cast<ast::ConstExp &>(test);
        types::InternalType * pIT = ce.getConstant();
        if (!pIT)
        {
            test.accept(cv.getExec());
            pIT = ce.getConstant();
        }
        if (pIT)
        {
            const bool result = pIT->isTrue();
            if (result)
            {
                shortcutExp = &e.getThen();
                e.getExps()[1] = nullptr;
            }
            else
            {
                shortcutExp = &e.getElse();
                e.getExps()[2] = nullptr;
            }
        }
    }
    else
    {
        ast::Exp * val = nullptr;
        bool isEq;
        if (test.isOpExp())
        {
            ast::OpExp & oe = static_cast<ast::OpExp &>(test);
            isEq = oe.getOper() == ast::OpExp::eq;
            if (isEq || oe.getOper() == ast::OpExp::ne)
            {
                if (oe.getLeft().isDoubleExp())
                {
                    ast::DoubleExp & de = static_cast<ast::DoubleExp &>(oe.getLeft());
                    if (types::InternalType * pIT = de.getConstant())
                    {
                        if (pIT->isDouble())
                        {
                            types::Double * pDbl = static_cast<types::Double *>(pIT);
                            if (pDbl->isEmpty())
                            {
                                val = &oe.getRight();
                            }
                        }
                    }
                }
                else if (oe.getRight().isDoubleExp())
                {
                    ast::DoubleExp & de = static_cast<ast::DoubleExp &>(oe.getRight());
                    if (types::InternalType * pIT = de.getConstant())
                    {
                        if (pIT->isDouble())
                        {
                            types::Double * pDbl = static_cast<types::Double *>(pIT);
                            if (pDbl->isEmpty())
                            {
                                val = &oe.getLeft();
                            }
                        }
                    }
                }
            }
        }
        else
        {
            ast::CallExp * ce = nullptr;
            if (test.isNotExp())
            {
                ast::NotExp & ne = static_cast<ast::NotExp &>(test);
                if (ne.getExp().isCallExp())
                {
                    ce = &static_cast<ast::CallExp &>(ne.getExp());
                    isEq = false;
                }
            }
            else if (test.isCallExp())
            {
                ce = &static_cast<ast::CallExp &>(test);
                isEq = true;
            }

            if (ce && ce->getName().isSimpleVar())
            {
                ast::SimpleVar & var = static_cast<ast::SimpleVar &>(ce->getName());
                const symbol::Symbol & sym = var.getSymbol();
                const std::wstring & name = sym.getName();
                if (name == L"isempty" && getCM().checkGlobalConstant(sym))
                {
                    const ast::exps_t args = ce->getArgs();
                    if (args.size() == 1)
                    {
                        val = args.front();
                    }
                }
            }
        }

        if (val)
        {
            // test is something like val == [], isempty(val), val ~= [], ~isempty(val)
            val->accept(*this);
            Result & res = getResult();
            TIType & ty = res.getType();
            if ((ty.ismatrix() && ty.isscalar()) || (getCM().check(ConstraintManager::STRICT_POSITIVE, ty.rows.getValue()) || getCM().check(ConstraintManager::STRICT_POSITIVE, ty.cols.getValue())))
            {
                if (isEq)
                {
                    shortcutExp = &e.getElse();
                    e.getExps()[2] = nullptr;
                }
                else
                {
                    shortcutExp = &e.getThen();
                    e.getExps()[1] = nullptr;
                }
            }
        }
    }

    if (shortcutExp)
    {
        e.replace(shortcutExp);
        shortcutExp->accept(*this);
    }
    else
    {
        dm.addBlock(Block::EXCLUSIVE, &e);
        e.getTest().accept(*this);
        dm.releaseTmp(getResult().getTempId(), &e.getTest());
        dm.addBlock(Block::NORMAL, &e.getThen());
        e.getThen().accept(*this);
        dm.finalizeBlock();
        dm.addBlock(Block::NORMAL, e.hasElse() ? &e.getElse() : nullptr);
        if (e.hasElse())
        {
            e.getElse().accept(*this);
        }
        dm.finalizeBlock();
        dm.finalizeBlock();
    }
}
}
