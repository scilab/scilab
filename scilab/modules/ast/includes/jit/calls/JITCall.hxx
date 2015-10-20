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

#ifndef __JIT_CALL_HXX__
#define __JIT_CALL_HXX__

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "llvm/IR/Intrinsics.h"

namespace jit
{
    
    class JITVisitor;
    class TIType;
    class Symbol;
    class JITScilabVal;
    typedef std::shared_ptr<JITScilabVal> JITScilabPtr;

    class JITCall
    {
    
    protected:

	typedef std::unordered_map<std::wstring, std::shared_ptr<JITCall>> FunMap;
	static FunMap funs;

    public:
	
        JITCall() { }
	
	virtual ~JITCall() { }

	virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit) = 0;

	virtual void init(const std::vector<analysis::TIType> & args, JITVisitor & jit) { }
	
	static bool call(const ast::CallExp & e, std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);

    private:

	static FunMap init();
	static bool callCompiledMacro(const uint64_t id, const ast::CallExp & e, std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);

    };

} // namespace jit

#endif // __JIT_CALL_HXX__
