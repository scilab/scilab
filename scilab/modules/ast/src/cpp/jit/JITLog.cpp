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
#include "calls/JITLog.hxx"

#include "core_math.h" // For PI value

namespace jit
{

    void JITLog::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        if (argType.type == analysis::TIType::DOUBLE)
        {
	    S_d(argType, arg, OType, out, jit);
        }
        else
        {
            JITOptimizedCall1::S(argType, arg, OType, out, jit);
        }
    }

    void JITLog::S_d(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::LLVMContext & context = jit.getContext();
        llvm::Function & function = jit.getFunction();
        llvm::Value * _arg = arg->loadData(jit);
        llvm::Value * zero = jit.getConstant<double>(0);
        Cast::castInt64(_arg, jit);

        /* if (x >= 0 || std::isnan(x)) { return (log(x), 0); } else { return (log(-x), PI); } */
        llvm::Value * cmp_pos_or_nan = builder.CreateFCmpUGE(_arg, zero);
	llvm::Type * dbl_ty = jit.getTy<double>();
	llvm::Value * args[1];
	llvm::Value * toCall = llvm::Intrinsic::getDeclaration(&jit.getModule(), id, dbl_ty);
        llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
        llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(context, "", &function);
	llvm::BasicBlock * after = llvm::BasicBlock::Create(context, "", &function);

	builder.CreateCondBr(cmp_pos_or_nan, bb1, bb2);

	builder.SetInsertPoint(bb1);
	args[0] = _arg;
	llvm::Value * ret = builder.CreateCall(toCall, args);
	llvm::Value * res_re_1 = ret;
	llvm::Value * res_im_1 = zero;
	builder.CreateBr(after);

	builder.SetInsertPoint(bb2);
	_arg = builder.CreateFNeg(_arg);
	args[0] = _arg;
	ret = builder.CreateCall(toCall, args);
	llvm::Value * res_re_2 = ret;
	llvm::Value * res_im_2 = jit.getConstant<double>(M_PI);
	builder.CreateBr(after);

	builder.SetInsertPoint(after);
	llvm::PHINode * res_re = builder.CreatePHI(dbl_ty, 2);
	res_re->addIncoming(res_re_1, bb1);
	res_re->addIncoming(res_re_2, bb2);
	llvm::PHINode * res_im = builder.CreatePHI(dbl_ty, 2);
	res_im->addIncoming(res_im_1, bb1);
	res_im->addIncoming(res_im_2, bb2);

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
