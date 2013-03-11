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

#ifndef SYMBOL_STACK_HH
#define SYMBOL_STACK_HH

#include <iostream>
#include <list>
#include <cassert>
#include "table.hxx"
#include "internal.hxx"
#include "export_symbol.h"

namespace symbol
{

/*-----------------------------------------------------------------------.
| This implements stacks for storing definitions of variables and        |
| functions as a stack of dictionnaries.  Each time a scope is opened, a |
| new dictionnary is added on the top of the stack; the dictionary is    |
| removed when the scope is closed.  Lookup of symbols is donne in the   |
| last added dictionnary first (LIFO).                                   |
`-----------------------------------------------------------------------*/
class EXTERN_SYMBOL Stack : public Table
{
public:
    /** Open a new scope */
    void scope_begin()
    {
        this->l_scope.push_front(new Scope());
    }

    /** Close the last scope, forgetting everything since the latest
    **	scope_begin (). */
    void scope_end()
    {
        Scope* scope = this->l_scope.front();
        delete scope;
        this->l_scope.pop_front();
    }

    void putInPreviousScope(const symbol::Symbol& key, types::InternalType &value)
    {
        if (l_scope.size() > 1)
        {
            std::list<Scope*>::iterator i;
            i = l_scope.begin();
            i++;
            (*i)->put(key, value);
        }
    }

    /** Associate value to key in the current scope. */
    void put(const symbol::Symbol& key, types::InternalType &value)
    {
        (this->l_scope.front())->put(key, value);
    }

    /** Remove Association between value and key in the current scope. */
    void remove(const symbol::Symbol& key)
    {
        (this->l_scope.front())->remove(key);
    }

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion. Otherwise return the empty pointer. */
    types::InternalType* get(const symbol::Symbol& key) const
    {
        types::InternalType* result = NULL;

        std::list<Scope*>::const_iterator it_list_scope = this->l_scope.begin();

        for (; it_list_scope != this->l_scope.end(); it_list_scope++)
        {
            result = (*it_list_scope)->get(key);
            if (result)
            {
                return result;
            }
        }
        return result;
    }

    /** If key was associated to some Entry_T in the open scopes, return the
    ** most recent insertion DESPITE the current/last one. Otherwise return the empty pointer. */
    types::InternalType* getAllButCurrentLevel(const symbol::Symbol& key) const
    {
        types::InternalType* result = 0;

        /*
        ** If we are in the top level scope, also look within it
        */
        if (l_scope.size() == 1)
        {
            return l_scope.front()->get(key);
        }

        /*
        ** else look in all previous scopes but not in the current one.
        */
        std::list<Scope*>::const_iterator it_list_scope = this->l_scope.begin();
        ++it_list_scope;
        for ( ; it_list_scope != this->l_scope.end(); ++it_list_scope)
        {
            result = (*it_list_scope)->get(key);
            if (result == 0)
            {
                continue ;
            }
            return result;
        }
        return result;
    }

    /** If key was associated to some Entry_T in the last opened scope, return it.
    ** Otherwise return the empty pointer. */
    types::InternalType* getCurrentLevel(const symbol::Symbol& key) const
    {
        return l_scope.front()->get(key);
    }


    std::list<symbol::Symbol>& getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd)
    {
        //get hightest scope
        std::list<Scope*>::iterator i;

        if (_bFromEnd)
        {
            i = l_scope.end();
            i--;
        }
        else
        {
            i = l_scope.begin();
        }

        return (*i)->get_names(_stModuleName);
    }
};

inline std::wostream& operator<< (std::wostream& ostr, const Stack &tbl)
{
    tbl.print (ostr);
    return ostr;
}
}


#endif /* ! SYMBOL_STACK_HH */

