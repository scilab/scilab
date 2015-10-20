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

#ifndef __JIT_JITTER_HXX__
#define __JIT_JITTER_HXX__

#include "internal.hxx"

namespace ast
{
    class CallExp;
}

namespace jit
{

    class JITVisitor;
    
    class Jitter
    {
    public:
	
	static bool analyzeAndJIT(const ast::CallExp & ce, const types::typed_list & in, types::typed_list & out);
	
    };
}

#endif // __JIT_JITTER_HXX__
