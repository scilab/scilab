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

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITBinOpCall.hxx"

namespace jit
{

    void JITVisitor::visit(const ast::LogicalOpExp & e)
    {
	visit(static_cast<const ast::OpExp &>(e));
    }

    void JITVisitor::visit(const ast::NotExp & e)
    {
	std::vector<analysis::TIType> typesOut(1, e.getDecorator().getResult().getType());
	std::vector<JITScilabPtr> out;
	const bool ret = negation.invoke(e, typesOut, out, *this);

	if (ret)
	{
	    setResult(out.front());
	}
    }

    void JITVisitor::visit(const ast::OpExp & e)
    {
	std::vector<analysis::TIType> typesOut(1, e.getDecorator().getResult().getType());
	std::vector<JITScilabPtr> out;
	bool ret;
	switch (e.getOper())
        {
            case ast::OpExp::plus :
            {
		ret = addition.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::unaryMinus :
            {
		ret = opposite.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::minus :
            {
		ret = subtraction.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::times:
            {
		ret = multiplication.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::ldivide:
            {
                break;
            }
            case ast::OpExp::dotldivide :
            {
                break;
            }
            case ast::OpExp::rdivide:
            {
		ret = rdivision.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::dotrdivide :
            {
                break;
            }
            case ast::OpExp::dottimes :
            {
		ret = multiplication.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::dotpower :
            {
                break;
            }
            case ast::OpExp::eq :
            {
		ret = equality.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::ne :
            {
		ret = not_equality.invoke(e, typesOut, out, *this);
		break;
            }
            case ast::OpExp::lt :
            {
		ret = lower_than.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::le :
            {
		ret = lower_or_eq.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::gt :
            {
		ret = greater_than.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::ge :
            {
		ret = greater_or_eq.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::power :
            {
		ret = power.invoke(e, typesOut, out, *this);
                break;
            }
            case ast::OpExp::krontimes :
            {
                break;
            }
            case ast::OpExp::kronrdivide :
            {
                break;
            }
            case ast::OpExp::kronldivide :
            {
                break;
            }
            default :
                break;
        }

	if (ret)
	{
	    setResult(out.front());
	}
    }
    
}
