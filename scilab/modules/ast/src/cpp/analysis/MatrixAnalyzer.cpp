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

#include "AnalysisVisitor.hxx"
#include "analyzers/MatrixAnalyzer.hxx"
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
    const unsigned int size = args.size();
    if (size != 2 && size != 3)
    {
        return false;
    }

    if (size == 2)
    {
        return analyze2Args(visitor, args, e);
    }

    ast::Exp * first = args[0];
    ast::Exp * second = args[1];
    ast::Exp * third = args[2];

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

    if (R2.getConstant().getDblValue(val))
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
    else if (GVN::Value * gvnValue = R2.getConstant().getGVNValue())
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

    if (R3.getConstant().getDblValue(val))
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
    else if (GVN::Value * gvnValue = R3.getConstant().getGVNValue())
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
    int tempId;
    if (R1.getTempId() != -1)
    {
        tempId = R1.getTempId();
    }
    else
    {
        tempId = visitor.getDM().getTmpId(resT, false);
    }
    Result & _res = e.getDecorator().setResult(Result(resT, tempId));
    visitor.setResult(_res);
    return true;
}

bool MatrixAnalyzer::analyze2Args(AnalysisVisitor & visitor, const ast::exps_t & args, ast::CallExp & e)
{
    ast::Exp * first = args[0];
    ast::Exp * second = args[1];

    first->accept(visitor);
    Result R1 = visitor.getResult();
    if (!R1.getType().ismatrix())
    {
        return false;
    }

    SymbolicDimension rows;
    SymbolicDimension cols;
    bool hasRC = false;

    if (second->isCallExp())
    {
        ast::CallExp & ce = static_cast<ast::CallExp &>(*second);
        if (ce.getName().isSimpleVar())
        {
            ast::SimpleVar & var = static_cast<ast::SimpleVar &>(ce.getName());
            const symbol::Symbol & sym = var.getSymbol();
            const std::wstring & name = sym.getName();
            if (name == L"size" && visitor.getCM().checkGlobalConstant(sym))
            {
                const ast::exps_t size_args = ce.getArgs();
                if (size_args.size() == 1)
                {
                    ast::Exp * e = size_args[0];
                    e->accept(visitor);
                    Result & res = visitor.getResult();
                    if (res.getType().ismatrix())
                    {
                        rows = res.getType().rows;
                        cols = res.getType().cols;
                        hasRC = true;
                    }
                }
            }
        }
    }

    if (hasRC)
    {
        const TIType & type = R1.getType();
        SymbolicDimension prod1 = type.rows * type.cols;
        SymbolicDimension prod2 = rows * cols;
        bool res = visitor.getCM().check(ConstraintManager::EQUAL, prod1.getValue(), prod2.getValue());
        if (res)
        {
            TIType resT(visitor.getGVN(), R1.getType().type, rows, cols);
            int tempId;
            if (R1.getTempId() != -1)
            {
                tempId = R1.getTempId();
            }
            else
            {
                tempId = visitor.getDM().getTmpId(resT, false);
            }
            Result & _res = e.getDecorator().setResult(Result(resT, tempId));
            visitor.setResult(_res);

            return true;
        }
    }

    return false;
}
}
