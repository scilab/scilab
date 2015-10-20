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
#include "calls/FunctionSignature.hxx"

namespace jit
{
    
    JITScilabPtr JITTransposition::S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit)
    {
        if (Ltype.type == analysis::TIType::COMPLEX)
	{
	    std::pair<llvm::Value *, llvm::Value *> reim = L->loadReIm(jit);
	    llvm::Value * res_re = reim.first;
	    llvm::Value * res_im = jit.getBuilder().CreateFNeg(reim.second);
	    return jit.getScalar(res_re, res_im, Otype.type);
	}

        return L;
    }

    void JITTransposition::M(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
    {
	llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Type * int64_ty = jit.getTy<int64_t>();
	llvm::Value * Lr = L->loadRows(jit);
	llvm::Value * Lc = L->loadCols(jit);
	const std::vector<llvm::Type *> types = FunctionSignature::getFunctionArgsTy(jit,
										     In<analysis::TIType::Type>(Ltype.type, 1),
										     In<llvm::Type>(int64_ty),
										     In<llvm::Type>(int64_ty),
										     In<analysis::TIType::Type>(Otype.type, 2));
	
	const std::vector<llvm::Value *> args = FunctionSignature::getFunctionArgs(jit,
										   In<JITScilabPtr, 0>(L),
										   In<llvm::Value>(Lr),
										   In<llvm::Value>(Lc),
										   In<JITScilabPtr, 1>(O));
	
	llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(scilabName, Ltype), llvm::FunctionType::get(jit.getTy<void>(), types, false)));
	builder.CreateCall(toCall, args);
	O->storeRows(jit, Lc);
	O->storeCols(jit, Lr);
    }
}
