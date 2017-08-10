/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <sstream>
#include "types.hxx"

extern "C"
{
#include "localization.h"
#include "charEncoding.h"
#include "os_string.h"
}



namespace types
{

static bool isCoordIndex(int _iIndex, int* _piCoord, int _iCoordCount)
{
    bool bFind = false;
    for (int j = 0 ; j < _iCoordCount ; j++)
    {
        if (_piCoord[j] == _iIndex)
        {
            bFind = true;
            break;
        }
    }
    return bFind;
}

std::wstring GenericType::DimToString()
{
    std::wostringstream ostr;
    ostr << getRows() << L"x" << getCols();
    return ostr.str();
}

bool GenericType::isScalar() //2 dims and each dim == 1
{
    if (m_iDims == 2 && m_piDims[0] == 1 && m_piDims[1] == 1)
    {
        return true;
    }

    return false;
}

bool GenericType::isIdentity(void)
{
    for (int i = 0; i < getDims(); i++)
    {
        if (m_piDims[i] != -1)
        {
            return false;
        }
    }

    return true;
}

bool GenericType::hasAllIndexesOfRow(int _iRow, int* _piCoord, int _iCoordCount)
{
    bool bAll = true;
    for (int i = 0; i < getCols(); i++)
    {
        //+1 to keep 1 based index
        int iIdx = _iRow + i * getCols() + 1;


        if (isCoordIndex(iIdx, _piCoord, _iCoordCount) == false)
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
    for (int i = 0; i < getRows(); i++)
    {
        //+1 to keep 1 based index
        int iIdx = i + _iCol * getRows() + 1;


        if (isCoordIndex(iIdx, _piCoord, _iCoordCount) == false)
        {
            bAll = false;
            break;
        }
    }
    return bAll;
}

int GenericType::getVarMaxDim(int _iCurrentDim, int _iMaxDim)
{
    int iDim = 1;
    if (m_iDims != 0)
    {
        if (_iMaxDim < m_iDims)
        {
            //view as "vector", not all dimensions are used
            if (_iCurrentDim == (_iMaxDim - 1))
            {
                for (int i = _iCurrentDim; i < m_iDims; i++)
                {
                    iDim *= m_piDims[i];
                }
            }
            else
            {
                iDim = m_piDims[_iCurrentDim];
            }
        }
        else if (_iCurrentDim >= m_iDims)
        {
            iDim = 1;
        }
        else
        {
            //normal view, all dimensions are used
            iDim = m_piDims[_iCurrentDim];
        }
    }
    else
    {
        //for non ArrayOf derived types, like list, tlist, mlist
        iDim = getSize();
    }

    return iDim;
}
}

