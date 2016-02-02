/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
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

#ifndef __BASIC_FUNCTION__
#define __BASIC_FUNCTION__

#include <stdlib.h>
#include <math.h>

#include "dynlib_elementary_functions.h"
#include "BOOL.h"
#include "elem_common.h"

#include "abs.h"
#include "cos.h"
#include "sin.h"
#include "log.h"
#include "tan.h"
#include "exp.h"
#include "pythag.h"
#include "sqrt.h"
#include "sign.h"

#define BY_ROWS			1
#define BY_COLS			2
#define BY_ALL			0
#define	BY_MTLB			-1

ELEMENTARY_FUNCTIONS_IMPEXP double dceilsEx(double _dblVal, int _iPrecision);
ELEMENTARY_FUNCTIONS_IMPEXP double dceils(double _dblVal);
ELEMENTARY_FUNCTIONS_IMPEXP double wasums(int _iNbElem, double* _pdblReal, double* _pdblImg);

ELEMENTARY_FUNCTIONS_IMPEXP void vCupro(int _iNbElem, double* _piIn, double* _piOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vCuproi(int _iNbElem, double* _piRealIn, double* _piImgIn, double* _piRealOut, double* _piImgOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vDvmul(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vWvmul(int _iNbElem, double* _piRealIn1, double* _piImgIn1, double* _piRealIn2, double* _piImgIn2, int _iIncIn1, int _iIncIn2, double* _piRealOut, double* _piImgOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vCusum(int _iNbElem, double *_dblIn, double *_dblOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vDadd(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vDless(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut);
ELEMENTARY_FUNCTIONS_IMPEXP void vDset(int _iNbElem, double _dblVal, double* _pdblIn, int _iInc);
ELEMENTARY_FUNCTIONS_IMPEXP void vDsearchC(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo);
ELEMENTARY_FUNCTIONS_IMPEXP void vDsearchD(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo);
ELEMENTARY_FUNCTIONS_IMPEXP double dfloors(double _dblVal);
ELEMENTARY_FUNCTIONS_IMPEXP double dfrexps(double _dblVal, double *_pdblExp);
ELEMENTARY_FUNCTIONS_IMPEXP double dblNearFloat(double _dblVal, double _dblMode);

ELEMENTARY_FUNCTIONS_IMPEXP void vDmProd(int _iMode, double* _pdblIn, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut, int _iInc);
ELEMENTARY_FUNCTIONS_IMPEXP void vWDmProd(int _iMode, double* _pdblIn1, double* _pdblIn2, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut1, double* _pdblOut2, int _iInc);

ELEMENTARY_FUNCTIONS_IMPEXP double durands(int *_iVal);
ELEMENTARY_FUNCTIONS_IMPEXP int nint(double _iVal);

ELEMENTARY_FUNCTIONS_IMPEXP void ddmsums(int _iMode, double *_pdblIn, int _iRows, int _iCols, double *_pdblOut);
ELEMENTARY_FUNCTIONS_IMPEXP void zdmsums(int _iMode, double *_pdblRealIn, double *_pdblImgIn, int _iRows, int _iCols, double *_pdblRealOut, double *_pdblImgOut);

ELEMENTARY_FUNCTIONS_IMPEXP void franck_matrix(int _iSize, double *_pData);
ELEMENTARY_FUNCTIONS_IMPEXP void hilb_matrix(int _iSize, double *_pData);
ELEMENTARY_FUNCTIONS_IMPEXP void magic_matrix(int _iSize, double *_pData);

ELEMENTARY_FUNCTIONS_IMPEXP void deyes(double *_pdblOut, int _iRows, int _iCols);

#endif //__BASIC_FUNCTION__
