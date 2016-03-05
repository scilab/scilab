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

#include "H5VariableScope.hxx"
#include "H5Object.hxx"

namespace org_modules_hdf5
{
std::vector<H5Object *> H5VariableScope::scope;
std::stack<int> H5VariableScope::freePlaces;

void H5VariableScope::initScope()
{
    scope.reserve(1024);
}

void H5VariableScope::clearScope()
{
    for (unsigned int i = 0; i < scope.size(); i++)
    {
        delete scope[i];
    }
}

int H5VariableScope::getVariableId(H5Object & obj)
{
    int ret;

    if (!freePlaces.empty())
    {
        ret = freePlaces.top();
        freePlaces.pop();
        scope[ret] = &obj;
    }
    else
    {
        ret = (int)scope.size();
        scope.push_back(&obj);
    }

    return ret;
}

H5Object * H5VariableScope::getVariableFromId(const int id)
{
    if (id >= 0 && id < (int)scope.size())
    {
        return scope[id];
    }

    return 0;
}

void H5VariableScope::removeIdAndDelete(const int id)
{
    if (id >= 0 && id < (int)scope.size() && scope[id])
    {
        H5Object * obj = scope[id];
        scope[id] = 0;
        freePlaces.push(id);
        delete obj;
    }
}

void H5VariableScope::removeId(const int id)
{
    if (id >= 0 && id < (int)scope.size() && scope[id])
    {
        scope[id] = 0;
        freePlaces.push(id);
    }
}
}


