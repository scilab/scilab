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

#include "call/SizeCall.hxx"

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITZeros.hxx"

namespace jit
{

bool JITZeros::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::exps_t args = ce.getArgs();
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Module & module = jit.getModule();
    llvm::Value * r;
    llvm::Value * c;
    const unsigned int argSize = args.size();

    switch (argSize)
    {
        case 0:
        {
            // no arg => zeros() == 0
            if (out.empty())
            {
                out.emplace_back(new JITScalDouble(jit.getConstant<double>(0)));
            }
            else
            {
                out.front()->storeData(jit, jit.getConstant<double>(0));
            }
            return true;
        }
        case 1:
        {
            // one arg
            args.front()->accept(jit);
            JITScilabPtr & res = jit.getResult();
            r = res->loadRows(jit);
            c = res->loadCols(jit);
            break;
        }
        case 2:
        {
            // two args
            args.front()->accept(jit);
            r = Cast::cast<int64_t>(jit.getResult()->loadData(jit), false, jit);
            args.back()->accept(jit);
            c = Cast::cast<int64_t>(jit.getResult()->loadData(jit), false, jit);
            break;
        }
        default:
            return false;
    }

    llvm::Value * rc = builder.CreateMul(r, c);
    rc = Cast::cast<int64_t>(rc, false, jit);
    llvm::Value * size = builder.CreateMul(rc, jit.getConstant<int64_t>(sizeof(double)));
    llvm::Function * __new = static_cast<llvm::Function *>(module.getOrInsertFunction("new", llvm::FunctionType::get(jit.getTy<int8_t *>(), llvm::ArrayRef<llvm::Type *>(jit.getTy<uint64_t>()), false)));
    //__new->addFnAttr(llvm::Attribute::NoAlias);
    __new->addAttribute(0, llvm::Attribute::NoAlias);
    //llvm::Value * __aligned_malloc = module.getOrInsertFunction("aligned_alloc", llvm::FunctionType::get(jit.getTy<int8_t *>(), llvm::ArrayRef<llvm::Type *>((llvm::Type *[]){ jit.getTy<int64_t>(), jit.getTy<int64_t>()}), false));
    llvm::CallInst * alloc = builder.CreateCall(__new, size);
    alloc->addAttribute(0, llvm::Attribute::NoAlias);
    //llvm::Value * alloc = builder.CreateCall2(__aligned_malloc, jit.getConstant<int64_t>(sizeof(double)), size);
    llvm::Value * dbl_alloc = builder.CreateBitCast(alloc, jit.getTy<double *>());
    llvm::Type * types[] = {jit.getTy<int8_t *>(), jit.getTy<uint64_t>()};
    llvm::Value * __memset = llvm::Intrinsic::getDeclaration(&module, llvm::Intrinsic::memset, types);
    //llvm::Value * __memset = llvm::Intrinsic::getDeclaration(&module, llvm::Intrinsic::memset, llvm::ArrayRef<llvm::Type *>((llvm::Type *[]){ jit.getTy<int8_t *>(), jit.getTy<uint64_t>()}));

    JITScilabPtr res;
    if (out.empty())
    {
        //res = new JITArrayofDouble(jit, "");
    }
    else
    {
        out.front()->storeData(jit, dbl_alloc);
        out.front()->storeRows(jit, r);
        out.front()->storeCols(jit, c);
    }

    llvm::Value * memset_args[] = { alloc, jit.getConstant<int8_t>(0), size, jit.getConstant<int32_t>(sizeof(double)), jit.getBool(false) };
    builder.CreateCall(__memset, memset_args);

    return true;
}
}


