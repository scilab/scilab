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

#ifndef __JIT_BINOP_HXX__
#define __JIT_BINOP_HXX__

#include "JITCall.hxx"

namespace jit
{
    
    class JITBinOp : public JITCall
    {

	std::string scilabName;
	
    public:
	
        JITBinOp(const std::string & _scilabName) : JITCall(), scilabName(_scilabName) { }
	
	virtual ~JITBinOp() { }

	virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);

	virtual bool op_S_S(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit) = 0;
    };

    class JITAddOp : public JITBinOp
    {

    public:
	
        JITAddOp() : JITBinOp("add") { }

	virtual bool op_S_S(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);
	
    };

} // namespace jit

#endif // __JIT_BINOP_HXX__
