/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "libraries.hxx"
#include "context.hxx"

namespace symbol
{
types::MacroFile* LibBox::getMacroFile(const Symbol& _key)
{
    return m_pLib->get(_key.name_get());
}

void Libraries::put(const Symbol& _key, types::InternalType& _iT)
{
    if (_iT.isLibrary() == false)
    {
        return;
    }

    types::Library* pLib = _iT.getAs<types::Library>();

    LibMap::iterator itVar = m_libmap.find(_key);
    LibBoxList* pL = NULL;
    LibBox* pBox = NULL;

    if (itVar != m_libmap.end())
    {
        //item alreay exists

        //check scope level
        LibBoxList::reverse_iterator itBox = itVar->second->rbegin();

        bool bInserted = false;
        for (; itBox != itVar->second->rend() ; itBox++)
        {
            pBox = *itBox;
            if ((*itBox)->getValue() == pLib)
            {
                return;
            }

            //update value
            (*itBox)->setValue(*pLib);
            bInserted = true;
            break;
        }
    }
    else
    {
        //new var
        pBox = new LibBox(m_iLevel, *pLib);
        pL = new LibBoxList();
        pL->push_back(pBox);
        m_libmap[_key] = pL;
        m_liblist.push_back(_key);
    }

    return;
}

types::MacroFile* Libraries::get(const Symbol& _key) const
{
    types::MacroFile* pIT = NULL;
    LibList::const_reverse_iterator it = m_liblist.rbegin();
    for (; it != m_liblist.rend() ; ++it)
    {
        LibBoxList* l = m_libmap.find(*it)->second;
        LibBoxList::const_reverse_iterator itBox = l->rbegin();
        for (; itBox != l->rend(); ++itBox)
        {
            pIT = (*itBox)->getMacroFile(_key);
            if (pIT)
            {
                Context::getInstance()->AddMacroFile(pIT);
                return pIT;
            }
        }
    }

    return pIT;
}

bool Libraries::remove(const Symbol& _key)
{
    LibMap::iterator itVar = m_libmap.find(_key);
    if (itVar != m_libmap.end())
    {
        LibBox* pBox = itVar->second->back();
        if (pBox->getLevel() != m_iLevel)
        {
            return false;
        }

        if (pBox->getValue())
        {
            delete pBox;
        }

        itVar->second->pop_back();

        //remove empty entries from variable map
        if (itVar->second->empty())
        {
            m_liblist.remove(itVar->first);
            m_libmap.erase(itVar);
        }
        return true;
    }
    else
    {
        return false;
    }
}

std::list<std::wstring>* Libraries::getMacrosName()
{
    std::list<std::wstring>* plOut = new std::list<std::wstring>();
    types::Library* pLib = NULL;
    LibMap::const_iterator it = m_libmap.begin();

    for (; it != m_libmap.end(); it++)
    {
        pLib = it->second->back()->getValue();
        std::list<std::wstring>* tmpList = pLib->getMacrosName();
        plOut->insert(plOut->end(), tmpList->begin(), tmpList->end());
        delete tmpList;
    }

    return plOut;
}

}
