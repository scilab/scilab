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

#include "dynlib_ast.h"
#include "machine.h"

EXTERN_AST double nc_eps();
EXTERN_AST double nc_safe_min();
EXTERN_AST double nc_base();
EXTERN_AST double nc_eps_machine();
EXTERN_AST double nc_num_mantissa_digits();
EXTERN_AST double nc_rounded();
EXTERN_AST double nc_exp_min();
EXTERN_AST double nc_double_min();
EXTERN_AST double nc_exp_max();
EXTERN_AST double nc_double_max();

//Fortran
EXTERN_AST double C2F(nc_eps)();
EXTERN_AST double C2F(nc_safe_min)();
EXTERN_AST double C2F(nc_base)();
EXTERN_AST double C2F(nc_eps_machine)();
EXTERN_AST double C2F(nc_num_mantissa_digits)();
EXTERN_AST double C2F(nc_rounded)();
EXTERN_AST double C2F(nc_exp_min)();
EXTERN_AST double C2F(nc_double_min)();
EXTERN_AST double C2F(nc_exp_max)();
EXTERN_AST double C2F(nc_double_max)();
