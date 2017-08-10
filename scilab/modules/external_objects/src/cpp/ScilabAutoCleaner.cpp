/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include "ScilabAutoCleaner.hxx"
#include <iterator>

namespace org_modules_external_objects
{

std::stack<_MapIds> ScilabAutoCleaner::stack;

void ScilabAutoCleaner::registerVariable(const int envId, const int varId)
{
    //const int level = C2F(recu).macr;
    //const int ssize = stack.size() - 1;

    //if (level == ssize)
    //{
    //    _MapIds & map = stack.top();
    //    _MapIds::iterator it = map.find(envId);
    //    if (it != map.end())
    //    {
    //        it->second.insert(varId);
    //    }
    //    else
    //    {
    //        std::set<int> set;
    //        set.insert(varId);
    //        map[envId] = set;
    //    }
    //}
    //else if (level > ssize)
    //{
    //    for (int i = 0; i < level - ssize; i++)
    //    {
    //        stack.push(_MapIds());
    //    }

    //    _MapIds & map = stack.top();
    //    std::set<int> set;
    //    set.insert(varId);
    //    map[envId] = set;
    //}
}

void ScilabAutoCleaner::unregisterVariable(const int envId, const int varId)
{
    unregisterVariable(envId, &varId, 1);
}

void ScilabAutoCleaner::unregisterVariable(const int envId, const int* varId, const int length)
{
    //    const int level = C2F(recu).macr;
    //    const int ssize = stack.size() - 1;
    //
    //    if (level == ssize)
    //    {
    //        _MapIds & map = stack.top();
    //        _MapIds::iterator it = map.find(envId);
    //        if (it != map.end())
    //        {
    //            for (int i = 0; i < length; i++)
    //            {
    //                it->second.erase(varId[i]);
    //            }
    //        }
    //    }
}

void ScilabAutoCleaner::goDown(void* _pvCtx)
{
    //if (stack.size() >= 2)
    //{
    //    const int level = C2F(recu).macr;
    //    const int ssize = stack.size() - 1;

    //    if (level < ssize)
    //    {
    //        _MapIds current = getAllObjectsAtCurrentLevel(_pvCtx);
    //        for (int i = 0; i < ssize - level; i++)
    //        {
    //            _MapIds & map = stack.top();
    //            removeUnusedObjects(current, map, 0);
    //            stack.pop();
    //        }

    //        if (level == 0 && stack.size() == 1)
    //        {
    //            removeUnusedObjects(current, stack.top(), 0);
    //        }
    //    }
    //}
}

_MapIds ScilabAutoCleaner::getAllObjectsAtCurrentLevel(void * pvApiCtx)
{
    _MapIds ids;
    //int cbot = Bot;
    //int k;
    //int last;

    //// Following code has been found in stackg.f
    //if (C2F(recu).macr != 0)
    //{
    //    k = C2F(iop).lpt[1 - 1] - (13 + nsiz);
    //    last = C2F(iop).lin[k + 5 - 1] - 1;
    //}
    //else
    //{
    //    last = C2F(vstk).isiz - 1;
    //}

    //for (k = cbot; k <= last; k++)
    //{
    //    int * addr = istk(iadr(*Lstk(k)));
    //    if (ScilabObjects::isValidExternal(addr, pvApiCtx))
    //    {
    //        int envId = ScilabObjects::getEnvironmentId(addr, pvApiCtx);
    //        int varId = ScilabObjects::getExternalId(addr, pvApiCtx);
    //        ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(envId);

    //        env.writeLog("getAllObjectsAtCurrentLevel", "Found id=%d at macro level %d.", varId, C2F(recu).macr);

    //        _MapIds::iterator it = ids.find(envId);
    //        if (it != ids.end())
    //        {
    //            it->second.insert(varId);
    //        }
    //        else
    //        {
    //            std::set<int> set;
    //            set.insert(varId);
    //            ids[envId] = set;
    //        }
    //    }
    //}

    return ids;
}

void ScilabAutoCleaner::removeUnusedObjects(const _MapIds & current, const _MapIds & previous, void * pvApiCtx)
{
    for (_MapIds::const_iterator itm = previous.begin(); itm != previous.end(); itm++)
    {
        _MapIds::const_iterator it = current.find(itm->first);
        if (it != current.end())
        {
            std::set<int> current_ids = it->second;
            std::set<int> previous_ids = itm->second;
            std::set<int> unused_ids;
            std::set_difference(previous_ids.begin(), previous_ids.end(), current_ids.begin(), current_ids.end(), std::inserter(unused_ids, unused_ids.end()));

            if (!unused_ids.empty())
            {
                ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(itm->first);

                for (std::set<int>::const_iterator itu = unused_ids.begin(); itu != unused_ids.end(); itu++)
                {
                    env.writeLog("removeUnusedObjects", "autoremove id=%d.", *itu);
                    env.autoremoveobject(*itu);
                }
            }
        }
        else
        {
            ScilabAbstractEnvironment & env = ScilabEnvironments::getEnvironment(itm->first);

            for (std::set<int>::const_iterator itu = itm->second.begin(); itu != itm->second.end(); itu++)
            {
                env.writeLog("removeUnusedObjects", "autoremove id=%d.", *itu);
                env.autoremoveobject(*itu);
            }
        }
    }
}
}
