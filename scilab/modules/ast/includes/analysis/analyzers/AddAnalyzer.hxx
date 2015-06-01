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

#ifndef __ADD_ANALYZER_HXX__
#define __ADD_ANALYZER_HXX__

#include <string>

#include "Analyzer.hxx"
#include "HypotCall.hxx"

namespace analysis
{

class AddAnalyzer : public Analyzer
{

public:

    AddAnalyzer() : Analyzer() { }

    bool analyze(AnalysisVisitor & visitor, ast::Exp & e)
    {
        ast::OpExp & oe = static_cast<ast::OpExp &>(e);
        ast::Exp & L = op.left_get();
        ast::Exp & R = op.right_get();
        if (L.is_call_exp())
        {
            ast::CallExp & ce = static_cast<ast::CallExp &>(L);
            const std::wstring & name = static_cast<ast::SimpleVar &>(ce.name_get()).name_get().name_get();
            if (name == L"ones")
            {
                // ones(...) + A
                // <=> 1 + A (if A matrix) or (1+A)*ones(...) (ie a fill)
                visitor.visitArguments(L"ones", ty, ce, ce.args_get());
                if (R.getDecorator().res.getType().isscalar())
                {
                    ce.getDecorator().setCall(new FillCall(ce.args_get(), R, true));
                    return true;
                }
            }
            else if (name == "zeros")
            {
                // zeros(...) + A
                // <=> A (if A matrix) or fill matrix with A
                if (R.getDecorator().res.getType().isscalar())
                {
                    e.getDecorator().setCall(new FillCall(e.args_get(), R, false));
                    return true;
                }
            }
        }

        if (R.is_call_exp())
        {
            ast::CallExp & ce = static_cast<ast::CallExp &>(R);
            const std::wstring & name = static_cast<ast::SimpleVar &>(ce.name_get()).name_get().name_get();
            if (name == L"ones")
            {
                if (L.getDecorator().res.getType().isscalar())
                {
                    e.getDecorator().setCall(new FillCall(e.args_get(), L, true));
                    return true;
                }
            }
            else if (name == "zeros")
            {
                // A + zeros(...)
                if (L.getDecorator().res.getType().isscalar())
                {
                    e.getDecorator().setCall(new FillCall(e.args_get(), L, false));
                    return true;
                }
            }
        }

        if (L.is_op_exp())
        {
            ast::OpExp & oe = static_cast<ast::OpExp &>(L);
            if (oe.oper_get() == ast::OpExp::times)
            {
                // A * B + C
                // this relation can be exploited when:
                // i) A,B,C are matrices
                // ii) C is a temp or C=A*B+C (which induces that C is a temp)
                // If these two conditions are met we can smartly use dgemm.
                // We could maybe use the fma intrinsic
                e.getDecorator().setCall(new FMACall(oe.left_get(), oe.right_get(), R));
                return true;
            }
        }

        if (R.is_op_exp())
        {
            // A + B * C
            ast::OpExp & oe = static_cast<ast::OpExp &>(R);
            if (oe.oper_get() == ast::OpExp::times)
            {
                e.getDecorator().setCall(new FMACall(oe.left_get(), oe.right_get(), L));
                return true;
            }
        }

        // Handle things like "abc"+string(x)+"def" (x is scalar)
        // The idea is to make one concatenation of several strings rather
        // than several concatenations of two strings.
        // For now, we just handle scalar case.
        if (L.getDecorator().res.getType().isScalarString())
        {
            std::vector<ast::Exp *> stexp;
            stexp.push_back(&L);
            ast::Exp * _e = &R;
            while (_e->is_op_exp() && static_cast<ast::OpExp *>(_e)->oper_get() == ast::OpExp::plus)
            {
                ast::Exp * _l = static_cast<ast::OpExp *>(_e)->left_get();
                if (_l.getDecorator().res.getType().isScalarString())
                {
                    stexp.push_back(_l);
                    _e = static_cast<ast::OpExp *>(_e)->right_get();
                }
                else
                {
                    break;
                }
            }
            if (_e.getDecorator().res.getType().isScalarString())
            {
                stexp.push_back(_e);
                _e = nullptr;
            }

            if (stexp.size() > 1)
            {
                e.getDecorator().setCall(new StringCatCall(stexp, _e));
                return true;
            }
        }

        return false;
    }

};
}

#endif // __ADD_ANALYZER_HXX__
