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

#ifndef __SCILABOBJECTS_HXX__
#define __SCILABOBJECTS_HXX__

#include <string.h>

#include "ExternalObjects.h"
#include "ScilabAbstractEnvironment.hxx"
#include "ScilabEnvironments.hxx"
#include "OptionsHelper.hxx"
#include "dynlib_external_objects_scilab.h"

extern "C" {
#include "api_scilab.h"
#include "stack-c.h"
#include "localization.h"
}

namespace org_modules_external_objects
{
class EXTERNAL_OBJECTS_SCILAB_IMPEXP ScilabObjects
{
    static bool isInit;
    static const char * _EOBJ[];
    static const char * _ECLASS[];
    static const char * _EVOID[];
    static const char * _INVOKE_;

public:

    static void initialization(ScilabAbstractEnvironment & env, void * pvApiCtx);

    static int createNamedEnvironmentObject(int type, const char * name, int id, const int envId, void * pvApiCtx);

    static void createEnvironmentObjectAtPos(int type, int pos, int id, const int envId, void * pvApiCtx);

    static void copyInvocationMacroToStack(int pos, ScilabAbstractEnvironment & env, void * pvApiCtx);

    static void removeTemporaryVars(const int envId, int * tmpvar);

    static void removeVar(int * addr, void * pvApiCtx);

    static bool unwrap(int idObj, int pos, const int envId, void * pvApiCtx);

    static int getMListType(int * mlist, void * pvApiCtx);

    static bool isValidExternal(int * mlist, void * pvApiCtx);

    static bool isExternalObj(int * mlist, void * pvApiCtx);

    static bool isExternalClass(int * mlist, void * pvApiCtx);

    static bool isExternalVoid(int * mlist, void * pvApiCtx);

    static bool isExternalObjOrClass(int * mlist, void * pvApiCtx);

    static char * getSingleString(int pos, void * pvApiCtx);

    static int isPositiveIntegerAtAddress(int * addr, void * pvApiCtx);

    static int getEnvironmentId(int * addr, void * pvApiCtx);

    static int getExternalId(int * addr, void * pvApiCtx);

    static int getArgumentId(int * addr, int * tmpvars, const bool isRef, const bool isClass, const int envId, void * pvApiCtx);

    template <typename T>
    inline static int wrap(const int row, const int col, T * data, const ScilabAbstractEnvironmentWrapper & wrapper, const bool isRef)
    {
        if (row == 0 || col == 0)
        {
            return 0;
        }
        else if (row == 1 && col == 1)
        {
            return wrapper.wrap(data, isRef);
        }
        else if (row == 1)
        {
            return wrapper.wrap(data, col, isRef);
        }

        return wrapper.wrap(data, row, col, isRef);
    }

    inline static int wrapBool(const int row, const int col, int * data, const ScilabAbstractEnvironmentWrapper & wrapper, const bool isRef)
    {
        if (row == 0 || col == 0)
        {
            return 0;
        }
        else if (row == 1 && col == 1)
        {
            return wrapper.wrapBool(data, isRef);
        }
        else if (row == 1)
        {
            return wrapper.wrapBool(data, col, isRef);
        }

        return wrapper.wrapBool(data, row, col, isRef);
    }

    inline static int wrap(const int row, const int col, double * real, double * imag, const ScilabAbstractEnvironmentWrapper & wrapper, const bool isRef)
    {
        if (row == 0 || col == 0)
        {
            return 0;
        }
        else if (row == 1 && col == 1)
        {
            return wrapper.wrap(real, imag, isRef);
        }
        else if (row == 1)
        {
            return wrapper.wrap(real, imag, col, isRef);
        }

        return wrapper.wrap(real, imag, row, col, isRef);
    }
};
}

#endif // __SCILABOBJECTS_HXX__
