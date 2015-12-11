/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
