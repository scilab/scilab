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
#include "calls/JITBinOpCall.hxx"
#include "Cast.hxx"

namespace jit
{

bool JITBinOpCall::invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit)
{
    const ast::OpExp & oe = static_cast<const ast::OpExp &>(e);
    const analysis::TIType & Ltype = oe.getLeft().getDecorator().getResult().getType();
    const analysis::TIType & Rtype = oe.getRight().getDecorator().getResult().getType();

    oe.getLeft().accept(jit);
    JITScilabPtr L = jit.getResult();
    oe.getRight().accept(jit);
    JITScilabPtr & R = jit.getResult();
    const uint8_t code = (Ltype.isscalar() ? 1 : 0) * 2 + (Rtype.isscalar() ? 1 : 0);

    switch (code)
    {
        case 0:
        {
            if (out.empty())
            {
                out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
            }
            MM(L, Ltype, R, Rtype, out.front(), typesOut.front(), jit);
            break;
        }
        case 1:
        {
            if (out.empty())
            {
                out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
            }
            MS(L, Ltype, R, Rtype, out.front(), typesOut.front(), jit);
            break;
        }
        case 2:
        {
            if (out.empty())
            {
                out.emplace_back(jit.getTemp(e.getDecorator().getResult().getTempId()));
            }
            SM(L, Ltype, R, Rtype, out.front(), typesOut.front(), jit);
            break;
        }
        default:
        {
            if (out.empty())
            {
                out.emplace_back(SS(L, Ltype, R, Rtype, typesOut.front(), jit));
            }
            else
            {
                out.front()->storeData(jit, SS(L, Ltype, R, Rtype, typesOut.front(), jit)->loadData(jit));
            }
            break;
        }
    }

    return true;
}

void JITBinOpCall::SM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
{
    jit.getModule().dump();
    llvm::LLVMContext & context = jit.getContext();
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Type * int64_ty = jit.getTy<int64_t>();
    llvm::Value * rows = R->loadRows(jit);
    llvm::Value * cols = R->loadCols(jit);
    llvm::Value * rc = builder.CreateMul(rows, cols);
    llvm::Type * types[] = { int64_ty, jit.getTy(Ltype), jit.getTy(Rtype, 1), jit.getTy(Otype, 2) };
    llvm::Value * args[] = { rc, L->loadData(jit), R->loadData(jit), O->getData(jit)};
    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(types), false)));

    llvm::AttrBuilder attrBuilder;
    attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
    llvm::AttributeSet attrSet = llvm::AttributeSet::get(context, 3, attrBuilder).addAttributes(context, 4, llvm::AttributeSet::get(context, 4, attrBuilder));
    toCall->setAttributes(attrSet);
    builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
    O->storeRows(jit, rows);
    O->storeCols(jit, cols);
}

void JITBinOpCall::MS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
{
    llvm::LLVMContext & context = jit.getContext();
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Type * int64_ty = jit.getTy<int64_t>();
    llvm::Value * rows = L->loadRows(jit);
    llvm::Value * cols = L->loadCols(jit);
    llvm::Value * rc = builder.CreateMul(rows, cols);
    llvm::Type * types[] = { int64_ty, jit.getTy(Ltype, 1), jit.getTy(Rtype), jit.getTy(Otype, 2) };
    llvm::Value * args[] = { rc, L->loadData(jit), R->loadData(jit), O->getData(jit)};
    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(types), false)));
    llvm::AttrBuilder attrBuilder;
    attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
    llvm::AttributeSet attrSet = llvm::AttributeSet::get(context, 2, attrBuilder).addAttributes(context, 4, llvm::AttributeSet::get(context, 4, attrBuilder));
    toCall->setAttributes(attrSet);
    builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
    O->storeRows(jit, rows);
    O->storeCols(jit, cols);
}

void JITBinOpCall::MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
{
    llvm::LLVMContext & context = jit.getContext();
    llvm::IRBuilder<> & builder = jit.getBuilder();
    llvm::Type * int64_ty = jit.getTy<int64_t>();
    llvm::Value * rows = L->loadRows(jit);
    llvm::Value * cols = L->loadCols(jit);
    llvm::Value * rc = builder.CreateMul(rows, cols);
    llvm::Type * types[] = { int64_ty, jit.getTy(Ltype, 1), jit.getTy(Rtype, 1), jit.getTy(Otype, 2) };
    llvm::Value * args[] = { rc, L->loadData(jit), R->loadData(jit), O->getData(jit)};
    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(types), false)));
    llvm::AttrBuilder attrBuilder;
    attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
    llvm::AttributeSet attrSet = llvm::AttributeSet::get(context, 2, attrBuilder).addAttributes(context, 3, llvm::AttributeSet::get(context, 3, attrBuilder)).addAttributes(context, 4, llvm::AttributeSet::get(context, 4, attrBuilder));
    toCall->setAttributes(attrSet);
    builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
    O->storeRows(jit, rows);
    O->storeCols(jit, cols);
}

}
