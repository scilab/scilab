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

#include "llvm/Support/FormattedStream.h"

#include "MemoryManager.hxx"
#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "alltypes.hxx"

#include "ScilabJITEventListener.hxx"


#define TIME_LLVM 1

namespace jit
{
const bool JITVisitor::__init__ = InitializeLLVM();

JITVisitor::JITVisitor(const analysis::AnalysisVisitor & _analysis) : ast::ConstVisitor(),
    analysis(_analysis),
    context(llvm::getGlobalContext()),
    module(new llvm::Module("JIT", context)),
    target(nullptr),
    engine(InitializeEngine(module, &target)),
    FPM(initFPM(module, engine, target)),
    function(nullptr),
    builder(context),
    uintptrType(getPtrAsIntTy(*module, context)),
    dblTy(llvm::Type::getDoubleTy(context)),
    int1Ty(llvm::Type::getInt1Ty(context)),
    int8Ty(llvm::Type::getInt8Ty(context)),
    int16Ty(llvm::Type::getInt16Ty(context)),
    int32Ty(llvm::Type::getInt32Ty(context)),
    int64Ty(llvm::Type::getInt64Ty(context)),
    voidTy(llvm::Type::getVoidTy(context)),
    dblPtrTy(llvm::Type::getDoublePtrTy(context)),
    int1PtrTy(llvm::Type::getInt1PtrTy(context)),
    int8PtrTy(llvm::Type::getInt8PtrTy(context)),
    int16PtrTy(llvm::Type::getInt16PtrTy(context)),
    int32PtrTy(llvm::Type::getInt32PtrTy(context)),
    int64PtrTy(llvm::Type::getInt64PtrTy(context)),
    _result(nullptr),
    cpx_rvalue(nullptr)
{
    //std::wcerr << "Map size=" << MemoryManager::getMapSize() << std::endl;
}

JITVisitor::~JITVisitor()
{
    delete engine;
}

void JITVisitor::run()
{

}

void JITVisitor::dump() const
{
    module->dump();
    //function->dump();
}

void JITVisitor::runOptimizationPasses()
{
#if TIME_LLVM == 1
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif
    for (llvm::Module::iterator it = module->begin(), end = module->end(); it != end; ++it)
    {
        FPM.run(*it);
    }

#if TIME_LLVM == 1
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    std::wcerr << "Optimization time=" << duration << " s." << std::endl;
#endif
}

void JITVisitor::compileModule()
{
    /*std::string error;
    llvm::raw_fd_ostream stream("/tmp/scilab.s", error, llvm::sys::fs::OpenFlags::F_None);
    llvm::formatted_raw_ostream frs(stream);
    llvm::PassManager PM;
    target->addPassesToEmitFile(PM, frs, llvm::TargetMachine::CGFT_AssemblyFile);
    PM.run(*module);
    frs.flush();
    stream.close();*/


#if TIME_LLVM == 1
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif

    engine->finalizeObject();

#if TIME_LLVM == 1
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    std::wcerr << "Compile time=" << duration << " s." << std::endl;
#endif
}

JITScilabPtr & JITVisitor::getCpxRValue()
{
    return cpx_rvalue;
}

llvm::Value * JITVisitor::getPtrFromIndex(const ast::CallExp & ce)
{
    if (ce.getDecorator().getResult().getType().isscalar())
    {
        ce.getName().accept(*this);
        JITScilabPtr mat = getResult();
        ast::exps_t args = ce.getArgs();
        const std::size_t size = args.size();
        llvm::Value * index = nullptr;
        if (size == 1)
        {
            const ast::Exp & first = *args.front();
            first.accept(*this);
            index = getResult()->loadData(*this);
            if (!first.getDecorator().getResult().isAnInt())
            {
                // argument is not an int64_t => cast
                index = Cast::cast<int64_t>(index, true, *this);
            }
            index = builder.CreateSub(index, getConstant<int64_t>(1));
        }
        else if (size == 2)
        {
            const ast::Exp & first = *args.front();
            const ast::Exp & second = *args.back();
            first.accept(*this);
            llvm::Value * index_R = getResult()->loadData(*this);
            second.accept(*this);
            llvm::Value * index_C = getResult()->loadData(*this);
            if (!first.getDecorator().getResult().isAnInt())
            {
                // argument is not an int64_t => cast
                index_R = Cast::cast<int64_t>(index_R, true, *this);
            }
            if (!second.getDecorator().getResult().isAnInt())
            {
                // argument is not an int64_t => cast
                index_C = Cast::cast<int64_t>(index_C, true, *this);
            }
            llvm::Value * one = getConstant<int64_t>(1);
            index_R = builder.CreateSub(index_R, one);
            index_C = builder.CreateSub(index_C, one);

            // now compute the index
            index = builder.CreateMul(index_C, mat->loadRows(*this));
            index = builder.CreateAdd(index_R, index);
        }

        if (index)
        {
            if (ce.getDecorator().safe)
            {
                return builder.CreateGEP(mat->loadData(*this), index);
            }
            else
            {
                llvm::Value * rc = builder.CreateMul(mat->loadRows(*this), mat->loadCols(*this));
                llvm::Value * check = builder.CreateICmpSLT(index, rc);
                // TODO: branch on error if check is false
                return builder.CreateGEP(mat->loadData(*this), index);
            }
        }
    }

    return nullptr;
}

void JITVisitor::closeEntryBlock()
{
    builder.SetInsertPoint(entryBlock);
    CreateBr(mainBlock);
}

void JITVisitor::CreateBr(llvm::BasicBlock * bb)
{
    llvm::BasicBlock * cur_block = builder.GetInsertBlock();
    if (cur_block->empty() || !llvm::isa<llvm::BranchInst>(cur_block->back()))
    {
        builder.CreateBr(bb);
    }
}

void JITVisitor::visit(const ast::SimpleVar & e)
{
    setResult(variables.find(e.getSymbol())->second);
}

void JITVisitor::visit(const ast::DollarVar & e)
{

}

void JITVisitor::visit(const ast::ColonVar & e)
{

}

void JITVisitor::visit(const ast::ArrayListVar & e)
{

}

void JITVisitor::visit(const ast::DoubleExp & e)
{
    if (e.getDecorator().getResult().isAnInt())
    {
        setResult(JITScilabPtr(new JITScalInt64(*this, (int64_t)e.getValue(), false, "")));
    }
    else
    {
        if (types::Double * pDbl = static_cast<types::Double *>(e.getConstant()))
        {
            if (pDbl->isComplex())
            {
                setResult(JITScilabPtr(new JITScalComplex(*this, std::complex<double>(pDbl->get(0), pDbl->getImg(0)), false, "")));
            }
            else
            {
                setResult(JITScilabPtr(new JITScalDouble(*this, pDbl->get(0), false, "")));
            }
        }
        else
        {
            setResult(JITScilabPtr(new JITScalDouble(*this, e.getValue(), false, "")));
        }
    }
}

void JITVisitor::visit(const ast::BoolExp & e)
{

}

void JITVisitor::visit(const ast::StringExp & e)
{

}

void JITVisitor::visit(const ast::NilExp & e)
{

}

void JITVisitor::visit(const ast::CellCallExp & e)
{

}

void JITVisitor::visit(const ast::AssignExp & e)
{

    if (e.getLeftExp().isSimpleVar()) // A = ...
    {
        const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(e.getLeftExp()).getSymbol();
        if (e.getRightExp().isSimpleVar())
        {
            // A = B so we just share the data
            const symbol::Symbol & Rsym = static_cast<ast::SimpleVar &>(e.getRightExp()).getSymbol();
            JITScilabPtr & Lvalue = variables.find(Lsym)->second;
            JITScilabPtr & Rvalue = variables.find(Rsym)->second;
            Lvalue->storeRows(*this, Rvalue->loadRows(*this));
            Lvalue->storeCols(*this, Rvalue->loadCols(*this));
            Lvalue->storeData(*this, Rvalue->loadData(*this));
            if (e.getRightExp().getDecorator().getResult().getType().type == analysis::TIType::COMPLEX)
            {
                Lvalue->storeImag(*this, Rvalue->loadImag(*this));
            }
        }
        else
        {
            e.getRightExp().accept(*this);
            // A = foo(...)...
            if (!e.getRightExp().isCallExp())
            {
                JITScilabPtr & Lvalue = variables.find(Lsym)->second;
                JITScilabPtr & Rvalue = getResult();
                Lvalue->storeRows(*this, Rvalue->loadRows(*this));
                Lvalue->storeCols(*this, Rvalue->loadCols(*this));
                Lvalue->storeData(*this, Rvalue->loadData(*this));
                if (e.getRightExp().getDecorator().getResult().getType().type == analysis::TIType::COMPLEX)
                {
                    Lvalue->storeImag(*this, Rvalue->loadImag(*this));
                }
            }
        }
    }
    else if (e.getLeftExp().isCallExp()) // A(12) = ...
    {
        ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
        if (ce.getName().isSimpleVar())
        {
            // We have an insertion
            /**
             *  Several possibilities:
             *    i) A(I) = B(I): usually in Scilab that means:
             *       temp = B(I) and then A(I) = temp
             *       If we infered that the ext/ins is safe we can make a for loop:
             *          for k = 1:size(I,'*'), A(I(k)) = B(I(k)), end
             *    ii) A(I) = fun(I): in the general case we should try to devectorize the expression
             *    iii) A(i) = B(i): no problem
             */
            const symbol::Symbol & symL = static_cast<ast::SimpleVar &>(ce.getName()).getSymbol();
            if (e.getDecorator().safe && ce.getDecorator().getResult().getType().isscalar())
            {
                llvm::Value * ptr = getPtrFromIndex(ce);
                e.getRightExp().accept(*this);
                builder.CreateStore(getResult()->loadData(*this), ptr);
            }
        }
    }
    else if (e.getLeftExp().isAssignListExp()) // [A, B] = ...
    {
        ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(e.getLeftExp());
        if (e.getRightExp().isCallExp())
        {
            e.getRightExp().accept(*this);
        }
    }
}

void JITVisitor::visit(const ast::TryCatchExp & e)
{

}

void JITVisitor::visit(const ast::CaseExp & e)
{
    // treated directly in SelectExp
}

void JITVisitor::visit(const ast::ReturnExp & e)
{

}

void JITVisitor::visit(const ast::FieldExp & e)
{

}

void JITVisitor::visit(const ast::TransposeExp & e)
{

}

void JITVisitor::visit(const ast::MatrixExp & e)
{

}

void JITVisitor::visit(const ast::MatrixLineExp & e)
{
    // treated in MatrixExp
}

void JITVisitor::visit(const ast::CellExp & e)
{

}

void JITVisitor::visit(const ast::SeqExp & e)
{
    for (ast::exps_t::const_iterator i = e.getExps().begin(), end = e.getExps().end(); i != end; ++i)
    {
        (*i)->accept(*this);
    }
}

void JITVisitor::visit(const ast::ArrayListExp & e)
{

}

void JITVisitor::visit(const ast::AssignListExp & e)
{

}

void JITVisitor::visit(const ast::FunctionDec & e)
{

}

void JITVisitor::visit(const ast::ListExp & e)
{

}

llvm::Type * JITVisitor::getType(const analysis::TIType::Type ty, const bool scalar)
{
    switch (ty)
    {
        case analysis::TIType::DOUBLE:
            return scalar ? dblTy : dblPtrTy;
        case analysis::TIType::INT8:
        case analysis::TIType::UINT8:
            return scalar ? int8Ty : int8PtrTy;
        case analysis::TIType::INT16:
        case analysis::TIType::UINT16:
            return scalar ? int16Ty : int16PtrTy;
        case analysis::TIType::INT32:
        case analysis::TIType::UINT32:
            return scalar ? int32Ty : int32PtrTy;
        case analysis::TIType::INT64:
        case analysis::TIType::UINT64:
            return scalar ? int64Ty : int64PtrTy;
        case analysis::TIType::BOOLEAN:
            return scalar ? int32Ty : int32PtrTy;
        default:
            return nullptr;
    }
}

// TODO: virer ty comme args => useless
JITScilabPtr JITVisitor::getScalar(llvm::Value * const re, llvm::Value * const im, const analysis::TIType::Type ty, const bool alloc, const std::string & name)
{
    return std::shared_ptr<JITScilabVal>(new JITScalComplex(*this, re, im, alloc, name));
}

JITScilabPtr JITVisitor::getScalar(llvm::Value * const value, const analysis::TIType::Type ty, const bool alloc, const std::string & name)
{
    switch (ty)
    {
        case analysis::TIType::DOUBLE:
            return std::shared_ptr<JITScilabVal>(new JITScalDouble(*this, value, alloc, name));
        case analysis::TIType::INT8:
            return std::shared_ptr<JITScilabVal>(new JITScalInt8(*this, value, alloc, name));
        case analysis::TIType::INT16:
            return std::shared_ptr<JITScilabVal>(new JITScalInt16(*this, value, alloc, name));
        case analysis::TIType::INT32:
            return std::shared_ptr<JITScilabVal>(new JITScalInt32(*this, value, alloc, name));
        case analysis::TIType::INT64:
            return std::shared_ptr<JITScilabVal>(new JITScalInt64(*this, value, alloc, name));
        case analysis::TIType::UINT8:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt8(*this, value, alloc, name));
        case analysis::TIType::UINT16:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt16(*this, value, alloc, name));
        case analysis::TIType::UINT32:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt32(*this, value, alloc, name));
        case analysis::TIType::UINT64:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt64(*this, value, alloc, name));
        case analysis::TIType::BOOLEAN:
            return std::shared_ptr<JITScilabVal>(new JITScalBool(*this, value, alloc, name));
        default:
            return nullptr;
    }
}

JITScilabPtr JITVisitor::getScalar(const analysis::TypeLocal & ty, const std::string & name)
{
    return getScalar(ty.type, ty.isAnInt, name);
}

JITScilabPtr JITVisitor::getScalar(const analysis::TIType::Type ty, const bool isAnInt, const std::string & name)
{
    switch (ty)
    {
        case analysis::TIType::DOUBLE:
            if (isAnInt)
            {
                return std::shared_ptr<JITScilabVal>(new JITScalInt64(*this, name));
            }
            else
            {
                return std::shared_ptr<JITScilabVal>(new JITScalDouble(*this, name));
            }
        case analysis::TIType::COMPLEX:
            return std::shared_ptr<JITScilabVal>(new JITScalComplex(*this, name));
        case analysis::TIType::INT8:
            return std::shared_ptr<JITScilabVal>(new JITScalInt8(*this, name));
        case analysis::TIType::INT16:
            return std::shared_ptr<JITScilabVal>(new JITScalInt16(*this, name));
        case analysis::TIType::INT32:
            return std::shared_ptr<JITScilabVal>(new JITScalInt32(*this, name));
        case analysis::TIType::INT64:
            return std::shared_ptr<JITScilabVal>(new JITScalInt64(*this, name));
        case analysis::TIType::UINT8:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt8(*this, name));
        case analysis::TIType::UINT16:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt16(*this, name));
        case analysis::TIType::UINT32:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt32(*this, name));
        case analysis::TIType::UINT64:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt64(*this, name));
        case analysis::TIType::BOOLEAN:
            return std::shared_ptr<JITScilabVal>(new JITScalBool(*this, name));
        default:
            return nullptr;
    }
}

/*JITScilabPtr JITVisitor::getMatrix(llvm::Value * const re, llvm::Value * const im, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const analysis::TIType::Type ty, const bool alloc, const std::string & name)
{

}*/

JITScilabPtr JITVisitor::getMatrix(llvm::Value * const value, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const analysis::TIType::Type ty, const bool alloc, const std::string & name)
{
    switch (ty)
    {
        case analysis::TIType::DOUBLE:
            return std::shared_ptr<JITScilabVal>(new JITArrayofDouble(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::INT8:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt8(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::INT16:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt16(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::INT32:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt32(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::INT64:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt64(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::UINT8:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt8(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::UINT16:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt16(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::UINT32:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt32(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::UINT64:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt64(*this, value, rows, cols, refCount, alloc, name));
        case analysis::TIType::BOOLEAN:
            return std::shared_ptr<JITScilabVal>(new JITArrayofBool(*this, value, rows, cols, refCount, alloc, name));
        default:
            return nullptr;
    }
}

JITScilabPtr JITVisitor::getMatrix(const analysis::TIType::Type ty, const std::string & name, const bool init)
{
    switch (ty)
    {
        case analysis::TIType::DOUBLE:
            return std::shared_ptr<JITScilabVal>(new JITArrayofDouble(*this, name, init));
        case analysis::TIType::INT8:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt8(*this, name, init));
        case analysis::TIType::INT16:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt16(*this, name, init));
        case analysis::TIType::INT32:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt32(*this, name, init));
        case analysis::TIType::INT64:
            return std::shared_ptr<JITScilabVal>(new JITArrayofInt64(*this, name, init));
        case analysis::TIType::UINT8:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt8(*this, name, init));
        case analysis::TIType::UINT16:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt16(*this, name, init));
        case analysis::TIType::UINT32:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt32(*this, name, init));
        case analysis::TIType::UINT64:
            return std::shared_ptr<JITScilabVal>(new JITArrayofUInt64(*this, name, init));
        case analysis::TIType::BOOLEAN:
            return std::shared_ptr<JITScilabVal>(new JITArrayofBool(*this, name, init));
        default:
            return nullptr;
    }
}

JITScilabPtr JITVisitor::getMatrix(const analysis::TypeLocal & ty, const std::string & name, const bool init)
{
    return getMatrix(ty.type, name, init);
}

void JITVisitor::action(analysis::FunctionBlock & fblock)
{
    variables.clear();
    temps.clear();

    //fblocks.emplace(&fblock);
    std::string name(fblock.getName().begin(), fblock.getName().end());
    const std::vector<std::pair<symbol::Symbol, analysis::TypeLocal>> ins = fblock.getTypesIn();
    const std::vector<std::pair<symbol::Symbol, analysis::TypeLocal>> outs = fblock.getTypesOut();
    mapNameFBlock.emplace(name, &fblock);
    std::string _name(name);

    // Firstly, we create the function signature
    llvm::Type * retTy = voidTy;
    std::vector<llvm::Type *> args;
    args.reserve(4 * (ins.size() + outs.size()));
    for (const auto & in : ins)
    {
        const analysis::TIType::Type ty = in.second.type;
        const bool scalar = in.second.isScalar();
        if (ty == analysis::TIType::COMPLEX)
        {
            llvm::Type * _ty = scalar ? dblTy : dblPtrTy;
            args.emplace_back(_ty);
            args.emplace_back(_ty);
        }
        else
        {
            args.emplace_back(getType(ty, scalar));
        }
        if (!scalar)
        {
            args.emplace_back(int64Ty); // for rows
            args.emplace_back(int64Ty); // for cols
            args.emplace_back(int64Ty); // for refcount
        }
        name += "_" + analysis::TIType::get_mangling(ty, scalar);
    }
    for (const auto & out : outs)
    {
        const analysis::TIType::Type ty = out.second.type;
        const bool scalar = out.second.isScalar();

        // Output arguments are passed by reference
        if (ty == analysis::TIType::COMPLEX)
        {
            llvm::Type * _ty = scalar ? dblPtrTy : llvm::PointerType::getUnqual(dblPtrTy);
            args.emplace_back(_ty);
            args.emplace_back(_ty);
        }
        else
        {
            args.emplace_back(llvm::PointerType::getUnqual(getType(ty, scalar)));
        }
        if (!scalar)
        {
            args.emplace_back(int64PtrTy); // for rows
            args.emplace_back(int64PtrTy); // for cols
            args.emplace_back(int64PtrTy); // for refcount
        }
    }

    llvm::FunctionType * ftype = llvm::FunctionType::get(retTy, llvm::ArrayRef<llvm::Type *>(args), /* isVarArgs */ false);
    //function = llvm::cast<llvm::Function>(module.getOrInsertFunction("jit_" + name, ftype));
    function = llvm::cast<llvm::Function>(module->getOrInsertFunction(_name, ftype));

    entryBlock = llvm::BasicBlock::Create(context, "EntryBlock", function);
    builder.SetInsertPoint(entryBlock);
    mainBlock = llvm::BasicBlock::Create(context, "MainBlock", function);
    returnBlock = llvm::BasicBlock::Create(context, "returnBlock", function);
    //errorBlock = llvm::BasicBlock::Create(context, "errorBlock", function);

    llvm::Function::arg_iterator ai = function->arg_begin();
    for (const auto & in : ins)
    {
        const analysis::TIType::Type ty = in.second.type;
        const bool scalar = in.second.isScalar();
        const std::string name(in.first.getName().begin(), in.first.getName().end());
        if (scalar)
        {
            if (ty == analysis::TIType::COMPLEX)
            {
                llvm::Value * re = ai++;
                llvm::Value * im = ai++;

                variables.emplace(in.first, getScalar(re, im, ty, true, name));
            }
            else
            {
                variables.emplace(in.first, getScalar(ai++, ty, true, name));
            }
        }
        else
        {
            llvm::Value * M = ai++;
            llvm::Value * R = ai++;
            llvm::Value * C = ai++;
            llvm::Value * RC = ai++;
            variables.emplace(in.first, getMatrix(M, R, C, RC, ty, true, name));
        }
    }

    for (const auto & out : outs)
    {
        const analysis::TIType::Type ty = out.second.type;
        const bool scalar = out.second.isScalar();
        const std::string name(out.first.getName().begin(), out.first.getName().end());
        if (scalar)
        {
            JITScilabPtr & ptr = variables.emplace(out.first, getScalar(ty, /* isAnInt */ false, name)).first->second;

            builder.SetInsertPoint(returnBlock);
            if (ty == analysis::TIType::COMPLEX)
            {
                llvm::Value * re = ai++;
                llvm::Value * im = ai++;
                JITScalComplex * cpx_ptr = static_cast<JITScalComplex *>(ptr.get());
                builder.CreateAlignedStore(cpx_ptr->loadReal(*this), re, sizeof(void *));
                builder.CreateAlignedStore(cpx_ptr->loadImag(*this), im, sizeof(void *));
            }
            else
            {
                builder.CreateAlignedStore(ptr->loadData(*this), ai++, sizeof(void *));
            }
            builder.SetInsertPoint(entryBlock);
        }
        else
        {
            llvm::Value * M = ai++;
            llvm::Value * R = ai++;
            llvm::Value * C = ai++;
            llvm::Value * RC = ai++;

            JITScilabPtr & ptr = variables.emplace(out.first, getMatrix(ty, name)).first->second;

            builder.SetInsertPoint(returnBlock);
            builder.CreateAlignedStore(ptr->loadData(*this), M, sizeof(void *));
            builder.CreateAlignedStore(ptr->loadRows(*this), R, sizeof(int64_t));
            builder.CreateAlignedStore(ptr->loadCols(*this), C, sizeof(int64_t));
            builder.SetInsertPoint(entryBlock);
            //builder.CreateAlignedStore(ptr->loadRefCount(*this), RC, sizeof(int64_t));
            //funOuts.emplace_back(getMatrix(M, R, C, RC, ty, false, ""));
        }
    }

    // Secondly, we need to create the variables used in the function.
    const analysis::tools::SymbolMap<std::set<analysis::TypeLocal>> & locals = fblock.getTypesLocals();
    for (const auto & local : locals)
    {
        const std::string name(local.first.getName().begin(), local.first.getName().end());
        for (const auto & ty : local.second)
        {
            if (ty.isScalar())
            {
                variables.emplace(local.first, getScalar(ty, name));
            }
            else
            {
                variables.emplace(local.first, getMatrix(ty, name));
            }
        }
    }

    // Thirdly, we need to create the temporaries.
    int total = 0;
    const std::map<analysis::TypeLocal, std::stack<int>> & temporaries = fblock.getTemp();
    for (const auto & p : temporaries)
    {
        total += p.second.size();
    }
    if (total > 0)
    {
        temps.resize(total);
        unsigned int id = 0;
        for (const auto & p : temporaries)
        {
            const analysis::TypeLocal & ty = p.first;
            stack<int> stack(p.second);
            while (!stack.empty())
            {
                const std::string name = std::to_string(id++) + "_tmp";
                if (ty.isScalar())
                {
                    temps[stack.top()] = getScalar(ty, name);
                }
                else
                {
                    temps[stack.top()] = getMatrix(ty, name, true);
                }
                stack.pop();
            }
        }
    }

    cpx_rvalue.reset(new JITScalComplex(*this, "0_cpx_rvalue"));

    //function->dump();

    builder.SetInsertPoint(mainBlock);

    fblock.getExp()->accept(*this);
    //function->dump();
    CreateBr(returnBlock);

    builder.SetInsertPoint(returnBlock);
    builder.CreateRetVoid();

    //function->dump();


    //module.dump();
}

void JITVisitor::makeCall(const std::wstring & name, const std::vector<types::InternalType *> & in, std::vector<types::InternalType *> & out)
{
    std::vector<llvm::Value *> args;
    std::string _name(name.begin(), name.end());

    for (auto pIT : in)
    {
        if (pIT->isGenericType())
        {
            types::GenericType * pGT = static_cast<types::GenericType *>(pIT);
            if (pGT->isScalar())
            {
                switch (pGT->getType())
                {
                    case types::InternalType::ScilabInt8:
                    {
                        const int8_t x = static_cast<types::Int8 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabUInt8:
                    {
                        const uint8_t x = static_cast<types::UInt8 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabInt16:
                    {
                        const int16_t x = static_cast<types::Int16 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabUInt16:
                    {
                        const uint16_t x = static_cast<types::UInt16 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabInt32:
                    {
                        const int32_t x = static_cast<types::Int32 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabUInt32:
                    {
                        const uint32_t x = static_cast<types::UInt32 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabInt64:
                    {
                        const int64_t x = static_cast<types::Int64 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabUInt64:
                    {
                        const uint64_t x = static_cast<types::UInt64 *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    case types::InternalType::ScilabDouble:
                        if (pGT->isComplex())
                        {
                            const double re = static_cast<types::Double *>(pGT)->getReal()[0];
                            const double im = static_cast<types::Double *>(pGT)->getImg()[0];
                            args.emplace_back(getValue(re));
                            args.emplace_back(getValue(im));
                            //args.emplace_back(getValue(std::complex<double>(re, im)));
                        }
                        else
                        {
                            const double x = static_cast<types::Double *>(pGT)->get(0);
                            args.emplace_back(getValue(x));
                        }
                        break;
                    case types::InternalType::ScilabBool:
                    {
                        const int32_t x = static_cast<types::Bool *>(pGT)->get(0);
                        args.emplace_back(getValue(x));
                        break;
                    }
                    default:
                        break;
                }
            }
            else
            {
                switch (pGT->getType())
                {
                    case types::InternalType::ScilabInt8:
                    {
                        makeArg<int8_t, types::Int8>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabUInt8:
                    {
                        makeArg<uint8_t, types::UInt8>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabInt16:
                    {
                        makeArg<int16_t, types::Int16>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabUInt16:
                    {
                        makeArg<uint16_t, types::UInt16>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabInt32:
                    {
                        makeArg<int32_t, types::Int32>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabUInt32:
                    {
                        makeArg<uint32_t, types::UInt32>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabInt64:
                    {
                        makeArg<int64_t, types::Int64>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabUInt64:
                    {
                        makeArg<uint64_t, types::UInt64>(args, pGT);
                        break;
                    }
                    case types::InternalType::ScilabDouble:
                        if (pGT->isComplex())
                        {
                        }
                        else
                        {
                            makeArg<double, types::Double>(args, pGT);
                        }
                        break;
                    case types::InternalType::ScilabBool:
                    {
                        makeArg<int32_t, types::Bool>(args, pGT);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    // now we put the output
    const std::vector<analysis::TIType> outs = mapNameFBlock.find(_name)->second->getOuts().tuple.types;
    const analysis::TIType & ty = outs.front();
    std::vector<OutContainer> llvmOuts;
    llvmOuts.reserve(out.size());
    llvmOuts.emplace_back(ty.type);

    if (ty.isscalar())
    {
        switch (ty.type)
        {
            case analysis::TIType::DOUBLE:
            {
                args.emplace_back(getValue(&llvmOuts.back().data.dbl));
                llvmOuts.back().rows = 1;
                llvmOuts.back().cols = 1;
                break;
            }
            case analysis::TIType::COMPLEX:
            {
                args.emplace_back(getValue(&llvmOuts.back().data.cpx[0]));
                args.emplace_back(getValue(&llvmOuts.back().data.cpx[1]));
                llvmOuts.back().rows = 1;
                llvmOuts.back().cols = 1;
                break;
            }
            case analysis::TIType::BOOLEAN:
            {
                args.emplace_back(getValue(&llvmOuts.back().data.boolean));
                llvmOuts.back().rows = 1;
                llvmOuts.back().cols = 1;
                break;
            }
            default:
                break;
        }
    }
    else
    {
        switch (ty.type)
        {
            case analysis::TIType::DOUBLE:
            {
                args.emplace_back(getValue(reinterpret_cast<double **>(&llvmOuts.back().data.ptr)));
                args.emplace_back(getValue(&llvmOuts.back().rows));
                args.emplace_back(getValue(&llvmOuts.back().cols));
                args.emplace_back(getValue(&llvmOuts.back().refcount));
                break;

                /*types::Double * pDbl = new types::Double();
                double ** x = &pDbl->m_pRealData;
                args.emplace_back(getValue(x));
                int32_t * r = &pDbl->m_iRows;
                args.emplace_back(getValue(r));
                int32_t * c = &pDbl->m_iCols;
                args.emplace_back(getValue(c));
                int32_t * refc = &pDbl->m_iRef;
                args.emplace_back(getValue(refc));
                out.push_back(pDbl);*/
            }
            default:
                break;
        }
    }

    llvm::Type * voidTy = getTy<void>();
    llvm::Function * toCall = module->getFunction(_name);
    llvm::Function * function = llvm::cast<llvm::Function>(module->getOrInsertFunction("main", voidTy, nullptr));
    llvm::BasicBlock * BB = llvm::BasicBlock::Create(context, "EntryBlock", function);
    builder.SetInsertPoint(BB);
    builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
    builder.CreateRetVoid();

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;

    closeEntryBlock();

    module->dump();

    runOptimizationPasses();

    module->dump();

    compileModule();

    std::wcerr << "main ptr: " << (void *)(intptr_t)engine->getFunctionAddress("main") << std::endl;

    start = std::chrono::steady_clock::now();
    reinterpret_cast<void (*)()>(engine->getFunctionAddress("main"))();
    end = std::chrono::steady_clock::now();
    double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    std::wcerr << "Exec time=" << duration << " s." << std::endl;

    types::InternalType * pIT = nullptr;
    if (ty.isscalar())
    {
        if (ty.type == analysis::TIType::COMPLEX)
        {
            //std::wcerr << "WTF=" << llvmOuts.back().data.cpx[1] << std::endl;
            pIT = new types::Double(llvmOuts.back().data.cpx[0], llvmOuts.back().data.cpx[1]);
        }
        else if (ty.type == analysis::TIType::DOUBLE)
        {
            pIT = new types::Double(llvmOuts.back().data.dbl);
        }
        else if (ty.type == analysis::TIType::BOOLEAN)
        {
            pIT = new types::Bool(llvmOuts.back().data.boolean);
        }
    }
    else
    {
        pIT = new types::Double(llvmOuts.back().rows, llvmOuts.back().cols, reinterpret_cast<double *>(llvmOuts.back().data.ptr));
    }

    out.emplace_back(pIT);
}

llvm::FunctionType * JITVisitor::getFunctionType(const analysis::TIType & out, const std::vector<const analysis::TIType *> & types)
{
    llvm::Type * out_ty;
    std::vector<llvm::Type *> args;
    if (out.type == analysis::TIType::COMPLEX)
    {
        out_ty = getTy<void>();
    }
    else
    {
        out_ty = getTy(out);
    }
    for (const auto type : types)
    {
        if (type->isscalar())
        {
            if (type->type == analysis::TIType::COMPLEX)
            {
                args.emplace_back(getTy<double>());
                args.emplace_back(getTy<double>());
            }
            else
            {
                args.emplace_back(getTy(*type));
            }
        }
        else
        {
            if (type->type == analysis::TIType::COMPLEX)
            {
                args.emplace_back(getTy<double *>());
                args.emplace_back(getTy<double *>());
            }
            else
            {
                args.emplace_back(getTy(*type, 1));
            }
            args.emplace_back(getTy<int64_t>());
            args.emplace_back(getTy<int64_t>());
        }
    }
    if (out.type == analysis::TIType::COMPLEX)
    {
        if (out.isscalar())
        {
            args.emplace_back(getTy<double *>());
            args.emplace_back(getTy<double *>());
        }
        else
        {
            args.emplace_back(getTy<double **>());
            args.emplace_back(getTy<double **>());
            args.emplace_back(getTy<int64_t *>());
            args.emplace_back(getTy<int64_t *>());
        }
    }

    return llvm::FunctionType::get(out_ty, llvm::ArrayRef<llvm::Type *>(args), false);
}

llvm::Type * JITVisitor::getPtrAsIntTy(llvm::Module & module, llvm::LLVMContext & ctxt)
{
#if LLVM_VERSION_MAJOR >= 3 && LLVM_VERSION_MINOR == 4
    return module.getPointerSize() == llvm::Module::Pointer32 ? llvm::Type::getInt32Ty(ctxt) : llvm::Type::getInt64Ty(ctxt);
#else
    return module.getDataLayout()->getPointerSize() == 32 ? llvm::Type::getInt32Ty(ctxt) : llvm::Type::getInt64Ty(ctxt);
#endif
}

bool JITVisitor::InitializeLLVM()
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    return true;
}

llvm::ExecutionEngine * JITVisitor::InitializeEngine(llvm::Module * module, llvm::TargetMachine ** target)
{
    std::string err;
    llvm::TargetOptions opt;
    opt.NoFramePointerElim = true;
    llvm::EngineBuilder & eb = llvm::EngineBuilder(module).setErrorStr(&err).setUseMCJIT(true).setMCJITMemoryManager(new MemoryManager()).setRelocationModel(llvm::Reloc::PIC_/*DynamicNoPIC*//*Static*/).setCodeModel(llvm::CodeModel::Small).setTargetOptions(opt);
    // TODO: when reloc model is Static there is a problem with address of global variables (used with dgemm_)

    *target = eb.selectTarget();
    llvm::ExecutionEngine * engine = eb.create(*target);
    engine->RegisterJITEventListener(new ScilabJITEventListener());

    module->setDataLayout(engine->getDataLayout()->getStringRepresentation());
    module->setTargetTriple((*target)->getTargetTriple().str());

    return engine;
}

llvm::FunctionPassManager JITVisitor::initFPM(llvm::Module * module, llvm::ExecutionEngine * engine, llvm::TargetMachine * target)
{
    llvm::FunctionPassManager FPM(module);

#if LLVM_VERSION_MAJOR >= 3 && LLVM_VERSION_MINOR == 4
    FPM.add(new llvm::DataLayout(*engine->getDataLayout()));
#else
    FPM.add(new llvm::DataLayoutPass(*engine->getDataLayout()));
#endif

    target->addAnalysisPasses(FPM);

    // TODO: mettre les bonnes passes la ou il faut

    FPM.add(llvm::createBasicAliasAnalysisPass());
    FPM.add(llvm::createTypeBasedAliasAnalysisPass());
    FPM.add(llvm::createCFGSimplificationPass());
    FPM.add(llvm::createAggressiveDCEPass());

    FPM.add(llvm::createPromoteMemoryToRegisterPass()); // remove useless alloca
    FPM.add(llvm::createInstructionCombiningPass()); // clean
    //FPM.add(llvm::createScalarReplAggregatesPass());
    //FPM.add(llvm::createInstructionCombiningPass());
    FPM.add(llvm::createJumpThreadingPass()); // thread jumps.
    FPM.add(llvm::createInstructionCombiningPass()); // clean
    FPM.add(llvm::createReassociatePass()); // use associativity to simplify
    FPM.add(llvm::createEarlyCSEPass()); // common sub-expression elimination
    FPM.add(llvm::createLoopIdiomPass()); // replace certains for-loop by memset/memcpy
    FPM.add(llvm::createLoopRotatePass()); // ??
    FPM.add(llvm::createLICMPass()); // Loop Invariant Code Motion
    FPM.add(llvm::createLoopUnswitchPass()); // For-If are permuted
    FPM.add(llvm::createInstructionCombiningPass()); // clean
    FPM.add(llvm::createIndVarSimplifyPass()); // Transform induction var in loops into something like (i = 0; i != A; ++i)
    FPM.add(llvm::createLoopDeletionPass()); // remove dead loops
    //FPM.add(llvm::createLoopStrengthReducePass());
    //FPM.add(llvm::createLoopUnrollPass(-1, -1, -1 /* allows partial unrolling (1 for unrolling) */, -1)); // unroll small loops
    FPM.add(llvm::createBBVectorizePass()); // vectorize loops
    FPM.add(llvm::createInstructionCombiningPass());
    FPM.add(llvm::createGVNPass());
    FPM.add(llvm::createLoopUnrollPass());
    FPM.add(llvm::createAggressiveDCEPass());

    FPM.add(llvm::createLoopVectorizePass()); // vectorize loops
    FPM.add(llvm::createInstructionCombiningPass()); //clean
    FPM.add(llvm::createSLPVectorizerPass());
    FPM.add(llvm::createGVNPass()); // global value numbering
    FPM.add(llvm::createSCCPPass()); // Sparse Conditional Constant Propagation
    FPM.add(llvm::createSinkingPass());
    FPM.add(llvm::createInstructionSimplifierPass());
    FPM.add(llvm::createInstructionCombiningPass());
    FPM.add(llvm::createDeadInstEliminationPass());
    FPM.add(llvm::createDeadStoreEliminationPass());
    FPM.add(llvm::createAggressiveDCEPass());

    FPM.doInitialization();

    /*
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

        FPM.doInitialization();*/

    return FPM;
}
}
