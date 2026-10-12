/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <cstring>
#include <sstream>
#include "exp.hxx"
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "callable.hxx"
#include "polynom.hxx"
#include "overload.hxx"
#include "configvariable.hxx"
#include "types_tools.hxx"
#include "scilabWrite.hxx"

#ifndef NDEBUG
#include "inspector.hxx"
#endif

extern "C"
{
#include "os_string.h"
}

namespace types
{
/**
** Constructor & Destructor (public)
*/
TList::TList() : List()
{
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

TList::~TList()
{
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

bool TList::getMemory(long long* _piSize, long long* _piSizePlusType)
{
    *_piSize = 0;
    *_piSizePlusType = 0;
    for (auto pData : *m_plData)
    {
        long long piS, piSPT;
        if (pData->getMemory(&piS, &piSPT))
        {
            *_piSize += piS;
            *_piSizePlusType += piSPT;
        }
    }

    *_piSizePlusType += sizeof(TList);
    return true;
}

/**
** Clone
** Create a new List and Copy all values.
*/
TList* TList::clone()
{
    return new TList(this);
}

bool TList::exists(const std::wstring& _sKey)
{
    if (getSize() < 1)
    {
        return false;
    }

    String* pS = getFieldNames();

    //first field is the tlist type
    for (int i = 1 ; i < pS->getSize() ; i++)
    {
        if (wcscmp(pS->get(i), _sKey.c_str()) == 0)
        {
            return true;
        }
    }
    return false;
}

bool TList::invoke(typed_list & in, optional_list & /*opt*/, int _iRetCount, typed_list & out, const ast::Exp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
        return true;
    }
    else if (in.size() == 1)
    {
        InternalType * arg = in[0];
        InternalType * _out = NULL;
        if (arg->isDouble() || arg->isInt() || arg->isBool() || arg->isImplicitList() || arg->isColon() || arg->isDollar())
        {
            _out = List::extract(&in);
            if (_out == NULL)
            {
                // invalid index
                return false;
            }

            List* pList = _out->getAs<types::List>();
            for (int i = 0; i < pList->getSize(); i++)
            {
                out.push_back(pList->get(i));
            }

            delete pList;
        }
        else if (arg->isString())
        {
            std::list<std::wstring> stFields;
            String * pString = arg->getAs<types::String>();
            for (int i = 0; i < pString->getSize(); ++i)
            {
                stFields.push_back(pString->get(i));
            }

            _out = extractStrings(stFields);
            if (_out == NULL)
            {
                // invalid index
                return false;
            }

            List* pList = _out->getAs<types::List>();
            for (int i = 0; i < pList->getSize(); i++)
            {
                out.push_back(pList->get(i));
            }

            delete pList;
        }
        else if (arg->isPoly())
        {
            Polynom* pPoly = arg->getAs<Polynom>();
            SinglePoly* pSinglePoly = pPoly->get(0);

            double dblParse = -1 * pSinglePoly->get(0);
            int iSize = getSize();

            if (pSinglePoly->getRank() < 2 && dblParse >= 0 && dblParse < (double)iSize)
            {
                out.push_back(get(iSize - 1 - dblParse));
            }
            else
            {
                return false;
            }
        }

        if (out.empty() == false)
        {
            return true;
        }
    }
    Callable::ReturnValue ret;
    // Overload of extraction need
    // the tlist from where we extract
    this->IncreaseRef();
    in.push_back(this);

    std::wstring wstrFuncName = L"%" + getShortTypeStr() + L"_e";

    ret = Overload::call(wstrFuncName, in, _iRetCount, out, false, false, e.getLocation());
    if(ret == types::Callable::OK_NoResult)
    {
        // overload not defined, try with the short name.
        // to compatibility with scilab 5 code.
        // tlist/mlist name are truncated to 8 first character
        std::wstring stType = getShortTypeStr();
        wstrFuncName = L"%" + stType.substr(0, 8) + L"_e";
        ret = Overload::call(wstrFuncName, in, _iRetCount, out, false, false, e.getLocation());
    }

    if(ret == types::Callable::OK_NoResult)
    {
        // last try that will throw an error if it not exists
        wstrFuncName = L"%l_e";
        ret = Overload::call(wstrFuncName, in, _iRetCount, out, false, true, e.getLocation());
    }

    // Remove this from "in" for keep "in" unchanged.
    this->DecreaseRef();
    in.pop_back();

    if (ret == Callable::Error)
    {
        throw ast::InternalError(ConfigVariable::getLastErrorMessage(), ConfigVariable::getLastErrorNumber(), e.getLocation());
    }

    // An extraction have to return something
    if(out.empty())
    {
        wchar_t wcstrError[512];
        os_swprintf(wcstrError, 512, _W("%ls: Extraction must have at least one output.\n").c_str(), wstrFuncName.c_str());
        throw ast::InternalError(wcstrError, 999, e.getLocation());
    }

    return true;
}

bool TList::extract(const std::wstring & name, InternalType *& out)
{
    if (exists(name))
    {
        out = getField(name);
        return true;
    }

    return false;
}

InternalType* TList::getField(const std::wstring& _sKey)
{
    return List::get(getIndexFromString(_sKey));
}

int TList::getIndexFromString(const std::wstring& _sKey)
{
    if (getSize() < 1)
    {
        return -1;
    }

    String* pS = getFieldNames();
    //first field is the tlist type
    for (int i = 1 ; i < pS->getSize() ; i++)
    {
        if (wcscmp(pS->get(i), _sKey.c_str()) == 0)
        {
            return i;
        }
    }
    return -1;
}

InternalType* TList::extractStrings(const std::list<std::wstring>& _stFields)
{
    int i = 0;
    List* pLResult = new List();
    std::list<std::wstring>::const_iterator it;
    for (it = _stFields.begin() ; it != _stFields.end() ; it++)
    {
        if (exists(*it) == false)
        {
            return pLResult;
        }
    }

    for (it = _stFields.begin() ; it != _stFields.end() ; it++, i++)
    {
        InternalType* pIT = getField(*it);
        if (pIT == NULL)
        {
            delete pLResult;
            return NULL;
        }

        pLResult->set(i, pIT);
    }

    return pLResult;
}

std::wstring TList::getTypeStr() const
{
    if (getSize() < 1)
    {
        return L"";
    }

    types::InternalType* pIT = (*m_plData)[0];
    if (pIT->isString() == false)
    {
        // malformed tlist without field names (e.g. built through api_scilab)
        return L"tlist";
    }

    return pIT->getAs<types::String>()->get(0);
}

std::wstring TList::getShortTypeStr() const
{
    return getTypeStr();
}

TList* TList::set(const std::wstring& _sKey, InternalType* _pIT)
{
    return List::set(getIndexFromString(_sKey), _pIT)->getAs<TList>();
}

TList* TList::set(const int _iIndex, InternalType* _pIT)
{
    return List::set(_iIndex, _pIT)->getAs<TList>();
}

String* TList::getFieldNames() const
{
    return (*m_plData)[0]->getAs<types::String>();
}

/**
** toString to display TLists
*/
bool TList::toString(std::wostringstream& ostr)
{
    //call overload %type_p if exists
    //fallthrough: call overload %l_p if exists
    types::typed_list in;
    types::typed_list out;

    IncreaseRef();
    in.push_back(this);
    types::Function::ReturnValue ret = Overload::generateNameAndCall(L"p", in, 0, out, false, false);
    if (ret == types::Callable::OK_NoResult)
    {
        std::wstring wstrFuncName = L"%l_p";
        ret = Overload::call(wstrFuncName, in, 1, out, false, false);
    }
    switch (ret) {
        case Function::OK_NoResult:
            // unresolved function, fallback to a basic display
            break;
        case Function::Error:
            ConfigVariable::setError();
            // fallthrough
        case Function::OK:
            ostr.str(L"");
            DecreaseRef();
            return true;
    };

    DecreaseRef();

    // call normal toString
    return List::toString(ostr);
}
} // end namespace types
