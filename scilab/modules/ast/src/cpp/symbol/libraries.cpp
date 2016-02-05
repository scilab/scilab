/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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

#include <algorithm>
#include "libraries.hxx"

namespace symbol
{

void Library::put(types::Library* _pLib, int _iLevel)
{
    if (empty() || top()->m_iLevel < _iLevel)
    {
        //create a new level
        stack.push(new ScopedLibrary(_iLevel, _pLib));
    }
    else
    {
        //update current level
        types::Library* pLib = top()->m_pLib;
        if (pLib != _pLib)
        {
            // data is manage by variables.
            // So if this library have to be killed
            // this is alredy done by variables.
            //pLib->killMe();
            top()->m_pLib = _pLib;
        }
    }
}

types::MacroFile* Library::get(const Symbol& _keyMacro) const
{
    if (empty() == false)
    {
        return top()->getMacroFile(_keyMacro);
    }

    return nullptr;
}

int Library::getMacrosName(std::list<std::wstring>& lst)
{
    if (empty() == false)
    {
        top()->m_pLib->getMacrosName(lst);
    }

    return static_cast<int>(lst.size());
}

Library* Libraries::getOrCreate(const Symbol& _key)
{
    MapLibs::const_iterator it = libs.find(_key);
    if (it == libs.end())
    {
        //create an empty StackedValues
        Library* lib = new Library(_key);
        libs[_key] = lib;
        return lib;
    }

    return it->second;
}

int Libraries::getLevel(const Symbol& _key) const
{
    MapLibs::const_iterator it = libs.find(_key);
    if (it != libs.end())
    {
        if (!it->second->empty())
        {
            return it->second->top()->m_iLevel;
        }
    }
    else
    {
        for (auto i = libs.rbegin(), end = libs.rend(); i != end; ++i)
        {
            Library * lib = i->second;
            if (!lib->empty())
            {
                types::MacroFile * pMF = lib->get(_key);
                if (pMF)
                {
                    return lib->top()->m_iLevel;
                }
            }
        }
    }

    return SCOPE_ALL;
}

void Libraries::put(const Symbol& _keyLib, types::Library* _pLib, int _iLevel)
{
    Library* lib = getOrCreate(_keyLib);
    lib->put(_pLib, _iLevel);
}

bool Libraries::putInPreviousScope(const Symbol& _keyLib, types::Library* _pLib, int _iLevel)
{
    Library* lib = getOrCreate(_keyLib);

    if (lib->empty())
    {
        lib->put(_pLib, _iLevel);
    }
    else if (lib->top()->m_iLevel > _iLevel)
    {
        ScopedLibrary* pLib = lib->top();
        lib->pop();
        putInPreviousScope(_keyLib, _pLib, _iLevel);
        lib->put(pLib);
    }
    else
    {
        lib->put(_pLib, _iLevel);
    }

    return true;
}

types::InternalType* Libraries::get(const Symbol& _key, int _iLevel)
{
    //does _key is a lib name
    auto lib = libs.find(_key);
    if (lib != libs.end())
    {
        if (lib->second->empty() == false)
        {
            if (_iLevel == SCOPE_ALL || lib->second->top()->m_iLevel == _iLevel)
            {
                return lib->second->top()->m_pLib;
            }
        }
    }

    //does _key is a macro in a lib
    auto it = libs.rbegin();
    for (auto it = libs.rbegin(), itEnd = libs.rend(); it != itEnd; ++it)
    {
        Library* lib = it->second;
        if (it->second->empty() == false)
        {
            if (_iLevel == SCOPE_ALL || it->second->top()->m_iLevel == _iLevel)
            {
                types::MacroFile* pMF = it->second->get(_key);
                if (pMF)
                {
                    return (types::InternalType*)pMF;
                }
            }
        }
    }

    return NULL;
}

bool Libraries::remove(const Symbol& _key, int _iLevel)
{
    MapLibs::iterator it = libs.find(_key);
    if (it != libs.end())
    {
        if (it->second->empty() == false)
        {
            if (it->second->top()->m_iLevel == _iLevel)
            {
                ScopedLibrary * pSL = it->second->top();
                it->second->pop();
                delete pSL;
                return true;
            }
        }
    }

    return false;
}

int Libraries::getMacrosName(std::list<std::wstring>& lst)
{
    MapLibs::iterator it = libs.begin();
    MapLibs::iterator itEnd = libs.end();
    for (auto it : libs)
    {
        it.second->getMacrosName(lst);
    }

    return static_cast<int>(lst.size());
}

int Libraries::getVarsName(std::list<std::wstring>& lst)
{
    for (auto it : libs)
    {
        if (it.second->empty() == false)
        {
            lst.push_back(it.first.getName().c_str());
        }
    }

    return static_cast<int>(lst.size());
}

int Libraries::getVarsToVariableBrowser(std::list<Library*>& lst)
{
    for (auto lib : libs)
    {
        if (lib.second->empty() == false)
        {
            lst.push_back(lib.second);
        }
    }

    return static_cast<int>(lst.size());
}

void Libraries::clearAll()
{
    for (auto lib : libs)
    {
        while (!lib.second->empty())
        {
            ScopedLibrary * pSL = lib.second->top();
            types::InternalType * pIT = pSL->m_pLib;
            pIT->killMe();
            lib.second->pop();
            delete pSL;
        }

        delete lib.second;
    }
}

bool Libraries::getVarsNameForWho(std::list<std::wstring>* lstVarName, int* iVarLenMax, bool bSorted) const
{
    for (auto it = libs.begin(), itEnd = libs.end(); it != itEnd; ++it)
    {
        std::wstring wstrVarName(it->first.getName().c_str());
        if (lstVarName && it->second->empty() == false)
        {
            lstVarName->push_back(wstrVarName);
            *iVarLenMax = std::max(*iVarLenMax, (int)wstrVarName.size());
        }
    }

    if (bSorted)
    {
        if (lstVarName)
        {
            lstVarName->sort();
        }
    }

    return true;
}

int Libraries::whereis(std::list<std::wstring>& lst, const Symbol& _key)
{
    for (auto lib : libs)
    {
        if (lib.second->get(_key) != NULL)
        {
            lst.push_back(lib.first.getName());
        }
    }
    return static_cast<int>(lst.size());
}

int Libraries::librarieslist(std::list<std::wstring>& lst)
{
    for (auto lib : libs)
    {
        if (lib.second->empty() == false)
        {
            lst.push_back(lib.first.getName());
        }
    }

    return static_cast<int>(lst.size());
}

}
