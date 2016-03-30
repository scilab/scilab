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
#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"

extern "C"
{
#include "api_scilab.h"
}

int API_PROTO(isDefined)(scilabEnv env, scilabVar var, int index)
{
    return scilab_isUndefined(env, var, index) ? 0 : 1;
}

int API_PROTO(isUndefined)(scilabEnv env, scilabVar var, int index)
{
    types::List* l = (types::List*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"isUndefined", _W("var must be a list variable"));
        return STATUS_ERROR;
    }
#endif
    types::InternalType* item = l->get(index);
    if (item->isListUndefined())
    {
        return 1;
    }

    return 0;
}

scilabVar API_PROTO(createList)(scilabEnv env)
{
    return (scilabVar)new types::List();
}

scilabVar API_PROTO(createTList)(scilabEnv env, const wchar_t* type)
{
    types::TList* t = new types::TList();
    t->append(new types::String(type));
    return (scilabVar)t;
}

scilabVar API_PROTO(createMList)(scilabEnv env, const wchar_t* type)
{
    types::MList* m = new types::MList();
    m->append(new types::String(type));
    return (scilabVar)m;
}

scilabVar API_PROTO(getListItem)(scilabEnv env, scilabVar var, int index)
{
    types::List* l = (types::List*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"getListItem", _W("var must be a list variable"));
        return nullptr;
    }
#endif

    if (index < 0 || index >= l->getSize())
    {
#ifdef __API_SCILAB_SAFE__
        scilab_setInternalError(env, L"getListItem", _W("index out of bounds"));
#endif
        return nullptr;
    }

    return (scilabVar)l->get(index);
}

scilabStatus API_PROTO(setListItem)(scilabEnv env, scilabVar var, int index, scilabVar val)
{
    types::List* l = (types::List*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"setListItem", _W("var must be a list variable"));
        return STATUS_ERROR;
    }
#endif

    bool ret = l->set(index, (types::InternalType*)val) != nullptr;
    return ret ? STATUS_OK : STATUS_ERROR;
}

int API_PROTO(getTListFieldNames)(scilabEnv env, scilabVar var, wchar_t*** fieldnames)
{
    types::TList* l = (types::TList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"getTListFieldNames", _W("var must be a tlist variable"));
        return 0;
    }
#endif

    types::String* names = l->getFieldNames();
    *fieldnames = names->get();
    return names->getSize();
}

scilabVar API_PROTO(getTListField)(scilabEnv env, scilabVar var, const wchar_t* field)
{
    types::TList* l = (types::TList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"getTListField", _W("var must be a tlist variable"));
        return nullptr;
    }
#endif

    return (scilabVar)l->getField(field);

}

scilabStatus API_PROTO(setTListField)(scilabEnv env, scilabVar var, const wchar_t* field, scilabVar val)
{
    types::TList* l = (types::TList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"setTListField", _W("var must be a tlist variable"));
        return STATUS_ERROR;
    }
#endif

    if (l->getIndexFromString(field) < 0)
    {
        //field does not, append it at last position
        types::String* fields = l->getFieldNames();

        fields->resize(1, fields->getSize() + 1);
        fields->set(fields->getSize() - 1, field);
    }

    bool ret = l->set(field, (types::InternalType*)val) != nullptr;
    return ret ? STATUS_OK : STATUS_ERROR;
}

int API_PROTO(getMListFieldNames)(scilabEnv env, scilabVar var, wchar_t*** fieldnames)
{
    types::MList* l = (types::MList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isTList() == false)
    {
        scilab_setInternalError(env, L"getMListFieldNames", _W("var must be a tlist variable"));
        return 0;
    }
#endif

    types::String* names = l->getFieldNames();
    *fieldnames = names->get();
    return names->getSize();
}

scilabVar API_PROTO(getMListField)(scilabEnv env, scilabVar var, const wchar_t* field)
{
    types::MList* l = (types::MList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isMList() == false)
    {
        scilab_setInternalError(env, L"getMListField", _W("var must be a mlist variable"));
        return nullptr;
    }
#endif
    return (scilabVar)l->getField(field);
}

scilabStatus API_PROTO(setMListField)(scilabEnv env, scilabVar var, const wchar_t* field, scilabVar val)
{
    types::MList* l = (types::MList*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isMList() == false)
    {
        scilab_setInternalError(env, L"setMListField", _W("var must be a mlist variable"));
        return STATUS_ERROR;
    }
#endif

    if (l->getIndexFromString(field) < 0)
    {
        //field does not, append it at last position
        types::String* fields = l->getFieldNames();

        fields->resize(1, fields->getSize() + 1);
        fields->set(fields->getSize() - 1, field);
    }

    bool ret = l->set(field, (types::InternalType*)val) != nullptr;
    return ret ? STATUS_OK : STATUS_ERROR;
}

scilabStatus API_PROTO(appendToList)(scilabEnv env, scilabVar var, scilabVar val)
{
    types::List* l = (types::List*)var;
#ifdef __API_SCILAB_SAFE__
    if (l->isList() == false)
    {
        scilab_setInternalError(env, L"appendToList", _W("var must be a list variable"));
        return STATUS_ERROR;
    }
#endif
    l->append((types::InternalType*)val);
    return STATUS_OK;
}

