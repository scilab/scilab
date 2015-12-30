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
#include "configvariable.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include <wchar.h>
#include "os_string.h"
}

namespace types
{
Library::Library(const std::string& _path) :
    m_path(_path)
{
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

Library::~Library()
{
    //delete all macrofile*
    for (auto macro : m_macros)
    {
        MacroFile* pMacro = macro.second;
        pMacro->DecreaseRef();
        if (pMacro->isDeletable())
        {
            delete pMacro;
        }
    }

    m_macros.clear();
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

bool Library::toString(std::ostringstream& ostr)
{
    char output[1024] = {0};
    os_sprintf(output, _("Functions files location : %s.\n"), m_path.c_str());

    ostr << output << std::endl;

    size_t iLineLen = (size_t)ConfigVariable::getConsoleWidth();

    size_t iCurrentLen = 0;
    for (auto macro : m_macros)
    {
        if (iCurrentLen + macro.first.length() + 2 > iLineLen)
        {
            ostr << std::endl;
            iCurrentLen = 0;
        }
        ostr << macro.first << "  ";
        iCurrentLen += macro.first.length() + 2;
    }

    ostr << std::endl;

    return true;
}

Library* Library::clone()
{
    IncreaseRef();
    return this;
}

bool Library::extract(const std::string & name, InternalType *& out)
{
    out = get(name);
    if (out == NULL)
    {
        char szError[bsiz];
        os_sprintf(szError, _("Unknown field : %ls.\n"), name.c_str());
        throw std::string(szError);
    }

    return true;
}

void Library::add(const std::string& _stName, MacroFile* _macro)
{
    _macro->IncreaseRef();
    m_macros[_stName] = _macro;
}

MacroFile* Library::get(const std::string& _stName)
{
    MacroMap::iterator it = m_macros.find(_stName);
    if (it != m_macros.end())
    {
        return it->second;
    }
    return NULL;
}

int Library::getMacrosName(std::list<std::string>& lst)
{
    for (auto macro : m_macros)
    {
        lst.push_back(macro.first);
    }

    return static_cast<int>(lst.size());
}

std::string Library::getPath()
{
    return m_path;
}
}
