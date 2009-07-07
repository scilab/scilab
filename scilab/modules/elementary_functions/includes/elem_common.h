/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __COMMON_H__
#define __COMMON_H__

#include "core_math.h"

#include "abs.h"
#include "cos.h"
#include "exp.h"
#include "log.h"
#include "pythag.h"
#include "sin.h"
#include "tan.h"
#include "sqrt.h"
#include "sign.h"

/*
'E' or 'e',   DLAMCH := eps ( relative machine precision )
'S' or 's ,   DLAMCH := sfmin ( safe minimum, such that 1/sfmin does not overflow )
'B' or 'b',   DLAMCH := base ( base of the machine )
'P' or 'p',   DLAMCH := eps*base ( eps*base )
'N' or 'n',   DLAMCH := t ( number of (base) digits in the mantissa )
'R' or 'r',   DLAMCH := rnd ( 1.0 when rounding occurs in addition, 0.0 otherwis )
'M' or 'm',   DLAMCH := emin ( minimum exponent before (gradual) underflow )
'U' or 'u',   DLAMCH := rmin ( underflow threshold - base**(emin-1) )
'L' or 'l',   DLAMCH := emax ( largest exponent before overflow )
'O' or 'o',   DLAMCH := rmax ( overflow threshold  - (base**emax)*(1-eps) )
*/

#include <stdlib.h>
#include "machine.h"

#include "doublecomplex.h"

#define getRelativeMachinePrecision()		C2F(dlamch)((char*)"e", 1L)
#define getUnderflowThreshold()					C2F(dlamch)((char*)"u", 1L)
#define getOverflowThreshold()					C2F(dlamch)((char*)"o", 1L)
#define isZero(x)												fabs((double)x) <= getRelativeMachinePrecision()
#define isEqual(x,y)										fabs((double)x - (double)y) <= getRelativeMachinePrecision()
#define ZeroIsZero(x)										fabs((double)x) <= getRelativeMachinePrecision() ? 0 : (double)x

extern double C2F(dlamch)  (char *_pszCommand, unsigned long int);
extern double C2F(logp1) (double *_pdblVal);
extern int C2F(dgemm)();
extern int C2F(dswap)();
extern double C2F(dasum)();
extern int C2F(dcopy)(int *_iSize, double *_pdblSrc, int* _piIncSrc, double *_pdblDest, int *_piDest);
extern int C2F(dscal)(int *_iSize, double *_pdblVal, double *_pdblDest, int *_iInc);
extern int C2F(zscal)(int *_iSize, doublecomplex *_pdblVal, doublecomplex *_pdblDest, int *_iInc);
extern int C2F(dset)(int *_iSize, double *_pdblVal, double *_pdblDest, int *_iInc);
extern double C2F(dlange)();
extern int C2F(dlacpy)();
extern int C2F(dgetrf)();
extern int C2F(dgecon)();
extern int C2F(dgetrs)();
extern int C2F(dgelsy1)();
extern double C2F(zlange)();
extern int C2F(zlacpy)();
extern int C2F(zgetrf)();
extern int C2F(zgecon)();
extern int C2F(zgetrs)();
extern int C2F(zgelsy1)();
extern double C2F(ddot)(int *_ipSize, double *_pdblVal1, int *_piInc1, double *_pdblVal2, int *_piInc2);

#endif /* __COMMON_H__ */
