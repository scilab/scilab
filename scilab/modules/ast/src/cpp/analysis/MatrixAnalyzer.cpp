/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AnalysisVisitor.hxx"
#include "calls/MatrixAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool MatrixAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 3)
    {
        return false;
    }

    ast::Exp * first = *args.begin();
    ast::Exp * second = *std::next(args.begin());
    ast::Exp * third = *std::next(std::next(args.begin()));

    first->accept(visitor);
    Result R1 = visitor.getResult();
    if (!R1.getType().ismatrix())
    {
        return false;
    }

    second->accept(visitor);
    Result R2 = visitor.getResult();
    third->accept(visitor);
    Result & R3 = visitor.getResult();

    double val;
    SymbolicDimension rows;
    SymbolicDimension cols;

    if (R2.getValue(val))
    {
        const int nrows = tools::cast<int>(val);
        if (nrows <= 0)
        {
            return false;
        }
        else
        {
            rows = SymbolicDimension(visitor.getGVN(), nrows);
        }
    }
    else if (GVN::Value * gvnValue = R2.getGVNValue())
    {
        if (gvnValue->poly->isConstant() && gvnValue->poly->constant <= 0)
        {
            return false;
        }
        rows.setValue(gvnValue);
        rows.setGVN(&visitor.getGVN());
    }
    else
    {
        return false;
    }

    if (R3.getValue(val))
    {
        const int ncols = tools::cast<int>(val);
        if (ncols <= 0)
        {
            return false;
        }
        else
        {
            cols = SymbolicDimension(visitor.getGVN(), ncols);
        }
    }
    else if (GVN::Value * gvnValue = R3.getGVNValue())
    {
        if (gvnValue->poly->isConstant() && gvnValue->poly->constant <= 0)
        {
            return false;
        }
        cols.setValue(gvnValue);
        cols.setGVN(&visitor.getGVN());
    }
    else
    {
        return false;
    }

    const TIType & type = R1.getType();
    SymbolicDimension prod1 = type.rows * type.cols;
    SymbolicDimension prod2 = rows * cols;
    bool res = visitor.getCM().check(ConstraintManager::EQUAL, prod1.getValue(), prod2.getValue());
    if (res)
    {
        res = visitor.getCM().check(ConstraintManager::POSITIVE, rows.getValue());
        if (!res)
        {
            return false;
        }
    }
    else
    {
        return false;
    }

    TIType resT(visitor.getGVN(), R1.getType().type, rows, cols);
    e.getDecorator().res = Result(resT, visitor.getTemp().add(resT));
    visitor.setResult(e.getDecorator().res);

    return true;
}
}
