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

#ifndef __OPTIONSHELPER_HXX__
#define __OPTIONSHELPER_HXX__

#include "ScilabOptionsSetter.hxx"
#include "dynlib_external_objects_scilab.h"

namespace org_modules_external_objects
{

class EXTERNAL_OBJECTS_SCILAB_IMPEXP OptionsHelper
{
    static bool copyOccurred;
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

    static void setCopyOccurred(const bool _copyOccurred)
    {
        copyOccurred = _copyOccurred;
    }

    static bool getCopyOccurred()
    {
        return copyOccurred;
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
