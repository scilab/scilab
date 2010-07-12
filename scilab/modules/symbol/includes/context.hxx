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

#ifndef CONTEXT_HH
# define CONTEXT_HH
#include "symbol.hxx"
#include "stack.hxx"
#include "heap.hxx"

namespace symbol
{

    /** \brief Define class Context.
    */
    class EXTERN_SYMBOL Context
    {
    public :
        Context();
        static Context* getInstance(void);

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
        InternalType*	get(const wstring& key) const;

        /** If key was associated to some Entry_T in the last opened scope, return it.
        ** Otherwise return the empty pointer. */
        InternalType*	getCurrentLevel(const wstring& key) const;

        /** If key was associated to some Entry_T in the open scopes, return the
         ** most recent insertion DESPITE the current/last one. Otherwise return the empty pointer. */
        InternalType*	getAllButCurrentLevel(const wstring& key) const;

        /** If key was associated to some Entry_T in the open scopes, return the
        ** most recent insertion. Otherwise return the empty pointer. */
        InternalType*	get_fun(const wstring& key) const;

        /*return function list in the module _stModuleName*/
        std::list<wstring>& get_funlist(const wstring& _stModuleName);


        /* global functions */

        /*return global variable visibility status*/
        bool isGlobalVisible(const wstring& key) const;

        /*return global variable, search in global scope ( highest )*/
        InternalType* getGlobalValue(const wstring& key) const;

        /*return global variable existance status*/
        bool isGlobalExists(const wstring& key) const;

        /*create or update a global variable*/
        void setGlobalValue(const wstring& key, InternalType &value);

        /*remove global variable and all visibility references */
        void removeGlobal(const wstring &key);

        /*remove all global variables and references */
        void removeGlobalAll();

        /*create an empty variable*/
        void createEmptyGlobalValue(const wstring& key);

        /*set variable visible/hidden in current global scope*/
        void setGlobalVisible(const wstring& key, bool bVisible = true);

        /*print all tables*/
        void print();

        /*add symbol and value in the stack*/
        bool put(const wstring& key, InternalType &type);
        /*add symbol and value in the previous scope*/
        bool put_in_previous_scope(const wstring& key, InternalType &type);

        /* remove symbol/value association */
        bool remove(const wstring& key);

        bool AddFunction(types::Function *_info);
        bool AddMacro(types::Macro *_info);
        bool AddMacroFile(types::MacroFile *_info);

    private :
        Stack PrivateFunTable;
        Stack PrivateVarTable;
        Heap HeapFunTable;
        Heap HeapVarTable;
        Stack EnvFunTable;
        Stack EnvVarTable;

        static Context* me;
    };
}
#endif // !CONTEXT_HH
