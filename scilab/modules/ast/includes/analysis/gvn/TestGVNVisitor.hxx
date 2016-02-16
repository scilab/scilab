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

#ifndef __TEST_GVN_VISITOR_HXX__
#define __TEST_GVN_VISITOR_HXX__

#include <iostream>
#include <string>
#include <map>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
//#include "Chrono.hxx"
#include "GVN.hxx"

namespace analysis
{

class TestGVNVisitor : public ast::Visitor /*, public Chrono */
{

    GVN gvn;
    const GVN::Value * _result;

public:

    TestGVNVisitor() : _result(nullptr)
    {
        //start_chrono();
    }

    virtual ~TestGVNVisitor()
    {
    }

    virtual TestGVNVisitor* clone()
    {
        return new TestGVNVisitor();
    }

    inline void print_info()
    {
        //stop_chrono();
        std::wcout << L"GVN: " << /* *static_cast<Chrono *>(this) << */ std::endl;
        std::wcout << gvn << std::endl;
    }

    inline void setResult(const GVN::Value * val)
    {
        _result = val;
    }

    inline const GVN::Value & getResult()
    {
        return *_result;
    }

    inline std::map<std::wstring, uint64_t> getSymMap() const
    {
        return gvn.getSymMap();
    }

private:

    void visit(ast::SimpleVar & e)
    {
        setResult(gvn.getValue(e.getSymbol()));
    }

    void visit(ast::DollarVar & e)
    {
        // nothing to do
    }

    void visit(ast::ColonVar & e)
    {
        // nothing to do
    }

    void visit(ast::ArrayListVar & e)
    {
    }

    void visit(ast::DoubleExp & e)
    {
        setResult(gvn.getValue(e.getValue()));
    }

    void visit(ast::BoolExp & e)
    {
    }

    void visit(ast::StringExp & e)
    {
    }

    void visit(ast::CommentExp & e)
    {
        // ignored
    }

    void visit(ast::NilExp & e)
    {
        // nothing to do
    }

    void visit(ast::CallExp & e)
    {
    }

    void visit(ast::CellCallExp & e)
    {
    }

    void visit(ast::OpExp & e)
    {
        e.getLeft().accept(*this);
        const GVN::Value & LV = getResult();
        e.getRight().accept(*this);
        const GVN::Value & RV = getResult();

        switch (e.getOper())
        {
            case ast::OpExp::plus:
                setResult(gvn.getValue(OpValue::PLUS, LV, RV));
                break;
            case ast::OpExp::minus:
                setResult(gvn.getValue(OpValue::MINUS, LV, RV));
                break;
            case ast::OpExp::unaryMinus:
                setResult(gvn.getValue(OpValue::UNARYMINUS, RV));
                break;
            case ast::OpExp::rdivide:
                setResult(gvn.getValue(OpValue::RDIV, LV, RV));
                break;
            case ast::OpExp::dotrdivide:
                setResult(gvn.getValue(OpValue::DOTRDIV, LV, RV));
                break;
            case ast::OpExp::times:
                setResult(gvn.getValue(OpValue::TIMES, LV, RV));
                break;
            case ast::OpExp::dottimes:
                setResult(gvn.getValue(OpValue::DOTTIMES, LV, RV));
                break;
            case ast::OpExp::power:
                setResult(gvn.getValue(OpValue::POWER, LV, RV));
                break;
            case ast::OpExp::dotpower:
                setResult(gvn.getValue(OpValue::DOTPOWER, LV, RV));
                break;
            case ast::OpExp::eq:
                if (LV.value == RV.value)
                {
                    setResult(gvn.getValue(int64_t(1)));
                }
                else
                {
                    setResult(gvn.getValue(int64_t(0)));
                }
                break;
            case ast::OpExp::ne:
                if (LV.value != RV.value)
                {
                    setResult(gvn.getValue(int64_t(1)));
                }
                else
                {
                    setResult(gvn.getValue(int64_t(0)));
                }
                break;
            case ast::OpExp::ldivide:
            case ast::OpExp::dotldivide:
            case ast::OpExp::krontimes:
            case ast::OpExp::kronrdivide:
            case ast::OpExp::kronldivide:
            case ast::OpExp::controltimes:
            case ast::OpExp::controlrdivide:
            case ast::OpExp::controlldivide:
            case ast::OpExp::lt:
            case ast::OpExp::le:
            case ast::OpExp::gt:
            case ast::OpExp::ge:
            case ast::OpExp::logicalAnd:
            case ast::OpExp::logicalOr:
            case ast::OpExp::logicalShortCutAnd:
            case ast::OpExp::logicalShortCutOr:
                std::cerr << "TestGVNVisitor: unsupported ast::OpExp" << std::endl;
                break;
        }
    }

    void visit(ast::LogicalOpExp & e)
    {
    }

    void visit(ast::AssignExp & e)
    {
        if (e.getLeftExp().isSimpleVar())
        {
            ast::SimpleVar & Lvar = static_cast<ast::SimpleVar &>(e.getLeftExp());
            symbol::Symbol & Lsym = Lvar.getSymbol();

            if (e.getRightExp().isCallExp())
            {
                ast::CallExp & ce = static_cast<ast::CallExp &>(e.getRightExp());
                std::unordered_map<uint64_t, const MultivariatePolynomial *> args;
                const symbol::Symbol & sym = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
                for (auto & arg : ce.getExps())
                {
                    if (arg->isAssignExp())
                    {
                        ast::AssignExp & ae = static_cast<ast::AssignExp &>(*arg);
                        const symbol::Symbol & _Lsym = static_cast<ast::SimpleVar &>(ae.getLeftExp()).getSymbol();
                        ae.getRightExp().accept(*this);
                        args[gvn.getValue(_Lsym)->value] = getResult().poly;
                    }
                }
                const GVN::Value * callee = gvn.getValue(sym);
                gvn.setValue(Lsym, callee->poly->eval(args));
            }
            else
            {
                e.getRightExp().accept(*this);
                gvn.setValue(Lsym, getResult());
            }
        }
    }

    void visit(ast::IfExp & e)
    {
    }

    void visit(ast::WhileExp & e)
    {
    }

    void visit(ast::ForExp & e)
    {
    }

    void visit(ast::BreakExp & e)
    {
        // nothing to do
    }

    void visit(ast::ContinueExp & e)
    {
        // nothing to do
    }

    void visit(ast::TryCatchExp & e)
    {
    }

    void visit(ast::SelectExp & e)
    {
    }

    void visit(ast::CaseExp & e)
    {
    }

    void visit(ast::ReturnExp & e)
    {
    }

    void visit(ast::FieldExp & e)
    {
    }

    void visit(ast::NotExp & e)
    {
    }

    void visit(ast::TransposeExp & e)
    {
    }

    void visit(ast::MatrixExp & e)
    {
    }

    void visit(ast::MatrixLineExp & e)
    {
    }

    void visit(ast::CellExp & e)
    {
    }

    void visit(ast::SeqExp & e)
    {
        for (auto exp : e.getExps())
        {
            exp->accept(*this);
        }
    }

    void visit(ast::ArrayListExp & e)
    {
    }

    void visit(ast::AssignListExp & e)
    {
    }

    void visit(ast::VarDec & e)
    {
    }

    void visit(ast::FunctionDec & e)
    {
    }

    void visit(ast::ListExp & e)
    {
    }

    void visit(ast::OptimizedExp & e)
    {
    }

    void visit(ast::MemfillExp & e)
    {
    }

    void visit(ast::DAXPYExp & e)
    {
    }

    void visit(ast::IntSelectExp & e)
    {
    }

    void visit(ast::StringSelectExp & e)
    {
    }
};

} // namespace analysis

#endif // __TEST_GVN_VISITOR_HXX__
