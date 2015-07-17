/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __VARIABLES_HXX__
#define __VARIABLES_HXX__

#include <string>
#include <stack>
#include "symbol.hxx"
#include "internal.hxx"
#include "double.hxx"

namespace symbol
{
struct ScopedVariable
{
    ScopedVariable(int _iLevel, types::InternalType* _pIT)
        : m_iLevel(_iLevel), m_pIT(_pIT), m_globalVisible(false), protect(false) {}

    int m_iLevel;
    types::InternalType* m_pIT;
    bool m_globalVisible;
    bool protect;
};

struct Variable
{
    typedef std::stack<ScopedVariable*> StackVar;

    Variable(const Symbol& _name) : name(_name), m_Global(false), m_GlobalValue(NULL), last(nullptr) {};
    ~Variable()
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

    void put(ScopedVariable* pSV)
    {
        last = pSV;
        stack.push(last);
    }

    void put(types::InternalType* _pIT, int _iLevel)
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

    inline types::InternalType* get() const
    {
        if (empty())
        {
            return NULL;
        }

        if (m_Global && top()->m_globalVisible)
        {
            return m_GlobalValue;
        }
        else
        {
            return top()->m_pIT;
        }
    }

    inline bool empty() const
    {
        return stack.empty();
    }

    inline ScopedVariable* top() const
    {
        return last;
    }

    inline void pop()
    {
        stack.pop();
        if (stack.empty())
        {
            last = nullptr;
        }
        else
        {
            last = stack.top();
        }
    }

    inline Symbol getSymbol() const
    {
        return name;
    }

    //globals

    inline void setGlobal(bool _bGlobal)
    {
        m_Global = _bGlobal;
    }

    inline bool isGlobal()
    {
        return m_Global;
    }

    inline bool isGlobalVisible(int _iLevel)
    {
        if (empty() == false)
        {
            if (top()->m_iLevel == _iLevel)
            {
                return top()->m_globalVisible;
            }
        }

        return false;
    }

    void setGlobalVisible(int _iLevel, bool _bVisible)
    {
        if (empty() || top()->m_iLevel != _iLevel)
        {
            last = new ScopedVariable(_iLevel, types::Double::Empty());
            stack.push(last);
        }

        top()->m_globalVisible = _bVisible;
    }

    void setGlobalValue(types::InternalType* _pIT)
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

    inline types::InternalType* getGlobalValue()
    {
        return m_GlobalValue;
    }

private :
    Symbol name;
    bool m_Global;
    types::InternalType* m_GlobalValue;
    StackVar stack;
    ScopedVariable* last;
};

struct Variables
{
    typedef std::map<Symbol, Variable*> MapVars;

    Variables() {};

    Variable* getOrCreate(const Symbol& _key)
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

    int getLevel(const Symbol& _key) const
    {
        MapVars::const_iterator it = vars.find(_key);
        if (it != vars.end() && !it->second->empty())
        {
            return it->second->top()->m_iLevel;
        }

        return SCOPE_ALL;
    }

    void put(const Symbol& _key, types::InternalType* _pIT, int _iLevel)
    {
        Variable* var = getOrCreate(_key);
        var->put(_pIT, _iLevel);
    }

    types::InternalType* get(const Symbol& _key, int _iLevel)
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

    types::InternalType* get(Variable* _var, int _iLevel)
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

    types::InternalType* getAllButCurrentLevel(const Symbol& _key, int _iLevel)
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

    bool remove(Variable* _var, int _iLevel)
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

    bool remove(const Symbol& _key, int _iLevel)
    {
        MapVars::iterator it = vars.find(_key);
        if (it != vars.end())
        {
            Variable* pVar = it->second;
            return remove(pVar, _iLevel);
        }

        return false;
    }

    int getMacrosName(std::list<std::wstring>& lst)
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

    int getVarsName(std::list<std::wstring>& lst)
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

    bool getVarsNameForWho(std::list<std::wstring>& lstVarName, int* iVarLenMax, bool bSorted = false) const
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

    bool getGlobalNameForWho(std::list<std::wstring>& lstVarName, int* iVarLenMax, bool bSorted = false) const
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

    int getProtectedVarsName(std::list<std::wstring>& lstVarName) const
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

    int getFunctionsName(std::list<std::wstring>& lst)
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

    int getFunctionList(std::list<Symbol>& lst, std::wstring _stModuleName, int _iLevel)
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

    int getVarsToVariableBrowser(std::list<Variable*>& lst)
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

    bool putInPreviousScope(Variable* _var, types::InternalType* _pIT, int _iLevel)
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

    void setGlobal(const Symbol& _key)
    {
        getOrCreate(_key)->setGlobal(true);
    }

    void setGlobalVisible(const Symbol& _key, bool _bVisible, int _iLevel)
    {
        Variable* pVar = getOrCreate(_key);
        pVar->setGlobalVisible(_iLevel, _bVisible);
        if (_bVisible)
        {
            pVar->setGlobal(true);
        }
    }

    bool isGlobalVisible(const Symbol& _key, int _iLevel)
    {
        return getOrCreate(_key)->isGlobalVisible(_iLevel);
    }

    bool isGlobal(const Symbol& _key, int /*_iLevel*/)
    {
        return getOrCreate(_key)->isGlobal();
    }

    types::InternalType* getGlobalValue(const Symbol& _key)
    {
        return getOrCreate(_key)->getGlobalValue();
    }

    void removeGlobal(const Symbol& _key, int _iLevel)
    {
        Variable* pVar = getOrCreate(_key);
        if (pVar->isGlobal())
        {
            pVar->setGlobal(false);
            pVar->setGlobalValue(NULL);
        }

        remove(pVar, _iLevel);
    }

    void clearAll()
    {
        for (auto var : vars)
        {
            delete var.second;
        }
    }

private:
    MapVars vars;
};
}

#endif // !__VARIABLES_HXX__
