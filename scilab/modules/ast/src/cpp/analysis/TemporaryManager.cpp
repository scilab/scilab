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
