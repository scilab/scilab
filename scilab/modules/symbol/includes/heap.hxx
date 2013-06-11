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

#ifndef SYMBOL_HEAP_HH
#define SYMBOL_HEAP_HH

#include <iostream>
#include <list>
#include <map>
#include "symbol.hxx"
#include "table.hxx"

#include "bool.hxx"
#include "double.hxx"

namespace symbol
{

/*-----------------------------------------------------------------------.
| This implements heaps for storing definitions of variables and         |
| functions as a list of dictionnaries.  Each time a scope is opened, a  |
| new dictionnary is added on the top of the heap; the dictionary is     |
| removed when the scope is closed.  Lookup of symbols is donne in the   |
| last added dictionnary first (LIFO).                                   |
`-----------------------------------------------------------------------*/
class EXTERN_SYMBOL Heap
{
private :
    types::Bool *m_True;
    typedef std::list<Scope*> ListScope;
    typedef std::map<Symbol, ListScope* > MapScope;
    MapScope namespaces;
    ListScope globals;
public:
    /**

    */
    Heap()
    {
        scope_begin();
        m_True = new types::Bool(1);
        m_True->IncreaseRef();
    }

    void namespace_add(const symbol::Symbol& name)
    {
        namespaces[name] = new ListScope;
    }

    void scope_begin()
    {
        globals.push_front(new Scope());
        MapScope::iterator it = namespaces.begin();
        for (; it != namespaces.end() ; it++)
        {
            it->second->push_front(new Scope());
        }
    }

    void scope_end()
    {
        delete globals.front();
        globals.pop_front();
        MapScope::iterator it = namespaces.begin();
        for (; it != namespaces.end() ; it++)
        {
            delete it->second->front();
            it->second->pop_front();
        }
    }

    void put(const symbol::Symbol& key, types::InternalType &value)
    {
        globals.back()->put(key, value);
    }

    void put(const symbol::Symbol& name, const symbol::Symbol& key, types::InternalType &value)
    {
        MapScope::iterator scope = namespaces.find(name);
        if (scope == namespaces.end())
        {
            //create namespace if does not exist
            namespace_add(name);
            scope = namespaces.find(name);
        }

        scope->second->back()->put(key, value);
    }

    types::InternalType* get(const symbol::Symbol& key) const
    {
        return globals.back()->get(key);
    }

    types::InternalType* get(const symbol::Symbol& name, const symbol::Symbol& key) const
    {
        MapScope::const_iterator scope = namespaces.find(name);
        if (scope != namespaces.end())
        {
            return scope->second->back()->get(key);
        }
        return NULL;
    }

    bool isGlobalVisible(const symbol::Symbol& key) const
    {
        if (globals.empty())
        {
            return false;
        }

        ListScope::const_iterator it;
        if (globals.front()->get(key))
        {
            return true;
        }
        //for (it = globals.begin() ; it != globals.end() ; it++)
        //{
        //    if((*it)->get(key))
        //    {
        //        return true;
        //    }
        //}
        return false;
    }

    bool isGlobalExists(const symbol::Symbol& key) const
    {
        if (getGlobalValue(key))
        {
            return true;
        }
        return false;
    }

    types::InternalType* getGlobalValue(const symbol::Symbol& key) const
    {
        return globals.back()->get(key);
    }

    void setGlobalValue(const symbol::Symbol& key, types::InternalType &value)
    {
        globals.back()->put(key, value);
    }

    void createEmptyGlobalValue(const symbol::Symbol& key)
    {
        setGlobalValue(key, *types::Double::Empty());
    }


    void setGlobalVisible(const symbol::Symbol& key, bool bVisible)
    {
        if (bVisible)
        {
            globals.front()->put(key, *m_True);
        }
        else
        {
            globals.front()->remove(key);
        }
    }

    void removeGlobal(const symbol::Symbol& key)
    {
        ListScope::const_iterator it;
        for (it = globals.begin() ; it != globals.end() ; it++)
        {
            (*it)->remove(key);
        }
    }

    void removeGlobalAll()
    {
        ListScope::const_iterator it;
        for (it = globals.begin() ; it != globals.end() ; it++)
        {
            std::map<symbol::Symbol, types::InternalType*>::const_iterator it_scope;
            std::map<symbol::Symbol, types::InternalType*>* pScope = (*it)->getInternalMap();

            it_scope = pScope->begin();
            while (it_scope != pScope->end())
            {
                (*it)->remove(it_scope->first);
                it_scope = pScope->begin();
            }
        }
    }

    void print(std::wostream& ostr) const
    {
        //print globals
        ListScope::const_iterator it;
        for (it = globals.begin() ; it != globals.end() ; it++)
        {
            (*it)->print(ostr);
        }

        //print namespace
        MapScope::const_iterator it_namespace;
        ListScope::const_iterator it_scope;
        for (it_namespace = namespaces.begin() ; it_namespace != namespaces.end() ; it_namespace++)
        {
            print(it_namespace->first, ostr);
        }

    }

    void print(const Symbol& name, std::wostream& ostr) const
    {
        MapScope::const_iterator scope = namespaces.find(name);
        if (scope == namespaces.end())
        {
            ostr << name.name_get() << L" not found" << std::endl;
            return;
        }

        ListScope::const_iterator it;

        ostr << scope->first.name_get() << "L : " << std::endl;
        for (it = scope->second->begin() ; it != scope->second->end() ; it++)
        {
            (*it)->print(ostr);
        }
    }

};

inline std::wostream& operator<< (std::wostream& ostr, const Heap &tbl)
{
    tbl.print (ostr);
    return ostr;
}
}


#endif /* ! SYMBOL_HEAP_HH */

