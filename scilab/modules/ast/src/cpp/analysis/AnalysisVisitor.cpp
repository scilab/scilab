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

#include "symbol.hxx"

#include "AnalysisVisitor.hxx"
#include "analyzers/ArgnAnalyzer.hxx"
#include "analyzers/CeilAnalyzer.hxx"
#include "analyzers/DiagAnalyzer.hxx"
#include "analyzers/LengthAnalyzer.hxx"
#include "analyzers/MatrixAnalyzer.hxx"
#include "analyzers/MemInitAnalyzer.hxx"
#include "analyzers/SizeAnalyzer.hxx"
#include "analyzers/TypeAnalyzer.hxx"
#include "analyzers/TypeofAnalyzer.hxx"
#include "analyzers/InttypeAnalyzer.hxx"
#include "analyzers/IconvertAnalyzer.hxx"
#include "analyzers/IsrealAnalyzer.hxx"
#include "analyzers/IsscalarAnalyzer.hxx"
#include "analyzers/FindAnalyzer.hxx"

//#include "analyzers/SqrtAnalyzer.hxx"

namespace analysis
{
AnalysisVisitor::MapSymCall AnalysisVisitor::symscall = AnalysisVisitor::initCalls();//a=1:3;b=2;c=3;testAnalysis("repmat","a","b","c")

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
    msc.emplace(L"type", std::shared_ptr<CallAnalyzer>(new TypeAnalyzer()));
    msc.emplace(L"typeof", std::shared_ptr<CallAnalyzer>(new TypeofAnalyzer()));
    msc.emplace(L"inttype", std::shared_ptr<CallAnalyzer>(new InttypeAnalyzer()));
    msc.emplace(L"iconvert", std::shared_ptr<CallAnalyzer>(new IconvertAnalyzer()));
    msc.emplace(L"isreal", std::shared_ptr<CallAnalyzer>(new IsrealAnalyzer()));
    msc.emplace(L"isscalar", std::shared_ptr<CallAnalyzer>(new IsscalarAnalyzer()));
    msc.emplace(L"find", std::shared_ptr<CallAnalyzer>(new FindAnalyzer()));

    return msc;
}


AnalysisVisitor::AnalysisVisitor() : cv(*this), pv(std::wcerr, true, false), logger("/tmp/analysis.log")
{
    start_chrono();
}

AnalysisVisitor::~AnalysisVisitor() { }

void AnalysisVisitor::reset()
{
    _result = Result();
    dm.reset();
    multipleLHS.clear();
    while (!loops.empty())
    {
        loops.pop();
    }
    start_chrono();
}

void AnalysisVisitor::print_info()
{
    stop_chrono();

    //std::wcout << getGVN() << std::endl << std::endl; function z=foo(x,y);z=argn(2);endfunction;jit("x=123;y=456;t=foo(x,y)")
    std::wcerr << L"Analysis: " << *static_cast<Chrono *>(this) << std::endl;
    //std::wcout << temp << std::endl;

    std::wcerr << dm << std::endl;
    std::wcerr << pmc << std::endl;

    std::wcerr << std::endl;
}

logging::Logger & AnalysisVisitor::getLogger()
{
    return logger;
}

bool AnalysisVisitor::asDouble(types::InternalType * pIT, double & out)
{
    if (pIT && pIT->isDouble())
    {
        types::Double * pDbl = static_cast<types::Double *>(pIT);
        if (!pDbl->isComplex() && pDbl->getSize() == 1)
        {
            out = pDbl->get()[0];
            return true;
        }
    }

    return false;
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

void AnalysisVisitor::visitArguments(const std::wstring & name, const unsigned int lhs, const TIType & calltype, ast::CallExp & e, const ast::exps_t & args)
{
    std::vector<Result> resargs;
    std::vector<TIType> vargs;
    vargs.reserve(args.size());
    resargs.reserve(args.size());

    const ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getName());
    const symbol::Symbol & sym = var.getSymbol();
    argIndices.emplace(var, args.size(), 0);

    for (auto arg : args)
    {
        argIndices.top().getIndex() += 1;
        arg->accept(*this);
        resargs.push_back(getResult());
        vargs.push_back(getResult().getType());
    }

    argIndices.pop();
    uint64_t functionId = 0;
    std::vector<TIType> out = getDM().call(*this, lhs, sym, vargs, &e, functionId);
    if (lhs > 1)
    {
        multipleLHS.clear();
        multipleLHS.reserve(out.size());
        for (const auto & type : out)
        {
            const int tempId = getDM().getTmpId(type, false);
            multipleLHS.emplace_back(type, tempId);
        }

        auto i = args.begin();
        for (const auto & resarg : resargs)
        {
            getDM().releaseTmp(resarg.getTempId(), *i);
            ++i;
        }
    }
    else if (lhs == 1)
    {
        int tempId = -1;
        if (resargs.size() == 1)
        {
            const int id = resargs.back().getTempId();
            if (id != -1 && Checkers::isElementWise(name) && out[0] == resargs.back().getType())
            {
                tempId = id;
            }
        }
        if (tempId == -1)
        {
            tempId = getDM().getTmpId(out[0], false);
            auto i = args.begin();
            for (const auto & resarg : resargs)
            {
                getDM().releaseTmp(resarg.getTempId(), *i);
                ++i;
            }
        }

        e.getDecorator().res = Result(out[0], tempId, functionId);
        e.getDecorator().setCall(name, vargs);
        setResult(e.getDecorator().res);
    }
}

int AnalysisVisitor::getTmpIdForEWOp(const TIType & resT, const Result & LR, const Result & RR, ast::Exp * Lexp, ast::Exp * Rexp)
{
    int tempId = -1;
    if (resT.isknown() && resT.ismatrix())
    {
        if (LR.isTemp() || RR.isTemp())
        {
            const int Lid = LR.getTempId();
            const int Rid = RR.getTempId();
            const TIType & LT = LR.getType();
            const TIType & RT = RR.getType();

            if (LT.isscalar())
            {
                if (RT.isscalar())
                {
                    if (Lid == -1)
                    {
                        if (resT == LT)
                        {
                            tempId = Rid;
                        }
                        else
                        {
                            tempId = getDM().getTmpId(resT, false);
                            getDM().releaseTmp(Rid, Rexp);
                        }
                    }
                    else
                    {
                        if (resT == LT)
                        {
                            tempId = Lid;
                            getDM().releaseTmp(Rid, Rexp);
                        }
                        else if (Rid != -1 && resT == RT)
                        {
                            tempId = Rid;
                            getDM().releaseTmp(Lid, Lexp);
                        }
                        else
                        {
                            tempId = getDM().getTmpId(resT, false);
                            getDM().releaseTmp(Lid, Lexp);
                        }
                    }
                }
                else
                {
                    if (Rid == -1)
                    {
                        tempId = getDM().getTmpId(resT, false);
                    }
                    else
                    {
                        if (resT == RT)
                        {
                            tempId = Rid;
                        }
                        else if (Lid != -1 && resT == LT)
                        {
                            tempId = Lid;
                            getDM().releaseTmp(Rid, Rexp);
                        }
                        else
                        {
                            tempId = getDM().getTmpId(resT, false);
                            getDM().releaseTmp(Rid, Rexp);
                        }
                    }
                    getDM().releaseTmp(Lid, Lexp);
                }
            }
            else
            {
                if (RT.isscalar())
                {
                    if (Lid == -1)
                    {
                        tempId = getDM().getTmpId(resT, false);
                    }
                    else
                    {
                        if (resT == LT)
                        {
                            tempId = Lid;
                        }
                        else if (Rid != -1 && resT == RT)
                        {
                            tempId = Rid;
                            getDM().releaseTmp(Lid, Lexp);
                        }
                        else
                        {
                            tempId = getDM().getTmpId(resT, false);
                            getDM().releaseTmp(Lid, Lexp);
                        }
                    }
                    getDM().releaseTmp(Rid, Rexp);
                }
                else
                {
                    if (Rid == -1)
                    {
                        if (resT == LT)
                        {
                            tempId = Lid;
                        }
                        else
                        {
                            tempId = getDM().getTmpId(resT, false);
                            getDM().releaseTmp(Lid, Lexp);
                        }
                    }
                    else
                    {
                        if (resT == RT)
                        {
                            tempId = Rid;
                        }
                        else if (Lid != -1 && resT == LT)
                        {
                            tempId = Lid;
                            getDM().releaseTmp(Rid, Rexp);
                        }
                        else
                        {
                            tempId = getDM().getTmpId(resT, false);
                            getDM().releaseTmp(Rid, Rexp);
                        }
                        getDM().releaseTmp(Lid, Lexp);
                    }
                }
            }
        }
        else
        {
            tempId = getDM().getTmpId(resT, false);
        }
    }

    return tempId;
}
}
