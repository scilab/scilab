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

#include "AnalysisVisitor.hxx"

namespace analysis
{

void AnalysisVisitor::visit(ast::WhileExp & e)
{
    logger.log(L"WhileExp", e.getLocation());
    loops.push(&e);

    dm.addBlock(Block::LOOP, &e);
    e.getTest().accept(*this);
    dm.releaseTmp(getResult().getTempId(), &e.getTest());
    dm.addBlock(Block::NORMAL, &e.getBody());
    e.getBody().accept(*this);

    if (dm.requiresAnotherTrip())
    {
        dm.finalizeBlock();
        dm.addBlock(Block::NORMAL, &e.getBody());

        // Since the variables in the test have maybe changed (e.g. a double-int became a double)
        // we need to re-analyze it to take into account these changes
        e.getTest().accept(*this);
        dm.releaseTmp(getResult().getTempId(), &e.getTest());

        e.getBody().accept(*this);

        if (dm.requiresAnotherTrip())
        {
            std::wcerr << "Invalid forexp: types or refcount are not the same before and after the loop" << std::endl;
        }
    }

    dm.finalizeBlock();
    dm.finalizeBlock();

    loops.pop();
}

} // namespace analysis
