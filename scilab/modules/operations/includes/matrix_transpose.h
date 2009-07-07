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

#ifndef __TRANSPOSE_H__
#define __TRANSPOSE_H__

#include "operation.hxx"

EXTERN_OP void vTransposeRealMatrix(
			double *_pdblRealIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut);

EXTERN_OP void vTransposeComplexMatrix(
			double *_pdblRealIn,	double *_pdblImgIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut,	double *_pdblImgOut);

EXTERN_OP void vTransposeDoubleComplexMatrix(
			doublecomplex *_poIn, 
			int _iRowsIn,			int _iColsIn, 
			doublecomplex *_poOut);

EXTERN_OP void vPretransposeRealMatrix(
			double *_pdblRealIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut);

EXTERN_OP void vPretransposeComplexMatrix(
			double *_pdblRealIn,	double *_pdblImgIn, 
			int _iRowsIn,			int _iColsIn, 
			double *_pdblRealOut,	double *_pdblImgOut);

#endif /* __TRANSPOSE_H__ */