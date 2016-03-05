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

#ifndef __SCILABAUTOCLEANER_HXX__
#define __SCILABAUTOCLEANER_HXX__

#include <algorithm>
#include <set>
#include <stack>
#include <map>

#include "ScilabEnvironments.hxx"
#include "ScilabAbstractEnvironment.hxx"
#include "ScilabObjects.hxx"
#include "dynlib_external_objects_scilab.h"

extern "C" {
#include "api_scilab.h"
}

namespace org_modules_external_objects
{

typedef std::map<int, std::set<int> > _MapIds;

class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabAutoCleaner
{
    static std::stack< _MapIds > stack;

public:
    static void goDown(void* _pvCtx);
    static void registerVariable(const int envId, const int varId);
    static void unregisterVariable(const int envId, const int varId);
    static void unregisterVariable(const int envId, const int * varId, const int length);

private:
    static _MapIds getAllObjectsAtCurrentLevel(void * pvApiCtx);
    static void removeUnusedObjects(const _MapIds & current, const _MapIds & previous, void * pvApiCtx);
};
}

#endif // __SCILABAUTOCLEANER_HXX__
