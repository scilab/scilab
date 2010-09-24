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
                                    Double(size_t _iRows, size_t _iCols, bool _bComplex = false);
                                    Double(size_t _iRows, size_t _iCols, double **_pdblReal);
                                    Double(size_t _iRows, size_t _iCols, double **_pdblReal, double **_pdblImg);
        static Double*              Empty();

        /*data management*/
        GenericType*                get_col_value(size_t _iPos);
        double*                     real_get() const;
        double                      real_get(size_t _iRows, size_t _iCols) const;
        double*                     img_get() const;
        double                      img_get(size_t _iRows, size_t _iCols) const;

        bool                        real_set(double *_pdblReal);
        bool                        real_set(int* _piReal); //to translate int to double matrix
        bool                        img_set(double *_pdblImg);

        bool                        val_set(size_t _iRows, size_t _iCols, double _dblReal);
        bool                        val_set(size_t _iRows, size_t _iCols, double _dblReal, double _dblImg);

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
        bool                        append(size_t _iRows, size_t _iCols, Double *_poSource);
        bool                        fillFromCol(size_t _iCols, Double *_poSource);
        bool                        fillFromRow(size_t _iRows, Double *_poSource);

        bool                        resize(size_t _iNewRows, size_t _iNewCols);
        bool                        insert(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, GenericType* _poSource, bool _bAsVector);
        static Double*              insert_new(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, Double* _poSource, bool _bAsVector);
        Double*                     extract(size_t _iSeqCount, size_t* _piSeqCoord, size_t* _piMaxDim, size_t* _piDimSize, bool _bAsVector);


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
        void                        CreateDouble(size_t _iRows, size_t _iCols, double **_pdblReal, double **_pdblImg);

    private :
        double*                       m_pdblReal;
        double*                       m_pdblImg;
        bool                          m_bComplex;
        size_t                        m_iSizeMax;
    };
}

#endif /* !__DOUBLE_HH__ */
