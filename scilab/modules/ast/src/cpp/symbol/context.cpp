/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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
Context* Context::me = nullptr;

Context::Context()
{
    m_iLevel = SCOPE_ALL;
    globals = new std::list<Symbol>();
    console = nullptr;
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
    if (me == nullptr)
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
        me = nullptr;
    }
}

void Context::scope_begin()
{
    m_iLevel++;
    if (m_iLevel == SCOPE_CONSOLE)
    {
        console = new VarList();
        varStack.push(console);
    }
    else
    {
        varStack.push(new VarList());
    }
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

    if (m_iLevel < SCOPE_CONSOLE)
    {
        console = nullptr;
    }
}

bool Context::clearCurrentScope(bool _bClose)
{
    if (varStack.empty())
    {
        return true;
    }

    VarList* varList = varStack.top();
    std::list<Symbol> toremove;
    for (auto var : *varList)
    {
        if (var.second->empty() == false)
        {
            ScopedVariable* pSV = var.second->top();
            if (pSV->m_iLevel == m_iLevel && (_bClose || pSV->protect == false))
            {
                types::InternalType * pIT = pSV->m_pIT;
                if (pIT->isLibrary())
                {
                    // at scilab exit, pIT have a ref == 2 because
                    // it is contains in libraries and variables.
                    // call remove will set ref to 1 then the next
                    // pIT->DecreaseRef(); pIT->killMe(); will delete it.
                    libraries.remove(var.first, m_iLevel);
                }

                pIT->DecreaseRef();
                pIT->killMe();

                var.second->pop();
                delete pSV;
                toremove.push_back(var.first);
            }
        }
    }

    if (_bClose)
    {
        delete varList;
        varStack.pop();
    }
    else
    {
        for (auto var : toremove)
        {
            varList->erase(var);
        }
    }
    return true;
}

Variable* Context::getOrCreate(const Symbol& _key)
{
    return variables.getOrCreate(_key);
}

int Context::getLevel(const Symbol & _key) const
{
    VarList::iterator it = varStack.top()->find(_key);
    if (it != varStack.top()->end())
    {
        if (!it->second->empty())
        {
            return it->second->top()->m_iLevel;
        }
    }
    else
    {
        const int ret = variables.getLevel(_key);
        if (ret == SCOPE_ALL)
        {
            return libraries.getLevel(_key);
        }
        else
        {
            return ret;
        }
    }

    return SCOPE_ALL;
}

types::InternalType* Context::get(const Symbol& _key)
{
    return get(_key, SCOPE_ALL);
}

types::InternalType* Context::get(const Variable* _var)
{
    types::InternalType* pIT = _var->get();
    if (pIT == NULL)
    {
        //look in libraries
        pIT = libraries.get(_var->getSymbol(), SCOPE_ALL);
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
    if (_iLevel == m_iLevel || _iLevel == SCOPE_ALL)
    {
        //look for in current VarList
        VarList::iterator it = varStack.top()->find(_key);
        if (it != varStack.top()->end())
        {
            if (it->second->empty() == false)
            {
                pIT = it->second->get();
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

types::InternalType* Context::getCurrentLevel(Variable* _var)
{
    return variables.get(_var, m_iLevel);
}

types::InternalType* Context::getAllButCurrentLevel(const Symbol& _key)
{
    return variables.getAllButCurrentLevel(_key, m_iLevel);
}

types::InternalType* Context::getAtLevel(const Symbol& _key, int level)
{
    return variables.getAllButCurrentLevel(_key, level == SCOPE_ALL ? m_iLevel : level + 1);
}

types::InternalType* Context::getFunction(const Symbol& _key)
{
    return get(_key);
}

int Context::getFunctionList(std::list<Symbol>& lst, const std::wstring& _stModuleName)
{
    return variables.getFunctionList(lst, _stModuleName, m_iLevel);
}

int Context::getFunctionList(std::list<types::Callable *> & lst, std::wstring _stModuleName)
{
    return variables.getFunctionList(lst, _stModuleName, m_iLevel);
}

int Context::getConsoleVarsName(std::list<std::wstring>& lst)
{
    if (console)
    {
        for (const auto & var : *console)
        {
            lst.push_back(var.first.getName());
        }
    }

    return static_cast<int>(lst.size());
}

int Context::getVarsName(std::list<std::wstring>& lst)
{
    variables.getVarsName(lst);
    libraries.getVarsName(lst);

    return static_cast<int>(lst.size());
}

int Context::getMacrosName(std::list<std::wstring>& lst)
{
    variables.getMacrosName(lst);
    libraries.getMacrosName(lst);
    return static_cast<int>(lst.size());
}

int Context::getFunctionsName(std::list<std::wstring>& lst)
{
    return variables.getFunctionsName(lst);
}

int Context::getVarsNameForWho(std::list<std::wstring>& lst, bool bSorted)
{
    int iZero = 0;
    variables.getVarsNameForWho(lst, &iZero, bSorted);
    return static_cast<int>(lst.size());
}

int Context::getGlobalNameForWho(std::list<std::wstring>& lst, bool bSorted)
{
    int iZero = 0;
    variables.getGlobalNameForWho(lst, &iZero, bSorted);
    return static_cast<int>(lst.size());
}

int Context::getWhereIs(std::list<std::wstring>& lst, const std::wstring& _str)
{
    return libraries.whereis(lst, Symbol(_str));
}

int Context::getLibrariesList(std::list<std::wstring>& lst)
{
    return libraries.librarieslist(lst);
}

bool Context::put(Variable* _var, types::InternalType* _pIT)
{
    if (_pIT->isLibrary())
    {
        Library* lib = libraries.getOrCreate(_var->getSymbol());
        lib->put((types::Library*)_pIT, m_iLevel);
    }

    if (_var->put(_pIT, m_iLevel) == false)
    {
        return false;
    }

    if (varStack.empty() == false)
    {
        (*varStack.top())[_var->getSymbol()] = _var;
    }

    return true;
}

bool Context::put(const Symbol& _key, types::InternalType* _pIT)
{
    Variable* var = variables.getOrCreate(_key);

    if (var->empty())
    {
        //box is empty, check if a macro from a library have this name.
        //in this case, add it to context before set new value.
        types::InternalType* pIT = get(_key);
        if (pIT && (pIT->isMacroFile() || pIT->isMacro()))
        {
            put(var, pIT);
            return put(var, _pIT);
        }
    }

    return put(var, _pIT);
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
    if (variables.putInPreviousScope(_var, _pIT, m_iLevel - 1) == false)
    {
        return false;
    }

    //add variable in stack of using variables
    if (varStack.empty() == false)
    {
        VarList * list = varStack.top();
        varStack.pop();
        if (varStack.empty() == false)
        {
            (*varStack.top())[_var->getSymbol()] = _var;
        }
        varStack.push(list);
    }

    if (_pIT->isLibrary())
    {
        libraries.putInPreviousScope(_var->getSymbol(), _pIT->getAs<types::Library>(), m_iLevel - 1);
    }
    return true;
}

bool Context::addFunction(types::Function *_info)
{
    Variable* var = variables.getOrCreate(Symbol(_info->getName()));
    variables.putInPreviousScope(var, _info, SCOPE_GATEWAY);
    return true;
}

bool Context::addMacro(types::Macro *_info)
{
    return put(Symbol(_info->getName()), _info);
}

bool Context::addMacroFile(types::MacroFile *_info)
{
    return put(Symbol(_info->getName()), _info);
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

bool Context::removeGlobal(const Symbol& _key)
{
    // skip permanant variables : %modalWarning, %toolboxes, %toolboxes_dir
    if (_key.getName() == L"%modalWarning"  ||
            _key.getName() == L"%toolboxes"     ||
            _key.getName() == L"%toolboxes_dir")
    {
        return false;
    }

    variables.removeGlobal(_key, m_iLevel);
    globals->remove(_key);
    return true;
}

void Context::removeGlobalAll()
{
    std::list<Symbol>::iterator it = globals->begin();

    while (it != globals->end())
    {
        if (removeGlobal(*it) == false)
        {
            globals->remove(*it);
        }

        it = globals->begin();
    }

    globals->clear();

    globals->emplace_back(L"%modalWarning");
    globals->emplace_back(L"%toolboxes");
    globals->emplace_back(L"%toolboxes_dir");
}

void Context::print(std::wostream& ostr, bool sorted) const
{
    std::list<std::wstring> lstVar;
    std::list<std::wstring> lstGlobal;
    int iVarLenMax = 10; // initialise to the minimal value of padding
    int iGlobalLenMax = 10; // initialise to the minimal value of padding
    variables.getVarsNameForWho(lstVar, &iVarLenMax);
    variables.getGlobalNameForWho(lstGlobal, &iGlobalLenMax);
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
    int iMemUsed = 0;
    int nbMaxVar = 0;

#ifdef _MSC_VER
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
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

int Context::getLibsToVariableBrowser(std::list<Library*>& lst)
{
    libraries.getVarsToVariableBrowser(lst);

    std::list<Library*> toremove;
    //list lib that have a variable with the same name
    for (auto lib : lst)
    {
        Variable* var = getOrCreate(lib->getSymbol());
        if (var->empty() == false)
        {
            toremove.push_back(lib);
        }
    }

    //remove
    for (auto lib : toremove)
    {
        lst.remove(lib);
    }

    return static_cast<int>(lst.size());
}

int Context::getVarsToVariableBrowser(std::list<Variable*>& lst)
{
    variables.getVarsToVariableBrowser(lst);
    return static_cast<int>(lst.size());
}

void Context::updateProtection(bool protect)
{
    if (varStack.empty() == false)
    {
        VarList* lst = varStack.top();
        for (auto var : *lst)
        {
            if (var.second->empty() == false)
            {
                ScopedVariable* pSV = var.second->top();
                //only for current scope but normally vars in VarStack are in the current scope
                if (pSV->m_iLevel == m_iLevel)
                {
                    pSV->protect = protect;
                }
                else
                {
                    std::wcerr << L"heu ... " << var.first.getName() << std::endl;
                }
            }
        }
    }
}

void Context::protect()
{
    updateProtection(true);
}

void Context::unprotect()
{
    updateProtection(false);
}

bool Context::isprotected(const Symbol& key)
{
    return isprotected(getOrCreate(key));
}

bool Context::isprotected(Variable* _var)
{
    //don't check protection on "ans"
    if (_var->getSymbol().getName() == L"ans")
    {
        return false;
    }

    if (_var->empty() == false)
    {
        ScopedVariable* pSV = _var->top();
        if (pSV->m_iLevel == m_iLevel && pSV->protect)
        {
            return true;
        }
    }
    return false;
}

int Context::protectedVars(std::list<std::wstring>& vars)
{
    return variables.getProtectedVarsName(vars);
}
}
