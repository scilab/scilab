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

void AnalysisVisitor::visit(ast::DoubleExp & e)
{
    logger.log(L"DoubleExp", e.getLocation());
    if (!e.getConstant())
    {
        e.accept(cv.getExec());
        cv.getExec().setResult(nullptr);
    }
    types::Double * pDbl = static_cast<types::Double *>(e.getConstant());
    if (pDbl->isComplex())
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::COMPLEX, pDbl->getRows(), pDbl->getCols()));
        res.getConstant() = e.getConstant();
        setResult(res);
    }
    else
    {
        Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::DOUBLE, pDbl->getRows(), pDbl->getCols()));
        res.getConstant() = e.getConstant();
        setResult(res);
    }
}

void AnalysisVisitor::visit(ast::BoolExp & e)
{
    logger.log(L"BoolExp", e.getLocation());
    if (!e.getConstant())
    {
        e.accept(cv.getExec());
        cv.getExec().setResult(nullptr);
    }
    types::Bool * pBool = static_cast<types::Bool *>(e.getConstant());
    Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::BOOLEAN, pBool->getRows(), pBool->getCols()));
    res.getConstant() = e.getConstant();
    setResult(res);
}

void AnalysisVisitor::visit(ast::StringExp & e)
{
    logger.log(L"StringExp", e.getLocation());
    if (!e.getConstant())
    {
        e.accept(cv.getExec());
        cv.getExec().setResult(nullptr);
    }
    types::String * pStr = static_cast<types::String *>(e.getConstant());
    Result & res = e.getDecorator().setResult(TIType(dm.getGVN(), TIType::STRING, pStr->getRows(), pStr->getCols()));
    res.getConstant() = e.getConstant();
    setResult(res);
}


} // namespace analysis
