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
#include "struct.hxx"
#include "string.hxx"
#include "list.hxx"

namespace types
{
    Struct::Struct()
    {
        SingleStruct** pIT  = NULL;
        int piDims[2] = {0, 0};
		create(piDims, 2, &pIT, NULL);
    }

    Struct::Struct(int _iRows, int _iCols)
    {
        SingleStruct** pIT  = NULL;
        int piDims[2] = {_iRows, _iCols};
		create(piDims, 2, &pIT, NULL);
        for(int i = 0 ; i < getSize() ; i++)
        {
            set(i, new SingleStruct());
        }
    }

    Struct::Struct(int _iDims, int* _piDims)
    {
        SingleStruct** pIT  = NULL;
		create(_piDims, _iDims, &pIT, NULL);
        for(int i = 0 ; i < getSize() ; i++)
        {
            set(i, new SingleStruct());
        }
    }

    Struct::~Struct()
    {
        if(isDeletable() == true)
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                SingleStruct *pStr = m_pRealData[i];
                if(pStr)
                {
                    pStr->DecreaseRef();
                    if(pStr->isDeletable())
                    {
                        delete pStr;
                    }
                }
            }
        }
    }

    Struct::Struct(Struct *_oStructCopyMe)
    {
        SingleStruct** pIT = NULL;
        create(_oStructCopyMe->getDimsArray(), _oStructCopyMe->getDims(), &pIT, NULL);
        for(int i = 0 ; i < getSize() ; i++)
        {
            m_pRealData[i] = NULL;
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            pIT[i] = _oStructCopyMe->get(i)->clone();
        }
    }

    InternalType* Struct::clone()
    {
        return new Struct(this);
    }

    bool Struct::set(int _iRows, int _iCols, SingleStruct* _pIT)
    {
        if(_iRows < getRows() && _iCols < getCols())
        {
            return set(_iCols * getRows() + _iRows, _pIT);
        }
        return false;
    }

    bool Struct::set(int _iRows, int _iCols, const SingleStruct* _pIT)
    {
        if(_iRows < getRows() && _iCols < getCols())
        {
            return set(_iCols * getRows() + _iRows, _pIT);
        }
        return false;
    }

    bool Struct::set(int _iIndex, SingleStruct* _pIT)
    {
        if(_iIndex < getSize())
        {
            if(m_pRealData[_iIndex] != NULL)
            {
                m_pRealData[_iIndex]->DecreaseRef();
                if(m_pRealData[_iIndex]->isDeletable())
                {
                    delete m_pRealData[_iIndex];
                }
            }

            m_pRealData[_iIndex] = _pIT->clone();
            return true;
        }
        return false;
    }

    bool Struct::set(int _iIndex, const SingleStruct* _pIT)
    {
        if(_iIndex < getSize())
        {
            if(m_pRealData[_iIndex] != NULL)
            {
                m_pRealData[_iIndex]->DecreaseRef();
                if(m_pRealData[_iIndex]->isDeletable())
                {
                    delete m_pRealData[_iIndex];
                }
            }

            m_pRealData[_iIndex] = const_cast<SingleStruct*>(_pIT)->clone();
            return true;
        }
        return false;
    }

    bool Struct::set(SingleStruct** _pIT)
    {
        for(int i = 0 ; i < getSize() ; i++)
        {
            if(set(i, _pIT[i]) == false)
            {
                return false;
            }
        }
        return true;
    }

    String* Struct::getFieldNames()
    {
        if(getSize() != 0)
        {
            return get(0)->getFieldNames();
        }
        else
        {
            return NULL;
        }
    }

    bool Struct::exists(const std::wstring& _sKey)
    {
        if(getSize() != 0)
        {
            return get(0)->exists(_sKey);
        }
        else
        {
            return false;
        }
    }
    
    bool Struct::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isStruct())
        {
            return false;
        }

        Struct* pStr = const_cast<InternalType &>(it).getAs<Struct>();

        for(int i = 0 ; i < getDims() ; i++)
        {
            if(pStr->getDimsArray()[i] != getDimsArray()[i])
            {
                return false;
            }
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            if(get(i) != pStr->get(i))
            {
                return false;
            }
        }
        return true;
    }

    bool Struct::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    SingleStruct* Struct::getNullValue()
    {
        return new SingleStruct();
    }

    Struct* Struct::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new Struct(_iDims, _piDims);
    }

    SingleStruct* Struct::copyValue(SingleStruct* _pData)
    {
        return _pData->clone();
    }

    void Struct::deleteAll()
    {
		for(int i = 0 ; i < getSize() ; i++)
		{
            m_pRealData[i]->DecreaseRef();
            if(m_pRealData[i]->isDeletable())
            {
                delete m_pRealData[i];
            }
		}
		delete[] m_pRealData;
        m_pRealData = NULL;
    }

    void Struct::deleteImg()
    {
        return;
    }

    SingleStruct** Struct::allocData(int _iSize)
    {
        SingleStruct** pData = new SingleStruct*[_iSize];
        for(int i = 0 ; i < _iSize ; i++)
        {
            pData[i] = NULL;
        }
        return pData;
   }

    void Struct::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
    }

    bool Struct::addField(const std::wstring& _sKey)
    {
        if(getSize() == 0)
        {
            //change dimension to 1x1 and add field
            resize(1,1);
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            if(get(i)->isRef(1))
            {//assign more than once
                //clone it before add field
                set(i, get(i)->clone());
            }

            get(i)->addField(_sKey);
        }
        return true;
    }

    std::wstring Struct::toString(int _iPrecision, int _iLineLen)
    {
        std::wostringstream ostr;

        if(getSize() == 0)
        {
            ostr << L"0x0 struct array with no field.";
        }
        else if(getSize() == 1)
        {
            SingleStruct* pSS =  get(0);
            String* pwstFields =  pSS->getFieldNames();
            for(int i = 0 ; i < pwstFields->getSize() ; i++)
            {
                std::wstring wstField(pwstFields->get(i));
                InternalType* pIT = pSS->get(wstField);

//                ostr << L"  " << wstField << ": ";
                ostr << L"  " << wstField << L": ";
                ostr << pIT->toStringInLine(_iPrecision, _iLineLen);
                ostr << std::endl;
            }
        }
        else
        {
            ostr << L"  ";
            for(int i = 0 ; i < m_iDims ; i++)
            {
                if(i > 0)
                {
                    ostr << L"x";
                }
                ostr << m_piDims[i];
            }
            ostr << L" struct array with ";

            String* pwstFields = getFieldNames();
            ostr <<  L"fields:" << std::endl;
            for(int i = 0 ; i < pwstFields->getSize() ; i++)
            {
                ostr << L"    " << pwstFields->get(i) << std::endl;
            }
        }

        return ostr.str();
    }

    std::vector<InternalType*> Struct::extractFields(std::list<std::wstring> _wstFields)
    {
        std::vector<InternalType*> ResultList;
        for(int i = 0 ; i < _wstFields.size() ; i++)
        {
            ResultList.push_back(new List());
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            std::vector<InternalType*> fields = get(i)->extract(_wstFields);

            for(int j = 0 ; j < fields.size() ; j++)
            {
                ResultList[j]->getAs<List>()->append(fields[j]);
            }
        }
        return ResultList;
    }
}
