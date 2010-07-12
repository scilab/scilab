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

#ifndef __DOUBLE_HH__
#define __DOUBLE_HH__

#include "types.hxx"

using std::string;

namespace types
{
    class Double : public GenericType
    {
    public :
        virtual						~Double();

                                    Double(double _dblReal);
                                    Double(double _dblReal, double _dblImg);
                                    Double(int _iRows, int _iCols, bool _bComplex = false);
                                    Double(int _iRows, int _iCols, double **_pdblReal);
                                    Double(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);

        /*data management*/
        GenericType*                get_col_value(int _iPos);
        double*                     real_get() const;
        double                      real_get(int _iRows, int _iCols) const;
        double*                     img_get() const;
        double                      img_get(int _iRows, int _iCols) const;

        bool                        real_set(double *_pdblReal);
        bool                        img_set(double *_pdblImg);

        bool                        val_set(int _iRows, int _iCols, double _dblReal);
        bool                        val_set(int _iRows, int _iCols, double _dblReal, double _dblImg);

        /*zero or one set filler*/
        bool                        zero_set();
        bool                        one_set();

        /*Config management*/
        void                        whoAmI();
        bool                        isEmpty();
        bool                        isComplex();
        void                        complex_set(bool _bComplex);

        Double*                     getAsDouble(void);
        wstring                     toString(int _iPrecision, int _iLineLen);

        Double*                     clone();
        bool                        append(int _iRows, int _iCols, Double *_poSource);
        bool                        fillFromCol(int _iCols, Double *_poSource);
        bool                        fillFromRow(int _iRows, Double *_poSource);

        bool                        resize(int _iNewRows, int _iNewCols);
        bool                        insert(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static Double*              insert_new(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, Double* _poSource, bool _bAsVector);
        Double*                     extract(int _iSeqCount, int* _piSeqCoord, int* _piMaxDim, int* _piDimSize, bool _bAsVector);


        bool                        operator==(const InternalType& it);
        bool                        operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual std::wstring        getTypeStr() {return L"constant";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual std::wstring        getShortTypeStr() {return L"s";}
    protected :
        RealType                    getType(void);

        /*clean values array*/
        void                        real_delete();
        void                        img_delete(bool _bSetReal = false);
        void                        all_delete(bool _bSetReal = false);

        /*Internal "constructor*/
        void                        CreateDouble(int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);

    private :
        double*                       m_pdblReal;
        double*                       m_pdblImg;
        bool                          m_bComplex;
        int                           m_iSizeMax;
    };
}

#endif /* !__DOUBLE_HH__ */
