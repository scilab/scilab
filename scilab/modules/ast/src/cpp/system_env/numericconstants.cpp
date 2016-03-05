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
