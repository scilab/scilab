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

#include "JavaOptionsHelper.hxx"
#include "JavaOptionsSetter.hxx"

using namespace org_modules_external_objects;

namespace org_scilab_modules_external_objects_java
{
const unsigned int JavaOptionsSetter::SHOWPRIVATE = LAST_OPTION_INDEX + 2;

JavaOptionsSetter::JavaOptionsSetter(JavaOptionsHelper & _helper, unsigned int _type) : ScilabOptionsSetter(_helper, _type) { }

JavaOptionsSetter::JavaOptionsSetter(const JavaOptionsSetter & setter) : ScilabOptionsSetter(setter) { }

void JavaOptionsSetter::set(const bool state)
{
    switch (type)
    {
        case SHOWPRIVATE:
            reinterpret_cast<JavaOptionsHelper *>(&helper)->setShowPrivate(state);
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
        default:
            return ScilabOptionsSetter::get();
    }
}
}
