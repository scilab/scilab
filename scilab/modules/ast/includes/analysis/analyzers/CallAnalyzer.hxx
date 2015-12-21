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

#ifndef __CALL_ANALYZER_HXX__
#define __CALL_ANALYZER_HXX__

namespace analysis
{
class AnalysisVisitor;
class CallExp;

class CallAnalyzer
{

public:

    enum FnName
    {
        ZEROS = 0, ONES, RAND, MATRIX, EYE, __COUNT__
    };

    CallAnalyzer() { }

    virtual bool analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e) = 0;

    virtual FnName getFnName() const
    {
        return __COUNT__;
    }
};

} // namespace analysis

#endif // __CALL_ANALYZER_HXX__
