/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "AnalysisVisitor.hxx"
#include "analyzers/CeilAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool CeilAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs <= 1)
    {
        const ast::exps_t args = e.getArgs();
        if (args.size() == 1)
        {
            ast::exps_t::const_iterator first = args.begin();
            (*first)->accept(visitor);
            Result & R = visitor.getResult();
            const TIType & Rtype = R.getType();
            const symbol::Symbol & sym = static_cast<ast::SimpleVar &>(e.getName()).getSymbol();
            const std::wstring & name = sym.getName();
            std::vector<TIType> vargs({ Rtype });
	    uint64_t functionId = 0;
            std::vector<TIType> out = visitor.getDM().call(visitor, lhs, sym, vargs, &e, functionId);
            if (Rtype.isintegral())
            {
                const ast::SimpleVar & var = static_cast<ast::SimpleVar &>(e.getName());

                e.getDecorator().res = Result(Rtype, R.getTempId(), functionId);
                e.getDecorator().setCall(name, vargs);
                visitor.setResult(e.getDecorator().res);

                return true;
            }

            if (out[0].type != TIType::UNKNOWN)
            {
                e.getDecorator().res = Result(Rtype, R.getTempId(), functionId);
                e.getDecorator().setCall(name, vargs);
                visitor.setResult(e.getDecorator().res);

                return true;
            }
            else
            {
                visitor.getDM().releaseTmp(R.getTempId(), *first);
            }
        }
    }

    return false;
}
}
