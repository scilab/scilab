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
#include <math.h>
#include "symbol.hxx"
#include "cell.hxx"
#include "double.hxx"
#include "tostring_common.hxx"
#include "core_math.h"

namespace types 
{
    /**
    ** Constructor & Destructor (public)
    */
    Cell::Cell() : Container()
    {
        m_plData = NULL;
        m_iRows = 0;
        m_iCols = 0;
        m_iSize = 0;
    }

    Cell::Cell(int _iRows, int _iCols)
    {
        createCell(_iRows, _iCols);
    }

    void Cell::createCell(int _iRows, int _iCols)
    {
        m_iRows = _iRows;
        m_iCols = _iCols;
        m_iSize = m_iRows * m_iCols;

        m_plData = new InternalType*[size_get()];

        Double* pEmpty = new Double(0,0);
        for(int i = 0 ; i < size_get() ; i++)
        {
            pEmpty->IncreaseRef();
            m_plData[i] = pEmpty;
        }
    }

    Cell::~Cell() 
    {
        if(isDeletable() == true)
        {
            for(int i = 0 ; i < size_get() ; i++)
            {
                m_plData[i]->DecreaseRef();
                if(m_plData[i]->isDeletable())
                {
                    delete m_plData[i];
                }
            }
            delete[] m_plData;
        }
    }

    /** 
    ** Private Copy Constructor and data Access
    */
    Cell::Cell(Cell *_oCellCopyMe)
    {
        createCell(_oCellCopyMe->rows_get(), _oCellCopyMe->cols_get());
        for(int i = 0 ; i < size_get() ; i++)
        {
            set(i, _oCellCopyMe->get(i));
        }
    }

    InternalType* Cell::get(int _iIndex)
    {
        if(_iIndex < size_get())
        {
            return m_plData[_iIndex];
        }
        return NULL;
    }

    InternalType* Cell::get(int _iRows, int _iCols)
    {
        if(_iRows < rows_get() && _iCols < cols_get())
        {
            return get(_iCols * rows_get() + _iRows);
        }
        return NULL;
    }

    bool Cell::set(int _iRows, int _iCols, InternalType* _pIT)
    {
        if(_iRows < rows_get() && _iCols < cols_get())
        {
            return set(_iCols * rows_get() + _iRows, _pIT);
        }
        return false;
    }

    bool Cell::set(int _iIndex, InternalType* _pIT)
    {
        if(_iIndex < size_get())
        {
            if(m_plData[_iIndex] != NULL)
            {
                m_plData[_iIndex]->DecreaseRef();
                if(m_plData[_iIndex]->isDeletable())
                {
                    delete m_plData[_iIndex];
                }
            }

            _pIT->IncreaseRef();
            m_plData[_iIndex] = _pIT;
            return true;
        }
        return false;
    }

    /**
    ** size_get
    ** Return the number of elements in struct
    */
    int Cell::size_get() 
    {
        return m_iSize;
    }

    /**
    ** Clone
    ** Create a new Struct and Copy all values.
    */
    Cell *Cell::clone()
    {
        return new Cell(this);
    }

    /**
    ** toString to display Structs
    ** FIXME : Find a better indentation process
    */
    wstring Cell::toString(int _iPrecision, int _iLineLen)
    {
        wostringstream ostr;

        if(size_get() == 0)
        {
            ostr << L"   {}";
        }
        else
        {
            //max len for each column
            int *piColLen = new int[cols_get()];
            int *piILen = new int[cols_get()];
            int *piJLen = new int[cols_get()];
            int *piSumLen = new int[cols_get()];

            memset(piColLen, 0x00, cols_get() * sizeof(int));
            memset(piILen,  0x00, cols_get() * sizeof(int));
            memset(piJLen, 0x00, cols_get() * sizeof(int));
            memset(piSumLen, 0x00, cols_get() * sizeof(int));

            for(int j = 0 ; j < cols_get() ; j++)
            {
                for(int i = 0 ; i < rows_get() ; i++)
                {
                    InternalType* pIT = get(i,j);

                    wstring strType = pIT->getTypeStr();
                    if(pIT->isAssignable())
                    {
                        //compute number of digits to write rows and cols
                        int iRowsLen = static_cast<int>(log10(static_cast<double>(pIT->getAsGenericType()->rows_get())) + 1);
                        int iColsLen = static_cast<int>(log10(static_cast<double>(pIT->getAsGenericType()->cols_get())) + 1);

                        if(piILen[j] < iRowsLen)
                        {
                            piILen[j] = iRowsLen;
                        }

                        if(piJLen[j] < iColsLen)
                        {
                            piJLen[j] = iColsLen;
                        }

                        if(piSumLen[j] < (iColsLen + iRowsLen + 1))
                        {
                            piSumLen[j] = iColsLen + iRowsLen + 1; //+1 for 'x' character
                        }
                    }

                    if(piColLen[j] < strType.size())
                    {
                        piColLen[j] = static_cast<int>(strType.size());
                    }
                }
            }

            for(int i = 0 ; i < rows_get() ; i++)
            {
                for(int j = 0 ; j < cols_get() ; j++)
                {
                    InternalType* pIT = get(i,j);

                    ostr << L"  [";
                    if(pIT->isAssignable())
                    {
                        wostringstream ostemp;
                        Config_Stream(&ostemp, piILen[j], _iPrecision, ' ');
                        ostemp << right << pIT->getAsGenericType()->rows_get();
                        ostemp << L"x";
                        Config_Stream(&ostemp, piJLen[j], _iPrecision, ' ');
                        ostemp << left << pIT->getAsGenericType()->cols_get();
                        Config_Stream(&ostemp, piSumLen[j] - static_cast<int>(ostemp.str().size()), _iPrecision, ' ');
                        ostemp << L"";//fill with space
                        ostr << ostemp.str();
                    }
                    else
                    {
                        Config_Stream(&ostr, piSumLen[j], _iPrecision, ' ');
                        ostr << L"";//fill with space
                    }
                    ostr << L" ";
                    Config_Stream(&ostr, piColLen[j], _iPrecision, ' ');
                    ostr << left << pIT->getTypeStr();
                    ostr << L"]";
                }
                ostr << std::endl;
            }

            delete[] piColLen;
            delete[] piILen;
            delete[] piJLen;
            delete[] piSumLen;
        }
        ostr << std::endl;
        return ostr.str();
    }

    bool Cell::resize(int _iNewRows, int _iNewCols)
    {
        if(_iNewRows <= rows_get() && _iNewCols <= cols_get())
        {//nothing to do
            return true;
        }

        //alloc new data array
        InternalType** pIT = NULL;

        pIT = new InternalType*[_iNewRows * _iNewCols];
        for(int i = 0 ; i < _iNewRows ; i++)
        {
            for(int j = 0 ; j < _iNewCols ; j++)
            {
                pIT[j * _iNewRows + i] = new Double(0,0);
            }
        }

        //copy existing values
        for(int i = 0 ; i < rows_get() ; i++)
        {
            for(int j = 0 ; j < cols_get() ; j++)
            {
                delete pIT[j * _iNewRows + i];
                pIT[j * _iNewRows + i] = m_plData[j * rows_get() + i];
            }
        }
        delete[] m_plData;
        m_plData	= pIT;

        m_iRows = _iNewRows;
        m_iCols	= _iNewCols;
        m_iSize = m_iRows * m_iCols;
        return true;
    }

    bool Cell::append(int _iRows, int _iCols, Cell *_poSource)
    {
        return true;
    }

    bool Cell::operator==(const InternalType& it)
    {
        if(const_cast<InternalType &>(it).getType() != RealCell)
        {
            return false;
        }

        Cell* pC = const_cast<InternalType &>(it).getAsCell();

        if(pC->rows_get() != rows_get() || pC->cols_get() != cols_get())
        {
            return false;
        }

        for(int i = 0 ; i < m_iRows ; i++)
        {
            for(int j = 0 ; j < m_iCols ; j++)
            {
                if(get(i,j) != pC->get(i,j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    bool Cell::operator!=(const InternalType& it)
    {
        return !(*this == it);
    }

    Cell* Cell::extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
    {
        Cell* pOut		= NULL;
        int iRowsOut	= 0;
        int iColsOut	= 0;

        //check input param

        if(	(_bAsVector && _piMaxDim[0] > size_get()) ||
            (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            (_bAsVector == false && _piMaxDim[1] > cols_get()))
        {
            return NULL;
        }

        if(_bAsVector)
        {//a([])
            if(rows_get() == 1)
            {
                iRowsOut	= 1;
                iColsOut	= _piDimSize[0];
            }
            else
            {
                iRowsOut	= _piDimSize[0];
                iColsOut	= 1;
            }
        }
        else
        {//a([],[])
            iRowsOut		= _piDimSize[0];
            iColsOut		= _piDimSize[1];
        }

        pOut					= new Cell(iRowsOut, iColsOut);


        if(_bAsVector)
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                pOut->set(i, get(_piSeqCoord[i] - 1));
            }
        }
        else
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                //convert vertical indexes to horizontal indexes
                int iCurIndex		= (i % iColsOut) * iRowsOut + (i / iColsOut);
                int iInIndex		= (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
                pOut->set(iCurIndex, get(iInIndex));
            }
        }
        return pOut;
    }

    vector<InternalType*> Cell::extract_cell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector)
    {
        vector<InternalType*> vectRet;

        //check input param
        if(	(_bAsVector && _piMaxDim[0] > size_get()) ||
            (_bAsVector == false && _piMaxDim[0] > rows_get()) ||
            (_bAsVector == false && _piMaxDim[1] > cols_get()))
        {
            return vectRet;
        }

        if(_bAsVector)
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                vectRet.push_back(m_plData[_piSeqCoord[i] - 1]);
            }
        }
        else
        {
            for(int i = 0 ; i < _iSeqCount ; i++)
            {
                //convert vertical indexes to horizontal indexes
                int iInIndex = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
                vectRet.push_back(m_plData[iInIndex]);
            }
        }

        return vectRet;
    }

    bool Cell::insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
    {
        int iNewRows = rows_get();
        int iNewCols = cols_get();
        //check input size
        if(_bAsVector == false)
        {
            if(rows_get() < _piMaxDim[0] || cols_get() < _piMaxDim[1])
            {//compute new dimensions
                iNewRows = Max(_piMaxDim[0], rows_get());
                iNewCols = Max(_piMaxDim[1], cols_get());
            }
        }
        else
        {
            if(size_get() < _piMaxDim[0])
            {
                if(rows_get() == 1 || size_get() == 0)
                {
                    iNewRows = 1;
                    iNewCols = _piMaxDim[0];
                }
                else if(cols_get() == 1)
                {
                    iNewRows = _piMaxDim[0];
                    iNewCols = 1;
                }
                else
                {
                    return false;
                }
            }
        }

        //check if the size of _poSource is compatible with the size of the variable
        if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
        {
            return false;
        }
        else if(_bAsVector == true && (iNewRows * iNewCols < _poSource->size_get()))
        {
            return false;
        }


        //check if the count of values is compatible with indexes
        if(_poSource->size_get() != 1 && _poSource->size_get() != _iSeqCount)
        {
            return false;
        }


        if(_poSource->isCell())
        {
            Cell *pIn = _poSource->getAsCell();

            //Only resize after all tests !
            if(resize(iNewRows, iNewCols) == false)
            {
                return false;
            }

            ////variable can receive new values.
            if(pIn->size_get() == 1)
            {//a(?) = x
                if(_bAsVector)
                {//a([]) = R
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        set(_piSeqCoord[i] - 1, pIn->get(0));
                    }
                }
                else
                {//a([],[]) = R
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
                        set(iPos, pIn->get(0));
                    }
                }
            }
            else
            {//a(?) = [x]
                if(_bAsVector)
                {//a([]) = [R]
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        set(_piSeqCoord[i] - 1, pIn->get(i));
                    }
                }
                else
                {//a([],[]) = [R]
                    for(int i = 0 ; i < _iSeqCount ; i++)
                    {
                        int iPos = (_piSeqCoord[i * 2] - 1) + (_piSeqCoord[i * 2 + 1] - 1) * rows_get();
                        int iTempR = i / pIn->cols_get();
                        int iTempC = i % pIn->cols_get();
                        int iNew_i = iTempR + iTempC * pIn->rows_get();

                        set(iPos, pIn->get(iNew_i));
                    }
                }
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    Cell* Cell::insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
    {
        Cell *pCell = NULL;

        if(_bAsVector)
        {
            pCell = new Cell(1, _piMaxDim[0]);
        }
        else
        {
            pCell = new Cell(_piMaxDim[0], _piMaxDim[1]);
        }

        if(pCell->insert_cell(_iSeqCount, _piSeqCoord, _piMaxDim, _poSource, _bAsVector) == false)
        {
            delete pCell;
            return NULL;
        }

        return pCell;
    }

    bool Cell::insert_cell(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector)
    {
        int iNewRows = rows_get();
        int iNewCols = cols_get();
        //check input size
        if(_bAsVector == false)
        {
            if(rows_get() < _piMaxDim[0] || cols_get() < _piMaxDim[1])
            {//compute new dimensions
                iNewRows = Max(_piMaxDim[0], rows_get());
                iNewCols = Max(_piMaxDim[1], cols_get());
            }
        }
        else
        {
            if(size_get() < _piMaxDim[0])
            {
                if(rows_get() == 1 || size_get() == 0)
                {
                    iNewRows = 1;
                    iNewCols = _piMaxDim[0];
                }
                else if(cols_get() == 1)
                {
                    iNewRows = _piMaxDim[0];
                    iNewCols = 1;
                }
                else
                {
                    return false;
                }
            }
        }

        //check if the size of _poSource is compatible with the size of the variable
        if(_bAsVector == false && (iNewRows < _poSource->rows_get() || iNewCols < _poSource->cols_get()))
        {
            return false;
        }
        else if(_bAsVector == true && (iNewRows * iNewCols < _poSource->size_get()))
        {
            return false;
        }

        //Only resize after all tests !
        if(resize(iNewRows, iNewCols) == false)
        {
            return false;
        }

        ////variable can receive new values.
        return set(_piSeqCoord[0] - 1, _poSource);;
    }
}
