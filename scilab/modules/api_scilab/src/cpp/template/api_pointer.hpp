/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#include "gatewaystruct.hxx"
#include "pointer.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createPointer)(scilabEnv env, const void* val)
{
    return (scilabVar)new types::Pointer(val);
}

scilabStatus API_PROTO(getPointer)(scilabEnv env, scilabVar var, void const** val)
{
    types::Pointer* p = (types::Pointer*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPointer() == false)
    {
        scilab_setInternalError(env, L"getPointer", _W("var must be a pointer variable"));
        return STATUS_ERROR;
    }
#endif
    *val = p->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(setPointer)(scilabEnv env, scilabVar var, const void* val)
{
    types::Pointer* p = (types::Pointer*)var;
#ifdef __API_SCILAB_SAFE__
    if (p->isPointer() == false)
    {
        scilab_setInternalError(env, L"setPointer", _W("var must be a pointer variable"));
        return STATUS_ERROR;
    }
#endif
    p->set(val);
    return STATUS_OK;
}
