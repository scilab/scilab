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
#include "analyzers/SizeAnalyzer.hxx"
#include "call/SizeCall.hxx"
#include "tools.hxx"
#include "double.hxx"

namespace analysis
{
bool SizeAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 2)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    SizeCall::Kind kind = SizeCall::DUNNO;

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
        visitor.getDM().releaseTmp(res.getTempId(), first);
        return false;
    }

    switch (size)
    {
        case 1:
            if (lhs == 1)
            {
                kind = SizeCall::BOTH;
            }
            else if (lhs == 2)
            {
                kind = SizeCall::R_C;
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
                        kind = SizeCall::R;
                    }
                    else if (arg2 == L"c")
                    {
                        kind = SizeCall::C;
                    }
                    else if (arg2 == L"*")
                    {
                        kind = SizeCall::RC;
                    }
                    else
                    {
                        visitor.getDM().releaseTmp(res.getTempId(), first);
                        return false;
                    }
                }
                else if (second->isDoubleExp())
                {
                    const double arg2 = static_cast<ast::DoubleExp *>(second)->getValue();
                    if (arg2 == 1)
                    {
                        kind = SizeCall::R;
                    }
                    else if (arg2 == 2)
                    {
                        kind = SizeCall::C;
                    }
                    else if (arg2 >= 3)
                    {
                        // TODO: we should handle hypermatrix
                        kind = SizeCall::ONE;
                    }
                    else
                    {
                        visitor.getDM().releaseTmp(res.getTempId(), first);
                        return false;
                    }
                }
            }
            else
            {
                visitor.getDM().releaseTmp(res.getTempId(), first);
                return false;
            }
            break;
        }
        default:
            visitor.getDM().releaseTmp(res.getTempId(), first);
            return false;
    }

    TIType type(visitor.getGVN(), TIType::DOUBLE);

    switch (kind)
    {
        case SizeCall::R:
        {
            SymbolicDimension & rows = res.getType().rows;
            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant() = rows.getValue();
            e.getDecorator().setCall(new SizeCall(SizeCall::R));
            visitor.setResult(_res);
            break;
        }
        case SizeCall::C:
        {
            SymbolicDimension & cols = res.getType().cols;
            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant() = cols.getValue();
            e.getDecorator().setCall(new SizeCall(SizeCall::C));
            visitor.setResult(_res);
            break;
        }
        case SizeCall::RC:
        {
            SymbolicDimension & rows = res.getType().rows;
            SymbolicDimension & cols = res.getType().cols;
            SymbolicDimension prod = rows * cols;

            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant() = prod.getValue();
            e.getDecorator().setCall(new SizeCall(SizeCall::RC));
            visitor.setResult(_res);
            break;
        }
        case SizeCall::R_C:
        case SizeCall::BOTH:
        {
            if (kind == SizeCall::BOTH)
            {
                TIType _type(visitor.getGVN(), TIType::DOUBLE, 1, 2);
                Result & _res = e.getDecorator().setResult(_type);
            }

            SymbolicDimension & rows = res.getType().rows;
            SymbolicDimension & cols = res.getType().cols;
            std::vector<Result> & mlhs = visitor.getLHSContainer();
            mlhs.clear();
            mlhs.reserve(2);
            mlhs.emplace_back(type);
            mlhs.back().getConstant() = rows.getValue();
            mlhs.emplace_back(type);
            mlhs.back().getConstant() = cols.getValue();

            e.getDecorator().setCall(new SizeCall(kind));
            break;
        }
        case SizeCall::ONE:
        {
            Result & _res = e.getDecorator().setResult(type);
            _res.getConstant() = new types::Double(1);
            e.getDecorator().setCall(new SizeCall(SizeCall::ONE));
            visitor.setResult(_res);
            break;
        }
        default:
            return false;
    }

    return true;
}
}
