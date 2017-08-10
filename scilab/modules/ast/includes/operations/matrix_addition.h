/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#ifndef __MATRIX_ADD_H__
#define __MATRIX_ADD_H__

int iAddScilabPolynomToScilabPolynom(double* _pCoef1, int _iRank1, double* _pCoef2, int _iRank2, double* _pCoefOut, int _iRanOut);
int iAddScilabPolynomToComplexPoly(double* _pCoef1, int _iRank1, double* _pCoefR2, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);
int iAddComplexPolyToComplexPoly(double* _pCoef1R, double* _pCoef1I, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);


/*tools functions*/
int GetSize(int* _piDims, int _iDims);

#endif /* __MATRIX_ADD_H__ */
