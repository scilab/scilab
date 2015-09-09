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
#include "calls/JITShortcutBinOpCall.hxx"
#include "Cast.hxx"

namespace jit
{

    bool JITShortcutBinOpCall::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
        const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
        const analysis::TIType & Ltype = oe.getLeft().getDecorator().getResult().getType();
        const analysis::TIType & Rtype = oe.getRight().getDecorator().getResult().getType();

        oe.getLeft().accept(jit);
        JITScilabPtr L = jit.getResult();
        oe.getRight().accept(jit);
        JITScilabPtr & R = jit.getResult();
	const uint8_t code = (Ltype.isscalar() ? 1 : 0) * 2 + (Rtype.isscalar() ? 1 : 0);

	switch (code)
	{
	case 0:
	{
	    out.emplace_back(MM(L, Ltype, R, Rtype, jit));
	    break;
	}
	case 1:
	{
	    out.emplace_back(MS(L, Ltype, R, Rtype, jit));
	    break;
	}
	case 2:
	{
	    out.emplace_back(SM(L, Ltype, R, Rtype, jit));
	    break;
	}
	default:
	{
	    out.emplace_back(SS(L, Ltype, R, Rtype, jit));
	    break;
	}
	}

        return true;
    }

    JITScilabPtr JITShortcutBinOpCall::SM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
        llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::Type * int64_ty = jit.getTy<int64_t>();
        llvm::Value * rows = R->loadRows(jit);
        llvm::Value * cols = R->loadCols(jit);
        llvm::Type * types[] = { jit.getTy(Ltype), jit.getTy(Rtype, 1), int64_ty, int64_ty };
        llvm::Value * args[] = { L->loadData(jit), R->loadData(jit), rows, cols};
	// TODO: we must put this function into a cache (map: name->llvm::Function*)
        llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<int32_t>(), llvm::ArrayRef<llvm::Type *>(types), false)));

        llvm::AttrBuilder attrBuilder;
        attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
        llvm::AttributeSet attrSet = llvm::AttributeSet::get(jit.getContext(), 2, attrBuilder);
        toCall->setAttributes(attrSet);
	llvm::Value * ret = builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));

	return std::shared_ptr<JITScilabVal>(new JITScalBool(ret));
    }

    JITScilabPtr JITShortcutBinOpCall::MS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
        llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::Type * int64_ty = jit.getTy<int64_t>();
        llvm::Value * rows = L->loadRows(jit);
        llvm::Value * cols = L->loadCols(jit);
        llvm::Type * types[] = { jit.getTy(Ltype, 1), int64_ty, int64_ty, jit.getTy(Rtype) };
        llvm::Value * args[] = { L->loadData(jit), rows, cols, R->loadData(jit)};
        llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<int32_t>(), llvm::ArrayRef<llvm::Type *>(types), false)));

	llvm::AttrBuilder attrBuilder;
        attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
        llvm::AttributeSet attrSet = llvm::AttributeSet::get(jit.getContext(), 1, attrBuilder);
        toCall->setAttributes(attrSet);
	llvm::Value * ret = builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));

	return std::shared_ptr<JITScilabVal>(new JITScalBool(ret));
    }

    JITScilabPtr JITShortcutBinOpCall::MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit)
    {
        llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::Type * int64_ty = jit.getTy<int64_t>();
        llvm::Value * rows = L->loadRows(jit);
        llvm::Value * cols = L->loadCols(jit);
        llvm::Type * types[] = { jit.getTy(Ltype, 1), int64_ty, int64_ty, jit.getTy(Rtype, 1), int64_ty, int64_ty };
        llvm::Value * args[] = { L->loadData(jit), rows, cols, R->loadData(jit), rows, cols};
        llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<int32_t>(), llvm::ArrayRef<llvm::Type *>(types), false)));
        llvm::AttrBuilder attrBuilder;
        attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
        llvm::AttributeSet attrSet = llvm::AttributeSet::get(jit.getContext(), 1, attrBuilder).addAttributes(jit.getContext(), 4, llvm::AttributeSet::get(jit.getContext(), 4, attrBuilder));
        toCall->setAttributes(attrSet);
	llvm::Value * ret = builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));

	return std::shared_ptr<JITScilabVal>(new JITScalBool(ret));
    }
}
