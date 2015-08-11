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

#ifndef __MATRIX_ANALYZER_HXX__
#define __MATRIX_ANALYZER_HXX__

#include "CallAnalyzer.hxx"

namespace analysis
{

class MatrixAnalyzer : public CallAnalyzer
{

public:

    MatrixAnalyzer() : CallAnalyzer() { }

    bool analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e);

private:

    bool analyze2Args(AnalysisVisitor & visitor, const ast::exps_t & args, ast::CallExp & e);
};

} // namespace analysis

#endif // __MATRIX_ANALYZER_HXX__
