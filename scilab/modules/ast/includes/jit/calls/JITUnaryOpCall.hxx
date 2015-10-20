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

#ifndef __JIT_UNARYOP_CALL_HXX__
#define __JIT_UNARYOP_CALL_HXX__

#include "JITCall.hxx"

namespace jit
{
    
    class JITUnaryOpCall : public JITCall
    {

    protected:

	const std::string scilabName;
	
    public:
	
        JITUnaryOpCall(const std::string & _scilabName) : scilabName(_scilabName) { }
	
	virtual ~JITUnaryOpCall() { }

	virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);
	virtual void init(const std::vector<analysis::TIType> & args, JITVisitor & jit) { }
	virtual JITScilabPtr S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit) = 0;
	virtual void M(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITOpposite : public JITUnaryOpCall
    {

    public:
	
	JITOpposite() : JITUnaryOpCall("opp") { }

	virtual ~JITOpposite() { }

	virtual JITScilabPtr S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITNegation : public JITUnaryOpCall
    {
	
    public:
	
	JITNegation() : JITUnaryOpCall("neg") { }

	virtual ~JITNegation() { }

	virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);
	virtual JITScilabPtr S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITTransposition : public JITUnaryOpCall
    {
	
    public:
	
	JITTransposition() : JITUnaryOpCall("transp") { }

	virtual ~JITTransposition() { }

	virtual JITScilabPtr S(JITScilabPtr & L, const analysis::TIType & Ltype, const analysis::TIType & Otype, JITVisitor & jit);
	virtual void M(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
    };
    
    
} // namespace jit

#endif // __JIT_UNARYOP_CALL_HXX__
