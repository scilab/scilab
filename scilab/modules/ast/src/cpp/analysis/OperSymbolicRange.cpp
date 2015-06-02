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
    bool AnalysisVisitor::operSymbolicRange(ast::OpExp & oe)
    {
        const ast::OpExp::Oper oper = oe.getOper();
        if (oper == ast::OpExp::plus || oper == ast::OpExp::minus || oper == ast::OpExp::times || oper == ast::OpExp::rdivide)
        {
            const Result & resL = oe.getLeft().getDecorator().getResult();
            const Result & resR = oe.getRight().getDecorator().getResult();
            const SymbolicRange & rangeL = resL.getRange();
            const SymbolicRange & rangeR = resR.getRange();
            const bool validL = rangeL.isValid();
            const bool validR = rangeR.isValid();
            SymbolicRange rangeOpL, rangeOpR;
            bool have2Ranges = false;

            if (validL && validR)
            {
                rangeOpL = rangeL;
                rangeOpR = rangeR;
                have2Ranges = true;
            }
            else if (validL)
            {
                rangeOpL = rangeL;
                GVN::Value * gvnR;
                if (resR.getConstant().getGVNValue(getGVN(), gvnR))
                {
                    rangeOpR.set(getGVN(), gvnR, gvnR);
                    have2Ranges = true;
                }
            }
            else if (validR)
            {
                rangeOpR = rangeR;
                GVN::Value * gvnL;
                if (resL.getConstant().getGVNValue(getGVN(), gvnL))
                {
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
		    Result & res = oe.getDecorator().setResult(typ);
		    res.getRange() = rangeOpL + rangeOpR;
		    setResult(res);
		    return true;
		}
		case ast::OpExp::minus:
		{
		    Result & res = oe.getDecorator().setResult(typ);
		    res.getRange() = rangeOpL - rangeOpR;
		    setResult(res);
		    return true;
		}
		case ast::OpExp::times:
		{
		    Result & res = oe.getDecorator().setResult(typ);
		    res.getRange() = rangeOpL * rangeOpR;
		    setResult(res);
		    return true;
		}
		case ast::OpExp::rdivide:
		{
		    Result & res = oe.getDecorator().setResult(typ);
		    res.getRange() = rangeOpL / rangeOpR;
		    setResult(res);
		    return true;
		}
		}
            }
        }

	return false;
    }
}
