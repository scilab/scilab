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

#ifndef __H5VARIABLESCOPE_HXX__
#define __H5VARIABLESCOPE_HXX__

#include <stack>
#include <vector>

namespace org_modules_hdf5
{

class H5Object;

class H5VariableScope
{

    static std::vector<H5Object *> & scope;
    static std::stack<int> & freePlaces;

public:

    static int getVariableId(H5Object & obj);
    static H5Object * getVariableFromId(const int id);
    static void removeId(const int id);
    static void removeIdAndDelete(const int id);
    static void clearScope();

private :

    static std::vector<H5Object *> * initScope()
    {
        std::vector<H5Object *> * _scope = new std::vector<H5Object *>();
        _scope->reserve(1024);

        return _scope;
    }
};
}

#endif // __H5VARIABLESCOPE_HXX__
