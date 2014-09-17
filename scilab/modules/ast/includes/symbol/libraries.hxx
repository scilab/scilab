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

#ifndef __LIBRARIES_HXX__
#define __LIBRARIES_HXX__

#include <stack>
#include "symbol.hxx"
#include "types.hxx"
#include "library.hxx"

namespace symbol
{
struct ScopedLibrary
{
    ScopedLibrary(int _iLevel, types::Library* _pLib) : m_iLevel(_iLevel), m_pLib(_pLib) {};

    types::MacroFile* getMacroFile(const Symbol& _key)
    {
        return m_pLib->get(_key.getName());
    }

    int m_iLevel;
    types::Library* m_pLib;
};

struct Library
{
    typedef std::stack<ScopedLibrary*> StackLib;

    Library(const Symbol& _name) : name(_name) {};

    void put(types::Library* _pLib, int _iLevel)
    {
        if (empty() || top()->m_iLevel < _iLevel)
        {
            //create a new level
            stack.push(new ScopedLibrary(_iLevel, _pLib));
            _pLib->IncreaseRef();
        }
        else
        {
            //update current level
            types::Library* pLib = top()->m_pLib;
            if (pLib != _pLib)
            {
                pLib->DecreaseRef();
                pLib->killMe();
                top()->m_pLib = _pLib;
                _pLib->IncreaseRef();
            }
        }
    }

    types::MacroFile* get(const Symbol& _keyMacro) const
    {
        if (empty() == false)
        {
            return top()->getMacroFile(_keyMacro);
        }

        return NULL;
    }

    std::list<std::wstring>* getMacrosName()
    {
        if (empty() == false)
        {
            return top()->m_pLib->getMacrosName();
        }

        return new std::list<std::wstring>();
    }

    bool empty() const
    {
        return stack.empty();
    }

    ScopedLibrary* top() const
    {
        return stack.top();
    }

    void pop()
    {
        stack.pop();
    }

private :
    StackLib stack;
    Symbol name;
    bool m_global;
};

struct Libraries
{
    typedef std::map<Symbol, Library*> MapLibs;

    Libraries() {};

    Library* getOrCreate(const Symbol& _key)
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

    void put(const Symbol& _keyLib, types::Library* _pLib, int _iLevel)
    {
        Library* lib = getOrCreate(_keyLib);
        lib->put(_pLib, _iLevel);
    }

    types::InternalType* get(const Symbol& _key, int _iLevel)
    {
        MapLibs::reverse_iterator it = libs.rbegin();
        for (; it != libs.rend() ; ++it)
        {
            if (it->second->empty() == false)
            {
                if (_iLevel == -1 || it->second->top()->m_iLevel == _iLevel)
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

    bool remove(const Symbol& _key, int _iLevel)
    {
        MapLibs::iterator it = libs.find(_key);
        if (it != libs.end())
        {
            if (it->second->empty() == false)
            {
                if (it->second->top()->m_iLevel == _iLevel)
                {
                    ScopedLibrary * pSL = it->second->top();
                    types::Library* pIT = pSL->m_pLib;
                    pIT->DecreaseRef();
                    pIT->killMe();
                    it->second->pop();
                    delete pSL;
                    return true;
                }
            }
        }

        return false;
    }

    std::list<std::wstring>* getMacrosName()
    {
        std::list<std::wstring>* names = new std::list<std::wstring>();
        MapLibs::iterator it = libs.begin();
        MapLibs::iterator itEnd = libs.end();
        for (; it != itEnd ; ++it)
        {
            std::list<std::wstring>* temp = it->second->getMacrosName();
            names->insert(names->end(), temp->begin(), temp->end());
            delete temp;
        }

        return names;
    }

    void clearAll()
    {
        for (MapLibs::iterator it = libs.begin(); it != libs.end() ; ++it)
        {
            while (!it->second->empty())
            {
                ScopedLibrary * pSL = it->second->top();
                types::InternalType * pIT = pSL->m_pLib;
                pIT->DecreaseRef();
                pIT->killMe();
                it->second->pop();
                delete pSL;
            }

            delete it->second;
        }
    }

private:
    MapLibs libs;
};
}
#endif /* !__LIBRARIES_HXX__ */
