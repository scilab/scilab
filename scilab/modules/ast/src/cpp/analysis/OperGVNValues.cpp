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
#include "tools.hxx"

namespace analysis
{
bool AnalysisVisitor::operGVNValues(ast::OpExp & oe)
{
    Result & resL = oe.getLeft().getDecorator().getResult();
    Result & resR = oe.getRight().getDecorator().getResult();
    const ConstantValue & valL = resL.getConstant();
    const ConstantValue & valR = resR.getConstant();
    GVN::Value * gvnL = nullptr;
    GVN::Value * gvnR = nullptr;
    bool LisInt = false;
    bool RisInt = false;

    if (valL.getKind() == ConstantValue::GVNVAL)
    {
        gvnL = valL.getGVNValue();
    }
    else
    {
        double val;
        if (valL.getDblValue(val) && tools::isAnInt(val))
        {
            gvnL = getGVN().getValue(val);
            LisInt = true;
        }
    }

    if (valR.getKind() == ConstantValue::GVNVAL)
    {
        gvnR = valR.getGVNValue();
    }
    else
    {
        double val;
        if (valR.getDblValue(val) && tools::isAnInt(val))
        {
            gvnR = getGVN().getValue(val);
            RisInt = true;
        }
    }

    if (gvnL && gvnR)
    {
        TIType typ(getGVN(), TIType::DOUBLE, 1, 1);

        switch (oe.getOper())
        {
            case ast::OpExp::plus:
            {
                if (LisInt)
                {
                    resL.getConstant() = gvnL;
                }
                if (RisInt)
                {
                    resR.getConstant() = gvnR;
                }
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::PLUS, *gvnL, *gvnR);
                oe.getDecorator().safe = true;
                setResult(res);
                return true;
            }
            case ast::OpExp::minus:
            {
                if (LisInt)
                {
                    resL.getConstant() = gvnL;
                }
                if (RisInt)
                {
                    resR.getConstant() = gvnR;
                }
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::MINUS, *gvnL, *gvnR);
                oe.getDecorator().safe = true;
                setResult(res);
                return true;
                break;
            }
            case ast::OpExp::times:
            {
                if (LisInt)
                {
                    resL.getConstant() = gvnL;
                }
                if (RisInt)
                {
                    resR.getConstant() = gvnR;
                }
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::TIMES, *gvnL, *gvnR);
                oe.getDecorator().safe = true;
                setResult(res);
                return true;
            }
            case ast::OpExp::dottimes:
            {
                if (LisInt)
                {
                    resL.getConstant() = gvnL;
                }
                if (RisInt)
                {
                    resR.getConstant() = gvnR;
                }
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::DOTTIMES, *gvnL, *gvnR);
                oe.getDecorator().safe = true;
                setResult(res);
                return true;
            }
            case ast::OpExp::power:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::POWER, *gvnL, *gvnR);
                oe.getDecorator().safe = true;
                setResult(res);
                return true;
            }
            case ast::OpExp::dotpower:
            {
                if (LisInt)
                {
                    resL.getConstant() = gvnL;
                }
                if (RisInt)
                {
                    resR.getConstant() = gvnR;
                }
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::DOTPOWER, *gvnL, *gvnR);
                oe.getDecorator().safe = true;
                setResult(res);
                return true;
            }
            case ast::OpExp::rdivide:
            {
                if (gvnL->poly->isDivisibleBy(*gvnR->poly))
                {
                    if (LisInt)
                    {
                        resL.getConstant() = gvnL;
                    }
                    if (RisInt)
                    {
                        resR.getConstant() = gvnR;
                    }
                    Result & res = oe.getDecorator().setResult(typ);
                    res.getConstant() = getGVN().getValue(OpValue::RDIV, *gvnL, *gvnR);
                    oe.getDecorator().safe = true;
                    setResult(res);
                    return true;
                }
                break;
            }
            case ast::OpExp::dotrdivide:
            {
                if (gvnL->poly->isDivisibleBy(*gvnR->poly))
                {
                    if (LisInt)
                    {
                        resL.getConstant() = gvnL;
                    }
                    if (RisInt)
                    {
                        resR.getConstant() = gvnR;
                    }
                    Result & res = oe.getDecorator().setResult(typ);
                    res.getConstant() = getGVN().getValue(OpValue::DOTRDIV, *gvnL, *gvnR);
                    oe.getDecorator().safe = true;
                    setResult(res);
                    return true;
                }
            }
        }
    }

    return false;
}
}
