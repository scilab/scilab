/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include "gatewaystruct.hxx"
extern "C"
{
#include "api_scilab.h"
}

void scilab_setError(scilabEnv env, const wchar_t* msg)
{
    ((types::GatewayCStruct*)env)->lasterror = msg;
}

//error
const wchar_t* scilab_lastError(scilabEnv env)
{
    return ((types::GatewayCStruct*)env)->lasterror.data();
}

void scilab_setInternalError(scilabEnv env, const std::wstring& name, const std::wstring& msg)
{
    std::wstring err(L"scilab_" + name + L": " + msg);
    scilab_setError(env, err.data());
}
