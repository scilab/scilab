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
#include "calls/JITAngle.hxx"

namespace jit
{

    void JITAngle::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
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

    void JITAngle::S_d(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        llvm::Value * zero = jit.getConstant<double>(0);
        if (out.get())
        {
            out->storeData(jit, zero);
        }
        else
        {
            jit.getScalar(zero, OType.type, false, "").swap(out);
        }
    }

    void JITAngle::S_c(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        llvm::LLVMContext & context = jit.getContext();
        llvm::Function & function = jit.getFunction();
	std::pair<llvm::Value *, llvm::Value *> reim = arg->loadReIm(jit);
	llvm::Value * re = reim.first;
	llvm::Value * im = reim.second;
        
	/* 
	   angle(z) = atan2(y, x);
	*/
	llvm::Type * dbl_ty = jit.getTy<double>();
	llvm::Type * types[] = { dbl_ty, dbl_ty };
	llvm::Value * args[] = { im, re };
	llvm::FunctionType * funtype = llvm::FunctionType::get(dbl_ty, types, false);
	llvm::Value * toCall = jit.getModule().getOrInsertFunction("atan2", funtype);
	llvm::Value * res = builder.CreateCall(toCall, args);

	if (out.get())
        {
            out->storeData(jit, res);
        }
        else
        {
            jit.getScalar(res, OType.type, false, "").swap(out);
        }
    }

} // namespace jit
