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
    void JITVisitor::visit(const ast::VarDec & e)
    {

    }

    void JITVisitor::visit(const ast::ForExp & e)
    {
	const ast::VarDec & vd = static_cast<const ast::VarDec &>(e.getVardec());
	if (vd.getInit().isListExp())
	{
	    const symbol::Symbol & symIterator = vd.getSymbol();
	    const ast::ListExp & le = static_cast<const ast::ListExp &>(vd.getInit());
	    if (le.getDecorator().getResult().getRange().isValid())
	    {
		// for i = start:step:end...
		// is equivalent to for (int64_t i = start; i < end + step; i += step)...
		
		le.getStart().accept(*this);
		llvm::Value * start = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
		le.getStep().accept(*this);
		llvm::Value * step = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
		le.getEnd().accept(*this);
		llvm::Value * end = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
		end = builder.CreateAdd(end, step);

		llvm::BasicBlock * cur_block = builder.GetInsertBlock();
		llvm::BasicBlock * condBlock = llvm::BasicBlock::Create(context, "for_cond", function);
		llvm::BasicBlock * loopBlock = llvm::BasicBlock::Create(context, "for_loop", function);
		llvm::BasicBlock * afterBlock = llvm::BasicBlock::Create(context, "for_after", function);

		blocks.emplace(condBlock, afterBlock);
		
		llvm::Value * cmp_i1 = builder.CreateICmpSLT(start, end);
		builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

		
		builder.SetInsertPoint(loopBlock);
		llvm::PHINode * i = builder.CreatePHI(getTy<int64_t>(), 2);
		i->addIncoming(start, cur_block);
		JITScilabPtr & it = variables.find(symIterator)->second;
		it->storeData(*this, i);

		e.getBody().accept(*this);
		builder.CreateBr(condBlock);

		builder.SetInsertPoint(condBlock);
		llvm::Value * ipstp_i64 = builder.CreateAdd(i, step);
		i->addIncoming(ipstp_i64, condBlock);
		cmp_i1 = builder.CreateICmpSLT(ipstp_i64, end);
		builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

		builder.SetInsertPoint(afterBlock);
/*
		llvm::Value * cmp_i1 = builder.CreateICmpSLT(start, end);
		builder.CreateCondBr(cmp_i1, BBBody, BBAfter);

		builder.SetInsertPoint(BBBody);
		llvm::PHINode * i = builder.CreatePHI(getTy<int64_t>(), 2);
		i->addIncoming(start, cur_block);
		JITScilabPtr & it = variables.find(symIterator)->second;
		it->storeData(*this, i);

		e.getBody().accept(*this);

		if (builder.GetInsertBlock() != BBAfter)
		{
		    BBAfter->moveAfter(builder.GetInsertBlock());
		}
		
		llvm::Value * ipstp_i64 = builder.CreateAdd(i, step);
		i->addIncoming(ipstp_i64, builder.GetInsertBlock());
		cmp_i1 = builder.CreateICmpSLT(ipstp_i64, end);
		builder.CreateCondBr(cmp_i1, BBBody, BBAfter);
		builder.SetInsertPoint(BBAfter);
*/
	    }
	    
	}

	
	// e.getBody().accept(*this);
    }

}
