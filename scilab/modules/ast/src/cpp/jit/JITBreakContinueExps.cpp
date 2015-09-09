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
    
    void JITVisitor::visit(const ast::BreakExp & e)
    {
	if (!blocks.empty())
	{
	    CreateBr(blocks.top().second);
	}
    }

    void JITVisitor::visit(const ast::ContinueExp & e)
    {
	if (!blocks.empty())
	{
	    CreateBr(blocks.top().first);
	}
    }

}
