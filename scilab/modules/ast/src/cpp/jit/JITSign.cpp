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

#include "tools.hxx"
#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITSign.hxx"

namespace jit
{

    void JITSign::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        if (argType.type == analysis::TIType::DOUBLE)
        {
	    S_d(argType, arg, OType, out, jit);
        }
	else if (argType.type == analysis::TIType::COMPLEX)
	{
	    S_c(argType, arg, OType, out, jit);
	}
        else
        {
            JITOptimizedCall1::S(argType, arg, OType, out, jit);
        }
    }

    void JITSign::S_d(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::LLVMContext & context = jit.getContext();
        llvm::Function & function = jit.getFunction();
        llvm::Value * _arg = arg->loadData(jit);
        llvm::Value * zero = jit.getConstant<double>(0);
        Cast::castInt64(_arg, jit);

        /* if (x == 0 || std::isnan(x)) { return x; } return x < 0 ? -1 : 1; */
        llvm::BasicBlock * cur_block = builder.GetInsertBlock();
        llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
        llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(context, "", &function);

        llvm::Value * cmp_zero_or_nan = builder.CreateFCmpUEQ(_arg, zero);
        builder.CreateCondBr(cmp_zero_or_nan, bb2, bb1);

        builder.SetInsertPoint(bb1);
        llvm::Value * cmp_sgn = builder.CreateFCmpOLT(_arg, zero);
        llvm::Value * sel = builder.CreateSelect(cmp_sgn, jit.getConstant<double>(-1), jit.getConstant<double>(1));
        builder.CreateBr(bb2);

        builder.SetInsertPoint(bb2);
        llvm::PHINode * res = builder.CreatePHI(jit.getTy<double>(), 2);
        res->addIncoming(_arg, cur_block);
        res->addIncoming(sel, bb1);

        if (out.get())
        {
            out->storeData(jit, res);
        }
        else
        {
            jit.getScalar(res, OType.type, false, "").swap(out);
        }
    }

    void JITSign::S_c(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::LLVMContext & context = jit.getContext();
        llvm::Function & function = jit.getFunction();
	std::pair<llvm::Value *, llvm::Value *> reim = arg->loadReIm(jit);
	llvm::Value * re = reim.first;
	llvm::Value * im = reim.second;
        llvm::Value * zero = jit.getConstant<double>(0);
        
	/* 
	   if (re == 0 && im == 0) { return (re, im); }
	   if (isnan(x) || isnan(y)) { return (NaN, NaN); }
	   return (x / sqrt(x ^ 2 + y ^ 2), y / sqrt(x ^ 2 + y ^ 2));
	*/
        llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
        llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(context, "", &function);
	llvm::BasicBlock * bb3 = llvm::BasicBlock::Create(context, "", &function);
	llvm::BasicBlock * bb4 = llvm::BasicBlock::Create(context, "", &function);
	llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);

        llvm::Value * cmp_re_zero = builder.CreateFCmpOEQ(re, zero);
	llvm::Value * cmp_im_zero = builder.CreateFCmpOEQ(im, zero);
	llvm::Value * and_cond = builder.CreateAnd(cmp_re_zero, cmp_im_zero);
	builder.CreateCondBr(and_cond, bb1, bb2);

	builder.SetInsertPoint(bb1);
	llvm::Value * res_re_1 = zero;
	llvm::Value * res_im_1 = res_re_1;
	builder.CreateBr(after);

	builder.SetInsertPoint(bb2);
	llvm::Value * or_nan = builder.CreateFCmpUNO(re, im);
	builder.CreateCondBr(or_nan, bb3, bb4);

	builder.SetInsertPoint(bb3);
	llvm::Value * res_re_2 = jit.getConstant<double>(analysis::tools::NaN());
	llvm::Value * res_im_2 = res_re_2;
	builder.CreateBr(after);

	builder.SetInsertPoint(bb4);
	llvm::Type * dbl_ty = jit.getTy<double>();
	llvm::Type * types[] = { dbl_ty, dbl_ty };
	llvm::Value * args[] = { re, im };
	llvm::FunctionType * funtype = llvm::FunctionType::get(dbl_ty, types, false);
	llvm::Value * toCall = jit.getModule().getOrInsertFunction("hypot", funtype);
	llvm::Value * ret = builder.CreateCall(toCall, args);
	llvm::Value * res_re_3 = builder.CreateFDiv(re, ret);
	llvm::Value * res_im_3 = builder.CreateFDiv(im, ret);
	builder.CreateBr(after);

	builder.SetInsertPoint(after);
	llvm::PHINode * res_re = builder.CreatePHI(dbl_ty, 3);
	res_re->addIncoming(res_re_1, bb1);
	res_re->addIncoming(res_re_2, bb3);
	res_re->addIncoming(res_re_3, bb4);
	llvm::PHINode * res_im = builder.CreatePHI(dbl_ty, 3);
	res_im->addIncoming(res_im_1, bb1);
	res_im->addIncoming(res_im_2, bb3);
	res_im->addIncoming(res_im_3, bb4);
	
        if (out.get())
        {
            out->storeReIm(jit, { res_re, res_im });
        }
        else
        {
	    out.reset(new JITScalComplex(jit, res_re, res_im, false, ""));
        }
    }

} // namespace jit
