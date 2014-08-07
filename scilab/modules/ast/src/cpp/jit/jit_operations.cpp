/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "JITVisitor.hxx"
#include "jit_operations.hxx"

namespace jit
{
std::shared_ptr<JITVal> add_D_D(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor)
{
    return std::shared_ptr<JITVal>(new JITScalarVal<double>(visitor, add<double>(L.get()->load(visitor), R.get()->load(visitor), visitor.getBuilder())));
}

std::shared_ptr<JITVal> sub_D_D(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor)
{
    return std::shared_ptr<JITVal>(new JITScalarVal<double>(visitor, sub<double>(L.get()->load(visitor), R.get()->load(visitor), visitor.getBuilder())));
}

std::shared_ptr<JITVal> dotmul_D_D(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor)
{
    return std::shared_ptr<JITVal>(new JITScalarVal<double>(visitor, dotmul<double>(L.get()->load(visitor), R.get()->load(visitor), visitor.getBuilder())));
}

static void debug(double * p)
{
    std::cout << "malloc=" << (void*)p << std::endl;
}

std::shared_ptr<JITVal> add_M_M(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor)
{
    // TODO: voir comment on peut avec l'analyzer recuperer une info sur le LHS de l'assignement pr eviter de reallouer
    // un tableau a chaque fois et dc reutiliser le precedent... (pbs potentiels avec truc du genre a=a*b mais pas avec a=a+b)

    llvm::LLVMContext & context = visitor.getContext();
    llvm::IRBuilder<> & builder = visitor.getBuilder();
    llvm::Value * size = builder.CreateMul(L.get()->loadR(visitor), L.get()->loadC(visitor));
    llvm::Value * eight = llvm::ConstantInt::get(getLLVMTy<int>(context), int(8));
    llvm::Value * malloc_size = builder.CreateMul(size, eight);
    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
    llvm::Value * alloc = llvm::CallInst::CreateMalloc(cur_block, getLLVMTy<int>(context), getLLVMTy<double>(context), malloc_size);
    cur_block->getInstList().push_back(llvm::cast<llvm::Instruction>(alloc));

    llvm::Value * toCall = visitor.getModule().getOrInsertFunction("add_M_M_d_d", getLLVMFuncTy<void, double *, long long, double *, double *>(context));


    size = builder.CreateIntCast(size, getLLVMTy<long long>(context), false);
    builder.CreateCall4(toCall, L.get()->load(visitor), size, R.get()->load(visitor), alloc);

    //llvm::Value * toCall_debug = visitor.getPointer(reinterpret_cast<void *>(&debug), getLLVMPtrFuncTy<void, double *>(context));
    //builder.CreateCall(toCall_debug, alloc);

    return std::shared_ptr<JITVal>(new JITMatrixVal<double>(visitor, L.get()->getR(), L.get()->getC(), alloc));
}

std::shared_ptr<JITVal> sub_M_M(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor)
{
    llvm::LLVMContext & context = visitor.getContext();
    llvm::IRBuilder<> & builder = visitor.getBuilder();
    llvm::Value * size = builder.CreateMul(L.get()->loadR(visitor), L.get()->loadC(visitor));
    llvm::Value * eight = llvm::ConstantInt::get(getLLVMTy<int>(context), int(8));
    llvm::Value * malloc_size = builder.CreateMul(size, eight);
    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
    llvm::Value * alloc = llvm::CallInst::CreateMalloc(cur_block, getLLVMTy<int>(context), getLLVMTy<double>(context), malloc_size);
    cur_block->getInstList().push_back(llvm::cast<llvm::Instruction>(alloc));

    llvm::Value * toCall = visitor.getModule().getOrInsertFunction("sub_M_M_d_d", getLLVMFuncTy<void, double *, long long, double *, double *>(context));

    size = builder.CreateIntCast(size, getLLVMTy<long long>(context), false);
    builder.CreateCall4(toCall, L.get()->load(visitor), size, R.get()->load(visitor), alloc);

    return std::shared_ptr<JITVal>(new JITMatrixVal<double>(visitor, L.get()->getR(), L.get()->getC(), alloc));
}

std::shared_ptr<JITVal> dotmul_M_M(std::shared_ptr<JITVal> & L, std::shared_ptr<JITVal> & R, JITVisitor & visitor)
{
    llvm::LLVMContext & context = visitor.getContext();
    llvm::IRBuilder<> & builder = visitor.getBuilder();
    llvm::Value * size = builder.CreateMul(L.get()->loadR(visitor), L.get()->loadC(visitor));
    llvm::Value * eight = llvm::ConstantInt::get(getLLVMTy<int>(context), int(8));
    llvm::Value * malloc_size = builder.CreateMul(size, eight);
    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
    llvm::Value * alloc = llvm::CallInst::CreateMalloc(cur_block, getLLVMTy<int>(context), getLLVMTy<double>(context), malloc_size);
    cur_block->getInstList().push_back(llvm::cast<llvm::Instruction>(alloc));

    llvm::Value * toCall = visitor.getModule().getOrInsertFunction("dotmul_M_M_d_d", getLLVMFuncTy<void, double *, long long, double *, double *>(context));

    size = builder.CreateIntCast(size, getLLVMTy<long long>(context), false);
    builder.CreateCall4(toCall, L.get()->load(visitor), size, R.get()->load(visitor), alloc);

    return std::shared_ptr<JITVal>(new JITMatrixVal<double>(visitor, L.get()->getR(), L.get()->getC(), alloc));
}

}
