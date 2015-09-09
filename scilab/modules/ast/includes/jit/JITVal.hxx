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

#ifndef __JIT_VAL_HXX__
#define __JIT_VAL_HXX__

#include <cassert>

#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/IRBuilder.h"

#include "internal.hxx"
#include "JITVisitor.hxx"
#include "TIType.hxx"

namespace jit
{
    class JITVal
    {

	llvm::Value * const value;
	const bool alloc;
	const bool issigned;

	JITVal(llvm::Value * const _value, const bool _alloc, const bool _issigned) : value(_value), alloc(_alloc), issigned(_issigned) { }

    public:

	inline llvm::Type * getType() const
	    {
		return value->getType();
	    }

	inline llvm::Value * getValue() const
	    {
		return value;
	    }

	inline bool isSigned() const
	    {
		return issigned;
	    }

	inline void store(JITVisitor & jit, const JITVal & var)
	    {
		store(jit, var.load(jit));
	    }

	inline void store(JITVisitor & jit, llvm::Value * _value)
	    {
		if (alloc)
		{
		    jit.getBuilder().CreateStore(_value, value);
		}
		else
		{
		    assert(false && "Cannot store a value");
		}
	    }
	
	inline llvm::Value * load(JITVisitor & jit) const
	    {
		if (alloc)
		{
		    return jit.getBuilder().CreateLoad(value);
		}
		else
		{
		    return value;
		}
	    }

	template<typename T>
	inline static JITVal getAlloc(JITVisitor & jit, const std::string & name = "")
	    {
		llvm::AllocaInst * alloca = jit.getBuilder().CreateAlloca(jit.getTy<T>(), nullptr, name);
		alloca->setAlignment(jit.getTySizeInBytes<T>());
		
		return JITVal(alloca, true, std::is_integral<T>::value && std::is_signed<T>::value);
	    }

	template<typename T>
	inline static JITVal getAlloc(JITVisitor & jit, const T val, const std::string & name = "")
	    {
		llvm::AllocaInst * alloca = jit.getBuilder().CreateAlloca(jit.getTy<T>(), nullptr, name);
		alloca->setAlignment(sizeof(T));
		builder.CreateAlignedStore(jit.getConstant<T>(val), alloca, sizeof(T));
		
		return JITVal(alloca, true, std::is_integral<T>::value && std::is_signed<T>::value);
	    }

	template<typename T>
	inline static JITVal get(JITVisitor & jit, const T val, const std::string & name = "")
	    {
		return JITVal(jit.getConstant<T>(val, name), false, std::is_integral<T>::value && std::is_signed<T>::value);
	    }

    };

} // namespace jit

#endif // __JIT_VAL_HXX__
