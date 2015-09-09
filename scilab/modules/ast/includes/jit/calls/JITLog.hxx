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

#ifndef __JIT_LOG_HXX__
#define __JIT_LOG_HXX__

#include "JITOptimizedCall1.hxx"

namespace jit
{
    
    class JITLog : public JITOptimizedCall1
    {
	
    public:

	enum Type { BASE_E, BASE_2, BASE_10 };
	
        JITLog(const Type type) : JITOptimizedCall1(getName(type), {{ }}, getID(type)) { }
	
	virtual ~JITLog() { }

	virtual void S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit);

	void S_d(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit);

    private:

	inline static std::string getName(const Type type)
	    {
		switch (type)
		{
		case BASE_E:
		    return "log";
		case BASE_2:
		    return "log2";
		case BASE_10:
		    return "log10";
		}
	    }

	inline static llvm::Intrinsic::ID getID(const Type type)
	    {
		switch (type)
		{
		case BASE_E:
		    return llvm::Intrinsic::log;
		case BASE_2:
		    return llvm::Intrinsic::log2;
		case BASE_10:
		    return llvm::Intrinsic::log10;
		}
	    }
    };

} // namespace jit

#endif // __JIT_LOG_HXX__
