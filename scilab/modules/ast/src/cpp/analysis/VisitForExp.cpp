/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
