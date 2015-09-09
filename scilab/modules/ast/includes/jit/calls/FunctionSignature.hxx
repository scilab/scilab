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

    class FunctionSignature
    {
	
    public:

	static llvm::FunctionType * makeTypesAndArgs(JITVisitor & jit, const std::vector<const analysis::TIType *> & outTypes, bool outInIn, const std::vector<const analysis::TIType *> & argsTypes, const std::vector<JITScilabPtr> & argsIn, const std::vector<JITScilabPtr> & argsOut, std::vector<llvm::Value *> & outArgs);

    };

} // namespace jit

#endif // __JIT_FUNCTION_SIGNATURE_HXX__
