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

namespace jit
{

    void JITVisitor::visit(const ast::AssignExp & e)
    {
        if (e.getLeftExp().isSimpleVar()) // A = ...
        {
            const ast::Exp & rExp = e.getRightExp();
            const symbol::Symbol & Lsym = static_cast<ast::SimpleVar &>(e.getLeftExp()).getSymbol();
            if (rExp.isSimpleVar())
            {
                // A = B so we just share the data
                const symbol::Symbol & Rsym = static_cast<const ast::SimpleVar &>(rExp).getSymbol();
                JITScilabPtr & Lvalue = variables.find(Lsym)->second;
                JITScilabPtr & Rvalue = variables.find(Rsym)->second;
                Lvalue->storeRows(*this, Rvalue->loadRows(*this));
                Lvalue->storeCols(*this, Rvalue->loadCols(*this));
                Lvalue->storeData(*this, Rvalue->loadData(*this));
                if (rExp.getDecorator().getResult().getType().type == analysis::TIType::COMPLEX)
                {
                    Lvalue->storeImag(*this, Rvalue->loadImag(*this));
                }
            }
            else
            {
                rExp.accept(*this);
                // A = foo(...)...
                if (!rExp.isCallExp())// && !rExp.isMemfillExp())
                {
                    JITScilabPtr & Lvalue = variables.find(Lsym)->second;
                    JITScilabPtr & Rvalue = getResult();
                    Lvalue->storeRows(*this, Rvalue->loadRows(*this));
                    Lvalue->storeCols(*this, Rvalue->loadCols(*this));
                    Lvalue->storeData(*this, Rvalue->loadData(*this));
                    if (rExp.getDecorator().getResult().getType().type == analysis::TIType::COMPLEX)
                    {
                        Lvalue->storeImag(*this, Rvalue->loadImag(*this));
                    }
                }
            }
        }
        else if (e.getLeftExp().isCallExp()) // A(12) = ...
        {
            ast::CallExp & ce = static_cast<ast::CallExp &>(e.getLeftExp());
            if (ce.getName().isSimpleVar())
            {
                // We have an insertion
                /**
                 *  Several possibilities:
                 *    i) A(I) = B(I): usually in Scilab that means:
                 *       temp = B(I) and then A(I) = temp
                 *       If we infered that the ext/ins is safe we can make a for loop:
                 *          for k = 1:size(I,'*'), A(I(k)) = B(I(k)), end
                 *    ii) A(I) = fun(I): in the general case we should try to devectorize the expression
                 *    iii) A(i) = B(i): no problem
                 */
                const ast::SimpleVar & var = static_cast<ast::SimpleVar &>(ce.getName());
                const symbol::Symbol & symL = var.getSymbol();
                if (e.getDecorator().safe && ce.getDecorator().getResult().getType().isscalar())
                {
                    const analysis::TIType & ty = var.getDecorator().getResult().getType();
                    if (ty.isscalar())
                    {
                        JITScilabPtr & Lvalue = variables.find(symL)->second;
                        e.getRightExp().accept(*this);
                        JITScilabPtr & Rvalue = getResult();
                        Lvalue->storeData(*this, Rvalue->loadData(*this));
                        if (ty.type == analysis::TIType::COMPLEX)
                        {
                            Lvalue->storeImag(*this, Rvalue->loadImag(*this));
                        }
                    }
                    else
                    {
			if (e.getRightExp().isCallExp())
			{
			    e.getRightExp().accept(*this);
			}
			else
			{
			    llvm::Value * ptr = getPtrFromIndex(ce);
			    e.getRightExp().accept(*this);
			    builder.CreateStore(getResult()->loadData(*this), ptr);
			}
		    }
                }
            }
        }
        else if (e.getLeftExp().isAssignListExp()) // [A, B] = ...
        {
            ast::AssignListExp & ale = static_cast<ast::AssignListExp &>(e.getLeftExp());
            if (e.getRightExp().isCallExp())
            {
                e.getRightExp().accept(*this);
            }
        }
    }

}
