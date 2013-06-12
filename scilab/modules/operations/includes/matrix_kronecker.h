/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "dynlib_operations.hxx"

EXTERN_OP void vKronR(double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
                      double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
                      double* _pdblDataOut, int _iIncOut);

EXTERN_OP void vKronC(double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
                      double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
                      double* _pdblRealOut, double* _pdblImgOut, int _iIncOut);

EXTERN_OP int conv_real_input(double* _pdblData, int _iSize);
EXTERN_OP int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize);

