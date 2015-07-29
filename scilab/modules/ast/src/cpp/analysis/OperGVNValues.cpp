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
