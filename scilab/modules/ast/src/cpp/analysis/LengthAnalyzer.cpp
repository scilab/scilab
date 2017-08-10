/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
    e.getDecorator().setCall(L"length", resType);
    visitor.setResult(_res);
    return true;
}
}
