/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __TYPES_HXX__
#define __TYPES_HXX__

#include <vector>
#include <iostream>
#include <string>
#include "internal.hxx"

extern "C"
{
#include "core_math.h"
}

#define MAX_DIMS 32
namespace types
{
/*
** Type
*/
class EXTERN_AST GenericType : public InternalType
{

protected :
    int                         m_iRows;
    int                         m_iCols;
    int                         m_iSize;
    int                         m_iSizeMax;
    int                         m_piDims[MAX_DIMS];
    int                         m_iDims;


    GenericType() : InternalType(), m_iRows(0), m_iCols(0), m_iSize(0), m_iSizeMax(0), m_iDims(0) {}

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

    bool                        isScalar();

    /*commun functions*/
    inline int                  getCols()
    {
        return m_iCols;
    }

    inline int                  getRows()
    {
        return m_iRows;
    }

    inline int                  getSize()
    {
        return m_iSize;
    }

    inline int                  getDims()
    {
        return m_iDims;
    }

    inline int*                 getDimsArray()
    {
        return m_piDims;
    }

    int                         getVarMaxDim(int _iCurrentDim, int _iMaxDim);

    std::wstring                DimToString();

    inline bool                 isGenericType()
    {
        return true;
    }

    virtual GenericType*        getColumnValues(int /*_iPos*/)
    {
        return NULL;
    }

    bool                        isIdentity(void);
    virtual bool                isAssignable(void)
    {
        return true;
    }

    virtual ScilabType          getType(void)
    {
        return ScilabGeneric;
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

    virtual GenericType*        clone(void)
    {
        return NULL;
    }

    virtual GenericType*        resize(int* /*_piDims*/, int /*_iDims*/)
    {
        return NULL;
    }

    virtual GenericType*        resize(int /*_iNewRows*/, int /*_iNewCols*/)
    {
        return NULL;
    }

    virtual GenericType*        reshape(int* /*_piDims*/, int /*_iDims*/)
    {
        return NULL;
    }

    virtual GenericType*        reshape(int /*_iNewRows*/, int /*_iNewCols*/)
    {
        return NULL;
    }

    virtual GenericType*        insert(typed_list* /*_pArgs*/, InternalType* /*_pSource*/)
    {
        return NULL;
    }

    virtual GenericType*        insertNew(typed_list* /*_pArgs*/)
    {
        return NULL;
    }

    virtual GenericType*        remove(typed_list* /*_pArgs*/)
    {
        return NULL;
    }

    virtual bool                extract(const std::wstring& /*name*/, InternalType *& /*out*/)
    {
        return false;
    }

    virtual InternalType*       extract(typed_list* /*_pArgs*/)
    {
        return NULL;
    }

};
}
#endif /* !__TYPES_HXX__ */
