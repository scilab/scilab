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

void AnalysisVisitor::visit(ast::ForExp & e)
{
    logger.log(L"ForExp", e.getLocation());
    loops.push(&e);

    dm.addBlock(Block::LOOP, &e);
    e.getVardec().accept(*this);
    dm.addBlock(Block::NORMAL, &e.getBody());
    e.getBody().accept(*this);

    if (dm.requiresAnotherTrip())
    {
        std::wcerr << "Invalid forexp: types or refcount are not the same before and after the loop" << std::endl;

        dm.finalizeBlock();
        dm.addBlock(Block::NORMAL, &e.getBody());
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
