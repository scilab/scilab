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

    bool JITNegation::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
        const ast::NotExp & ne = static_cast<const ast::NotExp &>(e);
        const analysis::TIType & Ltype = ne.getExp().getDecorator().getResult().getType();

        ne.getExp().accept(jit);
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
    
    JITScilabPtr JITNegation::S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit)
    {
        llvm::Value * ret = nullptr;
        llvm::IRBuilder<> & builder = jit.getBuilder();
	
        // Integral types
        if (!Ltype.isfloating())
        {
	    llvm::Value * l = L->loadData(jit);
	    if (Ltype.type == analysis::TIType::BOOLEAN)
	    {
		ret = builder.CreateICmpEQ(l, jit.getConstant<int32_t>(0));
		ret = builder.CreateZExt(ret, jit.getTy<int32_t>());
	    }
	    else
	    {
		ret = builder.CreateNot(l);
	    }
	}
        else if (Ltype.type == analysis::TIType::DOUBLE)
	{
	    llvm::Value * l = L->loadData(jit);
	    if (l->getType() == jit.getTy<int64_t>())
	    {
		ret = builder.CreateICmpEQ(l, jit.getConstant<int64_t>(0));
		ret = builder.CreateZExt(ret, jit.getTy<int32_t>());
	    }
	    else
	    {
		ret = builder.CreateFCmpOEQ(l, jit.getConstant<double>(0));
		ret = builder.CreateZExt(ret, jit.getTy<int32_t>());
	    }
	}
	else
	{
	    // TODO: since ~A <=> A==0, imo it is the good algo but in Scilab we just consider the real part...
	    // so see with Tonio
	    llvm::LLVMContext & context = jit.getContext();
	    llvm::Function & function = jit.getFunction();
	    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
	    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
	    llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);
	    llvm::Value * zero = jit.getConstant<double>(0);
	    llvm::Value * l_re = L->loadReal(jit);
	    llvm::Value * cmp_re = builder.CreateFCmpOEQ(l_re, zero);
	    builder.CreateCondBr(cmp_re, bb1, after);

	    builder.SetInsertPoint(bb1);
	    llvm::Value * l_im = L->loadImag(jit);
	    llvm::Value * cmp_im = builder.CreateFCmpOEQ(l_im, zero);
	    cmp_im = builder.CreateZExt(cmp_im, jit.getTy<int32_t>());
	    builder.CreateBr(after);

	    builder.SetInsertPoint(after);
	    llvm::PHINode * res = builder.CreatePHI(jit.getTy<int32_t>(), 2);
	    res->addIncoming(jit.getConstant<int32_t>(0), cur_block);
	    res->addIncoming(cmp_im, bb1);

	    ret = res;
	}

        return jit.getScalar(ret, Otype.type);
    }

}
