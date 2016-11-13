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
        scilab_setInternalError(env, "createStruct", _("memory allocation error"));
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
        scilab_setInternalError(env, "createStructMatrix", _("dims array cannot be NULL"));
        return nullptr;
    }

    for (int i = 0; i < dim; ++i)
    {
        if (dims[i] < 0)
        {
            scilab_setInternalError(env, "createStructMatrix", _("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Struct* s = new types::Struct(dim, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, "createStructMatrix", _("memory allocation error"));
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
            scilab_setInternalError(env, "createStructMatrix2d", _("dimensions cannot be negative"));
            return nullptr;
        }
    }
#endif
    types::Struct* s = new types::Struct(2, dims);
#ifdef __API_SCILAB_SAFE__
    if (s == nullptr)
    {
        scilab_setInternalError(env, "createStructMatrix2d", _("memory allocation error"));
        return nullptr;
    }
#endif
    return (scilabVar)s;
}

/*fields*/
scilabStatus API_PROTO(addFields)(scilabEnv env, scilabVar var, int count, const char** fields)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "addFields", _("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif

    for (int i = 0; i < count; ++i)
    {
        s->addField(fields[i]);
    }

    return STATUS_OK;
}

scilabStatus API_PROTO(addField)(scilabEnv env, scilabVar var, const char* field)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "addField", _("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    s->addField(field);
    return STATUS_OK;
}

int API_PROTO(getFields)(scilabEnv env, scilabVar var, char* const**  fields)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "getFields", _("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    types::String* str = s->getFieldNames();
    *fields = str->get();
    return str->getSize();
}

/*data*/
scilabVar API_PROTO(getStructMatrixData)(scilabEnv env, scilabVar var, const char* field, const int* index)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "getStructMatrixData", _("var must be a struct variable"));
        return nullptr;
    }
#endif

    types::SingleStruct* ss = s->get(s->getIndex(index));
    return (scilabVar)ss->get(field);
}
scilabVar API_PROTO(getStructMatrix2dData)(scilabEnv env, scilabVar var, const char* field, int row, int col)
{
    int index[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "getStructMatrix2dData", _("var must be a struct variable"));
        return nullptr;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(index));
    return (scilabVar)ss->get(field);
}

scilabStatus API_PROTO(setStructMatrixData)(scilabEnv env, scilabVar var, const char* field, const int* index, scilabVar data)
{
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "setStructMatrixData", _("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(index));
    return ss->set(field, (types::InternalType*)data) ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(setStructMatrix2dData)(scilabEnv env, scilabVar var, const char* field, int row, int col, scilabVar data)
{
    int index[2] = {row, col};
    types::Struct* s = (types::Struct*)var;
#ifdef __API_SCILAB_SAFE__
    if (s->isStruct() == false)
    {
        scilab_setInternalError(env, "setStructMatrix2dData", _("var must be a struct variable"));
        return STATUS_ERROR;
    }
#endif
    types::SingleStruct* ss = s->get(s->getIndex(index));
    return ss->set(field, (types::InternalType*)data) ? STATUS_OK : STATUS_ERROR;
}

