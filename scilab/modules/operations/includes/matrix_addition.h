/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __MATRIX_ADD_H__
#define __MATRIX_ADD_H__

#include "operation.hxx"

EXTERN_OP int iAddRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblOut);
EXTERN_OP int iAddRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double *_pdblImgOut);
EXTERN_OP int iAddComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows2, int _iCols2, double* _pdblRealOut, double *_pdblImgOut);
EXTERN_OP int iAddComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows2, int _iCols2, double* _pdblRealOut, double *_pdblImgOut);
EXTERN_OP int iAddRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut);
EXTERN_OP int iAddRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iAddComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);




EXTERN_OP int iAddRealPolyToRealPoly(double* _pCoef1, int _iRank1, double* _pCoef2, int _iRank2, double* _pCoefOut, int _iRanOut);
EXTERN_OP int iAddRealPolyToComplexPoly(double* _pCoef1, int _iRank1, double* _pCoefR2, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);
EXTERN_OP int iAddComplexPolyToComplexPoly(double* _pCoef1R, double* _pCoef1I, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);

#endif /* __MATRIX_ADD_H__ */