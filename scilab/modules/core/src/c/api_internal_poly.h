/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#ifndef __INTERNAL_POLY_API__
#define __INTERNAL_POLY_API__

//internal poly functions
int getCommonMatrixOfPoly(int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
int createCommonMatrixOfPoly(int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
int fillCommonMatrixOfPoly(int* _piAddress, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen);
int createCommonNamedMatrixOfPoly(char* _pstName, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
int readCommonNamedMatrixOfPoly(char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
int createCommonMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
int readCommonMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);


#endif /* __INTERNAL_POLY_API__ */

