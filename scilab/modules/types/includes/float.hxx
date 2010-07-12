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

namespace types
{
    class Float : public GenericType
    {
    public :
        virtual				~Float();
                            Float(float _fReal);
                            Float(int _iRows, int _iCols, bool _bComplex = false);
                            Float(int _iRows, int _iCols, float **_pfReal);
                            Float(int _iRows, int _iCols, float **_pfReal, float **_pfImg);

        Float*              clone();

        /*data management*/
        GenericType*        get_col_value(int _iPos);

        float*              real_get() const;
        float               real_get(int _iRows, int _iCols) const;
        float*              img_get() const;
        float               img_get(int _iRows, int _iCols) const;

        bool                real_set(float *_pfReal);
        bool                img_set(float *_pfImg);


        bool                val_set(int _iRows, int _iCols, float _fReal);
        bool                val_set(int _iRows, int _iCols, float _fReal, float _fImg);

        /*zero or one set filler*/
        bool                zero_set();
        bool                one_set();

        /*Config management*/
        void                whoAmI();
        bool                isComplex();

        Float*				getAsFloat(void);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring getTypeStr() {return L"float";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring getShortTypeStr() {return L"f";}
    protected :
        RealType            getType(void);

        /*clean values array*/
        void                real_delete();
        void                img_delete(bool _bSetReal = false);
        void                all_delete(bool _bSetReal = false);

        /*Internal "constructor*/
        void                CreateFloat(int _iRows, int _iCols, float **_pfReal, float **_pfImg);
        std::wstring        toString(int _iPrecision, int _iLineLen);

        bool                operator==(const InternalType& it);
        bool                operator!=(const InternalType& it);

    private :
        float*              m_pfReal;
        float*              m_pfImg;
        bool                m_bComplex;
    };
}

#endif /* !__FLOAT_HH__ */
