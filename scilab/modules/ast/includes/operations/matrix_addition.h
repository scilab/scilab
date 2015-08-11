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

int iAddScilabPolynomToScilabPolynom(double* _pCoef1, int _iRank1, double* _pCoef2, int _iRank2, double* _pCoefOut, int _iRanOut);
int iAddScilabPolynomToComplexPoly(double* _pCoef1, int _iRank1, double* _pCoefR2, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);
int iAddComplexPolyToComplexPoly(double* _pCoef1R, double* _pCoef1I, int _iRank1, double* _pCoef2R, double* _pCoef2I, int _iRank2, double* _pCoefOutR, double* _pCoefOutI, int _iRanOut);


/*tools functions*/
int GetSize(int* _piDims, int _iDims);

#endif /* __MATRIX_ADD_H__ */
