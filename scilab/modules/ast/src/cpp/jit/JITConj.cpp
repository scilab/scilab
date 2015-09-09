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
#include "calls/JITConj.hxx"

namespace jit
{

    void JITConj::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
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

    void JITConj::S_d(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
	llvm::Value * res = arg->loadData(jit);
	Cast::castInt64(res, jit);

        if (out.get())
        {
            out->storeData(jit, res);
        }
        else
        {
            jit.getScalar(res, OType.type, false, "").swap(out);
        }
    }

    void JITConj::S_c(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
    {
	std::pair<llvm::Value *, llvm::Value *> reim = arg->loadReIm(jit);
	llvm::Value * res_re = reim.first;
	llvm::Value * res_im = jit.getBuilder().CreateFNeg(reim.second);

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
