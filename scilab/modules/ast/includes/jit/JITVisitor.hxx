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

#include <complex>
#include <map>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

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
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Support/TargetRegistry.h"
//#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Analysis/Passes.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize.h"

#include "llvm/ADT/Triple.h"
#include "llvm/Support/Host.h"

#include "visitor.hxx"
#include "allexp.hxx"
#include "allvar.hxx"
#include "AnalysisVisitor.hxx"

#include "calls/JITBinOpCall.hxx"
#include "calls/JITUnaryOpCall.hxx"
#include "calls/JITShortcutBinOpCall.hxx"

#include "dynlib_ast.h"

namespace jit
{

#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 7
typedef llvm::legacy::FunctionPassManager LLVM_FunctionPassManager;
#else
typedef llvm::FunctionPassManager LLVM_FunctionPassManager;
#endif

class JITScilabVal;
typedef std::shared_ptr<JITScilabVal> JITScilabPtr;

class EXTERN_AST JITVisitor : public ast::ConstVisitor, public analysis::FBlockEmittedListener
{

    struct OutContainer
    {
        union Value
        {
            int8_t i8;
            int16_t i16;
            int32_t i32;
            int64_t i64;
            uint8_t ui8;
            uint16_t ui16;
            uint32_t ui32;
            uint64_t ui64;
            double dbl;
            int32_t boolean;
            double cpx[2];
            void * ptr;
        };

        analysis::TIType::Type type;
        Value data;
        uint64_t rows;
        uint64_t cols;
        uint64_t refcount;

        OutContainer(const analysis::TIType::Type _type) : type(_type) { }
    };

    typedef std::map<symbol::Symbol, JITScilabPtr> JITSymbolMap;

    static const bool __init__;

    const analysis::AnalysisVisitor & analysis;

    std::string err;
    llvm::LLVMContext & context;
    llvm::Module * module;
    llvm::TargetMachine * target;
    llvm::ExecutionEngine * engine;
    llvm::legacy::PassManager MPM;
    llvm::legacy::FunctionPassManager FPM;
    llvm::Function * function;
    llvm::IRBuilder<> builder;
    llvm::Type * uintptrType;
    llvm::Type * dblTy;
    llvm::Type * int8Ty;
    llvm::Type * int16Ty;
    llvm::Type * int32Ty;
    llvm::Type * int64Ty;
    llvm::Type * int1Ty;
    llvm::Type * voidTy;
    llvm::Type * dblPtrTy;
    llvm::Type * int8PtrTy;
    llvm::Type * int16PtrTy;
    llvm::Type * int32PtrTy;
    llvm::Type * int64PtrTy;
    llvm::Type * int1PtrTy;
    llvm::BasicBlock * entryBlock;
    llvm::BasicBlock * mainBlock;
    llvm::BasicBlock * returnBlock;
    llvm::BasicBlock * errorBlock;

    JITScilabPtr _result;
    JITScilabPtr cpx_rvalue;
    std::vector<JITScilabPtr> multipleLHS;
    JITSymbolMap variables;
    std::vector<JITScilabPtr> temps;
    std::unordered_map<std::string, llvm::GlobalVariable *> globals;
    std::stack<std::pair<llvm::BasicBlock * , llvm::BasicBlock *>> blocks;
    std::queue<analysis::FunctionBlock *> fblocks;
    std::unordered_map<std::string, analysis::FunctionBlock *> mapNameFBlock;
    std::unordered_map<std::string, llvm::Value *> specialVars;

public:

    JITAddition addition;
    JITSubtraction subtraction;
    JITOpposite opposite;
    JITMultiplication multiplication;
    JITRDivision rdivision;
    JITPower power;
    JITEquality equality;
    JITNotEquality not_equality;
    JITLowerThan lower_than;
    JITGreaterThan greater_than;
    JITLowerOrEq lower_or_eq;
    JITGreaterOrEq greater_or_eq;
    JITNegation negation;
    JITShortcutEq shortcut_eq;
    JITShortcutNe shortcut_ne;
    JITShortcutLt shortcut_lt;
    JITShortcutGt shortcut_gt;
    JITShortcutLe shortcut_le;
    JITShortcutGe shortcut_ge;

    JITVisitor(const analysis::AnalysisVisitor & _analysis);
    virtual ~JITVisitor();

    void run();

    void dump() const;

    template<typename T>
    inline llvm::Type * getTy() const
    {
        if (std::is_const<T>::value)
        {
            return getTy<typename std::remove_const<T>::type>();
        }
        else if (std::is_pointer<T>::value && std::is_pointer<typename std::remove_pointer<T>::type>::value)
        {
            return llvm::PointerType::getUnqual(getTy<typename std::remove_pointer<T>::type>());
        }

        return nullptr;
    }

    inline llvm::Type * getTy(const analysis::TIType & ty, const unsigned char level = 0) const
    {
        llvm::Type * lty;

        switch (ty.type)
        {
            case analysis::TIType::BOOLEAN:
                lty = int32Ty;
                break;
            case analysis::TIType::DOUBLE:
                lty = dblTy;
                break;
            case analysis::TIType::INT16:
                lty = int16Ty;
                break;
            case analysis::TIType::INT32:
                lty = int32Ty;
                break;
            case analysis::TIType::INT64:
                lty = int64Ty;
                break;
            case analysis::TIType::INT8:
                lty = int8Ty;
                break;
            case analysis::TIType::UINT16:
                lty = int16Ty;
                break;
            case analysis::TIType::UINT32:
                lty = int32Ty;
                break;
            case analysis::TIType::UINT64:
                lty = int64Ty;
                break;
            case analysis::TIType::UINT8:
                lty = int8Ty;
                break;
            default:
                lty = voidTy;
        }

        for (unsigned char i = 0; i < level; ++i)
        {
            lty = llvm::PointerType::getUnqual(lty);
        }

        return lty;
    }

    inline unsigned int getTySizeInBytes(const llvm::Type * ty) const
    {
        return ty->isPointerTy() ? sizeof(void *) : (ty->getPrimitiveSizeInBits() / 8);
    }

    inline unsigned int getTySizeInBytes(const llvm::Value * v) const
    {
        return getTySizeInBytes(v->getType());
    }

    template<typename T>
    inline unsigned int getTySizeInBytes() const
    {
        return getTy<T>()->isPointerTy() ? sizeof(void *) : (getTy<T>()->getPrimitiveSizeInBits() / 8);
    }

    inline llvm::LLVMContext & getContext()
    {
        return context;
    }

    inline llvm::Module & getModule()
    {
        return *module;
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

    inline llvm::Value * getPointer(void * const ptr, llvm::Type * const type)
    {
        llvm::Value * v = llvm::ConstantInt::get(uintptrType, reinterpret_cast<uintptr_t>(ptr));
        v = builder.CreateIntToPtr(v, type);

        return v;
    }

    template<typename T>
    inline llvm::Value * getValue(const T val, const bool allocated = false, const std::string & name = "")
    {
        if (allocated)
        {
            return getAlloca<T>(val, name);
        }
        else
        {
            return getConstant<T>(val, name);
        }
    }

    inline llvm::Value * getValue(llvm::Value * val, const bool allocated = false, const std::string & name = "")
    {
        if (allocated)
        {
            return getAlloca(val, name);
        }
        else
        {
            return val;
        }
    }

    inline llvm::Value * getBool(const bool b)
    {
        return b ? llvm::ConstantInt::getTrue(context) : llvm::ConstantInt::getFalse(context);
    }


    template<typename T, bool isPtr> struct __getConstant;
    template<typename T> struct __getConstant<T, true>
    {
        inline static llvm::Value * get(JITVisitor * jit, const T val, const std::string & name = "")
        {
            return jit->getPointer((void *)val, jit->getTy<T>());
        }
    };

    template<typename T> struct __getConstant<T, false>
    {
        inline static llvm::Value * get(JITVisitor * jit, const T val, const std::string & name = "")
        {
            return llvm::ConstantInt::get(jit->getTy<T>(), uint64_t(val), !std::is_unsigned<T>::value);
        }
    };

    template<typename T>
    inline llvm::Value * getConstant(const T val, const std::string & name = "")
    {
        return __getConstant<T, std::is_pointer<T>::value>::get(this, val, name);
    }

    template<typename T>
    inline llvm::Value * getAlloca(const T val, const std::string & name = "")
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(getTy<T>(), nullptr, name);
        alloca->setAlignment(sizeof(T));
        builder.CreateAlignedStore(getConstant<T>(val), alloca, sizeof(T));

        return alloca;
    }

    template<typename T>
    inline llvm::Value * getAlloca(const std::string & name = "")
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(getTy<T>(), nullptr, name);
        alloca->setAlignment(sizeof(T));

        return alloca;
    }

    inline llvm::Value * getAlloca(llvm::Value * val, llvm::Type * type, int type_size, const std::string & name = "")
    {
        llvm::AllocaInst * alloca = builder.CreateAlloca(type, nullptr, name);
        alloca->setAlignment(type_size);
        builder.CreateAlignedStore(val, alloca, type_size);

        return alloca;
    }

    inline llvm::Value * getAlloca(llvm::Value * val, const std::string & name = "")
    {
        return getAlloca(val, val->getType(), getTySizeInBytes(val->getType()), name);
    }

    inline JITScilabPtr & getResult()
    {
        return  _result;
    }

    inline void setResult(JITScilabPtr & val)
    {
        _result = val;
    }

    inline void setResult(JITScilabPtr && val)
    {
        _result = val;
    }

    inline JITScilabPtr & getTemp(const int32_t id)
    {
        if (id >= 0 && id < temps.size())
        {
            return temps[id];
        }
        else
        {
            assert(false && " Bad temp id...");
        }
    }

    inline void addGlobal(const std::string & name, llvm::GlobalVariable * gv)
    {
        globals.emplace(name, gv);
    }

    inline llvm::GlobalVariable * getGlobal(const std::string & name)
    {
        auto i = globals.find(name);
        if (i == globals.end())
        {
            return nullptr;
        }
        return i->second;
    }

    inline llvm::Value * getSpecialVar(const std::string & name) const
    {
        auto i = specialVars.find(name);
        if (i != specialVars.end())
        {
            return i->second;
        }
        return nullptr;
    }

    inline void addSpecialVar(const std::string & name, llvm::Value * val)
    {
        specialVars.emplace(name, val);
    }

    inline llvm::BasicBlock * getEntryBlock()
    {
        return entryBlock;
    }

    void makeCall(const std::wstring & name, const std::vector<types::InternalType *> & in, std::vector<types::InternalType *> & out);

    JITScilabPtr getScalar(llvm::Value * const value, const analysis::TIType::Type ty, const bool alloc = false, const std::string & name = "");
    JITScilabPtr getScalar(llvm::Value * const re, llvm::Value * const im, const analysis::TIType::Type ty, const bool alloc = false, const std::string & name = "");
    JITScilabPtr & getCpxRValue();
    JITScilabPtr getScalar(const analysis::TIType::Type ty, const bool isAnInt, const std::string & name);
    JITScilabPtr getScalar(const analysis::TypeLocal & ty, const std::string & name);
    JITScilabPtr getMatrix(llvm::Value * const value, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const analysis::TIType::Type ty, const bool alloc, const std::string & name);
    JITScilabPtr getMatrix(const analysis::TIType::Type ty, const std::string & name, const bool init = false);
    JITScilabPtr getMatrix(const analysis::TypeLocal & ty, const std::string & name, const bool init = false);

    llvm::FunctionType * getFunctionType(const analysis::TIType & out, const std::vector<const analysis::TIType *> & types);

private:

    inline std::vector<JITScilabPtr> & getLHSContainer()
    {
        return multipleLHS;
    }

    inline void callThrow(const char * msg)
    {
        /*llvm::Value * _msg = builder.CreateGlobalStringPtr(llvm::StringRef(msg));
          llvm::Value * v = llvm::ConstantInt::get(uintptrType, reinterpret_cast<uintptr_t>(&jit::jit_throw));
          v = builder.CreateIntToPtr(v, getLLVMPtrFuncTy<void, char *>(context));
          builder.CreateCall(v, _msg);*/
    }

    template<typename T, typename U>
    inline void makeArg(std::vector<llvm::Value *> & args, types::GenericType * pGT)
    {
        T * x = (T *)static_cast<U *>(pGT)->get();
        int64_t r = static_cast<U *>(pGT)->getRows();
        int64_t c = static_cast<U *>(pGT)->getCols();
        int64_t refc = static_cast<U *>(pGT)->getRef();
        args.emplace_back(getValue(x));
        args.emplace_back(getValue(r));
        args.emplace_back(getValue(c));
        args.emplace_back(getValue(refc));
    }

    void visit(ast::CallExp & e, const unsigned int lhs);
    void visit(const ast::CallExp & e);

    void visit(const ast::SimpleVar & e);
    void visit(const ast::DollarVar & e);
    void visit(const ast::ColonVar & e);
    void visit(const ast::ArrayListVar & e);
    void visit(const ast::DoubleExp & e);
    void visit(const ast::BoolExp & e);
    void visit(const ast::StringExp & e);
    void visit(const ast::NilExp & e);
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
    void visit(const ast::IntSelectExp & e);
    void visit(const ast::SmallIntSelectExp & e);
    void visit(const ast::TableIntSelectExp & e);
    void visit(const ast::MapIntSelectExp & e);
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
    void visit(const ast::MemfillExp & e);

    void visit(const ast::OptimizedExp & e) { }
    void visit(const ast::DAXPYExp & e) { }
    void visit(const ast::StringSelectExp & e) { }
    void visit(const ast::CommentExp & e) { }

    void action(analysis::FunctionBlock & fblock);
    llvm::Type * getType(const analysis::TIType::Type ty, const bool scalar);
    llvm::Value * getPtrFromIndex(const ast::CallExp & ce);
    void runOptimizationPasses();
    void compileModule();
    void cloneSyms(const ast::Exp & e);
    void makeSwitch(const ast::IntSelectExp & e, const std::map<int64_t, ast::Exp *> & map);
    void CreateBr(llvm::BasicBlock * bb);
    void closeEntryBlock();
    void initPassManagers();

    static bool InitializeLLVM();
    static llvm::Type * getPtrAsIntTy(llvm::Module & module, llvm::LLVMContext & ctxt);
    static llvm::ExecutionEngine * InitializeEngine(llvm::Module * module, llvm::TargetMachine ** target);

};

template<>
inline llvm::Value * JITVisitor::getConstant<double>(const double val, const std::string & name)
{
    return llvm::ConstantFP::get(context, llvm::APFloat(val));
}

template<>
inline llvm::Value * JITVisitor::getConstant<std::complex<double>>(const std::complex<double> val, const std::string & name)
{
    llvm::Constant * re = llvm::ConstantFP::get(context, llvm::APFloat(val.real()));
    llvm::Constant * im = llvm::ConstantFP::get(context, llvm::APFloat(val.imag()));
    llvm::Constant * arr[] = { re, im };
    return llvm::ConstantVector::get(llvm::ArrayRef<llvm::Constant *>(arr));
}

template<>
inline llvm::Type * JITVisitor::getTy<double>() const
{
    return dblTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<bool>() const
{
    return int1Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<int8_t>() const
{
    return int8Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<int16_t>() const
{
    return int16Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<int32_t>() const
{
    return int32Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<int64_t>() const
{
    return int64Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint8_t>() const
{
    return int8Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint16_t>() const
{
    return int16Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint32_t>() const
{
    return int32Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint64_t>() const
{
    return int64Ty;
}

template<>
inline llvm::Type * JITVisitor::getTy<void>() const
{
    return voidTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<double *>() const
{
    return dblPtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<bool *>() const
{
    return int1PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<int8_t *>() const
{
    return int8PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<int16_t *>() const
{
    return int16PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<int32_t *>() const
{
    return int32PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<int64_t *>() const
{
    return int64PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint8_t *>() const
{
    return int8PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint16_t *>() const
{
    return int16PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint32_t *>() const
{
    return int32PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<uint64_t *>() const
{
    return int64PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const double *>() const
{
    return dblPtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const bool *>() const
{
    return int1PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const int8_t *>() const
{
    return int8PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const int16_t *>() const
{
    return int16PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const int32_t *>() const
{
    return int32PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const int64_t *>() const
{
    return int64PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const uint8_t *>() const
{
    return int8PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const uint16_t *>() const
{
    return int16PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const uint32_t *>() const
{
    return int32PtrTy;
}

template<>
inline llvm::Type * JITVisitor::getTy<const uint64_t *>() const
{
    return int64PtrTy;
}

} // namespace jit

#endif // __JIT_VISITOR_HXX__
