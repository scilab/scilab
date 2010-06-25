/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef SYMBOL_SCOPE_HH
# define SYMBOL_SCOPE_HH

#include <iostream>
#include <map>
#include "symbol.hxx"
#include "alltypes.hxx"
#include "export_symbol.h"

using namespace types;

namespace symbol
{

    //template< class Entry_T, class Key_T = Symbol >
    class EXTERN_SYMBOL Scope
    {
    public:
        /** Constructor & Destructor */
        /** \brief Construct a Scope */
        explicit Scope()
        {
            _scope = new map<string, InternalType*>();
            _name = "";
        }
        /** \brief Construct a named Scope i.e Namespace */
        explicit Scope(string name)
        {
            _scope = new map<string, InternalType*>();
            _name = name;
        }

        ~Scope()
        {
            map<string, InternalType*>::const_iterator i;
            for(i = _scope->begin() ; i != _scope->end() ; ++i)
            {
                //std::cout << i->first << std::endl;
                i->second->DecreaseRef();
                if(i->second->isDeletable())
                {
                    //					std::cout << "--- DELETE : " << i->first << " " << i->second << " " << i->second->getRef() << std::endl;
                    delete i->second;
                }
                else
                {
                    //					std::cout << "--- KEEP : " << i->first << " " << i->second << " " << i->second->getRef() << std::endl;
                }
            }
            delete _scope;
        }

        bool isUsed(InternalType* pIT) const
        {
            map<string, InternalType*>::const_iterator it_scope;
            for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
            {
                if((*it_scope).second == pIT)
                    return true;
            }
            return false;
        }

        /** Associate value to key in the current scope. */
        InternalType*	put (const string& key, InternalType &value)
        {
            InternalType *pOld = (*_scope)[key];

            if(pOld == &value)
            {
                return NULL;
            }

            if(pOld != NULL)
            {
                pOld->DecreaseRef();
                if(pOld->isDeletable() == true)
                {
                    delete pOld;
                    pOld = NULL;
                }
            }
            /*
            if(value.isRef())
            {
            cout << "Need copy : " << key.name_get() << endl;
            }
            */
            (*_scope)[key] = &value;
            value.IncreaseRef();
            return NULL;
        }

        void remove(const string& key)
        {
            InternalType *pOld = (*_scope)[key];
            
            if(pOld)
            {
                _scope->erase(key);
                pOld->DecreaseRef();
                if(pOld->isDeletable() == true)
                {
                    delete pOld;
                    pOld = NULL;
                }
            }
        }

        /** If key was associated to some Entry_T in the open scopes, return the
        ** most recent insertion. Otherwise return the empty pointer. */
        InternalType*	get (const string& key) const
        {
            map<string, InternalType*>::const_iterator it_scope;

            it_scope = (*_scope).find(key);
            if (it_scope == (*_scope).end())
                return 0;
            return (*it_scope).second;
        }

        /** Return name of current scope, use for namespace. */
        string get_name() const
        {
            return _name;
        }

        /** Return name of current scope, use for namespace. */
        list<string>& get_names(const string& _stModuleName) const
        {
            list<string>* pNames = new list<string>;
            map<string, InternalType*>::const_iterator it_scope;
            for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
            {
                if(it_scope->second->isFunction() || it_scope->second->isMacro() || it_scope->second->isMacroFile())
                {
                    types::Callable* pC = it_scope->second->getAsCallable();
                    if(pC->getModule() == _stModuleName)
                    {
                        pNames->push_back(it_scope->first);
                    }
                }
            }

            return *pNames;
        }

        /** Send the content of this table on ostr in a readable manner, the top
        ** of the stack being displayed last. */
        void	print (ostream& ostr) const
        {
            map<string, InternalType*>::const_iterator it_scope;
            for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
            {
                ostr << it_scope->first << " = " << it_scope->second->toString(10,75) << std::endl;
            }
        }

        map<string, InternalType*>* getInternalMap()
        {
            return _scope;
        }

    private:
        map<string, InternalType*>* 	_scope;
        string				_name;
    };

    inline ostream& operator<< (ostream& ostr,
        const Scope &scope)
    {
        scope.print(ostr);
        return ostr;
    }
}


#endif /* ! SYMBOL_SCOPE_HH */

