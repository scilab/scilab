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

#ifndef __JIT_OPTIMIZED_CALL_1_HXX__
#define __JIT_OPTIMIZED_CALL_1_HXX__

#include "JITCall.hxx"

namespace jit
{

class JITOptimizedCall1 : public JITCall
{

protected:

    const std::string jitName;
    const std::map<analysis::TIType::Type, std::string> map;
    const llvm::Intrinsic::ID id;

public:

    JITOptimizedCall1(const std::string & _jitName, const std::map<analysis::TIType::Type, std::string> & _map, const llvm::Intrinsic::ID _id = llvm::Intrinsic::not_intrinsic) : jitName(_jitName), map(_map), id(_id) { }
    JITOptimizedCall1(const std::string & _jitName, const llvm::Intrinsic::ID _id = llvm::Intrinsic::not_intrinsic) : jitName(_jitName), id(_id) { }

    virtual ~JITOptimizedCall1() { }

    virtual bool invoke(const ast::Exp & e, const std::vector<analysis::TIType> & typesOut, std::vector<JITScilabPtr> & out, JITVisitor & jit);

    virtual void init(const std::vector<analysis::TIType> & args, JITVisitor & jit) { }
    virtual void S(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit);
    virtual void M(const analysis::TIType & argType, JITScilabPtr & arg, const analysis::TIType & OType, JITScilabPtr & out, JITVisitor & jit);
    virtual void Sc_c(const std::string & name, JITScilabPtr & arg, JITScilabPtr & out, JITVisitor & jit);
    
private:

    void makeOutput(JITVisitor & jit, llvm::Value * ret, const std::vector<llvm::Value *> & outArgs, const analysis::TIType & OType, JITScilabPtr & out);

};

} // namespace jit

#endif // __JIT_OPTIMIZED_CALL_1_HXX__
