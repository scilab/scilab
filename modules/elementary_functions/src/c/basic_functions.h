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
#include "stack-c.h"
#include <stdlib.h>
#include <math.h>

extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));
extern double C2F(logp1) _PARAMS((double *x));

#ifndef _CST_BASIC__
#define _CST_BASIC__
/*Constants*/
#define ROW_LETTER		'r'
#define COL_LETTER		'c'
#define STAR_LETTER		'*'
#define MTLB_LETTER		'm'

#define BY_ROWS			1
#define BY_COLS			2
#define BY_ALL			0
#define	BY_MTLB			-1
#endif //_CST_BASIC__

double lnp1m1(double _dblVar);
double dabss(double _dblVal);
double dabsz(double _dblRealVal, double _dblImgVal);
double dcoss(double _dblVal);
double dsins(double _dblVal);
double dtans(double _dblVal);
double dcoshs(double _dblVal);
double dsinhs(double _dblVal);
double dacoss(double _dblVal);
double dasins(double _dblVal);
double datans(double _dblVal);
double datan2s(double _dblValX, double _dblValY);
double dsqrts(double _dblVal);
double dlogs(double _dblVal);
void wacos(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);
void wasin(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);
void watan(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);
double dsigns(double _dblRef, double _dblVal);
double dceilsEx(double _dblVal, int _iPrecision);
int dceils(double _dblVal);
double wasums(int _iNbElem, double* _pdblReal, double* _pdblImg);
double dasums(int _iNbElem, double* _pdblReal);
void ddscals(double* _pdblIn, int _iNbElem, double _dblMulti, double* _pdblOut);
void vCupro(int _iNbElem, double* _piIn, double* _piOut);
void vCuproi(int _iNbElem, double* _piRealIn, double* _piImgIn, double* _piRealOut, double* _piImgOut);
void vDvmul(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut);
void vWvmul(int _iNbElem, double* _piRealIn1, double* _piImgIn1, double* _piRealIn2, double* _piImgIn2, int _iIncIn1, int _iIncIn2, double* _piRealOut, double* _piImgOut);
void vCusum(int _iNbElem, double *_dblIn, double *_dblOut);
void vDadd(int _iNbElem, double* _piIn1, double* _piIn2, int _iIncIn1, int _iIncIn2, double* _piOut);
void vDset(int _iNbElem, double _dblVal, double* _pdblIn, int _iInc);
void vDsearchC(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo);
void vDsearchD(double *_pdblX, int _iNbElemX, double *_pdblVal, int _iNbElemVal, double* _pdblInd, double *_pdblOcc, double *_pdblInfo);
