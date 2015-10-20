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
#include "calls/JITOptimizedCall1.hxx"
#include "calls/FunctionSignature.hxx"

namespace jit
{

bool JITOptimizedCall1::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
{
    const ast::CallExp & ce = static_cast<const ast::CallExp &>(e);
    const ast::Exp * first = ce.getArgs().front();
    const analysis::TIType & firstTy = first->getDecorator().getResult().getType();

    first->accept(jit);
    JITScilabPtr & arg1 = jit.getResult();

    if (firstTy.isscalar())
    {
        if (out.empty())
        {
            out.emplace_back(JITScilabPtr(nullptr));
        }
        S(firstTy, arg1, typesOut.front(), out.front(), jit);
    }
    else
    {
        if (out.empty())
        {
            out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
        }
        M(firstTy, arg1, typesOut.front(), out.front(), jit);
    }

    return true;
}

void JITOptimizedCall1::S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
{
    llvm::IRBuilder<> & builder = jit.getBuilder();
    if (argType.type == analysis::TIType::COMPLEX && OType.type == analysis::TIType::COMPLEX)
    {
        auto i = map.find(argType.type);
        if (i != map.end())
        {
            Sc_c(i->second, arg, out, jit);
            return;
        }
    }

    const std::vector<const analysis::TIType *> outTypes({&OType});
    const std::vector<const analysis::TIType *> argsTypes({&argType});
    const std::vector<JITScilabPtr> argsIn({arg});
    const std::vector<JITScilabPtr> argsOut({out});
    std::vector<llvm::Value *> outArgs;
    llvm::FunctionType * funtype = FunctionSignature::makeTypesAndArgs(jit, outTypes, false /* out in input */, argsTypes, argsIn, argsOut, outArgs);
    llvm::Value * toCall = nullptr;

    if (argType.type == analysis::TIType::DOUBLE && id != llvm::Intrinsic::not_intrinsic)
    {
        toCall = llvm::Intrinsic::getDeclaration(&jit.getModule(), id, jit.getTy<double>());
    }

    if (!toCall)
    {
        auto i = map.find(argType.type);
        if (i == map.end())
        {
            toCall = jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(jitName, argType), funtype);
        }
        else
        {
            toCall = jit.getModule().getOrInsertFunction(i->second, funtype);
        }
    }

    llvm::Value * ret = builder.CreateCall(toCall, outArgs);
    makeOutput(jit, ret, outArgs, OType, out);
}

void JITOptimizedCall1::M(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit)
{
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Type * int64_ty = jit.getTy<int64_t>();
    llvm::Value * rows = arg->loadRows(jit);
    llvm::Value * cols = arg->loadCols(jit);
    llvm::Value * rc = builder.CreateMul(rows, cols);
    const std::vector<llvm::Type *> types = FunctionSignature::getFunctionArgsTy(jit,
										 In<llvm::Type>(int64_ty),
										 In<analysis::TIType::Type>(argType.type, argType.isscalar() ? 0 : 1),
										 In<analysis::TIType::Type>(OType.type, OType.isscalar() ? 1 : 2));
    
    const std::vector<llvm::Value *> args = FunctionSignature::getFunctionArgs(jit,
									       In<llvm::Value>(rc),
									       In<JITScilabPtr, 0>(arg),
									       In<JITScilabPtr, 1>(out));
    
    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(jitName, argType), llvm::FunctionType::get(jit.getTy<void>(), types, false)));
    /*llvm::AttrBuilder attrBuilder;
      attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
      llvm::AttributeSet attrSet = llvm::AttributeSet::get(context, 2, attrBuilder).addAttributes(context, 3, llvm::AttributeSet::get(context, 3, attrBuilder)).addAttributes(context, 4, llvm::AttributeSet::get(context, 4, attrBuilder));
      toCall->setAttributes(attrSet);*/
    builder.CreateCall(toCall, args);
    out->storeRows(jit, rows);
    out->storeCols(jit, cols);
}
    
void JITOptimizedCall1::makeOutput(JITVisitor & jit, llvm::Value * ret, const std::vector<llvm::Value *> & outArgs, const analysis::TIType & OType, JITScilabPtr & out)
{
    llvm::IRBuilder<> & builder = jit.getBuilder();
    if (out.get())
    {
        if (OType.type == analysis::TIType::COMPLEX)
        {
            auto last = std::prev(outArgs.end());
            auto penult = std::prev(last);
            llvm::Value * im = builder.CreateAlignedLoad(*last, sizeof(double));
            llvm::Value * re = builder.CreateAlignedLoad(*penult, sizeof(double));
            out->storeReIm(jit, {re, im});
        }
        else
        {
            out->storeData(jit, ret);
        }
    }
    else
    {
        if (OType.type == analysis::TIType::COMPLEX)
        {
            auto last = std::prev(outArgs.end());
            auto penult = std::prev(last);
            llvm::Value * im = builder.CreateAlignedLoad(*last, sizeof(double));
            llvm::Value * re = builder.CreateAlignedLoad(*penult, sizeof(double));
            out.reset(new JITScalComplex(jit, re, im, false, ""));
        }
        else
        {
            jit.getScalar(ret, OType.type, false, "").swap(out);
        }
    }
}

void JITOptimizedCall1::Sc_c(const std::string & name, JITScilabPtr & arg, JITScilabPtr & out, JITVisitor & jit)
{
    // We call something like std::complex<double> std::sin(const std::complex<double> &)
    // TODO: Adapt the call to the platform (depends on ABI)

    // std::complex<double> std::sin(const std::complex<double> &) => {double, double} csin(double, double)
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Type * dbl_ty = jit.getTy<double>();
    llvm::Type * two_dbl_ty[] = {dbl_ty, dbl_ty};
    llvm::StructType * struct_ty = llvm::StructType::get(jit.getContext(), two_dbl_ty, false);
    llvm::FunctionType * funtype = llvm::FunctionType::get(struct_ty, two_dbl_ty, false);
    llvm::Value * toCall = jit.getModule().getOrInsertFunction(name, funtype);
    std::pair<llvm::Value *, llvm::Value *> arg_reim = arg->loadReIm(jit);
    llvm::Value * args[] = {arg_reim.first, arg_reim.second};
    llvm::CallInst * ret = builder.CreateCall(toCall, args);
    ret->setTailCall(true);
    llvm::Value * ret_re = builder.CreateExtractValue(ret, 0);
    llvm::Value * ret_im = builder.CreateExtractValue(ret, 1);

    if (out.get())
    {
        out->storeReIm(jit, {ret_re, ret_im});
    }
    else
    {
        out.reset(new JITScalComplex(jit, ret_re, ret_im, false, ""));
    }
}
    
} // namespace jit
