/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <sstream>
#include "macrofile.hxx"
#include "library.hxx"
#include "localization.hxx"
#include "configvariable.hxx"

extern "C"
{
#include <wchar.h>
#include "os_swprintf.h"
}

namespace types
{
Library::Library(const std::wstring& _wstPath) :
    m_wstPath(_wstPath)
{
}

Library::~Library()
{
    //delete all macrofile*
    for (MacroMap::iterator it = m_macros.begin(); it != m_macros.end() ; ++it)
    {
        MacroFile* pMacro = it->second;
        pMacro->DecreaseRef();
        if (pMacro->isDeletable())
        {
            delete pMacro;
        }
    }

    m_macros.clear();
}

bool Library::toString(std::wostringstream& ostr)
{
    wchar_t output[1024] = {0};
    os_swprintf(output, 1024, _W("Functions files location : %s.\n").c_str(), m_wstPath.c_str());

    ostr << output << std::endl;

    size_t iLineLen = (size_t)ConfigVariable::getConsoleWidth();

    size_t iCurrentLen = 0;
    MacroMap::iterator it = m_macros.begin();
    for (int i = 0; it != m_macros.end() ; ++it, ++i)
    {
        if (iCurrentLen + it->first.length() + 2 > iLineLen)
        {
            ostr << std::endl;
            iCurrentLen = 0;
        }
        ostr << it->first << "  ";
        iCurrentLen += it->first.length() + 2;
    }

    ostr << std::endl;
    return true;
}

InternalType* Library::clone()
{
    IncreaseRef();
    return this;
}

bool Library::extract(const std::wstring & name, InternalType *& out)
{
    out = get(name);
    if (out == NULL)
    {
        wchar_t szError[bsiz];
        os_swprintf(szError, bsiz, _W("Unknown field : %ls.\n").c_str(), name.c_str());

        throw std::wstring(szError);
    }

    return true;
}

void Library::add(const std::wstring& _wstName, MacroFile* _macro)
{
    _macro->IncreaseRef();
    m_macros[_wstName] = _macro;
}

MacroFile* Library::get(const std::wstring& _wstName)
{
    MacroMap::iterator it = m_macros.find(_wstName);
    if (it != m_macros.end())
    {
        return it->second;
    }
    return NULL;
}

std::list<std::wstring>* Library::getMacrosName()
{
    std::list<std::wstring>* pOut = new std::list<std::wstring>;
    MacroMap::iterator it = m_macros.begin();
    for (; it != m_macros.end() ; ++it)
    {
        pOut->push_back(it->first);
    }

    return pOut;
}
}
