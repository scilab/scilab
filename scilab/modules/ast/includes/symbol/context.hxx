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

/**
 ** \file symbol/context.hh
 ** \brief Define class Context.
 */

#ifndef __CONTEXT_HXX__
#define __CONTEXT_HXX__

#include "function.hxx"
#include "typesdecl.hxx"
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
public :
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

    /** If key was associated to some Entry_T in the last opened scope, return it.
    ** Otherwise return the empty pointer. */
    types::InternalType* getCurrentLevel(const Symbol& key);

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion DESPITE the current/last one. Otherwise return the empty pointer. */
    types::InternalType* getAllButCurrentLevel(const Symbol& key);

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion. Otherwise return the empty pointer. */
    types::InternalType* getFunction(const Symbol& key);

    /*return function list in the module _stModuleName*/
    std::list<Symbol>* getFunctionList(std::wstring _stModuleName);

    std::list<std::wstring>* getVarsName();
    std::list<std::wstring>* getMacrosName();
    std::list<std::wstring>* getFunctionsName();

    /* global functions */

    /*return global variable visibility status*/
    bool isGlobalVisible(const Symbol& key);

    /*return global variable existance status*/
    bool isGlobal(const Symbol& key);

    /*remove global variable and all visibility references */
    //clearglobal("a")
    void removeGlobal(const Symbol& key);

    /*remove all global variables and references */
    //clearglobal
    void removeGlobalAll();

    void clearAll();

    /*set variable visible/hidden in current global scope*/
    void setGlobalVisible(const Symbol& key, bool bVisible);
    void setGlobal(const Symbol& key);

    types::InternalType* getGlobalValue(const Symbol& _key);

    /*add symbol and value in the stack*/
    void put(const Symbol& _key, types::InternalType* _pIT);
    void put(Variable* _var, types::InternalType* _pIT);
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
    void print(std::wostream& ostr) const;
    int getScopeLevel();

private :

    types::InternalType* get(const Symbol& key, int _iLevel);
    bool clearCurrentScope(bool _bClose);

    std::list<Symbol>* globals;
    VarStack varStack;
    Variables variables;
    Libraries libraries;
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
