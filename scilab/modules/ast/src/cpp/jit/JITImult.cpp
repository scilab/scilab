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
#include "calls/JITImult.hxx"

namespace jit
{

    void JITImult::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
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

    void JITImult::S_d(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
	llvm::Value * res_re = jit.getConstant<double>(0);
	llvm::Value * _arg = arg->loadData(jit);
	Cast::castInt64(_arg, jit);
	llvm::Value * res_im = _arg;
		
	if (out.get())
        {
            out->storeReIm(jit, { res_re, res_im });
        }
        else
        {
	    out.reset(new JITScalComplex(jit, res_re, res_im, false, ""));
        }
    }

    void JITImult::S_c(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
	std::pair<llvm::Value *, llvm::Value *> reim = arg->loadReIm(jit);
	llvm::Value * re = reim.first;
	llvm::Value * im = reim.second;

	llvm::Value * res_re = jit.getBuilder().CreateFNeg(im);
	llvm::Value * res_im = re;

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
