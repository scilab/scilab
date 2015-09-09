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
#include "calls/JITUnaryOpCall.hxx"
#include "Cast.hxx"

namespace jit
{

    bool JITUnaryOpCall::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
        const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
        const analysis::TIType & Ltype = oe.getRight().getDecorator().getResult().getType();

        oe.getRight().accept(jit);
        JITScilabPtr L = jit.getResult();

	if (Ltype.isscalar())
	{
	    if (out.empty())
	    {
		out.emplace_back(S(L, Ltype, typesOut.front(), jit));
	    }
	    else
	    {
		out.front()->storeData(jit, S(L, Ltype, typesOut.front(), jit)->loadData(jit));
	    }
	}
	else
	{
	    if (out.empty())
	    {
		out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
	    }
	    M(L, Ltype, out.front(), typesOut.front(), jit);
	}

	return true;
    }

    void JITUnaryOpCall::M(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
    {
	llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Type * int64_ty = jit.getTy<int64_t>();
	llvm::Value * rows = L->loadRows(jit);
	llvm::Value * cols = L->loadCols(jit);
        llvm::Type * types[] = { jit.getTy(Ltype, 1), int64_ty, int64_ty, jit.getTy(Otype, 2) };
	llvm::Value * args[] = { L->loadData(jit), rows, cols, O->getData(jit)};
        llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(scilabName, Ltype), llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(types), false)));
	llvm::AttrBuilder attrBuilder;
	attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
	llvm::AttributeSet attrSet = llvm::AttributeSet::get(context, 1, attrBuilder).addAttributes(context, 4, llvm::AttributeSet::get(context, 4, attrBuilder));
	toCall->setAttributes(attrSet);
        builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
	O->storeRows(jit, rows);
	O->storeCols(jit, cols);
    }
    
}
