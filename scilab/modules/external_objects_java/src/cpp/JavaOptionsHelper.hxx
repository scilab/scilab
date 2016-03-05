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

public :

    JavaOptionsHelper () : OptionsHelper(), showPrivate(false)
    {
        setUseScilabIndex(true);
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

    JavaOptionsSetter getSetter(unsigned int type)
    {
        return JavaOptionsSetter(*this, type);
    }
};

}

#endif // __JAVAOPTIONSHELPER_HXX__
