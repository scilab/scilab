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

#include "data/LoopBlock.hxx"

namespace analysis
{
bool LoopBlock::requiresAnotherTrip()
{
    if (first)
    {
        for (auto & p : symMap)
        {
            std::map<symbol::Symbol, Info>::iterator it;
            Block * block = getParent()->getDefBlock(p.first, it, false);
            if (block)
            {
                Info & info = it->second;
                if (info.type != p.second.type || !info.data->same(p.second.data))
                {
                    return true;
                }
            }
        }
    }

    return false;
}

    Block * LoopBlock::getDefBlock(const symbol::Symbol & sym, std::map<symbol::Symbol, Info>::iterator & it, const bool global)
{
    if (first)
    {
        return Block::getDefBlock(sym, it, global);
    }
    else
    {
        it = symMap.find(sym);
        if (it != symMap.end())
        {
            return this;
        }
        return getParent()->getFirstBlock()->getDefBlock(sym, it, global);
    }
}

Info & LoopBlock::getInfo(const symbol::Symbol & sym)
{
    if (first)
    {
        return Block::getInfo(sym);
    }
    else
    {
        return getParent()->getFirstBlock()->getInfo(sym);
    }
}

void LoopBlock::finalize() { }

Block * LoopBlockHead::addBlock(const unsigned int id, BlockKind kind, ast::Exp * exp)
{
    Block * b = new LoopBlock(id, this, exp, blocks.size() == 0);
    blocks.push_back(b);

    return b;
}

void LoopBlockHead::finalize()
{
    if (false && blocks.size() == 2)
    {
        // TODO
    }
    else
    {
        pullup(symMap);
        pullup(getFirstBlock()->getMap());
    }
}
}
