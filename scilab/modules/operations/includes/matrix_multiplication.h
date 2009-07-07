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

#ifndef __MATRIX_MULT__
#define __MATRIX_MULT__

#include "operation.hxx"

EXTERN_OP int iMultiComplexMatrixByComplexMatrix(
		double *_pdblReal1,		double *_pdblImg1, int _iRows1, int _iCols1,
		double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_OP int iMultiRealMatrixByRealMatrix(
		double *_pdblReal1,	int _iRows1, int _iCols1,
		double *_pdblReal2,	int _iRows2, int _iCols2,
		double *_pdblRealOut);

EXTERN_OP int iMultiRealMatrixByComplexMatrix(
		double *_pdblReal1,		int _iRows1, int _iCols1,
		double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_OP int iMultiComplexMatrixByRealMatrix(
		double *_pdblReal1,		double *_pdblImg1,	int _iRows1, int _iCols1,
		double *_pdblReal2,							int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_OP int iMultiRealScalarByRealMatrix(
		double _dblReal1,
		double *_pdblReal2,	int _iRows2, int _iCols2,
		double *_pdblRealOut);

EXTERN_OP int iMultiRealScalarByComplexMatrix(
		double _dblReal1,
		double *_pdblReal2,	double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut, double *_pdblImgOut);

EXTERN_OP int iMultiComplexScalarByRealMatrix(
		double _dblReal1,		double _dblImg1,
		double *_pdblReal2,		int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_OP int iMultiComplexScalarByComplexMatrix(
		double _dblReal1,		double _dblImg1,
		double *_pdblReal2,		double *_pdblImg2, int _iRows2, int _iCols2,
		double *_pdblRealOut,	double *_pdblImgOut);


/*polynomial multiplications*/
EXTERN_OP int iMultiRealPolyByRealPoly(
		double *_pdblReal1,	int _iRank1,
		double *_pdblReal2,	int _iRank2,
		double *_pdblRealOut, int _iRankOut);
		
EXTERN_OP int iMultiComplexPolyByRealPoly(
		double *_pdblReal1,	double *_pdblImg1, int _iRank1,
		double *_pdblReal2,	int _iRank2,
		double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

EXTERN_OP int iMultiRealPolyByComplexPoly(
		double *_pdblReal1,	int _iRank1,
		double *_pdblReal2,	double *_pdblImg2, int _iRank2,
		double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

EXTERN_OP int iMultiComplexPolyByComplexPoly(
		double *_pdblReal1,	double *_pdblImg1, int _iRank1,
		double *_pdblReal2,	double *_pdblImg2, int _iRank2,
		double *_pdblRealOut, double *_pdblImgOut, int _iRankOut);

#endif /* __MATRIX_MULT__ */