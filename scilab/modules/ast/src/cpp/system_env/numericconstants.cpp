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

#include "numericconstants.hxx"

double NumericConstants::eps;
double NumericConstants::safe_min;
double NumericConstants::base;
double NumericConstants::eps_machine;
double NumericConstants::num_mantissa_digits;
double NumericConstants::rounded;
double NumericConstants::exp_min;
double NumericConstants::double_min;
double NumericConstants::exp_max;
double NumericConstants::double_max;

void NumericConstants::Initialize()
{
    eps = C2F(dlamch)("E", 1L);
    safe_min = C2F(dlamch)("S", 1L);
    base = C2F(dlamch)("B", 1L);
    eps_machine = C2F(dlamch)("P", 1L);
    num_mantissa_digits = C2F(dlamch)("N", 1L);
    rounded = C2F(dlamch)("R", 1L);
    exp_min = C2F(dlamch)("M", 1L);
    double_min = C2F(dlamch)("U", 1L);
    exp_max = C2F(dlamch)("L", 1L);
    double_max = C2F(dlamch)("O", 1L);
}
