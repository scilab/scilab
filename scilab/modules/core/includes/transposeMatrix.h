/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
double *transposeMatrixDouble(int C, int L, double *MatrixDouble);

/**
* transpose a (int*)
* param[in] C
* param[in] L
* param[in] MatrixInt
* return transposed matrix
*/
int *transposeMatrixInt(int C, int L, int *MatrixInt);

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
