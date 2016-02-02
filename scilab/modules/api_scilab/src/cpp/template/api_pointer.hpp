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
*/

#include "gatewaystruct.hxx"
#include "pointer.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createPointer)(scilabEnv env, void* val)
{
    return (scilabVar)new types::Pointer(val);
}

scilabStatus API_PROTO(getPointer)(scilabEnv env, scilabVar var, void** val)
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

scilabStatus API_PROTO(setPointer)(scilabEnv env, scilabVar var, void* val)
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
