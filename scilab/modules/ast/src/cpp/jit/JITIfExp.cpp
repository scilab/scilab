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

    void JITVisitor::visit(const ast::IfExp & e)
    {
	llvm::LLVMContext & context = getContext();
	llvm::IRBuilder<> & builder = getBuilder();
	llvm::BasicBlock * trueBlock = llvm::BasicBlock::Create(context, "if_then", function);
	llvm::BasicBlock * falseBlock = llvm::BasicBlock::Create(context, "if_else", function);
	llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "if_after", function);
	
	ShortcutEval se(*this, trueBlock, falseBlock);
	e.getTest().accept(se);

	JITScilabPtr & L = se.getResult();
	if (L.get())
	{
	    llvm::Value * v = builder.CreateTrunc(L->loadData(*this), getTy<bool>());
	    builder.CreateCondBr(v, trueBlock, falseBlock);
	}

	builder.SetInsertPoint(trueBlock);
	e.getThen().accept(*this);
	CreateBr(after);

	builder.SetInsertPoint(falseBlock);
	e.getElse().accept(*this);
	CreateBr(after);

	builder.SetInsertPoint(after);
    }
    
}
