/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <sstream>
#include "graphichandle.hxx"
#include "tostring_common.hxx"
#include "scilabexception.hxx"

extern "C"
{
#include <stdio.h>
#include "localization.h"
#include "os_strdup.h"
#include "MALLOC.h"
}

using namespace std;

namespace types
{

    /****************
     * SingleHandle *
     ****************/

    SingleHandle::SingleHandle() : m_pstHandleID(NULL)
    {
    }

    SingleHandle::SingleHandle(char* _pstHandleID)
    {
        m_pstHandleID = os_strdup(_pstHandleID);
    }

    SingleHandle::~SingleHandle()
    {
    }

    InternalType* SingleHandle::clone()
    {
        return new SingleHandle(getHandleID());
    }

    InternalType::RealType SingleHandle::getType()
    {
        return RealSingleHandle;
    }

    bool SingleHandle::isSingleHandle()
    {
        return true;
    }

    std::wstring SingleHandle::toString(int /*_iPrecision*/, int /*_iLineLen*/)
    {
        return L"SingleHandle::toString";
    }

    char* SingleHandle::getHandleID()
    {
        return m_pstHandleID;
    }

    void SingleHandle::setHandleID(char* _pstHandleID)
    {
        if(m_pstHandleID)
        {
            FREE(m_pstHandleID);
        }

        m_pstHandleID = os_strdup(_pstHandleID);
    }

    bool SingleHandle::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isBool() == false)
        {
            return false;
        }

        SingleHandle* pSH = const_cast<InternalType &>(it).getAs<types::SingleHandle>();
        return (strcmp(pSH->getHandleID(), getHandleID()) == 0);
    }

    bool SingleHandle::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    /*****************
     * GraphicHandle *
     *****************/
    GraphicHandle::GraphicHandle(SingleHandle* _pSingleHandle)
    {
        int piDims[2]   = {1, 1};
        SingleHandle** pSH = NULL;
        create(piDims, 2, &pSH, NULL);
        pSH[0] = _pSingleHandle;
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
    }

    GraphicHandle::GraphicHandle(int _iRows, int _iCols)
    {
        int piDims[2]   = {_iRows, _iCols};
        SingleHandle** pSH = NULL;
        create(piDims, 2, &pSH, NULL);
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
    }

    GraphicHandle::GraphicHandle(int _iDims, int* _piDims)
    {
        SingleHandle** pSH = NULL;
        create(_piDims, _iDims, &pSH, NULL);
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
    }

    void GraphicHandle::whoAmI()
    {
        std::cout << "types::GraphicHandle";
    }


    GraphicHandle::~GraphicHandle()
    {
		if(isDeletable() == true)
		{
			deleteAll();
		}
#ifndef NDEBUG
        Inspector::removeItem(this);
#endif
    }

    InternalType::RealType GraphicHandle::getType(void)
    {
        return RealHandle;
    }

    InternalType* GraphicHandle::clone()
    {
        GraphicHandle* pGH = new GraphicHandle(getDims(), getDimsArray());
        for(int i = 0 ; i < getSize() ; i++)
        {
            pGH->set(i, get(i));
        }
        return pGH;
    }

    bool GraphicHandle::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isBool() == false)
        {
            return false;
        }

        GraphicHandle* pGH = const_cast<InternalType &>(it).getAs<types::GraphicHandle>();

        if(pGH->getDims() != getDims())
        {
            return false;
        }

        for(int i = 0 ; i < getDims() ; i++)
        {
            if(pGH->getDimsArray()[i] != getDimsArray()[i])
            {
                return false;
            }
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            if(*get(i) == *pGH->get(i))
            {
                return false;
            }
        }
        return true;
    }

    bool GraphicHandle::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    void GraphicHandle::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims, int _iPrecision, int _iLineLen)
    {
    }

    SingleHandle* GraphicHandle::getNullValue()
    {
        return new SingleHandle();
    }

    GraphicHandle* GraphicHandle::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new GraphicHandle(_iDims, _piDims);
    }

    SingleHandle* GraphicHandle::copyValue(SingleHandle* _pData)
    {
        return dynamic_cast<SingleHandle*>(_pData->clone());
    }

    void GraphicHandle::deleteAll()
    {
        delete[] m_pRealData;
        m_pRealData = NULL;
        deleteImg();
    }

    void GraphicHandle::deleteImg()
    {
    }

    SingleHandle** GraphicHandle::allocData(int _iSize)
    {
        return new SingleHandle*[_iSize];
    }

}
