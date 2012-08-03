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

#ifndef __OPTIONSHELPER_HXX__
#define __OPTIONSHELPER_HXX__

#include "ScilabOptionsSetter.hxx"
#include "dynlib_external_objects_scilab.h"

namespace org_modules_external_objects
{

class EXTERNAL_OBJECTS_SCILAB_IMPEXP OptionsHelper
{
    static bool copyOccured;
    static int envId;

    bool useLastName;
    bool methodOfConv;
    bool allowReload;
    bool autoUnwrap;
    bool useCopy;
    bool useScilabIndex;
    bool newAllowed;

public:

    OptionsHelper() : useLastName(true), methodOfConv(false), allowReload(false), autoUnwrap(true), useCopy(true), useScilabIndex(false), newAllowed(true) { }

    ~OptionsHelper() { }

    static void setCopyOccured(const bool _copyOccured)
    {
        copyOccured = _copyOccured;
    }

    static bool getCopyOccured()
    {
        return copyOccured;
    }

    static void setEnvId(const int _envId)
    {
        envId = _envId;
    }

    static int getEnvId()
    {
        return envId;
    }

    void setNewAllowed(const bool _newAllowed)
    {
        newAllowed  = _newAllowed;
    }

    bool getNewAllowed() const
    {
        return newAllowed;
    }

    void setUseLastName(const bool _useLastName)
    {
        useLastName  = _useLastName;
    }

    bool getUseLastName() const
    {
        return useLastName;
    }

    void setMethodOfConv(const bool _methodOfConv)
    {
        methodOfConv = _methodOfConv;
    }

    void setAllowReload(const bool _allowReload)
    {
        allowReload = _allowReload;
    }

    void setAutoUnwrap(const bool _autoUnwrap)
    {
        autoUnwrap = _autoUnwrap;
    }

    bool getMethodOfConv() const
    {
        return methodOfConv;
    }

    bool getAllowReload() const
    {
        return allowReload;
    }

    bool getAutoUnwrap() const
    {
        return autoUnwrap;
    }

    void setUseCopy(const bool _useCopy)
    {
        useCopy = _useCopy;
    }

    bool getUseCopy()
    {
        return useCopy;
    }

    void setUseScilabIndex(const bool _useScilabIndex)
    {
        useScilabIndex = _useScilabIndex;
    }


    bool getUseScilabIndex()
    {
        return useScilabIndex;
    }

    ScilabOptionsSetter getSetter(unsigned int type)
    {
        return ScilabOptionsSetter(*this, type);
    }

};
}

#endif // __OPTIONSHELPER_HXX__
