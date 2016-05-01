/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

#include <sstream>
#include "symbol.hxx"
#include "singlestruct.hxx"
#include "string.hxx"
#include "double.hxx"
#include "localization.hxx"
#include "scilabWrite.hxx"

namespace types
{
SingleStruct::SingleStruct()
{
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

SingleStruct::~SingleStruct()
{
    if (isDeletable() == true)
    {
        for (auto data : m_Data)
        {
            data->DecreaseRef();
            data->killMe();
        }
    }
#ifndef NDEBUG
    Inspector::removeItem(this);
#endif
}

SingleStruct::SingleStruct(SingleStruct *_oSingleStructCopyMe) : m_wstFields(_oSingleStructCopyMe->getFields()), m_Data(_oSingleStructCopyMe->getData())
{
    for (auto data : m_Data)
    {
        data->IncreaseRef();
    }
#ifndef NDEBUG
    Inspector::addItem(this);
#endif
}

std::vector<InternalType *> & SingleStruct::getData()
{
    return m_Data;
}

std::unordered_map<std::wstring, int> & SingleStruct::getFields()
{
    return m_wstFields;
}

int SingleStruct::getFieldIndex(const std::wstring & _field)
{
    const auto i = m_wstFields.find(_field);
    if (i != m_wstFields.end())
    {
        return i->second;
    }

    return -1;
}

bool SingleStruct::set(const std::wstring& _sKey, InternalType *_typedValue)
{
    const int index = getFieldIndex(_sKey);
    if (index == -1)
    {
        return false;
    }

    InternalType* pOld = m_Data[index];
    if (pOld != _typedValue)
    {
        /* Look if we are replacing some existing value */
        if (pOld != nullptr)
        {
            pOld->DecreaseRef();
            pOld->killMe();
        }

        if (_typedValue)
        {
            _typedValue->IncreaseRef();
            m_Data[index] = _typedValue;
        }
        else
        {
            m_Data[index] = nullptr;
        }
    }
    return true;
}


InternalType* SingleStruct::get(const std::wstring& _sKey)
{
    int index = getFieldIndex(_sKey);
    if (index == -1)
    {
        return nullptr;
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
        throw ast::InternalError(os.str());
    }

    if ((*_pArgs)[0]->isString() == false)
    {
        std::wostringstream os;
        os << _W("Assignment between unlike types is not allowed.\n");
        throw ast::InternalError(os.str());
    }

    String* pstKey = (*_pArgs)[0]->getAs<String>();
    for (int i = 0 ; i < pstKey->getSize() ; ++i)
    {
        set(pstKey->get(i), _pSource);
    }

    return this;
}

std::vector<InternalType*> SingleStruct::extract(std::vector<std::wstring> & _stFields)
{
    std::vector<InternalType*> Result;

    for (const auto & f : _stFields)
    {
        if (!exists(f))
        {
            return Result;
        }
    }

    for (const auto & f : _stFields)
    {
        Result.push_back(get(f));
    }

    return Result;
}

String* SingleStruct::getFieldNames()
{
    String* pOut = new String((int)m_wstFields.size(), 1);
    for (const auto & field : m_wstFields)
    {
        pOut->set(field.second, field.first.data());
    }
    return pOut;
}

bool SingleStruct::removeField(const std::wstring & _sKey)
{
    const auto i = m_wstFields.find(_sKey);
    if (i != m_wstFields.end())
    {
        const int pos = i->second;
        m_Data[pos]->DecreaseRef();
        m_Data[pos]->killMe();
        m_wstFields.erase(i);

        for (auto & p : m_wstFields)
        {
            if (p.second > pos)
            {
                --p.second;
            }
        }

        m_Data.erase(m_Data.begin() + pos);
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
    InternalType* pIT = Double::Empty();
    pIT->IncreaseRef();
    m_Data.push_back(pIT);
    m_wstFields.emplace(_sKey, m_Data.size() - 1);

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
    InternalType* pIT = Double::Empty();
    pIT->IncreaseRef();
    m_Data.insert(m_Data.begin(), pIT);

    for (auto & p : m_wstFields)
    {
        p.second++;
    }

    m_wstFields.emplace(_sKey, 0);
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

        for (auto & p : m_wstFields)
        {
            ostr << p.first << L" : " << m_Data[p.second]->getTypeStr() << std::endl;
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

    std::unordered_map<std::wstring, int> & otherFieldNames = other->getFields();
    std::vector<InternalType*> & otherFieldData = other->getData();

    if (m_wstFields.size() != otherFieldNames.size())
    {
        return false;
    }

    for (const auto & p : m_wstFields)
    {
        const auto i = otherFieldNames.find(p.first);
        if (i == otherFieldNames.end() || (*m_Data[p.second] != *otherFieldData[i->second]))
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
