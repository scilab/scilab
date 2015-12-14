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
#include "graphichandle.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createHandleMatrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createHandleMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createHandleMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::GraphicHandle* h = new types::GraphicHandle(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (h == nullptr)
    {
        scilab_setInternalError(env, L"createHandleMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)h;
}

scilabVar API_PROTO(createHandleMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createHandleMatrix2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::GraphicHandle* h = new types::GraphicHandle(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (h == nullptr)
    {
        scilab_setInternalError(env, L"createHandleMatrix2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)h;
}

scilabVar API_PROTO(createHandle)(scilabEnv env)
{
    types::GraphicHandle* h = new types::GraphicHandle(-1);
#ifdef __API_SCILAB_SAFE__
    if (h == nullptr)
    {
        scilab_setInternalError(env, L"createHandle", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)h;
}

scilabStatus API_PROTO(getHandleArray)(scilabEnv env, scilabVar var, const long long** vals)
{
    types::GraphicHandle* h = (types::GraphicHandle*)var;
#ifdef __API_SCILAB_SAFE__
    if (h->isHandle() == false)
    {
        scilab_setInternalError(env, L"getHandleArray", _W("var must be a handle variable"));
        return STATUS_ERROR;
    }
#endif

    *vals = h->get();
    return STATUS_OK;
}

scilabStatus API_PROTO(setHandleArray)(scilabEnv env, scilabVar var, const long long* vals)
{
    types::GraphicHandle* h = (types::GraphicHandle*)var;
#ifdef __API_SCILAB_SAFE__
    if (h->isHandle() == false)
    {
        scilab_setInternalError(env, L"setHandleArray", _W("var must be a handle variable"));
        return STATUS_ERROR;
    }
#endif

    bool bset = h->set(vals);
#ifdef __API_SCILAB_SAFE__
    if (bset == false)
    {
        scilab_setInternalError(env, L"setHandleArray", _W("unable to set data"));
        return STATUS_ERROR;
    }
#endif
    return bset ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(getHandle)(scilabEnv env, scilabVar var, long long* vals)
{
    types::GraphicHandle* h = (types::GraphicHandle*)var;
#ifdef __API_SCILAB_SAFE__
    if (h->isHandle() == false || h->isScalar() == false)
    {
        scilab_setInternalError(env, L"getHandle", _W("var must be a scalar handle variable"));
        return STATUS_ERROR;
    }
#endif
    *vals = h->get()[0];
    return STATUS_OK;
}

scilabStatus API_PROTO(setHandle)(scilabEnv env, scilabVar var, long long val)
{
    types::GraphicHandle* h = (types::GraphicHandle*)var;
#ifdef __API_SCILAB_SAFE__
    if (h->isHandle() == false || h->isScalar() == false)
    {
        scilab_setInternalError(env, L"setHandle", _W("var must be a scalar handle variable"));
        return STATUS_ERROR;
    }
#endif
    h->get()[0] = val;
    return STATUS_OK;
}
