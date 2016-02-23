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
