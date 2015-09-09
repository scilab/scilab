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
#include "blocks/ShortcutEval.hxx"

namespace jit
{

    void JITVisitor::visit(const ast::WhileExp & e)
    {
	llvm::LLVMContext & context = getContext();
	llvm::IRBuilder<> & builder = getBuilder();
	llvm::BasicBlock * condBlock = llvm::BasicBlock::Create(context, "while_cond", function);
	llvm::BasicBlock * trueBlock = llvm::BasicBlock::Create(context, "while_loop", function);
	llvm::BasicBlock * falseBlock = llvm::BasicBlock::Create(context, "while_after", function);
	blocks.emplace(condBlock, falseBlock);

	CreateBr(condBlock);
	builder.SetInsertPoint(condBlock);
	ShortcutEval se(*this, trueBlock, falseBlock);
	e.getTest().accept(se);

	JITScilabPtr & L = se.getResult();
	if (L.get())
	{
	    llvm::Value * v = builder.CreateTrunc(L->loadData(*this), getTy<bool>());
	    builder.CreateCondBr(v, trueBlock, falseBlock);
	}

	builder.SetInsertPoint(trueBlock);
	e.getBody().accept(*this);
	CreateBr(condBlock);
	builder.SetInsertPoint(falseBlock);

	blocks.pop();
    }
    
}
