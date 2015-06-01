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
        const Result & resL = oe.getLeft().getDecorator().getResult();
        const Result & resR = oe.getRight().getDecorator().getResult();
        const ConstantValue & valL = resL.getConstant();
        const ConstantValue & valR = resR.getConstant();
        GVN::Value * gvnL = nullptr;
        GVN::Value * gvnR = nullptr;

        if (valL.getKind() == ConstantValue::GVNVAL)
        {
            gvnL = valL.get<GVN::Value *>();
        }
        else
        {
            double val;
            if (valL.getDblValue(val) && tools::isAnInt(val))
            {
                gvnL = getGVN().getValue(val);
            }
        }

        if (valR.getKind() == ConstantValue::GVNVAL)
        {
            gvnR = valR.get<GVN::Value *>();
        }
        else
        {
            double val;
            if (valR.getDblValue(val) && tools::isAnInt(val))
            {
                gvnR = getGVN().getValue(val);
            }
        }

        if (gvnL && gvnR)
        {
	    TIType typ(getGVN(), TIType::DOUBLE, 1, 1);
        
            switch (oe.getOper())
            {
            case ast::OpExp::plus:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::PLUS, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            case ast::OpExp::minus:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::MINUS, *gvnL, *gvnR);
		setResult(res);
		return true;
                break;
            }
            case ast::OpExp::times:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::TIMES, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            case ast::OpExp::dottimes:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::DOTTIMES, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            case ast::OpExp::power:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::POWER, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            case ast::OpExp::dotpower:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::DOTPOWER, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            case ast::OpExp::rdivide:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::RDIV, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            case ast::OpExp::dotrdivide:
            {
                Result & res = oe.getDecorator().setResult(typ);
                res.getConstant() = getGVN().getValue(OpValue::DOTRDIV, *gvnL, *gvnR);
		setResult(res);
		return true;
            }
            }
        }

	return false;
    }
}
