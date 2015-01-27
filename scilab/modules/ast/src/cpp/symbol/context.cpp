/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <iomanip>
#include <cctype>

#include "context.hxx"
#include "internal.hxx"
#include "function.hxx"
#include "macro.hxx"
#include "macrofile.hxx"
#include "variables.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "getmemory.h"
#include "os_string.h"
}

namespace symbol
{
Context* Context::me;

Context::Context()
{
    m_iLevel = 0;
    varStack.push(new VarList());
    globals = new std::list<Symbol>();
}

Context::~Context()
{
    while (!varStack.empty())
    {
        VarList * pVL = varStack.top();
        pVL->clear();
        delete pVL;
        varStack.pop();
    }
    if (globals)
    {
        delete globals;
    }
}

Context* Context::getInstance(void)
{
    if (me == 0)
    {
        me = new Context();
    }
    return me;
}

void Context::destroyInstance(void)
{
    if (me)
    {
        delete me;
    }
}

void Context::scope_begin()
{
    m_iLevel++;
    varStack.push(new VarList());
}

void Context::clearAll()
{
    libraries.clearAll();
    variables.clearAll();
}

void Context::scope_end()
{
    //clear varList of current scope
    if (varStack.empty() == false)
    {
        clearCurrentScope(true);
    }

    m_iLevel--;
}

bool Context::clearCurrentScope(bool _bClose)
{
    if (varStack.empty())
    {
        return true;
    }

    VarList* varList = varStack.top();
    std::map<Symbol, Variable*>::iterator it = varList->begin();
    for (; it != varList->end() ; ++it)
    {
        if (it->second->empty() == false && it->second->top()->m_iLevel == m_iLevel)
        {
            ScopedVariable * pSV = it->second->top();
            types::InternalType * pIT = pSV->m_pIT;
            pIT->DecreaseRef();
            pIT->killMe();
            it->second->pop();
            delete pSV;
        }
    }

    varList->clear();

    if (_bClose)
    {
        delete varList;
        varStack.pop();
    }

    return true;
}

Variable* Context::getOrCreate(const Symbol& _key)
{
    return variables.getOrCreate(_key);
}

types::InternalType* Context::get(const Symbol& _key)
{
    return get(_key, -1);
}

types::InternalType* Context::get(const Variable* _var)
{
    types::InternalType* pIT = _var->get();
    if (pIT == NULL)
    {
        //look in libraries
        pIT = libraries.get(_var->getSymbol(), -1);
        if (pIT && pIT->isLibrary() == false)
        {
            put((Variable*)_var, pIT);
        }
    }

    return pIT;
}

types::InternalType* Context::get(const Symbol& _key, int _iLevel)
{
    types::InternalType* pIT = NULL;
    if (_iLevel == m_iLevel || _iLevel == -1)
    {
        //look for in current VarList
        VarList::iterator it = varStack.top()->find(_key);
        if (it != varStack.top()->end())
        {
            if (it->second->empty() == false)
            {
                return it->second->top()->m_pIT;
            }
        }
    }

    if (pIT == NULL)
    {
        pIT = variables.get(_key, _iLevel);
        if (pIT == NULL)
        {
            //find in libraries
            pIT = libraries.get(_key, _iLevel);
        }
    }

    return pIT;
}

types::InternalType* Context::getCurrentLevel(const Symbol& _key)
{
    return variables.get(_key, m_iLevel);
}

types::InternalType* Context::getAllButCurrentLevel(const Symbol& _key)
{
    return variables.getAllButCurrentLevel(_key, m_iLevel);
}

types::InternalType* Context::getFunction(const Symbol& _key)
{
    return get(_key);
}

std::list<Symbol>* Context::getFunctionList(std::wstring _stModuleName)
{
    return variables.getFunctionList(_stModuleName, m_iLevel);
}

std::list<std::wstring>* Context::getVarsName()
{
    std::list<std::wstring>* vars = variables.getVarsName();
    std::list<std::wstring>* libs = libraries.getVarsName();
    vars->insert(vars->end(), libs->begin(), libs->end());
    delete libs;

    for (auto it = globals->begin(), itEnd = globals->end(); it != itEnd; ++it)
    {
        vars->push_back((*it).getName());
    }
    return vars;
}

std::list<std::wstring>* Context::getMacrosName()
{
    std::list<std::wstring>* vars = variables.getMacrosName();
    std::list<std::wstring>* libs = libraries.getMacrosName();
    vars->insert(vars->end(), libs->begin(), libs->end());
    delete libs;
    return vars;
}

std::list<std::wstring>* Context::getFunctionsName()
{
    return variables.getFunctionsName();
}

std::list<std::wstring>* Context::getVarsNameForWho(bool bSorted)
{
    std::list<std::wstring>* lstVar = new std::list<std::wstring>();
    int iZero = 0;
    variables.getVarsNameForWho(lstVar, &iZero, NULL, &iZero, bSorted);
    return lstVar;
}

std::list<std::wstring>* Context::getGlobalNameForWho(bool bSorted)
{
    std::list<std::wstring>* lstVar = new std::list<std::wstring>();
    int iZero = 0;
    variables.getVarsNameForWho(NULL, &iZero, lstVar, &iZero, bSorted);
    return lstVar;
}

void Context::put(Variable* _var, types::InternalType* _pIT)
{
    if (_pIT->isLibrary())
    {
        Library* lib = libraries.getOrCreate(_var->getSymbol());
        lib->put((types::Library*)_pIT, m_iLevel);
    }
    else
    {
        _var->put(_pIT, m_iLevel);
        if (varStack.empty() == false)
        {
            (*varStack.top())[_var->getSymbol()] = _var;
        }
    }
}

void Context::put(const Symbol& _key, types::InternalType* _pIT)
{
    Variable* var = variables.getOrCreate(_key);
    put(var, _pIT);
}

bool Context::remove(const Symbol& _key)
{
    bool ret = variables.remove(_key, m_iLevel);

    if (ret)
    {
        varStack.top()->erase(_key);
    }

    ret = ret | libraries.remove(_key, m_iLevel);
    return ret;
}

bool Context::removeAll()
{
    return clearCurrentScope(false);
}

bool Context::putInPreviousScope(Variable* _var, types::InternalType* _pIT)
{
    //add variable in previous scope
    variables.putInPreviousScope(_var, _pIT, m_iLevel - 1);

    //add variable in stack of using variables
    if (varStack.empty() == false)
    {
        VarList * list = varStack.top();
        varStack.pop();
        if (varStack.empty() == false)
        {
            (*varStack.top())[_var->getSymbol()] = _var;
            varStack.push(list);
        }
    }
    return true;
}

bool Context::addFunction(types::Function *_info)
{
    Variable* var = variables.getOrCreate(Symbol(_info->getName()));
    variables.putInPreviousScope(var, _info, 0);
    return true;
}

bool Context::addMacro(types::Macro *_info)
{
    put(Symbol(_info->getName()), _info);
    return true;
}

bool Context::addMacroFile(types::MacroFile *_info)
{
    put(Symbol(_info->getName()), _info);
    return true;
}

bool Context::isGlobalVisible(const Symbol& _key)
{
    return variables.isGlobalVisible(_key, m_iLevel);
}

/*return global variable existance status*/
bool Context::isGlobal(const Symbol& _key)
{
    return variables.isGlobal(_key, m_iLevel);
}

types::InternalType* Context::getGlobalValue(const Symbol& _key)
{
    return variables.getGlobalValue(_key);
}

void Context::setGlobalVisible(const Symbol& _key, bool bVisible)
{
    variables.setGlobalVisible(_key, bVisible, m_iLevel);
}

void Context::setGlobal(const Symbol& _key)
{
    variables.setGlobal(_key);
    globals->push_back(_key);
}

void Context::removeGlobal(const Symbol& _key)
{
    variables.removeGlobal(_key, m_iLevel);
    globals->remove(_key);
}

void Context::removeGlobalAll()
{
    std::list<Symbol>::iterator it = globals->begin();
    while (it != globals->end())
    {
        removeGlobal(*it);
        it = globals->begin();
    }

    globals->clear();
}

void Context::print(std::wostream& ostr, bool sorted) const
{
    std::list<std::wstring> lstVar;
    std::list<std::wstring> lstGlobal;
    int iVarLenMax = 10; // initialise to the minimal value of padding
    int iGlobalLenMax = 10; // initialise to the minimal value of padding
    variables.getVarsNameForWho(&lstVar, &iVarLenMax, &lstGlobal, &iGlobalLenMax);
    libraries.getVarsNameForWho(&lstVar, &iVarLenMax);

    if (sorted)
    {
        lstVar.sort();
        lstGlobal.sort();
    }

#define strSize 64
    wchar_t wcsVarElem[strSize];
    wchar_t wcsVarVariable[strSize];
    wchar_t wcsGlobalElem[strSize];
    wchar_t wcsGlobalVariable[strSize];

    int iMemTotal = 0;
    int iMemUsed  = 0;
    int nbMaxVar  = 0;

#ifdef _MSC_VER
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx (&statex);
    iMemTotal = (int)(statex.ullTotalPhys / (1024 * 1024));
#else
    iMemTotal = getmemorysize();
#endif

    ostr << _W("Your variables are:") << std::endl << std::endl;
    std::list<std::wstring>::const_iterator it = lstVar.begin();
    int iWidth = ConfigVariable::getConsoleWidth();
    int iCurrentWidth = 0;
    for (int i = 1; it != lstVar.end(); ++it, i++)
    {
        if (iCurrentWidth + iVarLenMax + 1 > iWidth)
        {
            ostr << std::endl;
            iCurrentWidth = 0;
        }
        ostr << std::setw(iVarLenMax + 1) << *it;
        iCurrentWidth += iVarLenMax + 1;
    }

    os_swprintf(wcsVarElem, strSize, _W(" using %10d elements out of  %10d.\n").c_str(), iMemUsed, iMemTotal);
    ostr << std::endl << wcsVarElem;

    os_swprintf(wcsVarVariable, strSize, _W(" and   %10d variables out of %10d.\n").c_str(), lstVar.size(), nbMaxVar);
    ostr << wcsVarVariable << std::endl;

    ostr << std::endl << _W("Your global variables are:") << std::endl << std::endl;
    it = lstGlobal.begin();
    for (int i = 1; it != lstGlobal.end(); ++it, i++)
    {
        ostr << std::setw(iGlobalLenMax + 1) << *it;
        if (i % 4 == 0)
        {
            ostr << std::endl;
        }
    }

    ostr << std::endl;

    os_swprintf(wcsGlobalElem, strSize, _W(" using %10d elements out of  %10d.\n").c_str(), iMemUsed, iMemTotal);
    ostr << std::endl << wcsGlobalElem;

    os_swprintf(wcsGlobalVariable, strSize, _W(" and   %10d variables out of %10d.\n").c_str(), lstGlobal.size(), nbMaxVar);
    ostr << wcsGlobalVariable;
}

int Context::getScopeLevel()
{
    return m_iLevel;
}

bool Context::isValidVariableName(const wchar_t* wcsVarName)
{
    static const wchar_t FORBIDDEN_CHARS[] = L" */\\.,;:^@><=+-&|()~\n\t'\"";
    if (wcslen(wcsVarName) == 0 || std::wcspbrk(wcsVarName, FORBIDDEN_CHARS) || isdigit(wcsVarName[0]))
    {
        return false;
    }
    return true;
}

bool Context::isValidVariableName(const char* name)
{
    bool isValid = false;
    wchar_t* wcsname = to_wide_string(name);
    if (wcsname)
    {
        isValid = isValidVariableName(wcsname);
        FREE(wcsname);
    }

    return isValid;
}
}
