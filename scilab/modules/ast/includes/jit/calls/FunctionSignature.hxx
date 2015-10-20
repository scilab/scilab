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

#ifndef __JIT_FUNCTION_SIGNATURE_HXX__
#define __JIT_FUNCTION_SIGNATURE_HXX__

#include <vector>

namespace jit
{

    template<typename T, unsigned int N = 0>
    struct In;

    template<unsigned int N>
    struct In<llvm::Type, N>
    {
	llvm::Type * const ty;
	int level;

	In(llvm::Type * _ty, const int _level = -1) : ty(_ty), level(_level) { }
	inline void get(std::vector<llvm::Type *> & v, JITVisitor &) const
	    {
		llvm::Type * _ty = ty;
		if (level == -1)
		{
		    for (unsigned int i = 0; i < N; ++i)
		    {
			_ty = llvm::PointerType::getUnqual(_ty);
		    }
		}
		else
		{
		    for (unsigned int i = 0; i < level; ++i)
		    {
			_ty = llvm::PointerType::getUnqual(_ty);
		    }
		}
		v.emplace_back(_ty);
	    }
    };

    template<>
    struct In<llvm::Value, 0>
    {
	llvm::Value * const val;

	In(llvm::Value * _val) : val(_val) { }
	inline void get(std::vector<llvm::Value *> & v, JITVisitor &) const
	    {
		v.emplace_back(val);
	    }
    };

    template<unsigned int N>
    struct In<analysis::TIType::Type, N>
    {
	const analysis::TIType::Type ty;
	int level;

	In(analysis::TIType::Type _ty, const int _level = -1) : ty(_ty), level(_level) { }
	inline void get(std::vector<llvm::Type *> & v, JITVisitor & jit) const
	    {
		if (level == -1)
		{
		    if (ty == analysis::TIType::COMPLEX)
		    {
			llvm::Type * _ty = jit.getTy(analysis::TIType::DOUBLE, N);
			v.emplace_back(_ty);
			v.emplace_back(_ty);
		    }
		    else
		    {
			v.emplace_back(jit.getTy(ty, N));
		    }
		}
		else
		{
		    if (ty == analysis::TIType::COMPLEX)
		    {
			llvm::Type * _ty = jit.getTy(analysis::TIType::DOUBLE, level);
			v.emplace_back(_ty);
			v.emplace_back(_ty);
		    }
		    else
		    {
			v.emplace_back(jit.getTy(ty, level));
		    }
		}
	    }
    };

    template<unsigned int N>
    struct In<JITScilabPtr, N>
    {
	static_assert(N == 0 || N == 1, "Invalid value");
	
	JITScilabPtr & val;

	In(JITScilabPtr & _val) : val(_val) { }
	inline void get(std::vector<llvm::Value *> & v, JITVisitor & jit) const
	    {
		if (N == 0)
		{
		    if (val->isComplex())
		    {
			v.emplace_back(val->loadReal(jit));
			v.emplace_back(val->loadImag(jit));
		    }
		    else
		    {
			v.emplace_back(val->loadData(jit));
		    }
		}
		else
		{
		    if (val->isComplex())
		    {
			v.emplace_back(val->getReal(jit));
			v.emplace_back(val->getImag(jit));
		    }
		    else
		    {
			v.emplace_back(val->getData(jit));
		    }
		}
	    }
    };
    
    class FunctionSignature
    {
	
    public:

	static llvm::FunctionType * makeTypesAndArgs(JITVisitor & jit, const std::vector<const analysis::TIType *> & outTypes, bool outInIn, const std::vector<const analysis::TIType *> & argsTypes, const std::vector<JITScilabPtr> & argsIn, const std::vector<JITScilabPtr> & argsOut, std::vector<llvm::Value *> & outArgs);

	template<typename... Args>
	inline static std::vector<llvm::Type *> getFunctionArgsTy(JITVisitor & jit, Args... args)
	    {
		std::vector<llvm::Type *> v;
		__getFunctionArgsTy(jit, v, args...);

		return v;
	    }

	template<typename... Args>
	inline static std::vector<llvm::Value *> getFunctionArgs(JITVisitor & jit, Args... args)
	    {
		std::vector<llvm::Value *> v;
		__getFunctionArgs(jit, v, args...);

		return v;
	    }

	template<typename... Args>
	inline static void getFunctionArgsTy(JITVisitor & jit, std::vector<llvm::Type *> & v, Args... args)
	    {
		__getFunctionArgsTy(jit, v, args...);
	    }

	template<typename... Args>
	inline static void getFunctionArgs(JITVisitor & jit, std::vector<llvm::Value *> & v, Args... args)
	    {
		__getFunctionArgs(jit, v, args...);
	    }

    private:
	
	template<typename T, typename... Args>
	inline static void __getFunctionArgsTy(JITVisitor & jit, std::vector<llvm::Type *> & v, T && ty, Args... args)
	    {
		ty.get(v, jit);
		__getFunctionArgsTy(jit, v, args...);
	    }

	template<typename T>
	inline static void __getFunctionArgsTy(JITVisitor & jit, std::vector<llvm::Type *> & v, T && ty)
	    {
		ty.get(v, jit);
	    }

	template<typename T, typename... Args>
	inline static void __getFunctionArgs(JITVisitor & jit, std::vector<llvm::Value *> & v, T && arg, Args... args)
	    {
		arg.get(v, jit);
		__getFunctionArgs(jit, v, args...);
	    }

	template<typename T>
	inline static void __getFunctionArgs(JITVisitor & jit, std::vector<llvm::Value *> & v, T && arg)
	    {
		arg.get(v, jit);
	    }
    };

} // namespace jit

#endif // __JIT_FUNCTION_SIGNATURE_HXX__
