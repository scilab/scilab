/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#include "api_list.hxx"
#include "api_common.hxx"

namespace api_scilab
{
List::List()
{
    data = new types::List();
    created = true;
}

List* List::getAsList(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isList())
    {
        return new List(*_pIT->getAs<types::List>());
    }

    return NULL;
}

int List::getSize()
{
    if (data)
    {
        return data->getSize();
    }

    return 0;
}

Variable* List::get(const int _iIndex)
{
    if (data)
    {
        return getAsVariable(data->get(_iIndex));
    }

    return NULL;
}

bool List::set(const int _iIndex, Variable* _pV)
{
    if (data)
    {
        data->set(_iIndex, (types::InternalType*)_pV->getReturnVariable());
    }

    return false;
}

TList::TList()
{
    data = new types::TList();
    created = true;
}

TList* TList::getAsTList(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isTList())
    {
        return new TList(*_pIT->getAs<types::TList>());
    }

    return NULL;
}

bool TList::exists(const std::wstring& _sKey)
{
    if (data)
    {
        return ((types::TList*)data)->exists(_sKey);
    }

    return false;
}

Variable* TList::getField(const std::wstring& _sKey)
{
    if (data)
    {
        return getAsVariable(((types::TList*)data)->getField(_sKey));
    }

    return NULL;
}

int TList::getIndexFromString(const std::wstring& _sKey)
{
    if (data)
    {
        return ((types::TList*)data)->getIndexFromString(_sKey);
    }

    return -1;
}

bool TList::set(const std::wstring& _sKey, Variable* _pV)
{
    if (data)
    {
        return ((types::TList*)data)->set(_sKey, (types::InternalType*)_pV->getReturnVariable()) != nullptr;
    }

    return false;
}

String* TList::getFieldNames()
{
    if (data)
    {
        return getAsString(((types::TList*)data)->getFieldNames());
    }

    return NULL;
}


MList::MList()
{
    data = new types::MList();
    created = true;
}

MList* MList::getAsMList(types::InternalType* _pIT)
{
    if (_pIT && _pIT->isMList())
    {
        return new MList(*_pIT->getAs<types::MList>());
    }

    return NULL;
}
}