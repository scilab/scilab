/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "inspector.hxx"
#include "types.hxx"

extern "C"
{
#include <stdio.h>
}

namespace types
{
#ifndef NDEBUG
std::vector<InternalType*> Inspector::m_vIT;

int Inspector::getItemCount()
{
    return (int)m_vIT.size();
}

int Inspector::getUnreferencedItemCount()
{
    int iCount = 0;
    for (int i = 0 ; i < m_vIT.size() ; i++)
    {
        if (m_vIT[i]->getRef() == 0)
        {
            iCount++;
        }
    }
    return iCount;
}


void Inspector::addItem(InternalType* _pIT)
{
    types::GenericType* pGT = _pIT->getAs<types::GenericType>();

    if (pGT)
    {
        //std::wcout << L"addItem " << pGT->getTypeStr() << L"[" << pGT->getSize() << L"] : " << pGT << std::endl;
    }
    else
    {
        //std::wcout << L"addItem " << _pIT->getTypeStr() << L" : " << _pIT << std::endl;
    }
    m_vIT.push_back(_pIT);
}

void Inspector::removeItem(InternalType* _pIT)
{
    std::vector<InternalType*>::iterator it;

    for (it = m_vIT.begin() ; it != m_vIT.end() ; it++)
    {
        if ((*it) == _pIT)
        {
            types::GenericType* pGT = _pIT->getAs<types::GenericType>();

            if (pGT)
            {
                //std::wcout << L"removeItem " << pGT->getTypeStr() << L"[" << pGT->getSize() << L"] : " << pGT << std::endl;
            }
            else
            {
                //std::wcout << L"removeItem " << _pIT->getTypeStr() << L" : " << _pIT << std::endl;
            }
            m_vIT.erase(it);
            break;
        }
    }
}

InternalType* Inspector::getItem(int _iPos)
{
    if (_iPos >= m_vIT.size())
    {
        return NULL;
    }
    return m_vIT[_iPos];
}

InternalType* Inspector::getUnreferencedItem(int _iPos)
{
    int iCount = 0;
    for (int i = 0 ; i < m_vIT.size() ; i++)
    {
        if (m_vIT[i]->getRef() == 0)
        {
            if (iCount == _iPos)
            {
                std::wcout << L"getUnreferencedItem : " << m_vIT[i] << std::endl;
                return m_vIT[i];
            }
            iCount++;
        }
    }

    return NULL;
}

std::wstring Inspector::showItem(int _iPos)
{
    std::wstring st;
    InternalType* pIT = getItem(_iPos);
    if (pIT == NULL)
    {
        st = L"NULL";
    }
    else
    {
        st = pIT->getTypeStr();
    }
    return st;
}

std::wstring Inspector::showUnreferencedItem(int _iPos)
{
    std::wstring st;
    InternalType* pIT = getUnreferencedItem(_iPos);
    if (pIT == NULL)
    {
        st = L"NULL";
    }
    else
    {
        st = pIT->getTypeStr();
    }
    return st;
}

void Inspector::deleteItems()
{
    InternalType** pIT = new InternalType*[m_vIT.size()];

    //copy item values
    for (int i = 0 ; i < m_vIT.size() ; i++)
    {
        pIT[i] = m_vIT[i];
    }

    //delete each item
    for (int i = 0 ; i < m_vIT.size() ; i++)
    {
        delete pIT[i];
    }

    //check vector update
    if (m_vIT.size() != 0)
    {
        printf("Oo\n");
    }
}
#endif
}
