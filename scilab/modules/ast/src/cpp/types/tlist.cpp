/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <cstring>
#include <sstream>
#include "string.hxx"
#include "list.hxx"
#include "tlist.hxx"
#include "listundefined.hxx"
#include "callable.hxx"
#include "overload.hxx"
#include "execvisitor.hxx"

#ifndef NDEBUG
#include "inspector.hxx"
#endif

extern "C"
{
#include "os_wcsdup.h"
}

namespace types
{
/**
** Constructor & Destructor (public)
*/
TList::TList() : List()
{
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

TList::~TList()
{
#ifndef NDEBUG
    //Inspector::removeItem(this);
#endif
}

/**
** Clone
** Create a new List and Copy all values.
*/
InternalType* TList::clone()
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

bool TList::invoke(typed_list & in, optional_list & /*opt*/, int /*_iRetCount*/, typed_list & out, ast::ConstVisitor & execFunc, const ast::CallExp & /*e*/)
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

    try
    {
        ret = Overload::call(L"%" + getShortTypeStr() + L"_e", in, 1, out, &execFunc);
    }
    catch (ast::ScilabError & /*se*/)
    {
        ret = Overload::call(L"%l_e", in, 1, out, &execFunc);
    }

    // Remove this from "in" for keep "in" unchanged.
    this->DecreaseRef();
    in.pop_back();

    if (ret == Callable::Error)
    {
        throw ast::ScilabError();
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

std::wstring TList::getTypeStr()
{
    if (getSize() < 1)
    {
        return L"";
    }

    return getFieldNames()->get(0);
}

std::wstring TList::getShortTypeStr()
{
    return getTypeStr();
}

bool TList::set(const std::wstring& _sKey, InternalType* _pIT)
{
    return List::set(getIndexFromString(_sKey), _pIT);
}

bool TList::set(const int _iIndex, InternalType* _pIT)
{
    return List::set(_iIndex, _pIT);
}

String* TList::getFieldNames()
{
    return (*m_plData)[0]->getAs<types::String>();
}

/**
** toString to display TLists
*/
bool TList::toString(std::wostringstream& ostr)
{
    //call overload %type_p if exists
    types::typed_list in;
    types::typed_list out;
    ast::ExecVisitor* exec = new ast::ExecVisitor();

    IncreaseRef();
    in.push_back(this);

    try
    {
        if (Overload::generateNameAndCall(L"p", in, 1, out, exec) == Function::OK)
        {
            ostr.str(L"");
            DecreaseRef();
            delete exec;
            return true;
        }
    }
    catch (ast::ScilabError /* &e */)
    {
        // avoid error message about undefined overload %type_p
    }

    DecreaseRef();
    delete exec;

    // special case for lss
    if (getSize() != 0 &&
            (*m_plData)[0]->isString() &&
            (*m_plData)[0]->getAs<types::String>()->getSize() > 0 &&
            wcscmp((*m_plData)[0]->getAs<types::String>()->get(0), L"lss") == 0)
    {
        wchar_t* wcsVarName = os_wcsdup(ostr.str().c_str());
        int iPosition = 1;
        const wchar_t * wcsDesc[7] = {L"  (state-space system:)", L"= A matrix =", L"= B matrix =", L"= C matrix =", L"= D matrix =", L"= X0 (initial state) =", L"= Time domain ="};
        std::vector<InternalType *>::iterator itValues;
        for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues, ++iPosition)
        {
            std::wostringstream nextVarName;
            ostr.str(L"");
            nextVarName << " " << wcsVarName << L"(" << iPosition << L")";
            ostr << std::endl << nextVarName.str() << wcsDesc[iPosition - 1] << std::endl << std::endl;
            scilabWriteW(ostr.str().c_str());
            VariableToString(*itValues, nextVarName.str().c_str());
        }

        free(wcsVarName);
        return true;
    }

    // call normal toString
    return List::toString(ostr);
}
} // end namespace types
