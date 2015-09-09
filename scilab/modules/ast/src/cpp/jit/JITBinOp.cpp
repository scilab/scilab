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
#include "calls/JITBinOp.hxx"

namespace jit
{

    bool JITBinOp::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
    {
	const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
	llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Module & module = jit.getModule();

	const analysis::TIType & Ltype = oe.getLeft().getDecorator().getResult().getType();	
	const analysis::TIType & Rtype = oe.getRight().getDecorator().getResult().getType();

	if (Ltype.isscalar() && Rtype.isscalar())
	{
	    return op_S_S(e, typesOut, out, jit);
	}

	const std::string funname = analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype);
	
	oe.getLeft().accept(jit);
	JITScilabPtr L = jit.getResult();
	oe.getRight().accept(jit);
	JITScilabPtr & R = jit.getResult();

	llvm::Value * opCall;
	
	if (Ltype.isscalar())
	{
	    // binSM
	    llvm::Type * opTypes[] = {jit.getTy(Ltype), jit.getTy(Rtype, 1), jit.getTy<int64_t>, jit.getTy<int64_t>};
	    opCall = module.getOrInsertFunction(funname, llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(opTypes), false));
	}
	else if (Rtype.isscalar())
	{
	    llvm::Type * opTypes[] = {jit.getTy(Ltype, 1), jit.getTy<int64_t>, jit.getTy<int64_t>, jit.getTy(Rtype)};
	    opCall = module.getOrInsertFunction(funname, llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(opTypes), false));
	}
	else
	{
	    llvm::Type * opTypes[] = {jit.getTy(Ltype, 1), jit.getTy<int64_t>, jit.getTy<int64_t>, jit.getTy(Rtype, 1), jit.getTy<int64_t>, jit.getTy<int64_t>};
	    opCall = module.getOrInsertFunction(funname, llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(opTypes), false));
	}


	llvm::Value * __aligned_malloc = module.getOrInsertFunction("aligned_alloc", llvm::FunctionType::get(jit.getTy<int8_t *>(), llvm::ArrayRef<llvm::Type *>((llvm::Type *[]){ jit.getTy<int64_t>(), jit.getTy<int64_t>()}), false));
	//llvm::Value * alloc = builder.CreateCall(__new, size);
	llvm::Value * alloc = builder.CreateCall2(__aligned_malloc, jit.getConstant<int64_t>(sizeof(double)), size);
	llvm::Value * dbl_alloc = builder.CreateBitCast(alloc, jit.getTy<double *>());
	llvm::Value * __memset = llvm::Intrinsic::getDeclaration(&module, llvm::Intrinsic::memset, llvm::ArrayRef<llvm::Type *>((llvm::Type *[]){ jit.getTy<int8_t *>(), jit.getTy<uint64_t>()}));
	out.front()->storeData(jit, dbl_alloc);
	out.front()->storeRows(jit, r);
	out.front()->storeCols(jit, c);
	builder.CreateCall5(__memset, alloc, jit.getConstant<int8_t>(0), size, jit.getConstant<int32_t>(sizeof(double)), jit.getBool(false));

	return false;
    }
}
