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
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Analysis/TargetTransformInfo.h"

//#undef NDEBUG
//#include "llvm/Support/Debug.h"

#include "MemoryManager.hxx"
#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "Debug.hxx"
#include "calls/FunctionSignature.hxx"
#include "alltypes.hxx"
#include "ScilabJITEventListener.hxx"
#include "UTF8.hxx"

#define TIME_LLVM 0

namespace jit
{
const bool JITVisitor::__init__ = InitializeLLVM();

JITVisitor::JITVisitor() : ast::ConstVisitor(),
    context(llvm::getGlobalContext()),
    module(new llvm::Module("JIT0", context)),
    target(nullptr),
    engine(InitializeEngine(module, &target)),
    MPM(),
    FPM(module),
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
    id(1),
    _result(nullptr),
    cpx_rvalue(nullptr)
{
    initPassManagers();

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
}

void JITVisitor::runOptimizationPasses()
{
#if TIME_LLVM == 1
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif

    FPM.doInitialization();
    for (llvm::Function & f : *module)
    {
        FPM.run(f);
    }
    FPM.doFinalization();
    MPM.run(*module);

#if TIME_LLVM == 1
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
    std::wcerr << "Optimization time=" << duration << " s." << std::endl;
#endif
}

void JITVisitor::compile()
{
    /*std::string error;
    llvm::raw_fd_ostream stream("/tmp/scilab.s", error, llvm::sys::fs::OpenFlags::F_None);
    llvm::formatted_raw_ostream frs(stream);
    llvm::PassManager PM;
    target->addPassesToEmitFile(PM, frs, llvm::TargetMachine::CGFT_AssemblyFile);
    PM.run(*module);
    frs.flush();
    stream.close();*/

    if (function)
    {
	dump();
	runOptimizationPasses();
	//dump();
#if TIME_LLVM == 1
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#endif
    
	engine->finalizeObject();
    
#if TIME_LLVM == 1
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	double duration = (double)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() * 1e-9;
	std::wcerr << "Compile time=" << duration << " s." << std::endl;
#endif
	
	for (llvm::Function & f : *module)
	{
	    //addFunction(f.getName().str(), &f);
	    f.deleteBody();
	}
    }
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

void JITVisitor::visit(const ast::TryCatchExp & e)
{

}

void JITVisitor::visit(const ast::ReturnExp & e)
{

}

void JITVisitor::visit(const ast::FieldExp & e)
{

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

    JITScilabPtr JITVisitor::getCreatedScalar(llvm::Value * const value, const analysis::TIType::Type ty, const bool alloc, const std::string & name)
{
    switch (ty)
    {
        case analysis::TIType::DOUBLE:
            return std::shared_ptr<JITScilabVal>(new JITScalDouble(value, alloc, name));
        case analysis::TIType::INT8:
            return std::shared_ptr<JITScilabVal>(new JITScalInt8(value, alloc, name));
        case analysis::TIType::INT16:
            return std::shared_ptr<JITScilabVal>(new JITScalInt16(value, alloc, name));
        case analysis::TIType::INT32:
            return std::shared_ptr<JITScilabVal>(new JITScalInt32(value, alloc, name));
        case analysis::TIType::INT64:
            return std::shared_ptr<JITScilabVal>(new JITScalInt64(value, alloc, name));
        case analysis::TIType::UINT8:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt8(value, alloc, name));
        case analysis::TIType::UINT16:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt16(value, alloc, name));
        case analysis::TIType::UINT32:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt32(value, alloc, name));
        case analysis::TIType::UINT64:
            return std::shared_ptr<JITScilabVal>(new JITScalUInt64(value, alloc, name));
        case analysis::TIType::BOOLEAN:
            return std::shared_ptr<JITScilabVal>(new JITScalBool(value, alloc, name));
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

JITScilabPtr JITVisitor::getMatrix(llvm::Value * const re, llvm::Value * const im, llvm::Value * const rows, llvm::Value * const cols, llvm::Value * const refCount, const analysis::TIType::Type ty, const bool alloc, const std::string & name)
{
    return std::shared_ptr<JITScilabVal>(new JITArrayofComplex(*this, re, im, rows, cols, refCount, alloc, name));
}

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
        case analysis::TIType::COMPLEX:
	    return std::shared_ptr<JITScilabVal>(new JITArrayofComplex(*this, name, init));
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
    if (info.find(fblock.getFunctionId()) != info.end())
    {
	// The function with this id has already been compiled
	return;
    }
	    
    variables.clear();
    temps.clear();
    specialVars.clear();

    //fblocks.emplace(&fblock);
    std::string functionName = "jit_" + scilab::UTF8::toUTF8(fblock.getName()) + "_";
    const std::vector<analysis::ArgIOInfo> ins = fblock.getTypesIn();
    const std::vector<analysis::ArgIOInfo> outs = fblock.getTypesOut();

    // Firstly, we create the function signature
    std::vector<llvm::Type *> args;
    args.reserve(4 * (ins.size() + outs.size()));
    
    for (const auto & in : ins)
    {
        const bool scalar = in.tl.isScalar();
	if (scalar)
	{
	    In<analysis::TIType::Type>(in.tl.type).get(args, *this);
	}
	else
	{
	    In<analysis::TIType::Type, 1>(in.tl.type).get(args, *this);
	    In<llvm::Type>(int64Ty).get(args, *this); // for rows
	    In<llvm::Type>(int64Ty).get(args, *this); // for cols
	    In<llvm::Type>(int64Ty).get(args, *this); // for refcount
	}
	
        functionName += analysis::TIType::get_mangling(in.tl.type, scalar);
    }

    JITInfo & functionInfo = info.emplace(fblock.getFunctionId(), functionName).first->second;

    for (const auto & in : ins)
    {
	functionInfo.addToInSignature(in.tl.isScalar(), in.tl.type);
    }
    
    for (const auto & out : outs)
    {
        const bool scalar = out.tl.isScalar();
	if (scalar)
	{
	    In<analysis::TIType::Type, 1>(out.tl.type).get(args, *this);
	}
	else
	{
	    In<analysis::TIType::Type, 2>(out.tl.type).get(args, *this);
	    In<llvm::Type>(int64PtrTy).get(args, *this); // for rows
	    In<llvm::Type>(int64PtrTy).get(args, *this); // for cols
	    In<llvm::Type>(int64PtrTy).get(args, *this); // for refcount
	}
	functionInfo.addToOutSignature(scalar, out.tl.type);
    }

    llvm::FunctionType * ftype = llvm::FunctionType::get(voidTy, args, /* isVarArgs */ false);
    function = llvm::cast<llvm::Function>(module->getOrInsertFunction(functionName, ftype));

    entryBlock = llvm::BasicBlock::Create(context, "EntryBlock", function);
    builder.SetInsertPoint(entryBlock);
    mainBlock = llvm::BasicBlock::Create(context, "MainBlock", function);
    returnBlock = llvm::BasicBlock::Create(context, "returnBlock", function);
    //errorBlock = llvm::BasicBlock::Create(context, "errorBlock", function);

    llvm::Function::arg_iterator ai = function->arg_begin();
    for (const auto & in : ins)
    {
        const analysis::TIType::Type ty = in.tl.type;
        const bool scalar = in.tl.isScalar();
	const std::string argName = scilab::UTF8::toUTF8(in.sym.getName());
        if (scalar)
        {
	    if (ty == analysis::TIType::COMPLEX)
            {
                llvm::Value * re = ai++;
                llvm::Value * im = ai++;

                variables.emplace(in.sym, getScalar(re, im, ty, true, argName));
            }
            else
            {
                variables.emplace(in.sym, getScalar(ai++, ty, true, argName));
            }
        }
        else
        {
            if (ty == analysis::TIType::COMPLEX)
            {
		llvm::Value * re = ai++;
		llvm::Value * im = ai++;
		llvm::Value * R = ai++;
		llvm::Value * C = ai++;
		llvm::Value * RC = ai++;
		variables.emplace(in.sym, getMatrix(re, im, R, C, RC, ty, true, argName));
	    }
	    else
	    {
		llvm::Value * M = ai++;
		llvm::Value * R = ai++;
		llvm::Value * C = ai++;
		llvm::Value * RC = ai++;
		variables.emplace(in.sym, getMatrix(M, R, C, RC, ty, true, argName));
	    }
        }
    }

    for (const auto & out : outs)
    {
        const analysis::TIType::Type ty = out.tl.type;
        const bool scalar = out.tl.isScalar();
        const std::string name(out.sym.getName().begin(), out.sym.getName().end());
        if (scalar)
        {
            JITScilabPtr & ptr = variables.emplace(out.sym, getScalar(ty, /* isAnInt */ false, name)).first->second;
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
	    JITScilabPtr & ptr = variables.emplace(out.sym, getMatrix(ty, name)).first->second;
            builder.SetInsertPoint(returnBlock);
	    if (ty == analysis::TIType::COMPLEX)
            {
                llvm::Value * re = ai++;
                llvm::Value * im = ai++;
		llvm::Value * R = ai++;
		llvm::Value * C = ai++;
		llvm::Value * RC = ai++;
		builder.CreateAlignedStore(ptr->loadReal(*this), re, sizeof(void *));
		builder.CreateAlignedStore(ptr->loadImag(*this), im, sizeof(void *));
		builder.CreateAlignedStore(ptr->loadRows(*this), R, sizeof(int64_t));
		builder.CreateAlignedStore(ptr->loadCols(*this), C, sizeof(int64_t));
	    }
	    else
	    {
                llvm::Value * M = ai++;
		llvm::Value * R = ai++;
		llvm::Value * C = ai++;
		llvm::Value * RC = ai++;
		builder.CreateAlignedStore(ptr->loadData(*this), M, sizeof(void *));
		builder.CreateAlignedStore(ptr->loadRows(*this), R, sizeof(int64_t));
		builder.CreateAlignedStore(ptr->loadCols(*this), C, sizeof(int64_t));
	    }
	    builder.SetInsertPoint(entryBlock);
		
            //builder.CreateAlignedStore(ptr->loadRefCount(*this), RC, sizeof(int64_t));
            //funOuts.emplace_back(getMatrix(M, R, C, RC, ty, false, ""));
        }
    }

    // Secondly, we need to create the variables used in the function.
    const analysis::tools::SymbolMap<analysis::LocalInfo> & locals = fblock.getTypesLocals();
    for (const auto & local : locals)
    {
        const std::string name(local.first.getName().begin(), local.first.getName().end());
        for (const auto & ty : local.second.set)
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
    if (total >= 0)
    {
        temps.resize(total + 1);
	temps[0] = JITScilabPtr(nullptr);
        unsigned int id = 0;
        for (const auto & p : temporaries)
        {
            const analysis::TypeLocal & ty = p.first;
            std::stack<int> stack(p.second);
            while (!stack.empty())
            {
                const std::string name = std::to_string(id++) + "_tmp";
                if (ty.isScalar())
                {
                    temps[stack.top() + 1] = getScalar(ty, name);
                }
                else
                {
                    temps[stack.top() + 1] = getMatrix(ty, name, true);
                }
                stack.pop();
            }
        }
    }

    cpx_rvalue.reset(new JITScalComplex(*this, "0_cpx_rvalue"));

    builder.SetInsertPoint(mainBlock);


    //Debug::printI64(*this, variables.find(symbol::Symbol(L"a"))->second->loadRows(*this));

    fblock.getExp()->accept(*this);
    CreateBr(returnBlock);
    builder.SetInsertPoint(returnBlock);
    builder.CreateRetVoid();

    closeEntryBlock();

    addFunction(function->getName().str(), function);
    
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

    return llvm::FunctionType::get(out_ty, args, false);
}

llvm::Type * JITVisitor::getPtrAsIntTy(llvm::Module & module, llvm::LLVMContext & ctxt)
{
    return module.getDataLayout().getPointerSize() == 32 ? llvm::Type::getInt32Ty(ctxt) : llvm::Type::getInt64Ty(ctxt);
}

bool JITVisitor::InitializeLLVM()
{
    llvm::PassRegistry & Registry = *llvm::PassRegistry::getPassRegistry();
    llvm::initializeCore(Registry);
    llvm::initializeScalarOpts(Registry);
    llvm::initializeObjCARCOpts(Registry);
    llvm::initializeVectorization(Registry);
    llvm::initializeIPO(Registry);
    llvm::initializeAnalysis(Registry);
    llvm::initializeTransformUtils(Registry);
    llvm::initializeInstCombine(Registry);
    llvm::initializeInstrumentation(Registry);
    llvm::initializeTarget(Registry);

    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    return true;
}

llvm::ExecutionEngine * JITVisitor::InitializeEngine(llvm::Module * module, llvm::TargetMachine ** target)
{
    std::string err;
    llvm::TargetOptions opt;
    llvm::EngineBuilder eb(std::move(std::unique_ptr<llvm::Module>(module)));
    eb.setEngineKind(llvm::EngineKind::JIT).setMCJITMemoryManager(std::move(std::unique_ptr<llvm::RTDyldMemoryManager> {new MemoryManager()})).setOptLevel(llvm::CodeGenOpt::Aggressive).setErrorStr(&err).setMCPU(llvm::sys::getHostCPUName());

    llvm::TargetMachine * tm = eb.selectTarget();
    llvm::Triple triple(llvm::sys::getProcessTriple());

    // The following line is mandatory for Windows OS
    triple.setObjectFormat(llvm::Triple::ELF);
    *target = tm->getTarget().createTargetMachine(triple.getTriple(), tm->getTargetCPU(), tm->getTargetFeatureString(), tm->Options, llvm::Reloc::Default, llvm::CodeModel::Default, llvm::CodeGenOpt::Level::Aggressive);
    delete tm;

    llvm::ExecutionEngine * engine = eb.create(*target);
    engine->setVerifyModules(false);
    engine->RegisterJITEventListener(new ScilabJITEventListener());

    module->setDataLayout(engine->getDataLayout()->getStringRepresentation());
    module->setTargetTriple((*target)->getTargetTriple().str());

    return engine;
}

    void JITVisitor::reset()
    {
	entryBlock = nullptr;
	mainBlock = nullptr;
	returnBlock = nullptr;
	errorBlock = nullptr;

	_result.reset();
	cpx_rvalue.reset();
	multipleLHS.clear();
	variables.clear();
	temps.clear();
	globals.clear();
	while (!blocks.empty())
	{
	    blocks.pop();
	}
	specialVars.clear();

	module = new llvm::Module("JIT" + std::to_string(id++), context);
	engine->addModule(std::unique_ptr<llvm::Module>(module));
	module->setDataLayout(*engine->getDataLayout());
	module->setTargetTriple(target->getTargetTriple().str());
	function = nullptr;
    }

void JITVisitor::initPassManagers()
{
    /* This pass is mandatory to detect register size to allow vectorization */
    MPM.add(llvm::createTargetTransformInfoWrapperPass(target->getTargetIRAnalysis()));
    FPM.add(llvm::createTargetTransformInfoWrapperPass(target->getTargetIRAnalysis()));

    llvm::PassManagerBuilder PMB;
    PMB.OptLevel = 2;
    PMB.SizeLevel = 0;
    /*PMB.BBVectorize = true;
    PMB.LoopVectorize = true;
    PMB.SLPVectorize = true;
    PMB.LoadCombine = true;
    PMB.DisableUnrollLoops = false;*/
    //PMB.RerollLoops = false;
    //PMB.DisableGVNLoadPRE = true;
    PMB.DisableUnitAtATime = false;
    PMB.populateFunctionPassManager(FPM);
    PMB.populateModulePassManager(MPM);

    //llvm::setCurrentDebugType("loop-vectorize");
    //llvm::DebugFlag = true;
}

}
