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
#include "analyzers/OperAnalyzer.hxx"
#include "tools.hxx"

namespace analysis
{

    void AnalysisVisitor::visit(ast::OpExp & e)
    {
        logger.log(L"OpExp", e.getLocation());
        TIType resT(getGVN());
        int tempId = -1;

        e.getLeft().accept(*this);
        Result LR = getResult();
        e.getRight().accept(*this);
        Result & RR = getResult();
        if (LR.getType().isknown() && RR.getType().isknown())
        {
            TIType & LT = LR.getType();
            TIType & RT = RR.getType();

            if (!operSymbolicRange(e) && !operGVNValues(e))
            {
                switch (e.getOper())
                {
                case ast::OpExp::plus :
                {
                    resT = check_____add____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());

                        if (ret)
                        {
                            resT = check_____add____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    if (resT.isscalar())
                    {
                    }
                    break;
                }
                case ast::OpExp::minus:
                {
                    resT = check_____sub____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____sub____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    if (resT.isscalar())
                    {
                    }
                    break;
                }
                case ast::OpExp::times:
                {
                    resT = check_____times____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.rows.getValue());
                        if (ret)
                        {
                            resT = check_____times____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::rdivide:
                {
                    resT = check_____rdivide____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____rdivide____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::ldivide:
                {
                    resT = check_____ldivide____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.rows.getValue(), RT.rows.getValue());
                        if (ret)
                        {
                            resT = check_____ldivide____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::power:
                {
                    resT = check_____power____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.rows.getValue(), LT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____power____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::dottimes :
                {
                    resT = check_____dottimes____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____dottimes____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    if (resT.isscalar())
                    {
                    }
                    break;
                }
                case ast::OpExp::dotrdivide:
                {
                    resT = check_____dotrdiv____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____dotrdiv____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::dotpower:
                {
                    resT = check_____dotpower____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::EQUAL, LT.cols.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____dotpower____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::unaryMinus :
                {
                    resT = check_____unaryminus____(getGVN(), RT);
                    break;
                }
                case ast::OpExp::krontimes :
                {
                    resT = check_____krontimes____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::eq:
                {
                    resT = check_____eq____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::ne:
                {
                    resT = check_____neq____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::lt:
                {
                    resT = check_____lt____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::le:
                {
                    resT = check_____le____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::gt:
                {
                    resT = check_____gt____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::ge:
                {
                    resT = check_____ge____(getGVN(), LT, RT);
                    break;
                }
                case ast::OpExp::logicalAnd:
                {
                    resT = check_____and____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____and____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::logicalOr:
                {
                    resT = check_____or____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____or____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::logicalShortCutAnd:
                {
                    resT = check_____andand____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____andand____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                case ast::OpExp::logicalShortCutOr:
                {
                    resT = check_____oror____(getGVN(), LT, RT);
                    if (resT.hasInvalidDims())
                    {
                        const bool ret = getCM().check(ConstraintManager::SAMEDIMS, LT.rows.getValue(), LT.cols.getValue(), RT.rows.getValue(), RT.cols.getValue());
                        if (ret)
                        {
                            resT = check_____oror____(getGVN(), LT, RT);
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
                    break;
                }
                }
            }
            else
            {
                // SymbolicRange or GVNValue ops.
                return;
            }
        }

        e.getDecorator().res = Result(resT, tempId);
        setResult(e.getDecorator().res);

	OperAnalyzer opAn;
	opAn.analyze(*this, e);
    }

    void AnalysisVisitor::visit(ast::NotExp & e)
    {
        logger.log(L"NotExp", e.getLocation());
        e.getExp().accept(*this);
        Result & LR = getResult();
        TIType & LT = LR.getType();
        if (LT.isknown())
        {
            TIType resT = check_____not____(getGVN(), LT);
            e.getDecorator().res = Result(resT, -1);
        }
        else
        {
            e.getDecorator().res = Result(TIType(getGVN()), -1);
        }
        setResult(e.getDecorator().res);
    }

    void AnalysisVisitor::visit(ast::TransposeExp & e)
    {
        logger.log(L"TransposeExp", e.getLocation());
        e.getExp().accept(*this);
        Result & res = getResult();
        const TIType & type = res.getType();
        e.getDecorator().res = Result(TIType(dm.getGVN(), type.type, type.cols, type.rows));
        setResult(e.getDecorator().res);
    }
}
