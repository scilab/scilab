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
#include "string.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createStringMatrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createStringMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createStringMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::String* s = new types::String(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStringMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createStringMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createStringMatrix2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::String* s = new types::String(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStringMatrix2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createString)(scilabEnv env, const wchar_t* val)
{
    types::String* s = new types::String(val);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createString", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabStatus API_PROTO(getStringArray)(scilabEnv env, scilabVar var, wchar_t*** strs)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false)
    {
        scilab_setInternalError(env, L"getStringArray", _W("var must be a string variable"));
        return STATUS_ERROR;
    }
#endif
    *strs = s->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(getString)(scilabEnv env, scilabVar var, wchar_t** str)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false || s->isScalar() == false)
    {
        scilab_setInternalError(env, L"getString", _W("var must be a scalar string variable"));
        return STATUS_ERROR;
    }
#endif
    *str = s->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(setStringArray)(scilabEnv env, scilabVar var, const wchar_t* const* strs)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false)
    {
        scilab_setInternalError(env, L"setStringArray", _W("var must be a string variable"));
        return STATUS_ERROR;
    }
#endif
    s->set(strs);
    return STATUS_OK;
}

scilabStatus API_PROTO(setString)(scilabEnv env, scilabVar var, wchar_t* str)
{
    types::String* s = (types::String*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isString() == false || s->isScalar() == false)
    {
        scilab_setInternalError(env, L"setString", _W("var must be a scalar string variable"));
        return STATUS_ERROR;
    }
#endif
    s->set(0, str);
    return STATUS_OK;
}
