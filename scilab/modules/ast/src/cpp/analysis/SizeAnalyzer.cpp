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
#include "calls/SizeAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool SizeAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 2)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    enum Kind
    {
        ROWS, COLS, ROWSTIMESCOLS, ROWSCOLS, ONE
    } kind;
    const std::size_t size = args.size();
    if (size == 0 || size >= 3)
    {
        return false;
    }

    ast::Exp * first = *args.begin();
    if (!first)
    {
        return false;
    }
    first->accept(visitor);
    Result & res = visitor.getResult();
    if (!res.getType().ismatrix())
    {
        return false;
    }

    switch (size)
    {
        case 1:
            if (lhs == 1)
            {
                kind = ROWS;
            }
            else if (lhs == 2)
            {
                kind = ROWSCOLS;
            }
            break;
        case 2:
        {
            ast::Exp * second = *std::next(args.begin());
            if (second && lhs == 1)
            {
                if (second->isStringExp())
                {
                    const std::wstring & arg2 = static_cast<ast::StringExp *>(second)->getValue();
                    if (arg2 == L"r")
                    {
                        kind = ROWS;
                    }
                    else if (arg2 == L"c")
                    {
                        kind = COLS;
                    }
                    else if (arg2 == L"*")
                    {
                        kind = ROWSTIMESCOLS;
                    }
                    else
                    {
                        return false;
                    }
                }
                else if (second->isDoubleExp())
                {
                    const double arg2 = static_cast<ast::DoubleExp *>(second)->getValue();
                    if (arg2 == 1)
                    {
                        kind = ROWS;
                    }
                    else if (arg2 == 2)
                    {
                        kind = COLS;
                    }
                    else if (arg2 >= 3)
                    {
                        kind = ONE;
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
            break;
        }
        default:
            return false;
    }

    TIType type(visitor.getGVN(), TIType::DOUBLEUINT);

    switch (kind)
    {
        case ROWS:
        {
            SymbolicDimension & rows = res.getType().rows;
            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant().set(rows.getValue());
            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"size"));
            visitor.setResult(_res);
            break;
        }
        case COLS:
        {
            SymbolicDimension & cols = res.getType().cols;
            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant().set(cols.getValue());
            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"size"));
            visitor.setResult(_res);
            break;
        }
        case ROWSTIMESCOLS:
        {
            SymbolicDimension & rows = res.getType().rows;
            SymbolicDimension & cols = res.getType().cols;
            SymbolicDimension prod = rows * cols;
            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant().set(prod.getValue());
            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"size"));
            visitor.setResult(_res);
            break;
        }
        case ROWSCOLS:
        {
            SymbolicDimension & rows = res.getType().rows;
            SymbolicDimension & cols = res.getType().cols;
            std::vector<Result> & mlhs = visitor.getLHSContainer();
            mlhs.clear();
            mlhs.reserve(2);
            mlhs.emplace_back(type);
            mlhs.back().getConstant().set(rows.getValue());
            mlhs.emplace_back(type);
            mlhs.back().getConstant().set(cols.getValue());

            e.getDecorator().setCall(Call(Call::IDENTITY, type, L"size"));
            break;
        }
    }

    return true;
}
}
