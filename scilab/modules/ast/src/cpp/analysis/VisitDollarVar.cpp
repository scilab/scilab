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

namespace analysis
{

void AnalysisVisitor::visit(ast::DollarVar & e)
{
    logger.log("DollarVar", e.getLocation());
    Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::POLYNOMIAL, 1, 1));
    res.getConstant() = getGVN().getValue(symbol::Symbol("$"));

    if (argIndices.empty())
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::POLYNOMIAL, 1, 1));
        res.getConstant() = getGVN().getValue(symbol::Symbol("$"));
        setResult(res);
    }
    else
    {
        // we have something like A(1, $ - 1)
        e.getDecorator().setDollarInfo(argIndices.top());
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::DOUBLE, 1, 1));
        res.getConstant() = getGVN().getValue(symbol::Symbol("$"));
        setResult(res);
    }
}

} // namespace analysis
