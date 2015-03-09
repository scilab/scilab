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

    TestGVNVisitor()
    {
        //start_chrono();
    }

    virtual ~TestGVNVisitor()
    {
    }

    inline void print_info()
    {
        //stop_chrono();
        std::wcout << L"GVN: " << /* *static_cast<Chrono *>(this) << */ std::endl;
        std::wcout << gvn << std::endl;
    }

    inline void result_set(const GVN::Value * val)
    {
        _result = val;
    }

    inline const GVN::Value & result_get()
    {
        return *_result;
    }

    inline std::map<std::wstring, unsigned long long> getSymMap() const
    {
        return gvn.getSymMap();
    }

private:

    void visit(ast::SimpleVar & e)
    {
        result_set(gvn.getValue(e.getSymbol()));
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
        result_set(gvn.getValue(e.getValue()));
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
        const GVN::Value & LV = result_get();
        e.getRight().accept(*this);
        const GVN::Value & RV = result_get();

        switch (e.getOper())
        {
            case ast::OpExp::plus :
                result_set(gvn.getValue(OpValue::PLUS, LV, RV));
                break;
            case ast::OpExp::minus :
                result_set(gvn.getValue(OpValue::MINUS, LV, RV));
                break;
            case ast::OpExp::unaryMinus :
                result_set(gvn.getValue(OpValue::UNARYMINUS, RV));
                break;
            case ast::OpExp::rdivide :
                result_set(gvn.getValue(OpValue::RDIV, LV, RV));
                break;
            case ast::OpExp::dotrdivide :
                result_set(gvn.getValue(OpValue::DOTRDIV, LV, RV));
                break;
            case ast::OpExp::times :
                result_set(gvn.getValue(OpValue::TIMES, LV, RV));
                break;
            case ast::OpExp::dottimes :
                result_set(gvn.getValue(OpValue::DOTTIMES, LV, RV));
                break;
            case ast::OpExp::power :
                result_set(gvn.getValue(OpValue::POWER, LV, RV));
                break;
            case ast::OpExp::dotpower :
                result_set(gvn.getValue(OpValue::DOTPOWER, LV, RV));
                break;
            case ast::OpExp::eq :
                if (LV.value == RV.value)
                {
                    result_set(gvn.getValue(1));
                }
                else
                {
                    result_set(gvn.getValue(0));
                }
                break;
            case ast::OpExp::ne :
                if (LV.value != RV.value)
                {
                    result_set(gvn.getValue(1));
                }
                else
                {
                    result_set(gvn.getValue(0));
                }
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
                std::unordered_map<unsigned long long, const MultivariatePolynomial *> args;
                const symbol::Symbol & sym = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
                for (auto & arg : ce.getExps())
                {
                    if (arg->isAssignExp())
                    {
                        ast::AssignExp & ae = static_cast<ast::AssignExp &>(*arg);
                        const symbol::Symbol & _Lsym = static_cast<ast::SimpleVar &>(ae.getLeftExp()).getSymbol();
                        ae.getRightExp().accept(*this);
                        args[gvn.getValue(_Lsym)->value] = result_get().poly;
                    }
                }
                const GVN::Value * callee = gvn.getValue(sym);
                gvn.setValue(Lsym, callee->poly->eval(args));
            }
            else
            {
                e.getRightExp().accept(*this);
                gvn.setValue(Lsym, result_get());
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

    void visit(ast::DAXPYExp & e)
    {
    }
};

} // namespace analysis

#endif // __TEST_GVN_VISITOR_HXX__
