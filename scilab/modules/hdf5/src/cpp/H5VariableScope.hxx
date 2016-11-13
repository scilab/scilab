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

#ifndef __H5VARIABLESCOPE_HXX__
#define __H5VARIABLESCOPE_HXX__

#include <stack>
#include <vector>

namespace org_modules_hdf5
{

class H5Object;

class H5VariableScope
{

    static std::vector<H5Object *> scope;
    static std::stack<int> freePlaces;

public:

    static int getVariableId(H5Object & obj);
    static H5Object * getVariableFromId(const int id);
    static void removeId(const int id);
    static void removeIdAndDelete(const int id);
    static void clearScope();
    static void initScope();
};
}

#endif // __H5VARIABLESCOPE_HXX__

