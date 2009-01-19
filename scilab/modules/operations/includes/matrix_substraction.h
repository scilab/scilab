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

#ifndef __MATRIX_SUBSTRACT_H__
#define __MATRIX_SUBSTRACT_H__

#include "operation.hxx"

EXTERN_OP int iSubstractRealMatrixToRealScalar(double* _pdblReal1, int _iRows, int _iCols, double _dblReal2, double* _pdblRealOut);
EXTERN_OP int iSubstractComplexMatrixToRealScalar(double* _pdblReal1, double* _pdblImg1, int _iRows, int _iCols, double _dblReal2, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iSubstractRealMatrixToComplexScalar(double* _pdblReal1, int _iRows, int _iCols, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iSubstractComplexMatrixToComplexScalar(double* _pdblReal1, double* _pdblImg1, int _iRows, int _iCols, double _dblReal2, double _dblImg2, double* _pdblRealOut, double* _pdblImgOut);

EXTERN_OP int iSubstractRealScalarToRealMatrix(double _dblReal1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut);
EXTERN_OP int iSubstractComplexScalarToRealMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iSubstractRealScalarToComplexMatrix(double _dblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iSubstractComplexScalarToComplexMatrix(double _dblReal1, double _dblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);

EXTERN_OP int iSubstractRealMatrixToRealMatrix(double* _pdblReal1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut);
EXTERN_OP int iSubstractComplexMatrixToRealMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iSubstractRealMatrixToComplexMatrix(double* _pdblReal1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);
EXTERN_OP int iSubstractComplexMatrixToComplexMatrix(double* _pdblReal1, double* _pdblImg1, double* _pdblReal2, double* _pdblImg2, int _iRows, int _iCols, double* _pdblRealOut, double* _pdblImgOut);

#endif /* __MATRIX_SUBSTRACT_H__ */