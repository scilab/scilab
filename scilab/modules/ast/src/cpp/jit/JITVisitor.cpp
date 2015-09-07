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

#include "JITValues.hxx"
#include "JITVisitor.hxx"
#include "jit_operations.hxx"

namespace jit
{
const bool JITVisitor::__init__ = InitializeLLVM();
llvm::Value * const JITVisitor::ONE = llvm::ConstantInt::get(getLLVMTy<int>(), int(1));
llvm::Value * const JITVisitor::TWO = llvm::ConstantInt::get(getLLVMTy<int>(), int(2));
llvm::Value * const JITVisitor::THREE = llvm::ConstantInt::get(getLLVMTy<int>(), int(3));
llvm::Value * const JITVisitor::FOUR = llvm::ConstantInt::get(getLLVMTy<int>(), int(4));

JITVisitor::JITVisitor(const analysis::AnalysisVisitor & _analysis) : ast::ConstVisitor(),
    analysis(_analysis),
    context(llvm::getGlobalContext()),
    module("JIT module", context),
    engine(InitializeEngine(&module)),
    FPM(initFPM(&module, engine)),
    function(llvm::cast<llvm::Function>(module.getOrInsertFunction("jit_main", getLLVMTy<void>(context), nullptr))),
    builder(context),
    uintptrType(getPtrAsIntTy(module, context)),
    _result(nullptr),
    start(0),
    step(0),
    end(0)
{
    module.setDataLayout(engine->getDataLayout()->getStringRepresentation());
    llvm::BasicBlock * BB = llvm::BasicBlock::Create(context, "EntryBlock", function);
    builder.SetInsertPoint(BB);

    /*    symbol::Context * ctxt = symbol::Context::getInstance();
        const analysis::AnalysisVisitor::MapSymInfo & info = analysis.get_infos();

        for (analysis::AnalysisVisitor::MapSymInfo::const_iterator it = info.begin(), end = info.end(); it != end; ++it)
        {
            if (it->second.read || it->second.write || it->second.replace)
            {
                const std::wstring & name = it->first.getName();
                const std::string _name(name.begin(), name.end());
                symbol::Variable * var = ctxt->getOrCreate(it->first);
                types::InternalType * pIT = symbol::Context::getInstance()->get(var);

                if (pIT)
                {
                    symMap3.emplace(it->first, std::shared_ptr<JITVal>(JITVal::get(*this, pIT, it->second.write || it->second.replace, _name)));
                }
                else
                {
                    symMap3.emplace(it->first, std::shared_ptr<JITVal>(JITVal::get(*this, it->second.current_type, it->second.write || it->second.replace, _name)));
                }
            }
        }

    */

}

void JITVisitor::run()
{
    // on reinjecte les resultats ds l'environnement a=1;jit("a=2");
    /*symbol::Context * ctxt = symbol::Context::getInstance();
    llvm::Value * llvmCtxt = getPointer(ctxt);
    llvm::Value * toCall_M = module.getOrInsertFunction("putInContext_M_D_ds", getLLVMFuncTy<void, char *, char *, double *, int , int>(context));
    llvm::Value * toCall_S = module.getOrInsertFunction("putInContext_S_D_d", getLLVMFuncTy<void, char *, char *, double>(context));
    const analysis::AnalysisVisitor::MapSymInfo & info = analysis.get_infos();

    for (JITSymbolMap::const_iterator i = symMap3.begin(), end = symMap3.end(); i != end; ++i)
    {
        analysis::AnalysisVisitor::MapSymInfo::const_iterator it = info.find(i->first);
        if (it != info.end() && !it->second.isJustRead())
        {
            std::wcout << L"push in context: " << i->first.getName() << std::endl;
            symbol::Variable * var = ctxt->getOrCreate(i->first);
            llvm::Value * llvmVar = getPointer(var);
            if (i->second.get()->is_scalar())
            {
                builder.CreateCall3(toCall_S, llvmCtxt, llvmVar, i->second.get()->load(*this));
            }
            else
            {
                builder.CreateCall5(toCall_M, llvmCtxt, llvmVar, i->second.get()->load(*this), i->second.get()->loadR(*this), i->second.get()->loadC(*this));
            }
        }
    }

    builder.CreateRetVoid();

    dump();

    for (llvm::Module::iterator it = module.begin(), end = module.end(); it != end; ++it)
    {
        FPM.run(*it);
    }

    dump();

    engine->finalizeObject();

    reinterpret_cast<void (*)()>(engine->getFunctionAddress("jit_main"))();
    */
}

void JITVisitor::dump() const
{
    module.dump();
    //function->dump();
}

template<>
llvm::Value * JITVisitor::getConstant<double>(const double val)
{
    llvm::Value * v = llvm::ConstantFP::get(context, llvm::APFloat(val));
    return v;
}

void JITVisitor::visit(const ast::SimpleVar &e)
{
    /*                symbol::Symbol & sym = e.getName();
                      std::map<symbol::Symbol, llvm::Value *>::iterator i = symMap.find(sym);
                      if (i != symMap.end())
                      {
                      if (llvm::isa<llvm::AllocaInst>(i->second))
                      {
                      llvm::LoadInst * tmp = builder.CreateLoad(llvm::cast<llvm::AllocaInst>(i->second));
                      tmp->setAlignment(sizeof(double));
                      setResult(tmp);
                      }
                      else
                      {
                      setResult(i->second);
                      }
                      }
                      else
                      {
                      std::wcout << L"que faire...=" << sym.getName() << std::endl;
                      }
    */
    /*              symbol::Symbol & sym = e.getName();
                    std::map<symbol::Symbol, JITVal>::iterator i = symMap2.find(sym);
                    if (i != symMap2.end())
                    {
                    llvm::Value * r = llvm::ConstantInt::get(getLLVMTy<int>(context), 1);
                    setResult(JITVal(r, r, i->second.load(builder)));
                    }
                    else
                    {
                    types::Double * pIT = static_cast<Double *>(symbol::Context::getInstance()->get(((ast::SimpleVar&)e).stack_get()));
                    llvm::Value * r = llvm::ConstantInt::get(getLLVMTy<int>(context), pIT->getRows());
                    llvm::Value * c = llvm::Cou onstantInt::get(getLLVMTy<int>(context), pIT->getCols());
                    llvm::Value * ptr = getPointer(pIT->get(), getLLVMTy<double *>(context));

                    setResult(JITVal(r, c, ptr));

                    //std::wcout << L"que faire...=" << sym.getName() << std::endl;
                    }
    */
    const symbol::Symbol & sym = e.getSymbol();
    JITSymbolMap::iterator i = symMap3.find(sym);
    if (i != symMap3.end())
    {
        setResult(i->second);
    }
    else
    {
        const std::wstring & name = sym.getName();
        const std::string _name(name.begin(), name.end());
        /*types::InternalType * pIT = symbol::Context::getInstance()->get(((ast::SimpleVar&)e).stack_get());

          setResult(std::shared_ptr<JITVal>(JITVal::get(*this, pIT, _name)));*/
        throw ast::InternalError("Variable not declared before JIT: " + _name);
    }
}

void JITVisitor::visit(const ast::DollarVar &e) //a=[1 2;3 4];b=[5 6;7 8];jit("a/b")
{

}

void JITVisitor::visit(const ast::ColonVar &e)
{

}

void JITVisitor::visit(const ast::ArrayListVar &e)
{

}

void JITVisitor::visit(const ast::DoubleExp &e)
{
    setResult(std::shared_ptr<JITVal>(new JITScalarVal<double>(*this, e.getValue(), false)));
}

void JITVisitor::visit(const ast::BoolExp &e)
{

}

void JITVisitor::visit(const ast::StringExp &e)
{

}

void JITVisitor::visit(const ast::CommentExp &e)
{
    // ignored
}

void JITVisitor::visit(const ast::NilExp &e)
{

}

void JITVisitor::visit(const ast::CallExp &e)
{

}

void JITVisitor::visit(const ast::CellCallExp &e)
{

}

void JITVisitor::visit(const ast::OpExp &e)
{
    e.getLeft().accept(*this);
    std::shared_ptr<JITVal> pITL = getResult();

    e.getRight().accept(*this);
    std::shared_ptr<JITVal> & pITR = getResult();

    llvm::Value * pResult = NULL;

    switch (e.getOper())
    {
        case ast::OpExp::plus:
        {
            //const analysis::TIType & LT = e.left_get().decorator_get().res.get_type();
            //const analysis::TIType & RT = e.right_get().decorator_get().res.get_type();

            if (pITL.get()->is_scalar() && pITR.get()->is_scalar())
            {
                setResult(add_D_D(pITL, pITR, *this));
            }
            else
            {
                setResult(add_M_M(pITL, pITR, *this));
            }
            return;
        }
        case ast::OpExp::minus:
        {
            if (pITL.get()->is_scalar())
            {
                setResult(sub_D_D(pITL, pITR, *this));
            }
            else
            {
                setResult(sub_M_M(pITL, pITR, *this));
            }
            return;
        }
        case ast::OpExp::times:
        {
            if (pITL.get()->is_scalar())
            {
                setResult(dotmul_D_D(pITL, pITR, *this));
            }
            else
            {
                setResult(dotmul_M_M(pITL, pITR, *this));
            }
            return;
        }
        default:
            if (pITL.get()->is_scalar())
            {
                setResult(add_D_D(pITL, pITR, *this));
            }
            else
            {
                setResult(add_M_M(pITL, pITR, *this));
            }
            return;
    }

    //llvm::Value * r = llvm::ConstantInt::get(getLLVMTy<int>(context), 1);
    //setResult(JITVal(r, r, pResult));
}

void JITVisitor::visit(const ast::LogicalOpExp &e)
{

}

void JITVisitor::visit(const ast::AssignExp &e)
{
    if (e.getLeftExp().isSimpleVar())
    {
        ast::SimpleVar & pVar = static_cast<ast::SimpleVar &>(e.getLeftExp());

        e.getRightExp().accept(*this);
        std::shared_ptr<JITVal> & pITR = getResult();
        llvm::Value * alloca = nullptr;
        JITSymbolMap::const_iterator i = symMap3.find(pVar.getSymbol());

        if (i != symMap3.end())
        {
            i->second.get()->store(*pITR.get(), *this);
        }
        else
        {
            const std::wstring & name = pVar.getSymbol().getName();
            const std::string _name(name.begin(), name.end());
            // TODO: virer ce truc... le param <double> est force...
            JITVal * jitV = new JITScalarVal<double>(*this, pITR.get(), _name);
            symMap3.emplace(pVar.getSymbol(), std::shared_ptr<JITVal>(jitV));
        }

        setResult(std::shared_ptr<JITVal>(nullptr));
    }
}

void JITVisitor::visit(const ast::IfExp &e)
{

}

void JITVisitor::visit(const ast::WhileExp &e)
{

}

void JITVisitor::visit(const ast::ForExp &e)
{
    //e.vardec_get().accept(*this);
    const ast::VarDec & vardec = *e.getVardec().getAs<ast::VarDec>();
    symbol::Symbol varName = vardec.getSymbol();
    const ast::Exp & init = vardec.getInit();

    if (init.isListExp())
    {
        const ast::ListExp & list = static_cast<const ast::ListExp &>(init);
        const analysis::ForList64 & list_info = vardec.getListInfo();
        const double * list_values = list.getValues();
        llvm::Value * start = nullptr, * step, * end;
        bool use_int = false;
        bool use_uint = false;
        bool inc = true;
        bool known_step = false;
        bool it_read_in_loop = list_info.is_read_in_loop();

        if (list_info.is_constant())
        {
            if (list_info.is_int())
            {
                use_int = true;
                if (list_info.is_uint())
                {
                    use_uint = true;
                    start = getConstant(list_info.get_min<uint64_t>());
                    step = getConstant(list_info.get_step<uint64_t>());
                    end = getConstant(list_info.get_max<uint64_t>());
                }
                else
                {
                    start = getConstant(list_info.get_min<int64_t>());
                    step = getConstant(list_info.get_step<int64_t>());
                    end = getConstant(list_info.get_max<int64_t>());
                }
            }
            else
            {
                start = getConstant(list_info.get_min<double>());
                step = getConstant(list_info.get_step<double>());
                end = getConstant(list_info.get_max<double>());
            }

            inc = list_info.get_step<double>() > 0;
            known_step = true;
        }

        if (!start)
        {
            if (!ISNAN(list_values[0]))
            {
                start = getConstant(list_values[0]);
            }
            else
            {
                list.getStart().accept(*this);
                start = getResult().get()->load(*this);
            }

            if (!ISNAN(list_values[1]))
            {
                step = getConstant(list_values[1]);
                inc = list_values[1] >= 0;
                known_step = true;
            }
            else
            {
                list.getStep().accept(*this);
                step = getResult().get()->load(*this);
            }

            if (!ISNAN(list_values[2]))
            {
                end = getConstant(list_values[2]);
            }
            else
            {
                list.getEnd().accept(*this);
                end = getResult().get()->load(*this);
            }
        }

        llvm::BasicBlock * BBBody = llvm::BasicBlock::Create(context, "for_body", function);
        llvm::BasicBlock * BBAfter = llvm::BasicBlock::Create(context, "for_after", function);

        llvm::BasicBlock * cur_block = builder.GetInsertBlock();
        llvm::Value * tmp;

        if (known_step)
        {
            if (inc)
            {
                tmp = use_int ? (use_uint ? builder.CreateICmpULE(start, end) : builder.CreateICmpSLE(start, end)) : builder.CreateFCmpOLE(start, end);
            }
            else
            {
                tmp = use_int ? (use_uint ? builder.CreateICmpUGE(start, end) : builder.CreateICmpSGE(start, end)) : builder.CreateFCmpOGE(start, end);
            }
        }
        else
        {
            //TODO: add something to handle this case
        }

        builder.CreateCondBr(tmp, BBBody, BBAfter);

        builder.SetInsertPoint(BBBody);
        llvm::PHINode * phi = use_int ? builder.CreatePHI(getLLVMTy<int64_t>(context), 2) : builder.CreatePHI(getLLVMTy<double>(context), 2);

        // TODO: the call to uitofp is not removed even if it use mainly useless...
        // a=1;b=1;jit("for i=1:21;c=a+b;a=b;b=c;end;")
        if (it_read_in_loop)
        {
            JITSymbolMap::const_iterator i = symMap3.find(varName);
            tmp = use_int ? (use_uint ? builder.CreateUIToFP(phi, getLLVMTy<double>(context)) : builder.CreateSIToFP(phi, getLLVMTy<double>(context))) : phi;
            i->second.get()->store(tmp, *this);
        }

        phi->addIncoming(start, cur_block);

        builder.SetInsertPoint(BBBody);
        e.getBody().accept(*this);

        tmp = use_int ? builder.CreateAdd(phi, step) : builder.CreateFAdd(phi, step);
        phi->addIncoming(tmp, builder.GetInsertBlock());

        if (known_step)
        {
            if (inc)
            {
                tmp = use_int ? (use_uint ? builder.CreateICmpULE(tmp, end) : builder.CreateICmpSLE(tmp, end)) : builder.CreateFCmpOLE(tmp, end);
            }
            else
            {
                tmp = use_int ? (use_uint ? builder.CreateICmpUGE(tmp, end) : builder.CreateICmpSGE(tmp, end)) : builder.CreateFCmpOGE(tmp, end);
            }
        }
        else
        {
            //TODO: add something to handle this case
        }

        builder.CreateCondBr(tmp, BBBody, BBAfter);

        builder.SetInsertPoint(BBAfter);

        //llvm::AllocaInst * cur = builder.CreateAlloca(getLLVMTy<double>(context));
        //llvm::StoreInst * cur_store = builder.CreateAlignedStore(phi, cur, sizeof(double));

        //symMap3.emplace(varName, std::shared_ptr<JITVal>(new JITScalarVal<double>(*this, cur)));
    }
    else
    {
        // Should not occured...
        // Normally, if the init is an iterator the for exp itself is not jittable
        // but take care of the case for i=int32(1:2:123)...
    }

    //function->dump();
}

void JITVisitor::visit(const ast::BreakExp &e)
{

}

void JITVisitor::visit(const ast::ContinueExp &e)
{

}

void JITVisitor::visit(const ast::TryCatchExp &e)
{

}

void JITVisitor::visit(const ast::SelectExp &e)
{

}

void JITVisitor::visit(const ast::CaseExp &e)
{

}

void JITVisitor::visit(const ast::ReturnExp &e)
{

}

void JITVisitor::visit(const ast::FieldExp &e)
{

}

void JITVisitor::visit(const ast::NotExp &e)
{

}

void JITVisitor::visit(const ast::TransposeExp &e)
{

}

void JITVisitor::visit(const ast::MatrixExp &e)
{

}

void JITVisitor::visit(const ast::MatrixLineExp &e)
{

}

void JITVisitor::visit(const ast::CellExp &e)
{

}

void JITVisitor::visit(const ast::SeqExp &e)
{
    for (ast::exps_t::const_iterator i = e.getExps().begin(), end = e.getExps().end(); i != end; ++i)
    {
        setResult(std::shared_ptr<JITVal>(nullptr));
        (*i)->accept(*this);
    }
}

void JITVisitor::visit(const ast::ArrayListExp &e)
{

}

void JITVisitor::visit(const ast::AssignListExp &e)
{

}

void JITVisitor::visit(const ast::VarDec &e)
{
    e.getInit().accept(*this);
}

void JITVisitor::visit(const ast::FunctionDec &e)
{

}

void JITVisitor::visit(const ast::ListExp &e)
{

}

void JITVisitor::visit(const ast::OptimizedExp &e)
{

}

void JITVisitor::visit(const ast::DAXPYExp &e)
{

}
}
