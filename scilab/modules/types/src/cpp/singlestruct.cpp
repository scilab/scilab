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

extern "C"
{
#include "localization.h"
#include "charEncoding.h"
}

namespace types
{
SingleStruct::SingleStruct()
{
}

SingleStruct::~SingleStruct()
{
    if (isDeletable() == true)
    {
        for (int i = 0 ; i < m_Data.size() ; i++)
        {
            m_Data[i]->DecreaseRef();
            if (m_Data[i]->isDeletable())
            {
                delete m_Data[i];
            }
        }
    }
}

SingleStruct::SingleStruct(SingleStruct *_oSingleStructCopyMe)
{
    std::vector<std::wstring> wstFields = _oSingleStructCopyMe->getFields();
    std::vector<InternalType *> Data = _oSingleStructCopyMe->getData();

    for (int i = 0 ; i < Data.size() ; i++)
    {
        m_wstFields.push_back(wstFields[i]);
        m_Data.push_back(Data[i]->clone());
    }
}

std::vector<InternalType *> SingleStruct::getData()
{
    return m_Data;
}

std::vector<std::wstring> SingleStruct::getFields()
{
    return m_wstFields;
}

int SingleStruct::getFieldIndex(const std::wstring& _field)
{
    for (int i = 0 ; i < m_wstFields.size() ; i++)
    {
        if (m_wstFields[i] == _field)
        {
            return i;
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

    /* Look if we are replacing some existing value */
    if (m_Data[index] != NULL)
    {
        m_Data[index]->DecreaseRef();
        if (m_Data[index]->isDeletable())
        {
            delete m_Data[index];
        }
    }

    if (_typedValue)
    {
        _typedValue->IncreaseRef();
        m_Data[index] = _typedValue;
    }
    else
    {
        m_Data[index] = NULL;
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

    return m_Data[index];
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

    for (int i = 0 ; i < m_wstFields.size() ; i++)
    {
        pOut->set(i, m_wstFields[i].c_str());
    }
    return pOut;
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
        for (int i = 0 ; i < m_Data.size() ; i++)
        {
            ostr << m_wstFields[i] << L" : " << m_Data[i]->getTypeStr() << std::endl;
        }
    }
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
}
