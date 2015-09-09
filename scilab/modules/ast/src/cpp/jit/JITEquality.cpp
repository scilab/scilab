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
#include "calls/JITBinOpCall.hxx"
#include "Cast.hxx"

namespace jit
{

    JITScilabPtr JITEquality::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit)
    {
        llvm::Value * ret = nullptr;
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::LLVMContext & context = jit.getContext();
	llvm::Function & function = jit.getFunction();
	
        // Integral types
        if (!Ltype.isfloating() || !Rtype.isfloating())
        {
	    llvm::Value * l = L->loadData(jit);
	    llvm::Value * r = R->loadData(jit);
	    Cast::promoteScilabValue(Ltype.issigned(), l, Rtype.issigned(), r, builder);
	    ret = builder.CreateICmpEQ(l, r);
        }
        else if (Ltype.isfloating() && Rtype.isfloating())
        {
	    if (Ltype.type == analysis::TIType::DOUBLE)
	    {
		llvm::Value * l = L->loadData(jit);
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // double == double
		    llvm::Value * r = R->loadData(jit);
		    llvm::Type * int64_ty = jit.getTy<int64_t>();
		    if (l->getType() == int64_ty && r->getType() == int64_ty)
		    {
			ret = builder.CreateICmpEQ(l, r);
		    }
		    else
		    {
			Cast::castInt64(l, jit);
			Cast::castInt64(r, jit);
			ret = builder.CreateFCmpOEQ(l, r);
		    }   
		}
		else
		{
		    // double == complex
		    // if l is an int64, then it needs to be casted into a double
		    Cast::castInt64(l, jit);
		    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);
		    llvm::Value * r_im = R->loadImag(jit);
		    llvm::Value * cmp_r_im_0 = builder.CreateFCmpOEQ(r_im, jit.getConstant<double>(0)); // R.im == 0
		    builder.CreateCondBr(cmp_r_im_0, bb1, after);

		    builder.SetInsertPoint(bb1);
		    llvm::Value * r_re = R->loadReal(jit);
		    llvm::Value * cmp_l_r_re = builder.CreateFCmpOEQ(l, r_re); // L == R.re
		    builder.CreateBr(after);

		    builder.SetInsertPoint(after);
		    llvm::PHINode * res = builder.CreatePHI(jit.getTy<bool>(), 2);
		    res->addIncoming(jit.getBool(false), cur_block);
		    res->addIncoming(cmp_l_r_re, bb1);
		    
		    ret = res;
		}
	    }
	    else
	    {
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // complex == double
		    // if l is an int64, then it needs to be casted into a double
		    llvm::Value * r = R->loadData(jit);
		    Cast::castInt64(r, jit);
		    
		    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);
		    llvm::Value * l_im = L->loadImag(jit);
		    llvm::Value * cmp_l_im_0 = builder.CreateFCmpOEQ(l_im, jit.getConstant<double>(0)); // L.im == 0
		    builder.CreateCondBr(cmp_l_im_0, bb1, after);

		    builder.SetInsertPoint(bb1);
		    llvm::Value * l_re = L->loadReal(jit);
		    llvm::Value * cmp_l_re_r = builder.CreateFCmpOEQ(l_re, r); // L.re == R
		    builder.CreateBr(after);

		    builder.SetInsertPoint(after);
		    llvm::PHINode * res = builder.CreatePHI(jit.getTy<bool>(), 2);
		    res->addIncoming(jit.getBool(false), cur_block);
		    res->addIncoming(cmp_l_re_r, bb1);
		    
		    ret = res;
		}
		else
		{
		    // complex == complex
		    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);
		    llvm::Value * l_re = L->loadReal(jit);
		    llvm::Value * r_re = R->loadReal(jit);
		    llvm::Value * cmp_re = builder.CreateFCmpOEQ(l_re, r_re); // L.re == R.re
		    builder.CreateCondBr(cmp_re, bb1, after);

		    builder.SetInsertPoint(bb1);
		    llvm::Value * l_im = L->loadImag(jit);
		    llvm::Value * r_im = R->loadImag(jit);
		    llvm::Value * cmp_im = builder.CreateFCmpOEQ(l_im, r_im); // L.im == R.im
		    builder.CreateBr(after);

		    builder.SetInsertPoint(after);
		    llvm::PHINode * res = builder.CreatePHI(jit.getTy<bool>(), 2);
		    res->addIncoming(jit.getBool(false), cur_block);
		    res->addIncoming(cmp_im, bb1);
		    
		    ret = res;
		}
	    }
	}

	Cast::castInt1(ret, jit);
        return jit.getScalar(ret, Otype.type);
    }
    
}
