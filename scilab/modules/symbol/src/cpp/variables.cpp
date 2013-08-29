/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "variables.hxx"
#include "callable.hxx"
#include "double.hxx"

namespace symbol
{
VarBox::VarBox(int _iLevel, types::InternalType* _pIT, bool _bGlobal, bool _bGlobalVisible)
    : m_iLevel(_iLevel), m_pIT(_pIT), m_bGlobal(_bGlobal), m_bGlobalVisible(_bGlobalVisible)
{
    if (m_pIT)
    {
        m_pIT->IncreaseRef();
    }
}

void Variables::put(const Symbol& _key, types::InternalType& _pIT)
{
    put(_key, _pIT, m_iLevel);
}

void Variables::put(const Symbol& _key, types::InternalType& _iT, int _iLevel)
{
    if (isGlobalVisible(_key))
    {
        setGlobalValue(_key, _iT);
        return;
    }
    VarMap::iterator itVar = m_vars.find(_key);
    VarBoxList* pL = NULL;
    VarBox* pBox = NULL;

    if (itVar != m_vars.end())
    {
        //item alreay exists

        //check scope level
        VarBoxList::reverse_iterator itBox = itVar->second->rbegin();

        bool bInserted = false;
        for (; itBox != itVar->second->rend() ; itBox++)
        {
            pBox = *itBox;
            if ((*itBox)->m_iLevel > _iLevel)
            {
                continue;
            }
            else if ((*itBox)->m_iLevel < _iLevel)
            {
                pBox = new VarBox(_iLevel, &_iT);
                itVar->second->insert(itBox.base(), pBox);
                bInserted = true;
                break;
            }
            else if ((*itBox)->m_iLevel == _iLevel)
            {
                if ((*itBox)->m_pIT == &_iT)
                {
                    return;
                }

                //increase ref
                _iT.IncreaseRef();

                (*itBox)->m_pIT->DecreaseRef();
                if ((*itBox)->m_pIT->isDeletable())
                {
                    delete (*itBox)->m_pIT;
                }

                //update value
                (*itBox)->m_pIT = &_iT;
                bInserted = true;
                break;
            }
        }

        if (bInserted == false)
        {
            //for case of first var ref is in higher scope
            pBox = new VarBox(_iLevel, &_iT);
            itVar->second->push_front(pBox);
        }
    }
    else
    {
        //new var
        pBox = new VarBox(_iLevel, &_iT);
        pL = new VarBoxList();
        pL->push_back(pBox);
        m_vars[_key] = pL;
    }

    return;
}

void Variables::putInPreviousScope(const Symbol& _key, types::InternalType& _iT)
{
    put(_key, _iT, Max(0, m_iLevel - 1));
}

types::InternalType* Variables::get(const Symbol& _key) const
{
    if (isGlobalExists(_key) && isGlobalVisible(_key))
    {
        return getGlobalValue(_key);
    }
    else
    {
        VarMap::const_iterator itVar = m_vars.find(_key);
        if (itVar != m_vars.end())
        {
            return itVar->second->back()->m_pIT;
        }
        else
        {
            return NULL;
        }
    }
}

types::InternalType* Variables::getCurrentLevel(const Symbol& _key) const
{
    if (isGlobalExists(_key) && isGlobalVisible(_key))
    {
        return getGlobalValue(_key);
    }
    else
    {
        VarMap::const_iterator itVar = m_vars.find(_key);
        if (itVar != m_vars.end() && itVar->second->back()->m_iLevel == m_iLevel)
        {
            return itVar->second->back()->m_pIT;
        }
        else
        {
            return NULL;
        }
    }
}

types::InternalType* Variables::getAllButCurrentLevel(const Symbol& _key) const
{
    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBoxList* pL = itVar->second;
        VarBoxList::iterator itBox = pL->begin();
        for (; itBox != pL->end() ; itBox++)
        {
            if ((*itBox)->m_iLevel < m_iLevel)
            {
                return (*itBox)->m_pIT;
            }
        }
    }

    return NULL;
}

types::InternalType* Variables::getInSpecificLevel(const Symbol& _key, int _iLevel) const
{
    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBoxList* pL = itVar->second;
        VarBoxList::iterator itBox = pL->begin();
        for (; itBox != pL->end() ; itBox++)
        {
            if ((*itBox)->m_iLevel == _iLevel)
            {
                return (*itBox)->m_pIT;
            }
        }
    }

    return NULL;
}

bool Variables::remove(const Symbol& _key)
{
    VarMap::iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBox* pBox = itVar->second->back();
        if (pBox->m_iLevel != m_iLevel)
        {
            return false;
        }

        if (pBox->m_pIT)
        {
            pBox->m_pIT->DecreaseRef();
            if (pBox->m_pIT->isDeletable())
            {
                delete pBox->m_pIT;
            }

            delete pBox;
        }

        itVar->second->pop_back();

        //remove empty entries from variable map
        if (itVar->second->empty())
        {
            m_vars.erase(itVar);
        }
        return true;
    }
    else
    {
        return false;
    }

}

std::list<std::wstring>& Variables::getVarsName()
{
    std::list<std::wstring>* plOut = new std::list<std::wstring>();
    types::InternalType* pIT = NULL;
    VarMap::const_iterator it = m_vars.begin();

    for (; it != m_vars.end(); it++)
    {
        pIT = it->second->front()->m_pIT;
        if (pIT && pIT->isCallable() == false)
        {
            plOut->push_back(it->first.name_get().c_str());
        }
    }

    return *plOut;
}

std::list<std::wstring>& Variables::getMacrosName()
{
    std::list<std::wstring>* plOut = new std::list<std::wstring>();
    types::InternalType* pIT = NULL;
    VarMap::const_iterator it = m_vars.begin();

    for (; it != m_vars.end(); it++)
    {
        pIT = it->second->front()->m_pIT;
        if (pIT && (pIT->isMacro() || pIT->isMacroFile()))
        {
            plOut->push_back(it->first.name_get().c_str());
        }
    }

    return *plOut;
}

std::list<std::wstring>& Variables::getFunctionsName()
{
    std::list<std::wstring>* plOut = new std::list<std::wstring>();
    types::InternalType* pIT = NULL;
    VarMap::const_iterator it = m_vars.begin();

    for (; it != m_vars.end(); it++)
    {
        pIT = it->second->front()->m_pIT;
        if (pIT && pIT->isFunction())
        {
            plOut->push_back(it->first.name_get().c_str());
        }
    }

    return *plOut;
}

std::list<symbol::Symbol>& Variables::getFunctionList(const std::wstring& _stModuleName, bool _bFromEnd) const
{
    bool bAll = _stModuleName == L"";
    std::list<symbol::Symbol>* lst = new std::list<symbol::Symbol>;

    VarMap::const_iterator it = m_vars.begin();
    for (; it != m_vars.end(); it++)
    {
        types::InternalType* pIT = NULL;
        if (it->second->size() == 0)
        {
            continue;
        }

        if (_bFromEnd)
        {
            pIT = it->second->front()->m_pIT;
        }
        else
        {
            VarBox* pBox = it->second->back();
            if (pBox->m_iLevel != m_iLevel)
            {
                continue;
            }

            pIT = pBox->m_pIT;
        }

        //exclude globals
        if (pIT == NULL)
        {
            continue;
        }

        if (pIT->isCallable() == false)
        {
            continue;
        }

        if (bAll)
        {
            lst->push_back(it->first);
            continue;
        }

        types::Callable* pC = pIT->getAs<types::Callable>();
        if (_stModuleName == pC->getModule())
        {
            lst->push_back(it->first);
        }

    }
    return *lst;
}

bool Variables::isGlobalVisible(const symbol::Symbol& _key) const
{
    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBoxList* pL = itVar->second;
        VarBox* pBox = pL->back();
        if (pBox->m_iLevel == m_iLevel && pBox->m_bGlobal && pBox->m_bGlobalVisible)
        {
            return true;
        }
    }

    return false;
}

void Variables::setGlobalVisible(const symbol::Symbol& _key, bool bVisible)
{
    VarBox* pBox = NULL;
    VarBoxList* pL = NULL;

    VarMap::const_iterator itVar = m_vars.find(_key);
    if (itVar != m_vars.end())
    {
        VarBoxList* pL = itVar->second;
        pBox = pL->back();
        if (pBox->m_iLevel == m_iLevel)
        {
            //update

            pBox->m_bGlobalVisible = true;
            pBox->m_bGlobal = true;
            //clear existing local var
            pBox->m_pIT->DecreaseRef();
            if (pBox->m_pIT->isDeletable())
            {
                delete pBox->m_pIT;
                pBox->m_pIT = NULL;
            }
        }
        else
        {
            //add in existing VarBoxList
            pBox = new VarBox(m_iLevel, NULL);
            pBox->m_bGlobalVisible = true;
            pBox->m_bGlobal = true;
            pL->push_back(pBox);
        }
    }
    else
    {
        //create a new VarBoxList
        pBox = new VarBox(m_iLevel, NULL);
        pBox->m_bGlobalVisible = true;
        pBox->m_bGlobal = true;
        pL = new VarBoxList();
        pL->push_back(pBox);
        m_vars[_key] = pL;
    }
}

void Variables::removeGlobal(const symbol::Symbol& _key)
{
    GlobalMap::iterator it = m_globals.find(_key);
    if (it != m_globals.end())
    {
        removeGlobal(it);
    }
}

void Variables::removeGlobal(GlobalMap::iterator& _it)
{
    //remove all refs in scopes
    Symbol key = _it->first;

    VarMap::iterator itVar = m_vars.find(key);
    if (itVar != m_vars.end())
    {
        VarBoxList* pLBox = itVar->second;
        VarBoxList::iterator itBox = pLBox->begin();
        for (; itBox != pLBox->end() ; itBox++)
        {
            VarBox* pBox = (*itBox);
            if (pBox->m_bGlobal == true && pBox->m_bGlobalVisible == true)
            {
                pLBox->erase(itBox);
                //itBox iterator becomes invalid, reinit it
                itBox = pLBox->begin();
                if (itBox == pLBox->end())
                {
                    break;
                }
            }
        }

        if (pLBox->empty())
        {
            m_vars.erase(key);
        }
    }

    _it->second->DecreaseRef();
    if (_it->second->isDeletable())
    {
        delete _it->second;
    }

    m_globals.erase(_it);
}

void Variables::removeGlobalAll()
{
    //for all globals, remove them and all references in scopes
    GlobalMap::iterator it;
    while ((it = m_globals.begin()) != m_globals.end())
    {
        removeGlobal(it);
    }
}

void Variables::createEmptyGlobalValue(const symbol::Symbol& _key)
{
    types::InternalType* pIT = types::Double::Empty();
    pIT->IncreaseRef();
    m_globals[_key] = pIT;
}

bool Variables::isGlobalExists(const symbol::Symbol& _key) const
{
    return (m_globals.find(_key) != m_globals.end());
}

types::InternalType* Variables::getGlobalValue(const symbol::Symbol& _key) const
{
    GlobalMap::const_iterator it = m_globals.find(_key);
    if (it != m_globals.end())
    {
        return it->second;
    }
    else
    {
        return NULL;
    }
}

void Variables::setGlobalValue(const symbol::Symbol& _key, types::InternalType& _value)
{
    GlobalMap::iterator it = m_globals.find(_key);
    if (it != m_globals.end())
    {
        if (it->second != NULL)
        {
            it->second->DecreaseRef();
            if (it->second->isDeletable())
            {
                delete it->second;
            }
        }

        _value.IncreaseRef();
        m_globals[_key] = &_value;
    }
}

}
