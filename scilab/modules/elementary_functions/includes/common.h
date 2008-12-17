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

#define getRelativeMachinePrecision()		C2F(dlamch)("e", 1L)
#define getUnderflowThreshold()					C2F(dlamch)("u", 1L)
#define getOverflowThreshold()					C2F(dlamch)("o", 1L)


#define C2F(name) name##_
#define F2C(name) name##_

extern double C2F(dlamch)  (char *CMACH, unsigned long int);
extern double C2F(logp1) (double *x);
extern int C2F(dgemm)();
extern int C2F(dswap)();
extern double C2F(dasum)();

#endif /* __COMMON_H__ */