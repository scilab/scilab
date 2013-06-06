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

#include "JavaOptionsHelper.hxx"
#include "JavaOptionsSetter.hxx"

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{
const unsigned int JavaOptionsSetter::SHOWPRIVATE = LAST_OPTION_INDEX + 2;
const unsigned int JavaOptionsSetter::ATTACHMODULE = LAST_OPTION_INDEX + 3;

JavaOptionsSetter::JavaOptionsSetter(JavaOptionsHelper & _helper, unsigned int _type) : ScilabOptionsSetter(_helper, _type) { }

void JavaOptionsSetter::set(const bool state)
{
    switch (type)
    {
        case SHOWPRIVATE:
            reinterpret_cast<JavaOptionsHelper *>(&helper)->setShowPrivate(state);
            break;
        case ATTACHMODULE:
            reinterpret_cast<JavaOptionsHelper *>(&helper)->setAttachModule(state);
            break;
        default:
            ScilabOptionsSetter::set(state);
    }
}

bool JavaOptionsSetter::get() const
{
    switch (type)
    {
        case SHOWPRIVATE:
            return reinterpret_cast<JavaOptionsHelper *>(&helper)->getShowPrivate();
        case ATTACHMODULE:
            return reinterpret_cast<JavaOptionsHelper *>(&helper)->getAttachModule();
        default:
            return ScilabOptionsSetter::get();
    }
}
}
