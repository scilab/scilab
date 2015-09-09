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

namespace jit
{

    void JITVisitor::visit(const ast::SelectExp & e)
    {
	// In the general case, a select is just a succession of if ... elseif... elseif... else... end
	// In few particular cases it could be optimized into a switch (cf JITIntSelect.cpp)
	
	llvm::LLVMContext & context = getContext();
	llvm::IRBuilder<> & builder = getBuilder();
	llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "sel_after", function);
	llvm::BasicBlock * defaultCase;
	if (e.hasDefault())
	{
	    defaultCase = llvm::BasicBlock::Create(context, "sel_default", function);
	}
	else
	{
	    defaultCase = after;
	}
	
	const analysis::TIType & Ltype = e.getSelect()->getDecorator().getResult().getType();
	e.getSelect()->accept(*this);
	JITScilabPtr select_res = getResult();
	ast::exps_t cases = e.getCases();
	for (ast::exps_t::const_iterator i = cases.begin(), end = cases.end(); i != end; ++i)
	{
	    const ast::Exp * c = *i;
	    const ast::CaseExp * ce = static_cast<const ast::CaseExp *>(c);
	    const ast::Exp * test = ce->getTest();
	    const analysis::TIType & Rtype = test->getDecorator().getResult().getType();
	    test->accept(*this);
	    JITScilabPtr & case_res = getResult(); 
	    const uint8_t code = (Ltype.isscalar() ? 1 : 0) * 2 + (Rtype.isscalar() ? 1 : 0);
	    JITScilabPtr test_res;
	    
	    switch (code)
	    {
	    case 0:
	    {
		test_res = shortcut_eq.MM(select_res, Ltype, case_res, Rtype, *this);
		break;
	    }
	    case 1:
	    {
		test_res = shortcut_eq.MS(select_res, Ltype, case_res, Rtype, *this);
		break;
	    }
	    case 2:
	    {
		test_res = shortcut_eq.SM(select_res, Ltype, case_res, Rtype, *this);
		break;
	    }
	    default:
	    {
		test_res = shortcut_eq.SS(select_res, Ltype, case_res, Rtype, *this);
		break;
	    }
	    }

	    llvm::BasicBlock * trueBlock = llvm::BasicBlock::Create(context, "", function);
	    llvm::BasicBlock * falseBlock;
	    if (std::next(i) == end)
	    {
		falseBlock = defaultCase;
	    }
	    else
	    {
		falseBlock = llvm::BasicBlock::Create(context, "", function);
	    }

	    llvm::Value * v = builder.CreateTrunc(test_res->loadData(*this), getTy<bool>());
	    builder.CreateCondBr(v, trueBlock, falseBlock);

	    builder.SetInsertPoint(trueBlock);
	    ce->getBody()->accept(*this);
	    builder.CreateBr(after);

	    builder.SetInsertPoint(falseBlock);
	}

	// At this point we are in the default case
	if (e.hasDefault())
	{
	    e.getDefaultCase()->accept(*this);
	    builder.CreateBr(after);
	}

	builder.SetInsertPoint(after);
    }

}
