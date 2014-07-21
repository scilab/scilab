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
        : m_iLevel(_iLevel), m_pIT(_pIT), m_globalVisible(false) {}

    int m_iLevel;
    bool m_globalVisible;
    types::InternalType* m_pIT;
};

struct Variable
{
    Variable(const Symbol& _name) : name(_name), m_Global(false), m_GlobalValue(NULL) {};
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
            stack.push(new ScopedVariable(_iLevel, _pIT));
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
        return stack.top();
    }

    inline void pop()
    {
        stack.pop();
    }

    inline Symbol name_get() const
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
            stack.push(new ScopedVariable(_iLevel, types::Double::Empty()));
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
    types::InternalType* m_GlobalValue;
    bool m_Global;
    typedef std::stack<ScopedVariable*> StackVar;
    StackVar stack;
};

struct Variables
{
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
            if (_iLevel == -1 || it->second->top()->m_iLevel == _iLevel)
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
            if (_iLevel == -1 || _var->top()->m_iLevel == _iLevel)
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
                it->second->put(pSave->m_pIT, pSave->m_iLevel);
                delete pSave;
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
            }
        }

        return true;
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

    std::list<std::wstring>* getMacrosName()
    {
        std::list<std::wstring>* plOut = new std::list<std::wstring>();
        MapVars::const_iterator it = vars.begin();
        for (; it != vars.end(); ++it)
        {
            if (it->second->empty() == false)
            {
                types::InternalType* pIT = it->second->top()->m_pIT;
                if (pIT && (pIT->isMacro() || pIT->isMacroFile()))
                {
                    plOut->push_back(it->first.name_get().c_str());
                }
            }
        }

        return plOut;
    }

    std::list<std::wstring>* getVarsName()
    {
        std::list<std::wstring>* plOut = new std::list<std::wstring>();
        MapVars::const_iterator it = vars.begin();
        for (; it != vars.end(); ++it)
        {
            if (it->second->empty() == false)
            {
                types::InternalType* pIT = it->second->top()->m_pIT;
                if (pIT &&
                        pIT->isMacro() == false &&
                        pIT->isMacroFile() == false &&
                        pIT->isFunction() == false)
                {
                    plOut->push_back(it->first.name_get().c_str());
                }
            }
        }

        return plOut;
    }

    std::list<std::wstring>* getFunctionsName()
    {
        std::list<std::wstring>* plOut = new std::list<std::wstring>();
        MapVars::const_iterator it = vars.begin();
        for (; it != vars.end(); ++it)
        {
            if (it->second->empty() == false)
            {
                types::InternalType* pIT = it->second->top()->m_pIT;
                if (pIT && pIT->isFunction())
                {
                    plOut->push_back(it->first.name_get().c_str());
                }
            }
        }

        return plOut;
    }

    std::list<Symbol>* getFunctionList(std::wstring _stModuleName, int _iLevel)
    {
        std::list<Symbol>* symb = new std::list<Symbol>();

        MapVars::iterator it = vars.begin();
        for (; it != vars.end() ; ++it)
        {
            if (it->second->empty())
            {
                continue;
            }

            if ((it->second->top()->m_iLevel == _iLevel || _iLevel == 1) && it->second->top()->m_pIT->isCallable())
            {
                types::Callable* pCall = it->second->top()->m_pIT->getAs<types::Callable>();
                if (_stModuleName == L"" || _stModuleName == pCall->getModule())
                {
                    symb->push_back(it->first);
                }
            }
        }

        return symb;
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
            _var->put(pVar->m_pIT, pVar->m_iLevel);
            delete pVar;
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

    bool isGlobal(const Symbol& _key, int _iLevel)
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
        for (MapVars::iterator it = vars.begin(); it != vars.end() ; ++it)
        {
            delete it->second;
        }
    }

private:
    typedef std::map<Symbol, Variable*> MapVars;
    MapVars vars;
};
}

#endif // !__VARIABLES_HXX__
