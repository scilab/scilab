/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

#include "data/XBlock.hxx"
#include "data/FunctionBlock.hxx"

namespace analysis
{
void XBlock::finalize() { }

Block * XBlockHead::addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp)
{
    if (kind == MACRO)
    {
        Block * b = new FunctionBlock(id, this, exp);
        testBlocks.push_back(b);

        return b;
    }

    Block * b = new XBlock(id, this, exp);
    blocks.push_back(b);

    return b;
}

void XBlockHead::finalize()
{
    pullup(symMap);
    std::vector<Block *>::iterator first;
    std::vector<Block *>::iterator end = blocks.end();
    for (first = blocks.begin(); first != end; ++first)
    {
        if (!(*first)->getReturn())
        {
            break;
        }
    }

    if (first != end)
    {
        tools::SymbolMap<Info> & map = (*first)->getMap();
        for (std::vector<Block *>::iterator i = std::next(first); i != end; ++i)
        {
            if (!(*i)->getReturn())
            {
                merge(map, (*i)->getMap());
            }
        }
        pullup((*first)->getMap());
    }
}
}
