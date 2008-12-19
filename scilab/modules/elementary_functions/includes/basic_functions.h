/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __BASIC_FUNCTION__
#define __BASIC_FUNCTION__

#include <stdlib.h>
#include <math.h>

#include "stack-c.h"
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


static double spdblExpmC[41] = {0};
static double sdblExpmN = 0;


double dceilsEx(double _dblVal, int _iPrecision);
double dceils(double _dblVal);
double wasums(int _iNbElem, double* _pdblReal, double* _pdblImg);

void vCupro(int _iNbElem, double* _piIn, double* _piOut);
void vCuproi(int _iNbElem, double* _piRealIn, double* _piImgIn, double* _piRealOut, double* _piImgOut);
void vDvmul(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut);
void vWvmul(int _iNbElem, double* _piRealIn1, double* _piImgIn1, double* _piRealIn2, double* _piImgIn2, int _iIncIn1, int _iIncIn2, double* _piRealOut, double* _piImgOut);
void vCusum(int _iNbElem, double *_dblIn, double *_dblOut);
void vDadd(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut);
void vDless(int _iNbElem, double* _pdblIn1, double* _pdblIn2, int _iIncIn1, int _iIncIn2, double* _pdblOut);
void vDset(int _iNbElem, double _dblVal, double* _pdblIn, int _iInc);
void vDsearchC(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo);
void vDsearchD(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo);
double dfloors(double _dblVal);
double dfrexps(double _dblVal, double *_pdblExp);
double dblNearFloat(double _dblVal, double _dblMode);

void vDmProd(int _iMode, double* _pdblIn, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut, int _iInc);
void vWDmProd(int _iMode, double* _pdblIn1, double* _pdblIn2, int _iLeadDim, int _iRows, int _iCols, double* _pdblOut1, double* _pdblOut2, int _iInc);

double durands(int *_iVal);
int nint(double _iVal);
double danints(double _dblVal);

void ddmsums(int _iMode, double *_pdblIn, int _iRows, int _iCols, double *_pdblOut);
void zdmsums(int _iMode, double *_pdblRealIn, double *_pdblImgIn, int _iRows, int _iCols, double *_pdblRealOut, double *_pdblImgOut);

void franck_matrix(int _iSize, double *_pData);
void hilb_matrix(int _iSize, double *_pData);
void magic_matrix(int _iSize, double *_pData);

void deyes(double *_pdblOut, int _iRows, int _iCols);

#endif //__BASIC_FUNCTION__
