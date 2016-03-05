/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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

#ifndef __LIBRARIES_HXX__
#define __LIBRARIES_HXX__

#include <map>
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

    void put(types::Library* _pLib, int _iLevel);
    types::MacroFile* get(const Symbol& _keyMacro) const;
    int getMacrosName(std::list<std::wstring>& lst);

    inline void put(ScopedLibrary* pSL)
    {
        stack.push(pSL);
    }

    inline bool empty() const
    {
        return stack.empty();
    }

    inline ScopedLibrary* top() const
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

    Libraries() {}

    Library* getOrCreate(const Symbol& _key);
    int getLevel(const Symbol& _key) const;
    void put(const Symbol& _keyLib, types::Library* _pLib, int _iLevel);
    bool putInPreviousScope(const Symbol& _keyLib, types::Library* _pLib, int _iLevel);
    types::InternalType* get(const Symbol& _key, int _iLevel);
    bool remove(const Symbol& _key, int _iLevel);
    int getMacrosName(std::list<std::wstring>& lst);
    int getVarsName(std::list<std::wstring>& lst);
    int getVarsToVariableBrowser(std::list<Library*>& lst);
    void clearAll();
    bool getVarsNameForWho(std::list<std::wstring>* lstVarName, int* iVarLenMax, bool bSorted = false) const;
    int whereis(std::list<std::wstring>& lst, const Symbol& _key);
    int librarieslist(std::list<std::wstring>& lst);

private:
    MapLibs libs;
};
}
#endif /* !__LIBRARIES_HXX__ */
