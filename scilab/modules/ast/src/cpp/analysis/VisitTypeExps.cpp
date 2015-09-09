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
