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

    Library(const Symbol& _name) : name(_name), m_global(false) {};

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

    void put(ScopedLibrary* pSL)
    {
        stack.push(pSL);
    }

    types::MacroFile* get(const Symbol& _keyMacro) const
    {
        if (empty() == false)
        {
            return top()->getMacroFile(_keyMacro);
        }

        return nullptr;
    }

    int getMacrosName(std::list<std::wstring>& lst)
    {
        if (empty() == false)
        {
            top()->m_pLib->getMacrosName(lst);
        }

        return static_cast<int>(lst.size());
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

    inline Symbol getSymbol() const
    {
        return name;
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

    int getLevel(const Symbol& _key) const
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

    void put(const Symbol& _keyLib, types::Library* _pLib, int _iLevel)
    {
        Library* lib = getOrCreate(_keyLib);
        lib->put(_pLib, _iLevel);
    }

    bool putInPreviousScope(const Symbol& _keyLib, types::Library* _pLib, int _iLevel)
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
            //decresef ref before, increase it in put
            //pVar->m_pIT->DecreaseRef();
            lib->put(pLib->m_pLib, pLib->m_iLevel);
        }
        else
        {
            lib->put(_pLib, _iLevel);
        }

        return true;
    }

    types::InternalType* get(const Symbol& _key, int _iLevel)
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
        for (auto it = libs.rbegin(), itEnd = libs.rend(); it != itEnd ; ++it)
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

    int getMacrosName(std::list<std::wstring>& lst)
    {
        MapLibs::iterator it = libs.begin();
        MapLibs::iterator itEnd = libs.end();
        for (auto it : libs)
        {
            it.second->getMacrosName(lst);
        }

        return static_cast<int>(lst.size());
    }

    int getVarsName(std::list<std::wstring>& lst)
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

    int getVarsToVariableBrowser(std::list<Library*>& lst)
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

    void clearAll()
    {
        for (auto lib : libs)
        {
            while (!lib.second->empty())
            {
                ScopedLibrary * pSL = lib.second->top();
                types::InternalType * pIT = pSL->m_pLib;
                pIT->DecreaseRef();
                pIT->killMe();
                lib.second->pop();
                delete pSL;
            }

            delete lib.second;
        }
    }

    bool getVarsNameForWho(std::list<std::wstring>* lstVarName, int* iVarLenMax, bool bSorted = false) const
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

    int whereis(std::list<std::wstring>& lst, const Symbol& _key)
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

    int librarieslist(std::list<std::wstring>& lst)
    {
        for (auto lib : libs)
        {
            lst.push_back(lib.first.getName());
        }

        return static_cast<int>(lst.size());
    }

private:
    MapLibs libs;
};
}
#endif /* !__LIBRARIES_HXX__ */
