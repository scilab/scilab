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
#include "blocks/ShortcutEval.hxx"

namespace jit
{

    void ShortcutEval::visit(const ast::OpExp & e)
    {
	switch (e.getOper())
        {
        case ast::OpExp::eq:
        {
	    std::vector<analysis::TIType> typesOut;
	    std::vector<JITScilabPtr> out;
	    jit.shortcut_eq.invoke(e, typesOut, out, jit);
	    setResult(out.front());
	    
            break;
        }
        case ast::OpExp::ne:
        {
	    std::vector<analysis::TIType> typesOut;
	    std::vector<JITScilabPtr> out;
	    jit.shortcut_ne.invoke(e, typesOut, out, jit);
	    setResult(out.front());
	    
            break;
        }
        case ast::OpExp::lt:
        {
	    std::vector<analysis::TIType> typesOut;
	    std::vector<JITScilabPtr> out;
	    jit.shortcut_lt.invoke(e, typesOut, out, jit);
	    setResult(out.front());
	    
            break;
        }
        case ast::OpExp::le:
        {
	    std::vector<analysis::TIType> typesOut;
	    std::vector<JITScilabPtr> out;
	    jit.shortcut_le.invoke(e, typesOut, out, jit);
	    setResult(out.front());
	    
            break;
        }
        case ast::OpExp::gt:
        {
	    std::vector<analysis::TIType> typesOut;
	    std::vector<JITScilabPtr> out;
	    jit.shortcut_gt.invoke(e, typesOut, out, jit);
	    setResult(out.front());
	    
            break;
        }
        case ast::OpExp::ge:
        {
	    std::vector<analysis::TIType> typesOut;
	    std::vector<JITScilabPtr> out;
	    jit.shortcut_ge.invoke(e, typesOut, out, jit);
	    setResult(out.front());
	    
            break;
        }
        case ast::OpExp::logicalShortCutAnd:
        case ast::OpExp::logicalAnd:
        {
	    llvm::Value * v;
	    llvm::LLVMContext & context = jit.getContext();
	    llvm::IRBuilder<> & builder = jit.getBuilder();
	    llvm::BasicBlock * saveTrue = trueBlock;
	    trueBlock = llvm::BasicBlock::Create(context, "", &jit.getFunction());
	    e.getLeft().accept(*this);
	    JITScilabPtr & L = getResult();
	    if (L.get())
	    {
		v = builder.CreateTrunc(L->loadData(jit), jit.getTy<bool>());
		builder.CreateCondBr(v, trueBlock, falseBlock);
	    }
		
	    builder.SetInsertPoint(trueBlock);
	    trueBlock = saveTrue;
	    e.getRight().accept(*this);
	    JITScilabPtr & R = getResult();
	    if (R.get())
	    {
		v = builder.CreateTrunc(R->loadData(jit), jit.getTy<bool>());
		builder.CreateCondBr(v, trueBlock, falseBlock);
		setResult(nullptr);
	    }
	    
            break;
        }
        case ast::OpExp::logicalShortCutOr:
        case ast::OpExp::logicalOr:
        {
	    llvm::Value * v;
	    llvm::LLVMContext & context = jit.getContext();
	    llvm::IRBuilder<> & builder = jit.getBuilder();
	    llvm::BasicBlock * saveFalse = falseBlock;
	    falseBlock = llvm::BasicBlock::Create(context, "", &jit.getFunction());
	    e.getLeft().accept(*this);
	    JITScilabPtr & L = getResult();
	    if (L.get())
	    {
		v = builder.CreateTrunc(L->loadData(jit), jit.getTy<bool>());
		builder.CreateCondBr(v, trueBlock, falseBlock);
	    }
		
	    builder.SetInsertPoint(falseBlock);
	    falseBlock = saveFalse;
	    e.getRight().accept(*this);
	    JITScilabPtr & R = getResult();
	    if (R.get())
	    {
		v = builder.CreateTrunc(R->loadData(jit), jit.getTy<bool>());
		builder.CreateCondBr(v, trueBlock, falseBlock);
		setResult(nullptr);
	    }

            break;
        }
	default:
	{
	    e.accept(jit);
	    setResult(toBool(getResult(), e.getDecorator().getResult().getType()));
	    break;
	}
        }
    }

    void ShortcutEval::visit(const ast::SimpleVar & e)
    {
	defaultVisit(e);
    }

    void ShortcutEval::visit(const ast::NotExp & e)
    {
	defaultVisit(e);
    }
    
    void ShortcutEval::visit(const ast::TransposeExp & e)
    {
	defaultVisit(e);
    }

    void ShortcutEval::visit(const ast::MatrixExp & e)
    {
	defaultVisit(e);
    }

    void ShortcutEval::visit(const ast::CellExp & e)
    {
	defaultVisit(e);
    }
    
    void ShortcutEval::visit(const ast::CallExp & e)
    {
	defaultVisit(e);
    }

    void ShortcutEval::visit(const ast::DoubleExp & e)
    {
	setResult(std::shared_ptr<JITScilabVal>(new JITScalBool(jit, (int32_t)e.getConstant()->isTrue(), false, "")));
    }

    void ShortcutEval::visit(const ast::BoolExp & e)
    {
	setResult(std::shared_ptr<JITScilabVal>(new JITScalBool(jit, (int32_t)e.getConstant()->isTrue(), false, "")));
    }
    
    void ShortcutEval::visit(const ast::LogicalOpExp & e)
    {
        visit(static_cast<const ast::OpExp &>(e));
    }

    JITScilabPtr ShortcutEval::toBool(JITScilabPtr & L, const analysis::TIType & Ltype)
    {
	llvm::Value * ret;
	if (Ltype.isscalar())
	{
	    ret = toBoolS(jit.getResult(), Ltype);
	}
	else
	{
	    ret = toBoolM(jit.getResult(), Ltype);
	}
	
	return std::shared_ptr<JITScilabVal>(new JITScalBool(ret));
    }
    
    llvm::Value * ShortcutEval::toBoolS(JITScilabPtr & L, const analysis::TIType & Ltype)
    {
	if (!Ltype.isfloating())
	{
	    // we have an integer type
	    return jit.getBuilder().CreateICmpNE(L->loadData(jit), llvm::ConstantInt::get(jit.getTy(Ltype), 0));
	}
	else if (!Ltype.iscomplex())
	{
	    // we have a double
	    return jit.getBuilder().CreateFCmpUNE(L->loadData(jit), jit.getConstant<double>(0));
	}
    }
    
    llvm::Value * ShortcutEval::toBoolM(JITScilabPtr & L, const analysis::TIType & Ltype)
    {
	llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Type * int64_ty = jit.getTy<int64_t>();
	llvm::Value * rows = L->loadRows(jit);
	llvm::Value * cols = L->loadCols(jit);
        llvm::Type * types[] = { jit.getTy(Ltype, 1), int64_ty, int64_ty };
	llvm::Value * args[] = { L->loadData(jit), rows, cols};
        llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling("toBoolean", Ltype), llvm::FunctionType::get(jit.getTy<int32_t>(), llvm::ArrayRef<llvm::Type *>(types), false)));
	llvm::AttrBuilder attrBuilder;
	attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
	llvm::AttributeSet attrSet = llvm::AttributeSet::get(jit.getContext(), 1, attrBuilder);
	toCall->setAttributes(attrSet);
	return builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
    }
    
} // namespace jit
