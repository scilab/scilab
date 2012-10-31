/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_HXX__
#define __TYPES_HXX__

#include <vector>
#include <iostream>
#include <string>
#include "internal.hxx"

extern "C"
{
#include "core_math.h"
#include "dynlib_types.h"
}

namespace types
{
/*
** Type
*/
class TYPES_IMPEXP GenericType : public InternalType
{

protected :
    int                         m_iRows;
    int                         m_iCols;
    int                         m_iSize;
    int                         m_iSizeMax;
    int                         m_piDims[50];
    int                         m_iDims;


    GenericType() : InternalType(), m_iRows(0), m_iCols(0), m_iSize(0), m_iDims(0) {}

    bool                        hasAllIndexesOfRow(int _iRow, int* _piCoord, int _iCoordCount);
    bool                        hasAllIndexesOfCol(int _iCol, int* _piCoord, int _iCoordCount);

public :
    virtual                     ~GenericType() {}
    void                        whoAmI(void)
    {
        std::cout << "types::GenericType";
    }
    virtual bool                isComplex()
    {
        return false;
    }
    virtual bool                isScalar()
    {
        return false;
    }
    /*commun functions*/
    int                         getCols();
    int                         getRows();
    int                         getSize();
    int                         getDims();
    int*                        getDimsArray();
    int                         getVarMaxDim(int _iCurrentDim, int _iMaxDim);

    std::wstring                DimToString();

    bool                        isGenericType()
    {
        return true;
    }

    /* FIXME : should be : virtual GenericType*	get(int _iPos) = 0; */
    virtual GenericType*        getColumnValues(int _iPos) = 0;

    bool                        isIdentity(void);
    virtual bool                isAssignable(void)
    {
        return true;
    }

    virtual RealType            getType(void)
    {
        return RealGeneric;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring        getTypeStr()
    {
        return L"generictype";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring        getShortTypeStr()
    {
        return L"";
    }

    virtual InternalType*       clone(void)
    {
        return NULL;
    }

    virtual bool                resize(int* _piDims, int _iDims)
    {
        return false;
    }
    virtual bool                resize(int _iNewRows, int _iNewCols)
    {
        return false;
    }

    virtual bool                reshape(int* _piDims, int _iDims)
    {
        return false;
    }
    virtual bool                reshape(int _iNewRows, int _iNewCols)
    {
        return false;
    }

};

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

static int get_max_size(int* _piDims, int _iDims)
{
    if (_iDims == 0)
    {
        return 0;
    }

    int iMax = 1;
    for (int i = 0 ; i < _iDims ; i++)
    {
        iMax *= _piDims[i];
    }
    return iMax;
}
}
#endif /* !__TYPES_HXX__ */
