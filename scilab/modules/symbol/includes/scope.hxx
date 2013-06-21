/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __NEWSCOPE_HXX__
#define __NEWSCOPE_HXX__

#include "variables.hxx"

namespace symbol
{
class Scope
{
public :
    Scope(Variables* _vars, int _iLevel) : m_vars(_vars), m_iLevel(_iLevel) {}
    virtual ~Scope();

    void put(const Symbol& _key, types::InternalType& _iT);
    void remove(const Symbol& _key);
    void print(std::wostream& ostr) const;
    void addVar(const symbol::Symbol& _key);
    void removeVar(const symbol::Symbol& _key);

    int getLevel()
    {
        return m_iLevel;
    }
private :
    typedef std::map<Symbol, int> SymbolMap;
    SymbolMap m_symbol;
    int m_iLevel;
    Variables* m_vars;
};

class Scopes
{
private :
    typedef std::list<Scope*> ScopesList;
    ScopesList m_scopes;
    int m_iLevel;
    Variables m_vars;

public :

    Scopes() : m_iLevel(-1) {}

    void scope_begin();
    void scope_end();

    /*setters*/

private :
    void put(const Symbol& _key, types::InternalType& _iT, int _iLevel);
public :
    void put(const Symbol& _key, types::InternalType& _iT);
    void putInPreviousScope(const Symbol& _key, types::InternalType& _iT);

    /* getters */
    types::InternalType* get(const Symbol& _key) const;
    types::InternalType* getCurrentLevel(const Symbol& _key) const;
    types::InternalType* getAllButCurrentLevel(const Symbol& _key) const;

    /*remove*/
    bool remove(const Symbol& _key);

    /*globals*/
    bool isGlobalVisible(const symbol::Symbol& _key) const;
    void removeGlobal(const symbol::Symbol& _key);
    void removeGlobalAll();
    void setGlobalVisible(const symbol::Symbol& _key, bool bVisible);
    types::InternalType* getGlobalValue(const symbol::Symbol& _key) const;
    bool isGlobalExists(const symbol::Symbol& _key) const;
    void setGlobalValue(const symbol::Symbol& _key, types::InternalType &value);
    void createEmptyGlobalValue(const symbol::Symbol& _key);

    /*tools*/
    std::list<symbol::Symbol>& getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd);
    void print(std::wostream& ostr) const;
};

inline std::wostream& operator<< (std::wostream& ostr, const Scopes& _scopes)
{
    _scopes.print(ostr);
    return ostr;
}

}

#endif /* !__NEWSCOPE_HXX__ */