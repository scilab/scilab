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

#include "data/LoopBlock.hxx"
#include "data/LoopAnalyzer.hxx"

namespace analysis
{
bool LoopBlock::requiresAnotherTrip()
{
    if (first)
    {
        bool ret = false;
        for (auto & p : symMap)
        {
            tools::SymbolMap<Info>::iterator it;
            if (p.second.type.type != TIType::UNKNOWN)
            {
                Block * block = getParent()->getDefBlock(p.first, it, false);
                if (block)
                {
                    const Info & info = it->second;
                    if (info.type != p.second.type || (info.type.type == TIType::DOUBLE && info.isAnInt() != p.second.isAnInt()))
                    {
                        getParent()->getExp()->getDecorator().addPromotion(p.first, info.type, p.second.type);
                        ret = true;
                    }
                    else if (!info.data->same(p.second.data))
                    {
                        ret = true;
                    }
                }
            }
        }
        return ret;
    }
    else
    {
        tools::SymbolMap<Info> & map = getParent()->getFirstBlock()->getMap();
        for (auto & p : symMap)
        {
            auto i = map.find(p.first);
            if (i != map.end())
            {
                Info & info = i->second;
                if (info.type != p.second.type)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

Block * LoopBlock::getDefBlock(const symbol::Symbol & sym, tools::SymbolMap<Info>::iterator & it, const bool global)
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

void LoopBlock::clone(const symbol::Symbol & sym, ast::Exp * exp)
{
    clonedSym.emplace(exp, sym);
}

void LoopBlockHead::finalize()
{
    if (blocks.size() == 2)
    {
        LoopBlock * fb = static_cast<LoopBlock *>(getFirstBlock());
        LoopBlock * sb = static_cast<LoopBlock *>(getSecondBlock());
        std::unordered_map<ast::Exp *, symbol::Symbol> onlyInFirst;
        std::unordered_map<ast::Exp *, symbol::Symbol> inBoth;

        for (const auto & p : fb->clonedSym)
        {
            const auto i = sb->clonedSym.find(p.first);
            if (i == sb->clonedSym.end())
            {
                // In first and not in second
                onlyInFirst.emplace(p);
            }
            else
            {
                // In first and in second
                inBoth.emplace(p);
                sb->clonedSym.erase(i);
            }
        }

        // Since we removed common elements from sb->clonedSym
        // the remainder is the elements which are only in second

        // Now we merge
        // i) clone is only in the first loop so we can move it in the loop header
        if (parent && !onlyInFirst.empty())
        {
            for (auto & p : onlyInFirst)
            {
                parent->clone(p.second, exp);
            }
        }

        // ii) clone is in first and in second so we clone the data at each iteration
        // iii) clone is only in second so we clone the data at each iteration except for the first one
        //      => for now, we clone at each iteration (TODO: improve that in removing clone at the first iteration)
        for (auto & p : inBoth)
        {
            p.first->getDecorator().addClone(p.second);
        }

        for (auto & p : sb->clonedSym)
        {
            p.first->getDecorator().addClone(p.second);
        }

        pullup(symMap);
        pullup(getSecondBlock()->getMap());
    }
    else
    {
        LoopBlock * fb = static_cast<LoopBlock *>(getFirstBlock());
        if (parent && !fb->clonedSym.empty())
        {
            for (auto & p : fb->clonedSym)
            {
                parent->clone(p.second, p.first);
            }
        }

        pullup(symMap);
        pullup(getFirstBlock()->getMap());
    }
}
}
