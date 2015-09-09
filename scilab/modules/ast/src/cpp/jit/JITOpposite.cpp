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
    
    JITScilabPtr JITOpposite::S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit)
    {
        llvm::Value * ret = nullptr;
        llvm::IRBuilder<> & builder = jit.getBuilder();

        // Integral types
        if (!Ltype.isfloating())
        {
	    llvm::Value * l = L->loadData(jit);
	    if (Ltype.issigned())
	    {
		ret = builder.CreateNSWNeg(l);
	    }
	    else
	    {
		ret = builder.CreateNUWNeg(l);
	    }
        }
        else if (Ltype.type == analysis::TIType::DOUBLE)
	{
	    llvm::Value * l = L->loadData(jit);
	    if (l->getType() == jit.getTy<int64_t>())
	    {
		ret = builder.CreateNSWNeg(l);
	    }
	    else
	    {
		ret = builder.CreateFNeg(l);
	    }
	}
	else
	{
	    // -complex
	    std::pair<llvm::Value *, llvm::Value *> l_reim = L->loadReIm(jit);
	    llvm::Value * ret_re = builder.CreateFNeg(l_reim.first);
	    llvm::Value * ret_im = builder.CreateFNeg(l_reim.second);
	    return jit.getScalar(ret_re, ret_im, Otype.type);
	}

        return jit.getScalar(ret, Otype.type);
    }

}
