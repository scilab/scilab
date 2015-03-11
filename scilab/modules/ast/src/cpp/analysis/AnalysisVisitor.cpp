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
#include "calls/ArgnAnalyzer.hxx"
#include "calls/CeilAnalyzer.hxx"
#include "calls/DiagAnalyzer.hxx"
#include "calls/LengthAnalyzer.hxx"
#include "calls/MatrixAnalyzer.hxx"
#include "calls/MemInitAnalyzer.hxx"
#include "calls/SizeAnalyzer.hxx"
#include "symbol.hxx"
//#include "calls/SqrtAnalyzer.hxx"

namespace analysis
{
AnalysisVisitor::MapSymCall AnalysisVisitor::symscall = AnalysisVisitor::initCalls();

AnalysisVisitor::MapSymCall AnalysisVisitor::initCalls()
{
    MapSymCall msc;

    msc.emplace(L"zeros", std::shared_ptr<CallAnalyzer>(new ZerosAnalyzer()));
    msc.emplace(L"ones", std::shared_ptr<CallAnalyzer>(new OnesAnalyzer()));
    msc.emplace(L"rand", std::shared_ptr<CallAnalyzer>(new RandAnalyzer()));
    msc.emplace(L"matrix", std::shared_ptr<CallAnalyzer>(new MatrixAnalyzer()));
    msc.emplace(L"eye", std::shared_ptr<CallAnalyzer>(new EyeAnalyzer()));

    std::shared_ptr<CallAnalyzer> ca(new CeilAnalyzer());
    msc.emplace(L"ceil", ca);
    msc.emplace(L"floor", ca);
    msc.emplace(L"round", ca);
    msc.emplace(L"fix", ca);
    msc.emplace(L"int", ca);

    //msc.emplace(L"sqrt", std::shared_ptr<CallAnalyzer>(new SqrtAnalyzer()));
    msc.emplace(L"argn", std::shared_ptr<CallAnalyzer>(new ArgnAnalyzer()));
    msc.emplace(L"size", std::shared_ptr<CallAnalyzer>(new SizeAnalyzer()));
    msc.emplace(L"length", std::shared_ptr<CallAnalyzer>(new LengthAnalyzer()));
    msc.emplace(L"diag", std::shared_ptr<CallAnalyzer>(new DiagAnalyzer()));

    return msc;
}

bool AnalysisVisitor::asDouble(ast::Exp & e, double & out)
{
    if (e.isDoubleExp())
    {
        out = static_cast<ast::DoubleExp &>(e).getValue();
        return true;
    }
    else if (e.isOpExp())
    {
        ast::OpExp & op = static_cast<ast::OpExp &>(e);
        if (op.getOper() == ast::OpExp::unaryMinus)
        {
            if (op.getRight().isDoubleExp())
            {
                out = -static_cast<ast::DoubleExp &>(op.getRight()).getValue();
                return true;
            }
        }
        else if (op.getLeft().isDoubleExp() && op.getRight().isDoubleExp())
        {
            const double L = static_cast<ast::DoubleExp &>(op.getLeft()).getValue();
            const double R = static_cast<ast::DoubleExp &>(op.getRight()).getValue();

            switch (op.getOper())
            {
                case ast::OpExp::minus:
                    out = L - R;
                    return true;
                case ast::OpExp::plus:
                    out = L + R;
                    return true;
                case ast::OpExp::times:
                case ast::OpExp::dottimes:
                case ast::OpExp::krontimes:
                    out = L * R;
                    return true;
                case ast::OpExp::rdivide:
                case ast::OpExp::dotrdivide:
                case ast::OpExp::kronrdivide:
                    out = L / R;
                    return true;
                case ast::OpExp::ldivide:
                case ast::OpExp::dotldivide:
                case ast::OpExp::kronldivide:
                    out = R / L;
                    return true;
                case ast::OpExp::power:
                case ast::OpExp::dotpower:
                    out = std::pow(L, R);
                    return true;
                default:
                    return false;
            }
        }
    }

    return false;
}

bool AnalysisVisitor::isDoubleConstant(const ast::Exp & e)
{
    if (e.isDoubleExp())
    {
        return true;
    }
    else if (e.isOpExp())
    {
        const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
        if (!oe.isBooleanOp())
        {
            return isDoubleConstant(oe.getLeft()) && isDoubleConstant(oe.getRight());
        }
        return false;
    }
    else if (e.isMatrixExp())
    {
        const ast::MatrixExp & me = static_cast<const ast::MatrixExp &>(e);
        const ast::exps_t & lines = me.getLines();
        for (const auto line : lines)
        {
            const ast::exps_t & columns = static_cast<ast::MatrixLineExp *>(line)->getColumns();
            for (const auto column : columns)
            {
                if (column && !isDoubleConstant(*column))
                {
                    return false;
                }
            }
        }
        return true;
    }
    else if (e.isListExp())
    {
        const ast::ListExp & le = static_cast<const ast::ListExp &>(e);
        return isDoubleConstant(le.getStart()) && isDoubleConstant(le.getStep()) && isDoubleConstant(le.getEnd());
    }
    else if (e.isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e);
        const symbol::Symbol & sym = var.getSymbol();
        const std::wstring & name = sym.getName();
        return name == L"%i" || name == L"%inf" || name == L"%nan" || name == L"%eps" || name == L"%pi" || name == L"%e";
    }
    else if (e.isCallExp())
    {
        const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(ce.getName());
        const std::wstring & name = var.getSymbol().getName();

        // TODO: check if 'ones' and 'zeros' are the expected functions
        // ie: ones="abc"; ones(1) !!!
        if (name == L"ones" || name == L"zeros")
        {
            const ast::exps_t args = ce.getArgs();
            switch (args.size())
            {
                case 0:
                    return true;
                case 1:
                    return isDoubleConstant(*args.front());
                case 2:
                    return isDoubleConstant(*args.front()) && isDoubleConstant(**std::next(args.cbegin()));
                default:
                    return false;
            }
        }
    }

    return false;
}

bool AnalysisVisitor::asDoubleMatrix(ast::Exp & e, types::Double *& data)
{
    if (isDoubleConstant(e))
    {
        ast::ExecVisitor exec;
        e.accept(exec);
        types::InternalType * pIT = exec.getResult();
        // TODO : handle complex case
        if (pIT && pIT->isDouble() && !pIT->getAs<types::Double>()->isComplex())
        {
            pIT->IncreaseRef();
            data = static_cast<types::Double *>(pIT);

            return true;
        }
    }

    return false;
}
}
