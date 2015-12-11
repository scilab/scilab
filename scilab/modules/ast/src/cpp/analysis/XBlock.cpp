/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
