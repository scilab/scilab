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

#ifndef __MATRIXPOLY_HH__
#define __MATRIXPOLY_HH__

#include "poly.hxx"
#include "double.hxx"

#ifdef isComplex
#undef isComplex
#endif

namespace types
{
    class MatrixPoly : public GenericType
    {
    public :
                                MatrixPoly();
                                MatrixPoly(wstring _szVarName, size_t _iRows, size_t _iCols, size_t *_piRank);
        virtual                 ~MatrixPoly();

        // FIXME : Should not return NULL
        MatrixPoly*             clone() { std::cout << "MatrixPoly::clone" << std::endl; return NULL; }

        GenericType*            get_col_value(size_t _iPos);

        Poly*                   poly_get(size_t _iRows, size_t _iCols);
        Poly*                   poly_get(size_t _iIdx);
        bool                    poly_set(size_t _iRows, size_t _iCols, Double *_pdblCoef);
        bool                    poly_set(size_t _iIdx, Double *_pdblCoef);

        MatrixPoly*             getAsPoly();
        RealType                getType();
        void                    whoAmI(void);
        bool                    isComplex(void);
        void                    complex_set(bool _bComplex);
        wstring                 var_get();
        bool                    rank_get(size_t *_piRank);
        size_t                  rank_max_get(void);
        Double*                 evaluate(Double* _pdblValue);
        void                    update_rank(void);
        Double*                 coef_get(void);
        void                    coef_set(Double *_pCoef);
        Double*                 extract_coef(size_t _iRank);
        bool                    insert_coef(size_t _iRank, Double* _pCoef);

        bool                    insert(size_t _iRows, size_t _iCols, MatrixPoly* _poSource);

        wstring                 toString(int _iPrecison, int _iLineLen);
        wstring                 GetRowString(int _iPrecison, int _iLineLen, bool _bComplex);
        wstring                 GetColString(int _iPrecison, int _iLineLen, bool _bComplex);
        wstring                 GetMatrixString(int _iPrecison, int _iLineLen, bool _bComplex);


        MatrixPoly&             operator= (MatrixPoly& poPoly);

        bool                    operator==(const InternalType& it);
        bool                    operator!=(const InternalType& it);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring         getTypeStr() {return L"matrixpoly";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring         getShortTypeStr() {return L"p";}

    private :
        Poly*                   m_poPolyMatrix;
        bool                    m_bComplex;
        wstring                 m_szVarName;
    };
}
#endif /* !__MATRIXPOLY_HH__ */
