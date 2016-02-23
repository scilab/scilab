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
#include "bool.hxx"

extern "C"
{
#include "api_scilab.h"
}

#include "api_internal_error.hxx"

/* Scilab 6 API*/
scilabVar API_PROTO(createBooleanMatrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createBooleanMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createBooleanMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Bool* b = new types::Bool(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (b == nullptr)
    {
        scilab_setInternalError(env, L"createBooleanMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)b;
}

scilabVar API_PROTO(createBooleanMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};

#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createBooleanMatrix2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Bool* b = new types::Bool(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (b == nullptr)
    {
        scilab_setInternalError(env, L"createBooleanMatrix2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)b;
}

scilabVar API_PROTO(createBoolean)(scilabEnv env, int val)
{
    types::Bool* b = new types::Bool(val);
#ifdef __API_SCILAB_SAFE__
    if (b == nullptr)
    {
        scilab_setInternalError(env, L"createBoolean", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)b;
}

scilabStatus API_PROTO(getBoolean)(scilabEnv env, scilabVar var, int* val)
{
    types::Bool* b = (types::Bool*)var;
#ifdef __API_SCILAB_SAFE__
    if (b->isBool() == false || b->isScalar() == false)
    {
        scilab_setInternalError(env, L"getBoolean", _W("var must be a scalar boolean variable"));
        return STATUS_ERROR;
    }
#endif

    *val = b->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(getBooleanArray)(scilabEnv env, scilabVar var, int** vals)
{
    types::Bool* b = (types::Bool*)var;
#ifdef __API_SCILAB_SAFE__
    if (b->isBool() == false)
    {
        scilab_setInternalError(env, L"getBooleanArray", _W("var must be a boolean variable"));
        return STATUS_ERROR;
    }
#endif

    *vals = b->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(setBooleanArray)(scilabEnv env, scilabVar var, const int* vals)
{
    types::Bool* b = (types::Bool*)var;
#ifdef __API_SCILAB_SAFE__
    if (b->isBool() == false)
    {
        scilab_setInternalError(env, L"setBooleanArray", _W("var must be a boolean variable"));
        return STATUS_ERROR;
    }
#endif

    bool bset = b->set(vals) != nullptr;
#ifdef __API_SCILAB_SAFE__
    if (bset == false)
    {
        scilab_setInternalError(env, L"setBooleanArray", _W("unable to set data"));
        return STATUS_ERROR;
    }
#endif
    return bset ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(setBoolean)(scilabEnv env, scilabVar var, int val)
{
    types::Bool* b = (types::Bool*)var;
#ifdef __API_SCILAB_SAFE__
    if (b->isBool() == false || b->isScalar() == false)
    {
        scilab_setInternalError(env, L"setBoolean", _W("var must be a scalar boolean variable"));
        return STATUS_ERROR;
    }
#endif
    b->get()[0] = val;
    return STATUS_OK;
}

