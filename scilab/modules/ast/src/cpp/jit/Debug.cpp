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

#include "JITVisitor.hxx"
#include "Debug.hxx"

namespace jit
{

void Debug::printI8(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printI8", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int8_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printUI8(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printUI8", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int8_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printI16(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printI16", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int16_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printUI16(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printUI16", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int16_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printI32(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printI32", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int32_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printUI32(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printUI32", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int32_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printI64(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printI64", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int64_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printUI64(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printUI64", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int64_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printD(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printD", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<double>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printB(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printB", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<int32_t>(), false));
    jit.getBuilder().CreateCall(print, x);
}

void Debug::printPtr(JITVisitor & jit, llvm::Value * x)
{
    llvm::Value * print = jit.getModule().getOrInsertFunction("printPtr", llvm::FunctionType::get(jit.getTy<void>(), jit.getTy<uint8_t *>(), false));
    jit.getBuilder().CreateCall(print, x);
}

} // namespace jit
