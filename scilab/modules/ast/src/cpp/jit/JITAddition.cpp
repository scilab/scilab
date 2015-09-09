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
    
    JITScilabPtr JITAddition::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit)
    {
        llvm::Value * ret = nullptr;
        llvm::IRBuilder<> & builder = jit.getBuilder();

        // Integral types
        if (!Ltype.isfloating() || !Rtype.isfloating())
        {
	    llvm::Value * l = L->loadData(jit);
	    llvm::Value * r = R->loadData(jit);
	    Cast::promoteScilabValue(Ltype.issigned(), l, Rtype.issigned(), r, builder);
            ret = builder.CreateAdd(l, r);
            if (Otype.type == analysis::TIType::DOUBLE)
            {
                // bool + bool = double !!
                ret = builder.CreateSIToFP(ret, jit.getTy<double>());
            }
        }
        else if (Ltype.isfloating() && Rtype.isfloating())
        {
	    if (Ltype.type == analysis::TIType::DOUBLE)
	    {
		llvm::Value * l = L->loadData(jit);
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // double + double
		    llvm::Value * r = R->loadData(jit);
		    llvm::Type * int64_ty = jit.getTy<int64_t>();
		    if (l->getType() == int64_ty && r->getType() == int64_ty)
		    {
			ret = builder.CreateAdd(l, r);
		    }
		    else
		    {
			Cast::castInt64(l, jit);
			Cast::castInt64(r, jit);
			ret = builder.CreateFAdd(l, r);
		    }   
		}
		else
		{
		    // double + complex
		    // if l is an int64, then it needs to be casted into a double
		    Cast::castInt64(l, jit);
		    std::pair<llvm::Value *, llvm::Value *> r_reim = R->loadReIm(jit);
                    ret = builder.CreateFAdd(l, r_reim.first);
		    return jit.getScalar(ret, r_reim.second, Otype.type);
		}
	    }
	    else
	    {
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // complex + double
		    llvm::Value * r = R->loadData(jit);
		    Cast::castInt64(r, jit);
		    std::pair<llvm::Value *, llvm::Value *> l_reim = R->loadReIm(jit);
		    ret = builder.CreateFAdd(l_reim.first, r);
		    return jit.getScalar(ret, l_reim.second, Otype.type);
		}
		else
		{
		    // complex + complex
		    std::pair<llvm::Value *, llvm::Value *> l_reim = L->loadReIm(jit);
		    std::pair<llvm::Value *, llvm::Value *> r_reim = R->loadReIm(jit);
		    llvm::Value * ret_re = builder.CreateFAdd(l_reim.first, r_reim.first);
		    llvm::Value * ret_im = builder.CreateFAdd(l_reim.second, r_reim.second);
		    return jit.getScalar(ret_re, ret_im, Otype.type);
		}
	    }
	}

        return jit.getScalar(ret, Otype.type);
    }

}
