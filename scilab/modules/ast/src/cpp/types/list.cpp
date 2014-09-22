/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include "double.hxx"
#include "list.hxx"
#include "listundefined.hxx"
#include "listinsert.hxx"
#include "types_tools.hxx"
#include "scilabexception.hxx"
#include "localization.hxx"

#ifndef NDEBUG
#include "inspector.hxx"
#endif

extern "C"
{
#include "os_wcsdup.h"
}

namespace types
{
/**
** Constructor & Destructor (public)
*/
List::List() : Container()
{
    m_plData = new std::vector<InternalType *>();
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

List::~List()
{
    if (isDeletable() == true)
    {
        std::vector<InternalType *>::iterator itValues;
        for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues)
        {
            (*itValues)->DecreaseRef();
            (*itValues)->killMe();
        }
        delete m_plData;
    }
#ifndef NDEBUG
    //Inspector::removeItem(this);
#endif
}

/**
** Private Copy Constructor and data Access
*/
List::List(List *_oListCopyMe)
{
    std::vector<InternalType *>::iterator itValues;
    m_plData = new std::vector<InternalType *>;

    for (int i = 0 ; i < (int)_oListCopyMe->getData()->size() ; i++)
    {
        append((*_oListCopyMe->getData())[i]);
    }

    m_iSize = static_cast<int>(m_plData->size());
#ifndef NDEBUG
    //Inspector::addItem(this);
#endif
}

std::vector<InternalType *> *List::getData()
{
    return m_plData;
}

/**
** size_get
** Return the number of elements in list
*/
int List::getSize()
{
    return static_cast<int>(m_plData->size());
}

/**
** append(InternalType *_typedValue)
** Append the given value to the end of the List
*/
void List::append(InternalType *_typedValue)
{
    m_plData->push_back(_typedValue->clone());
    m_plData->back()->IncreaseRef();
    m_iSize = static_cast<int>(m_plData->size());
}

/**
** Clone
** Create a new List and Copy all values.
*/
InternalType *List::clone()
{
    return new List(this);
}

GenericType* List::getColumnValues(int /*_iPos*/)
{
    return NULL;
}

/**
** toString to display Lists
** FIXME : Find a better indentation process
*/
bool List::toString(std::wostringstream& ostr)
{
    wchar_t* wcsVarName = os_wcsdup(ostr.str().c_str());
    ostr.str(L"");

    if (getSize() == 0)
    {
        ostr << wcsVarName << L"()" << std::endl;
    }
    else
    {
        int iPosition = 1;
        std::vector<InternalType *>::iterator itValues;
        for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues, ++iPosition)
        {
            ostr << "     " << wcsVarName << L"(" << iPosition << L")" << std::endl;
            //maange lines
            (*itValues)->toString(ostr);
            ostr << std::endl;
        }
    }

    free(wcsVarName);
    return true;
}

std::vector<InternalType*>	List::extract(typed_list* _pArgs)
{
    std::vector<InternalType*> outList;
    //check input param
    if (_pArgs->size() != 1)
    {
        return outList;
    }

    typed_list pArg;
    int iDims           = (int)_pArgs->size();

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    //evaluate each argument and replace by appropriate value and compute the count of combinations
    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //outList.push_back(Double::Empty());
    }

    for (int i = 0 ; i < iSeqCount ; i++)
    {
        int idx = (int)pArg[0]->getAs<Double>()->get(i);
        if (idx > getSize() || idx < 1)
        {
            outList.clear();
            break;
        }
        InternalType* pIT = (*m_plData)[idx - 1];
        outList.push_back(pIT);
    }

    //free pArg content
    for (int iArg = 0 ; iArg < (int)pArg.size() ; iArg++)
    {
        if (pArg[iArg] != (*_pArgs)[iArg])
        {
            pArg[iArg]->killMe();
        }
    }

    return outList;
}

InternalType* List::insert(typed_list* _pArgs, InternalType* _pSource)
{
    //check input param
    if (_pArgs->size() != 1)
    {
        return NULL;
    }

    typed_list pArg;
    int iDims           = (int)_pArgs->size();

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    if (iSeqCount == 0)
    {
        //do nothing
        return this;
    }
    else if (iSeqCount > 1)
    {
        std::wostringstream os;
        os << _W("Unable to insert multiple item in a list.\n");
        throw ast::ScilabError(os.str());
    }
    else if (iSeqCount < 0)
    {
        return NULL;
    }

    int idx = (int)pArg[0]->getAs<Double>()->get(0);
    if (_pSource->isListDelete())
    {
        //delete item
        if (idx == 0)
        {
            //do nothing
            return this;
        }
        else if (idx <= (int)m_plData->size())
        {
            InternalType* pIT = (*m_plData)[idx - 1];
            if (pIT)
            {
                pIT->killMe();
            }
            m_plData->erase(m_plData->begin() + idx - 1);
        }
    }
    else if (_pSource->isListInsert())
    {
        //insert item
        if (idx == 0)
        {
            std::wostringstream os;
            os << _W("Index out of bounds.\n");
            throw ast::ScilabError(os.str());
        }

        InternalType* pInsert = _pSource->getAs<ListInsert>()->getInsert()->clone();
        pInsert->IncreaseRef();
        if (idx > (int)m_plData->size())
        {
            //try to insert after the last index, increase list size and assign value
            while ((int)m_plData->size() < idx)
            {
                //incease list size and fill with "Undefined"
                m_plData->push_back(new ListUndefined());
            }
            (*m_plData)[idx - 1] = pInsert;
        }
        else
        {
            m_plData->insert(m_plData->begin() + idx - 1, pInsert);
        }
    }
    else if (idx == 0)
    {
        //special cazse to insert at the first position
        InternalType* pInsert = NULL;
        pInsert = _pSource->clone();
        pInsert->IncreaseRef();
        m_plData->insert(m_plData->begin(), pInsert);
    }
    else
    {
        while ((int)m_plData->size() < idx)
        {
            //incease list size and fill with "Undefined"
            InternalType* pLU = new ListUndefined();
            pLU->IncreaseRef();
            m_plData->push_back(pLU);
        }

        InternalType* pIT = (*m_plData)[idx - 1];
        pIT->DecreaseRef();
        if (pIT)
        {
            pIT->killMe();
        }

        (*m_plData)[idx - 1] = _pSource->clone();
        (*m_plData)[idx - 1]->IncreaseRef();
    }

    m_iSize = (int)m_plData->size();

    //free pArg content
    for (int iArg = 0 ; iArg < (int)pArg.size() ; iArg++)
    {
        if (pArg[iArg] != (*_pArgs)[iArg])
        {
            pArg[iArg]->killMe();
        }
    }

    return this;
}

InternalType* List::get(const int _iIndex)
{
    if (_iIndex >= 0 && _iIndex < (int)m_plData->size())
    {
        return (*m_plData)[_iIndex];
    }
    return NULL;
}

bool List::set(const int _iIndex, InternalType* _pIT)
{
    if (_iIndex < 0)
    {
        return false;
    }

    while ((int)m_plData->size() <= _iIndex)
    {
        //incease list size and fill with "Undefined"
        m_plData->push_back(new ListUndefined());
        m_iSize = getSize();
    }

    InternalType* pOld = (*m_plData)[_iIndex];

    _pIT->IncreaseRef();
    (*m_plData)[_iIndex] = _pIT;

    //manage ref on the old value
    if (pOld)
    {
        pOld->DecreaseRef();
        pOld->killMe();
    }

    return true;
}

bool List::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isList() == false)
    {
        return false;
    }

    List* plst = const_cast<InternalType &>(it).getAs<List>();

    if (getSize() != plst->getSize())
    {
        return false;
    }

    for (int i = 0; i < getSize(); i++)
    {
        if (*(*m_plData)[i] != *plst->get(i))
        {
            return false;
        }
    }

    return true;
}

}
