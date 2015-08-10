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

extern "C"
{
#include "numericconstants_interface.h"
}

#include "numericconstants.hxx"

double nc_eps()
{
    return NumericConstants::eps;
}

double nc_safe_min()
{
    return NumericConstants::safe_min;
}

double nc_base()
{
    return NumericConstants::base;
}

double nc_eps_machine()
{
    return NumericConstants::eps_machine;
}

double nc_num_mantissa_digits()
{
    return NumericConstants::num_mantissa_digits;
}

double nc_rounded()
{
    return NumericConstants::rounded;
}

double nc_exp_min()
{
    return NumericConstants::exp_min;
}

double nc_double_min()
{
    return NumericConstants::double_min;
}

double nc_exp_max()
{
    return NumericConstants::exp_max;
}

double nc_double_max()
{
    return NumericConstants::double_max;
}


//Fortran
double C2F(nc_eps)()
{
    return NumericConstants::eps;
}

double C2F(nc_safe_min)()
{
    return NumericConstants::safe_min;
}

double C2F(nc_base)()
{
    return NumericConstants::base;
}

double C2F(nc_eps_machine)()
{
    return NumericConstants::eps_machine;
}

double C2F(nc_num_mantissa_digits)()
{
    return NumericConstants::num_mantissa_digits;
}

double C2F(nc_rounded)()
{
    return NumericConstants::rounded;
}

double C2F(nc_exp_min)()
{
    return NumericConstants::exp_min;
}

double C2F(nc_double_min)()
{
    return NumericConstants::double_min;
}

double C2F(nc_exp_max)()
{
    return NumericConstants::exp_max;
}

double C2F(nc_double_max)()
{
    return NumericConstants::double_max;
}
