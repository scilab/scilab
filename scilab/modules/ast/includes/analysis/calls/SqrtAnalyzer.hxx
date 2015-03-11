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

#ifndef __SQRT_ANALYZER_HXX__
#define __SQRT_ANALYZER_HXX__

#include <string>

#include "CallAnalyzer.hxx"
#include "HypotCall.hxx"

namespace analysis
{

class SqrtAnalyzer : public CallAnalyzer
{

public:

    SqrtAnalyzer() : CallAnalyzer() { }

    bool analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
    {
        const std::list<ast::Exp *> & args = e.args_get();
        if (args.size() == 1)
        {
            ast::Exp & arg = **args.begin();
            if (arg.is_call_exp())
            {
                ast::CallExp & ce = static_cast<ast::CallExp &>(arg);
                const symbol::Symbol & sym = static_cast<ast::SimpleVar &>(e.name_get()).name_get();
                if (sym.name_get() == L"abs")
                {

                }

            }
            else if (arg.is_double_exp())
            {
                const double sqrtx = std::sqrt(static_cast<ast::DoubleExp &>(arg).value_get());
            }

            if (arg.is_op_exp())
            {
                ast::OpExp & op = static_cast<ast::OpExp &>(arg);
                if (op.oper_get() == ast::OpExp::Oper::plus)
                {
                    ast::Exp & L = op.left_get();
                    ast::Exp & R = op.right_get();
                    if (L.is_op_exp() && R.is_op_exp())
                    {
                        ast::OpExp & lop = static_cast<ast::OpExp &>(L);
                        ast::OpExp & rop = static_cast<ast::OpExp &>(R);
                        if (lop.oper_get() == lop.oper_get())
                        {
                            if (lop.oper_get() == ast::OpExp::Oper::dotpower || lop.oper_get() == ast::OpExp::Oper::power)
                            {
                                ast::Exp & L2 = lop.right_get();
                                ast::Exp & R2 = rop.right_get();
                                if (L2.is_double_exp() && R2.is_double_exp() && static_cast<ast::DoubleExp &>(L2).value_get() == 2 && static_cast<ast::DoubleExp &>(L2).value_get() == 2)
                                {
                                    // We have something like sqrt(a^2+b^2) or sqrt(a.^2+b.^2)
                                    static_cast<ast::SimpleVar &>(e.name_get()).accept(visitor);
                                    const TIType & ty = visitor.getResult().getType();
                                    std::vector<ast::Exp *> args;
                                    args.reserve(2);
                                    args.push_back(&lop.left_get());
                                    args.push_back(&rop.left_get());
                                    if (lop.oper_get() == ast::OpExp::Oper::dotpower)
                                    {
                                        visitor.visitArguments(L"dothypot", lhs, ty, e, args);
                                        std::vector<TIType> v;
                                        e.getDecorator().setCall(new HypotCall(ty, lop.left_get(), rop.left_get(), v));
                                    }
                                    else
                                    {
                                        visitor.visitArguments(L"hypot", lhs, ty, e, args);
                                        e.getDecorator().getCall()->setKind(Call::HYPOT);
                                    }

                                    return true;
                                }
                            }
                        }
                    }
                }
            }
        }

        return false;
    }
};

} // namespace analysis

#endif // __SQRT_ANALYZER_HXX__
