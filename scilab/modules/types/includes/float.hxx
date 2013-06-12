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


#ifndef __FLOAT_HH__
#define __FLOAT_HH__

#include "types.hxx"
#include "dynlib_types.h"

namespace types
{
class TYPES_IMPEXP Float : public GenericType
{
public :
    virtual				~Float();
    Float(float _fReal);
    Float(int _iRows, int _iCols, bool _bComplex = false);
    Float(int _iRows, int _iCols, float **_pfReal);
    Float(int _iRows, int _iCols, float **_pfReal, float **_pfImg);

    InternalType*       clone();

    /*data management*/
    GenericType*        getColumnValues(int _iPos);

    float*              getReal() const;
    float               getReal(int _iRows, int _iCols) const;
    float*              getImg() const;
    float               getImg(int _iRows, int _iCols) const;

    bool                set(float *_pfReal);
    bool                setImg(float *_pfImg);


    bool                setValue(int _iRows, int _iCols, float _fReal);
    bool                setValue(int _iRows, int _iCols, float _fReal, float _fImg);

    /*zero or one set filler*/
    bool                setZeros();
    bool                setOnes();

    /*Config management*/
    void                whoAmI();
    bool                isComplex();

    bool                isFloat()
    {
        return true;
    }

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring getTypeStr()
    {
        return L"float";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring getShortTypeStr()
    {
        return L"f";
    }
protected :
    RealType            getType(void);

    /*clean values array*/
    void                deleteReal();
    void                deleteImg(bool _bSetReal = false);
    void                deleteAll(bool _bSetReal = false);

    /*Internal "constructor*/
    void                createFloat(int _iRows, int _iCols, float **_pfReal, float **_pfImg);
    bool                toString(std::wostringstream& ostr);

    bool                operator==(const InternalType& it);
    bool                operator!=(const InternalType& it);

private :
    float*              m_pfReal;
    float*              m_pfImg;
    bool                m_bComplex;
};
}

#endif /* !__FLOAT_HH__ */
