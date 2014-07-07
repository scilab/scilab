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

bool TList::invoke(typed_list & in, optional_list & opt, int _iRetCount, typed_list & out, ast::ConstVisitor & execFunc, const ast::CallExp & e)
{
    if (in.size() == 0)
    {
        out.push_back(this);
        return true;
    }
    else if (in.size() == 1)
    {
        InternalType * arg = in[0];
        std::vector<InternalType *> _out;
        if (arg->isDouble() || arg->isInt() || arg->isBool() || arg->isImplicitList() || arg->isColon() || arg->isDollar())
        {
            _out = List::extract(&in);
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
        }

        if (!_out.empty())
        {
            out.swap(_out);
            return true;
        }
    }

    Callable::ReturnValue ret;
    this->IncreaseRef();
    in.push_back(this);

    try
    {
        ret = Overload::call(L"%" + getShortTypeStr() + L"_e", in, 1, out, &execFunc);
    }
    catch (ast::ScilabError & se)
    {
        ret = Overload::call(L"%l_e", in, 1, out, &execFunc);
    }

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

std::vector<InternalType*> TList::extractStrings(const std::list<std::wstring>& _stFields)
{
    std::vector<InternalType*> Result;
    std::list<std::wstring>::const_iterator it;
    for (it = _stFields.begin() ; it != _stFields.end() ; it++)
    {
        if (exists(*it) == false)
        {
            return Result;
        }
    }

    for (it = _stFields.begin() ; it != _stFields.end() ; it++)
    {
        Result.push_back(getField(*it));
    }
    return Result;
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
** FIXME : Find a better indentation process
*/
bool TList::toString(std::wostringstream& ostr)
{
    wchar_t* wcsVarName = os_wcsdup(ostr.str().c_str());
    ostr.str(L"");

    if (getSize() == 0)
    {
        ostr << wcsVarName << L"()" << std::endl;
    }
    else if ((*m_plData)[0]->isString() &&
             (*m_plData)[0]->getAs<types::String>()->getSize() > 0 &&
             wcscmp((*m_plData)[0]->getAs<types::String>()->get(0), L"lss") == 0)
    {
        int iPosition = 1;
        const wchar_t * wcsDesc[7] = {L"  (state-space system:)", L"= A matrix =", L"= B matrix =", L"= C matrix =", L"= D matrix =", L"= X0 (initial state) =", L"= Time domain ="};
        std::vector<InternalType *>::iterator itValues;
        for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues, ++iPosition)
        {
            ostr << "     " << wcsVarName << L"(" << iPosition << L") " << wcsDesc[iPosition - 1] << std::endl;
            //maange lines
            bool bFinish = (*itValues)->toString(ostr);
            ostr << std::endl;
        }
    }
    else
    {
        int iPosition = 1;
        std::vector<InternalType *>::iterator itValues;
        for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues, ++iPosition)
        {
            ostr << "     " << wcsVarName << L"(" << iPosition << L")" << std::endl;
            //maange lines
            bool bFinish = (*itValues)->toString(ostr);
            ostr << std::endl;
        }
    }

    free(wcsVarName);
    return true;
}
} // end namespace types
