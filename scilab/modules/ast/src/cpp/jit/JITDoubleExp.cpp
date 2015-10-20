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

    void JITVisitor::visit(const ast::DoubleExp & e)
    {
        if (e.getDecorator().getResult().isAnInt())
        {
            setResult(JITScilabPtr(new JITScalInt64(*this, (int64_t)e.getValue(), false, "")));
        }
        else
        {
            if (types::Double * pDbl = static_cast<types::Double *>(e.getConstant()))
            {
                if (pDbl->isComplex())
                {
                    setResult(JITScilabPtr(new JITScalComplex(*this, std::complex<double>(pDbl->get(0), pDbl->getImg(0)), false, "")));
                }
                else
                {
                    setResult(JITScilabPtr(new JITScalDouble(*this, pDbl->get(0), false, "")));
                }
            }
            else
            {
                setResult(JITScilabPtr(new JITScalDouble(*this, e.getValue(), false, "")));
            }
        }
    }

}
