/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Harris Bakiras <harris.bakiras@lip6.fr>,
 *  Peter Senna Tschudin <peter.senna@lip6.fr>
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef AST_JITVISITOR_HXX
#define AST_JITVISITOR_HXX

#include <time.h>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <iostream>

#include "visitor_common.hxx"
#include "shortcutvisitor.hxx"
#include "printvisitor.hxx"
#include "mutevisitor.hxx"

// Needed by visitprivate(const OpExp &)
// Needed by visitprivate(const LogicalOpExp &)
#include "generic_operations.hxx"
#include "types_or_and.hxx"
#include "configvariable.hxx"
#include "overload.hxx"
#include "scilabexception.hxx"

//#include "matrix_transpose_int.hxx"

extern "C" {
#include "doublecomplex.h"
#include "matrix_transpose.h"
#include "os_swprintf.h"
#include "more.h"
#include "sciprint.h"
    //#include "HandleManagement.h"
}

#include "timer.hxx"
#include "localization.h"

#include "scilabWrite.hxx"
#include "context.hxx"

#include "all.hxx"
#include "types.hxx"
#include "alltypes.hxx"

#undef ID
#undef LT
#undef GT

#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"

#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/PassManager.h"
#include "llvm/IR/DataLayout.h"
#include "vmkit_core.h"

#undef WHITE
#undef Max

#include <sys/types.h>

#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/CodeGen/GCStrategy.h"
#include "llvm/CodeGen/JITCodeEmitter.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ManagedStatic.h"
//#include "llvm/Support/Debug.h"
//#include "llvm/Support/raw_ostream.h"
#include <llvm/LinkAllPasses.h>

namespace llvm
{
#include "../src/cpp/llvm-wrapper-generated.cpp"
}

namespace ast
{
typedef double (*jitptr_t) ();

class JITVisitor : public ConstVisitor
{
private:
    /*
     * Attributes
     */
    llvm::Value* _result;
    bool m_bSingleResult;
    llvm::LLVMContext *context;
    llvm::Module *TheModule;
    llvm::IRBuilder<> *Builder;
    llvm::ExecutionEngine* ee;
    llvm::FunctionPassManager* pm;
    llvm::Type* uintptrType;
    symbol::Context * scilabContext;


    void visit (const SeqExp  &e)
    {
        visitprivate(e);
    }

    void visit (const IntExp &e)
    {
        visitprivate(e);
    }

    void visit (const FloatExp &e)
    {
        visitprivate(e);
    }

    void visit (const DoubleExp &e)
    {
        visitprivate(e);
    }

    void visit (const OpExp &e)
    {
        visitprivate(e);
    }

    void visit (const SimpleVar &e)
    {
        visitprivate(e);
    }


public:
    JITVisitor() : ConstVisitor()
    {
        llvm::InitializeNativeTarget();
        context = &llvm::getGlobalContext();
        Builder = new llvm::IRBuilder<> (*context);
        _result = NULL;
        m_bSingleResult = false;
        TheModule = new llvm::Module("scilab jit", *context);

        std::string err;
        llvm::EngineBuilder engine (TheModule);
        llvm::TargetOptions options;
        options.NoFramePointerElim = true;
        engine.setTargetOptions(options);
        engine.setEngineKind(llvm::EngineKind::JIT);
        engine.setErrorStr(&err);

        ee = engine.create();
        if (!ee)
        {
            fprintf(stderr, "Could not create ExecutionEngine: %s\n", err.c_str());
            exit(1);
        }
        ee->DisableLazyCompilation(0);
        ee->addModule(TheModule);
        TheModule->setDataLayout(ee->getDataLayout()->getStringRepresentation());
        pm = NULL; /* TODO : init */

        uintptrType = TheModule->getPointerSize() == llvm::Module::Pointer32 ?
                      llvm::Type::getInt32Ty(*context) : llvm::Type::getInt64Ty(*context);

        TheModule = llvm::makeLLVMModuleContents(TheModule);
        //TheModule->dump();

        for (llvm::Module::iterator cur = TheModule->begin(), end = TheModule->end(); cur != end; cur++)
        {
            void* ptr = dlsym(RTLD_DEFAULT, cur->getName().data());
            //printf("%s ---> %p\n", cur->getName().data(), ptr);
            if (ptr)
            {
                ee->updateGlobalMapping(cur, ptr);
            }
        }

        scilabContext = symbol::Context::getInstance();
    }

    void result_set(llvm::Value* const gtVal)
    {
        m_bSingleResult = true;
        _result = gtVal;
    }

    llvm::Value* result_get()
    {
        return  _result;
    }

    void visitprivate(const DoubleExp &e)
    {
        if (e.getBigDouble() == NULL)
        {
            Double *pdbl = new Double(e.value_get());
            (const_cast<DoubleExp *>(&e))->setBigDouble(pdbl);
        }

        //              llvm::Value* res = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(e.getBigDouble()));
        llvm::Value* res = llvm::ConstantInt::get(uintptrType, (uintptr_t)e.getBigDouble());
        res = Builder->CreateIntToPtr(res, llvm::PointerType::getUnqual(TheModule->getTypeByName("class.types::Double")));
        res->dump();
        result_set(res);
        _result->dump();
    }

    void visitprivate(const FloatExp &e)
    {
    }

    void visitprivate(const IntExp &e)
    {
    }

    void visitprivate(const SeqExp &e)
    {

        //T execMe;
        std::list<Exp *>::const_iterator    itExp;

        for (itExp = e.exps_get().begin (); itExp != e.exps_get().end (); ++itExp)
        {
            //reset default values
            result_set(NULL);
            llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), false);
            llvm::Function *TheFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "TheFunction", TheModule);
            llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", TheFunction);
            Builder->SetInsertPoint(BB);

            (*itExp)->accept(*this);

            if (1 || result_get() != NULL)
            {
                Builder->CreateRet(result_get());
                TheFunction->dump();
                void* res = ee->getPointerToFunction(TheFunction);
                jitptr_t myJit = (jitptr_t) res;
                double result = myJit();
                std::cout << "coucou result : " << result << std::endl;
            }
        }
    }

    llvm::Value* const llvm_value_one = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(1.0));
    llvm::Value* llvm_rec_power (llvm::Value* x, llvm::Value* n)
    {
        llvm::ConstantFP* ncfp = llvm::dyn_cast<llvm::ConstantFP>(n);

        if (ncfp->isZero())
        {
            // return 1
            return llvm_value_one;
        }
        if (ncfp->isNegative())
        {
            llvm::Value* moduloN = Builder->CreateFMul(llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(-1.0)), n);
            // return 1 / (x * llvm_rec_power(x, (-1 * n)-1))
            return Builder->CreateFDiv(llvm_value_one, Builder->CreateFMul(x, llvm_rec_power(x, Builder->CreateFSub(moduloN, llvm_value_one, "OpExp::minus")), "OpExp::times"), "OpExp::rdivide");
        }
        else
        {
            // return x * llvm_rec_power(x, n-1)
            return Builder->CreateFMul(x, llvm_rec_power(x, Builder->CreateFSub(n, llvm_value_one, "OpExp::minus")), "OpExp::times");
        }
    }
    void visitprivate(const OpExp &e)
    {
        /*
          switch (e.kind_get())
          {
          case OpExp::invalid_kind :
          {
          std::cout << "OpExp::invalid_kind" << std::endl;
          break;
          }
          case OpExp::bool_kind :
          {
          std::cout << "OpExp::bool_kind" << std::endl;
          break;
          }
          case OpExp::string_kind :
          {
          std::cout << "OpExp::string_kind" << std::endl;
          break;
          }
          case OpExp::integer_kind :
          {
          std::cout << "OpExp::integer_kind" << std::endl;
          break;
          }
          case OpExp::float_kind :
          {
          std::cout << "OpExp::float_kind" << std::endl;
          break;
          }
          case OpExp::double_kind :
          {
          std::cout << "OpExp::double_kind" << std::endl;
          break;
          }
          case OpExp::float_complex_kind :
          {
          break;
          }
          case OpExp::double_complex_kind :
          {
          break;
          }
          case OpExp::bool_matrix_kind :
          {
          break;
          }
          case OpExp::string_matrix_kind :
          {
          break;
          }
          case OpExp::integer_matrix_kind :
          {
          break;
          }
          case OpExp::float_matrix_kind :
          {
          break;
          }
          case OpExp::double_matrix_kind :
          {
          break;
          }
          case OpExp::float_complex_matrix_kind :
          {
          break;
          }
          case OpExp::double_complex_matrix_kind :
          {
          break;
          }
          case OpExp::matrix_kind :
          {
          break;
          }
          }
        */

        /*getting what to assign*/
        e.left_get().accept(*this);
        llvm::Value *pITL = result_get();

        /*getting what to assign*/
        e.right_get().accept(*this);
        llvm::Value *pITR = result_get();

        llvm::Value *pResult = NULL;
        switch (e.oper_get())
        {
            case OpExp::plus :
            {
                //%5 = call i32 @_Z17AddDoubleToDoublePN5types6DoubleES1_PS1_(%"class.types::Double"* %3, %"class.types::Double"* %4, %"class.types::Double"** %pdbl1)
                //                      pResult = Builder->CreateFAdd(pITL, pITR, "OpExp::plus");

                llvm::Value * tmp = Builder->CreateAlloca(llvm::PointerType::getUnqual(TheModule->getTypeByName("class.types::Double")));

                //printf("%p \n",TheModule->getFunction("_Z17AddDoubleToDoublePN5types6DoubleES1_PS1_"));

                Builder->CreateCall3(TheModule->getFunction("_Z17AddDoubleToDoublePN5types6DoubleES1_PS1_"), pITR, pITL, tmp);

                pResult = Builder->CreateLoad(tmp);
                break;
            }
            case OpExp::minus :
            {
                pResult = Builder->CreateFSub(pITL, pITR, "OpExp::minus");
                break;
            }
            case OpExp::times :
            {
                pResult = Builder->CreateFMul(pITL, pITR, "OpExp::times");
                break;
            }
            case OpExp::rdivide :
            {
                pResult = Builder->CreateFDiv(pITL, pITR, "OpExp::rdivide");
                break;
            }
            case OpExp::ldivide :
            {
                pResult = Builder->CreateFDiv(pITR, pITL, "OpExp::ldivide");
                break;
            }
            case OpExp::power :
            {
                pResult = llvm_rec_power(pITL, pITR);
                break;
            }
            case OpExp::unaryMinus :
            {
                pResult = Builder->CreateFSub(pITL, pITR, "OpExp::unaryMinus");
                break;
            }
            case OpExp::eq :
            {
                pResult = Builder->CreateUIToFP(Builder->CreateFCmpUEQ(pITL, pITR, "OpExp::eq"), llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltodouble");
                break;
            }
            case OpExp::ne :
            {
                pResult = Builder->CreateUIToFP(Builder->CreateNot(Builder->CreateFCmpUEQ(pITL, pITR, "OpExp::eq"), "not"), llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltodouble");
                break;
            }
            case OpExp::lt :
            {
                pResult = Builder->CreateUIToFP(Builder->CreateFCmpULT(pITL, pITR, "OpExp::lt"), llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltodouble");
                break;
            }
            case OpExp::le :
            {
                pResult = Builder->CreateUIToFP(Builder->CreateFCmpULE(pITL, pITR, "OpExp::le"), llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltodouble");
                break;
            }
            case OpExp::gt :
            {
                pResult = Builder->CreateUIToFP(Builder->CreateFCmpUGT(pITL, pITR, "OpExp::gt"), llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltodouble");
                break;
            }
            case OpExp::ge :
            {
                pResult = Builder->CreateUIToFP(Builder->CreateFCmpUGE(pITL, pITR, "OpExp::ge"), llvm::Type::getDoubleTy(llvm::getGlobalContext()), "booltodouble");
                break;
            }
            default:
            {
                std::cout << "Operation not supported, returning 42..." << std::endl;
                pResult = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(42.0));
                break;
            }
        }

        if (pResult == NULL)
        {
            result_set(llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(42.0)));
        }
        result_set(pResult);
    }

    void visitprivate (const AssignExp &e)
    {
        /*getting what to assign*/

        const SimpleVar *pVar = dynamic_cast<const SimpleVar*>(&e.left_exp_get());
        if (pVar)
        {
            // x = ?
            /*getting what to assign*/
            e.right_exp_get().accept(*this);

            llvm::Value *pITR = result_get();
            //reset result
            result_set(NULL);

            //mod->getFunction("_ZN6symbol7Context3putERKNS_6SymbolERN5types12InternalTypeE");
            llvm::Value * llvmScilabContext = llvm::ConstantInt::get(uintptrType, (uintptr_t)scilabContext);
            llvmScilabContext = Builder->CreateIntToPtr(llvmScilabContext, llvm::PointerType::getUnqual(TheModule->getTypeByName("class.symbol::Context")));

            llvm::Value * llvmSym = llvm::ConstantInt::get(uintptrType, (uintptr_t)&pVar->name_get());
            llvmSym = Builder->CreateIntToPtr(llvmSym, llvm::PointerType::getUnqual(TheModule->getTypeByName("class.symbol::Symbol")));
            llvmSym->dump();

            Builder->CreateCall3(TheModule->getFunction("_ZN6symbol7Context3putERKNS_6SymbolERN5types12InternalTypeE"), llvmScilabContext, llvmSym, pITR);

            //symbol::Context::getInstance()->put(pVar->name_get(), *pIT);
            return;
        }

        llvm::Value *pResult = NULL;
    }

    void visitprivate (const SimpleVar &e)
    {
        /*               InternalType * pI = symbol::Context::getInstance()->get(e.name_get());
                result_set(pI);
                if (pI != NULL)
                {
                    if (e.is_verbose() && pI->isCallable() == false && ConfigVariable::isPromptShow())
                    {
                        std::wostringstream ostr;
                        ostr << e.name_get().name_get() << L"  = " << L"(" << pI->getRef() << L")" << std::endl;
                        ostr << std::endl;
                        scilabWriteW(ostr.str().c_str());
                        VariableToString(pI);
                    }
            else
            {

            }
                }
                else
                {
                    wchar_t szError[bsiz];
                    os_swprintf(szError, bsiz, _W("Undefined variable: %ls\n"), e.name_get().name_get().c_str());
                    throw ScilabError(szError, 999, e.location_get());
                    //Err, SimpleVar doesn't exist in Scilab scopes.
            }*/
    }

    void visit (const BoolExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const NilExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const ColonVar &e)
    {
        //            visitprivate(e);
    }

    void visit (const DollarVar &e)
    {
        //            visitprivate(e);
    }

    void visit (const ArrayListVar &e)
    {
        //            visitprivate(e);
    }

    void visit (const FieldExp &e)
    {
        //            visitprivate(e);
    }


    void visit (const LogicalOpExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const AssignExp &e)
    {
        visitprivate(e);
    }

    void visit (const CellCallExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const CallExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const IfExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const TryCatchExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const WhileExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const ForExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const BreakExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const ContinueExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const ReturnExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const SelectExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const CaseExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const ArrayListExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const AssignListExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const NotExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const TransposeExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const VarDec &e)
    {
        //            visitprivate(e);
    }

    void visit (const FunctionDec &e)
    {
        //            visitprivate(e);
    }

    void visit(const ListExp &e)
    {
        //            visitprivate(e);
    }
    void visit (const MatrixExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const MatrixLineExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const CellExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const StringExp &e)
    {
        //            visitprivate(e);
    }

    void visit (const CommentExp &e)
    {
        //            visitprivate(e);
    }
};
}
#endif // !AST_JITVISITOR_HXX


