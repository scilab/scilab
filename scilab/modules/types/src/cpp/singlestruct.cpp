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

#include <sstream>
#include "symbol.hxx"
#include "singlestruct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "scilabexception.hxx"
#include "localization.hxx"

namespace types
{
SingleStruct::SingleStruct()
{
}

SingleStruct::~SingleStruct()
{
    if (isDeletable() == true)
    {
        std::list<InternalType*>::iterator iterFieldData;
        for (iterFieldData = m_Data.begin(); iterFieldData != m_Data.end() ; iterFieldData++)
        {
            (*iterFieldData)->DecreaseRef();
            if ((*iterFieldData)->isDeletable())
            {
                delete (*iterFieldData);
            }
        }
    }
}

SingleStruct::SingleStruct(SingleStruct *_oSingleStructCopyMe)
{
    std::list<std::wstring> wstFields = _oSingleStructCopyMe->getFields();
    std::list<InternalType *> Data = _oSingleStructCopyMe->getData();

    std::list<InternalType*>::iterator iterFieldData;
    std::list<std::wstring>::iterator iterFieldName = wstFields.begin();
    for (iterFieldData = Data.begin(); iterFieldData != Data.end() ; iterFieldData++, iterFieldName++)
    {
        m_wstFields.push_back(*iterFieldName);
        m_Data.push_back((*iterFieldData)->clone());
    }
}

std::list<InternalType *> SingleStruct::getData()
{
    return m_Data;
}

std::list<std::wstring> SingleStruct::getFields()
{
    return m_wstFields;
}

int SingleStruct::getFieldIndex(const std::wstring& _field)
{
    std::list<std::wstring>::iterator iterFieldNames = m_wstFields.begin();
    for (int idx = 0; iterFieldNames != m_wstFields.end() ; iterFieldNames++, idx++)
    {
        if (*iterFieldNames == _field)
        {
            return idx;
        }
    }

    return -1;
}

bool SingleStruct::set(const std::wstring& _sKey, InternalType *_typedValue)
{
    int index = getFieldIndex(_sKey);
    if (index == -1)
    {
        return false;
    }

    std::list<InternalType*>::iterator iterFieldData = m_Data.begin();
    std::advance(iterFieldData, index);

    InternalType* pOld = *iterFieldData;
    if (pOld != _typedValue)
    {
        /* Look if we are replacing some existing value */
        if (pOld != NULL)
        {
            pOld->DecreaseRef();
            if (pOld->isDeletable())
            {
                delete pOld;
            }
        }

        if (_typedValue)
        {
            _typedValue->IncreaseRef();
            *iterFieldData = _typedValue;
        }
        else
        {
            *iterFieldData = NULL;
        }
    }
    return true;
}


InternalType* SingleStruct::get(const std::wstring& _sKey)
{
    int index = getFieldIndex(_sKey);

    if (index == -1)
    {
        return NULL;
    }

    std::list<InternalType*>::iterator iterFieldData = m_Data.begin();
    std::advance(iterFieldData, index);

    return *iterFieldData;
}

bool SingleStruct::exists(const std::wstring& _sKey)
{
    return (getFieldIndex(_sKey) != -1);
}

SingleStruct* SingleStruct::clone()
{
    return new SingleStruct(this);
}

InternalType* SingleStruct::insert(typed_list* _pArgs, InternalType* _pSource)
{
    //check input param
    if (_pArgs->size() != 1)
    {
        std::wostringstream os;
        os << _W("Unable to insert multiple item in a struct.\n");
        throw ast::ScilabError(os.str());
    }

    if ((*_pArgs)[0]->isString() == false)
    {
        std::wostringstream os;
        os << _W("Assignment between unlike types is not allowed.\n");
        throw ast::ScilabError(os.str());
    }

    String* pstKey = (*_pArgs)[0]->getAs<String>();
    for (int i = 0 ; i < pstKey->getSize() ; ++i)
    {
        set(std::wstring(pstKey->get(i)), _pSource);
    }

    return this;

}

std::vector<InternalType*> SingleStruct::extract(std::list<std::wstring> _stFields)
{
    std::vector<InternalType*> Result;

    std::list<std::wstring>::const_iterator it;
    for (it = _stFields.begin() ; it != _stFields.end() ; it++)
    {
        if (exists(*it) == false)
        {
            return Result;
        }
    }

    for (it = _stFields.begin() ; it != _stFields.end() ; it++)
    {
        Result.push_back(get(*it));
    }
    return Result;
}

String* SingleStruct::getFieldNames()
{
    String* pOut = new String((int)m_wstFields.size(), 1);
    std::list<std::wstring>::iterator iterFieldNames = m_wstFields.begin();

    for (int i = 0 ; iterFieldNames != m_wstFields.end() ; iterFieldNames++, i++)
    {
        pOut->set(i, (*iterFieldNames).c_str());
    }
    return pOut;
}

bool SingleStruct::removeField(const std::wstring& _sKey)
{
    std::list<std::wstring>::iterator iterFieldNames = m_wstFields.begin();
    std::list<InternalType*>::iterator iterFieldData = m_Data.begin();
    std::list<std::wstring> wstFields;
    std::list<InternalType *> Data;

    for (int i = 0; iterFieldNames != m_wstFields.end(); iterFieldNames++, iterFieldData++, i++)
    {
        if (*iterFieldNames == _sKey)
        {
            (*iterFieldData)->DecreaseRef();
            if ((*iterFieldData)->isDeletable())
            {
                delete (*iterFieldData);
            }

            continue;
        }

        wstFields.push_back(*iterFieldNames);
        Data.push_back(*iterFieldData);
    }

    m_wstFields = wstFields;
    m_Data = Data;

    return true;
}

bool SingleStruct::addField(const std::wstring& _sKey)
{
    if (exists(_sKey))
    {
        //field already exists, do nothing and return false
        return false;
    }

    //not found so add field with []
    m_wstFields.push_back(_sKey);
    m_Data.push_back(Double::Empty());
    return true;
}

bool SingleStruct::addFieldFront(const std::wstring& _sKey)
{
    if (exists(_sKey))
    {
        //field already exists, do nothing and return false
        return false;
    }

    //not found so add field with []
    m_wstFields.push_front(_sKey);
    m_Data.push_front(Double::Empty());
    return true;
}

bool SingleStruct::toString(std::wostringstream& ostr)
{
    if (m_Data.size() == 0)
    {
        ostr << L"empty SingleStruct";
        return true;
    }
    else
    {
        std::list<std::wstring>::iterator iterFieldNames;
        std::list<InternalType*>::iterator iterFieldData = m_Data.begin();
        for (iterFieldNames = m_wstFields.begin() ; iterFieldNames != m_wstFields.end(); iterFieldNames++, iterFieldData++)
        {
            ostr << *iterFieldNames << L" : " << (*iterFieldData)->getTypeStr() << std::endl;
        }
    }
    return true;
}

bool SingleStruct::operator==(const InternalType& it)
{
    if (const_cast<InternalType &>(it).isSingleStruct() == false)
    {
        return false;
    }

    SingleStruct* other = const_cast<InternalType &>(it).getAs<SingleStruct>();

    std::list<std::wstring> otherFieldNames = other->getFields();
    std::list<InternalType*> otherFieldData = other->getData();

    if (m_wstFields.size() != otherFieldNames.size())
    {
        return false;
    }

    if (m_Data.size() != otherFieldData.size())
    {
        return false;
    }

    std::list<std::wstring>::iterator itFieldNames = m_wstFields.begin();
    std::list<InternalType*>::iterator itFieldData = m_Data.begin();

    std::list<std::wstring>::iterator itOtherFieldNames = otherFieldNames.begin();
    std::list<InternalType*>::iterator itOtherFieldData = otherFieldData.begin();

    for (; itFieldNames != m_wstFields.end(); itFieldNames++, itOtherFieldNames++, itFieldData++, itOtherFieldData++)
    {
        if (*itFieldNames != *itOtherFieldNames)
        {
            return false;
        }

        if (*(*itFieldData) != *(*itOtherFieldData))
        {
            return false;
        }
    }

    return true;
}

bool SingleStruct::operator!=(const InternalType& it)
{
    return !(*this == it);
}

}
