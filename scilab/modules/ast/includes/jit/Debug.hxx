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

#ifndef __JIT_DEBUG_HXX__
#define __JIT_DEBUG_HXX__

namespace jit
{

    class JITVisitor;
    
    class Debug
    {

    public:

	static void printI8(JITVisitor & jit, llvm::Value * x);
	static void printUI8(JITVisitor & jit, llvm::Value * x);
	static void printI16(JITVisitor & jit, llvm::Value * x);
	static void printUI16(JITVisitor & jit, llvm::Value * x);
	static void printI32(JITVisitor & jit, llvm::Value * x);
	static void printUI32(JITVisitor & jit, llvm::Value * x);
	static void printI64(JITVisitor & jit, llvm::Value * x);
	static void printUI64(JITVisitor & jit, llvm::Value * x);
	static void printD(JITVisitor & jit, llvm::Value * x);
	static void printB(JITVisitor & jit, llvm::Value * x);

    };

} // namespace jit

#endif // __JIT_DEBUG_HXX__
