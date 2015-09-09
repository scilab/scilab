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

#ifndef __JIT_SHORTCUT_BINOP_CALL_HXX__
#define __JIT_SHORTCUT_BINOP_CALL_HXX__

#include "JITCall.hxx"

namespace jit
{

    // TODO: add shortcut for not operator (~)
    
    class JITShortcutBinOpCall : public JITCall
    {

	const std::string scilabName;
	
    public:
	
        JITShortcutBinOpCall(const std::string & _scilabName) : scilabName(_scilabName) { }
	
	virtual ~JITShortcutBinOpCall() { }

	virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);
	virtual void init(const std::vector<analysis::TIType> & args, JITVisitor & jit) { }
	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit) = 0;
	virtual JITScilabPtr SM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	virtual JITScilabPtr MS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	virtual JITScilabPtr MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
    };

    class JITShortcutEq : public JITShortcutBinOpCall
    {

    public:
	
        JITShortcutEq() : JITShortcutBinOpCall("breakOn_eq") { }
	~JITShortcutEq() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	
    };
    
    class JITShortcutNe : public JITShortcutBinOpCall
    {

    public:
	
        JITShortcutNe() : JITShortcutBinOpCall("breakOn_ne") { }
	~JITShortcutNe() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	
    };

    class JITShortcutGt : public JITShortcutBinOpCall
    {

    public:
	
        JITShortcutGt() : JITShortcutBinOpCall("breakOn_gt") { }
	~JITShortcutGt() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	
    };

    class JITShortcutLt : public JITShortcutBinOpCall
    {

    public:
	
        JITShortcutLt() : JITShortcutBinOpCall("breakOn_lt") { }
	~JITShortcutLt() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	
    };

    class JITShortcutGe : public JITShortcutBinOpCall
    {

    public:
	
        JITShortcutGe() : JITShortcutBinOpCall("breakOn_ge") { }
	~JITShortcutGe() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	
    };

    class JITShortcutLe : public JITShortcutBinOpCall
    {

    public:
	
        JITShortcutLe() : JITShortcutBinOpCall("breakOn_le") { }
	~JITShortcutLe() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITVisitor & jit);
	
    };

    
} // namespace jit

#endif // __JIT_SHORTCUT_BINOP_CALL_HXX__
