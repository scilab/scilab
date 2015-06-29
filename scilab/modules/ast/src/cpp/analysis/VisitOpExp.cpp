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
	bool safe;

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
		    resT = checkEWBinOp<check_____add____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::minus:
                {
		    resT = checkEWBinOp<check_____sub____>(LT, RT, LR, RR, safe, tempId);
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
			    safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
		    else
		    {
			safe = true;
		    }

		    tempId = dm.getTmpId(resT, false);
		    dm.releaseTmp(LR.getTempId());
		    dm.releaseTmp(RR.getTempId());
		    
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
			    safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
		    else
		    {
			safe = true;
		    }
		    
		    tempId = dm.getTmpId(resT, false);
		    dm.releaseTmp(LR.getTempId());
		    dm.releaseTmp(RR.getTempId());
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
			    safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
		    else
		    {
			safe = true;
		    }
		    
		    tempId = dm.getTmpId(resT, false);
		    dm.releaseTmp(LR.getTempId());
		    dm.releaseTmp(RR.getTempId());
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
			    safe = true;
                        }
                        else
                        {
                            resT = resT.asUnknownMatrix();
                        }
                    }
		    else
		    {
			safe = true;
		    }
		    
		    tempId = dm.getTmpId(resT, false);
		    dm.releaseTmp(LR.getTempId());
		    dm.releaseTmp(RR.getTempId());
                    break;
                }
                case ast::OpExp::dottimes :
                {
		    resT = checkEWBinOp<check_____dottimes____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::dotrdivide:
                {
		    resT = checkEWBinOp<check_____dotrdiv____>(LT, RT, LR, RR, safe, tempId);
		    break;
                }
                case ast::OpExp::dotpower:
                {
		    resT = checkEWBinOp<check_____dotpower____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::unaryMinus :
                {
                    resT = check_____unaryminus____(getGVN(), RT);
		    if (!resT.hasInvalidDims())
		    {
			safe = true;
		    }
		    tempId = RR.getTempId();
                    break;
                }
                case ast::OpExp::krontimes :
                {
                    resT = check_____krontimes____(getGVN(), LT, RT);
		    if (!resT.hasInvalidDims())
		    {
			safe = true;
		    }
		    tempId = dm.getTmpId(resT, false);
		    dm.releaseTmp(LR.getTempId());
		    dm.releaseTmp(RR.getTempId());
                    break;
                }
                case ast::OpExp::eq:
                {
		    resT = checkEWBinOp<check_____eq____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::ne:
                {
		    resT = checkEWBinOp<check_____neq____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::lt:
                {
		    resT = checkEWBinOp<check_____lt____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::le:
                {
		    resT = checkEWBinOp<check_____le____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::gt:
                {
		    resT = checkEWBinOp<check_____gt____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::ge:
                {
		    resT = checkEWBinOp<check_____ge____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalAnd:
                {
		    resT = checkEWBinOp<check_____and____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalOr:
                {
		    resT = checkEWBinOp<check_____or____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalShortCutAnd:
                {
		    resT = checkEWBinOp<check_____andand____>(LT, RT, LR, RR, safe, tempId);
                    break;
                }
                case ast::OpExp::logicalShortCutOr:
                {
		    resT = checkEWBinOp<check_____oror____>(LT, RT, LR, RR, safe, tempId);
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

	e.getDecorator().safe = safe;
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
	const int tempId = LR.getTempId();
        if (LT.isknown())
        {
            TIType resT = check_____not____(getGVN(), LT);
            e.getDecorator().res = Result(resT, tempId);
	    e.getDecorator().safe = true;
        }
        else
        {
            e.getDecorator().res = Result(TIType(getGVN()), tempId);
        }
        setResult(e.getDecorator().res);
    }

    void AnalysisVisitor::visit(ast::TransposeExp & e)
    {
        logger.log(L"TransposeExp", e.getLocation());
        e.getExp().accept(*this);
        Result & res = getResult();
        const TIType & type = res.getType();
	TIType resType(dm.getGVN(), type.type, type.cols, type.rows);
	e.getDecorator().res = Result(resType, dm.getTmpId(resType, false));
	e.getDecorator().safe = true;
	dm.releaseTmp(res.getTempId());
	
        setResult(e.getDecorator().res);
    }
}
