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
#include "types.hxx"

namespace types
{
    bool InternalType::isGenericType(void)
    {
        return (dynamic_cast<GenericType*>(this) != NULL);
    }

	/*commun functions*/
	int GenericType::cols_get()
	{
		return m_iCols;
	}

	int GenericType::rows_get()
	{
		return m_iRows;
	}

	int GenericType::size_get()
	{
		return m_iSize;
	}

	wstring GenericType::DimToString()
	{
		wostringstream ostr;
		ostr << L"(" << rows_get() << L" x " << cols_get() << L")";
		return ostr.str();
	}

	bool GenericType::isIdentity(void)
	{
		if(m_iRows == -1 && m_iCols == -1)
		{
			return true;
		}
		return false;
	}

    bool GenericType::hasAllIndexesOfRow(int _iRow, int* _piCoord, int _iCoordCount)
    {
        bool bAll = true;
        for(int i = 0 ; i < cols_get() ; i++)
        {
            bool bFind = false;
            //+1 to keep 1 based index
            int iIdx = _iRow + i * cols_get() + 1;


            if(isCoordIndex(iIdx, _piCoord, _iCoordCount) == false)
            {
                bAll = false;
                break;
            }
        }
        return bAll;
    }

    bool GenericType::hasAllIndexesOfCol(int _iCol, int* _piCoord, int _iCoordCount)
    {
        bool bAll = true;
        for(int i = 0 ; i < rows_get() ; i++)
        {
            bool bFind = false;
            //+1 to keep 1 based index
            int iIdx = i + _iCol * rows_get() + 1;


            if(isCoordIndex(iIdx, _piCoord, _iCoordCount) == false)
            {
                bAll = false;
                break;
            }
        }
        return bAll;
    }

    static bool isCoordIndex(int _iIndex, int* _piCoord, int _iCoordCount)
    {
        bool bFind = false;
        for(int j = 0 ; j < _iCoordCount ; j++)
        {
            if(_piCoord[j] == _iIndex)
            {
                bFind = true;
                break;
            }
        }

        return bFind;
    }
}

