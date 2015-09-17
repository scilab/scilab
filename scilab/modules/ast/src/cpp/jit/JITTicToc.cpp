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
#include "calls/JITTicToc.hxx"

namespace jit
{

bool JITTic::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
{
    const std::vector<llvm::Type *> types;
    const std::vector<llvm::Value *> args;
    llvm::FunctionType * funtype = llvm::FunctionType::get(jit.getTy<void>(), types, false);
    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction("tic", funtype));
    llvm::CallInst * ret = jit.getBuilder().CreateCall(toCall, args);
    ret->setTailCall(true);

    return true;
}

bool JITToc::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
{
    const std::vector<llvm::Type *> types;
    const std::vector<llvm::Value *> args;
    llvm::FunctionType * funtype = llvm::FunctionType::get(jit.getTy<double>(), types, false);
    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction("toc", funtype));
    llvm::CallInst * ret = jit.getBuilder().CreateCall(toCall, args);
    ret->setTailCall(true);

    if (out.empty())
    {
        out.emplace_back(jit.getScalar(ret, analysis::TIType::DOUBLE, false, ""));
    }
    else
    {
        out.front()->storeData(jit, ret);
    }

    return true;
}

} // namespace jit
