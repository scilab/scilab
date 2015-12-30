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

#include "AnalysisVisitor.hxx"
#include "analyzers/LengthAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{
bool LengthAnalyzer::analyze(AnalysisVisitor & visitor, const unsigned int lhs, ast::CallExp & e)
{
    if (lhs > 2)
    {
        return false;
    }

    const ast::exps_t args = e.getArgs();
    if (args.size() != 1)
    {
        return false;
    }

    ast::Exp * first = args.front();
    if (!first)
    {
        return false;
    }
    first->accept(visitor);
    Result & res = visitor.getResult();
    TIType & resType = res.getType();
    if (!resType.ismatrix() || resType.type == TIType::STRING)
    {
        return false;
    }

    TIType type(visitor.getGVN(), TIType::DOUBLE);
    SymbolicDimension prod = resType.rows * resType.cols;

    Result & _res = e.getDecorator().setResult(type);
    _res.getConstant() = prod.getValue();
    e.getDecorator().setCall("length", resType);
    visitor.setResult(_res);
    return true;
}
}
