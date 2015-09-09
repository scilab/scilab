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

#ifndef __JIT_BINOP_CALL_HXX__
#define __JIT_BINOP_CALL_HXX__

#include "JITCall.hxx"

namespace jit
{
    
    class JITBinOpCall : public JITCall
    {

    protected:

	const std::string scilabName;
	
    public:
	
        JITBinOpCall(const std::string & _scilabName) : scilabName(_scilabName) { }
	
	virtual ~JITBinOpCall() { }

	virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);
	virtual void init(const std::vector<analysis::TIType> & args, JITVisitor & jit) { }
	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit) = 0;
	virtual void SM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
	virtual void MS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
	virtual void MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITAddition : public JITBinOpCall
    {

    public:
	
	JITAddition() : JITBinOpCall("add") { }

	virtual ~JITAddition() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITSubtraction : public JITBinOpCall
    {

    public:
	
	JITSubtraction() : JITBinOpCall("sub") { }

	virtual ~JITSubtraction() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITMultiplication : public JITBinOpCall
    {

    public:
	
	JITMultiplication() : JITBinOpCall("times") { }

	virtual ~JITMultiplication() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);

	virtual void MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITEquality : public JITBinOpCall
    {

    public:
	
	JITEquality() : JITBinOpCall("eq") { }

	virtual ~JITEquality() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITNotEquality : public JITBinOpCall
    {

    public:
	
	JITNotEquality() : JITBinOpCall("ne") { }

	virtual ~JITNotEquality() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITComparison : public JITBinOpCall
    {

    public:
	
	JITComparison(const std::string & name) : JITBinOpCall(name) { }

	virtual ~JITComparison() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);

	virtual llvm::Value * cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r) = 0;
	virtual llvm::Value * cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r) = 0;
	virtual llvm::Value * cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r) = 0;
    };

    class JITLowerThan : public JITComparison
    {

    public:
	
	JITLowerThan() : JITComparison("lt") { }

	virtual ~JITLowerThan() { }

	virtual llvm::Value * cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
    };

    class JITGreaterThan : public JITComparison
    {

    public:
	
	JITGreaterThan() : JITComparison("gt") { }

	virtual ~JITGreaterThan() { }

	virtual llvm::Value * cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
    };

    class JITLowerOrEq : public JITComparison
    {

    public:
	
	JITLowerOrEq() : JITComparison("le") { }

	virtual ~JITLowerOrEq() { }

	virtual llvm::Value * cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
    };

    class JITGreaterOrEq : public JITComparison
    {

    public:
	
	JITGreaterOrEq() : JITComparison("ge") { }

	virtual ~JITGreaterOrEq() { }

	virtual llvm::Value * cmpIU(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpIS(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
	virtual llvm::Value * cmpD(llvm::IRBuilder<> & builder, llvm::Value * l, llvm::Value * r);
    };
    
    class JITRDivision : public JITBinOpCall
    {

    public:
	
	JITRDivision() : JITBinOpCall("rdiv") { }

	virtual ~JITRDivision() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit);

	virtual void MM(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit);
    };

    class JITPower : public JITBinOpCall
    {

    public:
	
	JITPower() : JITBinOpCall("power") { }
	
	virtual ~JITPower() { }

	virtual JITScilabPtr SS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, const analysis::TIType & Otype, JITVisitor & jit) override;

	virtual void MS(JITScilabPtr & L, const analysis::TIType & Ltype, JITScilabPtr & R, const analysis::TIType & Rtype, JITScilabPtr & O, const analysis::TIType & Otype, JITVisitor & jit) override;

    private:

	static llvm::Value * pow_d_d(JITVisitor & jit, llvm::Value * l, llvm::Value * r);
	static std::pair<llvm::Value *, llvm::Value *> pow_c_d(JITVisitor & jit, llvm::Value * re, llvm::Value * im, llvm::Value * r);
    };
    
} // namespace jit

#endif // __JIT_BINOP_CALL_HXX__
