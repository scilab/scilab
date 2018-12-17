/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
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

/**
** \file symbol/context.hh
** \brief Define class Context.
*/

#ifndef __CONTEXT_HXX__
#define __CONTEXT_HXX__

#include "function.hxx"
#include "variables.hxx"
#include "libraries.hxx"

extern "C"
{
#include "dynlib_ast.h"
}

namespace symbol
{

/** \brief Define class Context.
*/
class EXTERN_AST Context
{
public:
    typedef std::map<Symbol, Variable*> VarList;
    typedef std::stack<VarList*> VarStack;

    static Context* getInstance(void);

    static void destroyInstance(void);

    /** Open a context scope i.e
    ** open the heap table one
    ** and the env table too. */
    void scope_begin();

    /** Close a context scope i.e
    ** close the heap table one
    ** and the env table too. */
    void scope_end();

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion. Otherwise return the empty pointer. */
    types::InternalType* get(const Symbol& key);
    types::InternalType* get(const Variable* _var);
    Variable* getOrCreate(const Symbol& _key);
    int getLevel(const Symbol & _key) const;

    /** If key was associated to some Entry_T in the last opened scope, return it.
    ** Otherwise return the empty pointer. */
    types::InternalType* getCurrentLevel(const Symbol& key);
    types::InternalType* getCurrentLevel(Variable* _var);

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion DESPITE the current/last one. Otherwise return the empty pointer. */
    types::InternalType* getAllButCurrentLevel(const Symbol& key);
    types::InternalType* getAtLevel(const Symbol& key, int level = SCOPE_ALL);

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion. Otherwise return the empty pointer. */
    types::InternalType* getFunction(const Symbol& key);

    /*return function list in the module _stModuleName*/
    int getFunctionList(std::list<Symbol>& lst, const std::wstring& _stModuleName);
    /*return function list in the module _stModuleName*/
    int getFunctionList(std::list<types::Callable *>& lst, std::wstring _stModuleName);

    int getConsoleVarsName(std::list<std::wstring>& lst);
    int getVarsName(std::list<std::wstring>& lst);
    int getMacrosName(std::list<std::wstring>& lst);
    int getFunctionsName(std::list<std::wstring>& lst);
    int getVarsNameForWho(std::list<std::wstring>& lst, bool sorted);
    int getGlobalNameForWho(std::list<std::wstring>& lst, bool sorted);
    int getWhereIs(std::list<std::wstring>& lst, const std::wstring& _str);
    int getLibrariesList(std::list<std::wstring>& lst);
    int getVarsToVariableBrowser(std::list<Variable*>& lst);
    int getLibsToVariableBrowser(std::list<Library*>& lst);
    /* global functions */

    /*return global variable visibility status*/
    bool isGlobalVisible(const Symbol& key);

    /*return global variable existance status*/
    bool isGlobal(const Symbol& key);

    /*remove global variable and all visibility references */
    //clearglobal("a")
    bool removeGlobal(const Symbol& key);

    /*remove all global variables and references */
    //clearglobal
    void removeGlobalAll();
    void clearAll();

    //predef
    void protect();
    void unprotect();
    bool isprotected(const Symbol& key);
    bool isprotected(Variable* _var);
    int protectedVars(std::list<std::wstring>& lst);

    /*set variable visible/hidden in current global scope*/
    void setGlobalVisible(const Symbol& key, bool bVisible);
    void setGlobal(const Symbol& key);

    types::InternalType* getGlobalValue(const Symbol& _key);

    /*add symbol and value in the stack*/
    bool put(const Symbol& _key, types::InternalType* _pIT);
    bool put(Variable* _var, types::InternalType* _pIT);
    /*add symbol and value in the previous scope*/
    bool putInPreviousScope(Variable* _var, types::InternalType* _pIT);

    /* remove symbol/value association */
    //clear("a")
    bool remove(const Symbol& key);
    //clear();
    bool removeAll();

    bool addFunction(types::Function *_info);
    bool addMacro(types::Macro *_info);
    bool addMacroFile(types::MacroFile *_info);
    void print(std::wostream& ostr, bool bSorted = false) const;
    int getScopeLevel();
    bool isValidVariableName(const wchar_t*);
    bool isValidVariableName(const char*);

    inline bool isOriginalSymbol(const symbol::Symbol & sym) const
    {
        return getLevel(sym) == 0;
    }

private:

    types::InternalType* get(const Symbol& key, int _iLevel);
    bool clearCurrentScope(bool _bClose);
    void updateProtection(bool protect);

    std::list<Symbol>* globals;
    VarStack varStack;
    Variables variables;
    Libraries libraries;
    VarList* console;
    int m_iLevel;

    Context();
    ~Context();

    static Context* me;
};

inline std::wostream& operator<< (std::wostream& ostr, const Context &ctx)
{
    ctx.print(ostr);
    return ostr;
}

}
#endif /* !__CONTEXT_HXX__ */
