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
#include "calls/JITCeil.hxx"

namespace jit
{

    void JITCeil::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
	if (argType.type == analysis::TIType::COMPLEX && id != llvm::Intrinsic::not_intrinsic)
	{
	    S_c(argType, arg, OType, out, jit);
	}
        else
        {
            JITOptimizedCall1::S(argType, arg, OType, out, jit);
        }
    }

    void JITCeil::S_c(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
	std::pair<llvm::Value *, llvm::Value *> reim = arg->loadReIm(jit);
	llvm::Value * toCall = llvm::Intrinsic::getDeclaration(&jit.getModule(), id, jit.getTy<double>());
	llvm::Value * args[] = { reim.first };
	llvm::Value * res_re = builder.CreateCall(toCall, args);
	args[0] = reim.second;
	llvm::Value * res_im = builder.CreateCall(toCall, args);
	
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
