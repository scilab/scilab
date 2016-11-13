/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#include <cmath>

#include "AnalysisVisitor.hxx"
#include "analyzers/DiagAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool DiagAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    const std::size_t size = args.size();
    if (size != 1 && size != 2)
    {
        return false;
    }

    ast::Exp * first = *args.begin();
    ast::Exp * second = size == 2 ? *std::next(args.begin()) : nullptr;
    int index = 0;

    first->accept(visitor);
    Result R1 = visitor.getResult();
    if (!R1.getType().ismatrix())
    {
        return false;
    }

    if (second)
    {
        second->accept(visitor);
        Result & R2 = visitor.getResult();
        double val;
        if (R2.getConstant().getDblValue(val))
        {
            index = tools::cast<int>(val);
        }
        else
        {
            return false;
        }
    }

    bool isOneDim = false;
    GVN::Value * dim;
    TIType & type = R1.getType();
    GVN::Value * ONE = visitor.getGVN().getValue(int64_t(1));
    isOneDim = visitor.getCM().check(ConstraintManager::EQUAL, type.rows.getValue(), ONE);
    if (isOneDim)
    {
        dim = type.cols.getValue();
    }
    else
    {
        isOneDim = visitor.getCM().check(ConstraintManager::EQUAL, type.cols.getValue(), ONE);
        if (isOneDim)
        {
            dim = type.rows.getValue();
        }
    }

    if (isOneDim)
    {
        SymbolicDimension sdim = index == 0 ? SymbolicDimension(&visitor.getGVN(), dim) : SymbolicDimension(&visitor.getGVN(), visitor.getGVN().getValue(OpValue::Kind::PLUS, *dim, *visitor.getGVN().getValue(int64_t(std::abs(index)))));
        TIType resT(visitor.getGVN(), R1.getType().type, sdim, sdim);
        e.getDecorator().setResult(Result(resT, visitor.getDM().getTmpId(resT, false)));
    }
    else
    {
        bool res = visitor.getCM().check(ConstraintManager::EQUAL, type.rows.getValue(), type.cols.getValue());
        if (res)
        {
            SymbolicDimension dimONE(visitor.getGVN(), 1.);
            TIType resT(visitor.getGVN(), R1.getType().type, type.rows, dimONE);
            e.getDecorator().setResult(Result(resT, visitor.getDM().getTmpId(resT, false)));
        }
        else
        {
            res = visitor.getCM().check(ConstraintManager::STRICT_GREATER, type.rows.getValue(), type.cols.getValue());
            if (res)
            {
                if (index == 0)
                {
                    SymbolicDimension dimONE(visitor.getGVN(), 1);
                    TIType resT(visitor.getGVN(), R1.getType().type, type.cols, dimONE);
                    e.getDecorator().setResult(Result(resT, visitor.getDM().getTmpId(resT, false)));
                }
                else
                {
                    return false;
                }
            }
            else
            {
                res = visitor.getCM().check(ConstraintManager::STRICT_GREATER, type.cols.getValue(), type.rows.getValue());
                if (res)
                {
                    if (index == 0)
                    {
                        SymbolicDimension dimONE(visitor.getGVN(), 1);
                        TIType resT(visitor.getGVN(), R1.getType().type, type.rows, dimONE);
                        e.getDecorator().setResult(Result(resT, visitor.getDM().getTmpId(resT, false)));
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
    }

    visitor.setResult(e.getDecorator().res);

    return true;
}
}
