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

    JITScilabPtr JITComparison::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit)
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
	    if (!Ltype.issigned() || Rtype.issigned())
	    {
		ret = cmpIU(builder, l, r);
	    }
	    else
	    {
		ret = cmpIS(builder, l, r);
	    }
        }
        else if (Ltype.isfloating() && Rtype.isfloating())
        {
	    if (Ltype.type == analysis::TIType::DOUBLE)
	    {
		llvm::Value * l = L->loadData(jit);
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // double < double
		    llvm::Value * r = R->loadData(jit);
		    llvm::Type * int64_ty = jit.getTy<int64_t>();
		    if (l->getType() == int64_ty && r->getType() == int64_ty)
		    {
			ret = cmpIS(builder, l, r);
		    }
		    else
		    {
			Cast::castInt64(l, jit);
			Cast::castInt64(r, jit);
			ret = cmpD(builder, l, r);
		    }   
		}
		else
		{
		    // double < complex
		    // TODO: R.im != 0 => error (must call d)
		    Cast::castInt64(l, jit);
		    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);
		    llvm::Value * r_im = R->loadImag(jit);
		    llvm::Value * cmp_r_im_0 = builder.CreateFCmpOEQ(r_im, jit.getConstant<double>(0)); // R.im == 0
		    builder.CreateCondBr(cmp_r_im_0, bb1, after);

		    builder.SetInsertPoint(bb1);
		    llvm::Value * r_re = R->loadReal(jit);
		    llvm::Value * cmp_l_r_re = cmpD(builder, l, r_re); // L < R.re
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
		    // complex < double
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
		    llvm::Value * cmp_l_re_r = cmpD(builder, l_re, r); // L.re < R
		    builder.CreateBr(after);

		    builder.SetInsertPoint(after);
		    llvm::PHINode * res = builder.CreatePHI(jit.getTy<bool>(), 2);
		    res->addIncoming(jit.getBool(false), cur_block);
		    res->addIncoming(cmp_l_re_r, bb1);
		    
		    ret = res;
		}
		else
		{
		    // complex < complex
		    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
		    llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(context, "", &function);
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);
		    llvm::Value * zero = jit.getConstant<double>(0);
		    llvm::Value * l_im = L->loadImag(jit);
		    llvm::Value * cmp = builder.CreateFCmpOEQ(l_im, zero);
		    builder.CreateCondBr(cmp, bb1, after);

		    builder.SetInsertPoint(bb1);
		    llvm::Value * r_im = R->loadImag(jit);
		    cmp = builder.CreateFCmpOEQ(r_im, zero);
		    builder.CreateCondBr(cmp, bb2, after);
		    
		    builder.SetInsertPoint(bb2);
		    llvm::Value * l_re = L->loadReal(jit);
		    llvm::Value * r_re = R->loadReal(jit);
		    llvm::Value * cmp_re = cmpD(builder, l_re, r_re); // L.re < R.re
		    builder.CreateBr(after);

		    builder.SetInsertPoint(after);
		    llvm::PHINode * res = builder.CreatePHI(jit.getTy<bool>(), 3);
		    llvm::Value * _false = jit.getBool(false);
		    res->addIncoming(_false, cur_block);
		    res->addIncoming(_false, bb1);
		    res->addIncoming(cmp_re, bb2);
		    
		    ret = res;
		}
	    }
	}

	Cast::castInt1(ret, jit);
        return jit.getScalar(ret, Otype.type);
    }

    llvm::Value * JITLowerThan::cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpULT(l, r);
    }

    llvm::Value * JITLowerThan::cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpSLT(l, r);
    }

    llvm::Value * JITLowerThan::cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateFCmpOLT(l, r);
    }    

    llvm::Value * JITGreaterThan::cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpUGT(l, r);
    }

    llvm::Value * JITGreaterThan::cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpSGT(l, r);
    }

    llvm::Value * JITGreaterThan::cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateFCmpOGT(l, r);
    }
    
    llvm::Value * JITLowerOrEq::cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpULE(l, r);
    }
    
    llvm::Value * JITLowerOrEq::cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpSLE(l, r);
    }

    llvm::Value * JITLowerOrEq::cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateFCmpOLE(l, r);
    }

    llvm::Value * JITGreaterOrEq::cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpUGE(l, r);
    }

    llvm::Value * JITGreaterOrEq::cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateICmpSGE(l, r);
    }

    llvm::Value * JITGreaterOrEq::cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r)
    {
	return builder.CreateFCmpOGE(l, r);
    }
    
}
