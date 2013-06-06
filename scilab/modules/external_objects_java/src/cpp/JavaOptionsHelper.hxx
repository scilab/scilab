/*
 * PIMS ( http://forge.scilab.org/index.php/p/pims ) - This file is part of PIMS
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

#ifndef __JAVAOPTIONSHELPER_HXX__
#define __JAVAOPTIONSHELPER_HXX__

#include "OptionsHelper.hxx"
#include "JavaOptionsSetter.hxx"

#include <jni.h>

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{

class JavaOptionsHelper : public OptionsHelper
{
    bool showPrivate;
    bool attachModule;

public :

    JavaOptionsHelper () : OptionsHelper(), showPrivate(false), attachModule(false)
    {

    }

    ~JavaOptionsHelper() { }

    void setShowPrivate(const bool _showPrivate)
    {
        showPrivate = _showPrivate;
    }

    bool getShowPrivate() const
    {
        return showPrivate;
    }

    void setAttachModule(const bool _attachModule)
    {
        attachModule = _attachModule;
    }

    bool getAttachModule() const
    {
        return attachModule;
    }

    JavaOptionsSetter getSetter(unsigned int type)
    {
        return JavaOptionsSetter(*this, type);
    }
};

}

#endif // __JAVAOPTIONSHELPER_HXX__
