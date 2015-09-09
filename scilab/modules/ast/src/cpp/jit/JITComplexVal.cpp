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

    llvm::Type * JITScalComplex::getCpx128Ty(JITVisitor & jit)
    {
        /*
          llvm::Type * types[] = { dblTy, dblTy };
          return llvm::StructType::get(jit.getContext(), llvm::ArrayRef<llvm::Type *>(types));
        */
	llvm::Type * dblTy = jit.getTy<double>();
        return llvm::VectorType::get(dblTy, 2);
    }

    llvm::Value * JITScalComplex::loadReal(JITVisitor & jit)
    {
	return loadData(jit);
    }

    llvm::Value * JITScalComplex::loadImag(JITVisitor & jit)
    {
	if (allocated)
	{
	    return jit.getBuilder().CreateAlignedLoad(imag, sizeof(double));
	}
	else
	{
	    return imag;
	}
    }

    std::pair<llvm::Value *, llvm::Value *> JITScalComplex::loadReIm(JITVisitor & jit)
    {	
        llvm::Value * re, * im;
	if (allocated)
	{
	    re = jit.getBuilder().CreateAlignedLoad(data, sizeof(double));
	    im = jit.getBuilder().CreateAlignedLoad(imag, sizeof(double));
	}
	else
	{
	    re = data;
	    im = imag;
	}
	
        return std::pair<llvm::Value *, llvm::Value *>(re, im);
    }

    void JITScalComplex::storeImag(JITVisitor & jit, llvm::Value * _imag)
    {
	if (allocated)
	{
	    jit.getBuilder().CreateAlignedStore(_imag, imag, sizeof(double));
	}
	else
	{
	    assert(false && "storeImag mustn't be called");
	}
    }

    void JITScalComplex::storeReIm(JITVisitor & jit, std::pair<llvm::Value *, llvm::Value *> reim)
    {
	if (allocated)
	{
	    jit.getBuilder().CreateAlignedStore(reim.first, data, sizeof(double));
	    jit.getBuilder().CreateAlignedStore(reim.second, imag, sizeof(double));
	}
	else
	{
	    assert(false && "storeReal/Imag mustn't be called");
	}
    }
    
    void JITScalComplex::setImag(llvm::Value * _imag)
    {
	imag = _imag;
    }
    
    llvm::Value * JITScalComplex::getImag(JITVisitor & jit) const
    {
	return imag;
    }

    /* JITArrayofComplex */
    llvm::Value * JITArrayofComplex::loadReal(JITVisitor & jit)
    {
	return loadData(jit);
    }

    llvm::Value * JITArrayofComplex::loadImag(JITVisitor & jit)
    {
	return jit.getBuilder().CreateAlignedLoad(imag, sizeof(void *));
    }

    std::pair<llvm::Value *, llvm::Value *> JITArrayofComplex::loadReIm(JITVisitor & jit)
    {	
	llvm::Value * re = jit.getBuilder().CreateAlignedLoad(data, sizeof(double));
	llvm::Value * im = jit.getBuilder().CreateAlignedLoad(imag, sizeof(double));
	
        return std::pair<llvm::Value *, llvm::Value *>(re, im);
    }

    void JITArrayofComplex::storeImag(JITVisitor & jit, llvm::Value * _imag)
    {
	jit.getBuilder().CreateAlignedStore(_imag, imag, sizeof(void *));
    }

    void JITArrayofComplex::storeReIm(JITVisitor & jit, std::pair<llvm::Value *, llvm::Value *> reim)
    {
	jit.getBuilder().CreateAlignedStore(reim.first, data, sizeof(double));
	jit.getBuilder().CreateAlignedStore(reim.second, imag, sizeof(double));
    }
    
    void JITArrayofComplex::setImag(llvm::Value * _imag)
    {
	imag = _imag;
    }
    
    llvm::Value * JITArrayofComplex::getImag(JITVisitor & jit) const
    {
	return imag;
    }

} // namespace jit
