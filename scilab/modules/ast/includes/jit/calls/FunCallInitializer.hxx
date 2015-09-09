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

#ifndef __FUN_CALL_INITIALIZER_HXX__
#define __FUN_CALL_INITIALIZER_HXX__

#include <vector>

#include "TIType.hxx"

namespace jit
{
    class JITVisitor;
    
    class FunCallInitializer
    {

    public:
	
	virtual void init(const std::vector<analysis::TIType> & args, JITVisitor & jit) { }
	virtual bool hasInit() const { return false; }
    };

} // namespace jit

#endif // __FUN_CALL_INITIALIZER_HXX__
    
