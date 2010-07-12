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
        m_iRows = _oStructCopyMe->rows_get();
        m_iCols = _oStructCopyMe->cols_get();
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
    int Struct::size_get() 
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
    Struct *Struct::clone()
    {
        return new Struct(this);
    }

    /**
    ** toString to display Structs
    ** FIXME : Find a better indentation process
    */
    wstring Struct::toString(int _iPrecision, int _iLineLen)
    {
        wostringstream ostr;

        if (size_get() == 0)
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
                    ostr << L"[ " << (itValues->second)->getAsStruct()->rows_get()
                        << L" x " << (itValues->second)->getAsStruct()->cols_get()
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

    std::vector<InternalType*> Struct::extract(list<wstring> _stFields)
    {
        std::vector<InternalType*> Result;

        std::list<wstring>::const_iterator it;
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
}
