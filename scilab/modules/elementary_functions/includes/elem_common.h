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

#ifdef __cplusplus
#define getRelativeMachinePrecision()		C2F(dlamch)(const_cast<char*>("e"), 1L)
#define isRealZero(x)						(fabs(static_cast<double>(x)) <= getRelativeMachinePrecision())
#define ZeroIsZero(x)						(fabs(static_cast<double>(x)) <= getRelativeMachinePrecision() ? 0 : static_cast<double>(x))
#else
#define getRelativeMachinePrecision()		C2F(dlamch)((char*)"e", 1L)
#define isZero(x)							(fabs((double)x) <= getRelativeMachinePrecision())
#define ZeroIsZero(x)						(fabs((double)x) <= getRelativeMachinePrecision() ? 0 : (double)x)
#endif

#define getUnderflowThreshold()					C2F(dlamch)((char*)"u", 1L)
#define getOverflowThreshold()					C2F(dlamch)((char*)"o", 1L)
#define isEqual(x,y)										fabs((double)x - (double)y) <= getRelativeMachinePrecision()

extern double C2F(dlamch) (char *_pszCommand, unsigned long int);
extern double C2F(logp1) (double *_pdblVal);
extern int C2F(dgemm) (char *_pstTransA, char *_pstTransB, int *_piN, int *_piM, int *_piK, double *_pdblAlpha, double *_pdblA, int *_piLdA,
                       double *_pdblB, int *_piLdB, double *_pdblBeta, double *_pdblC, int *_piLdC);
extern int C2F(dswap) (int *_piSize, double *_pdblX, int *_piIncX, double *_pdblY, int *_piIncY);
extern double C2F(dasum) (int *_iSize, double *_pdbl, int *_iInc);
extern int C2F(dcopy) (int *_iSize, double *_pdblSrc, int *_piIncSrc, double *_pdblDest, int *_piDest);
extern int C2F(dscal) (int *_iSize, double *_pdblVal, double *_pdblDest, int *_iInc);
extern int C2F(zscal) (int *_iSize, doublecomplex * _pdblVal, doublecomplex * _pdblDest, int *_iInc);
extern int C2F(dset) (int *_iSize, double *_pdblVal, double *_pdblDest, int *_iInc);
extern double C2F(dlange) (char *_pstNorm, int *_piM, int *_piN, double *_pdblA, int *_piLdA, double *_pdblWork);
extern int C2F(dlacpy) (char *_pstUplo, int *piM, int *_piN, double *_pdblA, int *_piLdA, double *_pdblB, int *_piLdB);
extern int C2F(dgetrf) (int *_piM, int *_piN, double *_pdblA, int *_piLdA, int *_piIPIV, int *_piINFO);
extern int C2F(dgecon) (char *_pstNORM, int *_piN, double *_pdblA, int *_piLDA, double *_pdblANORM, double *_pdblRCOND, double *_pdblWORK,
                        int *_piIWORK, int *_piINFO);
extern int C2F(dgetrs) (char *_pstTRANS, int *_piN, int *_piNRHS, double *_pdblA, int *_piLDA, int *_piIPIV, double *_pdblB, int *_piLDB,
                        int *_piINFO);
extern int C2F(dgelsy1) (int *_piM, int *_piN, int *_piNRHS, double *_pdblA, int *_piLDA, double *_pdblB, int *_piLDB, int *_piJPVT,
                         double *_pdblRCOND, int *_piRANK, double *_pdblWORK, int *_piLWORK, int *_piINFO);
extern double C2F(zlange) (char *_pstNORM, int *_piM, int *_piN, double *_pdblA, int *_piLDA, double *_pdblWORK);
extern int C2F(zlacpy) (char *_pstUPLO, int *_piM, int *_piN, double *_pdblA, int *_piLDA, double *_pdblB, int *_piLDB);
extern int C2F(zgetrf) (int *_piM, int *_piN, double *_pdblA, int *_piLDA, int *_piIPIV, int *_piINFO);
extern void C2F(zgecon) (char *_pstNORM, int *_piN, doublecomplex * _pdblA, int *_piLDA, double *_pdblANORM, double *_pdblRNORM,
                         doublecomplex * _pdblWORK, double *_pdblRWORD, int *_piINFO);
extern int C2F(zgetrs) (char *_pstTRANS, int *_piN, int *_piNRHS, double *_pdblA, int *_piLDA, int *_piIPIV, double *_pdblB, int *_piLDB,
                        int *_piINFO);
extern int C2F(zgelsy1) (int *_piM, int *_piN, int *_piNRHS, doublecomplex * pdblA, int *_piLDA, doublecomplex * _pdblB, int *_piLDB, int *_piJPVT,
                         double *_pdblRCOND, int *_piRANK, doublecomplex * _pdblWORK, int *_piLWORK, double *_pdblRWORK, int *_piINFO);
extern double C2F(ddot) (int *_ipSize, double *_pdblVal1, int *_piInc1, double *_pdblVal2, int *_piInc2);
extern void C2F(wmul) (double *ar, double *ai, double *br, double *bi, double *cr, double *ci);
extern void C2F(wdiv) (double *ar, double *ai, double *br, double *bi, double *cr, double *ci);
extern void C2F(dad) (double *a, int *na, int *i1, int *i2, int *j1, int *j2, double *r, int *isw);
extern int C2F(entier) (int *_iSize, double *_pdbl, int *_pi);
extern int C2F(simple) (int *_iSize, double *_pdbl, float *_pf);
extern double C2F(nearfloat) (double*, double*);

#endif /* __COMMON_H__ */
