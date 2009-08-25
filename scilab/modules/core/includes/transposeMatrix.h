/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __TRANSPOSEMATRIX_H__
#define __TRANSPOSEMATRIX_H__

/**
* transpose a (double*)
* param[in] C
* param[in] L
* param[in] MatrixDouble
* return transposed matrix
*/
double *transposeMatrixDouble(int C,int L,double *MatrixDouble);

/**
* transpose a (int*)
* param[in] C
* param[in] L
* param[in] MatrixInt
* return transposed matrix
*/
int *transposeMatrixInt(int C,int L,int *MatrixInt);

/**
* transpose a (char **)
* param[in] C
* param[in] L
* param[in] MatrixStr
* return transposed matrix
*/
//char **transposeMatrixStrings(int C,int L,char **MatrixStr);

#endif /* __TRANSPOSEMATRIX_H__ */
/*--------------------------------------------------------------------------*/
