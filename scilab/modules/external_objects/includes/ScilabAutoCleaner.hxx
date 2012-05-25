/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "stack-c.h"
}

namespace org_modules_external_objects
{

typedef std::map<int, std::set<int> > _MapIds;

class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabAutoCleaner
{
    static std::stack< _MapIds > stack;

public:
    static void goDown();
    static void registerVariable(const int envId, const int varId);
    static void unregisterVariable(const int envId, const int varId);

private:
    static _MapIds getAllObjectsAtCurrentLevel(void * pvApiCtx);
    static void removeUnusedObjects(const _MapIds & current, const _MapIds & previous, void * pvApiCtx);
};
}

#endif // __SCILABAUTOCLEANER_HXX__
