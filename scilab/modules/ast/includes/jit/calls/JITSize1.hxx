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

#ifndef __JIT_SIZE1_HXX__
#define __JIT_SIZE1_HXX__

#include "JITCall1.hxx"

namespace jit
{

    class JITSize1 : public FunCallInitializer
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

    };
    
} // namespace jit

#endif // __JIT_SIZE1_HXX__
