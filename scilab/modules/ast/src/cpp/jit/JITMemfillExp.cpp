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

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"

namespace jit
{

void JITVisitor::visit(const ast::MemfillExp & e)
{
    llvm::IRBuilder<> & builder = getBuilder();
    llvm::Module & module = getModule();
    const ast::exps_t args = e.getArgs();
    const ast::Exp & valExp = e.getValue();
    bool isZero = false;
    if (valExp.isDoubleExp())
    {
        const ast::DoubleExp & de = static_cast<const ast::DoubleExp &>(valExp);
        if (de.getConstant())
        {
            types::Double * pDbl = static_cast<types::Double *>(de.getConstant());
            if (pDbl->getSize() == 1)
            {
                isZero = pDbl->get(0) == 0.;
            }
        }
        else
        {
            isZero = de.getValue() == 0.;
        }
    }
    valExp.accept(*this);
    const analysis::TIType & valTy = valExp.getDecorator().getResult().getType();
    llvm::Value * value = Cast::cast<double>(getResult()->loadData(*this), valTy.isintegral() && valTy.issigned(), *this);
    llvm::Value * r;
    llvm::Value * c;
    const unsigned int argSize = args.size();

    switch (argSize)
    {
        case 0:
        {
            setResult(getScalar(value, analysis::TIType::DOUBLE, false, ""));
            return;
        }
        case 1:
        {
            // one arg
            args.front()->accept(*this);
            JITScilabPtr & res = getResult();
            r = res->loadRows(*this);
            c = res->loadCols(*this);
            break;
        }
        case 2:
        {
            // two args
            args.front()->accept(*this);
            r = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
            args.back()->accept(*this);
            c = Cast::cast<int64_t>(getResult()->loadData(*this), false, *this);
            break;
        }
        default:
            return;
    }

    llvm::Value * rc = builder.CreateMul(r, c);
    rc = Cast::cast<int64_t>(rc, false, *this);
    llvm::Value * size = builder.CreateMul(rc, getConstant<int64_t>(sizeof(double)));
    llvm::Function * __new = static_cast<llvm::Function *>(module.getOrInsertFunction("new", llvm::FunctionType::get(getTy<int8_t *>(), llvm::ArrayRef<llvm::Type *>(getTy<uint64_t>()), false)));
    __new->addAttribute(0, llvm::Attribute::NoAlias);
    llvm::CallInst * alloc = builder.CreateCall(__new, size);
    alloc->addAttribute(0, llvm::Attribute::NoAlias);
    llvm::Value * dbl_alloc = builder.CreateBitCast(alloc, getTy<double *>());


    if (isZero)
    {
        llvm::Type * memset_types[] = { getTy<int8_t *>(), getTy<uint64_t>() };
        llvm::Value * __memset = llvm::Intrinsic::getDeclaration(&module, llvm::Intrinsic::memset, memset_types);
        llvm::Value * memset_args[] = { alloc, getConstant<int8_t>(0), size, getConstant<int32_t>(sizeof(double)), getBool(false) };
        builder.CreateCall(__memset, memset_args);
    }
    else
    {
        // just make a loop to fill the array
        llvm::BasicBlock * cur_block = builder.GetInsertBlock();
        llvm::BasicBlock * loopBlock = llvm::BasicBlock::Create(context, "memfill_loop", function);
        llvm::BasicBlock * afterBlock = llvm::BasicBlock::Create(context, "memfill_after", function);
        llvm::Value * zero_i64 = getConstant<int64_t>(0);

        llvm::Value * cmp_i1 = builder.CreateICmpSLT(zero_i64, rc);
        builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

        builder.SetInsertPoint(loopBlock);
        llvm::PHINode * i = builder.CreatePHI(getTy<int64_t>(), 2);
        i->addIncoming(zero_i64, cur_block);

        llvm::Value * ptr = builder.CreateGEP(dbl_alloc, i);
        builder.CreateAlignedStore(value, ptr, sizeof(double));
        llvm::Value * inc = builder.CreateAdd(i, getConstant<int64_t>(1));
        i->addIncoming(inc, loopBlock);
        cmp_i1 = builder.CreateICmpSLT(inc, rc);
        builder.CreateCondBr(cmp_i1, loopBlock, afterBlock);

        builder.SetInsertPoint(afterBlock);
    }

    JITScilabPtr Lptr(nullptr);

    if (e.getParent()->isAssignExp())
    {
        const ast::AssignExp & ae = *static_cast<const ast::AssignExp *>(e.getParent());
        if (ae.getLeftExp().isSimpleVar()) // A = ...
        {
            const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar &>(ae.getLeftExp()).getSymbol();
            Lptr = variables.find(Lsym)->second;
        }
    }
    else
    {
        Lptr = getTemp(e.getDecorator().getResult().getTempId());
    }

    if (Lptr.get())
    {
        Lptr->storeData(*this, dbl_alloc);
        Lptr->storeRows(*this, r);
        Lptr->storeCols(*this, c);
    }

    setResult(Lptr);
}
}
