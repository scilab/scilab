/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "H5VariableScope.hxx"
#include "H5Object.hxx"

namespace org_modules_hdf5
{
std::vector<H5Object *> & H5VariableScope::scope = *initScope();
std::stack<int> & H5VariableScope::freePlaces = *new std::stack<int>();

void H5VariableScope::clearScope()
{
    for (unsigned int i = 0; i < scope.size(); i++)
    {
        delete scope[i];
    }

    delete &scope;
    scope = *initScope();

    delete &freePlaces;
    freePlaces = *new std::stack<int>();
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

