/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Bruno JOFRET
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
//#include "runvisitor.hxx"
//#include "execvisitor.hxx"
//#include "timedvisitor.hxx"
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

#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/PassManager.h"
#include "llvm/DataLayout.h"
#include "vmkit_core.h"

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
        llvm::Value* res = llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(e.getBigDouble()->get(0)));
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
        std::list<Exp *>::const_iterator	itExp;

        for (itExp = e.exps_get().begin (); itExp != e.exps_get().end (); ++itExp)
        {
            //reset default values
            result_set(NULL);
            llvm::FunctionType *FT = llvm::FunctionType::get(llvm::Type::getDoubleTy(*context), false);
            llvm::Function *TheFunction = llvm::Function::Create(FT, llvm::Function::ExternalLinkage, "TheFunction", TheModule);
            llvm::BasicBlock *BB = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", TheFunction);
            Builder->SetInsertPoint(BB);

            (*itExp)->accept(*this);


            if (result_get() != NULL)
            {
                Builder->CreateRet(result_get());
                TheFunction->dump();
                void* res = ee->getPointerToFunction(TheFunction);
                jitptr_t myJit = (jitptr_t) res;
                double result = myJit();
                std::cout << "result : " << result << std::endl;
            }
        }
    }

    void visitprivate(const OpExp &e)
    {
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
                //            		pResult = GenericPlus(pITL, pITR);
                //Sum
                pResult = Builder->CreateFAdd(pITL, pITR, "addtmp");
                break;
            }
            default:
            {
                std::cout << "Operation not supported, returning 42..." << std::endl;
                break;
            }
        }

        if (pResult == NULL)
        {
            result_set(llvm::ConstantFP::get(llvm::getGlobalContext(), llvm::APFloat(42.0)));
        }
        result_set(pResult);
    }

    void visitprivate (const SimpleVar &e)
    {
        /* Not implemented. It only exits Scilab... */
        std::cout << "VISIT SIMPLEVAR, EXITING NOW..." << std::endl;
        ConfigVariable::setExitStatus(0);
        ConfigVariable::setForceQuit(true);
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
        //            visitprivate(e);
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

