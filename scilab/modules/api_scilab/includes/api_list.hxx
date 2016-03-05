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

#ifdef _MSC_VER
#pragma warning( disable : 4275 )
#endif

#ifndef __API_LIST_HXX__
#define __API_LIST_HXX__

#include "list.hxx"
#include "tlist.hxx"
#include "mlist.hxx"
#include "api_string.hxx"

namespace api_scilab
{
class API_SCILAB_IMPEXP List : public Variable
{
protected :
    types::List* data;
    //internal constructor for existing variable
    List(types::List& _p)
    {
        data = &_p;
        created = false;
    }

public :
    List();

    virtual ~List()
    {
        if (created)
        {
            delete data;
        }
    };
    void* getReturnVariable()
    {
        return (void*)data;
    }
    virtual types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabList;
    }
    static List* getAsList(types::InternalType* _pIT);
    virtual int getSize();

    virtual Variable* get(const int _iIndex);
    virtual bool set(const int _iIndex, Variable* _pV);

};

class API_SCILAB_IMPEXP TList : public List
{
private:
    TList(types::TList& _p)
    {
        data = &_p;
        created = false;
    }
public :
    TList();

    virtual types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabTList;
    }
    static TList* getAsTList(types::InternalType* _pIT);

    bool exists(const std::wstring& _sKey);
    Variable* getField(const std::wstring& _sKey);
    int getIndexFromString(const std::wstring& _sKey);
    bool set(const std::wstring& _sKey, Variable* _pIT);
    String* getFieldNames();
};

class API_SCILAB_IMPEXP MList : public TList
{
private:
    MList(types::MList& _p)
    {
        data = &_p;
        created = false;
    }
public :
    MList();

    virtual types::InternalType::ScilabType getType()
    {
        return types::InternalType::ScilabMList;
    }
    static MList* getAsMList(types::InternalType* _pIT);
};
}

#endif /* !__API_LIST_HXX__ */