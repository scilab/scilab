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

#ifndef __VARIABLES_HXX__
#define __VARIABLES_HXX__

#include <string>
#include <map>
#include <stack>
#include "symbol.hxx"
#include "internal.hxx"
#include "double.hxx"
#include "dynlib_ast.h"
#include "callable.hxx"

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

struct EXTERN_AST Variable
{
    typedef std::stack<ScopedVariable*> StackVar;

    Variable(const Symbol& _name) : name(_name), m_Global(false), m_GlobalValue(NULL), last(nullptr) {};
    ~Variable();

    bool put(types::InternalType* _pIT, int _iLevel);
    void setGlobalValue(types::InternalType* _pIT);
    void setGlobalVisible(int _iLevel, bool _bVisible);

    inline bool put(ScopedVariable* pSV)
    {
        last = pSV;
        stack.push(last);
        return true;
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

    Variable* getOrCreate(const Symbol& _key);
    int getLevel(const Symbol& _key) const;
    void put(const Symbol& _key, types::InternalType* _pIT, int _iLevel);
    types::InternalType* get(const Symbol& _key, int _iLevel);
    types::InternalType* get(Variable* _var, int _iLevel);
    types::InternalType* getAllButCurrentLevel(const Symbol& _key, int _iLevel);
    bool remove(Variable* _var, int _iLevel);
    bool remove(const Symbol& _key, int _iLevel);
    int getMacrosName(std::list<std::wstring>& lst);
    int getVarsName(std::list<std::wstring>& lst);
    bool getVarsNameForWho(std::list<std::wstring>& lstVarName, int* iVarLenMax, bool bSorted = false) const;
    bool getGlobalNameForWho(std::list<std::wstring>& lstVarName, int* iVarLenMax, bool bSorted = false) const;
    int getProtectedVarsName(std::list<std::wstring>& lstVarName) const;
    int getFunctionsName(std::list<std::wstring>& lst);
    int getFunctionList(std::list<Symbol>& lst, std::wstring _stModuleName, int _iLevel);
    int getFunctionList(std::list<types::Callable *>& lst, std::wstring _stModuleName, int _iLevel);
    int getVarsToVariableBrowser(std::list<Variable*>& lst);
    bool putInPreviousScope(Variable* _var, types::InternalType* _pIT, int _iLevel);

    //globals

    void setGlobal(const Symbol& _key);
    void setGlobalVisible(const Symbol& _key, bool _bVisible, int _iLevel);
    bool isGlobalVisible(const Symbol& _key, int _iLevel);
    bool isGlobal(const Symbol& _key, int /*_iLevel*/);
    types::InternalType* getGlobalValue(const Symbol& _key);
    void removeGlobal(const Symbol& _key, int _iLevel);
    void clearAll();

private:
    MapVars vars;
};
}

#endif // !__VARIABLES_HXX__
