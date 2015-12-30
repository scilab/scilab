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

std::string GenericType::DimToString()
{
    std::ostringstream ostr;
    ostr << getRows() << "x" << getCols();
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
    if (m_piDims)
    {
        for (int i = 0; i < getDims(); i++)
        {
            if (m_piDims[i] != -1)
            {
                return false;
            }
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

