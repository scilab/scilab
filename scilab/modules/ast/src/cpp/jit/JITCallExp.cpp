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

#include "JITScalars.hxx"
#include "JITArrayofs.hxx"
#include "JITVisitor.hxx"
#include "calls/JITCall.hxx"

namespace jit
{

void JITVisitor::visit(const ast::CallExp & e)
{
    if (e.getName().isSimpleVar())
    {
        const ast::SimpleVar & var = static_cast<const ast::SimpleVar &>(e.getName());
        const analysis::TIType & funty = var.getDecorator().getResult().getType();

        if (funty.type == analysis::TIType::FUNCTION || funty.type == analysis::TIType::MACRO || funty.ismatrix())
        {
            if (e.getParent()->isAssignExp())
            {
                const ast::AssignExp & ae = *static_cast<const ast::AssignExp *>(e.getParent());
                if (ae.getLeftExp().isSimpleVar()) // A = ...
                {
                    const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar &>(ae.getLeftExp()).getSymbol();
                    std::vector<analysis::TIType> typesOut;
                    typesOut.emplace_back(ae.getLeftExp().getDecorator().getResult().getType());
                    std::vector<JITScilabPtr> out;
                    out.emplace_back(variables.find(Lsym)->second);
                    JITCall::call(e, typesOut, out, *this);
                }
                else if (ae.getLeftExp().isAssignListExp()) // [A, B] = ...
                {
                    ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(ae.getLeftExp());
                    std::vector<analysis::TIType> typesOut;
                    std::vector<JITScilabPtr> out;
                    const ast::exps_t & exps = ale.getExps();
                    out.reserve(exps.size());

                    for (const auto exp : exps)
                    {
                        if (exp->isSimpleVar())
                        {
                            const symbol::Symbol & Lsym = static_cast<const ast::SimpleVar *>(exp)->getSymbol();
                            out.emplace_back(variables.find(Lsym)->second);
                            typesOut.emplace_back(exp->getDecorator().getResult().getType());
                        }
                    }
                    JITCall::call(e, typesOut, out, *this);
                }
                else if (ae.getLeftExp().isCallExp()) // A(i, j) = ...
                {
                    const ast::CallExp & ce = static_cast<const ast::CallExp &>(ae.getLeftExp());
                    llvm::Value * ptr = getPtrFromIndex(ce);
                    std::vector<analysis::TIType> typesOut;
                    typesOut.emplace_back(ce.getDecorator().getResult().getType());
                    std::vector<JITScilabPtr> out;
                    out.emplace_back(getCreatedScalar(ptr, ce.getDecorator().getResult().getType().type, true, ""));
                    JITCall::call(e, typesOut, out, *this);
                }
            }
            else
            {
                std::vector<analysis::TIType> typesOut;
                typesOut.emplace_back(e.getDecorator().getResult().getType());
                std::vector<JITScilabPtr> out;
                JITCall::call(e, typesOut, out, *this);
                if (!out.empty())
                {
                    setResult(out.front());
                }
            }
        }
    }
}

}
