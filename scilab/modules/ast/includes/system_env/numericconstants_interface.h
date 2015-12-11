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
