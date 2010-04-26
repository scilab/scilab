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
#include "cell.hxx"
#include "double.hxx"

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
        return m_iRows * m_iCols;
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
    std::string Cell::toString(int _iPrecision, int _iLineLen)
    {
        std::ostringstream ostr;
        ostr << " (" << rows_get() << "," << cols_get() << ")" << std::endl;
        return ostr.str();
    }

    bool Cell::resize(int _iNewRows, int _iNewCols)
    {
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
}
