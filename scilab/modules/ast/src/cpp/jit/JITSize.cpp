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


#include "call/SizeCall.hxx"

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITSize.hxx"

namespace jit
{

    bool JITSize::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
	const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
	analysis::SizeCall & sc = *static_cast<analysis::SizeCall *>(ce.getDecorator().getCall());
	const ast::exps_t args = ce.getArgs();
	const analysis::TIType & firstTy = args.front()->getDecorator().getResult().getType();
	
	switch (sc.getKind())
	{
	case analysis::SizeCall::R:
	    // we have something like size(a, "r") or size(a, 1)
	    if (firstTy.isscalar())
	    {
		if (out.empty())
		{
		    out.emplace_back(new JITScalInt64(jit.getConstant<int64_t>(1)));
		}
		else
		{
		    // we have r = size(a, "r"), so r is an int64_t
		    out.front()->storeData(jit, jit.getConstant<int64_t>(1));
		}
	    }
	    else
	    {
		args.front()->accept(jit);
		JITScilabPtr & firstRes = jit.getResult();
		if (out.empty())
		{
		    out.emplace_back(new JITScalInt64(firstRes->loadRows(jit)));
		}
		else
		{
		    out.front()->storeData(jit, firstRes->loadRows(jit));
		}
	    }
	    return true;
	case analysis::SizeCall::C:
	    // we have something like size(a, "c") or size(a, 2)
	    if (firstTy.isscalar())
	    {
		if (out.empty())
		{
		    out.emplace_back(new JITScalInt64(jit.getConstant<int64_t>(1)));
		}
		else
		{
		    // we have c = size(a, "c"), so c is an int64_t
		    out.front()->storeData(jit, jit.getConstant<int64_t>(1));
		}
	    }
	    else
	    {
		args.front()->accept(jit);
		JITScilabPtr & firstRes = jit.getResult();
		if (out.empty())
		{
		    out.emplace_back(new JITScalInt64(firstRes->loadCols(jit)));
		}
		else
		{
		    out.front()->storeData(jit, firstRes->loadCols(jit));
		}
	    }
	    return true;
	case analysis::SizeCall::RC:
	    // we have something like size(a, "*")
	    if (firstTy.isscalar())
	    {
		if (out.empty())
		{
		    out.emplace_back(new JITScalInt64(jit.getConstant<int64_t>(1)));
		}
		else
		{
		    // we have rc = size(a, "*"), so rc is an int64_t
		    out.front()->storeData(jit, jit.getConstant<int64_t>(1));
		}
	    }
	    else
	    {
		args.front()->accept(jit);
		JITScilabPtr & firstRes = jit.getResult();
		llvm::Value * r = firstRes->loadRows(jit);
		llvm::Value * c = firstRes->loadCols(jit);
		llvm::Value * rc = jit.getBuilder().CreateMul(r, c);
		
		if (out.empty())
		{
		    out.emplace_back(new JITScalInt64(rc));
		}
		else
		{
		    out.front()->storeData(jit, rc);
		}
	    }
	    return true;
	case analysis::SizeCall::R_C:
	    // we have something like [r,c] = size(a)
	    if (firstTy.isscalar())
	    {
		llvm::Value * one = jit.getConstant<int64_t>(1);
		out.front()->storeData(jit, one);
		out.back()->storeData(jit, one);
	    }
	    else
	    {
		args.front()->accept(jit);
		JITScilabPtr & firstRes = jit.getResult();
		out.front()->storeData(jit, firstRes->loadRows(jit));
		out.back()->storeData(jit, firstRes->loadCols(jit));
	    }
	    return true;
	case analysis::SizeCall::ONE:
	    if (out.empty())
	    {
		out.emplace_back(new JITScalInt64(jit.getConstant<int64_t>(1)));
	    }
	    else
	    {
		out.front()->storeData(jit, jit.getConstant<int64_t>(1));
	    }
	    return true;
	default:
	    return false;
	}
    }
}

