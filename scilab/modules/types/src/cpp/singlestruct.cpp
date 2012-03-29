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
        m_pData = new std::map<std::wstring, InternalType *>();
    }

    SingleStruct::~SingleStruct()
    {
        if(isDeletable() == true)
        {
            std::map<std::wstring, InternalType *>::iterator it;
            for(it = m_pData->begin() ; it != m_pData->end() ; it++)
            {
                (*it).second->DecreaseRef();
                if((*it).second->isDeletable())
                {
                    //std::wcout << L"delete child(" << (*it).first << L")" << std::endl;
                    delete (*it).second;
                }
                else
                {
                    //std::wcout << L"!!!!!!!!!!!!!!! pas touche child(" << (*it).first << L")" << std::endl;
                }
            }
            
            //std::wcout << L"delete SingleStruct : " << this << std::endl;
            delete m_pData;
        }
        else
        {
            //std::wcout << L"!!!!!!!!!!!!!!! pas touche SingleStruct" << std::endl;
        }        
    }

    SingleStruct::SingleStruct(SingleStruct *_oSingleStructCopyMe)
    {
        std::map<std::wstring, InternalType *>::iterator itValues;
        m_pData = new std::map<std::wstring, InternalType *>;

        for (itValues = _oSingleStructCopyMe->getData()->begin();
            itValues != _oSingleStructCopyMe->getData()->end();
            itValues++)
        {
            (*m_pData)[(*itValues).first] = (*itValues).second->clone();
            (*m_pData)[(*itValues).first]->IncreaseRef();
        }
    }

    std::map<std::wstring, InternalType *> *SingleStruct::getData()
    {
        return m_pData;
    }

    bool SingleStruct::set(const std::wstring& _sKey, InternalType *_typedValue)
    {
        if (exists(_sKey) == false)
        {
            return false;
        }

        /* Look if we are replacing some existing value */
        if((*m_pData)[_sKey] != NULL)
        {
            (*m_pData)[_sKey]->DecreaseRef();
            if ((*m_pData)[_sKey]->isDeletable())
            {
                delete (*m_pData)[_sKey];
            }
        }

        if(_typedValue)
        {
            _typedValue->IncreaseRef();
            (*m_pData)[_sKey] = _typedValue;
        }
        else
        {
            (*m_pData)[_sKey] = NULL;
        }
        return true;
    }


    InternalType* SingleStruct::get(const std::wstring& _sKey)
    {
        return (*m_pData)[_sKey];
    }

    bool SingleStruct::exists(const std::wstring& _sKey)
    {
        if (m_pData->find(_sKey) ==  m_pData->end())
        {
            return false;
        }
        return true;
    }

    SingleStruct* SingleStruct::clone()
    {
        return new SingleStruct(this);
    }

    InternalType* SingleStruct::insert(typed_list* _pArgs, InternalType* _pSource)
    {
        //check input param
        if(_pArgs->size() != 1)
        {
            std::wostringstream os;
            os << _W("Unable to insert multiple item in a struct.\n");
            throw ast::ScilabError(os.str());
        }

        if((*_pArgs)[0]->isString() == false)
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

    String* SingleStruct::getFieldNames()
    {
        String* pOut = new String((int)m_pData->size(), 1);
        std::map<std::wstring, InternalType *>::iterator it;

        int i = 0;
        for (it = m_pData->begin() ; it != m_pData->end() ; it++, i++)
        {
            pOut->set(i, it->first.c_str());
        }
        return pOut;
    }

    std::wstring SingleStruct::toString()
    {
        if(m_pData->size() == 0)
        {
            return L"empty SingleStruct";
        }
        else
        {
            std::wstring str;
            std::map<std::wstring, InternalType *>::iterator itValues;

            for (itValues = getData()->begin() ; itValues != getData()->end() ; itValues++)
            {
                str += (*itValues).first + L" : " + (*itValues).second->getTypeStr() + L"\n";
            }
            return str;
        }
        return L"";
    }

    bool SingleStruct::addField(const std::wstring& _sKey)
    {
        if(exists(_sKey))
        {//field already exists, do nothing and return false
            return false;
        }

        //not found so add field with []
        (*m_pData)[_sKey] = Double::Empty();
        return true;
    }
}
