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

#ifndef __POSITIVITY_VISITOR_HXX__
#define __POSITIVITY_VISITOR_HXX__

#include <iostream>
#include <string>
#include <map>

#include "visitor.hxx"
#include "execvisitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "Chrono.hxx"
#include "GVN.hxx"

namespace analysis
{

class PositivityVisitor : public ast::Visitor, public Chrono
{

    bool _result;
    AnalysisVisitor & parent;

public:

    PositivityVisitor(AnalysisVisitor & _parent) : parent(_parent) { }
    {
        start_chrono();
    }

    virtual ~PositivityVisitor()
    {
    }

    inline void print_info()
    {
        stop_chrono();
    }

private:

    inline void setResult(const bool val)
    {
        _result = val;
    }

    inline bool getResult()
    {
        return _result;
    }

    inline const TIType & getType(const symbol::Symbol & sym)
    {
        return parent.getSymInfo(sym).type;
    }

    void visit(ast::SimpleVar & e)
    {
        setResult(false);
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
        setResult(e.value_get() >= 0);
    }

    void visit(ast::DoubleExp & e)
    {
        setResult(e.value_get() >= 0);
    }

    void visit(ast::BoolExp & e)
    {
        setResult(true);
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
        const symbol::Symbol & sym = static_cast<ast::SimpleVar &>(e.name_get()).name_get();
        const std::wstring & name = sym.name_get();
        if (name == L"abs" || name == L"exp" || name == "norm")
        {
            setResult(true);
        }
        else if (name == L"sqrt")
        {
            (*e.args_get())->accept(*this);
        }
    }

    void visit(ast::CellCallExp & e)
    {
    }

    void visit(ast::OpExp & e)
    {
        e.left_get().accept(*this);
        const bool l = getResult();
        e.right_get().accept(*this);
        const bool r = getResult();

        switch (e.oper_get())
        {
            case ast::OpExp::plus:
            {
                e.left_get().accept(*this);
                const bool l = getResult();
                e.right_get().accept(*this);
                setResult(l && getResult());
                break;
            }
            case ast::OpExp::minus:
                setResult(false);
                break;
            case ast::OpExp::unaryMinus:
                break;
            case ast::OpExp::rdivide:
                if (e.right_get().is_double_exp() && static_cast<ast::DoubleExp &>(e.right_get()).value_get() > 0)
                {
                    e.left_get().accept(*this);
                }
                break;
            case ast::OpExp::dotrdivide:
                e.left_get().accept(*this);
                const bool l = getResult();
                e.right_get().accept(*this);
                setResult(l && getResult());
                break;
            case ast::OpExp::times:
            {
                e.left_get().accept(*this);
                const bool l = getResult();
                e.right_get().accept(*this);
                if (l && getResult())// || e.left_get() == e.right_get())
                {
                    setResult(true);
                }
                break;
            }
            case ast::OpExp::dottimes:
            {
                e.left_get().accept(*this);
                const bool l = getResult();
                e.right_get().accept(*this);
                setResult(l && getResult());
                break;
            }
            case ast::OpExp::power:
                if (e.right_get().is_double_exp())
                {
                    const double expo = static_cast<ast::DoubleExp &>(e.right_get()).value_get();
                    if (expo == tools::trunc(expo) && expo >= 0)
                    {
                        // exponent is a positive integer so if A >= 0 then A^n >= 0
                        e.left_get().accept(*this);
                    }
                    else
                    {
                        setResult(false);
                    }
                }
                else
                {
                    setResult(false);
                }
                break;
            case ast::OpExp::dotpower:
            {
                if (e.right_get().is_double_exp())
                {
                    const double expo = static_cast<ast::DoubleExp &>(e.right_get()).value_get();
                    if (expo == tools::trunc(expo))
                    {
                        unsigned long long _expo = (unsigned long long)expo;
                        if (expo & 1)
                        {
                            e.left_get().accept(*this);
                        }
                        else
                        {
                            setResult(true);
                        }
                    }
                    else
                    {
                        e.left_get().accept(*this);
                    }
                }
                break;
            }
            default:
        }
    }

    void visit(ast::LogicalOpExp & e)
    {
    }

    void visit(ast::AssignExp & e)
    {
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
};

} // namespace analysis

#endif // __POSITIVITY_VISITOR_HXX__
