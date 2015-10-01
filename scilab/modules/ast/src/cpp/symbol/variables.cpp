/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <algorithm>
#include "variables.hxx"

namespace symbol
{
Variable::~Variable()
{
    while (!empty())
    {
        ScopedVariable * pSV = top();
        types::InternalType * pIT = pSV->m_pIT;
        pIT->DecreaseRef();
        pIT->killMe();
        pop();
        delete pSV;
    }

    if (m_GlobalValue)
    {
        m_GlobalValue->DecreaseRef();
        m_GlobalValue->killMe();
    }
}

void Variable::setGlobalValue(types::InternalType* _pIT)
{
    if (m_GlobalValue != _pIT)
    {
        if (m_GlobalValue)
        {
            m_GlobalValue->DecreaseRef();
            m_GlobalValue->killMe();
        }

        m_GlobalValue = _pIT;
        if (_pIT != NULL)
        {
            _pIT->IncreaseRef();
        }
    }
}

void Variable::setGlobalVisible(int _iLevel, bool _bVisible)
{
    if (empty() || top()->m_iLevel != _iLevel)
    {
        last = new ScopedVariable(_iLevel, types::Double::Empty());
        stack.push(last);
    }

    top()->m_globalVisible = _bVisible;
}


void Variable::put(types::InternalType* _pIT, int _iLevel)
{
    if (isGlobal() && isGlobalVisible(_iLevel))
    {
        setGlobalValue(_pIT);
        return;
    }

    if (empty() || top()->m_iLevel < _iLevel)
    {
        //create a new level
        last = new ScopedVariable(_iLevel, _pIT);
        stack.push(last);
        _pIT->IncreaseRef();
    }
    else
    {
        //update current level
        types::InternalType* pIT = top()->m_pIT;
        if (pIT != _pIT)
        {
            // _pIT may contained in pIT
            // so increases ref of _pIT before kill pIT
            top()->m_pIT = _pIT;
            _pIT->IncreaseRef();
            pIT->DecreaseRef();
            pIT->killMe();
        }
    }
}

Variable* Variables::getOrCreate(const Symbol& _key)
{
    MapVars::const_iterator it = vars.find(_key);
    if (it == vars.end())
    {
        //create an empty StackedValues
        Variable* var = new Variable(_key);
        vars[_key] = var;
        return var;
    }

    return it->second;
}

int Variables::getLevel(const Symbol& _key) const
{
    MapVars::const_iterator it = vars.find(_key);
    if (it != vars.end() && !it->second->empty())
    {
        return it->second->top()->m_iLevel;
    }

    return SCOPE_ALL;
}

void Variables::put(const Symbol& _key, types::InternalType* _pIT, int _iLevel)
{
    Variable* var = getOrCreate(_key);
    var->put(_pIT, _iLevel);
}

types::InternalType* Variables::get(const Symbol& _key, int _iLevel)
{
    MapVars::const_iterator it = vars.find(_key);
    if (it != vars.end() && it->second->empty() == false)
    {
        if (_iLevel == SCOPE_ALL || it->second->top()->m_iLevel == _iLevel)
        {
            return it->second->get();
        }
    }

    return NULL;
}

types::InternalType* Variables::get(Variable* _var, int _iLevel)
{
    if (_var != NULL && _var->empty() == false)
    {
        if (_iLevel == SCOPE_ALL || _var->top()->m_iLevel == _iLevel)
        {
            return _var->get();
        }
    }

    return NULL;
}

types::InternalType* Variables::getAllButCurrentLevel(const Symbol& _key, int _iLevel)
{
    MapVars::const_iterator it = vars.find(_key);
    if (it != vars.end() && it->second->empty() == false)
    {
        if (it->second->top()->m_iLevel < _iLevel)
        {
            return it->second->get();
        }
        else
        {
            ScopedVariable* pSave = it->second->top();
            it->second->pop();
            types::InternalType* pIT = getAllButCurrentLevel(_key, _iLevel);
            it->second->put(pSave);
            return pIT;
        }
    }

    return NULL;
}

bool Variables::remove(Variable* _var, int _iLevel)
{
    if (_var->empty() == false)
    {
        if (_var->top()->m_iLevel == _iLevel)
        {
            ScopedVariable* pSave = _var->top();
            types::InternalType* pIT = pSave->m_pIT;
            pIT->DecreaseRef();
            pIT->killMe();
            _var->pop();
            delete pSave;
            return true;
        }
    }

    return false;
}

bool Variables::remove(const Symbol& _key, int _iLevel)
{
    MapVars::iterator it = vars.find(_key);
    if (it != vars.end())
    {
        Variable* pVar = it->second;
        return remove(pVar, _iLevel);
    }

    return false;
}

int Variables::getMacrosName(std::list<std::wstring>& lst)
{
    for (auto it : vars)
    {
        if (it.second->empty() == false)
        {
            types::InternalType* pIT = it.second->top()->m_pIT;
            if (pIT && (pIT->isMacro() || pIT->isMacroFile()))
            {
                lst.push_back(it.first.getName().c_str());
            }
        }
    }

    return static_cast<int>(lst.size());
}

int Variables::getVarsName(std::list<std::wstring>& lst)
{
    for (auto it : vars)
    {
        if (it.second->empty() == false)
        {
            types::InternalType* pIT = it.second->top()->m_pIT;
            if (pIT &&
                    pIT->isMacro() == false &&
                    pIT->isMacroFile() == false &&
                    pIT->isFunction() == false)
            {
                lst.push_back(it.first.getName().c_str());
            }
        }
    }

    return static_cast<int>(lst.size());
}

bool Variables::getVarsNameForWho(std::list<std::wstring>& lstVarName, int* iVarLenMax, bool bSorted) const
{
    for (auto it : vars)
    {
        std::wstring wstrVarName(it.first.getName().c_str());
        if (it.second->empty() == false)
        {
            types::InternalType* pIT = it.second->top()->m_pIT;
            if (pIT && pIT->isFunction() == false)
            {
                lstVarName.push_back(wstrVarName);
                *iVarLenMax = std::max(*iVarLenMax, (int)wstrVarName.size());
            }
        }
    }

    if (bSorted)
    {
        lstVarName.sort();
    }

    return true;
}

bool Variables::getGlobalNameForWho(std::list<std::wstring>& lstVarName, int* iVarLenMax, bool bSorted) const
{
    for (auto it : vars)
    {
        if (it.second->empty() == false && it.second->isGlobal())
        {
            std::wstring wstrVarName(it.first.getName().c_str());
            lstVarName.push_back(wstrVarName);
            *iVarLenMax = std::max(*iVarLenMax, (int)wstrVarName.size());
        }
    }

    if (bSorted)
    {
        lstVarName.sort();
    }

    return true;
}

int Variables::getProtectedVarsName(std::list<std::wstring>& lstVarName) const
{
    for (auto it : vars)
    {
        if (it.second->empty() == false)
        {
            ScopedVariable* pSV = it.second->top();
            if (pSV->protect && it.first.getName() != L"ans")
            {
                lstVarName.push_back(it.first.getName());
            }
        }
    }

    return static_cast<int>(lstVarName.size());
}

int Variables::getFunctionsName(std::list<std::wstring>& lst)
{
    for (auto it : vars)
    {
        if (it.second->empty() == false)
        {
            types::InternalType* pIT = it.second->top()->m_pIT;
            if (pIT && pIT->isFunction())
            {
                lst.push_back(it.first.getName().c_str());
            }
        }
    }

    return static_cast<int>(lst.size());
}

int Variables::getFunctionList(std::list<Symbol>& lst, std::wstring _stModuleName, int _iLevel)
{
    for (auto var : vars)
    {
        if (var.second->empty())
        {
            continue;
        }

        if ((var.second->top()->m_iLevel == _iLevel || _iLevel == 1) && var.second->top()->m_pIT->isCallable())
        {
            types::Callable* pCall = var.second->top()->m_pIT->getAs<types::Callable>();
            if (_stModuleName == L"" || _stModuleName == pCall->getModule())
            {
                lst.push_back(var.first);
            }
        }
    }

    return static_cast<int>(lst.size());
}

int Variables::getVarsToVariableBrowser(std::list<Variable*>& lst)
{
    for (auto var : vars)
    {
        if (var.second->empty() == false)
        {
            types::InternalType* pIT = var.second->top()->m_pIT;
            if (pIT &&
                    pIT->isMacro() == false &&
                    pIT->isMacroFile() == false &&
                    pIT->isFunction() == false)
            {
                lst.push_back(var.second);
            }
        }
    }

    return static_cast<int>(lst.size());
}

bool Variables::putInPreviousScope(Variable* _var, types::InternalType* _pIT, int _iLevel)
{
    if (_var->empty())
    {
        _var->put(_pIT, _iLevel);
    }
    else if (_var->top()->m_iLevel > _iLevel)
    {
        ScopedVariable* pVar = _var->top();
        _var->pop();
        putInPreviousScope(_var, _pIT, _iLevel);
        //decresef ref before, increase it in put
        //pVar->m_pIT->DecreaseRef();
        _var->put(pVar);
    }
    else
    {
        _var->put(_pIT, _iLevel);
    }

    return true;
}

//globals

void Variables::setGlobal(const Symbol& _key)
{
    getOrCreate(_key)->setGlobal(true);
}

void Variables::setGlobalVisible(const Symbol& _key, bool _bVisible, int _iLevel)
{
    Variable* pVar = getOrCreate(_key);
    pVar->setGlobalVisible(_iLevel, _bVisible);
    if (_bVisible)
    {
        pVar->setGlobal(true);
    }
}

bool Variables::isGlobalVisible(const Symbol& _key, int _iLevel)
{
    return getOrCreate(_key)->isGlobalVisible(_iLevel);
}

bool Variables::isGlobal(const Symbol& _key, int /*_iLevel*/)
{
    return getOrCreate(_key)->isGlobal();
}

types::InternalType* Variables::getGlobalValue(const Symbol& _key)
{
    return getOrCreate(_key)->getGlobalValue();
}

void Variables::removeGlobal(const Symbol& _key, int _iLevel)
{
    Variable* pVar = getOrCreate(_key);
    if (pVar->isGlobal())
    {
        pVar->setGlobal(false);
        pVar->setGlobalValue(NULL);
    }

    remove(pVar, _iLevel);
}

void Variables::clearAll()
{
    for (auto var : vars)
    {
        delete var.second;
    }

    vars.clear();
}


}