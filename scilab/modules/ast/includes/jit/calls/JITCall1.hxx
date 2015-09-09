/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __JIT_CALL1_HXX__
#define __JIT_CALL1_HXX__

#include <string>
#include <unordered_map>

#include "llvm/IR/Intrinsics.h"

#include "FunCallInitializer.hxx"

namespace jit
{
    class JITVisitor;
    class TIType;

    class JITCall1 : public FunCallInitializer
    {
    
    protected:

	const std::string scilabName;
        const std::string libname;
        const std::string cpx_libname;
	llvm::Value * cpx_arr;
	const llvm::Intrinsic::ID id;

	typedef std::unordered_map<std::wstring, JITCall1 *> FunMap;
	static FunMap funs;

    public:
	
        JITCall1(const::string & _scilabName = "", const std::string & _name = "", const std::string & cpx_name = "", const llvm::Intrinsic::ID _id = llvm::Intrinsic::not_intrinsic) : scilabName(_scilabName), libname(_name), cpx_libname(cpx_name), id(_id), cpx_arr(nullptr) { }

        virtual JITScilabPtr S(const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & jit) const;

        virtual JITScilabPtr S_d(const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & jit) const;

        virtual void M(const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITScilabPtr & O, JITVisitor & jit) const;
	
	virtual JITScilabPtr M(const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & jit) const;

        virtual JITScilabPtr S_c(const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & jit) const;

	bool hasInit() const { return !cpx_libname.empty(); }
	void init(const std::vector<analysis::TIType> & args, JITVisitor & jit);

	inline static JITScilabPtr callS(const std::wstring & name, const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & jit)
	    {
		FunMap::const_iterator it = funs.find(name);
		if (it != funs.end())
		{
		     return it->second->S(LType, L, OType, jit);
		}
		
		return JITScilabPtr(nullptr);
	    }

	/*inline static JITScilabPtr callS(const ast::Symbol & sym, const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & visitor)
	    {
		
	    }*/

	inline static bool callM(const std::wstring & name, const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITScilabPtr & O, JITVisitor & jit)
	    {
		FunMap::const_iterator it = funs.find(name);
		if (it != funs.end())
		{
		    it->second->M(LType, L, OType, O, jit);
		    return true;
		}
		
		return false;
	    }

	inline static JITScilabPtr callM(const std::wstring & name, const analysis::TIType & LType, JITScilabPtr & L, const analysis::TIType & OType, JITVisitor & jit)
	    {
		FunMap::const_iterator it = funs.find(name);
		if (it != funs.end())
		{
		    return it->second->M(LType, L, OType, jit);
		}

		return JITScilabPtr(nullptr);		
	    }

	inline static void registerCall(const std::wstring & name, JITCall1 * call)
	    {
		funs.emplace(name, call);
	    }

	inline static void initialize(const std::wstring & name, const std::vector<analysis::TIType> & args, JITVisitor & jit)
	    {
		FunMap::iterator it = funs.find(name);
		if (it != funs.end() && it->second->hasInit())
		{
		    it->second->init(args, jit);
		}
	    }

    private:

	static FunMap __init__();
    };

} // namespace jit

#endif // __JIT_CALL1_HXX__
