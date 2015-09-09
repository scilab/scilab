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

    void JITRDivision::MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
    {
	llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Type * int64_ty = jit.getTy<int64_t>();
	llvm::Value * L_rows = L->loadRows(jit);
	llvm::Value * L_cols = L->loadCols(jit);
	llvm::Value * R_rows = R->loadRows(jit);
	llvm::Value * R_cols = R->loadCols(jit);
        llvm::Type * types[] = { jit.getTy(Ltype, 1), int64_ty, int64_ty, jit.getTy(Rtype, 1), int64_ty, int64_ty, jit.getTy(Otype, 2) };
	llvm::Value * args[] = { L->loadData(jit), L_rows, L_cols, R->loadData(jit), R_rows, R_cols, O->getData(jit)};
        llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling(scilabName, Ltype, Rtype), llvm::FunctionType::get(jit.getTy<void>(), llvm::ArrayRef<llvm::Type *>(types), false)));
	llvm::AttrBuilder attrBuilder;
	attrBuilder.addAttribute(llvm::Attribute::NoAlias).addAttribute(llvm::Attribute::NoCapture);
	llvm::AttributeSet attrSet = llvm::AttributeSet::get(context, 1, attrBuilder).addAttributes(context, 4, llvm::AttributeSet::get(context, 4, attrBuilder)).addAttributes(context, 7, llvm::AttributeSet::get(context, 7, attrBuilder));
	toCall->setAttributes(attrSet);
        builder.CreateCall(toCall, llvm::ArrayRef<llvm::Value *>(args));
	O->storeRows(jit, L_rows);
	O->storeCols(jit, R_rows);
    }

    JITScilabPtr JITRDivision::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit)
    {
	llvm::Value * ret = nullptr;
        llvm::IRBuilder<> & builder = jit.getBuilder();

        // Integral types
        if (!Ltype.isfloating() || !Rtype.isfloating())
        {
	    llvm::Value * l = L->loadData(jit);
	    llvm::Value * r = R->loadData(jit);
	    Cast::promoteScilabValue(Ltype.issigned(), l, Rtype.issigned(), r, builder);
	    //TODO: it is an error 1/int8(2) => 0.5 so it is a double !!
	    if (!Ltype.issigned() || !Rtype.issigned())
	    {
		ret = builder.CreateUDiv(l, r);
	    }
	    else
	    {
		ret = builder.CreateSDiv(l, r);
	    }
	    
            if (Otype.type == analysis::TIType::DOUBLE)
            {
                // bool / bool = double !!
                ret = builder.CreateSIToFP(ret, jit.getTy<double>());
            }
        }
        else if (Ltype.isfloating() && Rtype.isfloating())
        {
	    if (Ltype.type == analysis::TIType::DOUBLE)
	    {
		llvm::Value * l = L->loadData(jit);
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // double / double
		    llvm::Value * r = R->loadData(jit);
		    llvm::Type * int64_ty = jit.getTy<int64_t>();
		    if (l->getType() == int64_ty && r->getType() == int64_ty)
		    {
			ret = builder.CreateUDiv(l, r);
		    }
		    else
		    {
			Cast::castInt64(l, jit);
			Cast::castInt64(r, jit);
			ret = builder.CreateFDiv(l, r);
		    }   
		}
		else
		{
		    // double / complex
		    /*
		      We use the following algorithm (more accurate than the basic one)
		        - for l and r=a+ib
			- if (a == 0) then
			   l / r = -i * l / b
			  else if (b == 0) then
			   l / r = l / a
			  else
			   n1 = |a| + |b|
			   l1 = l / n1
			   a1 = a / n1
			   b1 = b / n1
			   s = a1 ^ 2 + b1 ^ 2 = (a ^ 2 + b ^ 2) / n1 ^ 2
			   l / r = (l1 * a1) / s + i * (-l1 * b1) / s

		    */
		    // if l is an int64, then it needs to be casted into a double
		    Cast::castInt64(l, jit);
		    llvm::Value * fabs = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::fabs, jit.getTy<double>());
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb3 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb4 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    std::pair<llvm::Value *, llvm::Value *> r_a_b = R->loadReIm(jit);
		    llvm::Value * a = r_a_b.first;
		    llvm::Value * b = r_a_b.second;
		    llvm::Value * cmp = builder.CreateFCmpOEQ(a, 0);
		    builder.CreateCondBr(cmp, bb1, bb2);

		    builder.SetInsertPoint(bb1);
		    llvm::Value * res_re_1 = jit.getConstant<double>(0);
		    llvm::Value * res_im_1 = builder.CreateFDiv(l, b);
		    res_im_1 = builder.CreateFNeg(res_im_1);
		    builder.CreateBr(after);

		    builder.SetInsertPoint(bb2);
		    cmp = builder.CreateFCmpOEQ(b, 0);
		    builder.CreateCondBr(cmp, bb3, bb4);

		    builder.SetInsertPoint(bb3);
		    llvm::Value * res_re_2 = builder.CreateFDiv(l, a);
		    llvm::Value * res_im_2 = jit.getConstant<double>(0);
		    builder.CreateBr(after);

		    builder.SetInsertPoint(bb4);
		    llvm::Value * abs_a = builder.CreateCall(fabs, llvm::ArrayRef<llvm::Value *>(a));
		    llvm::Value * abs_b = builder.CreateCall(fabs, llvm::ArrayRef<llvm::Value *>(b));
		    llvm::Value * n1 = builder.CreateFAdd(abs_a, abs_b);
		    llvm::Value * l1 = builder.CreateFDiv(l, n1);
		    llvm::Value * a1 = builder.CreateFDiv(a, n1);
		    llvm::Value * b1 = builder.CreateFDiv(b, n1);
		    llvm::Value * sq_a1 = builder.CreateFMul(a1, a1);
		    llvm::Value * sq_b1 = builder.CreateFMul(b1, b1);
		    llvm::Value * s = builder.CreateFAdd(sq_a1, sq_b1);
		    llvm::Value * res_re_3 = builder.CreateFMul(l1, a1);
		    res_re_3 = builder.CreateFDiv(res_re_3, s);
		    llvm::Value * res_im_3 = builder.CreateFMul(l1, b1);
		    res_im_3 = builder.CreateFDiv(res_im_3, s);
		    res_im_3 = builder.CreateFNeg(res_im_3);
		    builder.CreateBr(after);

		    builder.SetInsertPoint(after);
		    llvm::PHINode * res_re = builder.CreatePHI(jit.getTy<double>(), 3);
		    res_re->addIncoming(res_re_1, bb1);
		    res_re->addIncoming(res_re_2, bb3);
		    res_re->addIncoming(res_re_3, bb4);
		    llvm::PHINode * res_im = builder.CreatePHI(jit.getTy<double>(), 3);
		    res_im->addIncoming(res_im_1, bb1);
		    res_im->addIncoming(res_im_2, bb3);
		    res_im->addIncoming(res_im_3, bb4);
		    
		    return jit.getScalar(res_re, res_im, Otype.type);
		}
	    }
	    else
	    {
		if (Rtype.type == analysis::TIType::DOUBLE)
		{
		    // complex / double
		    std::pair<llvm::Value *, llvm::Value *> l_reim = L->loadReIm(jit);
		    llvm::Value * r = R->loadData(jit);
		    Cast::castInt64(r, jit);
		    llvm::Value * res_re = builder.CreateFDiv(l_reim.first, r);
		    llvm::Value * res_im = builder.CreateFDiv(l_reim.second, r);
		    return jit.getScalar(res_re, res_im, Otype.type);
		}
		else
		{
		    // complex / complex
		    /*
		      We use the following algorithm (more accurate than the basic one) due to Smith
		        - for x=a+ib and y=c+id
			- if (c == 0) then
			   x / y = b / d - i * (a / d)
			  else if (d == 0) then
			   x / y = a / c + i * (b / c)
			  else if (|c| >= |d|) then
			   ratio = d / c
			   x / y = (a + b * ratio) / (c + d * ratio) + i * (b - a * ratio) / (c + d * ratio)
			  else if (|d| >= |c|) then
			   ratio = c / d
			   x / y = (a * ratio + b) / (c * ratio + d) + i * (b * ratio - a) / (c * ratio + d)

		    */
		    llvm::Value * fabs = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::fabs, jit.getTy<double>());
		    llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb3 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb4 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb5 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * bb6 = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    llvm::BasicBlock * after = llvm::BasicBlock::Create(jit.getContext(), "", &jit.getFunction());
		    std::pair<llvm::Value *, llvm::Value *> l_a_b = L->loadReIm(jit);
		    std::pair<llvm::Value *, llvm::Value *> r_c_d = R->loadReIm(jit);
		    llvm::Value * zero = jit.getConstant<double>(0);
		    llvm::Value * a = l_a_b.first;
		    llvm::Value * b = l_a_b.second;
		    llvm::Value * c = r_c_d.first;
		    llvm::Value * d = r_c_d.second;

		    llvm::Value * cmp = builder.CreateFCmpOEQ(c, zero);
		    builder.CreateCondBr(cmp, bb1, bb2);
		    
		    builder.SetInsertPoint(bb1);
		    llvm::Value * res_re_1 = builder.CreateFDiv(b, d);
		    llvm::Value * res_im_1 = builder.CreateFDiv(a, d);
		    res_im_1 = builder.CreateFNeg(res_im_1);
		    builder.CreateBr(after);
		    
		    builder.SetInsertPoint(bb2);
		    cmp = builder.CreateFCmpOEQ(d, zero);
		    builder.CreateCondBr(cmp, bb3, bb4);
		    
		    builder.SetInsertPoint(bb3);
		    llvm::Value * res_re_2 = builder.CreateFDiv(a, c);
		    llvm::Value * res_im_2 = builder.CreateFDiv(b, c);
		    builder.CreateBr(after);
		    
		    builder.SetInsertPoint(bb4);
		    llvm::Value * abs_c = builder.CreateCall(fabs, llvm::ArrayRef<llvm::Value *>(c));
		    llvm::Value * abs_d = builder.CreateCall(fabs, llvm::ArrayRef<llvm::Value *>(d));
		    cmp = builder.CreateFCmpOGE(abs_c, abs_d);
		    builder.CreateCondBr(cmp, bb5, bb6);
		    
		    builder.SetInsertPoint(bb5);
		    llvm::Value * ratio = builder.CreateFDiv(d, c); // d / c
		    llvm::Value * den = builder.CreateFMul(d, ratio); // d * ratio
		    den = builder.CreateFAdd(c, den); // den <- c + d * ratio
		    llvm::Value * res_re_3 = builder.CreateFMul(b, ratio); // b * ratio
		    res_re_3 = builder.CreateFAdd(a, res_re_3); // a + b * ratio
		    res_re_3 = builder.CreateFDiv(res_re_3, den); // (a + b * ratio) / den
		    llvm::Value * res_im_3 = builder.CreateFMul(a, ratio); // a * ratio
		    res_im_3 = builder.CreateFSub(b, res_im_3); // b - a * ratio
		    res_im_3 = builder.CreateFDiv(res_im_3, den); // (b - a * ratio) / den
		    builder.CreateBr(after);

		    builder.SetInsertPoint(bb6);
		    ratio = builder.CreateFDiv(c, d); // c / d
		    den = builder.CreateFMul(c, ratio); // c * ratio
		    den = builder.CreateFAdd(den, d); // den <- c * ratio + d
		    llvm::Value * res_re_4 = builder.CreateFMul(a, ratio); // a * ratio
		    res_re_4 = builder.CreateFAdd(res_re_4, b); // a * ratio + b
		    res_re_4 = builder.CreateFDiv(res_re_4, den); // (a * ratio + b) / den
		    llvm::Value * res_im_4 = builder.CreateFMul(b, ratio); // b * ratio
		    res_im_4 = builder.CreateFSub(res_im_4, a); // b * ratio - a
		    res_im_4 = builder.CreateFDiv(res_im_4, den); // (b * ratio - a) / den
		    builder.CreateBr(after);
		    
		    builder.SetInsertPoint(after);
		    llvm::PHINode * res_re = builder.CreatePHI(jit.getTy<double>(), 4);
		    res_re->addIncoming(res_re_1, bb1);
		    res_re->addIncoming(res_re_2, bb3);
		    res_re->addIncoming(res_re_3, bb5);
		    res_re->addIncoming(res_re_4, bb6);
		    llvm::PHINode * res_im = builder.CreatePHI(jit.getTy<double>(), 4);
		    res_im->addIncoming(res_im_1, bb1);
		    res_im->addIncoming(res_im_2, bb3);
		    res_im->addIncoming(res_im_3, bb5);
		    res_im->addIncoming(res_im_4, bb6);

		    return jit.getScalar(res_re, res_im, Otype.type);
		}
	    }
	}

        return jit.getScalar(ret, Otype.type);
    }

}
