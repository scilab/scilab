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

namespace analysis
{

void AnalysisVisitor::visit(ast::DollarVar & e)
{
    logger.log(L"DollarVar", e.getLocation());
    Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::POLYNOMIAL, 1, 1));
    res.getConstant() = getGVN().getValue(symbol::Symbol(L"$"));

    if (argIndices.empty())
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::POLYNOMIAL, 1, 1));
        res.getConstant() = getGVN().getValue(symbol::Symbol(L"$"));
        setResult(res);
    }
    else
    {
        // we have something like A(1, $ - 1)
        e.getDecorator().setDollarInfo(argIndices.top());
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::DOUBLE, 1, 1));
        res.getConstant() = getGVN().getValue(symbol::Symbol(L"$"));
        setResult(res);
    }
}

} // namespace analysis
