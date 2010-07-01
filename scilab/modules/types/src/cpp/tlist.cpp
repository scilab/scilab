/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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
#include "tlist.hxx"
#include "string.hxx"

namespace types 
{
    /**
    ** Constructor & Destructor (public)
    */
    TList::TList() : List()
    {
    }

    /** 
    ** Private Copy Constructor and data Access
    */
    TList::TList(TList *_oTListCopyMe)
    {
        std::vector<InternalType *>::iterator itValues;
        m_plData = new std::vector<InternalType *>;

        for(int i = 0 ; i < _oTListCopyMe->getData()->size() ; i++)
        {
            InternalType* pIT = (*_oTListCopyMe->getData())[i];
            append(pIT);
        }

        m_iSize = static_cast<int>(m_plData->size());
    }

    /**
    ** Clone
    ** Create a new List and Copy all values.
    */
    TList* TList::clone()
    {
        return new TList(this);
    }

    bool TList::exists(const std::string& _sKey)
    {
        if(size_get() < 1)
        {
            return false;
        }

        String* pS = (*m_plData)[0]->getAsString();

        //first field is the tlist type
        for(int i = 1 ; i < pS->size_get() ; i++)
        {
            if(string(pS->string_get(i)) == _sKey)
            {
                return true;
            }
        }
        return false;
    }

    InternalType* TList::get(const std::string& _sKey)
    {
        return get(getIndexFromString(_sKey));
    }

    InternalType* TList::get(const int _iIndex)
    {
        if(size_get() < 2)
        {
            return NULL;
        }

        if(_iIndex > 0 && _iIndex < (*m_plData)[0]->getAsString()->size_get())
        {
            return (*m_plData)[_iIndex];
        }
        return NULL;
    }

    int TList::getIndexFromString(const std::string _sKey)
    {
        if(size_get() < 1)
        {
            return -1;
        }

        String* pS = (*m_plData)[0]->getAsString();
        //first field is the tlist type
        for(int i = 1 ; i < pS->size_get() ; i++)
        {
            if(string(pS->string_get(i)) == _sKey)
            {
                return i;
            }
        }
        return -1;
    }

    std::vector<InternalType*> TList::extract_string(list<string> _stFields)
    {
        std::vector<InternalType*> Result;

        std::list<string>::const_iterator it;
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
    
    std::string TList::getTypeStr() 
    {
        if(size_get() < 1)
        {
            return "";
        }

        return (*m_plData)[0]->getAsString()->string_get(0);
    }

    std::string TList::getShortTypeStr() 
    {
        return getTypeStr();
    }

}