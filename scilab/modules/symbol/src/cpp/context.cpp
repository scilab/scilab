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

#include "context.hxx"

namespace symbol
{
    Context* Context::me;

    Context::Context()
    {
        PrivateFunTable.scope_begin();
        PrivateVarTable.scope_begin();
        HeapFunTable.scope_begin();
        HeapVarTable.scope_begin();
        EnvFunTable.scope_begin();
        EnvVarTable.scope_begin();
    }

    Context* Context::getInstance(void)
    {
        if (me == 0)
        {
            me = new Context();
        }
        return me;
    }

    void Context::scope_begin() {
        PrivateFunTable.scope_begin();
        PrivateVarTable.scope_begin();
        HeapFunTable.scope_begin();
        HeapVarTable.scope_begin();
        EnvFunTable.scope_begin();
        EnvVarTable.scope_begin();
    }

    void Context::scope_end() {
        PrivateFunTable.scope_end();
        PrivateVarTable.scope_end();
        EnvFunTable.scope_end();
        EnvVarTable.scope_end();
        HeapFunTable.scope_end();
        HeapVarTable.scope_end();
    }

    InternalType*	Context::get(const string& key) const
    {
        // FIXME
        InternalType* pI = NULL;
        pI = EnvVarTable.get(key);

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = EnvFunTable.get(key);
            if(pI != NULL)
            {
                return pI;
            }
            else
            {
                return NULL;
            }
        }
    }

    InternalType*	Context::getCurrentLevel(const string& key) const
    {
        // FIXME
        InternalType* pI = NULL;
        pI = EnvVarTable.getCurrentLevel(key);

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = EnvFunTable.getCurrentLevel(key);
            if(pI != NULL)
            {
                return pI;
            }
            else
            {
                return NULL;
            }
        }
    }

    InternalType*	Context::getAllButCurrentLevel(const string& key) const
    {
       // FIXME
        InternalType* pI = NULL;
        pI = EnvVarTable.getAllButCurrentLevel(key);

        if(pI != NULL)
        {
            return pI;
        }
        else
        {
            pI = EnvFunTable.getAllButCurrentLevel(key);
            if(pI != NULL)
            {
                return pI;
            }
            else
            {
                return NULL;
            }
        }
    }

    InternalType*	Context::get_fun(const string& key) const
    {
        return EnvFunTable.get(key);
        // FIXME
    }

    std::list<string>& Context::get_funlist(const string& _stModuleName)
    {
        return EnvFunTable.get_funlist(_stModuleName);
    }

    bool Context::put(const string& key, InternalType &type)
    {
        // FIXME
        EnvVarTable.put(key, type);
        return true;
    }

    bool Context::remove(const string& key)
    {
        // First look in Variables Environment
        if (EnvVarTable.getCurrentLevel(key) == NULL)
        {
            // If not found, look in Functions Environment
            if (EnvFunTable.getCurrentLevel(key) == NULL)
            {
                return false;
            }
            else
            {
                EnvFunTable.remove(key);
                return true;
            }
        }

        EnvVarTable.remove(key);
        return true;
    }

    bool Context::put_in_previous_scope(const string& key, InternalType &type)
    {
        // FIXME
        EnvVarTable.put_in_previous_scope(key, type);
        return true;
    }

    void Context::print()
    {
        std::cout << "PrivateFunTable : " << std::endl;
        std::cout << PrivateFunTable << std::endl << std::endl;
        std::cout << "PrivateVarTable : " << std::endl;
        std::cout << PrivateVarTable << std::endl << std::endl;
        std::cout << "EnvFunTable : " << std::endl;
        std::cout << EnvFunTable << std::endl << std::endl;
        std::cout << "EnvVarTable : " << std::endl;
        std::cout << EnvVarTable << std::endl << std::endl;
        std::cout << "HeapFunTable : " << std::endl;
        std::cout << HeapFunTable << std::endl << std::endl;
        std::cout << "HeapVarTable : " << std::endl;
        std::cout << HeapVarTable << std::endl << std::endl;
        //FIXME
    }

    bool Context::AddFunction(types::Function *_info)
    {
        EnvFunTable.put(_info->getName(), *_info);
        return true;
    }

    bool Context::AddMacro(types::Macro *_info)
    {
        EnvFunTable.put(_info->getName(), *_info);
        return true;
    }

    bool Context::AddMacroFile(types::MacroFile *_info)
    {
        EnvFunTable.put(_info->getName(), *_info);
        return true;
    }
}

