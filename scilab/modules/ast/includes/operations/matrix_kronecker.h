/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2011 - DIGITEO - Cedric Delamarre
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

#ifndef __MATRIX_KRONECKER_H__
#define __MATRIX_KRONECKER_H__

void vKronR(double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
            double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
            double* _pdblDataOut, int _iIncOut);

void vKronC(double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
            double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
            double* _pdblRealOut, double* _pdblImgOut, int _iIncOut);

int conv_real_input(double* _pdblData, int _iSize);
int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize);

#endif /* !__MATRIX_KRONECKER_H__ */