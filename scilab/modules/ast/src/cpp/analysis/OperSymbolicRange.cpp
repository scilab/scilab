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
bool AnalysisVisitor::operSymbolicRange(ast::OpExp & oe)
{
    const ast::OpExp::Oper oper = oe.getOper();
    if (oper == ast::OpExp::plus || oper == ast::OpExp::minus || oper == ast::OpExp::times || oper == ast::OpExp::rdivide)
    {
        Result & resL = oe.getLeft().getDecorator().getResult();
        Result & resR = oe.getRight().getDecorator().getResult();
        const SymbolicRange & rangeL = resL.getRange();
        const SymbolicRange & rangeR = resR.getRange();
        const bool validL = rangeL.isValid();
        const bool validR = rangeR.isValid();
        SymbolicRange rangeOpL, rangeOpR;
        bool have2Ranges = false;
        bool LisInt = false;
        bool RisInt = false;
        GVN::Value * gvnR;
        GVN::Value * gvnL;

        if (validL && validR)
        {
            rangeOpL = rangeL;
            rangeOpR = rangeR;
            have2Ranges = true;
        }
        else if (validL)
        {
            rangeOpL = rangeL;
            if (resR.getConstant().getGVNValue(getGVN(), gvnR))
            {
                RisInt = true;
                rangeOpR.set(getGVN(), gvnR, gvnR);
                have2Ranges = true;
            }
        }
        else if (validR)
        {
            rangeOpR = rangeR;
            if (resL.getConstant().getGVNValue(getGVN(), gvnL))
            {
                LisInt = true;
                rangeOpL.set(getGVN(), gvnL, gvnL);
                have2Ranges = true;
            }
        }

        if (have2Ranges)
        {
            TIType typ(getGVN(), TIType::DOUBLE, 1, 1);
            switch (oper)
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
                    res.getRange() = rangeOpL + rangeOpR;
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
                    res.getRange() = rangeOpL - rangeOpR;
                    oe.getDecorator().safe = true;
                    setResult(res);
                    return true;
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
                    res.getRange() = rangeOpL * rangeOpR;
                    oe.getDecorator().safe = true;
                    setResult(res);
                    return true;
                }
                case ast::OpExp::rdivide:
                {
                    if (rangeOpL.getStart()->poly->isDivisibleBy(*rangeOpR.getEnd()->poly) && rangeOpL.getEnd()->poly->isDivisibleBy(*rangeOpR.getStart()->poly))
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
                        res.getRange() = rangeOpL / rangeOpR;
                        oe.getDecorator().safe = true;
                        setResult(res);
                        return true;
                    }
                }
            }
        }
    }

    return false;
}
}
