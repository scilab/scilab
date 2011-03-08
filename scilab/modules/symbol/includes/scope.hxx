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

#ifndef __SCOPE_HXX__
#define __SCOPE_HXX__

#include <iostream>
#include <map>
#include <list>
//#include "internal.hxx"
#include "double.hxx"
#include "callable.hxx"
#include "export_symbol.h"

extern "C"
{
#include "charEncoding.h"
#include "MALLOC.h"
}

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
            _scope = new std::map<symbol::Symbol, types::InternalType*>();
            _name = L"";
        }
        /** \brief Construct a named Scope i.e Namespace */
        explicit Scope(std::wstring name)
        {
            _scope = new std::map<symbol::Symbol, types::InternalType*>();
            _name = name;
        }

        ~Scope()
        {
            std::map<symbol::Symbol, types::InternalType*>::const_iterator i;
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

        bool isUsed(types::InternalType* pIT) const
        {
            std::map<symbol::Symbol, types::InternalType*>::const_iterator it_scope;
            for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
            {
                if((*it_scope).second == pIT)
                    return true;
            }
            return false;
        }

        /** Associate value to key in the current scope. */
        types::InternalType*	put (const symbol::Symbol& key, types::InternalType &value)
        {
            types::InternalType *pOld = (*_scope)[key];

            if(pOld == &value)
            {
                return NULL;
            }

            /*increase ref before delete old value to prevent pointed variable deletion*/
            /*a = {1,2,3}*/
            /*a = a{1}*/
            value.IncreaseRef();

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
            return NULL;
        }

        void remove(const symbol::Symbol& key)
        {
            if((*_scope).find(key) != (*_scope).end())
            {
                types::InternalType *pOld = (*_scope)[key];

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
        }

        /** If key was associated to some Entry_T in the open scopes, return the
        ** most recent insertion. Otherwise return the empty pointer. */
        types::InternalType*	get (const symbol::Symbol& key) const
        {
            std::map<symbol::Symbol, types::InternalType*>::const_iterator it_scope;

            it_scope = (*_scope).find(key);
            if (it_scope == (*_scope).end())
                return 0;
            return (*it_scope).second;
        }

        /** Return name of current scope, use for namespace. */
        std::wstring get_name() const
        {
            return _name;
        }

        /** Return name of current scope, use for namespace. */
        std::list<symbol::Symbol>& get_names(const std::wstring& _stModuleName) const
        {
            std::list<symbol::Symbol>* pNames = new std::list<symbol::Symbol>;
            std::map<symbol::Symbol, types::InternalType*>::const_iterator it_scope;
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
        void	print (std::wostream& ostr) const
        {
            std::map<symbol::Symbol, types::InternalType*>::const_iterator it_scope;
            for(it_scope = _scope->begin() ; it_scope != _scope->end() ; ++it_scope)
            {
                ostr << it_scope->first.name_get() << " = " << it_scope->second->toString(10,75) << std::endl;
            }
        }

        std::map<symbol::Symbol, types::InternalType*>* getInternalMap()
        {
            return _scope;
        }

    private:
        std::map<symbol::Symbol, types::InternalType*>* 	_scope;
        std::wstring				            _name;
    };

    inline std::wostream& operator<< (std::wostream& ostr,
        const Scope &scope)
    {
        scope.print(ostr);
        return ostr;
    }
}


#endif /* ! __SCOPE_HX__ */

