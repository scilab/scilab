/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
