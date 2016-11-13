/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
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
