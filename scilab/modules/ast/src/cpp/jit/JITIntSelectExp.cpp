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

#include <unordered_map>

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"

namespace jit
{

    void JITVisitor::visit(const ast::IntSelectExp & e)
    {
	switch (e.getType())
	{
	case ast::Exp::TABLEINTSELECTEXP:
	{
	    visit(static_cast<const ast::TableIntSelectExp &>(e));
	    break;
	}
	case ast::Exp::SMALLINTSELECTEXP:
	{
	    visit(static_cast<const ast::SmallIntSelectExp &>(e));
	    break;
	}
	case ast::Exp::MAPINTSELECTEXP:
	{
	    visit(static_cast<const ast::MapIntSelectExp &>(e));
	    break;
	}
	}
    }

    void JITVisitor::visit(const ast::SmallIntSelectExp & e)
    {
	std::map<int64_t, ast::Exp *> map;
	const ast::exps_t & exps = e.getExps();
	const std::vector<int64_t> & table = e.getTable();
	const std::size_t size = table.size();
	for (std::size_t i = 0; i < size; ++i)
	{
	    map.emplace(table[i], exps[i + 1]);
	}
	makeSwitch(e, map);
    }

    void JITVisitor::visit(const ast::TableIntSelectExp & e)
    {
	std::map<int64_t, ast::Exp *> map;
	const int64_t min = e.getMin();
	const std::vector<ast::Exp *> & table = e.getTable();
	int64_t i = 0;
	
	for (const auto exp : table)
	{
	    if (exp)
	    {
		map.emplace(i + min, exp);
	    }
	    ++i;
	}
	makeSwitch(e, map);
    }

    void JITVisitor::visit(const ast::MapIntSelectExp & e)
    {
	std::map<int64_t, ast::Exp *> map;
	const std::unordered_map<int64_t, ast::Exp *> & table = e.getTable();
	for (const auto & p : table)
	{
	    map.emplace(p.first, p.second);
	}
	makeSwitch(e, map);
    }
    
    void JITVisitor::makeSwitch(const ast::IntSelectExp & e, const std::map<int64_t, ast::Exp *> & map)
    {
	llvm::LLVMContext & context = getContext();
	llvm::IRBuilder<> & builder = getBuilder();
	llvm::BasicBlock * swBlock = llvm::BasicBlock::Create(context, "sw_block", function);
	llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "sw_after", function);
	llvm::BasicBlock * defaultCase;
	llvm::Value * selectValue = nullptr;
	
	if (e.hasDefault())
	{
	    defaultCase = llvm::BasicBlock::Create(context, "sw_default", function);
	}
	else
	{
	    defaultCase = after;
	}
	
	std::vector<llvm::BasicBlock *> blocks;
	blocks.reserve(map.size());
	
	for (const auto & p : map)
	{
	    blocks.push_back(llvm::BasicBlock::Create(context, std::string("sw") + std::to_string(p.first), function));
	}

	const analysis::TIType & selectType = e.getSelect()->getDecorator().getResult().getType();
	e.getSelect()->accept(*this);
	JITScilabPtr & res = getResult();
	if (selectType.isscalar())
	{
	    if (selectType.type == analysis::TIType::DOUBLE)
	    {
		llvm::Value * res_dbl = res->loadData(*this);
		llvm::Value * res_i64;
		if (res_dbl->getType() == getTy<int64_t>())
		{
		    selectValue = res_dbl;
		    builder.CreateBr(swBlock);
		}
		else
		{
		    llvm::Value * res_i64 = Cast::cast<int64_t>(res_dbl, true, *this);
		    llvm::Value * res_i64_dbl = Cast::cast<double>(res_i64, true, *this);
		    llvm::Value * cmp_i1 = builder.CreateFCmpOEQ(res_dbl, res_i64_dbl);
		    builder.CreateCondBr(cmp_i1, swBlock, defaultCase);
		    selectValue = res_i64;
		}
	    }
	    else if (selectType.isintegral())
	    {
		// TODO : handle uint64_t case
		selectValue = Cast::cast<int64_t>(res->loadData(*this), true, *this);
		builder.CreateBr(swBlock);
	    }
	}
	else
	{
	    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", function);
	    llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(context, "", function);
	    
	    llvm::Value * one_i64 = getConstant<int64_t>(1);
	    llvm::Value * zero_i64 = getConstant<int64_t>(0);
	    llvm::Value * rows_i64 = res->loadRows(*this);
	    llvm::Value * cmp_i1 = builder.CreateICmpEQ(rows_i64, one_i64);
	    builder.CreateCondBr(cmp_i1, bb1, defaultCase);
	    
	    builder.SetInsertPoint(bb1);
	    llvm::Value * cols_i64 = res->loadCols(*this);
	    cmp_i1 = builder.CreateICmpEQ(cols_i64, one_i64);
	    builder.CreateCondBr(cmp_i1, bb2, defaultCase);
	    
	    builder.SetInsertPoint(bb2);
	    llvm::Value * res_ptr = res->loadData(*this);
	    if (selectType.type == analysis::TIType::DOUBLE)
	    {
		llvm::Value * res_dbl = builder.CreateGEP(res_ptr, zero_i64);
		llvm::Value * res_i64 = Cast::cast<int64_t>(res_dbl, true, *this);
		llvm::Value * res_i64_dbl = Cast::cast<double>(res_i64, true, *this);
		selectValue = res_i64;
		cmp_i1 = builder.CreateFCmpOEQ(res_dbl, res_i64_dbl);
		builder.CreateCondBr(cmp_i1, swBlock, defaultCase);
	    }
	    else if (selectType.isintegral())
	    {
		// TODO : handle uint64_t case
		llvm::Value * res_integer = builder.CreateGEP(res_ptr, zero_i64);
		selectValue = Cast::cast<int64_t>(res_integer, true, *this);
		builder.CreateBr(swBlock);
	    }
	}

	builder.SetInsertPoint(swBlock);
	if (selectValue)
	{
	    const std::size_t size = blocks.size();
	    llvm::SwitchInst * sw = builder.CreateSwitch(selectValue, defaultCase, size);
	    std::size_t i = 0;
	    for (const auto & p : map)
	    {
		sw->addCase(static_cast<llvm::ConstantInt *>(getConstant<int64_t>(p.first)), blocks[i]);
		++i;
	    }

	    i = 0;
	    for (const auto & p : map)
	    {
		builder.SetInsertPoint(blocks[i]);
		static_cast<ast::CaseExp *>(p.second)->getBody()->accept(*this);
		builder.CreateBr(after);
		++i;
	    }

	    if (e.hasDefault())
	    {
		builder.SetInsertPoint(defaultCase);
		e.getDefaultCase()->accept(*this);
		builder.CreateBr(after);
	    }
	}

	builder.SetInsertPoint(after);
    }

}
