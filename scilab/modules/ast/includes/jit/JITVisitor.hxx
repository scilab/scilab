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

#ifndef __JIT_VISITOR_HXX__
#define __JIT_VISITOR_HXX__

#include <map>
#include <memory>

//#include "llvm/IR/Verifier.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
//#include "llvm/Support/raw_ostream.h"
#include "llvm/PassManager.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/ADT/Triple.h"
#include "llvm/Support/Host.h"

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "ScilabJITTraits.hxx"
#include "AnalysisVisitor.hxx"

#include "dynlib_ast.h"

namespace jit
{

class JITVal;

class EXTERN_AST JITVisitor : public ast::ConstVisitor
{
    typedef std::map<symbol::Symbol, std::shared_ptr<JITVal>> JITSymbolMap;

    static const bool __init__;

    const analysis::AnalysisVisitor & analysis;

    std::string err;
    llvm::LLVMContext & context;
    llvm::Module module;
    llvm::ExecutionEngine * engine;
    llvm::FunctionPassManager FPM;
    llvm::Function * function;
    llvm::IRBuilder<> builder;
    llvm::Type * uintptrType;
    std::shared_ptr<JITVal> _result;

    //std::map<symbol::Symbol, llvm::Value *> symMap;
    //std::map<symbol::Symbol, JITVal> symMap2;

    JITSymbolMap symMap3;

    llvm::Value * start;
    llvm::Value * step;
    llvm::Value * end;
public:

    static llvm::Value * const ONE;
    static llvm::Value * const TWO;
    static llvm::Value * const THREE;
    static llvm::Value * const FOUR;

    JITVisitor(const analysis::AnalysisVisitor & _analysis);

    void run();

    void dump() const;

    inline llvm::LLVMContext & getContext()
    {
        return context;
    }

    inline llvm::Module & getModule()
    {
        return module;
    }

    inline llvm::IRBuilder<> & getBuilder()
    {
        return builder;
    }

    inline llvm::Function & getFunction()
    {
        return *function;
    }

    inline llvm::Type * getPtrTy()
    {
        return uintptrType;
    }

    inline llvm::Value * getPointer(void * const ptr)
    {
        llvm::Value * v = llvm::ConstantInt::get(uintptrType, reinterpret_cast<uintptr_t>(ptr));
        v = builder.CreateIntToPtr(v, llvm::PointerType::getInt8PtrTy(context));

        return v;
    }

    inline llvm::Value * getPointer(void * const ptr, const char * const type)
    {
        llvm::Value * v = llvm::ConstantInt::get(uintptrType, reinterpret_cast<uintptr_t>(ptr));
        v = builder.CreateIntToPtr(v, llvm::PointerType::getUnqual(module.getTypeByName(type)));

        return v;
    }

    inline llvm::Value * getPointer(void * const ptr, llvm::Type * const type)
    {
        llvm::Value * v = llvm::ConstantInt::get(uintptrType, reinterpret_cast<uintptr_t>(ptr));
        v = builder.CreateIntToPtr(v, type);

        return v;
    }

    template<typename T>
    llvm::Value * getConstant(const T val)
    {
        llvm::Value * const v = llvm::ConstantInt::get(getLLVMTy<T>(context), uint64_t(val), !std::is_unsigned<T>::value);

        return v;
    }

    template<typename T>
    llvm::Value * getConstantPtr(const T * val)
    {
        llvm::Value * const v = getPointer((void *)val, getLLVMTy<T *>(context));

        return v;
    }

    template<typename T>
    llvm::Value * getAlloca(const T val, const std::string & name)
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(getLLVMTy<T>(context), 0, name.c_str());
        alloca->setAlignment(sizeof(T));
        builder.CreateAlignedStore(getConstant<T>(val), alloca, sizeof(T));

        return alloca;
    }

    llvm::Value * getAlloca(llvm::Value * val, llvm::Type * type, int type_size, const std::string & name)
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(type, 0, name.c_str());
        alloca->setAlignment(type_size);
        builder.CreateAlignedStore(val, alloca, type_size);

        return alloca;
    }

    template<typename T>
    llvm::Value * getAllocaPtr(const T * val, const std::string & name)
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(getLLVMTy<T *>(context), 0, name.c_str());
        alloca->setAlignment(sizeof(T *));
        builder.CreateAlignedStore(getPointer((void *)val, getLLVMTy<T *>(context)), alloca, sizeof(T));

        return alloca;
    }

    template<typename T>
    llvm::Value * getAllocaPtr(llvm::Value * val)
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(getLLVMTy<T *>(context), 0);
        alloca->setAlignment(sizeof(T *));
        builder.CreateAlignedStore(val, alloca, sizeof(T));

        return alloca;
    }

private:

    inline void result_set(std::shared_ptr<JITVal> & val)
    {
        _result = val;
    }

    inline void result_set(std::shared_ptr<JITVal> && val)
    {
        _result = val;
    }

    inline std::shared_ptr<JITVal> & result_get()
    {
        return  _result;
    }

    inline void call_throw(const char * msg)
    {
        llvm::Value * _msg = builder.CreateGlobalStringPtr(llvm::StringRef(msg));
        llvm::Value * v = llvm::ConstantInt::get(uintptrType, reinterpret_cast<uintptr_t>(&jit::jit_throw));
        v = builder.CreateIntToPtr(v, getLLVMPtrFuncTy<void, char *>(context));
        builder.CreateCall(v, _msg);
    }

    void visit(const ast::SimpleVar & e);
    void visit(const ast::DollarVar & e);
    void visit(const ast::ColonVar & e);
    void visit(const ast::ArrayListVar & e);
    void visit(const ast::IntExp & e);
    void visit(const ast::FloatExp & e);
    void visit(const ast::DoubleExp & e);
    void visit(const ast::BoolExp & e);
    void visit(const ast::StringExp & e);
    void visit(const ast::CommentExp & e);
    void visit(const ast::NilExp & e);
    void visit(const ast::CallExp & e);
    void visit(const ast::CellCallExp & e);
    void visit(const ast::OpExp & e);
    void visit(const ast::LogicalOpExp & e);
    void visit(const ast::AssignExp & e);
    void visit(const ast::IfExp & e);
    void visit(const ast::WhileExp & e);
    void visit(const ast::ForExp & e);
    void visit(const ast::BreakExp & e);
    void visit(const ast::ContinueExp & e);
    void visit(const ast::TryCatchExp & e);
    void visit(const ast::SelectExp & e);
    void visit(const ast::CaseExp & e);
    void visit(const ast::ReturnExp & e);
    void visit(const ast::FieldExp & e);
    void visit(const ast::NotExp & e);
    void visit(const ast::TransposeExp & e);
    void visit(const ast::MatrixExp & e);
    void visit(const ast::MatrixLineExp & e);
    void visit(const ast::CellExp & e);
    void visit(const ast::SeqExp & e);
    void visit(const ast::ArrayListExp & e);
    void visit(const ast::AssignListExp & e);
    void visit(const ast::VarDec & e);
    void visit(const ast::FunctionDec & e);
    void visit(const ast::ListExp & e);
};

template<> llvm::Value * JITVisitor::getConstant<double>(const double val);

} // namespace jit

#endif // __JIT_VISITOR_HXX__
