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
#include "symbol.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "scilabexception.hxx"

extern "C"
{
#include "localization.h"
#include "charEncoding.h"
}

namespace types
{
    /**
    ** Constructor & Destructor (public)
    */
    Struct::Struct() : Container()
    {
        m_plData = new std::map<std::wstring, InternalType *>();
        m_iRows = 0;
        m_iCols = 0;
        m_iSize = 0;
    }

    Struct::~Struct()
    {
        if(isDeletable() == true)
        {
            delete m_plData;
        }
    }

    /**
    ** Private Copy Constructor and data Access
    */
    Struct::Struct(Struct *_oStructCopyMe)
    {
        m_iRows = _oStructCopyMe->getRows();
        m_iCols = _oStructCopyMe->getCols();
        std::map<std::wstring, InternalType *>::iterator itValues;
        m_plData = new std::map<std::wstring, InternalType *>;

        for (itValues = _oStructCopyMe->getData()->begin();
            itValues != _oStructCopyMe->getData()->end();
            itValues++)
        {
            ((*itValues).second)->IncreaseRef();
            (*m_plData)[(*itValues).first] = (*itValues).second;
        }
    }

    std::map<std::wstring, InternalType *> *Struct::getData()
    {
        return m_plData;
    }

    /**
    ** size_get
    ** Return the number of elements in struct
    */
    int Struct::getSize()
    {
        return (int)m_plData->size();
    }

    /**
    ** add(symbol::Symbol*_psKey, InternalType *_typedValue)
    ** Append the given value to the struct
    */
    void Struct::add(const std::wstring& _sKey, InternalType *_typedValue)
    {
        m_iRows = 1;
        m_iCols = 1;
        /* Look if we are replacing some existing value */
        if (exists(_sKey))
        {
            if((*m_plData)[_sKey] != NULL)
            {
                (*m_plData)[_sKey]->DecreaseRef();
                if ((*m_plData)[_sKey]->isDeletable())
                {
                    delete (*m_plData)[_sKey];
                }
            }
        }

        //it seams std::map implementation is different between windows and linux
        //so cloning in temporary variable before assign it.
        _typedValue->IncreaseRef();
        (*m_plData)[_sKey] = _typedValue;
    }


    /**
    ** add(symbol::Symbol*_psKey)
    ** Append an null value to the struct
    */
    void Struct::add(const std::wstring& _sKey)
    {
        /* Look if we are replacing some existing value */
        if (exists(_sKey))
        {
            return;
        }
        (*m_plData)[_sKey] = NULL;
    }

    /**
    ** get(symbol::Symbol*_psKey)
    ** Append the given value to the end of the List
    */
    InternalType* Struct::get(const std::wstring& _sKey)
    {
        return (*m_plData)[_sKey];
    }

    bool Struct::exists(const std::wstring& _sKey)
    {
        std::map<std::wstring, InternalType *>::iterator it = m_plData->find(_sKey);
        if (it ==  m_plData->end())
        {
            return false;
        }
        return true;
    }

    /**
    ** Clone
    ** Create a new Struct and Copy all values.
    */
    InternalType* Struct::clone()
    {
        return new Struct(this);
    }

    GenericType* Struct::getColumnValues(int _iPos)
    {
        return NULL;
    }
    /**
    ** toString to display Structs
    ** FIXME : Find a better indentation process
    */
    std::wstring Struct::toString(int _iPrecision, int _iLineLen)
    {
        std::wostringstream ostr;

        if (getSize() == 0)
        {
            ostr << L"Empty struct" << std::endl;
        }
        else
        {
            int iPosition = 1;
            std::map<std::wstring, InternalType *>::iterator itValues;
            for (itValues = m_plData->begin() ; itValues != m_plData->end() ; ++itValues)
            {
                ostr << itValues->first << L" : ";
                switch  ((itValues->second)->getType())
                {
                case RealStruct :
                    ostr << L"[ " << (itValues->second)->getAsStruct()->getRows()
                        << L" x " << (itValues->second)->getAsStruct()->getCols()
                        << L" struct ]";
                    break;
                default :
                    ostr << (itValues->second)->toString(_iPrecision, _iLineLen);
                    break;
                }
                ostr << std::endl;
            }
        }
        return ostr.str();
    }

    InternalType* Struct::insert(typed_list* _pArgs, InternalType* _pSource)
    {
        //check input param
        if(_pArgs->size() != 1)
        {
            std::wostringstream os;
            os << _W("Unable to insert multiple item in a struct.\n");
            throw ast::ScilabError(os.str());
        }
        String* pstKey = (*_pArgs)[0]->getAs<String>();

        if(pstKey == NULL)
        {
            std::wostringstream os;
            os << _W("Assignment between unlike types is not allowed.\n");
            throw ast::ScilabError(os.str());
        }

        for (int i = 0 ; i < pstKey->getSize() ; ++i)
        {
            this->add(std::wstring(pstKey->get(i)), _pSource);
        }

        return this;

    }

    std::vector<InternalType*> Struct::extract(std::list<std::wstring> _stFields)
    {
        std::vector<InternalType*> Result;

        std::list<std::wstring>::const_iterator it;
        for(it = _stFields.begin() ; it != _stFields.end() ; it++)
        {
            if(exists(*it) == false)
            {
                return Result;
            }
        }

        for(it = _stFields.begin() ; it != _stFields.end() ; it++)
        {
            Result.push_back(get(*it));
        }
        return Result;
    }

    String* Struct::getFieldNames()
    {
        String* pOut = new String((int)m_plData->size(), 1);
        std::map<std::wstring, InternalType *>::iterator it;

        int i = 0;
        for (it = m_plData->begin() ; it != m_plData->end() ; it++, i++)
        {
            pOut->set(i, it->first.c_str());
        }
        return pOut;
    }
}
