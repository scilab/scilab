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

/*--------------------------------------------------------------------------*/

#ifndef __SCILABGATEWAYOPTIONS_HXX__
#define __SCILABGATEWAYOPTIONS_HXX__

extern "C" {
#include "api_scilab.h"
}

namespace org_modules_external_objects
{

class ScilabGatewayOptions
{

    int objId;
    bool isNew;
    const char * methodName;

public:

    ScilabGatewayOptions() : objId(0), isNew(false), methodName(0) { }

    ~ScilabGatewayOptions() { }

    void setObjId(const int _objId)
    {
        objId = _objId;
    }

    int getObjId() const
    {
        return objId;
    }

    void setIsNew(const bool _isNew)
    {
        isNew = _isNew;
    }

    bool getIsNew() const
    {
        return isNew;
    }

    void setMethodName(const char * _methodName)
    {
        if (methodName)
        {
            freeAllocatedSingleString(const_cast<char *>(methodName));
        }
        methodName = _methodName;
    }

    const char * getMethodName() const
    {
        return methodName;
    }

};
}

#endif // __SCILABGATEWAYOPTIONS_HXX__
