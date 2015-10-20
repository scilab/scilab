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
#include "calls/FunctionSignature.hxx"

namespace jit
{

    bool JITUnaryOpCall::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
        const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
        const analysis::TIType & Ltype = oe.getRight().getDecorator().getResult().getType();

	// TODO: qd tranposeexp, oe.getExp() .... Bruno t'es vraiment une sous merde

        oe.getRight().accept(jit);
        JITScilabPtr L = jit.getResult();

	if (Ltype.isscalar())
	{
	    if (out.empty())
	    {
		out.emplace_back(S(L, Ltype, typesOut.front(), jit));
	    }
	    else
	    {
		out.front()->storeData(jit, S(L, Ltype, typesOut.front(), jit)->loadData(jit));
	    }
	}
	else
	{
	    if (out.empty())
	    {
		out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
	    }
	    M(L, Ltype, out.front(), typesOut.front(), jit);
	}

	return true;
    }

    void JITUnaryOpCall::M(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
    {
	llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Type * int64_ty = jit.getTy<int64_t>();
	llvm::Value * rows = L->loadRows(jit);
	llvm::Value * cols = L->loadCols(jit);
	llvm::Value * rc = builder.CreateMul(rows, cols);
	const std::vector<llvm::Type *> types = FunctionSignature::getFunctionArgsTy(jit,
										     In<llvm::Type>(int64_ty),
										     In<analysis::TIType::Type>(Ltype.type, Ltype.isscalar() ? 0 : 1),
										     In<analysis::TIType::Type>(Otype.type, Otype.isscalar() ? 1 : 2));
	
	const std::vector<llvm::Value *> args = FunctionSignature::getFunctionArgs(jit,
										   In<llvm::Value>(rc),
										   In<JITScilabPtr, 0>(L),
										   In<JITScilabPtr, 1>(O));
	
	llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(scilabName, Ltype), llvm::FunctionType::get(jit.getTy<void>(), types, false)));
	builder.CreateCall(toCall, args);
	O->storeRows(jit, rows);
	O->storeCols(jit, cols);
    }
    
}
