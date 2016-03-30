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


#include "ScilabOptionsSetter.hxx"
#include "OptionsHelper.hxx"

namespace org_modules_external_objects
{

const unsigned int ScilabOptionsSetter::METHODOFCONV = 0;
const unsigned int ScilabOptionsSetter::ALLOWRELOAD = 1;
const unsigned int ScilabOptionsSetter::AUTOUNWRAP = 2;
const unsigned int ScilabOptionsSetter::USECOPY = 3;
const unsigned int ScilabOptionsSetter::USESCILABINDEX = 4;

void ScilabOptionsSetter::set(const bool state)
{
    switch (type)
    {
        case METHODOFCONV:
            helper.setMethodOfConv(state);
            break;
        case ALLOWRELOAD:
            helper.setAllowReload(state);
            break;
        case AUTOUNWRAP:
            helper.setAutoUnwrap(state);
            break;
        case USECOPY:
            helper.setUseCopy(state);
            break;
        case USESCILABINDEX:
            helper.setUseScilabIndex(state);
            break;
    }
}

bool ScilabOptionsSetter::get() const
{
    switch (type)
    {
        case METHODOFCONV:
            return helper.getMethodOfConv();
        case ALLOWRELOAD:
            return helper.getAllowReload();
        case AUTOUNWRAP:
            return helper.getAutoUnwrap();
        case USECOPY:
            return helper.getUseCopy();
        case USESCILABINDEX:
            return helper.getUseScilabIndex();
    }

    return false;
}
}
