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

#include <limits>

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITBinOpCall.hxx"
#include "Cast.hxx"
#include "calls/FunctionSignature.hxx"

namespace jit
{

    void JITPower::MS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit)
    {
	llvm::LLVMContext & context = jit.getContext();
        llvm::IRBuilder<> & builder = jit.getBuilder();
	llvm::Function & function = jit.getFunction();
	llvm::Type * int64_ty = jit.getTy<int64_t>();
	llvm::Type * dbl_ty = jit.getTy<double>();
	llvm::Value * r = R->loadData(jit);
	llvm::Value * Lrows = L->loadRows(jit);
	llvm::Value * expo = nullptr;

	if (Rtype.type == analysis::TIType::DOUBLE)
	{
	    if (llvm::isa<llvm::ConstantFP>(r))
	    {
		const double pow = static_cast<llvm::ConstantFP *>(r)->getValueAPF().convertToDouble();
		int64_t powi;
		if (analysis::tools::asInteger(pow, powi))
		{
		    expo = jit.getConstant(powi);
		}
	    }
	    else if (r->getType() == jit.getTy<int64_t>())
	    {
		expo = r;
	    }
	    else
	    {
		// we check that min::int64_t <= abs(r) <= max::int64_t && floor(r)==r;
		llvm::Value * abs = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::fabs, dbl_ty);
		llvm::Value * abs_r = builder.CreateCall(abs, r);
		llvm::Value * min_int64 = jit.getConstant<double>(std::numeric_limits<int64_t>::min());
		llvm::Value * max_int64 = jit.getConstant<double>(std::numeric_limits<int64_t>::max());
		llvm::Value * cmp1 = builder.CreateFCmpOLE(min_int64, abs_r);
		llvm::Value * cmp2 = builder.CreateFCmpOLE(abs_r, max_int64);
		llvm::Value * cmp3 = builder.CreateAnd(cmp1, cmp2);
		llvm::BasicBlock * bb1 = llvm::BasicBlock::Create(context, "", &function);
		llvm::BasicBlock * bb2 = llvm::BasicBlock::Create(context, "", &function);
		llvm::BasicBlock * bb3 = llvm::BasicBlock::Create(context, "", &function);
		builder.CreateCondBr(cmp3, bb1, bb3);
		builder.SetInsertPoint(bb1);
		llvm::Value * floor = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::floor, dbl_ty);
		llvm::Value * floor_r = builder.CreateCall(floor, r);
		llvm::Value * cmp4 = builder.CreateFCmpOEQ(r, floor_r);
		builder.CreateCondBr(cmp4, bb2, bb3);

		builder.SetInsertPoint(bb2);
		// here the exponent is an integer
		llvm::Value * r_i64 = builder.CreateFPToSI(r, int64_ty);
		const std::vector<llvm::Type *> types = FunctionSignature::getFunctionArgsTy(jit,
											     In<llvm::Type>(int64_ty),
											     In<analysis::TIType::Type>(Ltype.type, 1),
											     In<llvm::Type>(int64_ty),
											     In<analysis::TIType::Type>(Otype.type, 2));
		
		const std::vector<llvm::Value *> args = FunctionSignature::getFunctionArgs(jit,
											   In<llvm::Value>(Lrows),
											   In<JITScilabPtr, 0>(L),
											   In<llvm::Value>(r_i64),
											   In<JITScilabPtr, 1>(O));
		
		llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(scilabName, Ltype) + "Si64", llvm::FunctionType::get(jit.getTy<void>(), types, false)));
		builder.CreateCall(toCall, args);

		O->storeRows(jit, Lrows);
		O->storeCols(jit, Lrows);
		builder.CreateBr(bb3);

		builder.SetInsertPoint(bb3);
	    }
	}
	else if (Rtype.isintegral())
	{
	    expo = Cast::cast<int64_t>(r, Rtype.issigned(), jit);
	}
		
	if (expo)
	{
	    const std::vector<llvm::Type *> types = FunctionSignature::getFunctionArgsTy(jit,
											 In<llvm::Type>(int64_ty),
											 In<analysis::TIType::Type>(Ltype.type, 1),
											 In<llvm::Type>(int64_ty),
											 In<analysis::TIType::Type>(Otype.type, 2));
	    
	    const std::vector<llvm::Value *> args = FunctionSignature::getFunctionArgs(jit,
										       In<llvm::Value>(Lrows),
										       In<JITScilabPtr, 0>(L),
										       In<llvm::Value>(expo),
										       In<JITScilabPtr, 1>(O));
	    
	    llvm::Function * toCall = static_cast<llvm::Function *>(jit.getModule().getOrInsertFunction(analysis::TIType::get_unary_mangling(scilabName, Ltype) + "Si64", llvm::FunctionType::get(jit.getTy<void>(), types, false)));
	    builder.CreateCall(toCall, args);
	    
	    O->storeRows(jit, Lrows);
	    O->storeCols(jit, Lrows);
	}
    }

    JITScilabPtr JITPower::SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit)
    {
        llvm::Value * ret = nullptr;
        llvm::IRBuilder<> & builder = jit.getBuilder();

        // Integral types
        if (!Ltype.isfloating() || !Rtype.isfloating())
        {
	    // TODO: crappy for the moment
            llvm::Value * l = L->loadData(jit);
            llvm::Value * r = R->loadData(jit);
            Cast::promoteScilabValue(Ltype.issigned(), l, Rtype.issigned(), r, builder);
            ret = builder.CreateMul(l, r);
            if (Otype.type == analysis::TIType::DOUBLE)
            {
                // bool * bool = double !!
                ret = builder.CreateSIToFP(ret, jit.getTy<double>());
            }
	    return jit.getScalar(ret, Otype.type);
        }
        else if (Ltype.isfloating() && Rtype.isfloating())
        {
	    // TODO: handle case where double are int64_t !!
	    // it would be weird to have something like i^j (where i,j are index)
	    // so we need probably to improve analysis to have the context...
            if (Ltype.type == analysis::TIType::DOUBLE)
            {
                llvm::Value * l = L->loadData(jit);
                if (Rtype.type == analysis::TIType::DOUBLE)
                {
                    // double ^ double
                    llvm::Value * r = R->loadData(jit);
		    return jit.getScalar(pow_d_d(jit, l, r), Otype.type);
                }
	    }

	    std::pair<llvm::Value *, llvm::Value *> r_reim = R->loadReIm(jit);
	    const std::vector<const analysis::TIType *> outTypes({ &Otype });
	    const std::vector<const analysis::TIType *> argsTypes({ &Ltype, &Rtype });
	    const std::vector<JITScilabPtr> argsIn({ L, R });
	    const std::vector<JITScilabPtr> argsOut({ JITScilabPtr(nullptr) });
	    std::vector<llvm::Value *> outArgs;
	    llvm::FunctionType * funtype = FunctionSignature::makeTypesAndArgs(jit, outTypes, true /* out in input */, argsTypes, argsIn, argsOut, outArgs);
	    llvm::Value * toCall = jit.getModule().getOrInsertFunction(analysis::TIType::get_binary_mangling("pow", Ltype, Rtype), funtype);
	    builder.CreateCall(toCall, outArgs);
	    auto last = std::prev(outArgs.end());
	    auto penult = std::prev(last);
	    llvm::Value * im = builder.CreateAlignedLoad(*last, sizeof(double));
	    llvm::Value * re = builder.CreateAlignedLoad(*penult, sizeof(double));
	    return jit.getScalar(re, im, Otype.type);
	}
    }

    llvm::Value * JITPower::pow_d_d(JITVisitor & jit, llvm::Value * l, llvm::Value * r)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        if (llvm::isa<llvm::ConstantFP>(r))
        {
            const double pow = static_cast<llvm::ConstantFP *>(r)->getValueAPF().convertToDouble();
            if (std::fabs(pow) == 0.5)
            {
                llvm::Value * toCall = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::sqrt, jit.getTy<double>());
                llvm::Value * ret = builder.CreateCall(toCall, l);
                return pow > 0 ? ret : builder.CreateFDiv(jit.getConstant<double>(1), ret);
            }
            else
            {
                int8_t powi;
                if (analysis::tools::asInteger(pow, powi))
                {
                    const bool negative = powi < 0;
                    const uint8_t powui = (uint8_t)std::abs(powi);
                    llvm::Value * ret;
                    switch (powui)
                    {
                    case 0:
                        return jit.getConstant<double>(1);
                    case 1:
                        ret = l;
                        break;
                    case 2:
                        ret = builder.CreateFMul(l, l);
                        break;
                    case 3:
                        ret = builder.CreateFMul(l, l);
                        ret = builder.CreateFMul(ret, l);
                        break;
                    case 4:
                        ret = builder.CreateFMul(l, l);
                        ret = builder.CreateFMul(ret, ret);
                        break;
                    default:
                        ret = nullptr;
                    }

                    if (ret)
                    {
                        return negative ? builder.CreateFDiv(jit.getConstant<double>(1), ret) : ret;
                    }
                }
            }
        }

	llvm::Value * args[] = { l, r };
        llvm::Value * toCall = llvm::Intrinsic::getDeclaration(&jit.getModule(), llvm::Intrinsic::pow, jit.getTy<double>());
        return builder.CreateCall(toCall, args);
    }

    std::pair<llvm::Value *, llvm::Value *> JITPower::pow_c_d(JITVisitor & jit, llvm::Value * re, llvm::Value * im, llvm::Value * r)
    {
        llvm::IRBuilder<> & builder = jit.getBuilder();
        if (llvm::isa<llvm::ConstantFP>(r))
        {
            const double pow = static_cast<llvm::ConstantFP *>(r)->getValueAPF().convertToDouble();
            int8_t powi;
            if (analysis::tools::asInteger(pow, powi) && powi >= 0)
            {
                const uint8_t powui = (uint8_t)powi;
		std::pair<llvm::Value *, llvm::Value *> ret;
                switch (powui)
                {
                case 0:
                    return { jit.getConstant<double>(1), jit.getConstant<double>(0) };
                case 1:
                    return { re, im };
                case 2:
		{
		    llvm::Value * re_2 = builder.CreateFMul(re, re);
		    llvm::Value * im_2 = builder.CreateFMul(im, im);
		    llvm::Value * res_re = builder.CreateFSub(re_2, im_2);
		    llvm::Value * res_im = builder.CreateFMul(re, im);
		    res_im = builder.CreateFMul(res_im, jit.getConstant<double>(2));
		    return { res_re, res_im };
		}
                case 3:
		{
		    llvm::Value * re_2 = builder.CreateFMul(re, re);
		    llvm::Value * im_2 = builder.CreateFMul(im, im);
		    llvm::Value * res_2_re = builder.CreateFSub(re_2, im_2);
		    llvm::Value * res_2_im = builder.CreateFMul(re, im);
		    res_2_im = builder.CreateFMul(res_2_im, jit.getConstant<double>(2));

		    llvm::Value * re_3 = builder.CreateFMul(res_2_re, re);
		    llvm::Value * im_3 = builder.CreateFMul(res_2_im, im);
		    llvm::Value * res_3_re = builder.CreateFSub(re_3, im_3);
		    llvm::Value * x = builder.CreateFMul(res_2_re, im);
		    llvm::Value * y = builder.CreateFMul(res_2_im, re);
		    llvm::Value * res_3_im = builder.CreateFAdd(x, y);
		    
		    return { res_3_re, res_3_im };
		}
                case 4:
		{
		    llvm::Value * re_2 = builder.CreateFMul(re, re);
		    llvm::Value * im_2 = builder.CreateFMul(im, im);
		    llvm::Value * res_2_re = builder.CreateFSub(re_2, im_2);
		    llvm::Value * res_2_im = builder.CreateFMul(re, im);
		    res_2_im = builder.CreateFMul(res_2_im, jit.getConstant<double>(2));

		    llvm::Value * re_4 = builder.CreateFMul(res_2_re, res_2_re);
		    llvm::Value * im_4 = builder.CreateFMul(res_2_im, res_2_im);
		    llvm::Value * res_4_re = builder.CreateFSub(re_4, im_4);
		    llvm::Value * res_4_im = builder.CreateFMul(res_2_re, res_2_im);
		    res_4_im = builder.CreateFMul(res_4_im, jit.getConstant<double>(2));
		    
		    return { res_4_re, res_4_im };
		}
                default:
		    break;
                }
            }
        }

        return { nullptr, nullptr };
    }

}
