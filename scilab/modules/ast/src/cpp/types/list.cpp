/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include <sstream>
#include "double.hxx"
#include "list.hxx"
#include "listundefined.hxx"
#include "listinsert.hxx"
#include "types_tools.hxx"
#include "localization.hxx"
#include "scilabWrite.hxx"
#include "types_tools.hxx"
#include "function.hxx"

#ifndef NDEBUG
#include "inspector.hxx"
#endif

extern "C"
{
#include "os_string.h"
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
    Inspector::addItem(this);
#endif
}

List::~List()
{
    if (isDeletable() == true)
    {
        for (auto data : *m_plData)
        {
            data->DecreaseRef();
            data->killMe();
        }
        delete m_plData;
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

/**
** Private Copy Constructor and data Access
*/
List::List(List *_oListCopyMe)
{
    m_plData = new std::vector<InternalType *>;
    std::vector<InternalType *>* lData = _oListCopyMe->getData();
    int size = lData->size();
    for (int i = 0 ; i < size ; i++)
    {
        append((*lData)[i]);
    }

    m_iSize = static_cast<int>(size);
#ifndef NDEBUG
    Inspector::addItem(this);
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
List* List::append(InternalType *_typedValue)
{
    List* pIT = checkRef(this, &List::append, _typedValue);
    if (pIT != this)
    {
        return pIT;
    }

    _typedValue->IncreaseRef();
    m_plData->push_back(_typedValue);
    m_iSize = static_cast<int>(m_plData->size());
    return this;
}

/**
** Clone
** Create a new List and Copy all values.
*/
List *List::clone()
{
    return new List(this);
}

/**
** toString to display Lists
*/
bool List::toString(std::wostringstream& ostr)
{
    if (getSize() == 0)
    {
        ostr.str(L"");
        ostr << L"     ()" << std::endl;
    }
    else
    {
        wchar_t* wcsVarName = os_wcsdup(ostr.str().c_str());
        int iPosition = 1;
        for (auto val : *m_plData)
        {
            std::wostringstream nextVarName;
            ostr.str(L"");
            nextVarName << " " << SPACES_LIST << wcsVarName << L"(" << iPosition++ << L")";
            ostr << std::endl << nextVarName.str() << std::endl << std::endl;
            scilabForcedWriteW(ostr.str().c_str());
            if (VariableToString(val, nextVarName.str().c_str()) == types::Function::Error)
            {
                free(wcsVarName);
                ostr.str(L"");
                return true;
            }
        }

        ostr.str(L"");
        free(wcsVarName);
    }

    return true;
}

InternalType* List::extract(typed_list* _pArgs)
{
    List* outList = new List();
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
    delete[] piMaxDim;
    delete[] piCountDim;

    for (int i = 0 ; i < iSeqCount ; i++)
    {
        int idx = (int)pArg[0]->getAs<Double>()->get(i);
        if (idx > getSize() || idx < 1)
        {
            delete outList;
            outList = NULL;
            break;
        }
        InternalType* pIT = (*m_plData)[idx - 1];
        outList->set(i, pIT);
    }

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

    return outList;
}

List* List::insert(typed_list* _pArgs, InternalType* _pSource)
{
    //check input param
    if (_pArgs->size() != 1)
    {
        return NULL;
    }

    List* pIT = checkRef(this, &List::insert, _pArgs, _pSource);
    if (pIT != this)
    {
        return pIT;
    }

    typed_list pArg;
    int iDims           = (int)_pArgs->size();

    int* piMaxDim       = new int[iDims];
    int* piCountDim     = new int[iDims];

    int iSeqCount = checkIndexesArguments(this, _pArgs, &pArg, piMaxDim, piCountDim);
    delete[] piMaxDim;
    delete[] piCountDim;
    if (iSeqCount == 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        //do nothing
        return this;
    }
    else if (iSeqCount > 1)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        std::wostringstream os;
        os << _W("Unable to insert multiple item in a list.\n");
        throw ast::InternalError(os.str());
    }
    else if (iSeqCount < 0)
    {
        //free pArg content
        cleanIndexesArguments(_pArgs, &pArg);
        return NULL;
    }

    int idx = (int)pArg[0]->getAs<Double>()->get(0);
    if (_pSource->isListDelete())
    {
        //delete item
        if (idx == 0)
        {
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            //do nothing
            return this;
        }
        else if (idx <= (int)m_plData->size())
        {
            InternalType* pIT = (*m_plData)[idx - 1];
            if (pIT)
            {
                pIT->DecreaseRef();
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
            //free pArg content
            cleanIndexesArguments(_pArgs, &pArg);
            std::wostringstream os;
            os << _W("Index out of bounds.\n");
            throw ast::InternalError(os.str());
        }

        InternalType* pInsert = _pSource->getAs<ListInsert>()->getInsert();
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
        //special case to insert at the first position
        _pSource->IncreaseRef();
        m_plData->insert(m_plData->begin(), _pSource);
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

        (*m_plData)[idx - 1] = _pSource;
        (*m_plData)[idx - 1]->IncreaseRef();

        pIT->DecreaseRef();
        pIT->killMe();
    }

    m_iSize = (int)m_plData->size();

    //free pArg content
    cleanIndexesArguments(_pArgs, &pArg);

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

List* List::set(const int _iIndex, InternalType* _pIT)
{
    if (_iIndex < 0)
    {
        return NULL;
    }

    List* pIT = checkRef(this, &List::set, _iIndex, _pIT);
    if (pIT != this)
    {
        return pIT;
    }

    while ((int)m_plData->size() < _iIndex)
    {
        //incease list size and fill with "Undefined"
        m_plData->push_back(new ListUndefined());
        m_plData->back()->IncreaseRef();
        m_iSize = getSize();
    }

    if ((int)m_plData->size() == _iIndex)
    {
        _pIT->IncreaseRef();
        m_plData->push_back(_pIT);
        m_iSize = getSize();
    }
    else
    {
        InternalType* pOld = (*m_plData)[_iIndex];

        _pIT->IncreaseRef();
        (*m_plData)[_iIndex] = _pIT;

        //manage ref on the old value
        if (pOld)
        {
            pOld->DecreaseRef();
            pOld->killMe();
        }
    }

    return this;
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
