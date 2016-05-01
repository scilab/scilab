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

#include "data/TemporaryManager.hxx"

namespace analysis
{

int TemporaryManager::getTmp(const TIType & type, const bool isAnInt)
{
    TypeLocal tl = TypeLocal::get(type, isAnInt);
    if (availableTmp.empty())
    {
        usedTmp.emplace(currentId, tl);
        return currentId++;
    }
    else
    {
        auto i = availableTmp.find(tl);
        if (i == availableTmp.end())
        {
            usedTmp.emplace(currentId, tl);
            return currentId++;
        }
        else
        {
            const int id = i->second.top();
            i->second.pop();
            if (i->second.empty())
            {
                availableTmp.erase(i);
            }
            usedTmp.emplace(id, tl);

            return id;
        }
    }
}

void TemporaryManager::releaseTmp(const int id)
{
    if (id >= 0)
    {
        const TypeLocal & tl = usedTmp.find(id)->second;
        auto i = availableTmp.find(tl);
        if (i == availableTmp.end())
        {
            i = availableTmp.emplace(tl, std::stack<int>()).first;
        }
        i->second.push(id);
    }
}

const std::map<TypeLocal, std::stack<int>> & TemporaryManager::getTemp() const
{
    return availableTmp;
}

} // namespace analysis
