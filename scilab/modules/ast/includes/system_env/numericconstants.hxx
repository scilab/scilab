/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __NUMRICAL_CONSTANTS__
#define __NUMRICAL_CONSTANTS__

#include "dynlib_ast.h"
#include "machine.h"

extern "C" double C2F(dlamch) (const char *_val, unsigned long int);

class NumericConstants
{
public:
    EXTERN_AST static double eps;                   //relative machine precision
    EXTERN_AST static double safe_min;              //safe minimum, such that 1/sfmin does not overflow
    EXTERN_AST static double base;                  //base of the machine
    EXTERN_AST static double eps_machine;           //eps*base
    EXTERN_AST static double num_mantissa_digits;   //number of (base) digits in the mantissa
    EXTERN_AST static double rounded;               //1.0 when rounding occurs in addition, 0.0 otherwise
    EXTERN_AST static double exp_min;               //minimum exponent before (gradual) underflow
    EXTERN_AST static double double_min;                   //underflow threshold - base**(emin-1)
    EXTERN_AST static double exp_max;               //largest exponent before overflow
    EXTERN_AST static double double_max;                   //overflow threshold  - (base**emax)*(1-eps)

    EXTERN_AST static void Initialize();

};
#endif /* !__NUMRICAL_CONSTANTS__ */
