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

#ifndef __CEIL_ANALYZER_HXX__
#define __CEIL_ANALYZER_HXX__

#include <string>

#include "CallAnalyzer.hxx"

namespace analysis
{

class CeilAnalyzer : public CallAnalyzer
{

public:

    CeilAnalyzer() : CallAnalyzer() { }

    bool analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
    {
        const ast::exps_t args = e.getArgs();
        if (args.size() == 1)
        {
            ast::exps_t::const_iterator first = args.begin();
            (*first)->accept(visitor);
            Result & R = visitor.getResult();
            const TIType & Rtype = R.getType();
            if (Rtype.isintegral())
            {
                e.getDecorator().res = Result(Rtype);
                e.getDecorator().setCall(Call(Call::IDENTITY, Rtype, L"ceil & co"));
                visitor.setResult(e.getDecorator().res);

                return true;
            }
        }

        return false;
    }
};

} // namespace analysis

#endif // __CEIL_ANALYZER_HXX__
