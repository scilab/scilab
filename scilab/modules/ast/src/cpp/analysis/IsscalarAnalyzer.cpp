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
#include "analyzers/IsscalarAnalyzer.hxx"

namespace analysis
{
bool IsscalarAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs != 1)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 1)
    {
        return false;
    }

    ast::Exp * first = args.front();
    first->accept(visitor);
    Result & R1 = visitor.getResult();
    TIType & type1 = R1.getType();

    if (!type1.ismatrix())
    {
        return false;
    }

    if (type1.isscalar())
    {
        TIType type(visitor.getGVN(), TIType::BOOLEAN);
        Result & res = e.getDecorator().setResult(type);
        res.getConstant() = new types::Bool(1);
        e.getDecorator().setCall("isscalar");
        visitor.setResult(res);
        return true;
    }

    return false;
}
}
