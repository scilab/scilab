/*
*  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*
*/

#include <string>
#include <iostream>
#include <map>
#include <unordered_set>

#include "inspector.hxx"
#include "types.hxx"
#include "string.hxx"

namespace types
{
#ifndef NDEBUG
std::unordered_set<InternalType*> Inspector::m_vIT;
// InternalType instances may be created or destroyed concurrently by the
// interpreter and external parser services (for example the Monaco LSP).
// Protect the debug registry against concurrent insertions, removals and
// iterations. The mutex is recursive because deleteItems() deletes registered
// objects whose destructors call removeItem().
std::recursive_mutex Inspector::m_mutex;

size_t Inspector::getItemCount()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    return m_vIT.size();
}

size_t Inspector::getUnreferencedItemCount()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    int iCount = 0;
    for (auto it : m_vIT)
    {
        if (it->getRef() == 0)
        {
            iCount++;
        }
    }
    return iCount;
}


void Inspector::addItem(InternalType* _pIT)
{
    types::GenericType* pGT = _pIT->getAs<types::GenericType>();
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_vIT.insert(pGT);
}

void Inspector::removeItem(InternalType* _pIT)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    m_vIT.erase(_pIT);
}

InternalType* Inspector::getItem(size_t _iPos)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    if (m_vIT.size() > _iPos)
    {
        return *std::next(m_vIT.begin(), _iPos);
    }

    return NULL;
}

InternalType* Inspector::getUnreferencedItem(size_t _iPos)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    size_t iCount = 0;
    for (auto it : m_vIT)
    {
        if (it->getRef() == 0)
        {
            if (iCount == _iPos)
            {
                std::wcout << L"getUnreferencedItem : " << it << std::endl;
                return it;
            }
            iCount++;
        }
    }

    return NULL;
}

std::wstring Inspector::showItem(size_t _iPos)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
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

std::wstring Inspector::showUnreferencedItem(size_t _iPos)
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
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
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    while (m_vIT.empty() == false)
    {
        delete *m_vIT.begin();
    }
}

void Inspector::displayMemleak()
{
    std::lock_guard<std::recursive_mutex> lock(m_mutex);
    std::map<std::wstring, size_t> statistics;

    if (m_vIT.size() != 0)
    {
        // construct the statistic map
        for (auto it : m_vIT)
        {
            statistics[it->getTypeStr()]++;
        }

        // display the result
        std::wcerr << L"Memory leaked, please file a bug on https://gitlab.com/scilab/scilab/-/issues" << std::endl;
        for (auto it = statistics.begin(), itEnd = statistics.end(); it != itEnd; ++it)
        {
            std::wcerr << L"    " << it->second << L" " << it->first;

            // list the not free-ed pointers
            std::wcerr << L" : ";
            bool isFirst = true;
            for (auto pi : m_vIT)
            {
                if (it->first == pi->getTypeStr())
                {
                    if (isFirst)
                    {
                        isFirst = false;
                    }
                    else
                    {
                        std::wcerr << " , ";
                    }

                    std::wcerr << pi;

                    if (pi->isString())
                    {
                        types::String* pS = pi->getAs<types::String>();
                        std::wcerr << pS->get(0) << std::endl;
                    }
                }
            }

            std::wcerr << std::endl;
        }
    }
}

#endif
}
