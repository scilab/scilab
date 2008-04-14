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
#include "stack-c.h"
#include <stdlib.h>
#include <math.h>

extern double C2F(dlamch)  __PARAMS((char *CMACH, unsigned long int));
extern double C2F(logp1) _PARAMS((double *x));

/*Constants*/
#define ROW_LETTER		'r'
#define COL_LETTER		'c'
#define STAR_LETTER		'*'
#define MTLB_LETTER		'm'

#define BY_ROWS			1
#define BY_COLS			2
#define BY_ALL			0
#define	BY_MTLB			-1

double lnp1m1(double _dblVar);

double dabss(double _dblVal);
double dabsz(double _dblRealVal, double _dblImgVal);

double dcoss(double _dblVal);
void zcoss(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut);

double dsins(double _dblVal);
void zsins(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut);

double dtans(double _dblVal);
void ztans(double _dblRealIn, double _dblImgIn, double* _dblRealOut, double* _dblImgOut);

double dacoss(double _dblVal);
void wacos(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);

double dasins(double _dblVal);
void wasin(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);

double datans(double _dblVal);
double datan2s(double _dblValX, double _dblValY);
void watan(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);

double dexps(double _dblVal);
void zexps(double _dblRealIn, double _dblImgIn, double *_pdblRealOut, double *_pdblImgOut);

double dcoshs(double _dblVal);
double dsinhs(double _dblVal);

double dsqrts(double _dblVal);
double dlogs(double _dblVal);
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


#endif //__BASIC_FUNCTION__