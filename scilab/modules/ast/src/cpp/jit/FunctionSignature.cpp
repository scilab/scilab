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
#include "calls/FunctionSignature.hxx"

namespace jit
{

    llvm::FunctionType * FunctionSignature::makeTypesAndArgs(JITVisitor & jit, const std::vector<const analysis::TIType *> & outTypes, bool outInIn, const std::vector<const analysis::TIType *> & argsTypes, const std::vector<JITScilabPtr> & argsIn, const std::vector<JITScilabPtr> & argsOut, std::vector<llvm::Value *> & outArgs)
    {
        // If we have more than 2 output args (or none) or single complex as output, outputs are put in input
        outInIn = outInIn || (outTypes.size() != 1 || outTypes.front()->type == analysis::TIType::COMPLEX);
        llvm::Type * out_ty;
        std::vector<llvm::Type *> argsLLVMTy;

        if (outInIn)
        {
            out_ty = jit.getTy<void>();
        }
        else
        {
            out_ty = jit.getTy(*outTypes.front());
        }

        auto i = argsIn.begin();
        for (const auto type : argsTypes)
        {
            const JITScilabPtr & ptr = *i;
            if (type->isscalar())
            {
                if (type->type == analysis::TIType::COMPLEX)
                {
                    argsLLVMTy.emplace_back(jit.getTy<double>());
                    argsLLVMTy.emplace_back(jit.getTy<double>());
                    outArgs.emplace_back(ptr->loadReal(jit));
                    outArgs.emplace_back(ptr->loadImag(jit));
                }
                else
                {
                    argsLLVMTy.emplace_back(jit.getTy(*type));
                    outArgs.emplace_back(ptr->loadData(jit));
                }
            }
            else
            {
                if (type->type == analysis::TIType::COMPLEX)
                {
                    argsLLVMTy.emplace_back(jit.getTy<double *>());
                    argsLLVMTy.emplace_back(jit.getTy<double *>());
                    outArgs.emplace_back(ptr->loadReal(jit));
                    outArgs.emplace_back(ptr->loadImag(jit));
                }
                else
                {
                    argsLLVMTy.emplace_back(jit.getTy(*type, 1));
                    outArgs.emplace_back(ptr->loadData(jit));
                }
                argsLLVMTy.emplace_back(jit.getTy<int64_t>()); 
                outArgs.emplace_back(ptr->loadRows(jit));
                argsLLVMTy.emplace_back(jit.getTy<int64_t>());
                outArgs.emplace_back(ptr->loadCols(jit));
            }
	    ++i;
        }

        if (outInIn && !outTypes.empty())
        {
	    if (outTypes.size() == 1 && outTypes.front()->type == analysis::TIType::COMPLEX)
	    {
		// we have a complex rvalue
		JITScilabPtr & ptr = jit.getCpxRValue();
		argsLLVMTy.emplace_back(jit.getTy<double *>());
		argsLLVMTy.emplace_back(jit.getTy<double *>());
		outArgs.emplace_back(ptr->getReal(jit));
		outArgs.emplace_back(ptr->getImag(jit));
	    }
	    else
	    {	
		i = argsOut.begin();
		for (const auto type : outTypes)
		{
		    const JITScilabPtr & ptr = *i;
		    if (type->isscalar())
		    {
			if (type->type == analysis::TIType::COMPLEX)
			{
			    argsLLVMTy.emplace_back(jit.getTy<double *>());
			    argsLLVMTy.emplace_back(jit.getTy<double *>());
			    outArgs.emplace_back(ptr->getReal(jit));
			    outArgs.emplace_back(ptr->getImag(jit));
			}
			else
			{
			    argsLLVMTy.emplace_back(jit.getTy(*type));
			    outArgs.emplace_back(ptr->getData(jit));
			}
		    }
		    else
		    {
			if (type->type == analysis::TIType::COMPLEX)
			{
			    argsLLVMTy.emplace_back(jit.getTy<double **>());
			    argsLLVMTy.emplace_back(jit.getTy<double **>());
			    outArgs.emplace_back(ptr->getReal(jit));
			    outArgs.emplace_back(ptr->getImag(jit));
			}
			else
			{
			    argsLLVMTy.emplace_back(jit.getTy(*type, 2));
			    outArgs.emplace_back(ptr->getData(jit));
			}
			argsLLVMTy.emplace_back(jit.getTy<int64_t *>());
			outArgs.emplace_back(ptr->getRows(jit));
			argsLLVMTy.emplace_back(jit.getTy<int64_t *>());
			outArgs.emplace_back(ptr->getCols(jit));
		    }
		    ++i;
		}
            }
        }

        return llvm::FunctionType::get(out_ty, llvm::ArrayRef<llvm::Type *>(argsLLVMTy), false);
    }


} // namespace jit
