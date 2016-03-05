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

/*--------------------------------------------------------------------------*/

#ifndef __SCILABGATEWAYOPTIONS_HXX__
#define __SCILABGATEWAYOPTIONS_HXX__

extern "C"
{
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
