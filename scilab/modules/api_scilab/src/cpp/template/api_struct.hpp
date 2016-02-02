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
#include "struct.hxx"

extern "C"
{
#include "api_scilab.h"
}

scilabVar API_PROTO(createStruct)(scilabEnv env)
{
    types::Struct* s = new types::Struct(1, 1);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStruct", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createStructMatrix)(scilabEnv env, int dim, const int* dims)
{
#ifdef __API_SCILAB_SAFE__
    if (dims == nullptr)
    {
        scilab_setInternalError(env, L"createStructMatrix", _W("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createStructMatrix", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Struct* s = new types::Struct(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStructMatrix", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

scilabVar API_PROTO(createStructMatrix2d)(scilabEnv env, int row, int col)
{
    int dims[2] = {row, col};
#ifdef __API_SCILAB_SAFE__
    for (int i = 0; i < 2; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, L"createStructMatrix2d", _W("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Struct* s = new types::Struct(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, L"createStructMatrix2d", _W("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

/*fields*/
scilabStatus API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, const wchar_t** fields)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"addFields", _W("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif

    for (int i = 0; i < count; ++i)
    {
        s->addField(fields[i]);
    }

    return STATUS_OK;
}

scilabStatus API_PROTO(addField)(scilabEnv env, scilabVar var, const wchar_t* field)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"addField", _W("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    s->addField(field);
    return STATUS_OK;
}

int API_PROTO(getFields)(scilabEnv env, scilabVar var, wchar_t***  fields)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getFields", _W("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    types::String* str = s->getFieldNames();
    *fields = str->get();
    return str->getSize();
}

/*data*/
scilabVar API_PROTO(getStructMatrixData)(scilabEnv env, scilabVar var, const wchar_t* field, const int* index)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getStructMatrixData", _W("var must be a struct variable"));
        return nullptr;
    }
#endif

    types::SingleStruct* ss = s->get(s->getIndex(index));
    return (scilabVar)ss->get(field);
}
scilabVar API_PROTO(getStructMatrix2dData)(scilabEnv env, scilabVar var, const wchar_t* field, int row, int col)
{
    int index[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"getStructMatrix2dData", _W("var must be a struct variable"));
        return nullptr;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(index));
    return (scilabVar)ss->get(field);
}

scilabStatus API_PROTO(setStructMatrixData)(scilabEnv env, scilabVar var, const wchar_t* field, const int* index, scilabVar data)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"setStructMatrixData", _W("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(index));
    return ss->set(field, (types::InternalType*)data) ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(setStructMatrix2dData)(scilabEnv env, scilabVar var, const wchar_t* field, int row, int col, scilabVar data)
{
    int index[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, L"setStructMatrix2dData", _W("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(index));
    return ss->set(field, (types::InternalType*)data) ? STATUS_OK : STATUS_ERROR;
}

