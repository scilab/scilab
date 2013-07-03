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

#include "scope.hxx"

namespace symbol
{

/*Scope*/
Scope::~Scope()
{
    SymbolMap::iterator it = m_symbol.begin();
    for (; it != m_symbol.end() ; it++)
    {
        m_vars->remove(it->first);
    }
}

void Scope::put(const Symbol& _key, types::InternalType& _iT)
{
    m_symbol[_key] = 0;
    m_vars->put(_key, _iT, m_iLevel);
}

void Scope::remove(const Symbol& _key)
{
    m_symbol.erase(_key);
    m_vars->remove(_key);
}

void Scope::print(std::wostream& ostr) const
{
    ostr << L"scope " << m_iLevel << L" : " << std::endl;

    SymbolMap::const_iterator it = m_symbol.begin();
    for (; it != m_symbol.end() ; it++)
    {
        ostr.width(25);
        ostr << it->first.name_get();
        types::InternalType* pIT = m_vars->getInSpecificLevel(it->first, m_iLevel);
        if (pIT)
        {
            ostr << L" (" << pIT->getRef();
            ostr << L") = " << pIT->getTypeStr() << std::endl;
        }
        else
        {
            ostr << L"NULL Oo";
        }
    }
}

void Scope::addVar(const symbol::Symbol& _key)
{
    m_symbol[_key] = 0;
}

void Scope::removeVar(const symbol::Symbol& _key)
{
    m_symbol.erase(_key);
}

/*Scopes*/
void Scopes::scope_begin()
{
    m_iLevel++;
    m_scopes.push_back(new Scope(&m_vars, m_iLevel));
    m_vars.IncreaseLevel();
}

void Scopes::scope_end()
{
    Scope* scope = m_scopes.back();
    delete scope;
    m_scopes.pop_back();
    m_iLevel--;
    m_vars.DecreaseLevel();
}

void Scopes::put(const Symbol& _key, types::InternalType& _iT, int _iLevel)
{
    ScopesList::iterator it = m_scopes.begin();
    for (; it != m_scopes.end() ; it++)
    {
        //fond good scope and put variable
        if ((*it)->getLevel() == _iLevel)
        {
            (*it)->put(_key, _iT);
        }
    }
}

void Scopes::put(const Symbol& _key, types::InternalType& _iT)
{
    put(_key, _iT, m_iLevel);
}

void Scopes::putInPreviousScope(const Symbol& _key, types::InternalType& _iT)
{
    put(_key, _iT, m_iLevel - 1);
}

/* getters */
types::InternalType* Scopes::get(const Symbol& _key) const
{
    return m_vars.get(_key);
}

types::InternalType* Scopes::getCurrentLevel(const Symbol& _key) const
{
    return m_vars.getCurrentLevel(_key);
}

types::InternalType* Scopes::getAllButCurrentLevel(const Symbol& _key) const
{
    return m_vars.getAllButCurrentLevel(_key);
}

/*remove*/
bool Scopes::remove(const Symbol& _key)
{
    m_scopes.back()->remove(_key);
    return true;
}

/*globales*/
bool Scopes::isGlobalVisible(const symbol::Symbol& _key) const
{
    return m_vars.isGlobalVisible(_key);
}

void Scopes::removeGlobal(const symbol::Symbol& _key)
{
    m_vars.removeGlobal(_key);
}

void Scopes::removeGlobalAll()
{
    m_vars.removeGlobalAll();
}

void Scopes::setGlobalVisible(const symbol::Symbol& _key, bool bVisible)
{
    Scope* pBack = m_scopes.back();
    if (bVisible)
    {
        pBack->addVar(_key);
    }
    else
    {
        pBack->removeVar(_key);
    }
    m_vars.setGlobalVisible(_key, bVisible);
}

types::InternalType* Scopes::getGlobalValue(const symbol::Symbol& _key) const
{
    return m_vars.getGlobalValue(_key);
}

bool Scopes::isGlobalExists(const symbol::Symbol& _key) const
{
    return m_vars.isGlobalExists(_key);
}

void Scopes::setGlobalValue(const symbol::Symbol& _key, types::InternalType& _value)
{
    m_vars.setGlobalValue(_key, _value);
}

void Scopes::createEmptyGlobalValue(const symbol::Symbol& _key)
{
    return m_vars.createEmptyGlobalValue(_key);
}


/*tools*/
std::list<symbol::Symbol>& Scopes::getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd)
{
    return m_vars.getFunctionList(_stModuleName, _bFromEnd);
}

std::list<std::wstring>& Scopes::getVarsName()
{
    return m_vars.getVarsName();
}

std::list<std::wstring>& Scopes::getMacrosName()
{
    return m_vars.getMacrosName();
}

std::list<std::wstring>& Scopes::getFunctionsName()
{
    return m_vars.getFunctionsName();
}

void Scopes::print(std::wostream& ostr) const
{
    ScopesList::const_iterator it = m_scopes.begin();

    for (; it != m_scopes.end() ; it++)
    {
        (*it)->print(ostr);
        ostr << std::endl << std::endl;
    }
}

}
