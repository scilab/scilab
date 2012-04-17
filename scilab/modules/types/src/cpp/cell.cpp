/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
#include <math.h>
#include "symbol.hxx"
#include "cell.hxx"
#include "double.hxx"
#include "list.hxx"
#include "colon.hxx"
#include "tostring_common.hxx"
#include "core_math.h"
#include "list.hxx"
#include "formatmode.h"

namespace types
{
    /**
    ** Constructor & Destructor (public)
    */
    Cell::Cell()
    {
        InternalType** pIT  = NULL;
        int piDims[2] = {0, 0};
		create(piDims, 2, &pIT, NULL);
    }

    Cell::Cell(int _iRows, int _iCols)
    {
        InternalType** pIT  = NULL;
        int piDims[2] = {_iRows, _iCols};
		create(piDims, 2, &pIT, NULL);
        Double* pEmpty = Double::Empty();
        for(int i = 0 ; i < getSize() ; i++)
        {
            pEmpty->IncreaseRef();
            m_pRealData[i] = pEmpty;
        }
    }

    Cell::Cell(int _iDims, int* _piDims)
    {
        InternalType** pIT  = NULL;
		create(_piDims, _iDims, &pIT, NULL);
        Double* pEmpty = Double::Empty();
        for(int i = 0 ; i < getSize() ; i++)
        {
            pEmpty->IncreaseRef();
            m_pRealData[i] = pEmpty;
        }
    }

    Cell::~Cell()
    {
        if(isDeletable() == true)
        {
            for(int i = 0 ; i < getSize() ; i++)
            {
                m_pRealData[i]->DecreaseRef();
                if(m_pRealData[i]->isDeletable())
                {
                    delete m_pRealData[i];
                }
            }
        }
#ifndef NDEBUG
        Inspector::removeItem(this);
#endif
    }

    /**
    ** Private Copy Constructor and data Access
    */
    Cell::Cell(Cell *_oCellCopyMe)
    {
        InternalType** pIT = NULL;
        create(_oCellCopyMe->getDimsArray(), _oCellCopyMe->getDims(), &pIT, NULL);
        for(int i = 0 ; i < getSize() ; i++)
        {
            m_pRealData[i] = NULL;
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            set(i, _oCellCopyMe->get(i)->clone());
        }
#ifndef NDEBUG
        Inspector::addItem(this);
#endif
    }

    bool Cell::set(int _iRows, int _iCols, InternalType* _pIT)
    {
        if(_iRows < getRows() && _iCols < getCols())
        {
            return set(_iCols * getRows() + _iRows, _pIT);
        }
        return false;
    }

    bool Cell::set(int _iRows, int _iCols, const InternalType* _pIT)
    {
        if(_iRows < getRows() && _iCols < getCols())
        {
            return set(_iCols * getRows() + _iRows, _pIT);
        }
        return false;
    }

    bool Cell::set(int _iIndex, InternalType* _pIT)
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

            _pIT->IncreaseRef();
            m_pRealData[_iIndex] = _pIT;
            return true;
        }
        return false;
    }

    bool Cell::set(int _iIndex, const InternalType* _pIT)
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

            const_cast<InternalType*>(_pIT)->IncreaseRef();
            m_pRealData[_iIndex] = const_cast<InternalType*>(_pIT);
            return true;
        }
        return false;
    }

    bool Cell::set(InternalType** _pIT)
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

    /**
    ** Clone
    ** Create a new Struct and Copy all values.
    */
    InternalType* Cell::clone()
    {
        return new Cell(this);
    }

    InternalType* Cell::getNullValue()
    {
        return Double::Empty();
    }

    Cell* Cell::createEmpty(int _iDims, int* _piDims, bool _bComplex)
    {
        return new Cell(_iDims, _piDims);
    }

    InternalType* Cell::copyValue(InternalType* _pData)
    {
        _pData->IncreaseRef();
        return _pData;
    }

    void Cell::deleteAll()
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

    void Cell::deleteImg()
    {
        return;
    }

    bool Cell::isEmpty()
    {
        if(getDims() == 2 && getRows() == 0 && getCols() == 0)
        {
            return true;
        }
        return false;
    }

    /**
    ** toString to display Structs
    ** FIXME : Find a better indentation process
    */
    bool Cell::subMatrixToString(std::wostringstream& ostr, int* _piDims, int _iDims)
    {
        int iPrecision = getFormatSize();

        if(isEmpty())
        {
            ostr << L"   {}";
        }
        else
        {
            //max len for each column
            int *piTypeLen = new int[getCols()];
            int *piSizeLen = new int[getCols()];

            memset(piTypeLen, 0x00, getCols() * sizeof(int));
            memset(piSizeLen, 0x00, getCols() * sizeof(int));

            for(int j = 0 ; j < getCols() ; j++)
            {
                for(int i = 0 ; i < getRows() ; i++)
                {
                    _piDims[0] = i;
                    _piDims[1] = j;

                    int iPos = getIndex(_piDims);
                    InternalType* pIT = get(iPos);

                    if(pIT->isAssignable())
                    {
                        //compute number of digits to write dimensions
                        int iTypeLen = 0;
                        if(pIT->getAs<GenericType>())
                        {
                            GenericType* pGT = pIT->getAs<GenericType>();
                            for(int k = 0 ; k < pGT->getDims() ; k++)
                            {
                                iTypeLen += static_cast<int>(log10(static_cast<double>(pGT->getDimsArray()[k])) + 1);
                            }
                            piSizeLen[j] = Max(piSizeLen[j], iTypeLen + (pGT->getDims() - 1));//add number of "x"
                        }
                        else
                        {//types non derived from ArrayOf.
                            int iSize = static_cast<int>(log10(static_cast<double>(pIT->getAs<GenericType>()->getRows())) + 1);
                            piSizeLen[j] = Max(piSizeLen[j], iSize);
                        }
                    }
                    else
                    {//no size so let a white space, size == 1
                        piSizeLen[j] = Max(piSizeLen[j], 1);
                    }

                    piTypeLen[j] = Max(piTypeLen[j], static_cast<int>(pIT->getTypeStr().size()));
                }
            }

            for(int i = 0 ; i < getRows() ; i++)
            {
                for(int j = 0 ; j < getCols() ; j++)
                {
                    _piDims[0] = i;
                    _piDims[1] = j;
                    int iPos = getIndex(_piDims);
                    InternalType* pIT = get(iPos);

                    ostr << L"  [";
                    if(pIT->isAssignable())
                    {
                        if(pIT->isGenericType())
                        {//"  ixjxkxl type   "
                            GenericType* pGT = pIT->getAs<GenericType>();
                            std::wostringstream ostemp;
                            for(int k = 0 ; k < pGT->getDims() ; k++)
                            {
                                if(k != 0)
                                {
                                    ostemp << L"x";
                                }
                                ostemp << pGT->getDimsArray()[k];
                            }
                            configureStream(&ostr, piSizeLen[j], iPrecision, ' ');
                            ostr << std::right << ostemp.str();
                        }
                        else
                        {//" i   "
                            configureStream(&ostr, piSizeLen[j], iPrecision, ' ');
                            if(pIT->isList())
                            {
                                ostr << std::right << pIT->getAs<List>()->getSize();
                            }
                            else
                            {
                                ostr << std::right << 1;
                            }
                        }
                    }
                    else
                    {
                        configureStream(&ostr, piSizeLen[j], iPrecision, ' ');
                        ostr << L"";//fill with space
                    }
                    ostr << L" ";
                    configureStream(&ostr, piTypeLen[j], iPrecision, ' ');
                    ostr << std::left << pIT->getTypeStr();
                    ostr << L"]";
                }
                ostr << std::endl;
            }

            delete[] piSizeLen;
            delete[] piTypeLen;
        }
        ostr << std::endl;
        return true;
    }

    //bool Cell::append(int _iRows, int _iCols, Cell *_poSource)
    //{
    //    return true;
    //}

    bool Cell::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).isCell())
        {
            return false;
        }

        Cell* pC = const_cast<InternalType &>(it).getAs<Cell>();

        for(int i = 0 ; i < getDims() ; i++)
        {
            if(pC->getDimsArray()[i] != getDimsArray()[i])
            {
                return false;
            }
        }

        for(int i = 0 ; i < getSize() ; i++)
        {
            if(get(i) != pC->get(i))
            {
                return false;
            }
        }
        return true;
    }

    bool Cell::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    List* Cell::extractCell(typed_list* _pArgs)
    {
        InternalType* pIT = extract(_pArgs);
        if(pIT->isCell() == false)
        {
            return NULL;
        }

        List* pList = new List();
        
        Cell* pCell = pIT->getAs<Cell>();
        for(int i = 0 ; i < pCell->getSize() ; i++)
        {
            pList->append(pCell->get(i));
        }
        delete pCell;
        return pList;
    }

    Cell* Cell::insertCell(typed_list* _pArgs, InternalType* _pSource)
    {
        Cell* pCell = new Cell(1, 1);
        pCell->set(0, _pSource);
        Cell* pOut = insert(_pArgs, pCell)->getAs<Cell>();
        return pOut;
    }

    Cell* Cell::insertNewCell(typed_list* _pArgs, InternalType* _pSource)
    {
        Cell* pCell = new Cell(1, 1);
        pCell->set(0, _pSource);
        Cell* pOut = Cell::insertNew(_pArgs, pCell)->getAs<Cell>();
        return pOut;
    }

    InternalType** Cell::allocData(int _iSize)
    {
        InternalType** pData = new InternalType*[_iSize];
        Double* pEmpty = Double::Empty();
        for(int i = 0 ; i < _iSize ; i++)
        {
            pEmpty->IncreaseRef();
            pData[i] = pEmpty;
        }
        return pData;
    }
}
