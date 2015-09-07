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

#ifndef __SCILAB_JIT_TRAITS_HXX__
#define __SCILAB_JIT_TRAITS_HXX__

#ifdef PACKAGE_BUGREPORT
#undef PACKAGE_BUGREPORT
#endif

#ifdef PACKAGE_NAME
#undef PACKAGE_NAME
#endif

#ifdef PACKAGE_STRING
#undef PACKAGE_STRING
#endif

#ifdef PACKAGE_TARNAME
#undef PACKAGE_TARNAME
#endif

#ifdef PACKAGE_VERSION
#undef PACKAGE_VERSION
#endif

#include "llvm/Config/config.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/PassManager.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/MCJIT.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Vectorize.h"

namespace jit
{
// The returned type is an int type where its size is equal to sizeof(void *)
// It is useful to pass a Scilab pointer into LLVM as an integer.
inline static llvm::Type * getPtrAsIntTy(llvm::Module & module, llvm::LLVMContext & ctxt)
{
#if LLVM_VERSION_MAJOR >= 3 && LLVM_VERSION_MINOR == 4
    return module.getPointerSize() == llvm::Module::Pointer32 ? llvm::Type::getInt32Ty(ctxt) : llvm::Type::getInt64Ty(ctxt);
#else
    return module.getDataLayout()->getPointerSize() == 32 ? llvm::Type::getInt32Ty(ctxt) : llvm::Type::getInt64Ty(ctxt);
#endif
}

inline static llvm::FunctionPassManager initFPM(llvm::Module * module, llvm::ExecutionEngine * engine)
{
    llvm::FunctionPassManager FPM(module);

#if LLVM_VERSION_MAJOR >= 3 && LLVM_VERSION_MINOR == 4
    FPM.add(new llvm::DataLayout(*engine->getDataLayout()));
#else
    FPM.add(new llvm::DataLayoutPass(*engine->getDataLayout()));
#endif

    // createBasicAliasAnalysisPass - This pass implements the stateless alias
    // analysis.
    FPM.add(llvm::createBasicAliasAnalysisPass());

    // Promote allocas to registers.
    // PromoteMemoryToRegister - This pass is used to promote memory references to
    // be register references. A simple example of the transformation performed by
    // this pass is:
    //
    //        FROM CODE                           TO CODE
    //   %X = alloca i32, i32 1                 ret i32 42
    //   store i32 42, i32 *%X
    //   %Y = load i32* %X
    //   ret i32 %Y
    FPM.add(llvm::createPromoteMemoryToRegisterPass());

    // Do simple "peephole" optimizations and bit-twiddling optzns.
    // This pass combines things like:
    //    %Y = add int 1, %X
    //    %Z = add int 1, %Y
    // into:
    //    %Z = add int 2, %X
    FPM.add(llvm::createInstructionCombiningPass());

    // Reassociate expressions.
    // Reassociate - This pass reassociates commutative expressions in an order that
    // is designed to promote better constant propagation, GCSE, LICM, PRE...
    //
    // For example:  4 + (x + 5)  ->  x + (4 + 5)
    FPM.add(llvm::createReassociatePass());

    // Eliminate Common SubExpressions.
    // GVN - This pass performs global value numbering and redundant load
    // elimination cotemporaneously.
    FPM.add(llvm::createGVNPass());

    // Simplify the control flow graph (deleting unreachable blocks, etc).
    FPM.add(llvm::createCFGSimplificationPass());

    FPM.add(llvm::createDeadInstEliminationPass());
    FPM.add(llvm::createDeadCodeEliminationPass());
    FPM.add(llvm::createLoopVectorizePass());

    FPM.doInitialization();

    return FPM;
}

template<typename>
inline static llvm::Type * getLLVMTy(llvm::LLVMContext & ctxt = llvm::getGlobalContext());

template<bool, typename>
struct _remove_u_helper;

template<typename T>
struct _remove_u_helper<true, T>
{
    typedef typename std::make_signed<T>::type type;
};

template<typename T>
struct _remove_u_helper<false, T>
{
    typedef T type;
};

template<typename T>
struct remove_u
{
    typedef typename _remove_u_helper<std::is_unsigned<T>::value, T>::type type;
};

template<typename T>
struct remove_pucv
{
    typedef typename remove_u<typename std::remove_cv<typename std::remove_pointer<T>::type>::type>::type type;
};

template<typename>
struct _get_llvm_ty;

template<bool, typename>
struct _get_llvm_ty_helper;

template<typename T>
struct _get_llvm_ty_helper<true, T>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::PointerType::getUnqual(_get_llvm_ty<typename std::remove_pointer<T>::type>::get(ctxt));
    }
};

template<typename T>
struct _get_llvm_ty_helper<false, T>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return _get_llvm_ty<typename remove_pucv<T>::type>::get(ctxt);
    }
};

/*    template<bool, typename>
    struct _get_llvm_int_ty_helper;

    template<typename T>
    struct _get_llvm_int_ty_helper<true, T>
    {
	inline static llvm::Type * get(llvm::LLVMContext & ctxt)
	    {
		return llvm::Type::get(ctxt, sizeof(T));
	    }
    };

    template<typename T>
    struct _get_llvm_int_ty_helper<false, T>
    {
	inline static llvm::Type * get(llvm::LLVMContext & ctxt)
	    {
		return _get_llvm_ty<typename remove_pucv<T>::type>::get(ctxt);
	    }
    };
*/

template<typename T>
struct _get_llvm_ty
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return _get_llvm_ty_helper<std::is_pointer<T>::value, T>::get(ctxt);
    }
};

template<>
struct _get_llvm_ty<double>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getDoubleTy(ctxt);
    }
};

template<>
struct _get_llvm_ty<bool>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getInt1Ty(ctxt);
    }
};

template<>
struct _get_llvm_ty<char>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getInt8Ty(ctxt);
    }
};

template<>
struct _get_llvm_ty<short>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getInt16Ty(ctxt);
    }
};

template<>
struct _get_llvm_ty<int>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getInt32Ty(ctxt);
    }
};

template<>
struct _get_llvm_ty<uint32_t>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getInt32Ty(ctxt);
    }
};

template<>
struct _get_llvm_ty<long long>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getInt64Ty(ctxt);
    }
};

template<>
struct _get_llvm_ty<long>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::IntegerType::get(ctxt, sizeof(long) * 8);
    }
};

template<>
struct _get_llvm_ty<void>
{
    inline static llvm::Type * get(llvm::LLVMContext & ctxt)
    {
        return llvm::Type::getVoidTy(ctxt);
    }
};

template<typename T>
inline static llvm::Type * getLLVMTy(llvm::LLVMContext & ctxt)
{
    return _get_llvm_ty<T>::get(ctxt);
}

template<typename Out>
inline llvm::FunctionType * getLLVMFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::FunctionType::get(getLLVMTy<Out>(ctxt), false);
}

template<typename Out, typename In1>
inline llvm::FunctionType * getLLVMFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::FunctionType::get(getLLVMTy<Out>(ctxt), llvm::ArrayRef<llvm::Type *>(getLLVMTy<In1>(ctxt)), false);
}

template<typename Out, typename In1, typename In2>
inline llvm::FunctionType * getLLVMFuncTy(llvm::LLVMContext & ctxt)
{
    llvm::Type * args[] = { getLLVMTy<In1>(ctxt), getLLVMTy<In2>(ctxt) };
    return llvm::FunctionType::get(getLLVMTy<Out>(ctxt), args, false);
}

template<typename Out, typename In1, typename In2, typename In3>
inline llvm::FunctionType * getLLVMFuncTy(llvm::LLVMContext & ctxt)
{
    llvm::Type * args[] = { getLLVMTy<In1>(ctxt), getLLVMTy<In2>(ctxt), getLLVMTy<In3>(ctxt) };
    return llvm::FunctionType::get(getLLVMTy<Out>(ctxt), llvm::ArrayRef<llvm::Type *>(args), false);
}

template<typename Out, typename In1, typename In2, typename In3, typename In4>
inline llvm::FunctionType * getLLVMFuncTy(llvm::LLVMContext & ctxt)
{
    llvm::Type * args[] = { getLLVMTy<In1>(ctxt), getLLVMTy<In2>(ctxt), getLLVMTy<In3>(ctxt), getLLVMTy<In4>(ctxt) };
    return llvm::FunctionType::get(getLLVMTy<Out>(ctxt), llvm::ArrayRef<llvm::Type *>(args), false);
}

template<typename Out, typename In1, typename In2, typename In3, typename In4, typename In5>
inline llvm::FunctionType * getLLVMFuncTy(llvm::LLVMContext & ctxt)
{
    llvm::Type * args[] = { getLLVMTy<In1>(ctxt), getLLVMTy<In2>(ctxt), getLLVMTy<In3>(ctxt), getLLVMTy<In4>(ctxt), getLLVMTy<In5>(ctxt) };
    return llvm::FunctionType::get(getLLVMTy<Out>(ctxt), llvm::ArrayRef<llvm::Type *>(args), false);
}

template<typename T>
inline llvm::Type * getLLVMPtrTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMTy<T>(ctxt));
}

template<typename Out>
inline llvm::Type * getLLVMPtrFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMFuncTy<Out>(ctxt));
}

template<typename Out, typename In1>
inline llvm::Type * getLLVMPtrFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMFuncTy<Out, In1>(ctxt));
}

template<typename Out, typename In1, typename In2>
inline llvm::Type * getLLVMPtrFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMFuncTy<Out, In1, In2>(ctxt));
}

template<typename Out, typename In1, typename In2, typename In3>
inline llvm::Type * getLLVMPtrFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMFuncTy<Out, In1, In2, In3>(ctxt));
}

template<typename Out, typename In1, typename In2, typename In3, typename In4>
inline llvm::Type * getLLVMPtrFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMFuncTy<Out, In1, In2, In3, In4>(ctxt));
}

template<typename Out, typename In1, typename In2, typename In3, typename In4, typename In5>
inline llvm::Type * getLLVMPtrFuncTy(llvm::LLVMContext & ctxt)
{
    return llvm::PointerType::getUnqual(getLLVMFuncTy<Out, In1, In2, In3, In4, In5>(ctxt));
}

template<typename T, typename U>
inline static void putInContext_S(symbol::Context * ctxt, symbol::Variable * var, U x)
{
    ctxt->put(var, new T(x));
}

template<typename T, typename U>
inline static void putInContext_M(symbol::Context * ctxt, symbol::Variable * var, U * x, int r, int c)
{
    double * data;
    ctxt->put(var, new T(r, c, &data));
    memcpy(data, x, sizeof(U) * r * c);
}

static void jit_throw(const char * msg)
{
    throw ast::InternalError(msg);
}

inline static bool InitializeLLVM()
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    return true;
}

inline static llvm::ExecutionEngine * InitializeEngine(llvm::Module * module)
{
    std::string err;
    llvm::TargetOptions opt;
    opt.JITEmitDebugInfo = true;
    return llvm::EngineBuilder(module).setErrorStr(&err).setUseMCJIT(true).setTargetOptions(opt).create();
}



/*    inline static preForLoop(llvm::Function * function, llvm::Context & context, llvm::IRBuilder<> & builder, const int start, const int step, const int end)
      {
      llvm::BasicBlock * BBBody = llvm::BasicBlock::Create(context, "for_body", function);
      llvm::BasicBlock * BBAfter = llvm::BasicBlock::Create(context, "for_after", function);
      llvm::Value * _start = llvm::ConstantInt::get(getLLVMTy<int>(context), start);
      llvm::Value * _step = llvm::ConstantInt::get(getLLVMTy<int>(context), step);
      llvm::Value * _end = llvm::ConstantInt::get(getLLVMTy<int>(context), end);

      llvm::BasicBlock * cur_block = builder.GetInsertBlock();
      llvm::Value * tmp = builder.CreateICmpSLT(_start, _end);
      builder.CreateCondBr(tmp, BBBody, BBAfter);

      builder.SetInsertPoint(BBBody);
      llvm::PHINode * phi = builder.CreatePHI(getLLVMTy<int>(context), 2);
      phi->addIncoming(_start, cur_block);

      builder.SetInsertPoint(BBBody);

      // put your body here

      tmp = builder.CreateAdd(phi, _step);
      phi->addIncoming(tmp, BBBody);

      tmp = builder.CreateICmpSLT(tmp, _end);
      builder.CreateCondBr(tmp, BBBody, BBAfter);

      builder.SetInsertPoint(BBAfter);
      }
*/


//template<typename> struct _Type { };

template<typename T> struct _Type
{
    inline llvm::Type * operator()(llvm::LLVMContext & ctxt = llvm::getGlobalContext())
    {
        return std::is_pointer<T>::value ? llvm::PointerType::getUnqual(getLLVMTy<typename std::remove_pointer<typename std::remove_cv<T>::type>::type>(ctxt)) : std::is_reference<T>:: value ? llvm::PointerType::getUnqual(getLLVMTy<typename std::remove_reference<typename std::remove_cv<T>::type>::type>(ctxt)) : getLLVMTy<typename std::remove_cv<T>::type>(ctxt);
    }
};

template<typename R, typename A1> struct _Type<R (*) (A1)>
{
    inline llvm::Type * operator()(llvm::LLVMContext & ctxt = llvm::getGlobalContext())
{
    return llvm::FunctionType::get(_Type<R>()(ctxt), llvm::ArrayRef<llvm::Type *>(_Type<A1>()(ctxt)), false);
}
};

template<typename T>
inline static llvm::Type * foo(llvm::LLVMContext & ctxt = llvm::getGlobalContext())
{
    return _Type<T>()(ctxt);
}


}

#endif // __SCILAB_JIT_TRAITS_HXX__
