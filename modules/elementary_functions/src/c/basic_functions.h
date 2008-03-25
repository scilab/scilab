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


